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

#include "Raster/QGVText.h"
#include "QGVMap.h"

#include <QPainter>

QGVText::QGVText()
{
    setFlag(QGV::ItemFlag::IgnoreScale);
    setFlag(QGV::ItemFlag::IgnoreAzimuth);
}

void QGVText::setGeometry(const QGV::GeoPos& geoPos, const QSizeF& textSize)
{
    mGeoPos = geoPos;
    mTextSize = textSize;
    mProjPos = {};
    mProjRect = {};
    calculateGeometry();
}

void QGVText::setText(const QString& text)
{
    mText = text;
    calculateGeometry();
}

QString QGVText::getText() const
{
    return mText;
}

QPointF QGVText::getPoint() const
{
    return mProjPos;
}

void QGVText::onProjection(QGVMap* geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    calculateGeometry();
}

QPainterPath QGVText::projShape() const
{
    QPainterPath path;
    path.addRect(mProjRect);
    return path;
}

void QGVText::projPaint(QPainter* painter)
{
    if (mGeoPos.isEmpty() || mProjRect.isEmpty()) {
        return;
    }

    painter->setPen(QPen(Qt::black));
    painter->drawText(mProjRect, mText);
}

void QGVText::calculateGeometry()
{
    if (getMap() == nullptr) {
        return;
    }

    // mProjPos = getMap()->geoToProj(mGeoPos);
    mProjRect = QRectF(mProjPos, mTextSize);
}
