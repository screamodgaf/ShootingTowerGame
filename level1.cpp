#include "level1.h"
#include <iostream>
#include <QDebug>
#include <QTimer>

Level1::Level1( )
 {
    std::cout << "Level1() ctor" << "\n";
    sceneRect = QRectF(0,0,800,600); ///its needed for scene.update(rect) - to update and so items dont leave any artifacts
    this->setSceneRect(sceneRect);

    m_view = new QGraphicsView(this);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "game"));
    m_view->resize(800, 600);
    m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
 m_view->setViewportUpdateMode(QGraphicsView::NoViewportUpdate );

    m_view->setScene(this);

    // a blue background
    m_view->setBackgroundBrush(Qt::black);
    m_view->show();

    //create items:
    tower = new Tower(this);
    //    enemy = new Enemy;

    player = new Player;
    player->setFlag(QGraphicsItem::ItemIsFocusable); ///only one item can respond to keyboard events
    player->setFocus();

    // player->setPos(player->mapToScene(30,500));
    //add to Scene
    this->addItem(tower);
    this->addItem(player);
    //set FPS counter stuff:
    setFPScounter();

    //periodic check if enemy within tower so it shoots every second and not every every frame:
    QTimer* towerAreaTimer = new QTimer();
    connect(towerAreaTimer, &QTimer::timeout, this,
            &Level1::checkTowersAreaPeriodicly);
    towerAreaTimer->start(700);
}


void Level1::advance()
{
    countFPS();
    std::cout << "Level1::advance()" << "\n";
    for (int i = 0; i < v_bullets.size(); ++i) {
        v_bullets[i]->update();
    }
    player->advance(1);


    update(sceneRect); ///so items dont leave any artifacts though works without it when using m_view->viewport()->repaint();
    m_view->viewport()->repaint();
//    player.passDelta(duration);countFPS();
}


void Level1::countFPS()
{
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    start = std::chrono::high_resolution_clock::now();
    //    std::cout << "FPS: " <<  1/duration.count()  << "\n"; //1 second =    1000 milliseconds
    fpsLabel->setText("FPS: " + QString::number(int(1/duration.count())));

    ///if framerate is faster than 0.016 - wait untill it reaches 0.0163463 by entering while loop; "duration" in the the while loops condition insreases every iteration of the while iteration by the time the while iteration takes
    auto tstart = std::chrono::high_resolution_clock::now();  ///10
    while (duration.count() <  0.016) {   ///60 fps = 16 miliseconds cause 60*16 = 1000 so 0,016  sec = 16 miliseconds
        auto tend = std::chrono::high_resolution_clock::now(); ///20
        duration += tend-tstart; ///20 - 10 = 10 so while iteration took 10 | then we add the while iteration duration to framerate duration
    }

}

void Level1::checkTowersAreaPeriodicly()
{
    if(tower->checkCollisionsWithAttackArea(player)){
        createBullet();
    }

}


void Level1::createBullet()
{
    bullet = new Bullet(tower);
    v_bullets.push_back(bullet);
    this->addItem(bullet);
    std::cout << "void Level1::keyPressEvent(QKeyEvent *event)" << "\n";
    bullet->estimateBulletTrajectory(tower, player, this);
    bullet->setRotationTowardTarget(tower, player, this);
    update(sceneRect); ///so items dont leave any artifacts though works without it when using m_view->viewport()->repaint();

}


void Level1::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key::Key_B){
       createBullet();
    }

    if(event->key() == Qt::Key::Key_Up || event->key() == Qt::Key::Key_Down ||
            event->key() == Qt::Key::Key_Left || event->key() == Qt::Key::Key_Right)
        player->keyPressEvent(event);


}

void Level1::setFPScounter()
{
    fpsLabel = new QLabel(m_view);
    fpsLabel->setGeometry(5,5, 60, 20); // so not only the first char appears - it must be widened
    fpsLabel->setStyleSheet("QLabel { color : lime; font-size: 15px;}");
    fpsLabel->show();

    timeElapsed = std::chrono::microseconds(0);
    start = std::chrono::high_resolution_clock::now();
}
