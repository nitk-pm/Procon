#ifndef EDITOR_MANAGER__H
#define EDITOR_MANAGER__H

#include <QtWidgets/QActionGroup>
#include <QtCore/QList>

class Editor;
class Document;

class EditorManager : public QActionGroup {
    Q_OBJECT

public:
    static EditorManager* instance();

    QList<Editor*> editors() const;
    void registerEditor(QAction *action, Editor *editor);
    void setDocument(Document *document);

private:
    Q_DISABLE_COPY(EditorManager);
    EditorManager();

    QList<Editor*> _editors;
};

#endif /* end of include guard: EDITOR_MANAGER__H */
