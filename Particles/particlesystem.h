#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <QGraphicsScene>
#include "Particles/particle.h"
#include "repeller.h"
class Level1;

class ParticleSystem
{
public:
    ParticleSystem(Level1 *scene, QPixmap* pixmap, QPointF &origin, std::vector<ParticleSystem*>* v_particleSystem);
    virtual ~ParticleSystem();
    void run();
    virtual void addParticle();
    void applyForce(QVector2D& force);
    void applyReppeler(Repeller *repeller);
    void setPosition(QPointF &pos_);
protected:
    Level1* m_scene;
    QPointF* m_origin;
    std::vector<Particle*> v_particles;
    QPixmap* m_pixmap = nullptr;
    std::vector<ParticleSystem*>* m_v_particleSystem = nullptr;
};

#endif // PARTICLESYSTEM_H
