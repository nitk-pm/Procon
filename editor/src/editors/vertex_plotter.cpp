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
        auto item = document()->scene()->itemAt(event->scenePos(), QTransform());
        qDebug() << item;
        auto pos = document()->scene()->modifyPos(event->scenePos());
        document()->addObject(new VertexObject(pos));
    }
}
