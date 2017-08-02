#ifndef EDITOR__H
#define EDITOR__H

#include <QtCore/QObject>

namespace Ui {
class MainWindow;
}

class QEvent;
class QGraphicsSceneMouseEvent;
class Document;
class EditorManager;
class CommandManager;

class Editor : public QObject {
    Q_OBJECT

public:
    Editor(QObject *parent = 0);

private:
    bool eventFilter(QObject *watched, QEvent *event);

public:
    Document* document() const;
    void setDocument(Document *document);

    EditorManager* editorManager() const;
    void setEditorManager(EditorManager *manager);

    CommandManager* commandManager() const;
    void setCommandManager(CommandManager *manager);

    virtual void sceneEvent(QGraphicsSceneMouseEvent *event) {}
    virtual void connectAction(Ui::MainWindow *ui) {}

public slots:
    virtual void begin() {}
    virtual void finish() {}

private:
    Document *_document;
    EditorManager *_editor_manager;
    CommandManager *_command_manager;
};

#endif /* end of include guard: EDITOR__H */
