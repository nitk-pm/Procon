#include "mode/select_mode.h"
#include "common/data_container.h"
#include "common/scene.h"

#include <QtCore/QDebug>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsEllipseItem>

SelectMode::SelectMode(DataContainer *container, QObject *parent) : QObject(parent) {
    this->container = container;
}

bool SelectMode::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::GraphicsSceneMousePress) {
        QGraphicsSceneMouseEvent *mouse_event = static_cast<QGraphicsSceneMouseEvent*>(event);
        select(mouse_event);
    }
    return QObject::eventFilter(watched, event);
}

void SelectMode::select(QGraphicsSceneMouseEvent *event) {
    QPointF pos = container->scene()->modifyPos(event->scenePos());
    QGraphicsEllipseItem *item = container->getVertex(pos);

    if (item && select_items.empty()) {
        item->setPen(QPen(Qt::blue));
        select_items.append(item);
    }
    else if (item && event->modifiers() == Qt::ControlModifier) {
        item->setPen(QPen(Qt::blue));
        select_items.append(item);
    }
    else {
        for (auto item : select_items) {
            item->setPen(QPen(Qt::black));
        }
        select_items.clear();
    }
}
