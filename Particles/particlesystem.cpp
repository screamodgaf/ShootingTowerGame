#include "particlesystem.h"
#include <iostream>
#include <QDebug>
#include <QPixmap>
#include "game.h"
ParticleSystem::ParticleSystem(QPixmap *pixmap, QPointF& origin): m_origin(origin), m_pixmap(pixmap)
{
    qDebug() << "m_origin in ParticleSystem: " << m_origin;
}



void ParticleSystem::run()
{
    for (auto& i : v_particles) {
        i->update();
        if(i->is_finished())
        {
            Game::getScene()->removeItem(i);
            delete i;
            i = nullptr;
            v_particles.erase(std::remove(v_particles.begin(), v_particles.end(),
                                          i), v_particles.end());
        }
    }
//    std::cout << "v_particles: " << v_particles.size() << "\n";
}

void ParticleSystem::addParticle()
{
    for (int i = 0; i < 1; ++i) {
//        if(v_particles.size()>60 ) return;
        Particle* particle = new Particle(m_pixmap, m_origin);
        Game::getScene()->addItem(particle);
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


ParticleSystem::~ParticleSystem()
{
    std::cout << "ParticleSystem::~ParticleSystem()" << "\n";
    delete m_pixmap;
    m_pixmap = nullptr;
}
