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

QGVLine::QGVLine(const QGV::GeoPos& start, const QGV::GeoPos& end, double lineSize, Qt::PenStyle penStyle, QColor color)
    : mStart(start)
    , mEnd(end)
    , mProjRect()
    , mProjStart()
    , mProjEnd()
    , mColor(color)
    , mLineSize(lineSize)
    , mPenStyle(penStyle)
{
}


void QGVLine::setGeometry(const QGV::GeoPos& start, const QGV::GeoPos& end)
{
    mStart = start;
    mEnd = end;
    // Geo coordinates need to be converted manually again to projection
    onProjection(getMap());

           // Now we can inform QGV about changes for this
    resetBoundary();
    refresh();
}

void QGVLine::setLineSize(double lineSize)
{
    mLineSize = lineSize;
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
    QGVDrawItem::onProjection(geoMap);
    QGV::GeoPos topLeft = {std::max(mStart.latitude(), mEnd.latitude()), std::min(mStart.longitude(), mEnd.longitude())};
    QGV::GeoPos bottomRight = {std::min(mStart.latitude(), mEnd.latitude()), std::max(mStart.longitude(), mEnd.longitude())};
    mProjStart = geoMap->getProjection()->geoToProj(mStart);
    mProjEnd = geoMap->getProjection()->geoToProj(mEnd);
    mProjRect = geoMap->getProjection()->geoToProj(QGV::GeoRect(topLeft,bottomRight));
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
    QPen pen = QPen(QBrush(Qt::red), 3);

           // Custom item highlight indicator
    if (isFlag(QGV::ItemFlag::Highlighted) && isFlag(QGV::ItemFlag::HighlightCustom)) {
        // We will use pen with bigger width
        pen = QPen(QBrush(Qt::black), 5);
    }

    pen.setStyle(mPenStyle);

    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawLine(QLineF(mProjStart,mProjEnd));

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


}

void QGVLine::projOnMouseDoubleClick(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Clickable).
    // Custom reaction to item double mouse click.
    // In this case we change color for item.

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

}

void QGVLine::projOnObjectStopMove(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to item move finished.
    // In this case we only log message.

    qInfo() << "object move stopped" << projPos;
}


