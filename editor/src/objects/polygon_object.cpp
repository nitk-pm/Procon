#include "objects/polygon_object.h"

PolygonObject::PolygonObject(const QPolygonF &polygon, QGraphicsItem *parent) : QGraphicsPolygonItem(polygon, parent) {
    setPen(QPen(Qt::black));
    setBrush(QBrush(Qt::gray));
}

void PolygonObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsPolygonItem::paint(painter, option, widget);
}
