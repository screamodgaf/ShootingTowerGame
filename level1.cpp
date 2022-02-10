#include "level1.h"
#include <iostream>
#include <QDebug>
#include <QTimer>

Level1::Level1()
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

    this->addItem(tower);
    //    this->addItem(enemy);

    this->addItem(player);


}



void Level1::advance()
{
    std::cout << "Level1::advance()" << "\n";
    for (int i = 0; i < v_bullets.size(); ++i) {
        v_bullets[i]->update();
    }

    player->advance(1);

    if(tower->checkCollisionsWithAttackArea(player)){

        createBullet();
    }

    update(sceneRect); ///so items dont leave any artifacts though works without it when using m_view->viewport()->repaint();
    m_view->viewport()->repaint();
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
