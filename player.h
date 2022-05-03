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
#include "ally.h"
class Player: public QGraphicsPixmapItem, public QObject, public Ally
{

public:
    explicit Player(QPixmap* pixmap = nullptr, QObject *parent = nullptr);
    ~Player();
protected:
    void advance(int step) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    virtual QRectF boundingRect() const override; //pure virtual public function - When writing a custom graphics item, you must implement QGraphicsItem's two pure virtual public functions: boundingRect(), which returns an estimate of the area painted by the item, and paint(), which implements the actual painting. In addition, we reimplement the shape() and advance().

    virtual void 	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override; //pure virtual public function
    virtual QPainterPath 	shape() const override;
    void applyForce(QVector2D force);

    virtual bool contains(const QPointF &point) const override;
    virtual bool 	isObscuredBy(const QGraphicsItem *item) const override;
    virtual QPainterPath 	opaqueArea() const override;
    virtual int 	type() const override;

    //    void update(const QRectF &rect = QRectF());
private:
    void checkCollisions();

//    void addToAcceleration(QVector2D desiredAcc_ = {0,0});


    float moveX =0.f;
    float moveY =0.f;
    QVector2D desiredAcc;
    float speed =0.f;
    void addToAcceleration();
    void reduceVelX();
    void reduceVelY();
    void rotateAccordingToDirection();
    double normalize_angle( const double value );

    float map2Ranges(float value, float minVel, float maxVel, float minSpeed, float maxSpeed);
private:

    QColor color;
    bool is_moving_left = false;
    bool is_moving_right = false;
    bool is_moving_up    = false;
    bool is_moving_down  = false;
    float d; //delta
    int frameCounter =0;
    float s =0.f;
    float dumpingSpeed =0.f;
    float prevAngle =0.f;
    QRectF rect;

    QPainter* m_painter = nullptr;


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

    float smoothAccY =0;
    float smoothAccX =0;
    QPointF desiredBulletPos = {0,0};

    PlayerDefences* playerDefences = nullptr;
    PlayerWeapons* playerWeapons = nullptr;
    friend class Level1;
    friend class PlayerWeapons;
signals:

};

#endif // PLAYER_H
