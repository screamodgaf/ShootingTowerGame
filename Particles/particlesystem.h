#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <QGraphicsScene>
#include "Particles/particle.h"
#include "repeller.h"

class ParticleSystem
{
public:
    ParticleSystem(QGraphicsScene *scene, QPointF &origin);
    void run();
    virtual void addParticle();
    void applyForce(QVector2D& force);
    void applyReppeler(Repeller *repeller);
protected:
    QGraphicsScene *m_scene;
    QPointF  m_origin;
    std::vector<Particle*> v_particles;


};

#endif // PARTICLESYSTEM_H
