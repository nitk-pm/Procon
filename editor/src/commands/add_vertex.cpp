#include "commands/add_vertex.h"
#include "models/document.h"
#include "models/vertex_object.h"

AddVertex::AddVertex(Document *doc, VertexObject *obj, QUndoCommand *parent) : QUndoCommand(parent) {
    document = doc;
    object = obj;
}

AddVertex::~AddVertex() {
    delete object;
}

void AddVertex::undo() {
    document->removeObject(object);
}

void AddVertex::redo() {
    document->addObject(object);
}
