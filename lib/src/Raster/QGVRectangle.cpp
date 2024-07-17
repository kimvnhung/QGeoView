#include <Raster/QGVRectangle.h>

#include <QBrush>
#include <QPainter>
#include <QPen>

QGVRectangle::QGVRectangle(const QGV::GeoRect& geoRect, QColor color)
    : mGeoRect(geoRect)
    , mColor(color)
{
}

void QGVRectangle::setRect(const QGV::GeoRect& geoRect)
{
    mGeoRect = geoRect;

           // Geo coordinates need to be converted manually again to projection
    onProjection(getMap());

           // Now we can inform QGV about changes for this
    resetBoundary();
    refresh();
}

QGV::GeoRect QGVRectangle::getRect() const
{
    return mGeoRect;
}

void QGVRectangle::onProjection(QGVMap* geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    mProjRect = geoMap->getProjection()->geoToProj(mGeoRect);
}

QPainterPath QGVRectangle::projShape() const
{
    QPainterPath path;
    path.addRect(mProjRect);
    return path;
}

void QGVRectangle::projPaint(QPainter* painter)
{
    QPen pen = QPen(QBrush(Qt::transparent), 1);

           // Custom item highlight indicator
    if (isFlag(QGV::ItemFlag::Highlighted) && isFlag(QGV::ItemFlag::HighlightCustom)) {
        // We will use pen with bigger width
        pen = QPen(QBrush(Qt::black), 5);
    }

    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(QBrush(mColor));
    painter->drawRect(mProjRect);

           // Custom item select indicator
    if (isSelected() && isFlag(QGV::ItemFlag::SelectCustom)) {
        // We will draw additional rect around our item
        painter->drawLine(mProjRect.topLeft(), mProjRect.bottomRight());
        painter->drawLine(mProjRect.topRight(), mProjRect.bottomLeft());
    }
}

QPointF QGVRectangle::projAnchor() const
{
    // This method is optional (needed flag is QGV::ItemFlag::Transformed).
    // In this case we will use center of item as base

    return mProjRect.center();
}

QTransform QGVRectangle::projTransform() const
{
    // This method is optional (needed flag is QGV::ItemFlag::Transformed).
    // Custom transformation for item.
    // In this case we rotate item by 45 degree.

    return QGV::createTransfromAzimuth(projAnchor(), 45);
}

QString QGVRectangle::projTooltip(const QPointF& projPos) const
{
    // This method is optional (when empty return then no tooltip).
    // Text for mouse tool tip.

    auto geo = getMap()->getProjection()->projToGeo(projPos);

    return "Line with color " + mColor.name() + "\nPosition " + geo.latToString() + " " + geo.lonToString();
}

void QGVRectangle::projOnMouseClick(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Clickable).
    // Custom reaction to item single mouse click.
    // To avoid collision with item selection this code applies only if item selection disabled.
    // In this case we change opacity for item.

    if (!isSelectable()) {
        if (getOpacity() <= 0.5)
            setOpacity(1.0);
        else
            setOpacity(0.5);

        qInfo() << "single click" << projPos;
    } else {
        setOpacity(1.0);
    }
}

void QGVRectangle::projOnMouseDoubleClick(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Clickable).
    // Custom reaction to item double mouse click.
    // In this case we change color for item.

    const QList<QColor> colors = { Qt::red, Qt::blue, Qt::green, Qt::gray, Qt::cyan, Qt::magenta, Qt::yellow };

    const auto iter =
            std::find_if(colors.begin(), colors.end(), [this](const QColor& color) { return color == mColor; });
    mColor = colors[(iter - colors.begin() + 1) % colors.size()];
    repaint();

    setOpacity(1.0);

    qInfo() << "double click" << projPos;
}

void QGVRectangle::projOnObjectStartMove(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to item move start.
    // In this case we only log message.

    qInfo() << "object move started at" << projPos;
}

void QGVRectangle::projOnObjectMovePos(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to mouse pos change when item move is started.
    // In this case actually changing location of object.

    auto newRect = mProjRect;
    newRect.moveCenter(projPos);

    setRect(getMap()->getProjection()->projToGeo(newRect));

    qInfo() << "object moved" << mGeoRect;
}

void QGVRectangle::projOnObjectStopMove(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to item move finished.
    // In this case we only log message.

    qInfo() << "object move stopped" << projPos;
}
