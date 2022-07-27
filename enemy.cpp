#include "enemy.h"
#include "level1.h"
#include <iostream>
#include "game.h"
#include "termcolor.hpp"
#include "loadresources.h"
#include "playerweapons.h"
#include "player.h"
Enemy::Enemy(Level1* scene, QPixmap pixmap, std::vector<Bullet*>* v_bullets, std::vector<ParticleSystem*>* v_particleSystem, Player* player, QGraphicsPixmapItem *parent)  : QGraphicsPixmapItem{parent}, m_scene{scene}, m_v_bullets{v_bullets}, m_v_particleSystem{v_particleSystem}, m_pixmap{pixmap}, m_player{player}
{
    std::cout << "Enemy() ctor: " << this << "\n";
    rect = boundingRect();
    setPos( 700- rect.width()/2, 300  - rect.height()/2) ;
    this->setTransformOriginPoint(this->boundingRect().center());
    loc = { static_cast<float>(pos().x()) ,static_cast<float>(pos().y()) };
    vel = { 0.f ,0.f };
    acc = { 0.f ,0.f };

    ///setting area within which the Enemy shoots bullets:
    attackArea = new QGraphicsEllipseItem(this);
    std::cout << "attackArea: " << attackArea << "\n";
    float radius = 500;
    //    attackArea->setRect(-radius/2 + rect.width()/2,
    //                        -radius/2 + rect.width()/2, 400,400);
    //    attackArea->setPos(this->boundingRect().width()/2, this->boundingRect().height()/2); ///without ships coordinates thus width not pos()+ width
    std::cout << termcolor::red << "Enemy boundingRect().width: " << this->rect.width() << " boundingRect().height(): " << this->rect.height()
              << termcolor::reset<< "\n";


    attackArea->setTransformOriginPoint(attackArea->boundingRect().center());
    //    attackArea->setRect(-radius/2 + this->rect.width()/4 ,
    //                        -radius/2 + this->rect.height()/4 , radius , radius);
    attackArea->setRect(-radius/2  + m_pixmap.width() /4,
                        -radius/2  + m_pixmap.height()/4, radius, radius);
    playerInCollision = nullptr;

    ///set pixmaps:
    firePixmap = loadResources.getFire1(); //Pixmap
    *firePixmap = firePixmap->scaled(QSize(200,200 ));

    nebulaPixmap = loadResources.getBonusNebula1Pixmap(); //Pixmap
    *nebulaPixmap = nebulaPixmap->scaled(QSize(200,200 ));

    m_pixmap = m_pixmap.scaled(QSize(100,200));
    this->setPixmap(m_pixmap);



    //Bullet managment element:
    //periodic check if enemy within Enemy so it shoots every second and not every every frame:
    EnemyAreaTimer = new QTimer;
    //    QTimer::connect(EnemyAreaTimer, &QTimer::timeout, this,
    //            &Enemy::checkShootingAreaPeriodicly);
    QTimer::connect(EnemyAreaTimer, &QTimer::timeout, [this](){ checkShootingAreaPeriodicly();});
    EnemyAreaTimer->start(700);


    //SET LIFE BAR:
    life = 100;
    lifeBar = new QProgressBar();
    lifeBar->setValue(100);
    lifeBar->setRange(0,100);
    lifeBar->resize(200, 17);
    lifeBar->setAlignment(Qt::AlignCenter);
    lifeBarProxy = m_scene->addWidget(lifeBar);
    lifeBarProxy->setPos(this->x() - lifeBar->width()/2 + this->boundingRect().width()/2 , this->y()-50);
    lifeBarProxy->show();
    m_scene->addItem(this);

    prevAngle = rotation();
}

QRectF Enemy::boundingRect() const
{
    return m_pixmap.rect();
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    painter->setRenderHints(QPainter::VerticalSubpixelPositioning, true);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::VerticalSubpixelPositioning, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->drawPixmap(0,0, m_pixmap);

    QPen attackAreaPen(QColor(255,20,147), 3);
    attackArea->setPen(attackAreaPen);
    //    painter->setPen(attackAreaPen);
    //    painter->setRenderHint(QPainter::Antialiasing);

    //test:
    //    painter->setPen(QPen(Qt::darkCyan , 25));
    //    painter->drawPoint(mapToScene(boundingRect().center() )  );
    QPen pen(QColor(255,20,147), 1);
    painter->setPen(pen);
    painter->drawRect(boundingRect());

    //test:
    QPen pen1(QColor(255,20,147), 5);
    painter->setPen(pen1);
    painter->drawPoint(this->boundingRect().center() );

}

QPainterPath Enemy::shape() const
{

    QPainterPath path;
    path.addRect(m_pixmap.rect());
    return path;

}

void Enemy::setLifeToZero()
{
    life = 0;
}

float Enemy::map(float value, float start1, float stop1, float start2, float stop2) {
    float result = start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
    return result;
}

float Enemy::normalize_angle(const float value ) {
    float start = -180.f;
    float end = 180.f;
    float width = end - start   ;
    float offsetValue = value - start ;
    return ( offsetValue - ( floor( offsetValue / width ) * width ) ) + start ;
}

void Enemy::applyForce(QVector2D force)
{
    acc = force;
}


void Enemy::advance(int phase)
{
    d = m_scene->getDelta();
    float speed = 110.f *d;



    float maxSpeed =  210.f *d;
    float maxForce = 0.02; ///turn sharpness
    QVector2D desiredVel = QVector2D(m_player->pos() - this->pos());
    float distance = desiredVel.length();
    desiredVel.normalize();
    if(distance < 1000){
        float m = map(distance, 0,1000,0, maxSpeed);
        desiredVel *=m;
    }else
        desiredVel *= maxSpeed;
    ///Reynolds formula:
    QVector2D steering = desiredVel - this->vel;
    steering.normalize();
    steering *= maxForce;
    applyForce(steering);


    rotateAccordingToDirection(); ///hmm

//    setRotationTowardTarget(); //to be deleted

//    std::cout << termcolor::red << "kąt: " << this->rotation() << termcolor::reset<< "\n";

    vel += acc;
    loc += vel;
    this->setPos(loc.x(), loc.y());
    acc = {0.f,0.f};


    ///===========================
    if(lifeBarProxy)
        lifeBarProxy->setPos(lifeBarProxy->x() + vel.x(), lifeBarProxy->y() + vel.y());
    fireSystemOrigin = pos();


    if(life<90 && fireSystem == nullptr){
        ///we're inside Enemy but Enemy is not parent of Particle, so Particle has own coordinates  - so both Enemy and Particle exist in scene coordinates,
        fireSystem = new ParticleSystem(m_scene, firePixmap, fireSystemOrigin);
        m_v_particleSystem->push_back(fireSystem);
    }
    if(fireSystem)
        fireSystem->setPosition(fireSystemOrigin);


    checkAreaCollisions();
    checkEnemyCollisions();
}

void Enemy::setRotationTowardTarget()
{
    ///player rotation toward target:
    QVector2D distBetweenPoints(((m_player->x() + m_player->boundingRect().width()/2 )
                                 - (this->x() + this->boundingRect().width()/2)),

                                (m_player->y() + m_player->boundingRect().height()/2)
                                - (this->y() + m_player->boundingRect().height()/2));

    double rad = std::atan2(distBetweenPoints.y(), distBetweenPoints.x()); //    float
    double degrees =  rad* 180.0 /M_PI;
    degrees = normalize_angle(degrees);
    this->setRotation(degrees+90);  // this->setRotation(degrees+270);
}


void Enemy::checkShootingAreaPeriodicly()
{
    if(playerInCollision == nullptr) return;
    //create bullet:
    QPointF desiredBulletPos = this->boundingRect().center();
    Bullet* bullet = new Bullet(m_scene, this, "enemy", playerInCollision, desiredBulletPos, vel); //shooter, target
    auto b = bullet->getPen();
    b->setColor(Qt::red); //    QPen bulletPen(Qt::red, 6);
    b->setWidth(6);
    b->setStyle(Qt::DotLine);
//    std::cout << "Enemy::bullet created: " << bullet << "\n";
    m_v_bullets->push_back(bullet);
}

void Enemy::checkAreaCollisions()
{
    QList<QGraphicsItem*> itemsThisCollides = attackArea->collidingItems(); ///get items in collision | Returns a list of all items that collide with this item.
    for (int i = 0; i < itemsThisCollides.size(); ++i) {

        try {
            if(dynamic_cast<Player*>(itemsThisCollides[i])){ //if the colliding item is of type Bullet*
                playerInCollision = itemsThisCollides[i];
                return;// no need to searech another
            }else
                playerInCollision = nullptr;
        }  catch (const std::bad_cast& e) {
            std::cout << "checkAreaCollisions(): " << e.what() << '\n';
        }
    }
}

void Enemy::checkEnemyCollisions()
{
    QList<QGraphicsItem*> itemsThisCollides =  collidingItems(); ///get items in collision | Returns a list of all items that collide with this item.
    for (int i = 0; i < itemsThisCollides.size(); ++i) {

        try {
            if(Bullet* b = dynamic_cast<Bullet*>(itemsThisCollides[i])){
                if(b->getShooter()!= nullptr && b->getTypeOfShooter() == "ally"){
//                    std::cout  << "b->getTypeOfShooter(): " << b->getTypeOfShooter()  << "\n";
                    life-=10;
                    //                    *firePixmap = firePixmap->scaled(100-life,100-life); //100-x
                    QPixmap fire = *loadResources.getFire1();
                    *firePixmap = fire.scaled(100-life,100-life); //100-x
                    if(lifeBar)
                        lifeBar->setValue(life);
                    m_scene->removeItem(itemsThisCollides[i]);
                    m_v_bullets->erase(std::remove(m_v_bullets->begin(), m_v_bullets->end(), itemsThisCollides[i]), m_v_bullets->end());
                    delete itemsThisCollides[i];
                    itemsThisCollides[i] = nullptr;
                }
            }

        }  catch (const std::bad_cast& e) {
            std::cout << "Enemy::checkEnemyCollisions()(): " << e.what() << '\n';
        }
    }
}

std::pair<int, QPointF> Enemy::checkEnemyLifeandPos()
{
    lifeAndPos.first = life;
    lifeAndPos.second = pos();
    return lifeAndPos;
}

void Enemy::rotateAccordingToDirection()
{
    //the method sets rotation and slows down rotation when the if conditions are matched:
//    float a =  std::atan2(vel.y(), vel.x())  *180.f/M_PI; //to degrees
//    a = normalize_angle(a);

    ///player rotation toward target:
    QVector2D vectorBetweenPoints(((m_player->x() + m_player->boundingRect().width()/2 )
                                 - (this->x() + this->boundingRect().width()/2)),

                                (m_player->y() + m_player->boundingRect().height()/2)
                                - (this->y() + m_player->boundingRect().height()/2));

    double angleBetweenPointsRad = std::atan2(vectorBetweenPoints.y(), vectorBetweenPoints.x()); //    float
    double angleBetweenPointsDeg =  angleBetweenPointsRad * 180.0 /M_PI;
    angleBetweenPointsDeg = normalize_angle(angleBetweenPointsDeg);

    prevAngle = normalize_angle(prevAngle);
    //    std::cout << "a - prevAngle: " << normalize_angle(a - prevAngle)  << "\n";

    float angleDiff = normalize_angle(angleBetweenPointsDeg  - prevAngle);

    float angleChange = 100.f*d; //by frame

//    if(angleDiff>=0 && angleDiff<angleChange )          angleBetweenPointsDeg = prevAngle + angleDiff;
//    else if(angleDiff>=angleChange && angleDiff<=180 )  angleBetweenPointsDeg = prevAngle + angleChange ;
//    else if(angleDiff<0 && angleDiff>-angleChange )     angleBetweenPointsDeg = prevAngle + angleDiff;
//    else if(angleDiff<=-angleChange && angleDiff>=-180) angleBetweenPointsDeg = prevAngle - angleChange;

    if(angleDiff>=0 && angleDiff<angleChange )          angleBetweenPointsDeg = prevAngle + angleDiff;
    else if(angleDiff>=angleChange && angleDiff<=180 )  angleBetweenPointsDeg = prevAngle + angleChange ;
    else if(angleDiff<0 && angleDiff>-angleChange )     angleBetweenPointsDeg = prevAngle + angleDiff;
    else if(angleDiff<=-angleChange && angleDiff>=-180) angleBetweenPointsDeg = prevAngle - angleChange;

    if(angleDiff>180.f || angleDiff<-180.f) qDebug() << "ERROR! nieprzeidziany zakres kata..." ;

    this->setTransformOriginPoint(this->boundingRect().center());
    this->setRotation(normalize_angle(angleBetweenPointsDeg-270.f));
    prevAngle = angleBetweenPointsDeg;
}

Enemy::~Enemy()
{
    std::cout << "Enemy destruction 1" << "\n";

    //    delete fire;
    //    fire = nullptr;

    //   delete firePixmap;
    //    firePixmap = nullptr;


    delete EnemyAreaTimer;
    EnemyAreaTimer = nullptr;

    lifeBar->deleteLater();
    lifeBar = nullptr;

    std::cout << "Enemy destruction 2" << "\n";

}
