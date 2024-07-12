#include "routeline.h"

#include <QPainter>
#include <QPen>

RouteLine::RouteLine(const QGV::GeoPos& startPos, const QGV::GeoPos& endPos, bool showDirection, Type type)
    :
    mStartPos(startPos),
    mEndPos(endPos),
    mShowDirection(showDirection),
    mType(type)
{

}

void RouteLine::setGeometry(const QGV::GeoPos& startPos, const QGV::GeoPos& endPos)
{
    mStartPos = startPos;
    mEndPos = endPos;

    // Geo coordinates need to be converted manually again to projection
    onProjection(getMap());

           // Now we can inform QGV about changes for this
    resetBoundary();
    refresh();
}

void RouteLine::setDirection(bool showDirection)
{
    mShowDirection = showDirection;
}

QGV::GeoPos RouteLine::getStartPos() const
{
    return mStartPos;
}

QGV::GeoPos RouteLine::getEndPos() const
{
    return mEndPos;
}

void RouteLine::onProjection(QGVMap* geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    QGV::GeoPos topLeft = {std::max(mStartPos.latitude(), mEndPos.latitude()), std::min(mStartPos.longitude(), mEndPos.longitude())};
    QGV::GeoPos bottomRight = {std::min(mStartPos.latitude(), mEndPos.latitude()), std::max(mStartPos.longitude(), mEndPos.longitude())};
    mProjStartPos = geoMap->getProjection()->geoToProj(mStartPos);
    mProjEndPos = geoMap->getProjection()->geoToProj(mEndPos);
    mProjRect = geoMap->getProjection()->geoToProj(QGV::GeoRect(topLeft,bottomRight));
}

QPainterPath RouteLine::projShape() const
{
    QPainterPath path;
    path.addRect(mProjRect);
    return path;
}

void RouteLine::projPaint(QPainter* painter)
{
    QPen pen = QPen(QBrush(Qt::red), 3);

    // Custom item highlight indicator
    if (isFlag(QGV::ItemFlag::Highlighted) && isFlag(QGV::ItemFlag::HighlightCustom)) {
        // We will use pen with bigger width
        pen = QPen(QBrush(Qt::black), 5);
    }

    if(mType == Type::CURRENT)
        pen.setStyle(Qt::DashLine);

    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawLine(QLineF(mProjStartPos,mProjEndPos));
}

QPointF RouteLine::projAnchor() const
{
    return mProjRect.center();
}

QTransform RouteLine::projTransform() const
{
    // This method is optional (needed flag is QGV::ItemFlag::Transformed).
    // Custom transformation for item.
    // In this case we rotate item by 45 degree.

    return QGV::createTransfromAzimuth(projAnchor(), 45);
}

QString RouteLine::projTooltip(const QPointF& projPos) const
{
    // This method is optional (when empty return then no tooltip).
    // Text for mouse tool tip.

    auto geo = getMap()->getProjection()->projToGeo(projPos);

    return "Line with color red\nPosition " + geo.latToString() + " " + geo.lonToString();
}

void RouteLine::projOnMouseClick(const QPointF& projPos)
{
    // This method is optional (when empty return then no action).
    // Mouse click event.
    // In this case we will change color of item to green


}

void RouteLine::projOnMouseDoubleClick(const QPointF& projPos)
{
    // This method is optional (when empty return then no action).
    // Mouse double click event.
    // In this case we will change color of item to blue

}

void RouteLine::projOnObjectStartMove(const QPointF& projPos)
{
    // This method is optional (when empty return then no action).
    // Start moving event.
    // In this case we will change color of item to yellow

}

void RouteLine::projOnObjectMovePos(const QPointF& projPos)
{
    // This method is optional (when empty return then no action).
    // Moving event.
    // In this case we will change color of item to cyan

}

void RouteLine::projOnObjectStopMove(const QPointF& projPos)
{
    // This method is optional (when empty return then no action).
    // Stop moving event.
    // In this case we will change color of item to magenta

}

