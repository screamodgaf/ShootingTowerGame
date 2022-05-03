#include "tower.h"
#include "level1.h"
#include <iostream>
#include "game.h"
#include "ally.h"
#include "termcolor.hpp"
Tower::Tower(QObject *parent)  : QObject{parent}
{
    std::cout << "Tower() ctor" << "\n";
    rect.setRect(0,0, 50,50); //bounding rect
    setPos( 400- rect.width()/2, 300  - rect.height()/2) ;

    //setting area within which the tower shoots bullets:

    attackArea = new QGraphicsEllipseItem(this);
    float radius = 400;
    attackArea->setRect(-radius/2 + rect.width()/2,
                        -radius/2 + rect.width()/2, 400,400);

    // Game::getScene()->addItem(attackArea); //already added by parent, so not adding again

    playerInCollision = nullptr;

    life = 100;
    lifeBar = new QProgressBar(Level1::getView());
    lifeBar->setValue(100);
    lifeBar->setRange(0,100);

    int sizeX = 200;

    lifeBar->resize(200,17);
    lifeBar->move(pos().x()- sizeX/2, pos().y()-100);
    lifeBar->setContentsMargins(10,10,10,10);
    lifeBar->setAlignment(Qt::AlignCenter);
    lifeBar->show();




    //Bullet managment element:
    //periodic check if enemy within tower so it shoots every second and not every every frame:
    QTimer* towerAreaTimer = new QTimer(this);
    connect(towerAreaTimer, &QTimer::timeout, this,
            &Tower::checkShootingAreaPeriodicly);
    towerAreaTimer->start(700);
}

QRectF Tower::boundingRect() const
{
    return rect;
}

void Tower::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

    QPen towerPen(Qt::red, 3);
    painter->setPen(towerPen);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(rect);

    QPen attackAreaPen(QColor(255,20,147), 3);
    attackArea->setPen(attackAreaPen);
    painter->setPen(attackAreaPen);
    painter->setRenderHint(QPainter::Antialiasing);

}

void Tower::checkShootingAreaPeriodicly()
{
    if(playerInCollision == nullptr) return;
    //create bullet:
    QPointF desiredBulletPos = this->boundingRect().center();
    Bullet* bullet = new Bullet(this, playerInCollision, desiredBulletPos); //shooter, target
    auto b = bullet->getPen();
    b->setColor(Qt::red); //    QPen bulletPen(Qt::red, 6);
    b->setWidth(6);
    b->setStyle(Qt::DotLine);
    std::cout << "Tower::bullet created: " << bullet << "\n";
    Level1::getBulletContainer()->push_back(bullet);
}

void Tower::checkAreaCollisions()
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

void Tower::checkTowerCollisions()
{
    QList<QGraphicsItem*> itemsThisCollides =  collidingItems(); ///get items in collision | Returns a list of all items that collide with this item.
     std::vector<Bullet*>* bullets = Level1::getBulletContainer();
    for (int i = 0; i < itemsThisCollides.size(); ++i) {

        try {
            if(Bullet* b =dynamic_cast<Bullet*>(itemsThisCollides[i])){
                if( dynamic_cast<Ally*>(b->getShooter())){
                    life-=10;
                    lifeBar->setValue(life);
                    Game::getScene()->removeItem(itemsThisCollides[i]);
                   bullets->erase(std::remove(bullets->begin(), bullets->end(), itemsThisCollides[i]), bullets->end());
                    delete itemsThisCollides[i];
                    itemsThisCollides[i] = nullptr;


//                    std::cout << termcolor::on_green << "BOOOOOOM-------------" << termcolor::reset<< "\n";
                }
            }

        }  catch (const std::bad_cast& e) {
            std::cout << "Tower::checkTowerCollisions()(): " << e.what() << '\n';
        }
    }
}

QProgressBar *Tower::getQProgressBar()
{
    return lifeBar;
}

void Tower::advance(int phase)
{
    checkAreaCollisions();
    checkTowerCollisions();
}
