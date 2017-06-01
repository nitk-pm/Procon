#include "models/polygon_object.h"
#include "util/geometry.h"

#include <QtGui/QPainter>
#include <QtCore/QDebug>

PolygonObject::PolygonObject(const QPolygonF &polygon, QGraphicsItem *parent) : ObjectModel(parent) {
    setId(ObjectID::Polygon);
    setPen(QPen(Qt::black));
    setBrush(QBrush(Qt::cyan));
    setZValue(1);
    setPolygon(polygon);
}

void PolygonObject::draw(QPainter *painter) {
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawPolygon(_polygon);
}

QPolygonF PolygonObject::polygon() const {
    return _polygon;
}

void PolygonObject::setPolygon(const QPolygonF &polygon) {
    _polygon = polygon;
    setBoundingRect(_polygon.boundingRect());
}

bool PolygonObject::containsPoint(const QPointF &pos) {
    rucm::geometry::Polygon poly;
    for (auto &p : _polygon) {
        poly.add(rucm::geometry::Point(p.x(), p.y()));
    }
    return poly.containsPoint(rucm::geometry::Point(pos.x(), pos.y()));
}
