#ifndef ROUTELINE_H
#define ROUTELINE_H

#include <QGeoView/QGVDrawItem.h>

class RouteLine : public QGVDrawItem
{
    Q_OBJECT
public:
    enum class Type {
        UNKNOWN,
        HISTORY,
        CURRENT
    };

    explicit RouteLine(const QGV::GeoPos& startPos, const QGV::GeoPos& endPos, bool showDirection = false, Type type = Type::HISTORY);


    void setGeometry(const QGV::GeoPos& startPos, const QGV::GeoPos& endPos);
    void setDirection(bool showDirection);

    QGV::GeoPos getStartPos() const;
    QGV::GeoPos getEndPos() const;

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
    QGV::GeoPos mStartPos;
    QGV::GeoPos mEndPos;

    QPointF mProjStartPos;
    QPointF mProjEndPos;
    bool mShowDirection;
    QRectF mProjRect;
    Type mType;
};

#endif // ROUTELINE_H
