#include "game.h"
#include "enemy.h"

std::vector<Bullet*> Level1::v_bullets; //declaration of static cullet container vector
std::chrono::duration<float> Level1::duration(0.f);
std::chrono::duration<float> Level1::timeElapsed(0.f);
Level1* Game::m_scene1 = nullptr;
Player* Level1::player = nullptr;
QGraphicsView* Level1::m_view = nullptr;

Game::Game(QObject *parent) : QObject{parent}
{
    m_scene1 = new Level1(); ///QGraphicsScene has no visual appearance of its own; it only manages the items. You need to create a QGraphicsView widget to visualize the scene.
    timer = new QTimer(this);

    QObject::connect(timer, &QTimer::timeout, m_scene1, &Level1::advance); ///Finally, we create a QTimer and connect its timeout() signal to the advance() slot of the scene. Every time the timer fires, the scene will advance one frame. We then tell the timer to fire every 1000/33 milliseconds. This will give us a frame rate of 30 frames a second
    timer->start(1000 /60   );
}

Level1 *Game::getScene()
{
    return m_scene1;
}

Game::~Game()
{
    std::cout << "Game::~Game()" << "\n";
    m_scene1->deleteLater();
    timer->deleteLater();
}



