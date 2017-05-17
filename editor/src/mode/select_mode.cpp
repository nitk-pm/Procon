#include "mode/select_mode.h"
#include "common/data_container.h"
#include "common/scene.h"

#include <QtCore/QDebug>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsEllipseItem>

SelectMode::SelectMode(DataContainer *container, QObject *parent) : QObject(parent) {
    this->container = container;
    can_move_item   = false;
}

bool SelectMode::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::GraphicsSceneMousePress) {
        QGraphicsSceneMouseEvent *mouse_event = static_cast<QGraphicsSceneMouseEvent*>(event);
        moveStartEvent(mouse_event);
    }
    else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
        QGraphicsSceneMouseEvent *mouse_event = static_cast<QGraphicsSceneMouseEvent*>(event);
        selectEvent(mouse_event);
    }
    else if (event->type() == QEvent::GraphicsSceneMouseMove && can_move_item) {
        QGraphicsSceneMouseEvent *mouse_event = static_cast<QGraphicsSceneMouseEvent*>(event);
        moveEvent(mouse_event);
    }
    return QObject::eventFilter(watched, event);
}

void SelectMode::selectEvent(QGraphicsSceneMouseEvent *event) {
    if (!can_move_item) {
        QPoint pos = container->modifiyPosCenter(event->scenePos());
        QPoin
        if (item) {
            if (!select_items.empty() && event->modifiers() != Qt::ControlModifier) reselect(item);
            else select(item);
        }
        else deselect();
    }
    can_move_item = false;
}

void SelectMode::moveStartEvent(QGraphicsSceneMouseEvent *event) {
    QPointF pos  = container->scene()->modifyPos(event->scenePos());
    auto    item = container->scene()->getItem(pos);

    if (item) {
        auto select_item = static_cast<QAbstractGraphicsShapeItem*>(item);
        if (select_item->pen().color() == Qt::blue) {
            prev_pos      = pos;
            can_move_item = true;
        }
    }
}

void SelectMode::moveEvent(QGraphicsSceneMouseEvent *event) {
    QPointF now_pos = container->scene()->modifyPos(event->scenePos());
    QPointF d       = now_pos - prev_pos;
    prev_pos = now_pos;

    for (auto item : select_items) {
        QPointF pos = item->pos() + d;
        item->setPos(pos);
    }
}

void SelectMode::select(QGraphicsItem *item) {
    auto select_item = static_cast<QAbstractGraphicsShapeItem*>(item);
    select_item->setData(0, select_item->pen());
    select_item->setData(1, select_item->pos());
    select_item->setPen(QPen(Qt::blue));
    select_items.append(select_item);
    setDeleteActionFlag(true);
}

void SelectMode::deselect() {
    for (auto item : select_items) {
        item->setPen(item->data(0).value<QPen>());
    }
    select_items.clear();
    setDeleteActionFlag(false);
}

void SelectMode::reselect(QGraphicsItem *item) {
    deselect();
    select(item);
}
