#include "bullet.h"
#include "level1.h"
#include <cmath>
#include <QDebug>
#include <iostream>
#include "termcolor.hpp"
#include "game.h"
Bullet::Bullet(Level1* scene, QGraphicsItem *shooter_, QGraphicsItem *target_, QPointF desiredBulletPos, std::vector<ParticleSystem *> *v_particleSystem, QVector2D vel_): target{target_}, m_scene{scene} , shooter {shooter_}, m_v_particleSystem{v_particleSystem}, shootersVel{vel_}
{
    if(shooter==nullptr || target == nullptr) return;
    std::cout << termcolor::red << "Bullet::Bullet Created: " << this << termcolor::reset<< "\n";
    rect.setSize(QSizeF(30,2 ));
    this->setTransformOriginPoint(0,boundingRect().height()/2);
    this->setPos(mapFromItem(shooter, desiredBulletPos));
    qDebug() << "shootersVel: " << shootersVel;

    ///adjust bullets velocity to shooters velocity:
    d = m_scene->getDelta();
    float speed = 187*d ;
//    float averageVel = std::sqrt(shootersVel.x()* shootersVel.x() + shootersVel.y()* shootersVel.y());
//    //    float scalar;
//    float desiredVel = speed ;
//    scalar = desiredVel/averageVel;
//    std::cout << termcolor::on_yellow << "scalar: " << scalar << termcolor::reset<< "\n";
////    vel*=scalar;
    vel = shootersVel;
    vel += acc;
    std::cout << termcolor::on_yellow << "vel: " << vel.x() << " " << vel.y() << termcolor::reset<< "\n";

    loc = { static_cast<float>(this->pos().x()), static_cast<float>(this->pos().y())};

    //    std::cout << termcolor::green << "vel: " << vel.x() << " " << vel.y() << termcolor::reset<< "\n";


    m_scene->addItem(this);
    estimateBulletTrajectory();
    setRotationTowardTarget();
    //    std::cout << termcolor::on_blue << "origin: " << this->transformOriginPoint().x() << " " << this->transformOriginPoint().y() << termcolor::reset<< "\n";
    qDebug() << "Bullet ctor End";
}


void Bullet::estimateBulletTrajectory()
{
    if(shooter==nullptr || target == nullptr) return;

    pen.setColor(Qt::red);

    shooterWidth = shooter->boundingRect().width();
    shooterHight = shooter->boundingRect().height();
    targetWidth = target->boundingRect().width();
    targetHight = target->boundingRect().height();

    ///draws line from tower CENTRE (+targetWidth/2 for centering) to target's CENTRE:
    //setting QLineF:
    //    ln.setP1(QPointF(target->pos().x() + targetWidth/2,
    //                     target->pos().y() + targetHight/2));
    //    ln.setP2(QPointF(shooter->scenePos().x() + shooterWidth/2,
    //                     shooter->scenePos().y() + shooterHight/2));
    //    line.setLine(ln);
    //    line.setPen(pen);

    //    Game::getScene()->addItem(&line);

    ///acceleration (normalized vector) from shooter to target to add to velocity every frame so bullet heads toward target
    ///acc.setX(ln.p1().rx() - ln.p2().rx()); //works too instead of acc.setX((target->x() + targetWidth/2) - (shooter->x() + orginWidth/2));
    ///acc.setY(ln.p1().ry() - ln.p2().ry()); //works too works too instead of acc.setY((target->y() + targetHight/2) - (shooter->y() + orginHight/2));


    acc.setX((target->x() + targetWidth/2) - (shooter->x() + shooterWidth/2));
    acc.setY((target->y() + targetHight/2) - (shooter->y() + shooterHight/2));

    acc.normalize();
}

void Bullet::setRotationTowardTarget()
{
    QVector2D distBetweenPoints((target->x() + targetWidth/2) - (shooter->x() + shooterWidth/2),  /// +orginWidth/2 so bullet wont lose trajectory
                                (target->y() + targetHight/2) - (shooter->y() + shooterHight/2));

    double rad = std::atan2(distBetweenPoints.y(), distBetweenPoints.x());
    double degrees =  rad* 180.0 /M_PI;
    this->setRotation( degrees );
    return;
}

void Bullet::update()
{
    //    auto angleRad = (M_PI/180) * shooter->rotation() + (M_PI/180)*90;
    //    float distanceFromCentre = -80;
    //    this->setX(distanceFromCentre * std::cos(angleRad) + shooter->pos().x() + shooter->boundingRect().width()/2);
    //    this->setY(distanceFromCentre * std::sin(angleRad) + shooter->pos().y() + shooter->boundingRect().height()/2);
    //    this->setRotation(shooter->rotation());
    //    return;
    d = m_scene->getDelta();

    //    std::cout << "Bullet::update()" << "\n";
    //    std::cout << termcolor::green <<"Bullet::update()" << termcolor::reset<< "\n";


    //    float speed = 50.f *  d;
    //    float averageVel = std::sqrt(shootersVel.x()* shootersVel.x() + shootersVel.y()* shootersVel.y());
    //    float scalar;
    //    float desiredVel = speed ;
    //    scalar = desiredVel/averageVel;
    //    acc*=scalar;

    vel += acc;
    loc += vel;
    this->setPos(loc.x(), loc.y());
    acc = {0,0};

}



QRectF Bullet::boundingRect() const
{
    return rect;
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    //    QPen bulletPen(Qt::red, 6);
    //    bulletPen.setStyle(Qt::DotLine);

    painter->setPen(bulletPen);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRect(rect);

}

QPen *Bullet::getPen()
{
    return &bulletPen;
}

bool Bullet::checkBulletsDistFromShooter()
{
    if(shooter == nullptr || target == nullptr) return false;

    ln2.setP1(QPointF(this->x(), this->y()));
    ln2.setP2(QPointF(shooter->x() + shooterWidth/2,
                      shooter->y() + shooterHight/2));

    if(ln2.length() >= shooter->boundingRect().width()*4){
        if(m_scene)
            m_scene->removeItem(this);

        return true;
        //        scene()->removeItem(this);
        ///remove this bullet from m_v_Bullets - container that has all Bullets inside, so we wont hit nullpointer
    }
    return false;
}

QGraphicsItem *Bullet::getShooter()
{
    return shooter;
}

//char Bullet::getShooterType()
//{
//    return shooterType;
//}

void Bullet::advance(int phase)
{
    //    std::cout << "void Bullet::advance(int phase)" << "\n";
}

Bullet::~Bullet()
{
    std::cout << termcolor::on_red << "BULLET:Bulle destroyed ~Bullet(): " << this << termcolor::reset<< "\n";
    //    std::cout << "BULLET destroyed ~Bullet(): " << this << "\n";
}
