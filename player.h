#ifndef PLAYER_H
#define PLAYER_H
#include <QPainter>
#include <QObject>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
class Player: public QGraphicsPixmapItem
{

public:
    explicit Player(QPixmap* pixmap/*, QObject *parent = nullptr*/);
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


    float moveX;
    float moveY;
    QVector2D desiredAcc;
        float speed;
    void addToAcceleration();
    void reduceVelX();
    void reduceVelY();
    void rotateAccordingToDirection();
    double normalize_angle( const double value );

    bool slowRotationDown();
    float map2Ranges(float value, float minVel, float maxVel, float minSpeed, float maxSpeed);
private:

    QColor color;
    bool is_moving_left ;
    bool is_moving_right;
    bool is_moving_up   ;
    bool is_moving_down ;
    float d; //delta
    int frameCounter =0;
    float s;

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


    QVector2D pos;
    QVector2D vel;
    QVector2D acc;

    float smoothAccY =0;
    float smoothAccX =0;


    friend class Level1;

signals:

};

#endif // PLAYER_H
