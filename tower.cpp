#include "tower.h"
#include "level1.h"
#include <iostream>
#include "game.h"
#include "ally.h"
#include "termcolor.hpp"
#include "loadresources.h"
#include "playerweapons.h"
Tower::Tower(Level1* scene, std::vector<Bullet*>* v_bullets, std::vector<ParticleSystem*>* v_particleSystem, QGraphicsItem *parent)  : QGraphicsItem{parent}, m_scene{scene}, m_v_bullets{v_bullets}, m_v_particleSystem{v_particleSystem}
{
    std::cout << "Tower() ctor: " << this << "\n";
    rect.setRect(0,0, 50,50); //bounding rect
    setPos( 700- rect.width()/2, 300  - rect.height()/2) ;
    this->setTransformOriginPoint(this->boundingRect().center());

    loc = {static_cast<float>(pos().x()) ,static_cast<float>(pos().y()) };
    vel = {1.f ,0.f };
    acc = {0.f ,0.f };

    ///setting area within which the tower shoots bullets:
    attackArea = new QGraphicsEllipseItem(this);
    std::cout << "attackArea: " << attackArea << "\n";
    float radius = 400;
    attackArea->setRect(-radius/2 + rect.width()/2,
                        -radius/2 + rect.width()/2, 400,400);

    // Game::getScene()->addItem(attackArea); //already added by parent, so not adding again

    playerInCollision = nullptr;

//    originalFire = *LoadResources::getFire1(); //Pixmap

    firePixmap = loadResources.getFire1(); //Pixmap
    *firePixmap = firePixmap->scaled(QSize(200,200 ));

    //    createFireParticeSystem();
    int sizeX = 200;




    //Bullet managment element:
    //periodic check if enemy within tower so it shoots every second and not every every frame:
    towerAreaTimer = new QTimer;
    //    QTimer::connect(towerAreaTimer, &QTimer::timeout, this,
    //            &Tower::checkShootingAreaPeriodicly);
    QTimer::connect(towerAreaTimer, &QTimer::timeout, [this](){ checkShootingAreaPeriodicly();});
    towerAreaTimer->start(700);


    //SET LIFE BAR:
    life = 100;
    lifeBar = new QProgressBar();
    lifeBar->setValue(100);
    lifeBar->setRange(0,100);
    lifeBar->resize(200, 17);
    lifeBar->setAlignment(Qt::AlignCenter);
    //    if(m_scene){
    lifeBarProxy = m_scene->addWidget(lifeBar);
    lifeBarProxy->setPos(this->x() - lifeBar->width()/2 + this->boundingRect().width()/2 , this->y()-50);
    lifeBarProxy->show();

    qDebug() << "lifeBarProxy->scenePos(): " << lifeBarProxy->scenePos() << "\n";
    //    }
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

    //test:
    //    painter->setPen(QPen(Qt::darkCyan , 25));
    //    painter->drawPoint(mapToScene(boundingRect().center() )  );

}


void Tower::advance(int phase)
{
    d = m_scene->getDelta();
    float speed = 110.f *d;
    vel += acc;
    loc += vel;
    //qDebug() << loc.x();
    this->setPos(loc.x(), loc.y());
    acc = {0.f,0.f};
    if(lifeBarProxy)
        lifeBarProxy->setPos(lifeBarProxy->x() + vel.x(), lifeBarProxy->y() + vel.y());


    fireSystemOrigin = pos();
    if(life<90 && fireSystem == nullptr){
///we're inside Tower but Tower is not parent of Particle, so Particle has own coordinates  - so both tower and Particle exist in scene coordinates,
        //        *firePixmap = firePixmap->scaled(50,50);
        fireSystem = new ParticleSystem(m_scene, firePixmap, fireSystemOrigin, m_v_particleSystem);
        m_v_particleSystem->push_back(fireSystem);
    }
    if(fireSystem)
        fireSystem->setPosition(fireSystemOrigin);
    checkAreaCollisions();
    checkTowerCollisions();



}

void Tower::checkShootingAreaPeriodicly()
{
    if(playerInCollision == nullptr) return;
    //create bullet:
    QPointF desiredBulletPos = this->boundingRect().center();
    Bullet* bullet = new Bullet(m_scene, this, playerInCollision, desiredBulletPos, m_v_particleSystem, vel); //shooter, target
    auto b = bullet->getPen();
    b->setColor(Qt::red); //    QPen bulletPen(Qt::red, 6);
    b->setWidth(6);
    b->setStyle(Qt::DotLine);
    std::cout << "Tower::bullet created: " << bullet << "\n";
    m_v_bullets->push_back(bullet);
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
    for (int i = 0; i < itemsThisCollides.size(); ++i) {

        try {
            if(Bullet* b =dynamic_cast<Bullet*>(itemsThisCollides[i])){
                if( dynamic_cast<Ally*>(b->getShooter())){
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
            std::cout << "Tower::checkTowerCollisions()(): " << e.what() << '\n';
        }
    }
}

std::pair<int, QPointF> Tower::checkTowerLifeandPos()
{
    lifeAndPos.first = life;
    lifeAndPos.second = pos();
    return lifeAndPos;
}



Tower::~Tower()
{
    std::cout << "Tower destruction 1" << "\n";

    //    delete fire;
    //    fire = nullptr;

    //   delete firePixmap;
    //    firePixmap = nullptr;


    delete towerAreaTimer;
    towerAreaTimer = nullptr;

    lifeBar->deleteLater();
    lifeBar = nullptr;

    std::cout << "Tower destruction 2" << "\n";

}
