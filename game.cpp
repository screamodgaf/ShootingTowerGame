#include "game.h"
#include "enemy.h"

Game::Game()
{/*qelapsedtime*/
    std::cout << "Game ctor" << "\n";
    LoadResources l; ///must be before  m_scene1 = new Level1();
    m_scene1 = new Level1(); ///QGraphicsScene has no visual appearance of its own; it only manages the items. You need to create a QGraphicsView widget to visualize the scene.

    qDebug() << "scene initialziation";
    timer = new QTimer();
    timer->setTimerType(Qt::CoarseTimer);
    timer->start(17 ); //16,66666666666667
    QObject::connect(timer, &QTimer::timeout, m_scene1, &Level1::advance);  ////Finally, we create a QTimer and connect its timeout() signal to the advance() slot of the scene. Every time the timer fires, the scene will advance one frame. We then tell the timer to fire every 1000/33 milliseconds. This will give us a frame rate of 30 frames a second

}



Game::~Game()
{
    std::cout << "Game::~Game() start" << "\n";
    timer->stop();
    timer->deleteLater();
    timer = nullptr;
//    m_scene1->deleteLater();
    delete m_scene1;
    m_scene1 = nullptr;

        std::cout << "Game::~Game() end" << "\n";
}



