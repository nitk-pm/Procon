#include "common/scene_view.h"
#include "common/scene.h"

#include <QtWidgets/QScrollBar>
#include <QtGui/QWheelEvent>
#include <QtCore/QTimeLine>
#include <QtCore/QTimer>

SceneView::SceneView(QWidget *parent) : QGraphicsView(parent) {
    scheduled_scalings = 0;
}

SceneView::~SceneView() {}

void SceneView::setScene(Scene *scene) {
    QGraphicsView::setScene(scene);
    QTimer::singleShot(100, [&]() {
        qreal w = this->rect().width() - this->scene()->width() - this->verticalScrollBar()->rect().width();
        qreal h = this->rect().height() - this->scene()->height() - this->horizontalScrollBar()->rect().height();
        this->translate(w / 2, h / 2);
    });
}

void SceneView::zoom(int delta) {
    int degrees = delta / 8;
    int steps   = degrees / 15;
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

void SceneView::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        zoom(event->delta());
    }
    else if (event->modifiers() & Qt::ShiftModifier) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - event->delta());
    }
    else QGraphicsView::wheelEvent(event);
}

void SceneView::scalingTime(qreal x) {
    const qreal factor  = 1.0 + qreal(scheduled_scalings) / 300.0;
    QPointF     old_pos = mapToScene(geometry().center());

    scale(factor, factor);

    QPointF new_pos = mapToScene(geometry().center());
    QPointF delta   = new_pos - old_pos;
    translate(delta.x(), delta.y());
}

void SceneView::animFinished() {
    if (scheduled_scalings > 0) scheduled_scalings--;
    else scheduled_scalings++;
    sender()->~QObject();
}
