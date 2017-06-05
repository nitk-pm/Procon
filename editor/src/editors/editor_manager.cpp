#include "editors/editor_manager.h"
#include "commands/command_manager.h"
#include "editors/editor.h"
#include "models/document.h"
#include "common/scene.h"

EditorManager::EditorManager(Document *document, CommandManager *manager, Ui::MainWindow *ui) : QActionGroup(0) {
    setExclusive(true);
    _document = document;
    _manager = manager;
    _ui = ui;
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(changeEditor(QAction*)));
}

void EditorManager::registerEditor(QAction *action, Editor *editor) {
    editor->setEditorManager(this);
    editor->setDocument(_document);
    editor->setCommandManager(_manager);
    editor->connectAction(_ui);
    connect(_document->scene(), SIGNAL(beginEditor()), editor, SLOT(begin()));
    connect(_document->scene(), SIGNAL(finishEditor()), editor, SLOT(finish()));

    QVariant variant;
    variant.setValue(editor);
    action->setData(variant);
    addAction(action);
}

void EditorManager::selectEditorAt(int index) {
    auto action = actions().value(index);
    if (action) {
        action->setChecked(true);
        Editor *editor = action->data().value<Editor*>();
        _document->scene()->changeEditor(editor);
    }
}

void EditorManager::changeEditor(QAction *action) {
    Editor *editor = action->data().value<Editor*>();
    if (editor) _document->scene()->changeEditor(editor);
}
