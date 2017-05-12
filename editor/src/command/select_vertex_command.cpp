#include "command/select_vertex_command.h"
#include "common/scene.h"

SelectVertexCommand::SelectVertexCommand(Scene *scene, const QPointF &pos, QUndoCommand *parent) : Command(parent), scene(scene), pos(pos) {

}

void SelectVertexCommand::undo() {
    scene->deselectVertex();
}

void SelectVertexCommand::redo() {
    scene->selectVertex(pos);
}
