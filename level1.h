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



class Level1 : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Level1();
    virtual void keyPressEvent(QKeyEvent *event);
    void setFPScounter();
    void countFPS();

    static std::vector<Bullet*> *getBulletContainer();
public slots:
    void advance();
    void checkTowersAreaPeriodicly();

//    void update(const QRectF &rect = QRectF());
signals:
private:
    void createBullet();
private:
    QGraphicsView* m_view;
//    Enemy* enemy;
    Tower* tower;


    Player* player;

    QRectF sceneRect;
    static  std::vector<Bullet*> v_bullets;
    QLabel* fpsLabel;
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end ;
    std::chrono::duration<float> duration, timeElapsed;
};

#endif // LEVEL1_H
