#ifndef FIREPARTICLESYSTEM_H
#define FIREPARTICLESYSTEM_H
#include "particlesystem.h"

class FireParticleSystem: public ParticleSystem
{
public:
    FireParticleSystem(QGraphicsScene *scene, QPixmap* pixmap, QPointF &origin);
    void addParticle();
private:
    QPixmap *m_pixmap;

};

#endif // FIREPARTICLESYSTEM_H
