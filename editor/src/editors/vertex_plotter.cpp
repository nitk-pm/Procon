#include "editors/vertex_plotter.h"
#include "common/scene.h"
#include "models/document.h"
#include "models/vertex_object.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtCore/QDebug>

VertexPlotter::VertexPlotter(Document *document, QObject *parent) : Editor(document, parent) {

}

void VertexPlotter::sceneEvent(QGraphicsSceneMouseEvent *event) {
    if (event->type() == QEvent::GraphicsSceneMousePress) {
        auto obj = document()->getObject(event->scenePos());
        if (!obj) {
            auto pos = document()->scene()->modifyPos(event->scenePos());
            document()->addObject(new VertexObject(pos));
            qDebug("put");
        }
        else {
            obj->setClicked(true);
        }
    }
}
