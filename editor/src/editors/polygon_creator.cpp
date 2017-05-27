#include "editors/polygon_creator.h"
#include "models/document.h"
#include "models/polygon_object.h"
#include "commands/command_manager.h"
#include "commands/create_polygon.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtCore/QDebug>

void PolygonCreator::sceneEvent(QGraphicsSceneMouseEvent *event) {
    if (event->type() == QEvent::GraphicsSceneMousePress) {
        auto obj = document()->getObject(event->scenePos());
        if (obj && obj->id() == ObjectID::Vertex) {
            if (!obj->isClicked()) {
                obj->setClicked(true);
                obj->setClickedPenColor(Qt::darkGreen);
                object_list.append(obj);
            }
            else if (obj == object_list.first()) {
                QPolygonF polygon;
                for (auto p : object_list) {
                    polygon << (p->pos() + p->boundingRect().center());
                }
                CommandManager::instance()->registerCommand(new CreatePolygon(document(), new PolygonObject(polygon)));
                finish();
            }
        }
    }
}

void PolygonCreator::finish() {
    for (auto obj : object_list) {
        obj->setClicked(false);
    }
    object_list.clear();
}
