#include "command/add_vertex_command.h"
#include "common/scene.h"


AddVertexCommand::AddVertexCommand(Scene *scene, const QPointF &pos, QUndoCommand *parent) : Command(parent) {
    this->scene = scene;
    this->pos   = pos;
}

void AddVertexCommand::undo() {
    // scene->removeVertex(pos);
}

void AddVertexCommand::redo() {
    // scene->addVertex(pos);
}
