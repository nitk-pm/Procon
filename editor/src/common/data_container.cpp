#include "common/data_container.h"
#include "common/scene.h"

#include <QtWidgets/QGraphicsPolygonItem>
#include <QtWidgets/QGraphicsItemGroup>
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

    vertex_group = new QGraphicsItemGroup();
    polygon_group = new QGraphicsItemGroup();
    _scene->addItem(vertex_group);
    _scene->addItem(polygon_group);
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
    return std::move(_background->mapToScene(pos * BASE_SIZE));
}

QPointF DataContainer::convertVirtualPosToRealPosCenter(const QPoint &pos) {
    return std::move(convertVirtualPosToRealPos(pos) + QPointF(BASE_SIZE, BASE_SIZE) / 2);
}

QPointF DataContainer::modifyPosCenter(const QPointF &pos) {
    QPoint p = convertRealPosToVirtualPos(pos);
    return convertVirtualPosToRealPosCenter(p);
}

// QGraphicsEllipseItem* DataContainer::getVertex(const QPoint &pos) const {
//     QGraphicsEllipseItem *item  = 0;
//
//     if (vertex_map.contains(pos)) {
//         item = vertex_map[pos];
//     }
//
//     return item;
// }

QGraphicsEllipseItem* DataContainer::addVertex(const QPoint &pos) {
    constexpr QPoint OFFSET = QPoint(RECT_SIZE, RECT_SIZE) / 2;
    QPointF p = convertVirtualPosToRealPosCenter(pos);

    if (_scene->itemAt(p, QTransform()) != _background) {
        qWarning("can't add vertex to this position");
        return 0;
    }

    QRectF rect = QRectF(0, 0, RECT_SIZE, RECT_SIZE);
    auto item = new QGraphicsEllipseItem(rect);
    item->setPen(QPen(Qt::red));
    item->setPos(convertVirtualPosToRealPosCenter(pos) - OFFSET);
    item->setData(0, pos);
    vertex_group->addToGroup(item);

    return item;
}

// void DataContainer::removeVertex(const QPoint &pos) {
//     if (!vertex_map.contains(pos)) {
//         qWarning("this position has not exist vertex");
//         return;
//     }
//
//     auto item = vertex_map[pos];
//     _scene->removeItem(item);
// }
//
// bool DataContainer::containsVertex(const QPoint &pos) const {
//     return vertex_map.contains(pos);
// }

// QGraphicsPolygonItem* DataContainer::getPolygon(int id) const {
//     QGraphicsPolygonItem *item = 0;
//
//     if (polygon_list.contains(id)) {
//         item = polygon_list[id];
//     }
//
//     return item;
// }

QGraphicsPolygonItem* DataContainer::addPolygon(const QPolygonF &polygon) {
    auto item = new QGraphicsPolygonItem(polygon);
    item->setPen(QPen(Qt::black));
    item->setBrush(QBrush(Qt::gray));
    polygon_group->addToGroup(item);
    return item;
}

// void DataContainer::removePolygon(int id) {
//     if (!polygon_list.contains(id)) {
//         qWarning("this id has not exist polygon");
//         return;
//     }
//
//     auto item = polygon_list[id];
//     _scene->removeItem(item);
//     polygon_list.remove(id);
// }
