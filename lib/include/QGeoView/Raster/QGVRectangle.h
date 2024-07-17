#ifndef QGVRECTANGLE_H
#define QGVRECTANGLE_H

#include <QGeoView/QGVDrawItem.h>


class QGV_LIB_DECL QGVRectangle : public QGVDrawItem
{
    Q_OBJECT
public:
    QGVRectangle(const QGV::GeoRect& geoRect, QColor color = Qt::red);

    void setRect(const QGV::GeoRect& geoRect);
    QGV::GeoRect getRect() const;

private:
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
    QGV::GeoRect mGeoRect;
    QRectF mProjRect;
    QColor mColor;
};

#endif // QGVRECTANGLE_H
