#ifndef BULLET_H
#define BULLET_H
#include <QGraphicsItem>
#include <QPainter>
#include <QVector2D>
#include <vector>
class ParticleSystem;
class Level1;

class Bullet: public QGraphicsItem
{
public:
    explicit Bullet(Level1* scene, QGraphicsItem *shooter_, std::string typeOfShooter, QGraphicsItem *target_, QPointF desiredBulletPos, QVector2D vel_);

    virtual ~Bullet();
    void estimateBulletTrajectory();
    void setRotationTowardTarget();
    void update();
    std::string getTypeOfShooter();
    QRectF boundingRect() const override;
    void advance(int phase) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

    QPen* getPen();
    bool checkBulletsDistFromShooter();
    QGraphicsItem* getShooter();
//    char getShooterType();
private:
//    char shooterType;
    QGraphicsItem * target = nullptr;
    Level1* m_scene = nullptr;
    QPen pen;
    QPen bulletPen;
    QLineF ln ;
    QLineF ln2;
    float d = 0.0;
    QRectF rect;
    QGraphicsItem *shooter = nullptr; //tower
    QVector2D loc;
    QVector2D vel;
    QVector2D acc;

    float shooterWidth =0.f;
    float shooterHight =0.f;
    float targetWidth =0.f;
    float targetHight =0.f;
    QVector2D shootersVel;
    QGraphicsLineItem line;
    float scalar;

    std::string m_typeOfShooter;

};

#endif // BULLET_H
