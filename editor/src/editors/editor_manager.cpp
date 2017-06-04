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
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(changeEditor(QAction*)));
    _scene = nullptr;
}

void EditorManager::registerEditor(QAction *action, Editor *editor) {
    QVariant variant;
    variant.setValue(editor);
    action->setData(variant);
    addAction(action);

    if (_scene) {
        connect(_scene, SIGNAL(beginEditor()), editor, SLOT(begin()));
        connect(_scene, SIGNAL(finishEditor()), editor, SLOT(finish()));
    }
    else {
        stack.push(editor);
    }
}

void EditorManager::setDocument(Document *document) {
    for (auto &action : actions()) {
        Editor *editor = action->data().value<Editor*>();
        editor->setDocument(document);
    }
}

void EditorManager::setScene(Scene *scene) {
    _scene = scene;

    while(!stack.empty()) {
        Editor *editor = stack.pop();
        connect(_scene, SIGNAL(beginEditor()), editor, SLOT(begin()));
        connect(_scene, SIGNAL(finishEditor()), editor, SLOT(finish()));
    }
}

void EditorManager::selectEditorAt(int index) {
    auto action = actions().value(index);
    if (action) {
        action->setChecked(true);
        Editor *editor = action->data().value<Editor*>();
        _scene->changeEditor(editor);
    }
}

void EditorManager::changeEditor(QAction *action) {
    Editor *editor = action->data().value<Editor*>();
    if (editor) _scene->changeEditor(editor);
}
