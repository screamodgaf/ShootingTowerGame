#ifndef TOWER_H
#define TOWER_H
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include "enemy.h"
#include <QProgressBar>
#include "Particles/particlesystem.h"
#include <utility>
#include <QGraphicsProxyWidget>
#include <QTimer>
#include "loadresources.h"
class Bullet;
class Level1;

class Tower :public QGraphicsItem, public Enemy
{

public:
    explicit Tower(Level1* scene, std::vector<Bullet*>* v_bullets, std::vector<ParticleSystem*>* v_particleSystem, QGraphicsItem *parent = nullptr);
    virtual ~Tower();

    std::pair<int, QPointF> checkTowerLifeandPos();
    void advance(int phase) override;

signals:
private slots:
        void checkShootingAreaPeriodicly();
private:

    void checkAreaCollisions();
    void checkTowerCollisions();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
    Level1 *m_scene = nullptr;
    std::vector<Bullet*>* m_v_bullets = nullptr;
    QGraphicsItem* playerInCollision = nullptr;
    int life;
    float d; //deltatime
    QRectF rect;
    QGraphicsEllipseItem* attackArea = nullptr;
    QProgressBar* lifeBar = nullptr;
    QGraphicsProxyWidget *lifeBarProxy = nullptr;
    std::vector<ParticleSystem*>* m_v_particleSystem;
    //for particle system:
    QPixmap* firePixmap = nullptr;
//    QPixmap originalFire;
    ParticleSystem* fireSystem = nullptr;
    QTimer* towerAreaTimer;
    std::pair<int, QPointF> lifeAndPos;
    LoadResources loadResources;
    QPointF fireSystemOrigin = {0,0};

    QVector2D loc;
    QVector2D vel;
    QVector2D acc;
};

#endif // TOWER_H



