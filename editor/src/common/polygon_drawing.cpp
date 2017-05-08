#include "common/polygon_drawing.h"
#include "common/scene.h"
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsLineItem>
#include <QtWidgets/QGraphicsEllipseItem>
#include <QtCore/QLineF>
#include <QtCore/QDebug>

PolygonDrawing::PolygonDrawing(QGraphicsItem *background) : background(background) {
    current_vertex = background->scene()->addEllipse(QRectF(0, 0, 0, 0), QPen(Qt::blue));
    current_line = background->scene()->addLine(QLineF(0, 0, 0, 0));
    current_vertex->setZValue(2);
    current_line->setZValue(1);
    setVisibleCurrentVertex(false);
}

PolygonDrawing::~PolygonDrawing() {}

void PolygonDrawing::setVisibleCurrentVertex(bool flag) {
    current_vertex->setVisible(flag);
    current_line->setVisible(flag);
}

void PolygonDrawing::updateCurrentVertex(const QPointF &pos) {}

void PolygonDrawing::move(const QPointF &pos) {
    if (!vertexes.isEmpty()) {
        auto start = vertexes.last().first->rect().center();
        current_vertex->setRect(QRectF(pos - QPoint(Scene::OFFSET, Scene::OFFSET) / 2, QSizeF(Scene::RECT_SIZE, Scene::RECT_SIZE)));
        current_line->setLine(QLineF(pos, start));
    }
    else {
        setVisibleCurrentVertex(false);
    }
}

void PolygonDrawing::push(const QPointF &pos) {
    auto pf = background->mapFromScene(pos) / Scene::BASE_SIZE;
    auto p = QPoint(pf.x(), pf.y()) * Scene::BASE_SIZE + QPoint(Scene::OFFSET, Scene::OFFSET);
    auto rect = background->mapRectToScene(QRectF(p, QSizeF(Scene::RECT_SIZE, Scene::RECT_SIZE)));

    QGraphicsEllipseItem *vertex_item = background->scene()->addEllipse(rect, QPen(Qt::red));
    vertex_item->setZValue(2);

    QLineF line;
    QGraphicsLineItem *line_item = 0;
    if (vertexes.isEmpty()) {
        line = QLineF(rect.center(), pos);
        setVisibleCurrentVertex(true);
    }
    else {
        line = QLineF(rect.center(), vertexes.last().first->rect().center());
        line_item = background->scene()->addLine(line);
        line_item->setZValue(1);
    }
    current_vertex->setRect(rect);
    current_line->setLine(line);

    vertexes.push_back(qMakePair(vertex_item, line_item));
}

void PolygonDrawing::pop() {
    if (!vertexes.isEmpty()) {
        auto pos = vertexes.last().first->rect().center();
        background->scene()->removeItem(vertexes.last().first);
        if (vertexes.last().second) background->scene()->removeItem(vertexes.last().second);
        vertexes.erase(vertexes.end() - 1);
        move(pos);
    }
}

void PolygonDrawing::clear() {
    setVisibleCurrentVertex(false);
    for (auto it : vertexes) {
        background->scene()->removeItem(it.first);
        if (it.second) background->scene()->removeItem(it.second);
    }
    vertexes.clear();
}

bool PolygonDrawing::empty() const {
    return vertexes.isEmpty();
}

int PolygonDrawing::count() const {
    return vertexes.length();
}

QVector<QPoint> PolygonDrawing::get() {
    QVector<QPoint> result = QVector<QPoint>();
    for (auto it : vertexes) {
        auto point = (background->mapFromScene(it.first->rect().topLeft()) / Scene::BASE_SIZE).toPoint();
        result.push_back(point);
    }
    return std::move(result);
}
