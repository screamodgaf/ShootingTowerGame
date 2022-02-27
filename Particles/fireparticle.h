#ifndef FIREPARTICLE_H
#define FIREPARTICLE_H
#include "particle.h"
#include <QPointF>

class FireParticle: public Particle
{
public:
    FireParticle(QPointF& origin);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

};

#endif // FIREPARTICLE_H
