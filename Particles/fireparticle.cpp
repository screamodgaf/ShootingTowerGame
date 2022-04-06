#include "fireparticle.h"
#include <QPainter>
FireParticle::FireParticle(QPixmap *pixmap, QPointF &origin): Particle(pixmap, origin)
{

}

void FireParticle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

    QColor red = Qt::red;
    if(lifespan>=0)
        red.setAlphaF(lifespan); ///qreal alpha is specified in the range 0.0-1.0.
    painter->setBrush(red);
    painter->setPen(Qt::transparent);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(rect);
    //    std::cout << "vel: " << vel.x() << "\n";
}
