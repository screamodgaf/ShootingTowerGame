#ifndef PARTICLE_H
#define PARTICLE_H

#include <QGraphicsItem>
#include <QObject>
#include <QVector2D>
class Particle : public QGraphicsItem
{

public:
    Particle(QPointF& origin);
    ~Particle();
    void update();
    QRectF boundingRect() const override;
    void advance(int phase) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
    bool is_finished();
    void applyForce(QVector2D& force);
protected:
    QVector2D pos;
    QVector2D vel;
    QVector2D acc;
    float lifespan;

    QRectF rect;
    QPointF m_origin;
};

#endif // PARTICLE_H
