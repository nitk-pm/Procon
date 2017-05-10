#include "command/add_vertex_command.h"
#include "common/scene.h"
#include <QtWidgets/QGraphicsEllipseItem>
#include <QtCore/QDebug>


AddVertexCommand::AddVertexCommand(Scene *scene, const QPointF &pos, QUndoCommand *parent) : Command(parent), scene(scene), modified_pos(pos) {

}

void AddVertexCommand::undo() {
    scene->removeItem(item);
}

void AddVertexCommand::redo() {
    QRectF rect = QRectF(modified_pos - QPointF(Scene::RECT_SIZE, Scene::RECT_SIZE) / 2, QSizeF(Scene::RECT_SIZE, Scene::RECT_SIZE));
    item = scene->addEllipse(rect, QPen(Qt::red));
    item->setZValue(2);
}
