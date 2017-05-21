#include "common/data_container.h"
#include "common/scene.h"

#include <QtWidgets/QGraphicsItem>
#include <QtCore/QDebug>

DataContainer::DataContainer(Scene *scene, int width, int height) {
    set(scene, width, height);
}

DataContainer::~DataContainer() {
    _scene->clear();
}

void DataContainer::set(Scene *scene, int width, int height) {
    _scene      = scene;
    _width      = width;
    _height     = height;
    _background = _scene->createBackground(_width, _height);

    vertex_list = new QGraphicsItemGroup();
    vertex_list->setSelected(true);
    // _scene->addItem(vertex_list);
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
    QPointF p = _background->mapToScene(pos * BASE_SIZE) + QPointF(BASE_SIZE, BASE_SIZE) / 2;
    return std::move(p);
}

QPointF DataContainer::modifyPosCenter(const QPointF &pos) {
    QPoint p = convertRealPosToVirtualPos(pos);
    return convertVirtualPosToRealPos(p);
}

QGraphicsEllipseItem* DataContainer::addVertex(const QPoint &pos) {
    constexpr QPoint OFFSET = QPoint(RECT_SIZE, RECT_SIZE) / 2;
    QPointF p = convertVirtualPosToRealPos(pos);

    qDebug() << _scene->itemAt(p, QTransform());

    if (_scene->itemAt(p, QTransform()) != _background) {
        qWarning("can't add vertex to this position");
        return 0;
    }

    QRectF rect = QRectF(0, 0, RECT_SIZE, RECT_SIZE);
    auto item = new QGraphicsEllipseItem(rect);
    item->setPen(QPen(Qt::red));
    item->setPos(p - OFFSET);
    item->setData(0, p);
    _scene->addItem(item);

    return item;
}

QGraphicsPolygonItem* DataContainer::addPolygon(const QPolygonF &polygon) {
    auto item = new QGraphicsPolygonItem(polygon);
    item->setPen(QPen(Qt::black));
    item->setBrush(QBrush(Qt::gray));
    _scene->addItem(item);
    return item;
}

QGraphicsItem* DataContainer::getItem(const QPoint &pos) {
    QPointF p = convertVirtualPosToRealPos(pos);
    auto item = _scene->itemAt(p, QTransform());

    if (!item || item == _background) return 0;
    return item;
}
