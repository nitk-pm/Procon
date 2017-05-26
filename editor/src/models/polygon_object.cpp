#include "models/polygon_object.h"

#include <QtGui/QPainter>
#include <QtGui/QVector2D>
#include <QtCore/QDebug>
#include <QtCore/QtGlobal>
#include <QtCore/qmath.h>

PolygonObject::PolygonObject(const QPolygonF &polygon, QGraphicsItem *parent) : ObjectModel(parent) {
    setId(ObjectID::Polygon);
    setPen(QPen(Qt::black));
    setBrush(QBrush(Qt::gray));
    setZValue(1);
    setPolygon(polygon);
}

QRectF PolygonObject::boundingRect() const {
    return bounding_rect;
}

void PolygonObject::draw(QPainter *painter) {
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawPolygon(_polygon);
    if (isClicked()) {
        QPen click_pen = QPen(Qt::blue);
        click_pen.setStyle(Qt::DotLine);
        painter->setPen(click_pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());
    }
}

QPolygonF PolygonObject::polygon() const {
    return _polygon;
}

void PolygonObject::setPolygon(const QPolygonF &polygon) {
    _polygon = polygon;
    bounding_rect = _polygon.boundingRect();
}

bool PolygonObject::containsPoint(const QPointF &pos) {
    const int size = _polygon.size();
    int wn = 0;
    for (int i = 0; i < size; i++) {
        if (_polygon[i].y() <= pos.y() && _polygon[(i + 1) % size].y() > pos.y()) {
            qreal vt = (pos.y() - _polygon[i].y()) / (_polygon[(i + 1) % size].y() - _polygon[i].y());
            if (pos.x() < (_polygon[i].x() + (vt * (_polygon[(i + 1) % size].x() - _polygon[i].x())))) {
                wn++;
            }
        }
        else if (_polygon[i].y() > pos.y() && _polygon[(i + 1) % size].y() <= pos.y()) {
            qreal vt = (pos.y() - _polygon[i].y()) / (_polygon[(i + 1) % size].y() - _polygon[i].y());
            if (pos.x() < (_polygon[i].x() + (vt * (_polygon[(i + 1) % size].x() - _polygon[i].x())))) {
                wn--;
            }
        }
    }
    qDebug() << wn;
    return wn != 0;
}
