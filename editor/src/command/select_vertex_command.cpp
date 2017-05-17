#include "command/select_vertex_command.h"
#include "common/scene.h"

SelectVertexCommand::SelectVertexCommand(Scene *scene, const QPointF &pos, QUndoCommand *parent) : Command(parent) {
    this->scene = scene;
    this->pos   = pos;
}

void SelectVertexCommand::undo() {
    // scene->popSelectVertex();
}

void SelectVertexCommand::redo() {
    // scene->pushSelectVertex(pos);
}
