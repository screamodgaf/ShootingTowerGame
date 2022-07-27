#include "player.h"
#include <iostream>
#include <QDebug>
#include <vector>
#include "bullet.h"
#include <iostream>
#include <QGraphicsScene>
#include "level1.h"
#include "termcolor.hpp"


Player::Player(Level1 *scene, std::vector<Bullet *> *v_bullets): m_scene{scene},
    m_v_bullets{v_bullets}, color{Qt::green}
{
    qDebug() << "Player ctor";

    loc = {100.f ,300.f };
    vel = {0.f ,0.f };
    acc = {0.f ,0.f };
    this->setPos(loc.x(), loc.y());

    is_moving_left  = false;
    is_moving_right = false;
    is_moving_up    = false;
    is_moving_down  = false;

    prevAngle = rotation();

    playerImage = loadResources.getPlayerImage();

    if(playerImage == nullptr)
        std::cout << "Player::Player image == nullptr" << "\n";
    qDebug() << playerImage;
    *playerImage = playerImage->convertToFormat(QImage::Format_ARGB32_Premultiplied/* ,Qt::ColorOnly*/);
    *playerImage = playerImage->scaled(56,110);
    this->setTransformationMode(Qt::SmoothTransformation);

    rect = playerImage->rect();
    this->setTransformOriginPoint(this->boundingRect().center());
    desiredBulletPos = {0.f + this->boundingRect().width()/2,  0.f };

    playerWeapons = new PlayerWeapons(m_scene, desiredBulletPos, m_v_bullets, this, this);
    playerDefences = new PlayerDefences(m_scene, this, m_v_bullets);
}

QRectF Player::boundingRect() const
{
    return rect;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHints(QPainter::VerticalSubpixelPositioning, true);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::VerticalSubpixelPositioning, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->drawImage(0,0, *playerImage);
}

QPainterPath Player::shape() const
{
    QPainterPath path;
    path.addRect(playerImage->rect());
    return path;
}

void Player::applyForce(QVector2D force)
{
    acc = force;
}

void Player::advance(int step)
{
    d = m_scene->getDelta();
    float speed = 210.f *d;
    spreadAccOverManyFrames();
    //    speed =0.2;
    vel += acc;
    loc += vel;

    this->setPos(loc.x(), loc.y());
    acc = {0.f,0.f};

    ///limit velocity if its too large:

    if(vel.x()>speed) vel.setX(speed);
    else if(vel.x()<-speed) vel.setX(-speed);
    if(vel.y()>speed) vel.setY(speed);
    else if(vel.y()<-speed) vel.setY(-speed);

    //        if(std::abs(vel.y()) < 0.01) vel.setY(0);
    //        if(std::abs(vel.x()) < 0.01) vel.setX(0);

    playerDefences->checkCollisions(this); //check which items this player ocllides
    playerWeapons->advance(1);
    rotateAccordingToDirection();
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    desiredAcc = {0.f,0.f};
    switch(event->key())  {
    case Qt::Key_Left :
        is_moving_left = false;
        break;
    case Qt::Key_Right :
        is_moving_right = false;
        break;
    case Qt::Key_Up :
        is_moving_up = false;
        break;
    case Qt::Key_Down :
        is_moving_down = false;
        break;
    }
    update();
}

QVector2D Player::getVel() const
{
    return vel;
}

void Player::keyPressEvent(QKeyEvent *event)
{
    d = m_scene->getDelta();
    float speedX = 320.f*d;
    float speedY = 320.f*d;

    ///every click changes
    switch(event->key())  {
    case Qt::Key_Left :  //acc.setX(-2);
        is_moving_left = true;
        moveX = -speedX;
        moveY =0.f;
        desiredAcc = {-speedX, 0.f};
        reduceVelY();          ///to reduce vel.y when left or right key is being pressed
        //        spreadAccOverManyFrames();
        break;
    case Qt::Key_Right :  //acc.setX(2);
        is_moving_right = true;
        moveX = speedX;
        moveY =0.f;
        desiredAcc = {speedX, 0.f};
        reduceVelY();           ///to reduce vel.y when left or right key is being pressed
        //        spreadAccOverManyFrames();
        break;
    case Qt::Key_Up :    //acc.setY(-2);
        is_moving_up = true;
        moveY = -speedY;
        moveX =0.f;
        desiredAcc = {0.f, speedY};
        reduceVelX();         ///to reduce vel.x when up or down key is being pressed
        //        spreadAccOverManyFrames();
        break;
    case Qt::Key_Down :      //acc.setY(2);
        is_moving_down = true;
        moveY = speedY;
        moveX =0.f;
        desiredAcc = {0.f, speedY};
        reduceVelX();          ///to reduce vel.x when up or down key is being pressed
        //        spreadAccOverManyFrames();
        break;
    }
}

void Player::spreadAccOverManyFrames() //this method is called every frame
{
    d = m_scene->getDelta();
    if(std::abs(acc.x())<15.f*d) acc.setX(0.f);
    if(std::abs(acc.y())<15.f*d) acc.setY(0.f);
    vel += acc;

    QVector2D desiredVel = vel +  desiredAcc;
    s = 0.9f*d;

    if(std::fabs(vel.x() - desiredVel.x())<0.0001 &&
            std::fabs(vel.y() - desiredVel.y())<0.0001){
        moveX =0.f;
        moveY =0.f;
        desiredAcc = {0.f, 0.f};
        return;
    }

    if(std::fabs(vel.x() - desiredVel.x())>0.0001)
        vel.setX(vel.x()+ moveX*s);
    if(std::fabs(vel.y() - desiredVel.y())>0.0001)
        vel.setY(vel.y()+ moveY*s);

    rotateAccordingToDirection(); // this bases on vel
}




void Player::reduceVelX() ///to reduce vel.x when up or down key is being pressed
{/*return;*/
    d = m_scene->getDelta();

    float s = 1.f*d;
    if(vel.x()>0){     //if
        if(vel.x() < s)
            acc.setX(0 ); //-vel.x()
        else
            acc.setX(-s);
    }
    else if(vel.x()<0 ){
        if(vel.x() > s)
            acc.setX(vel.x()); //vel.x()
        else
            acc.setX(s);
    }

    vel += acc;
    rotateAccordingToDirection();
}

void Player::reduceVelY()  //to reduce vel.y when left or right key is being pressed
{
    //            return;
    d = m_scene->getDelta();
    float s = 1.f*d;

    if(vel.y()>0){
        if(vel.y() < s)
            acc.setY(0.f); //-vel.y()
        else
            acc.setY(-s);
    }
    else if(vel.y()<0){
        if(vel.y() > s)
            acc.setY(0.f); //vel.y()
        else
            acc.setY(s);
    }

    vel += acc;
    rotateAccordingToDirection();
}

float Player::normalize_angle(const float value ) {
    float start = -180.f;
    float end = 180.f;
    float width = end - start   ;
    float offsetValue = value - start ;
    return ( offsetValue - ( floor( offsetValue / width ) * width ) ) + start ;
}

void Player::rotateAccordingToDirection()
{
    //the method sets rotation and slows down rotation when the if conditions are matched:
    float a =  std::atan2(vel.y(), vel.x())  *180.f/M_PI; //to degrees
    a = normalize_angle(a);
    prevAngle = normalize_angle(prevAngle);
    //    std::cout << "a - prevAngle: " << normalize_angle(a - prevAngle)  << "\n";

    float angleDiff = normalize_angle(a  - prevAngle);

    float angleChange =  50.f*d; //by frame

    if(angleDiff>=0 && angleDiff<angleChange )        a = prevAngle + angleDiff;
    else if(angleDiff>=angleChange && angleDiff<=180 )      a = prevAngle + angleChange  ;
    else if(angleDiff<0 && angleDiff>-angleChange )           a = prevAngle + angleDiff;
    else if(angleDiff<=-angleChange && angleDiff>=-180)         a = prevAngle - angleChange  ;

    if(angleDiff>180.f || angleDiff<-180.f) qDebug() << "ERROR! nieprzeidziany zakres kata..." ;

    this->setRotation(normalize_angle(a-270.f));
    prevAngle =a;
}



bool Player::contains(const QPointF &point) const
{
    std::cout << "Player::contains(const QPointF &point) const " << "\n";
    return false;
}

bool Player::isObscuredBy(const QGraphicsItem *item) const
{
    std::cout << "Player::isObscuredBy(const QGraphicsItem *item) const " << "\n";
    return false;
}

QPainterPath Player::opaqueArea() const
{
    std::cout << "QPainterPath Player::opaqueArea() const " << "\n";
    return  QPainterPath();
}

int Player::type() const
{
    std::cout << "int Player::type() const " << "\n";
    return 0;
}

Player::~Player()
{
    std::cout << "~Player()" << "\n";
    delete playerDefences;
    playerDefences = nullptr;
}

PlayerWeapons *Player::getPlayerWeapons()
{
    return playerWeapons;
}
