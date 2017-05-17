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

QGraphicsRectItem* Scene::createBackground(int width, int height) {
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
    auto background = new QGraphicsPixmapItem(pixmap);
    // background->setPos(image_size.left(), image_size.top());
    setSceneRect(scene_size);
    addItem(background);
    return background;
}

void Scene::changeMode(QAction *action) {
    removeEventFilter(current_mode);
    // qDebug("check point");
    current_mode = action->data().value<QObject*>();
    // qDebug("check point 2");
    installEventFilter(current_mode);
}
