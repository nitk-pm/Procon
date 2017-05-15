#include "common/scene.h"
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QAction>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtCore/QDebug>
#include <QtCore/QtGlobal>

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

void Scene::addVertex(const QPointF &pos) {
    int index = convertPosToIndex(pos);
    if (vertex_map.contains(index)) {
        qWarning("already exists vertex");
        return;
    }
    QRectF rect = QRectF(pos - QPointF(Scene::RECT_SIZE, Scene::RECT_SIZE) / 2, QSizeF(Scene::RECT_SIZE, Scene::RECT_SIZE));
    auto item = addEllipse(rect, QPen(Qt::red));
    item->setData(0, QVariant(false));
    item->setZValue(1);
    vertex_map.insert(index, item);
}

void Scene::removeVertex(const QPointF &pos) {
    int index = convertPosToIndex(pos);
    if (!vertex_map.contains(index)) {
        qWarning("not exists vertex");
        return;
    }
    removeItem(vertex_map[index]);
    vertex_map.remove(index);
}

void Scene::pushSelectVertex(const QPointF &pos) {
    int index = convertPosToIndex(pos);
    if (!vertex_map.contains(index)) {
        qWarning("not exists vertex");
        return;
    }
    auto item = vertex_map[index];
    item->setPen(QPen(Qt::blue));
    item->setData(0, QVariant(true));

    if (!select_vertexes.empty()) {
        auto edge = addLine(QLineF(pos, select_vertexes.back()));
        edge_list.append(edge);
    }

    select_vertexes.append(pos);
}

void Scene::popSelectVertex() {
    if (select_vertexes.empty()) {
        qWarning("has not select any vertex");
        return;
    }
    auto item = vertex_map[convertPosToIndex(select_vertexes.back())];
    item->setPen(QPen(Qt::red));
    item->setData(0, QVariant(false));

    if (!edge_list.empty()) {
        removeItem(edge_list.back());
        edge_list.pop_back();
    }

    select_vertexes.pop_back();
}

int Scene::createPolygon() {
    QPolygonF polygon;
    for (auto pos : select_vertexes) {
        polygon.append(pos);
    }

    int id = qrand();
    auto polygon_item = addPolygon(polygon, QPen(Qt::black), QBrush(Qt::gray));
    polygon_list.insert(id, polygon_item);

    while (!select_vertexes.empty()) {
        popSelectVertex();
    }
    return id;
}

void Scene::destroyPolygon(int id) {
    if (!polygon_list.contains(id)) {
        qWarning("not exists polygon");
        return;
    }
    removeItem(polygon_list.value(id));
    polygon_list.remove(id);
}

const QVector<QPointF>& Scene::selectVartexes() const {
    return select_vertexes;
}

QGraphicsPolygonItem* Scene::polygonItem(int id) const {
    if (!polygon_list.contains(id)) {
        qWarning("not exists polygon");
        return nullptr;
    }
    return polygon_list.value(id);
}

void Scene::changeMode(QAction *action) {
    removeEventFilter(current_mode);
    qDebug("check point");
    current_mode = action->data().value<QObject*>();
    qDebug("check point 2");
    installEventFilter(current_mode);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // QPointF pos = modifyPos(event->scenePos());
    // int index = convertPosToIndex(pos);
    // if (vertex_map.contains(index)) {
    //     if (!vertex_map[index]->data(0).toBool()) {
    //         Command::stack->push(new SelectVertexCommand(this, pos));
    //     }
    //     else if (select_vertexes.front() == pos) {
    //         Command::stack->push(new CreatePolygonCommand(this));
    //     }
    // }
    // else {
    //     Command::stack->push(new AddVertexCommand(this, pos));
    // }
}
