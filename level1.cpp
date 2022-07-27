#include "level1.h"
#include <iostream>
#include <QDebug>
#include <QTimer>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QProgressBar>
#include "termcolor.hpp"

Level1::Level1()
{
    std::cout << "Level1() ctor" << "\n";
    //    sceneRect = QRectF(0,0,800,600); ///its needed for scene.update(rect) - to update and so items dont leave any artifacts
    loadResources = new LoadResources;
    this->setSceneRect(sceneRect);

    m_view = new QGraphicsView();
    //opengl start
    gl = new QOpenGLWidget(m_view);
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setOptions(QSurfaceFormat::DeprecatedFunctions);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    format.setSwapInterval(1);
    format.setSamples(4);
    //     QSurfaceFormat::setDefaultFormat(format);
    gl->setFormat(format);
    m_view->setViewport(gl);
    //opengl end


    m_view->setScene(this);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "game"));
    m_view->resize(800, 600);
    m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform  | QPainter::LosslessImageRendering | QPainter::TextAntialiasing);
    m_view->setViewportUpdateMode(QGraphicsView::NoViewportUpdate );
    m_view->setCacheMode(QGraphicsView::CacheBackground);

    m_view->updateGeometry();
    //    painter = new QPainter();


    // a blue background
    //    m_view->setBackgroundBrush(Qt::black);
    m_view->setBackgroundBrush(QPixmap(":/resources/Resources/sky.png"));
    m_view->show();
    //        m_view->showFullScreen();

    //set FPS counter stuff:
    setFPScounter();

    //set gravity:
    gravity = {0.f,0.1f};

    //initialize repeller:
    //    QVector2D rpos(170, 300);
    //    repeller = new Repeller(nullptr, &rpos);

    m_view->setSceneRect(0,0,100000000,100000000);

    //CREATE ITEMS:
    player = new Player(this,/* playerPixmap, */&v_bullets);
    player->setFlag(QGraphicsItem::ItemIsFocusable); ///only one item can respond to keyboard events
    player->setFocus();
    addItem(player);

    QPixmap enemyPixmap1 = *loadResources->getEnemyPixmap1();
    enemy = new Enemy(this, enemyPixmap1, &v_bullets, &v_particleSystem, player);
//    addItem(tower);

    //    playerPixmap = new QPixmap("E:\\Qt_workspace\\ShootingTower\\ship1.png");



    timer = new QElapsedTimer;



    repeller = new Repeller(nullptr, player);
    addItem(repeller);
    std::cout << "Level1 ctor end" << "\n";
}


void Level1::advance()
{


    countFPS();

    //tower->moveBy(2,0);
    if(player)
        player->advance(1);

    if(enemy)
        enemy->advance(1);
    if(m_view)
        m_view->centerOn(player);

    //    m_view->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    //    m_view->translate(player->x() + player->boundingRect().center().x(),
    //                 player->y() + player->boundingRect().center().y()
    //                 );

    //m_view-> translate(player->x(), player->y());
    //    mScene->setX(int(-(pos.x() - width() / 2)));
    //    mScene->setY(int(-(pos.y() - height() / 2)));
    //    playerWeapons->advance(1); ///its outside player, cause if it was inside Player class, than would be of type Player, and would extand the player;s collision area
    //    QVector2D playerPos(player->pos().x(), player->pos().y());
    //    repeller->update(playerPos);

    //TODO
    for (int i = 0; i < v_bullets.size(); ++i) {
        if(v_bullets[i]!= nullptr && v_bullets[i]->checkBulletsDistFromShooter()){ //if certain bullet is too far the shooter - erase:

            delete v_bullets[i] ;
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



    if(enemy && enemy->checkEnemyLifeandPos().first ==0){

//        qDebug() << "Level1 if(tower && tower->checkTowerLifeandPos().first ==0)";
        bonusNebulaPixmap = loadResources->getBonusNebula1Pixmap();
        *bonusNebulaPixmap = bonusNebulaPixmap->scaled(QSize(140,140));

        QPointF originNebula = enemy->checkEnemyLifeandPos().second;
//         QPointF origin = tower->pos();
//        QPointF origin = tower->mapToScene(tower->checkTowerLifeandPos().second);


//        std::cout << termcolor::red << "at destruction: " << originNebula.x() <<  " " << originNebula.y() << termcolor::reset<< "\n";
        nebulaSystem = new ParticleSystem(this, bonusNebulaPixmap, originNebula);
        v_particleSystem.push_back(nebulaSystem);


        this->removeItem(enemy);
        delete enemy  ;
        enemy = nullptr;



        for (int i = 0; i < v_bullets.size(); ++i) {

            this->removeItem(v_bullets.at(i));
//            std::cout << "44" << "\n";
            //                getBulletContainer()->erase(std::remove(getBulletContainer()->begin(),
            //                                                        getBulletContainer()->end(), getBulletContainer()->at(i)), getBulletContainer()->end());
            //                     getBulletContainer()->at(i)->deleteLater();

        }
        v_bullets.clear();
    }



    //        update(sceneRect);
    ///so items dont leave any artifacts though works without it when using m_view->viewport()->repaint();
    update();

    //        m_view->viewport()->repaint();
    //    player.passDelta(duration);countFPS();

}



float Level1::getDelta()
{
    //    deltatime = 0.016;
    return deltatime;
}



void Level1::countFPS()
{
    //klatki?
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    start = std::chrono::high_resolution_clock::now();
    //    std::cout << "FPS: " <<  1/duration.count()  << "\n"; //1 second =    1000 milliseconds
    fpsLabel->setText("FPS: " + QString::number(int(1/duration.count())));

    ///if framerate is faster than 0.016 - wait untill it reaches 0.0163463 by entering while loop; "duration" in the the while loops condition insreases every iteration of the while iteration by the time the while iteration takes
    auto tstart = std::chrono::high_resolution_clock::now();  ///10
    //    std::cout << "duration.count(): " << duration.count() << "\n";
    while (duration.count() <  0.017 ) {   ///60 fps = 16 miliseconds cause 60*16 = 1000 so 0,016  sec = 16 miliseconds
        auto tend = std::chrono::high_resolution_clock::now(); ///20
        duration += tend-tstart; ///20 - 10 = 10 so while iteration took 10 | then we add the while iteration duration to framerate duration

    }
    deltatime = duration.count();
    //     std::cout << "deltatime: " << deltatime << "\n";
}

//void Level1::countFPS()
//{
//    //klatki?
//    timer->start();


//   qint64 duration =  timer->elapsed();
//   fpsLabel->setText("FPS: " + QString::number(int(1/duration)));
//    while (duration <  0.015) {   ///60 fps = 16 miliseconds cause 60*16 = 1000 so 0,016  sec = 16 miliseconds

//    }
//    deltatime = duration;
//    // std::cout << "deltatime: " << deltatime << "\n";
//}





void Level1::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key::Key_B){
        enemy->setLifeToZero();
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
    std::cout << " ~Level1() start" << "\n";
    //    delete player;
    //    player = nullptr;

    m_view->setViewport(nullptr);

    delete enemy;
    enemy = nullptr;

    delete repeller;
    repeller = nullptr;

    delete particleSystem2;
    particleSystem2 = nullptr;

    delete fireParticleSystem;
    fireParticleSystem = nullptr;
   std::cout << " ~Level1(() end mid" << "\n";
//    delete bonusNebula1;
//    bonusNebula1 = nullptr;

//    delete bonusNebulaPixmap;
//    bonusNebulaPixmap = nullptr;

    //    gl->deleteLater(); //was causing crash at closing window
    //    gl = nullptr;


    //    m_view->deleteLater();
    //    m_view = nullptr;
    std::cout << " ~Level1() end" << "\n";
}
