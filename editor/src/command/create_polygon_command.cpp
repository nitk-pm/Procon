#include "command/create_polygon_command.h"
#include "common/scene.h"

CreatePolygonCommand::CreatePolygonCommand(Scene *scene, QUndoCommand *parent) : Command(parent), scene(scene) {}

void CreatePolygonCommand::undo() {
    scene->destroyPolygon(index);
}

void CreatePolygonCommand::redo() {
    index = scene->createPolygon();
}
