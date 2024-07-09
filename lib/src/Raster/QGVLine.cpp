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

QGVLine::QGVLine()
{
    setFlag(QGV::ItemFlag::IgnoreScale);
    setFlag(QGV::ItemFlag::IgnoreAzimuth);
}

void QGVLine::setGeometry(const QGV::GeoPos& start, const QGV::GeoPos& end, bool enableDirection)
{
    mStart = start;
    mEnd = end;
    mProjStart = {};
    mProjEnd = {};
    mEnableDirection = enableDirection;
    calculateGeometry();
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
    calculateGeometry();
}

QPainterPath QGVLine::projShape() const
{
    QPainterPath path;
    path.moveTo(mProjStart);
    path.lineTo(mProjEnd);
    return path;
}

void QGVLine::projPaint(QPainter* painter)
{
    if (mStart.isEmpty() || mEnd.isEmpty() || mProjStart.isNull() || mProjEnd.isNull()) {
        return;
    }

    painter->drawLine(mProjStart, mProjEnd);

    if (mEnableDirection) {
        // QGV::GeoPos direction = mEnd - mStart;
        // QGV::GeoPos normal = direction.normal();
        // QPointF arrowStart = mProjEnd;
        // QPointF arrowEnd = mProjEnd - QPointF(normal.x(), normal.y()) * 10;
        // QPointF arrowLeft = arrowEnd + QPointF(normal.y(), -normal.x()) * 5;
        // QPointF arrowRight = arrowEnd - QPointF(normal.y(), -normal.x()) * 5;
        // painter->drawLine(arrowStart, arrowLeft);
        // painter->drawLine(arrowStart, arrowRight);
    }
}

void QGVLine::calculateGeometry()
{
    if (getMap() == nullptr) {
        return;
    }

    if(!mStart.isEmpty() || !mEnd.isEmpty()) {
        mProjStart = getMap()->getProjection()->geoToProj(mStart);
        mProjEnd = getMap()->getProjection()->geoToProj(mEnd);
    }
    mProjRect = QRectF(mProjStart, mProjEnd);

    resetBoundary();
    refresh();
}


