#include "routeline.h"

#include <QPainter>
#include <QPen>

#define ARROW_SIZE 10 // in pixels

RouteLine::RouteLine(const QGV::GeoPos& startPos, const QGV::GeoPos& endPos, bool showDirection, Type type)
    :
    mStartPos(startPos),
    mEndPos(endPos),
    mProjStartPos(),
    mProjEndPos(),
    mArrowAnchor(),
    mRotateAngle(),
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

    if(mShowDirection)
    {
        // Get random visible point on section from mProjStartPos to mProjEndPos
        mArrowAnchor = QPointF((mProjStartPos.x() + mProjEndPos.x())/2,(mProjStartPos.y() + mProjEndPos.y())/2);
        // Find angle between y-axis and line from mProjStartPos to mProjEndPos
        mRotateAngle = atan((mProjStartPos.y() - mProjEndPos.y())/(mProjStartPos.x() - mProjEndPos.x()))*180/M_PI - 90;
    }
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

    if(mShowDirection)
    {
        pen.setWidth(1);
        pen.setStyle(Qt::SolidLine);
        painter->setPen(pen);
        painter->setBrush(Qt::red);
        // Calculate arrow size in coordinates
        QSizeF viewSize = getMap()->size();
        QRectF viewCoordinateRect = getMap()->getCamera().projRect();
        double arrowSizeInX = ARROW_SIZE * viewCoordinateRect.width() / viewSize.width();
        double arrowSizeInY = ARROW_SIZE * viewCoordinateRect.height() / viewSize.height();

        QPainterPath path = QPainterPath();
        path.moveTo(mArrowAnchor);
        path.lineTo(QPointF(mArrowAnchor.x() + arrowSizeInX, mArrowAnchor.y() + arrowSizeInY));
        path.lineTo(QPointF(mArrowAnchor.x() - arrowSizeInX, mArrowAnchor.y() + arrowSizeInY));
        path.lineTo(mArrowAnchor);
        path.closeSubpath();

        QTransform t;
        t.translate(mArrowAnchor.x(),mArrowAnchor.y());
        t.rotate(mRotateAngle);
        t.translate(-mArrowAnchor.x(),-mArrowAnchor.y());
        QPainterPath rotatedPath = t.map(path);

        // painter->drawPath(path);
        painter->drawPath(rotatedPath);

        qDebug()<<"****************************************";
        qDebug()<<"TopLeft : "<<mProjRect.topLeft();
        qDebug()<<"BottomRight : "<<mProjRect.bottomRight();
        qDebug()<<"mArrowAnchor : "<<mArrowAnchor;
        qDebug()<<"mRotateAngle : "<<mRotateAngle;
        qDebug()<<"mProjRect.with: "<<QString::number(mProjRect.width(),'f',2);
        // qDebug()<<"Map ratio: "<<getMap()->getProjection()->geodesicMeters()
    }
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

