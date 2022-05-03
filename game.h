#ifndef GAME_H
#define GAME_H
#include <QObject>
#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTimer>
#include "level1.h"



class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
    static Level1* getScene();
    ~Game();
private:
    static  Level1* m_scene1;
    QTimer* timer = nullptr;

signals:

};

#endif // GAME_H
