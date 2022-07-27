#ifndef PLAYERDEFENCES_H
#define PLAYERDEFENCES_H
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QProgressBar>
class Bullet;

class PlayerDefences
{
public:
    PlayerDefences(QGraphicsScene* scene, QGraphicsPixmapItem *parent, std::vector<Bullet*>* v_bullets);
    ~PlayerDefences();
//    void checkCollisions(QGraphicsItem *item);
    void setActualLife(QGraphicsItem *item);
    void setActualLife();
    void checkCollisions(QGraphicsItem* item);

private:
    std::vector<Bullet*>* m_v_bullets = nullptr;
    QGraphicsScene* m_scene = nullptr;
    QGraphicsView* m_view = nullptr;
    QProgressBar* lifeBar  = nullptr;
    int life;
};

#endif // PLAYERDEFENCES_H
