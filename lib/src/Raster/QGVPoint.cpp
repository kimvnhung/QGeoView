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

#include "Raster/QGVPoint.h"
#include "QGVMap.h"

#include <QPainter>

QGVPoint::QGVPoint()
{
    setFlag(QGV::ItemFlag::IgnoreScale);
    setFlag(QGV::ItemFlag::IgnoreAzimuth);
}

void QGVPoint::setGeometry(const QGV::GeoPos& geoPos, const QSizeF& pointSize, const QBrush& color)
{
    mGeoPos = geoPos;
    mProjPos = {};
    mPointSize = pointSize;
    mProjRect = {};
    mColor = color;
    calculateGeometry();
}

QPointF QGVPoint::getPoint() const
{
    return mProjPos;
}

QGV::GeoPos QGVPoint::getGeoPos() const
{
    return mGeoPos;
}

void QGVPoint::onProjection(QGVMap* geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    calculateGeometry();
}

QPainterPath QGVPoint::projShape() const
{
    QPainterPath path;
    path.addRect(mProjRect);
    return path;
}

void QGVPoint::projPaint(QPainter* painter)
{
    if (mGeoPos.isEmpty() || mProjRect.isEmpty()) {
        return;
    }

    painter->setBrush(mColor);

    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawEllipse(mProjRect);
}

void QGVPoint::calculateGeometry()
{
    qDebug()<<__FUNCTION__;
    if (getMap() == nullptr) {
        return;
    }

    if (!mGeoPos.isEmpty()) {
        mProjPos = getMap()->getProjection()->geoToProj(mGeoPos);
    }

    qDebug()<<mProjPos;
    const QSizeF baseSize = !mPointSize.isEmpty() ? mPointSize : QSizeF(1, 1);
    const QPointF baseAnchor = QPointF(baseSize.width() / 2, baseSize.height() / 2);

    mProjRect = QRectF(mProjPos - baseAnchor, baseSize);
    qDebug()<<mProjRect;
    resetBoundary();
    refresh();
}
