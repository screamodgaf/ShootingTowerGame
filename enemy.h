#ifndef ENEMY_H
#define ENEMY_H
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QProgressBar>
#include "Particles/particlesystem.h"
#include <utility>
#include <QGraphicsProxyWidget>
#include <QTimer>
#include "loadresources.h"
class Bullet;
class Level1;
class Player;

class Enemy :public QGraphicsPixmapItem
{

public:
    explicit Enemy(Level1* scene, QPixmap pixmap, std::vector<Bullet*>* v_bullets, std::vector<ParticleSystem*>* v_particleSystem, Player* player, QGraphicsPixmapItem *parent = nullptr);
    virtual ~Enemy();

    std::pair<int, QPointF> checkEnemyLifeandPos();
    void advance(int phase) override;
    void setLifeToZero();
signals:
private slots:
        void checkShootingAreaPeriodicly();
private:

    void checkAreaCollisions();
    void checkEnemyCollisions();
    void rotateAccordingToDirection();
    void setRotationTowardTarget();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
    QPainterPath shape() const;
    float normalize_angle(const float value );
    float map(float value, float start1, float stop1, float start2, float stop2);
    void applyForce(QVector2D force);
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

    Player* m_player = nullptr;
    //for particle system:
    QPixmap* firePixmap = nullptr;
    QPixmap* nebulaPixmap = nullptr;
//    QPixmap originalFire;
    ParticleSystem* fireSystem = nullptr;
//    ParticleSystem* nebulaSystem = nullptr;
    QTimer* EnemyAreaTimer;
    float prevAngle =0.f;
    std::pair<int, QPointF> lifeAndPos;
    LoadResources loadResources;
    QPointF fireSystemOrigin = {0,0};
    QPixmap m_pixmap;
    QVector2D loc;
    QVector2D vel;
    QVector2D acc;
};

#endif // Enemy_H



