#include "player.h"
#include <iostream>
#include <QDebug>
#include <vector>
#include "bullet.h"
#include <iostream>
#include <QGraphicsScene>
#include "level1.h"
#include "termcolor.hpp"
Player::Player(QPixmap *pixmap/*, QObject *parent*/ )
    : /*QObject{parent}, */color(Qt::green), m_pixmap(pixmap)
{
    pos = {100.f,100.f};
    vel = {0.f,0.f};
    acc = {0.f,0.f};


    is_moving_left  = false;
    is_moving_right = false;
    is_moving_up    = false;
    is_moving_down  = false;

    if(m_pixmap == nullptr)
        std::cout << "Player::Player pixmap == nullptr" << "\n";
    *m_pixmap = m_pixmap->scaled(57,100);

    this->setTransformationMode(Qt::SmoothTransformation);
    //    this->setOffset(this->x() + 50 , this->y()+ 50);

    rect = pixmap->rect();
    //rect.setSize(QSize(100,100));

    this->setTransformOriginPoint(this->boundingRect().width()/2, this->boundingRect().height()/2  );
}

QRectF Player::boundingRect() const
{
    return rect;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //        std::cout << "paint" << "\n";
    //        painter->setCompositionMode(QPainter::CompositionMode_Plus);
    painter->drawPixmap( 0,0, *m_pixmap);
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
    //painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    //pix.setCompositionMode(QPainter::CompositionMode_Plus);
    //     painter->fillRect(m_pixmap->rect(), QColor(110, 110, 110,  140)); // colorize the light in

    //    m_painter = painter;
    //    m_painter->setBrush(color);
    //    m_painter->drawRect(rect);
}

QPainterPath Player::shape() const
{
    //    std::cout << "QPainterPath Player::shape() " << "\n";
    QPainterPath path;
    path.addRect(m_pixmap->rect());

    return path;
}

void Player::applyForce(QVector2D force)
{
    acc = force;
}



void Player::advance(int step)
{

    //     std::cout << "advance(int step)" << "\n";
    //    if (!step) return;

    //    if(checkCollisions()==true){   ///we dont want to move the scene if there is a collision
    //        std::cout << "checkCollisions()==true" << "\n";
    //        return;
    //    }


    //if(std::abs(vel.x()) <1 || std::abs(vel.y()) <1) speed=speed/2;


    d  = Level1::getDelta();
    addToAcceleration( );
    vel += acc;
    pos += vel;

    this->setPos(pos.x(), pos.y());
    acc = {0,0};

    //limit velocity if its too large
    speed = 240;
    if(vel.x()>speed *d) vel.setX(speed *d);
    else if(vel.x()<-speed *d) vel.setX(-speed * d);
    if(vel.y()>speed *d) vel.setY(speed *d);
    else if(vel.y()<-speed *d) vel.setY(-speed *d);

    if(std::abs(vel.y()) < 0.01) vel.setY(0);
    if(std::abs(vel.x()) < 0.01) vel.setX(0);
    //    rotateAccordingToDirection();

    checkCollisions(); //check which items this player ocllides

}

void Player::keyReleaseEvent(QKeyEvent *event)
{
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
}

//float Player::map2Ranges(float value, float minVel, float maxVel, float minSpeed, float maxSpeed)
//{
//    return minSpeed + (maxSpeed - minSpeed) * ((value - minVel)/(maxVel - minVel)); //return speed
//}

void Player::keyPressEvent(QKeyEvent *event)
{
    //    if(std::abs(vel.x()) <2) speed=speed/2;
    //    if(std::abs(vel.y()) <2) speed=speed/2;

    //     speed = 400.f;
    //    if(std::abs(vel.x()) < speed*d/15 || std::abs(vel.y()) < speed*d/15) speed*=0.5;

    //every click changes
    switch(event->key())  {
    case Qt::Key_Left :  //acc.setX(-2);
        is_moving_left = true;
        moveX = -speed *d;
        moveY =0;
        desiredAcc = {-speed *d, 0};
        frameCounter=0;
        reduceVelY();          ///to reduce vel.y when left or right key is being pressed
        addToAcceleration();
        break;
    case Qt::Key_Right :  //acc.setX(2);
        is_moving_right = true;
        moveX = speed *d;
        moveY =0;
        desiredAcc = {speed *d, 0};
        frameCounter=0;
        reduceVelY();           ///to reduce vel.y when left or right key is being pressed
        addToAcceleration();
        break;
    case Qt::Key_Up :    //acc.setY(-2);
        is_moving_up = true;
        moveY = -speed *d;
        moveX =0;
        desiredAcc = {0, speed *d};
        frameCounter=0;
        reduceVelX();         ///to reduce vel.x when up or down key is being pressed
        addToAcceleration();
        break;
    case Qt::Key_Down :      //acc.setY(2);
        is_moving_down = true;
        moveY = speed *d;
        moveX =0;
        desiredAcc = {0, speed *d};
        frameCounter=0;
        reduceVelX();          ///to reduce vel.x when up or down key is being pressed
        addToAcceleration();
        break;
    }


}



void Player::addToAcceleration() //this method is called every frame
{
    int delayTurnInFrames = 300;
    s = 0.01;
    if(moveX==0 && moveY ==0) /// moveX and moveY are set to 0 when
        return;
    frameCounter++;
    //    std::cout << "frameCounter: " << frameCounter << " " << "\n";
    if(frameCounter> delayTurnInFrames){
        frameCounter=0;
        moveX =0;
        moveY =0;
        desiredAcc = {0, 0};
    }
    //    std::cout << "s: " << s << " moveX*0.1: " << moveX*s << " moveY*0.1: "<< moveY*s << "\n";
    if(acc.x()< std::abs(desiredAcc.x()))
        acc.setX(moveX*s);
    if(acc.y() < std::abs(desiredAcc.y()))
        acc.setY(moveY*s);


    vel += acc;
//     if(acc.x()<0.1) acc.setX(0);
//    if(acc.y()<0.1) acc.setY(0);
    rotateAccordingToDirection(); // this bases on vel
}

void Player::reduceVelX() ///to reduce vel.x when up or down key is being pressed
{return;
    if(vel.x()>0){     //if
        if(vel.x() < 0.1)
            acc.setX(0); //-vel.x()
        else
            acc.setX(-0.1);
    }
    else if(vel.x()<0){
        if(vel.x() > 0.1)
            acc.setX(vel.x()); //vel.x()
        else
            acc.setX(0.1);
    }

    vel += acc;
    rotateAccordingToDirection();
}

void Player::reduceVelY()  //to reduce vel.y when left or right key is being pressed
{
    return;
    if(vel.y()>0){
        if(vel.y() < 0.1)
            acc.setY(0); //-vel.y()
        else
            acc.setY(-0.1);
    }
    else if(vel.y()<0){
        if(vel.y() > 0.1)
            acc.setY(0); //vel.y()
        else
            acc.setY(0.1);
    }

    vel += acc;
    rotateAccordingToDirection();
}

float prevRotation;






double Player::normalize_angle( const double value ) {
    double start = -180.0;
    double end = 180.0;
    double width = end - start   ;
    double offsetValue = value - start ;
    return ( offsetValue - ( floor( offsetValue / width ) * width ) ) + start ;
}



bool go_on= true;


bool Player::slowRotationDown()
{

    float a =  std::atan2(vel.y(), vel.x())  *180/M_PI; //to degrees
    a = normalize_angle(a);
    prevRotation = normalize_angle(prevRotation);
    std::cout << "slowRotationDown(): " << normalize_angle(a - prevRotation)  << "\n";

    float angleDiff = normalize_angle(a  - prevRotation);



//   if(std::abs(angleDiff) <5) angleDiff =0;

//    if(angleDiff > 0 && angleDiff <=  180 )
//        a = prevRotation+5.f;
//    else if(angleDiff < 0 && angleDiff  >= -180 )
//        a = prevRotation-5.f;



    if(angleDiff>=0 && angleDiff<5 )        a = prevRotation + angleDiff;
else if(angleDiff>=5 && angleDiff<=180 )      a = prevRotation + 5.f;
else if(angleDiff<0 && angleDiff>-5 )           a = prevRotation + angleDiff;
else if(angleDiff<=-5 && angleDiff>=-180)         a = prevRotation - 5.f;

if(angleDiff>180. || angleDiff<-180.) qDebug() << "ERROR! nieprzeidziany zakres kata..." ;

// std::cout << termcolor::red << "a: " << a << termcolor::reset<< "\n";

// std::cout << termcolor::green << "vel.x: " << vel.x() << termcolor::reset<< "\n";
//  std::cout << termcolor::green << "vel.y: " << vel.y() << termcolor::reset<< "\n";
//  std::cout << "------------------------" << "\n";
 this->setRotation(normalize_angle(a-270));
    prevRotation =a;
    return true;
}


void Player::rotateAccordingToDirection()
{

//    float rotAngle1 =  std::atan2(vel.y(), vel.x())  *180/M_PI; //to degrees
//    rotAngle1 = normalize_angle(rotAngle1);


//    if(normalize_angle(rotAngle1 - prevRotation) !=0){
        slowRotationDown();
//        return;
//    }
//     std::cout << "cccc " << rotAngle1 << "\n";
//    prevRotation = rotAngle1;

//    this->setRotation(normalize_angle(rotAngle1-270));
//    return;
}



void Player::checkCollisions()
{
    QList<QGraphicsItem*> itemsThisCollides = this->collidingItems(); ///get items in collision | Returns a list of all items that collide with this item.

    for (int i = 0; i < itemsThisCollides.size(); ++i) {
        if(dynamic_cast<Bullet*>(itemsThisCollides[i])){ //if the colliding item is of type Bullet*
            std::cout << "Deleting bullet that hit the player" << "\n";
            scene()->removeItem(itemsThisCollides[i]);

            auto v = Level1::getBulletContainer();

            ///            v->erase(v->begin()+i) - this wont work, cause this index is different than bullets index!
            v->erase(std::remove(v->begin(), v->end(), itemsThisCollides[i]), v->end()); //remove the bullet from bullet container
            delete itemsThisCollides[i];

        }
    }
}





bool Player::contains(const QPointF &point) const
{
    std::cout << "Player::contains(const QPointF &point) const " << "\n";
}

bool Player::isObscuredBy(const QGraphicsItem *item) const
{
    std::cout << "Player::isObscuredBy(const QGraphicsItem *item) const " << "\n";
}

QPainterPath Player::opaqueArea() const
{
    std::cout << "QPainterPath Player::opaqueArea() const " << "\n";
}

int Player::type() const
{
    std::cout << "int Player::type() const " << "\n";
}

//void Player::update(const QRectF &rect)
//{
//    //    std::cout << "update" << "\n";
//}
