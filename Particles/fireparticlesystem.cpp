#include "fireparticlesystem.h"
#include "fireparticle.h"
FireParticleSystem::FireParticleSystem(QGraphicsScene *scene, QPixmap *pixmap, QPointF &origin): ParticleSystem(scene, pixmap, origin), m_pixmap(pixmap)
{

}

void FireParticleSystem::addParticle()
{

    for (int i = 0; i < 5; ++i) {
        FireParticle* particle = new FireParticle(m_pixmap, m_origin);
        m_scene->addItem(particle);
        v_particles.push_back(particle);
    }
}

