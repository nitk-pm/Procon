#include "common/vertex_object.h"
#include "common/scene.h"

#include <QtGui/QPainter>

VertexObject::VertexObject(const QPointF &pos, QGraphicsItem *parent = 0) : QGraphicsItem(parent) {
    setPos(pos);

    pen = QPen(Qt::red);
}

QRectF VertexObject::boundingRect() const {
    return QRectF(0, 0, Scene::BASE_SIZE, Scene::BASE_SIZE);
}

void VertexObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    int r = Scene::BASE_SIZE * 0.2;
    QPointF offset = QPointF(Scene::BASE_SIZE, Scene::BASE_SIZE) / 2;
    painter->setPen(pen);
    painter->drawEllipse(offset, r, r);
}
