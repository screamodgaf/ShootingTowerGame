#include "particlesystem.h"
#include <iostream>
#include <QDebug>
ParticleSystem::ParticleSystem(QGraphicsScene* scene, QPointF& origin): m_scene(scene),
    m_origin(origin)
{
    qDebug() << "m_origin in ParticleSystem: " << m_origin;
 
}

void ParticleSystem::run()
{
    for (auto& i : v_particles) {
        i->update();
        if(i->is_finished())
        {
            
            m_scene->removeItem(i);
            delete i;
            v_particles.erase(std::remove(v_particles.begin(), v_particles.end(),
                                          i), v_particles.end());
            
        }
    }
//    std::cout << "v_particles: " << v_particles.size() << "\n";
}

void ParticleSystem::addParticle()
{
    
    for (int i = 0; i < 5; ++i) {
        Particle* particle = new Particle(m_origin);
        m_scene->addItem(particle);
        v_particles.push_back(particle);
    }
}

void ParticleSystem::applyForce(QVector2D &force)
{
    for (auto& i : v_particles) {
        i->applyForce(force);
    }
}

void ParticleSystem::applyReppeler(Repeller* repeller)
{
//    std::cout << "ParticleSystem::applyReppeler" << "\n";
    for (Particle* i: v_particles) {

        QVector2D repelForce = repeller->repel(i);
//        qDebug() << "repelForce: " << repelForce;

//repelForce={-1,-0.1};
        i->applyForce(repelForce);
    }
}


