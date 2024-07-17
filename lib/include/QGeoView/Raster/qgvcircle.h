#ifndef QGVCIRCLE_H
#define QGVCIRCLE_H

#include <QGeoView/QGVDrawItem.h>

class QGV_LIB_DECL QGVCircle : public QGVDrawItem
{
    Q_OBJECT
public:
    // centerPos: center of the circle
    // radius: radius of the circle in meters
    QGVCircle(const QGV::GeoPos &centerPos, double radius = 0,double strokeWidth = 1,Qt::PenStyle penStyle = Qt::SolidLine ,QColor penColor = Qt::transparent ,QColor fillColor = Qt::red);

    void setStrokeWidth(double strokeWidth);
    void setPenColor(QColor penColor);
    void setFillColor(QColor fillColor);
    void setPenStyle(Qt::PenStyle penStyle);

    QGV::GeoPos getCenterPos() const;
    double getRadius() const;
    double getStrokeWidth() const;
    QColor getPenColor() const;
    QColor getFillColor() const;
    Qt::PenStyle getPenStyle() const;

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
    QGV::GeoPos mCenter;
    double mRadius;
    QRectF mProjRect;
    QPointF mProjCenter;
    QColor mPenColor;
    QColor mFillColor;
    double mStrokeWidth;
    Qt::PenStyle mPenStyle;

};

#endif // QGVCIRCLE_H
