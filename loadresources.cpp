#include "loadresources.h"
#include <iostream>
LoadResources::LoadResources()
{
    std::cout << "LoadResources() ctor" << "\n";

    fire1 = new QPixmap(":/resources/Resources/fire1.png");
    bonusNebula1 = new QPixmap(":/resources/Resources/bonusNebula1.png");
    playerImage = new QImage(":/resources/Resources/ship1.png");
    enemyShip1Pixmap = new QPixmap(":/resources/Resources/cardassianshipc.png");
}


QPixmap* LoadResources::getFire1()
{
    if(fire1 == nullptr) std::cout << "fire1 == nullpointer" << "\n";
    return fire1;
}

QImage *LoadResources::getPlayerImage()
{
    return playerImage;
}

QPixmap *LoadResources::getBonusNebula1Pixmap()
{
    return  bonusNebula1;
}

QPixmap *LoadResources::getEnemyPixmap1()
{
    return enemyShip1Pixmap;
}

LoadResources::~LoadResources()
{
    std::cout << "~LoadResources()" << "\n";
}

