#include "mode/plotting_mode.h"
#include "common/scene.h"
#include "common/data_container.h"

#include <QtCore/QDebug>
#include <QtWidgets/QGraphicsSceneMouseEvent>

PlottingMode::PlottingMode(DataContainer *container, QObject *parent) : QObject(parent) {
    this->container = container;
}

bool PlottingMode::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::GraphicsSceneMousePress) {
        QGraphicsSceneMouseEvent *mouse_event = static_cast<QGraphicsSceneMouseEvent*>(event);
        plotting(mouse_event);
    }
    return QObject::eventFilter(watched, event);
}

void PlottingMode::plotting(QGraphicsSceneMouseEvent *event) {
    QPointF pos = container->scene()->modifyPos(event->scenePos());
    container->addVertex(pos);
}
