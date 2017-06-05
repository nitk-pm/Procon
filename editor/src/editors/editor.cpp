#include "editors/editor_manager.h"
#include "commands/command_manager.h"
#include "editors/editor.h"
#include "models/document.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>

Editor::Editor(QObject *parent) : QObject(parent) {
    _document = nullptr;
}

Document* Editor::document() const {
    return _document;
}

void Editor::setDocument(Document *document) {
    _document = document;
}

EditorManager* Editor::editorManager() const {
    return _editor_manager;
}

void Editor::setEditorManager(EditorManager *manager) {
    _editor_manager = manager;
}

CommandManager* Editor::commandManager() const {
    return _command_manager;
}

void Editor::setCommandManager(CommandManager *manager) {
    _command_manager = manager;
}

bool Editor::eventFilter(QObject *watched, QEvent *event) {
    if (_document) {
        auto scene_event = static_cast<QGraphicsSceneMouseEvent*>(event);
        sceneEvent(scene_event);
    }
    return QObject::eventFilter(watched, event);
}
