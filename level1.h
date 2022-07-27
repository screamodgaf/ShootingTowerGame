#ifndef LEVEL1_H
#define LEVEL1_H
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "enemy.h"
#include "bullet.h"
#include <QGraphicsLineItem>
#include "player.h"
#include <QKeyEvent>
#include <vector>
#include <QLabel>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include "Particles/particlesystem.h"
#include "Particles/fireparticlesystem.h"
#include "Particles/repeller.h"
#include "playerweapons.h"
#include "loadresources.h"

class Game;

class Level1 : public QGraphicsScene
{
public:
    explicit Level1();
    ~Level1();
    virtual void keyPressEvent(QKeyEvent *event);
    void setFPScounter();
    void countFPS();

        float getDelta();
public slots:
    void advance();



    //    void update(const QRectF &rect = QRectF());
signals:
private:
    void createBullet();

private:
    QGraphicsView* m_view = nullptr;
    Enemy* enemy = nullptr;
    Player* player = nullptr;
    QElapsedTimer* timer = nullptr;
 QProgressBar* b = nullptr;
     QOpenGLWidget *gl = nullptr;
    QRectF sceneRect;

    QLabel* fpsLabel = nullptr;
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end ;
    std::chrono::duration<float> duration, timeElapsed;
    std::vector<Bullet*> v_bullets; //main bulelt container - there is only one container with bullets

//    PlayerWeapons* playerWeapons = nullptr;

    float deltatime =0.0;
    QGraphicsPixmapItem* pixmapItem = nullptr;
    ParticleSystem* nebulaSystem = nullptr;
    QPixmap* bonusNebulaPixmap = nullptr;

    ParticleSystem* particleSystem2 = nullptr;
    FireParticleSystem* fireParticleSystem = nullptr;
    std::vector<ParticleSystem*> v_particleSystem;
    QVector2D gravity = {0.f,0.f};
    Repeller* repeller = nullptr;
    QPainter* painter;
    LoadResources* loadResources;
};

#endif // LEVEL1_H
