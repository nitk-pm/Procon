#include "models/vertex_object.h"
#include "common/scene.h"

#include <QtGui/QPainter>

VertexObject::VertexObject(const QPointF &pos, QGraphicsItem *parent) : ObjectModel(parent) {
    setId(ObjectID::Vertex);
    setPen(QPen(Qt::red));
    setZValue(3);
    setSelected(true);
    setPos(pos);
    bounding_rect = QRectF(0, 0, Scene::BASE_SIZE, Scene::BASE_SIZE);
}

QRectF VertexObject::boundingRect() const {
    return bounding_rect;
}

void VertexObject::draw(QPainter *painter) {
    int r = Scene::BASE_SIZE * 0.2;
    QPointF offset = QPointF(Scene::BASE_SIZE, Scene::BASE_SIZE) / 2;

    painter->setPen(pen());
    painter->drawEllipse(offset, r, r);
    if (isClicked()) {
        QPen click_pen = QPen(Qt::blue);
        click_pen.setStyle(Qt::DotLine);
        painter->setPen(click_pen);
        painter->drawRect(boundingRect());
    }
}
