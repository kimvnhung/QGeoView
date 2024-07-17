#include <Raster/QGVCircle.h>

#include <QPainter>

QGVCircle::QGVCircle(const QGV::GeoPos& center, double radius,double strokeWidth, Qt::PenStyle penStyle, QColor penColor, QColor fillColor)
    : mCenter(center)
    , mRadius(radius)
    , mProjRect()
    , mProjCenter()
    , mPenColor(penColor)
    , mFillColor(fillColor)
    , mStrokeWidth(strokeWidth)
    , mPenStyle(penStyle)
{
}

void QGVCircle::setStrokeWidth(double strokeWidth)
{
    mStrokeWidth = strokeWidth;
}

void QGVCircle::setPenColor(QColor penColor)
{
    mPenColor = penColor;
    refresh();
}

void QGVCircle::setFillColor(QColor fillColor)
{
    mFillColor = fillColor;
    refresh();
}

void QGVCircle::setPenStyle(Qt::PenStyle penStyle)
{
    mPenStyle = penStyle;
    refresh();
}

QGV::GeoPos QGVCircle::getCenterPos() const
{
    return mCenter;
}

double QGVCircle::getRadius() const
{
    return mRadius;
}

double QGVCircle::getStrokeWidth() const
{
    return mStrokeWidth;
}

QColor QGVCircle::getPenColor() const
{
    return mPenColor;
}

QColor QGVCircle::getFillColor() const
{
    return mFillColor;
}

Qt::PenStyle QGVCircle::getPenStyle() const
{
    return mPenStyle;
}

void QGVCircle::onProjection(QGVMap* geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    // Find the rectangle that contains the circle
    // Convert distance mRadius in meters to angle in longtitute and latitude
    double angleInDegree = geoMap->getProjection()->geodesicDegrees(mRadius);

    QGV::GeoPos topLeft = {mCenter.latitude() + angleInDegree, mCenter.longitude() + angleInDegree};
    QGV::GeoPos bottomRight = {mCenter.latitude() - angleInDegree, mCenter.longitude() - angleInDegree};
    mProjRect = QRectF(geoMap->getProjection()->geoToProj(topLeft), geoMap->getProjection()->geoToProj(bottomRight));
    mProjCenter = geoMap->getProjection()->geoToProj(mCenter);

}

QPainterPath QGVCircle::projShape() const
{
    QPainterPath path;
    path.addEllipse(mProjRect);
    return path;
}

void QGVCircle::projPaint(QPainter* painter)
{
    painter->setPen(QPen(mPenColor, mStrokeWidth, mPenStyle));
    painter->setBrush(QBrush(mFillColor));

    painter->drawEllipse(mProjRect);
}

QPointF QGVCircle::projAnchor() const
{
    return mProjCenter;
}

QTransform QGVCircle::projTransform() const
{
    // This method is optional (needed flag is QGV::ItemFlag::Transformed).
    // Custom transformation for item.
    // In this case we rotate item by 45 degree.

    return QGV::createTransfromAzimuth(projAnchor(), 45);
}

QString QGVCircle::projTooltip(const QPointF& projPos) const
{
    // This method is optional (when empty return then no tooltip).
    // Text for mouse tool tip.

    return "";
}

void QGVCircle::projOnMouseClick(const QPointF& projPos)
{
  // This method is optional (needed flag is QGV::ItemFlag::Clickable).
  // Custom reaction to item single mouse click.
  // To avoid collision with item selection this code applies only if item selection disabled.
  // In this case we change opacity for item.


}

void QGVCircle::projOnMouseDoubleClick(const QPointF& projPos)
{
  // This method is optional (needed flag is QGV::ItemFlag::Clickable).
  // Custom reaction to item double mouse click.
  // In this case we change color for item.

}

void QGVCircle::projOnObjectStartMove(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to item move start.
    // In this case we only log message.

    qInfo() << "object move started at" << projPos;
}

void QGVCircle::projOnObjectMovePos(const QPointF& projPos)
{
  // This method is optional (needed flag is QGV::ItemFlag::Movable).
  // Custom reaction to mouse pos change when item move is started.
  // In this case actually changing location of object.

}

void QGVCircle::projOnObjectStopMove(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to item move finished.
    // In this case we only log message.

    qInfo() << "object move stopped" << projPos;
}

