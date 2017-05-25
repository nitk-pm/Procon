#include "editors/editor_manager.h"
#include "editors/editor.h"
#include "models/document.h"

EditorManager* EditorManager::instance() {
    static EditorManager manager;
    return &manager;
}

EditorManager::EditorManager() : QActionGroup(0) {
    setExclusive(true);
}

QList<Editor*> EditorManager::editors() const {
    return _editors;
}

void EditorManager::registerEditor(QAction *action, Editor *editor) {
    QVariant variant;
    variant.setValue(editor);
    action->setData(variant);
    addAction(action);
    _editors.append(editor);
}

void EditorManager::setDocument(Document *document) {
    for (auto editor : _editors) {
        editor->setDocument(document);
    }
}
