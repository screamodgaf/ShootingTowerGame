#ifndef TOWER_H
#define TOWER_H
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include "enemy.h"
#include <QProgressBar>
class Tower : public QObject, public QGraphicsItem, public Enemy
{
    Q_OBJECT
public:
    explicit Tower(QObject *parent = nullptr);
    QRectF boundingRect() const override;
    void advance(int phase) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
//    void checkCollisionsWithAttackArea(QGraphicsItem *item);

    void checkAreaCollisions();
    void checkTowerCollisions();
    QProgressBar* getQProgressBar();
signals:
private:
    void checkShootingAreaPeriodicly();
private:
   QGraphicsItem* playerInCollision;
    int life;
    QRectF rect;
    QGraphicsEllipseItem* attackArea;
    QProgressBar* lifeBar;
};

#endif // TOWER_H



