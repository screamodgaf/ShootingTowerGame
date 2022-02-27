#ifndef FIREPARTICLESYSTEM_H
#define FIREPARTICLESYSTEM_H
#include "particlesystem.h"

class FireParticleSystem: public ParticleSystem
{
public:
    FireParticleSystem(QGraphicsScene *scene, QPointF &origin);
    void addParticle();

};

#endif // FIREPARTICLESYSTEM_H
