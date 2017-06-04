#include "common/scene.h"
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);
}

Scene::Scene(int width, int height, QObject *parent) : QGraphicsScene(parent) {
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
    _background = new QGraphicsPixmapItem(pixmap);
    _background->setPos(image_size.left(), image_size.top());
    setSceneRect(scene_size);
    addItem(_background);
}

QPointF Scene::modifyDataPos(const QPointF &pos) {
    QPointF p = _background->mapFromScene(pos) / BASE_SIZE;
    return QPointF(int(p.x()), int(p.y()));
}

QPointF Scene::modifyPos(const QPointF &pos) {
    QPointF p = _background->mapToScene(modifyDataPos(pos) * BASE_SIZE);
    return p;
}

QGraphicsPixmapItem* Scene::background() const {
    return _background;
}

void Scene::changeEditor(QObject *editor) {
    removeEventFilter(current_editor);
    finishEditor();
    current_editor = editor;
    beginEditor();
    installEventFilter(current_editor);
}
