#ifndef BULLET_H
#define BULLET_H
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QVector2D>
#include <vector>

class Bullet: public QGraphicsItem, public QObject
{
public:
    explicit Bullet(QGraphicsItem* shooter_ = nullptr, QGraphicsItem *target_ = nullptr/*, char shooterType_*/, QPointF desiredBulletPos = {0,0});
    virtual ~Bullet();
    void estimateBulletTrajectory();
    void setRotationTowardTarget();
    void update();

    QRectF boundingRect() const override;
    void advance(int phase) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

    QPen* getPen();
    bool checkBulletsDistFromShooter();
    QGraphicsItem* getShooter();
//    char getShooterType();
private:
//    char shooterType;
    QGraphicsItem *target;
    QPen pen;
    QPen bulletPen;
    QLineF ln ;
    QLineF ln2;

    QRectF rect;
    QGraphicsItem *shooter ; //tower
    QVector2D loc;
    QVector2D vel;
    QVector2D acc;
    float d =0.f;; //delta
    float shooterWidth =0.f;
    float shooterHight =0.f;
    float targetWidth =0.f;
    float targetHight =0.f;

    QGraphicsLineItem line;



};

#endif // BULLET_H
