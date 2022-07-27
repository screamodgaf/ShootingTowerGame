#include "playerdefences.h"
#include "bullet.h"
#include <iostream>
#include "termcolor.hpp"
#include <typeinfo>
#include "enemy.h"
#include "level1.h"
//#include <boost/stacktrace.hpp>

PlayerDefences::PlayerDefences(QGraphicsScene *scene, QGraphicsPixmapItem *parent, std::vector<Bullet *> *v_bullets):
    m_v_bullets{v_bullets}, m_scene{scene}
{
    m_view = m_scene->views()[0];
    life = 100;
    lifeBar = new QProgressBar(m_view);

    lifeBar->setRange(0,100);
    //    lifeBar->setValue(90);

    lifeBar->resize(200,17);
    lifeBar->setContentsMargins(10,10,10,10);
    lifeBar->setAlignment(Qt::AlignCenter);
    setActualLife();
    lifeBar->show();

}



void PlayerDefences::checkCollisions(QGraphicsItem* item)
{
    //    QList<QGraphicsItem*> temp;
    try {
        QList<QGraphicsItem*> itemsCollideWithShip = item->collidingItems(); ///get items in collision | Returns a list of all items that collide with this item.
        for (int i = 0; i < itemsCollideWithShip.size(); i++) {

            if(itemsCollideWithShip[i]==nullptr) continue;

            if(Bullet* b = dynamic_cast<Bullet*>(itemsCollideWithShip[i]) ){ //if the colliding item is of type Bullet*
                if(b->getShooter() != nullptr && b->getTypeOfShooter() == "enemy" ){ // checking if bullet in collision with the ship is not coming from the ship, but from Enemy
                    life-=10;
                    setActualLife();
                    m_scene->removeItem(itemsCollideWithShip[i]);
                    m_v_bullets->erase(std::remove(m_v_bullets->begin(), m_v_bullets->end(), itemsCollideWithShip[i]), m_v_bullets->end());
                    delete itemsCollideWithShip[i];
                    itemsCollideWithShip[i] = nullptr;
                    return;
                }
            }
        }
    } catch (const std::bad_cast& e) {
        std::cout << e.what() << '\n';

    }


    //    for (int z = 0; z < temp.size(); ++z) {
    //        delete temp[z];
    //        temp[z] = nullptr;
    //        std::cout << "deleeee" << "\n";
    //    }
}

void PlayerDefences::setActualLife()
{
    lifeBar->setValue(life);
    //     lifeBar->show();
}

PlayerDefences::~PlayerDefences()
{
    std::cout << "~PlayerDefences()" << "\n";
    lifeBar->deleteLater();
}
