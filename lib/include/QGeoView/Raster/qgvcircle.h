#ifndef QGVCIRCLE_H
#define QGVCIRCLE_H

#include <QGeoView/QGVDrawItem.h>

class QGV_LIB_DECL QGVCircle : public QGVDrawItem
{
    Q_OBJECT
public:
    QGVCircle();


    // QGVDrawItem interface
public:
    QPainterPath projShape() const;
    void projPaint(QPainter *painter);
};

#endif // QGVCIRCLE_H
