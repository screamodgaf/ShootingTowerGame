#ifndef PLAYER_H
#define PLAYER_H
#include <QPainter>
#include <QObject>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "playerweapons.h"
#include "playerdefences.h"
#include "loadresources.h"
class Level1;
class Player: public QGraphicsPixmapItem
{

public:
    explicit Player(Level1 *scene, std::vector<Bullet*>* v_bullets);
    ~Player();
    PlayerWeapons* getPlayerWeapons();
protected:
    void advance(int step) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    QVector2D getVel() const;
    virtual QRectF boundingRect() const override; //pure virtual public function - When writing a custom graphics item, you must implement QGraphicsItem's two pure virtual public functions: boundingRect(), which returns an estimate of the area painted by the item, and paint(), which implements the actual painting. In addition, we reimplement the shape() and advance().

    virtual void 	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override; //pure virtual public function
    virtual QPainterPath 	shape() const override;
    void applyForce(QVector2D force);

    virtual bool contains(const QPointF &point) const override;
    virtual bool 	isObscuredBy(const QGraphicsItem *item) const override;
    virtual QPainterPath 	opaqueArea() const override;
    virtual int 	type() const override;


private:

    void spreadAccOverManyFrames();
    void reduceVelX();
    void reduceVelY();
    void rotateAccordingToDirection();
    float normalize_angle( const float value );


private:
    Level1* m_scene = nullptr;
    QImage* playerImage = nullptr;
    std::vector<Bullet*>* m_v_bullets = nullptr;
    QColor color;
    bool is_moving_left = false;
    bool is_moving_right = false;
    bool is_moving_up    = false;
    bool is_moving_down  = false;
    float d; //delta
    float s =0.f;
    float dumpingSpeed =0.f;
    float prevAngle =0.f;
    QRectF rect;
    LoadResources loadResources;
    QPainter* m_painter = nullptr;

    float moveX =0.f;
    float moveY =0.f;
    QVector2D desiredAcc;
    float speed =0.f;

    std::vector<QGraphicsItem*> v_itemsInCollisionWithPlayer;

    qreal old_x;
    qreal old_y;
    qreal oldScene_x;
    qreal oldScene_y;

    bool is_draw;
    bool is_sceneChanged;

    QRectF pastRect;
    QPainter *preCollisionPainter = nullptr;

    QPixmap* m_pixmap;


    QVector2D loc;
    QVector2D vel;
    QVector2D acc;



    float smoothAccY =0.f;
    float smoothAccX =0.f;
    QPointF desiredBulletPos = {0.f,0.f};

    PlayerDefences* playerDefences = nullptr;
    PlayerWeapons* playerWeapons = nullptr;
    friend class Level1;
    friend class PlayerWeapons;
    friend class Enemy;

signals:

};

#endif // PLAYER_H
