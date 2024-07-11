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

#pragma once

#include <QGeoView/QGVDrawItem.h>

class QGV_LIB_DECL QGVLine : public QGVDrawItem
{
    Q_OBJECT
public:
    QGVLine(const QGV::GeoPos& start = {}, const QGV::GeoPos& end = {}, double lineSize = 4, bool enableDirection = false, QColor color = Qt::red);

    void setGeometry(const QGV::GeoPos& start, const QGV::GeoPos& end);
    void setStart(const QGV::GeoPos& start);
    void setEnd(const QGV::GeoPos& end);
    void setLineSize(double lineSize);
    void setEnableDirection(bool enableDirection);
    void setColor(QColor color);

    QGV::GeoPos getStart() const;
    QGV::GeoPos getEnd() const;
    double getLineSize() const;
    bool getEnableDirection() const;
    QColor getColor() const;

protected:
    void onProjection(QGVMap* geoMap) override;
    QPainterPath projShape() const override;
    void projPaint(QPainter* painter) override;
    QPointF projAnchor() const override;
    QTransform projTransform() const override;
    QString projTooltip(const QPointF& projPos) const override;
    void projOnMouseClick(const QPointF& projPos) override;
    void projOnMouseDoubleClick(const QPointF& projPos) override;
    void projOnObjectStartMove(const QPointF& projPos) override;
    void projOnObjectMovePos(const QPointF& projPos) override;
    void projOnObjectStopMove(const QPointF& projPos) override;

private:
    void calculateGeometry();

private:
    QGV::GeoPos mStart;
    QGV::GeoPos mEnd;
    QRectF mProjRect;
    QPointF mProjStart;
    QPointF mProjEnd;
    QColor mColor;
    double mLineSize;
    bool mEnableDirection;

};

