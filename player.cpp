#include "player.h"
#include <iostream>
#include <QDebug>
#include <vector>

Player::Player(QObject *parent)
    : QObject{parent}, color(Qt::green)
{
    speed = 5.f;
    is_moving_left  = false;
    is_moving_right = false;
    is_moving_up    = false;
    is_moving_down  = false;

    rect.setSize(QSizeF(30.f,30.f));


}

QRectF Player::boundingRect() const
{
    return rect;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    std::cout << "paint" << "\n";
    m_painter = painter;
    m_painter->setBrush(color);
    m_painter->drawRect(rect);
    m_painter->setRenderHint(QPainter::Antialiasing);
}

void Player::update(const QRectF &rect)
{
//    std::cout << "update" << "\n";
}


void Player::advance(int step)
{
//    std::cout << "advance(int step)" << "\n";
//    if (!step) return;

//    if(checkCollisions()==true){   ///we dont want to move the scene if there is a collision
//        std::cout << "checkCollisions()==true" << "\n";
//        return;
//    }


    float _moveby = speed ;
    //    std::cout << "_moveby: " << _moveby << "\n";
    if(_moveby > 10.f) _moveby =5.f;

    if(is_moving_up){
        this->moveBy(0,-_moveby);
    }
    else if(is_moving_down){
        this->moveBy(0,_moveby);
    }
    if(is_moving_left){
        this->moveBy(-_moveby, 0);
    }
    else if(is_moving_right){
        this->moveBy(_moveby, 0);
    }
}


void Player::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())  {
    case Qt::Key_Left :
        is_moving_left = true;
        break;
    case Qt::Key_Right :
        is_moving_right = true;
        break;
    case Qt::Key_Up :
        is_moving_up = true;
        break;
    case Qt::Key_Down :
        is_moving_down = true;
        break;
    }
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())  {
    case Qt::Key_Left :
        is_moving_left = false;
        break;
    case Qt::Key_Right :
        is_moving_right = false;
        break;
    case Qt::Key_Up :
        is_moving_up = false;
        break;
    case Qt::Key_Down :
        is_moving_down = false;
        break;
    }
}







bool Player::checkCollisions()
{
    sceneItems = collidingItems(); ///get items in collision | Returns a list of all items that collide with this item.
    return true;
}

