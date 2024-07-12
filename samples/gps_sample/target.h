#ifndef TARGET_H
#define TARGET_H

#include <QGeoView/QGVDrawItem.h>


class Target : public QGVDrawItem
{
    Q_OBJECT
public:
    enum class Type {
        UNKNOWN,
        HISTORY,
        CURRENT
    };

    explicit Target(const QGV::GeoPos& geoPos, Type type = Type::HISTORY);

    QPointF getPoint() const;
    QGV::GeoPos getGeoPos() const;

protected:
    void onProjection(QGVMap* geoMap) override;
    QPainterPath projShape() const override;
    void projPaint(QPainter* painter) override;

private:
    void calculateGeometry();

private:
    QGV::GeoPos mGeoPos;
    QPointF mProjPos;
    QSizeF mPointSize;
    QRectF mProjRect;
    QBrush mColor;

};

#endif // TARGET_H
