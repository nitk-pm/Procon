#ifndef POLYGON_OBJECT__H
#define POLYGON_OBJECT__H

#include <QtWidgets/QGraphicsPolygonItem>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QPolygonF>
#include <QtCore/QRectF>

class PolygonObject : public QGraphicsPolygonItem {
    Q_OBJECT

public:
    PolygonObject(const QPolygonF &polygon, QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif /* end of include guard: POLYGON_OBJECT__H */
