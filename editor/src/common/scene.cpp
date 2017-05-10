#include "common/scene.h"
#include "command/add_vertex_command.h"
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtCore/QDebug>
#include <QtGui/QTransform>

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

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF pos = modifyPos(event->scenePos());
    auto item = itemAt(pos, QTransform());
    if (item == background) {
        auto command = new AddVertexCommand(this, pos);
        Command::stack->push(command);
    }
}
