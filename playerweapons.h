#ifndef PLAYERWEAPONS_H
#define PLAYERWEAPONS_H
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include "bullet.h"
class Player;
class Level1;

class PlayerWeapons: public QGraphicsPolygonItem, public QObject
{

public:
    explicit PlayerWeapons(Level1 *scene, QPointF desiredBulletPos_, std::vector<Bullet*>* v_bullets, Player* parentPlayer,  QGraphicsPixmapItem *parent = nullptr);
    ~PlayerWeapons();
    QRectF boundingRect() const override;
    void advance(int phase) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
    bool contains(const QPointF &point) const override;
    std::vector<Bullet*>* getBulletcontainer();

signals:
private:
    void checkViewfinderCollisions();
public slots:
    void createBullet();
private:
    Level1* m_scene = nullptr;
    QRectF rect;

    QPolygonF triangle;
    std::vector<ParticleSystem*>* m_v_particleSystem = nullptr;
    float d = 0.0;
    Bullet* bullet = nullptr;
    std::vector<Bullet*>* m_v_bullets;
    QGraphicsItem* targetHit = nullptr;;
    QGraphicsPixmapItem *parent = nullptr;
    QPointF desiredBulletPos;
    Player* m_parentPlayer  = nullptr;
};

#endif // PLAYERWEAPONS_H
