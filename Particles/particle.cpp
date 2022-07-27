#include "particle.h"
#include <QPen>
#include <QPainter>
#include <ctime>
#include <QPixmap>
#include <QGraphicsScene>
#include "level1.h"
#include <iostream>
Particle::Particle(QPixmap *pixmap, QPointF &origin, Level1 *scene): m_origin{&origin}, m_pixmap{pixmap}, m_scene{scene}
{


    //    rect.setSize(QSizeF(40,40));

    //generate random float:
    this->setTransformOriginPoint(this->boundingRect().center());
    float min = -1;
    float max = 1;
    float r = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
    float r1 = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));

    vel = {r, r1 };
    //    acc = {0, 0.02};
    pos = {(float)m_origin->x(), (float)m_origin->y()};
//    pos = {400,400};

//    pos = {mapToScene((float)m_origin->x(), (float)m_origin->y())};

    this->setPos(pos.x(), pos.y());
    //rect.setSize(QSizeF(std::abs(r *60), std::abs(r *60)));
    lifespan = 1;

    //set QPixmap
    if(pixmap == nullptr)
        std::cout << "pixmap == nullptr" << "\n";
    else {
        //        QPixmap* pixmap1 = new QPixmap("E:\\Qt_workspace\\ShootingTower\\smoke.png");


//        this->setPixmap( *pixmap );

    }

//qDebug() << "in particle: " << *m_origin;
}


void Particle::update()
{
    d = m_scene->getDelta();
    if(lifespan>=0)
        this->setOpacity(lifespan); ///qreal alpha is specified in the range 0.0-1.0.


    vel += acc;
    pos += vel;
//qDebug() << "in particle: " << pos;
    //limit velocity if its too large
    //    if( abs(vel.x()) > 20){
    //        vel.setX(vel.x()/2);
    //        vel.setY(vel.y()/2);
    //    }
    this->setPos(pos.x() , pos.y() );

//qDebug() << "in particle: " << pos;
//    qDebug() << "in particle: " << pos;
    acc = {0,0}; ///reseting acceleration to 0
    //    lifespan-=0.005;
    lifespan-=0.015;
}



void Particle::advance(int phase)
{

}

//void Particle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    QRectF rect = boundingRect();

//    QColor white = Qt::white;
//    if(lifespan>=0)
//        white.setAlphaF(lifespan); ///qreal alpha is specified in the range 0.0-1.0.
//    painter->setBrush(white);
//    painter->setPen(Qt::transparent);
//    painter->setRenderHint(QPainter::Antialiasing);
//    painter->drawEllipse(rect);

//}

bool Particle::is_finished()
{
    if(lifespan<=0)
        return true;
    else
        return false;
}

void Particle::applyForce(QVector2D &force)
{
        float speed = 20.f;
        acc *= speed*  d;

    //qDebug() << acc;
        acc+= force;
}

void Particle::setPosition(QPointF pos_)
{
    std::cout << "pos" << "\n";
}

void Particle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //    std::cout << "void Particle::paint(QPainter" << "\n";
    painter->setCompositionMode(QPainter::CompositionMode_Plus);

    //    m_pixmap = m_pixmap->scaled(10,10);

    painter->drawPixmap(m_pixmap->rect(), *m_pixmap);
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
    //painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    //pix.setCompositionMode(QPainter::CompositionMode_Plus);
    //painter->fillRect(m_pixmap->rect(), QColor(110, 110, 110,  140)); // colorize the light in



}

Particle::~Particle()
{
    //         std::cout << "particle DESTROYED" << "\n";
}
