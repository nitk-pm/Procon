#include "common/scene.h"
#include "common/editor_manager.h"
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QAction>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);
}

Scene::Scene(int width, int height, QObject *parent = 0) : QGraphicsScene(parent) {
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    setBoardSize(width, height);
}

void Scene::setBoardSize(int width, int height) {
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
    QPointF p = background->mapFromScene(pos) / BASE_SIZE;
    p = QPointF(int(p.x()) * BASE_SIZE, int(p.y()) * BASE_SIZE);
    p = background->mapToScene(p);
    return std::move(p);
}

void Scene::changeEditor(QAction *action) {
    removeEventFilter(current_editor);
    current_editor = action->data().value<Editor*>();
    installEventFilter(current_editor);
}
