#include "mode/plotting_mode.h"
#include "common/scene.h"
#include "common/data_container.h"

#include <QtCore/QDebug>
#include <QtWidgets/QGraphicsSceneMouseEvent>

PlottingMode::PlottingMode(DataContainer *container, QObject *parent) : QObject(parent) {
    this->container = container;
}

bool PlottingMode::eventFilter(QObject *watched, QEvent *event) {
    if (!container) return QObject::eventFilter(watched, event);
    if (event->type() == QEvent::GraphicsSceneMousePress) {
        QGraphicsSceneMouseEvent *mouse_event = static_cast<QGraphicsSceneMouseEvent*>(event);
        plotting(mouse_event);
    }
    return QObject::eventFilter(watched, event);
}

void PlottingMode::plotting(QGraphicsSceneMouseEvent *event) {
    QPoint pos = container->convertRealPosToVirtualPos(event->scenePos());
    container->addVertex(pos);
}
