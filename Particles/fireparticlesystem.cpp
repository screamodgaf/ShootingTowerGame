#include "fireparticlesystem.h"
#include "fireparticle.h"
FireParticleSystem::FireParticleSystem(QGraphicsScene *scene, QPointF &origin): ParticleSystem(scene, origin)
{

}

void FireParticleSystem::addParticle()
{

    for (int i = 0; i < 5; ++i) {
        FireParticle* particle = new FireParticle(m_origin);
        m_scene->addItem(particle);
        v_particles.push_back(particle);
    }
}

