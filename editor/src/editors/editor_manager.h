#ifndef EDITOR_MANAGER__H
#define EDITOR_MANAGER__H

#include <QtWidgets/QActionGroup>
#include <QtCore/QList>
#include <QtCore/QStack>

class Editor;
class Document;
class Scene;

class EditorManager : public QActionGroup {
    Q_OBJECT

public:
    static EditorManager* instance();

    void registerEditor(QAction *action, Editor *editor);
    void setDocument(Document *document);
    void setScene(Scene *scene);

public slots:
    void selectEditorAt(int index);
    void changeEditor(QAction *action);

private:
    Q_DISABLE_COPY(EditorManager);
    EditorManager();

    Scene *_scene;
    QStack<Editor*> stack;
};

#endif /* end of include guard: EDITOR_MANAGER__H */
