#include "common/scene.h"
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtCore/QDebug>
#include <QtGui/QTransform>

#include "command/add_vertex_command.h"
#include "command/select_vertex_command.h"
#include "command/create_polygon_command.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
}

void Scene::createBackground(int width, int height) {
    QRect scene_size = QRect(0, 0, width * BASE_SIZE * 2, height * BASE_SIZE * 2);
    QRect image_size = QRect(width * BASE_SIZE / 2, height * BASE_SIZE / 2, width * BASE_SIZE, height * BASE_SIZE);
    QImage image = QImage(image_size.width(), image_size.height(), QImage::Format_RGB32);
    int center = BASE_SIZE / 2;
    for (int h = 0; h < image.height(); h++) {
        for (int w = 0; w < image.width(); w++) {
            int _w = w % BASE_SIZE, _h = h % BASE_SIZE;
            if (_w >= center - 1 && _w <= center && _h >= center - 1 && _h <= center) image.setPixelColor(w, h, Qt::gray);
            else image.setPixelColor(w, h, Qt::white);
        }
    }

    background_size = QSize(width, height);

    QPixmap pixmap = QPixmap::fromImage(image);
    background = new QGraphicsPixmapItem(pixmap);
    background->setPos(image_size.left(), image_size.top());
    setSceneRect(scene_size);
    addItem(background);
}

QPointF Scene::modifyPos(const QPointF &pos) {
    QPointF pf = background->mapFromScene(pos) / Scene::BASE_SIZE;
    QPoint p = QPoint(pf.x(), pf.y()) * Scene::BASE_SIZE + QPoint(Scene::BASE_SIZE, Scene::BASE_SIZE) / 2;
    return std::move(background->mapToScene(p));
}

int Scene::convertPosToIndex(const QPointF &pos) {
    return int(pos.x()) + int(pos.y()) * background_size.width();
}

void Scene::selectVertex(const QPointF &pos) {
    int index = convertPosToIndex(pos);
    if (!vertex_map.contains(index)) {
        qWarning("not exist vertex");
        return;
    }
    auto item = vertex_map[index];
    item->setPen(QPen(Qt::blue));
    item->setData(0, QVariant(true));

    if (!select_vertex_list.empty()) {
        auto prev_vertex = select_vertex_list.back();
        auto line = addLine(QLineF(item->rect().center(), prev_vertex->rect().center()));
        line_list.append(line);
    }

    select_vertex_list.append(item);
}

void Scene::deselectVertex(const QPointF &pos) {
    int index = convertPosToIndex(pos);
    if (!vertex_map.contains(index)) {
        qWarning("not exist vertex");
        return;
    }
    auto item = vertex_map[index];
    item->setPen(QPen(Qt::red));
    item->setData(0, QVariant(false));

    if (!line_list.empty()) {
        removeItem(line_list.back());
        line_list.pop_back();
    }

    select_vertex_list.pop_back();
}

void Scene::addVertex(const QPointF &pos) {
    QRectF rect = QRectF(pos - QPointF(Scene::RECT_SIZE, Scene::RECT_SIZE) / 2, QSizeF(Scene::RECT_SIZE, Scene::RECT_SIZE));
    auto item = addEllipse(rect, QPen(Qt::red));
    item->setData(0, QVariant(false));
    item->setZValue(1);
    vertex_map.insert(convertPosToIndex(pos), item);
}

void Scene::removeVertex(const QPointF &pos) {
    int index = convertPosToIndex(pos);
    if (!vertex_map.contains(index)) {
        qWarning("not exist vertex");
        return;
    }
    removeItem(vertex_map[index]);
    vertex_map.remove(index);
}

int Scene::createPolygon() {
    QPolygonF polygon;
    for (auto it : select_vertex_list) {
        polygon.append(it->rect().center());
    }

    auto polygon_item = addPolygon(polygon, QPen(Qt::black), QBrush(Qt::blue));
    polygon_list.append(polygon_item);

    while (!select_vertex_list.empty()) {
        deselectVertex(select_vertex_list.back()->rect().center());
    }
    return polygon_list.length() - 1;
}

void Scene::destroyPolygon(int index) {
    auto polygon_item = polygon_list[index];
    auto polygon = polygon_item->polygon();

    removeItem(polygon_item);
    for (auto it : polygon) {
        selectVertex(it);
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF pos = modifyPos(event->scenePos());
    int index = convertPosToIndex(pos);
    if (vertex_map.contains(index)) {
        if (!vertex_map[index]->data(0).toBool()) {
            auto command = new SelectVertexCommand(this, pos);
            Command::stack->push(command);
        }
        else if (select_vertex_list.front()->rect().center() == pos) {
            auto command = new CreatePolygonCommand(this);
            Command::stack->push(command);
        }
    }
    else {
        auto command = new AddVertexCommand(this, pos);
        Command::stack->push(command);
    }
}
