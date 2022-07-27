#include "playerweapons.h"
#include <iostream>
#include <QGraphicsPolygonItem>
#include "level1.h"
#include <QTimer>
#include <iostream>
#include "termcolor.hpp"
PlayerWeapons::PlayerWeapons(Level1* scene, QPointF desiredBulletPos_, std::vector<Bullet *> *v_bullets, Player* parentPlayer, QGraphicsPixmapItem *parent): QGraphicsPolygonItem{parentPlayer},m_scene{scene}, m_v_bullets{v_bullets}, desiredBulletPos{desiredBulletPos_}, m_parentPlayer{parentPlayer}
{
    std::cout << "PlayerWeapons::PlayerWeapons Ctor" << "\n";

    triangle.append(QPointF(80.f,0.f));
    triangle.append(QPointF(40.f,200.f));
    triangle.append(QPointF(0.f,0.f));

    //     Add the triangle polygon to the scene
    //    viewfinderItem = new QGraphicsPolygonItem(this);
    //    pTriangleItem->setPos(parent->boundingRect().width()/2, 200);
    this->setPolygon(triangle);   //parent->boundingRect().height()
    //    this->setPos(parent->x()-14 ,  parent->y() - parent->boundingRect().height()-100);

    float playerWidth = parent->boundingRect().width(); //56px
    float thisWidth   = this->boundingRect().width(); //80px
    float offsetWidth = thisWidth - playerWidth; //80px - 56px = 24 px

    float playerHeight = parent->boundingRect().height(); //56px

    this->setPos(0  + offsetWidth/4  ,0 - playerHeight*2+10);

    rect.setSize(triangle.boundingRect().size());

    setTransformOriginPoint(this->boundingRect().center());
    QTimer* areaTimer = new QTimer();
    connect(areaTimer, &QTimer::timeout, this,  &PlayerWeapons::createBullet);
    areaTimer->start(1000);

}

PlayerWeapons::~PlayerWeapons()
{
    std::cout << "~PlayerWeapons()" << "\n";
}


bool PlayerWeapons::contains(const QPointF &point) const
{
//    std::cout << "Player::contains(const QPointF &point) const " << "\n";
    return false;
}


QRectF PlayerWeapons::boundingRect() const
{
    return rect ;
}

void PlayerWeapons::advance(int phase)
{
    //        std::cout << "PlayerWeapons::advance" << "\n";
    checkViewfinderCollisions();


}

void PlayerWeapons::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);

    if(targetHit!= nullptr){
        QPen trianglePen(Qt::green, 2);
        trianglePen.setStyle(Qt::PenStyle::DashDotLine);
        painter->setPen(trianglePen);
        painter->drawPolygon(triangle);
    }

    //    painter->setPen(QPen(Qt::yellow, 1));
    //    painter->drawRect(this->boundingRect());

    //------------------
    //    painter->setPen(QPen(Qt::darkCyan , 25));
    //    painter->drawPoint(0 - this->boundingRect().width()/2, 0    );
}

void PlayerWeapons::checkViewfinderCollisions()
{
    targetHit= nullptr; ///very important - sets targetHit to null - so new Bullet directed at deleted tower is not created
    QList<QGraphicsItem*> itemsThisCollides = this->collidingItems(); ///get items in collision | Returns a list of all items that collide with this item.

    for (int i = 0; i < itemsThisCollides.size(); ++i) {
        try {
            if(Enemy* enemy = dynamic_cast<Enemy*>(itemsThisCollides[i])){  ///if the colliding item is of type Bullet*

                targetHit = itemsThisCollides[i];
                //                std::cout << "itemsThisCollides: " << itemsThisCollides[i] << "\n";
                //                std::cout << "targetHit: " << targetHit << "\n";
                break;// no need to searech another
            }else
                targetHit = nullptr;
        }  catch (const std::bad_cast& e) {
            std::cout << "catch (const std::bad_cast& e in PlayerWeapons: " << e.what() << "\n";
        }
    }
}




void PlayerWeapons::createBullet() //targetHit != tower
{
//    qDebug() << "PlayerWeapons::createBullet()";
    //    std::cout << termcolor::on_yellow << "targetHit: "  << targetHit << " tower: " <<
    //                 Level1::getTower() << termcolor::reset<< "\n";


    if(targetHit == nullptr) return;
//    std::cout << termcolor::on_blue << "PlayerWeapons::createBullet()" << termcolor::reset<< "\n";

    //    std::cout << termcolor::red << "m_parentPlayer->getVel(): " << m_parentPlayer->getVel().x() << " " << m_parentPlayer->getVel().y() << termcolor::reset<< "\n";


    bullet = new Bullet(m_scene, this->parentItem(), "ally", targetHit, desiredBulletPos, m_parentPlayer->getVel());

//qDebug() << "PlayerWeapons::createBullet()";
    bullet->getPen()->setColor(Qt::yellow);
    bullet->getPen()->setStyle(Qt::DotLine);
    bullet->getPen()->setWidth(6);

    m_v_bullets->push_back(bullet);

}

std::vector<Bullet*>* PlayerWeapons::getBulletcontainer()
{
    return m_v_bullets;
}

