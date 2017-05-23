#include "common/editor_manager.h"
#include "common/editor.h"

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
}
