#include "command/add_vertex_command.h"
#include "common/scene.h"


AddVertexCommand::AddVertexCommand(Scene *scene, const QPointF &pos, QUndoCommand *parent) : Command(parent), scene(scene), pos(pos) {

}

void AddVertexCommand::undo() {
    scene->removeVertex(pos);
}

void AddVertexCommand::redo() {
    scene->addVertex(pos);
}
