#include "editors/editor_manager.h"
#include "editors/editor.h"
#include "models/document.h"
#include "common/scene.h"

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

void EditorManager::registerEditor(QAction *action, Editor *editor, bool default_clicked) {
    QVariant variant;
    variant.setValue(editor);
    action->setData(variant);
    addAction(action);
    _editors.append(editor);

    if (default_clicked) {
        default_checked_action = action;
    }
}

void EditorManager::setDocument(Document *document) {
    for (auto editor : _editors) {
        editor->setDocument(document);
    }
}

void EditorManager::connectScene(Scene *scene) {
    for (auto editor : _editors) {
        connect(scene, SIGNAL(beginEditor()), editor, SLOT(begin()));
        connect(scene, SIGNAL(finishEditor()), editor, SLOT(finish()));
    }
    connect(this, SIGNAL(triggered(QAction*)), scene, SLOT(changeEditor(QAction*)));

    if (default_checked_action) {
        default_checked_action->setChecked(true);
        scene->changeEditor(default_checked_action);
    }
}
