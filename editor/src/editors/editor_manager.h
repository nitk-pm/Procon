#ifndef EDITOR_MANAGER__H
#define EDITOR_MANAGER__H

#include <QtWidgets/QActionGroup>
#include <QtCore/QList>
#include <QtCore/QStack>

namespace Ui {
class MainWindow;
}

class Editor;
class Document;
class CommandManager;

class EditorManager : public QActionGroup {
    Q_OBJECT

public:
    EditorManager(Document *document, CommandManager *manager, Ui::MainWindow *ui);
    void registerEditor(QAction *action, Editor *editor);

public slots:
    void selectEditorAt(int index);
    void changeEditor(QAction *action);

private:
    Document *_document;
    CommandManager *_manager;
    Ui::MainWindow *_ui;
};

#endif /* end of include guard: EDITOR_MANAGER__H */
