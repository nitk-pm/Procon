#include "common/data_container.h"
#include "common/scene.h"

#include <QtWidgets/QGraphicsPolygonItem>
#include <QtCore/QtGlobal>
#include <QtCore/QDebug>

DataContainer::DataContainer(Scene *scene, int width, int height) {
    set(scene, width, height);
}

void DataContainer::set(Scene *scene, int width, int height) {
    _scene  = scene;
    _width  = width;
    _height = height;

    _scene->createBackground(_width, _height);
}

Scene* DataContainer::scene() const {
    return _scene;
}

int DataContainer::width() const {
    return _width;
}

int DataContainer::height() const {
    return _height;
}

int DataContainer::convertPosToIndex(const QPointF &pos) const {
    return pos.y() * _width + pos.x();
}

QGraphicsEllipseItem* DataContainer::getVertex(const QPointF &pos) const {
    QGraphicsEllipseItem *item  = 0;
    int                   index = convertPosToIndex(pos);

    if (vertex_map.contains(index)) {
        item = vertex_map[index];
    }

    return item;
}

QGraphicsEllipseItem* DataContainer::addVertex(const QPointF &pos) {
    int index = convertPosToIndex(pos);

    if (vertex_map.contains(index)) {
        qWarning("this position has already vertex");
        return 0;
    }

    QRectF rect = QRectF(pos - QPointF(Scene::RECT_SIZE, Scene::RECT_SIZE) / 2, QSizeF(Scene::RECT_SIZE, Scene::RECT_SIZE));
    auto   item = _scene->addEllipse(rect, QPen(Qt::black));
    vertex_map.insert(index, item);

    return item;
}

void DataContainer::removeVertex(const QPointF &pos) {
    int index = convertPosToIndex(pos);

    if (!vertex_map.contains(index)) {
        qWarning("this position has not exist vertex");
        return;
    }

    auto item = vertex_map[index];
    _scene->removeItem(item);
    vertex_map.remove(index);
}

bool DataContainer::containsVertex(const QPointF &pos) const {
    return vertex_map.contains(convertPosToIndex(pos));
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
