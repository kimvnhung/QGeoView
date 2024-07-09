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

class QGV_LIB_DECL QGVText : public QGVDrawItem
{
    Q_OBJECT
public:
    QGVText();

    void setGeometry(const QGV::GeoPos& geoPos, const QSizeF& textSize = QSizeF(100,100));

    void setText(const QString& text);
    QString getText() const;

    QPointF getPoint() const;

protected:
    void onProjection(QGVMap* geoMap) override;
    QPainterPath projShape() const override;
    void projPaint(QPainter* painter) override;

private:
    void calculateGeometry();

private:
    QGV::GeoPos mGeoPos;
    QPointF mProjPos;
    QSizeF mTextSize;
    QRectF mProjRect;
    QString mText;
};

