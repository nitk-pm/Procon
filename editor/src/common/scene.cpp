#include "common/scene.h"
#include "common/data_container.h"
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

QGraphicsPixmapItem* Scene::createBackground(int width, int height) {
    int base_size = DataContainer::BASE_SIZE;
    QRect scene_size = QRect(0, 0, width * base_size * 2, height * base_size * 2);
    QRect image_size = QRect(width * base_size / 2, height * base_size / 2, width * base_size, height * base_size);
    QImage image = QImage(image_size.width(), image_size.height(), QImage::Format_RGB32);
    int center = base_size / 2;
    for (int h = 0; h < image.height(); h++) {
        for (int w = 0; w < image.width(); w++) {
            int _w = w % base_size, _h = h % base_size;
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
