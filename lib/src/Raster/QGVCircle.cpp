#include <Raster/QGVCircle.h>

#include <QPainter>

QGVCircle::QGVCircle()
{

}

QPainterPath QGVCircle::projShape() const
{
    QPainterPath path;
    path.addEllipse(QPointF(0, 0), 1, 1);
    return path;
}


void QGVCircle::projPaint(QPainter *painter)
{
    painter->drawEllipse(QPointF(0, 0), 1, 1);
}
