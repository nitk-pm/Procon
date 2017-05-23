#include "models/polygon_object.h"

#include <QtGui/QPainter>

PolygonObject::PolygonObject(const QPolygonF &polygon, QGraphicsItem *parent) : ObjectModel(parent) {
    setPen(QPen(Qt::black));
    setBrush(QBrush(Qt::gray));
    setPolygon(polygon);
}

QRectF PolygonObject::boundingRect() const {
    return bounding_rect;
}

void PolygonObject::draw(QPainter *painter) {
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawPolygon(_polygon);
}

void PolygonObject::drawClicked(QPainter *painter) {

}

QPolygonF PolygonObject::polygon() const {
    return _polygon;
}

void PolygonObject::setPolygon(const QPolygonF &polygon) {
    _polygon = polygon;
    bounding_rect = _polygon.boundingRect();
}
