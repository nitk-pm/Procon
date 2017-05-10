#include "common/scene_view.h"
#include "common/scene.h"
#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QtCore/QTimeLine>
#include <QtCore/QTimer>
#include <QtCore/QDebug>

SceneView::SceneView(QWidget *parent) : QGraphicsView(parent), scheduled_scalings(0), translate_scalings(1.0) {
}

SceneView::~SceneView() {

}

void SceneView::setScene(Scene *scene) {
    QGraphicsView::setScene(scene);
    QTimer::singleShot(0, [&]() {
        qreal x = this->geometry().width() - this->scene()->width();
        qreal y = this->geometry().height() - this->scene()->height();
        this->translate(x / 2, y / 2);
    });
    scene->installEventFilter(this);
}

bool SceneView::eventFilter(QObject *watched, QEvent *event) {
    if (watched == scene()) {
        if (event->type() == QEvent::GraphicsSceneMouseMove || event->type() == QEvent::GraphicsSceneMousePress) {
            return !hasMouseTracking();
        }
    }
    return QGraphicsView::eventFilter(watched, event);
}

void SceneView::mousePressEvent(QMouseEvent *event) {
    old_mouse_pos = event->pos();
    QGraphicsView::mousePressEvent(event);
}

void SceneView::mouseMoveEvent(QMouseEvent *event) {
    QPointF new_mouse_pos = event->pos();
    QPointF delta = new_mouse_pos - old_mouse_pos;
    old_mouse_pos = new_mouse_pos;
    if (!hasMouseTracking()) {
        translate(delta.x() / translate_scalings, delta.y() / translate_scalings);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void SceneView::wheelEvent(QWheelEvent *event) {
    int degrees = event->delta() / 8;
    int steps = degrees / 15;
    scheduled_scalings += steps;
    if (scheduled_scalings * steps < 0) {
        scheduled_scalings = steps;
    }

    QTimeLine *anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);
    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (scalingTime(qreal)));
    connect(anim, SIGNAL (finished()), SLOT (animFinished()));
    anim->start();
}

void SceneView::scalingTime(qreal x) {
    const qreal factor = 1.0 + qreal(scheduled_scalings) / 300.0;
    translate_scalings *= 1.0 + qreal(scheduled_scalings) / 300.0;
    QPointF old_pos = mapToScene(geometry().center());

    scale(factor, factor);

    QPointF new_pos = mapToScene(geometry().center());
    QPointF delta = new_pos - old_pos;
    translate(delta.x(), delta.y());
}

void SceneView::animFinished() {
    if (scheduled_scalings > 0) scheduled_scalings--;
    else scheduled_scalings++;
    sender()->~QObject();
}

void SceneView::changeModeMoving() {
    qDebug() << "change mode moving";
    setMouseTracking(false);
    // static_cast<Scene*>(scene())->setVisibleCurrentLine(false);
}

void SceneView::changeModeEditPiece() {
    qDebug() << "change mode edit piece";
    setMouseTracking(true);
    // static_cast<Scene*>(scene())->setVisibleCurrentLine(true);
}
