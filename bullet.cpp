#include "bullet.h"
#include "level1.h"
#include <cmath>
#include <QDebug>
#include <iostream>

Bullet::Bullet(QGraphicsItem *parent) : QGraphicsItem{parent}
{

    rect.setSize(QSizeF(100,10));
    //    pos = {0,0};
    vel = {0,0};
    acc = {0,0};
    float parWidth = parent->boundingRect().width();
    float parHight = parent->boundingRect().height();
    pos = {0 + parWidth/2, 0 + parHight/2};  //set position toward parent (was {0,0}) so its in parents centre
    this->setX(pos.x());
    this->setY(pos.y());
}
//tower, player, this);
void Bullet::estimateBulletTrajectory(QGraphicsItem* origin, QGraphicsItem* target, QGraphicsScene* scene)
{

    QPen pen;
    pen.setColor(Qt::red);
    orginWidth = origin->boundingRect().width();
    orginHight = origin->boundingRect().height();
    QLineF ln(target->pos(), QPointF(origin->scenePos().x() + orginWidth/2,
                                     origin->scenePos().y() + orginHight/2));
    scene->addLine(ln,pen);

    acc.setX(target->x() - this->mapToScene(x(),y()).x()  + orginWidth/2); //distance vector //mapToScene from parent
    acc.setY(target->y() - this->mapToScene(x(),y()).y()  + orginHight/2);
    acc.normalize();
}
void Bullet::setRotationTowardTarget(QGraphicsItem *origin, QGraphicsItem *target, QGraphicsScene *scene)
{
    QVector2D distBetweenPoints(target->x() - (origin->x() + orginWidth/2),  /// +orginWidth/2 so bullet wont lose trajectory
                                target->y() - (origin->y() + orginHight/2));
    double rad = std::atan2(distBetweenPoints.y(), distBetweenPoints.x());
    double degrees =  rad* 180.0 /M_PI;
    this->setRotation( degrees );
    return;
}

void Bullet::update()
{
    vel += acc;
    pos += vel;

    //limit velocity if its too large
    //    if( abs(vel.x()) > 20){
    //        vel.setX(vel.x()/2);
    //        vel.setY(vel.y()/2);
    //    }


    this->setPos(pos.x(), pos.y());
    vel = {0,0};

}

QRectF Bullet::boundingRect() const
{
    return rect;
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    QPen pen(Qt::red, 8);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRect(rect);
}

void Bullet::advance(int phase)
{

}
