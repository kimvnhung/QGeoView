/***************************************************************************
 * QGeoView is a Qt / C ++ widget for visualizing geographic data.
 * Copyright (C) 2018-2024 Andrey Yaroshenko.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, see https://www.gnu.org/licenses.
 ****************************************************************************/


#include "Raster/QGVLine.h"

#include "QGVMap.h"

#include <QPainter>

QGVLine::QGVLine(const QGV::GeoPos& start, const QGV::GeoPos& end, double lineSize, bool enableDirection, QColor color)
    : mStart(start)
    , mEnd(end)
    , mProjRect()
    , mProjStart()
    , mProjEnd()
    , mColor(color)
    , mLineSize(lineSize)
    , mEnableDirection(enableDirection)
{
}


void QGVLine::setGeometry(const QGV::GeoPos& start, const QGV::GeoPos& end)
{
    mStart = start;
    mEnd = end;
    mProjStart = {};
    mProjEnd = {};
    mProjRect = {};
    onProjection(getMap());
}

void QGVLine::setStart(const QGV::GeoPos& start)
{
    mStart = start;
    mProjStart = {};
    mProjRect = {};
    onProjection(getMap());
}

void QGVLine::setEnd(const QGV::GeoPos& end)
{
    mEnd = end;
    mProjEnd = {};
    mProjRect = {};
    onProjection(getMap());
}

void QGVLine::setLineSize(double lineSize)
{
    mLineSize = lineSize;
    onProjection(getMap());
}

void QGVLine::setEnableDirection(bool enableDirection)
{
    mEnableDirection = enableDirection;
    onProjection(getMap());
}

void QGVLine::setColor(QColor color)
{
    mColor = color;
    refresh();
}

QGV::GeoPos QGVLine::getStart() const
{
    return mStart;
}

QGV::GeoPos QGVLine::getEnd() const
{
    return mEnd;
}

void QGVLine::onProjection(QGVMap* geoMap)
{
    if(geoMap == NULL)
        return;

    QGVDrawItem::onProjection(geoMap);
    calculateGeometry();
}

QPainterPath QGVLine::projShape() const
{
    QPainterPath path;

    // Draw line from mProjStart to mProjEnd with thickness of mLineSize
    path.addRect(mProjRect);

    return path;
}

void QGVLine::projPaint(QPainter* painter)
{
    if (mStart.isEmpty() || mEnd.isEmpty() || mProjStart.isNull() || mProjEnd.isNull()) {
        return;
    }

    QPen pen = QPen(QBrush(mColor), mLineSize);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawRect(mProjRect);
    painter->drawLine(mProjStart, mProjEnd);
    painter->drawEllipse(mProjStart.x(),mProjStart.y(),10,10);
    // painter->drawEllipse(mProjEnd.toPoint(),10,10);
    painter->drawEllipse(projAnchor(),20,20);

}

QPointF QGVLine::projAnchor() const
{
    // This method is optional (needed flag is QGV::ItemFlag::Transformed).
    // In this case we will use center of item as base

    return mProjRect.center();
}

QTransform QGVLine::projTransform() const
{
    // This method is optional (needed flag is QGV::ItemFlag::Transformed).
    // Custom transformation for item.
    // In this case we rotate item by 45 degree.

    return QGV::createTransfromAzimuth(projAnchor(), 45);
}

QString QGVLine::projTooltip(const QPointF& projPos) const
{
    // This method is optional (when empty return then no tooltip).
    // Text for mouse tool tip.

    auto geo = getMap()->getProjection()->projToGeo(projPos);

    return "Rectangle with color " + mColor.name() + "\nPosition " + geo.latToString() + " " + geo.lonToString();
}

void QGVLine::projOnMouseClick(const QPointF& projPos)
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

void QGVLine::projOnMouseDoubleClick(const QPointF& projPos)
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

void QGVLine::projOnObjectStartMove(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to item move start.
    // In this case we only log message.

    qInfo() << "object move started at" << projPos;
}

void QGVLine::projOnObjectMovePos(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to mouse pos change when item move is started.
    // In this case actually changing location of object.

    auto newRect = mProjRect;
    newRect.moveCenter(projPos);

    QGV::GeoRect geoRect = getMap()->getProjection()->projToGeo(newRect);
    setGeometry(geoRect.topLeft(),geoRect.bottomRight());

    qInfo() << "object moved" << geoRect;
}

void QGVLine::projOnObjectStopMove(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to item move finished.
    // In this case we only log message.

    qInfo() << "object move stopped" << projPos;
}

void QGVLine::calculateGeometry()
{
    qDebug()<<__FUNCTION__;
    if (getMap() == nullptr) {
        return;
    }

    if(!mStart.isEmpty() && !mEnd.isEmpty()) {
        QGV::GeoPos topLeft = {std::min(mStart.latitude(),mEnd.latitude()), std::max(mStart.longitude(),mEnd.longitude())};
        QGV::GeoPos bottomRight = {std::max(mStart.latitude(),mEnd.latitude()), std::min(mStart.longitude(),mEnd.longitude())};
        mProjRect = getMap()->getProjection()->geoToProj(QGV::GeoRect(topLeft, bottomRight));
    }
    // scale mProjRect to reduce width height but keep ratio around topLeft point
    // mProjRect.setWidth(mProjRect.width()*0.005);
    // mProjRect.setHeight(mProjRect.height()*0.005);


    resetBoundary();
    refresh();
}


