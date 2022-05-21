#ifndef GAME_H
#define GAME_H
#include <QObject>
#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTimer>
#include "level1.h"
#include "loadresources.h"


class Game
{
public:
    explicit Game();
    ~Game();
private:
    Level1* m_scene1;
    QTimer* timer = nullptr;


signals:

};

#endif // GAME_H
