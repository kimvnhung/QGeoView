#include "target.h"

#include <QPainter>

#define DEFAULT_SIZE 16

Target::Target(const QGV::GeoPos& geoPos, Type type)
{
    setFlag(QGV::ItemFlag::IgnoreScale);
    setFlag(QGV::ItemFlag::IgnoreAzimuth);

    mGeoPos = geoPos;
    mProjPos = {};
    mPointSize = {DEFAULT_SIZE, DEFAULT_SIZE};
    mProjRect = {};
    mColor = type == Type::HISTORY ? Qt::blue : Qt::yellow;
    calculateGeometry();
}

QPointF Target::getPoint() const
{
    return mProjPos;
}

QGV::GeoPos Target::getGeoPos() const
{
    return mGeoPos;
}

void Target::onProjection(QGVMap* geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    calculateGeometry();
}

QPainterPath Target::projShape() const
{
    QPainterPath path;
    path.addRect(mProjRect);
    return path;
}

void Target::projPaint(QPainter* painter)
{
    if (mGeoPos.isEmpty() || mProjRect.isEmpty()) {
        return;
    }

    //Setting
    painter->setPen(QPen(Qt::transparent,1));
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    //Draw inner circle
    painter->setBrush(Qt::white);
    painter->drawEllipse(mProjPos,mPointSize.width()/2,mPointSize.height()/2);
    // Draw outer circle
    painter->setBrush(mColor);
    painter->drawEllipse(mProjPos,mPointSize.width()*0.3,mPointSize.height()*0.3);
}

void Target::calculateGeometry()
{
    if(getMap() == NULL)
        return;

    if(!mGeoPos.isEmpty()){
        mProjPos = getMap()->getProjection()->geoToProj(mGeoPos);
    }

    const QSizeF outerSize = !mPointSize.isEmpty() ? mPointSize : QSizeF(DEFAULT_SIZE, DEFAULT_SIZE);
    // const QSizeF innerSize = !mPointSize.isEmpty() ? mPointSize*0.8 : QSizeF(8, 8);
    const QPointF outerAnchor = QPointF(outerSize.width()/2, outerSize.height()/2);
    mProjRect = QRectF(mProjPos - outerAnchor, mPointSize);

    resetBoundary();
    refresh();
}


