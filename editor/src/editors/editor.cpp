#include "editors/editor.h"
#include "models/document.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>

Editor::Editor(Document *document, QObject *parent) : QObject(parent) {
    setDocument(document);
}

Document* Editor::document() const {
    return _document;
}

void Editor::setDocument(Document *document) {
    _document = document;
}

bool Editor::eventFilter(QObject *watched, QEvent *event) {
    if (_document) {
        auto scene_event = static_cast<QGraphicsSceneMouseEvent*>(event);
        sceneEvent(scene_event);
    }
    return QObject::eventFilter(watched, event);
}
