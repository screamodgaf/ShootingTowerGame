#ifndef LEVEL1_H
#define LEVEL1_H
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "tower.h"
#include "enemy.h"
#include "bullet.h"
#include <QGraphicsLineItem>
#include "player.h"
#include <QKeyEvent>
#include <vector>
#include <QLabel>
#include <QElapsedTimer>

#include "Particles/particlesystem.h"
#include "Particles/fireparticlesystem.h"
#include "Particles/repeller.h"
#include "playerweapons.h"
#include "bulletshooting.h"
class Level1 : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Level1();
    ~Level1();
    virtual void keyPressEvent(QKeyEvent *event);
    void setFPScounter();
    void countFPS();
    static std::vector<Bullet*> *getBulletContainer();
    void drawBackground(QPainter *painter, const QRectF &rect);
public slots:
    void advance();
    static float getDelta();
    static QGraphicsView *getView();
    static Player *getPlayer();
    //    void update(const QRectF &rect = QRectF());
signals:
private:
    void createBullet();
private:
    static QGraphicsView* m_view;
    Tower* tower = nullptr;
    static Player* player;
 QProgressBar* b = nullptr;

    QRectF sceneRect;

    QLabel* fpsLabel = nullptr;
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end ;
    static std::chrono::duration<float> duration, timeElapsed;
    static std::vector<Bullet*> v_bullets; //main bulelt container - there is only one container with bullets

//    PlayerWeapons* playerWeapons = nullptr;
    BulletShooting* bulletShooting = nullptr;

    QGraphicsPixmapItem* pixmapItem = nullptr;
    ParticleSystem* particleSystem = nullptr;
    ParticleSystem* particleSystem1 = nullptr;
    ParticleSystem* particleSystem2 = nullptr;
    FireParticleSystem* fireParticleSystem = nullptr;
    std::vector<ParticleSystem*> v_particleSystem;
    QPixmap* playerPixmap = nullptr;
    QVector2D gravity = {0.f,0.f};
    Repeller* repeller = nullptr;
    QPainter* painter;
};

#endif // LEVEL1_H
