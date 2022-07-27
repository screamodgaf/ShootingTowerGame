#ifndef LOADRESOURCES_H
#define LOADRESOURCES_H
#include <QPixmap>

class LoadResources
{
public:
    LoadResources();
    ~LoadResources();
    QPixmap* getFire1();
    QImage* getPlayerImage();
    QPixmap* getBonusNebula1Pixmap();
    QPixmap* getEnemyPixmap1();
private:
    QPixmap* fire1;
    QPixmap* bonusNebula1;
    QPixmap* enemyShip1Pixmap;
    QImage* playerImage;
};

#endif // LOADRESOURCES_H
