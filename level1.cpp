#include "level1.h"
#include <iostream>
#include <QDebug>
#include <QTimer>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QProgressBar>
#include "termcolor.hpp"



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

    painter = new QPainter();




    m_view->setScene(this);
//b  = new QProgressBar(m_view);
    // a blue background
    m_view->setBackgroundBrush(Qt::black);
    m_view->show();

    //create items:
    tower = new Tower(this);
    //    enemy = new Enemy;

    //create player:
    playerPixmap = new QPixmap("E:\\Qt_workspace\\ShootingTower\\ship1.png");
    //   *playerPixmap = playerPixmap->scaled(QSize(40,40 ));

    player = new Player(playerPixmap, this);
    player->setFlag(QGraphicsItem::ItemIsFocusable); ///only one item can respond to keyboard events
    player->setFocus();
    //create targetting area for player:
//    playerWeapons = new PlayerWeapons(player);

    //create bullet shooting control:
    bulletShooting = new BulletShooting(this);

    // player->setPos(player->mapToScene(30,500));
    //add to Scene
    this->addItem(tower);
    this->addItem(player);
//    this->addItem(playerWeapons);
    //set FPS counter stuff:
    setFPScounter();

    //load qPixmap:
    QPixmap* pixmap1 = new QPixmap("E:\\Qt_workspace\\ShootingTower\\smoke4.png");
    *pixmap1 = pixmap1->scaled(QSize(140,140 ));


    //create particles:
    QPointF origin1 = {100, 100};
    particleSystem = new ParticleSystem(pixmap1, origin1);
    v_particleSystem.push_back(particleSystem);

    ///add another particle system:
    QPixmap* pixmap2 = new QPixmap("E:\\Qt_workspace\\ShootingTower\\light.png");
    *pixmap2 = pixmap2->scaled(QSize(140,140 ));


    //pixmap2->setDevicePixelRatio(0.1);
    //create particles:
    QPointF origin2 = {350, 100};
    //    QPainter pix2(pixmap2);
    //    pix2.fillRect(pixmap2->rect(), QColor(100, 30, 30, 128));
    //    pix2.setCompositionMode(QPainter::CompositionMode_Overlay);

    //    pix2.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);


    //    QPixmap* pixmap3 = new QPixmap("E:\\Qt_workspace\\ShootingTower\\5.png");

    //    pix2.drawPixmap(pixmap2->rect(), *pixmap2, pixmap3->rect());

    particleSystem2 = new ParticleSystem(pixmap2, origin2);
    v_particleSystem.push_back(particleSystem2);

    //set gravity:
    gravity = {0,0.1};

    //initialize repeller:
    //    QVector2D rpos(170, 300);
    //    repeller = new Repeller(nullptr, &rpos);
    repeller = new Repeller(player, nullptr);
    this->addItem(repeller);
}


void Level1::advance()
{
//    drawBackground(painter, sceneRect);

    countFPS();
    tower->advance(1);
    player->advance(1);
//    playerWeapons->advance(1); ///its outside player, cause if it was inside Player class, than would be of type Player, and would extand the player;s collision area
    //    QVector2D playerPos(player->pos().x(), player->pos().y());
    //    repeller->update(playerPos);

//TODO
        for (int i = 0; i < v_bullets.size(); ++i) {
            if(v_bullets[i]->checkBulletsDistFromShooter()){ //if certain bullet is too far the shooter - erase:

                delete v_bullets[i];
                v_bullets[i] = nullptr;

                 v_bullets.erase(std::remove(v_bullets.begin(), v_bullets.end(), v_bullets[i]), v_bullets.end());

                continue;
            }
            v_bullets[i]->update();
        }


    for (size_t i = 0; i < v_particleSystem.size(); ++i) {

        v_particleSystem[i]->applyForce(gravity);

        QVector2D v = { (float)player->x(), (float)player->y() };
        repeller->update(v);
        //        repeller->setPos(player->x(), player->y());

        v_particleSystem[i]->applyReppeler(repeller);

        v_particleSystem[i]->addParticle();
        v_particleSystem[i]->run();
    }



    //    update(sceneRect)
    ///so items dont leave any artifacts though works without it when using m_view->viewport()->repaint();
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

std::vector<Bullet *> *Level1::getBulletContainer()
{
//    std::cout << termcolor::red << "&v_bullets1: " << &v_bullets << termcolor::reset<< "\n";
    return &v_bullets;
}

void Level1::drawBackground(QPainter *painter, const QRectF &rect)
{
     QPixmap p = QPixmap("E:\\Qt_workspace\\ShootingTower\\sky.png" );
     painter->begin(&p);
    painter->setBackground(p);
//    painter->drawPixmap(QPoint(0,0), p,  rect);
    painter->end();
}



float Level1::getDelta()
{
    return duration.count();
}

QGraphicsView* Level1::getView()
{
    return m_view;
}

Player *Level1::getPlayer()
{
    return player;
}


void Level1::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key::Key_B){
//        createBullet();
    }

    if(event->key() == Qt::Key::Key_Up || event->key() == Qt::Key::Key_Down ||
            event->key() == Qt::Key::Key_Left || event->key() == Qt::Key::Key_Right)
        player->keyPressEvent(event);
}




void Level1::setFPScounter()
{
    fpsLabel = new QLabel(m_view);
    fpsLabel->setGeometry(5,17, 60, 20); // so not only the first char appears - it must be widened
    fpsLabel->setStyleSheet("QLabel { color : lime; font-size: 15px;}");
    fpsLabel->show();

    timeElapsed = std::chrono::microseconds(0);
    start = std::chrono::high_resolution_clock::now();
}

Level1::~Level1()
{
    std::cout << "Level1::~Level1()" << "\n";
    player->deleteLater();
    m_view->deleteLater();
    delete playerPixmap;
    playerPixmap = nullptr;

//    delete playerWeapons;
//    playerWeapons = nullptr;

    delete repeller;
    repeller = nullptr;

    delete pixmapItem;
    pixmapItem = nullptr;

    delete particleSystem;
    particleSystem = nullptr;

    delete particleSystem1;
    particleSystem1 = nullptr;

    delete particleSystem2;
    particleSystem2 = nullptr;

    delete fireParticleSystem;
    fireParticleSystem = nullptr;

}
