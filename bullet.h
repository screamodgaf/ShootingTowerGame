#ifndef BULLET_H
#define BULLET_H
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QVector2D>
class Bullet:  public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Bullet(QGraphicsItem *parent = nullptr);
    void estimateBulletTrajectory(QGraphicsItem *origin, QGraphicsItem *target, QGraphicsScene *scene);
    void setRotationTowardTarget(QGraphicsItem *origin, QGraphicsItem *target, QGraphicsScene *scene);
    void update();

    QRectF boundingRect() const override;
    void advance(int phase) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
private:
    QRectF rect;

    QVector2D pos;
    QVector2D vel;
    QVector2D acc;

    float orginWidth;
    float orginHight;
};

#endif // BULLET_H
