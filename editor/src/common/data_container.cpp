#include "common/data_container.h"
#include "common/scene.h"

#include <QtWidgets/QGraphicsPolygonItem>
#include <QtCore/QtGlobal>
#include <QtCore/QDebug>

DataContainer::DataContainer(Scene *scene, int width, int height) {
    set(scene, width, height);
}

void DataContainer::set(Scene *scene, int width, int height) {
    _scene      = scene;
    _width      = width;
    _height     = height;
    _background = _scene->createBackground(_width, _height);
}

Scene* DataContainer::scene() const {
    return _scene;
}

QGraphicsPixmapItem* DataContainer::background() const {
    return _background;
}

int DataContainer::width() const {
    return _width;
}

int DataContainer::height() const {
    return _height;
}

QPoint DataContainer::convertRealPosToVirtualPos(const QPointF &pos) {
    QPointF p = _background->mapFromScene(pos) / BASE_SIZE;
    return std::move(QPoint(p.x(), p.y()));
}

QPointF DataContainer::convertVirtualPosToRealPos(const QPoint &pos) {
    QPoint p = pos * BASE_SIZE;
    return std::move(_background->mapToScene(pos * BASE_SIZE));
}

QPointF DataContainer::convertVirtualPosToRealPosCenter(const QPoint &pos) {
    return std::move(convertVirtualPosToRealPos(pos + QPoint(BASE_SIZE, BASE_SIZE) / 2));
}

QPointF DataContainer::modifiyPosCenter(const QPointF &pos) {
    QPoint p = convertRealPosToVirtualPos(pos);
    return convertVirtualPosToRealPosCenter(p);
}

QGraphicsEllipseItem* DataContainer::getVertex(const QPoint &pos) const {
    QGraphicsEllipseItem *item  = 0;

    if (vertex_map.contains(pos)) {
        item = vertex_map[pos];
    }

    return item;
}

QGraphicsEllipseItem* DataContainer::addVertex(const QPoint &pos) {
    if (vertex_map.contains(pos)) {
        qWarning("this position has already vertex");
        return 0;
    }

    QRectF rect = QRectF(0, 0, RECT_SIZE, RECT_SIZE);
    auto   item = _scene->addEllipse(rect, QPen(Qt::red));
    item->setPos(convertVirtualPosToRealPos(pos));
    item->setData(0, pos);
    vertex_map.insert(pos, item);

    return item;
}

void DataContainer::removeVertex(const QPoint &pos) {
    if (!vertex_map.contains(pos)) {
        qWarning("this position has not exist vertex");
        return;
    }

    auto item = vertex_map[pos];
    _scene->removeItem(item);
    vertex_map.remove(pos);
}

bool DataContainer::containsVertex(const QPoint &pos) const {
    return vertex_map.contains(pos);
}

QGraphicsPolygonItem* DataContainer::getPolygon(int id) const {
    QGraphicsPolygonItem *item = 0;

    if (polygon_list.contains(id)) {
        item = polygon_list[id];
    }

    return item;
}

int DataContainer::addPolygon(const QPolygonF &polygon) {
    auto item = _scene->addPolygon(polygon, QPen(Qt::black), QBrush(Qt::gray));
    int  id   = qrand();

    polygon_list.insert(id, item);

    return id;
}

void DataContainer::removePolygon(int id) {
    if (!polygon_list.contains(id)) {
        qWarning("this id has not exist polygon");
        return;
    }

    auto item = polygon_list[id];
    _scene->removeItem(item);
    polygon_list.remove(id);
}
