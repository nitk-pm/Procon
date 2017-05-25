#include "editors/vertex_plotter.h"
#include "common/scene.h"
#include "models/document.h"
#include "models/vertex_object.h"
#include "commands/add_vertex.h"
#include "commands/command_manager.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtCore/QDebug>

void VertexPlotter::sceneEvent(QGraphicsSceneMouseEvent *event) {
    if (event->type() == QEvent::GraphicsSceneMousePress) {
        auto obj = document()->getObject(event->scenePos());
        if (!obj) {
            auto pos = document()->scene()->modifyPos(event->scenePos());
            CommandManager::instance()->registerCommand(new AddVertex(document(), new VertexObject(pos)));
        }
    }
}
