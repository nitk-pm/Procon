#ifndef EDITOR_MANAGER__H
#define EDITOR_MANAGER__H

#include <QtWidgets/QActionGroup>
#include <QtCore/QList>

class Editor;
class Document;
class Scene;

class EditorManager : public QActionGroup {
public:
    static EditorManager* instance();

    QList<Editor*> editors() const;
    void registerEditor(QAction *action, Editor *editor, bool default_clicked = false);
    void setDocument(Document *document);
    void connectScene(Scene *scene);

private:
    Q_DISABLE_COPY(EditorManager);
    EditorManager();

    QAction *default_checked_action;
    QList<Editor*> _editors;
};

#endif /* end of include guard: EDITOR_MANAGER__H */
