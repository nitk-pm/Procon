#include "commands/create_polygon.h"
#include "models/document.h"
#include "models/polygon_object.h"

CreatePolygon::CreatePolygon(Document *doc, PolygonObject *poly, QUndoCommand *parent) : QUndoCommand(parent) {
    document = doc;
    polygon = poly;
}

CreatePolygon::~CreatePolygon() {
    delete polygon;
}

void CreatePolygon::undo() {
    document->removeObject(polygon);
}

void CreatePolygon::redo() {
    document->addObject(polygon);
    document->serialize();
}
