#include "commands/remove_object.h"
#include "models/document.h"
#include "models/object_model.h"

RemoveObject::RemoveObject(Document *doc, QList<ObjectModel*> list) : QUndoCommand(0) {
    document = doc;
    remove_list = list;
}

RemoveObject::~RemoveObject() {
    for (auto obj : remove_list) {
        document->removeObject(obj);
        delete obj;
    }
    remove_list.clear();
}

void RemoveObject::undo() {
    for (auto obj : remove_list) {
        document->addObject(obj);
    }
}

void RemoveObject::redo() {
    for (auto obj : remove_list) {
        document->removeObject(obj);
    }
}
