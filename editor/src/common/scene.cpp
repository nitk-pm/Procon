#include "common/scene.h"
#include "common/polygon_drawing.h"
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

void Scene::setVisibleCurrentVertex(bool flag) {
    drawing->setVisibleCurrentVertex(flag);
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
    
    drawing = new PolygonDrawing(background);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    drawing->move(event->scenePos());
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    auto command = new AddVertexCommand(drawing, event->scenePos());
    Command::stack->push(command);
    // drawing->push(event->scenePos());
    qDebug() << drawing->get();
}
