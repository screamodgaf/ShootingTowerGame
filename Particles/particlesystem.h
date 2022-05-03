#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <QGraphicsScene>
#include "Particles/particle.h"
#include "repeller.h"

class ParticleSystem
{
public:
    ParticleSystem(QPixmap* pixmap, QPointF &origin);
    virtual ~ParticleSystem();
    void run();
    virtual void addParticle();
    void applyForce(QVector2D& force);
    void applyReppeler(Repeller *repeller);
protected:
    QPointF m_origin;
    std::vector<Particle*> v_particles;
    QPixmap* m_pixmap = nullptr;

};

#endif // PARTICLESYSTEM_H
