#include "command/create_polygon_command.h"
#include "common/scene.h"
#include <QtWidgets/QGraphicsPolygonItem>

CreatePolygonCommand::CreatePolygonCommand(Scene *scene, QUndoCommand *parent) : Command(parent), scene(scene) {}

void CreatePolygonCommand::undo() {
    // QPolygonF polygon = scene->polygonItem(id)->polygon();
    // scene->destroyPolygon(id);
    // for (auto pos : polygon) {
    //     scene->pushSelectVertex(pos);
    // }
}

void CreatePolygonCommand::redo() {
    // id = scene->createPolygon();
}
