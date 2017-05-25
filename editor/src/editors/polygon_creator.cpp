#include "editors/polygon_creator.h"
#include "models/document.h"
#include "models/object_model.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>

void PolygonCreator::sceneEvent(QGraphicsSceneMouseEvent *event) {
    if (event->type() == QEvent::GraphicsSceneMousePress) {
        auto obj = document()->getObject(event->scenePos());
        if (obj && !obj->isClicked()) {
            obj->setClicked(true);
            object_list.append(obj);
        }
    }
}

void PolygonCreator::finish() {
    for (auto obj : object_list) {
        obj->setClicked(false);
    }
    object_list.clear();
}
