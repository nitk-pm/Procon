#ifndef EDITOR_MANAGER__H
#define EDITOR_MANAGER__H

#include <QtWidgets/QActionGroup>
#include <QtCore/QList>

class Editor;

class EditorManager : public QActionGroup {
    Q_OBJECT

public:
    EditorManager();

    QList<Editor*> editors() const;
    void registerEditor(QAction *action, Editor *editor);

private:
    QList<Editor*> _editors;
};

#endif /* end of include guard: EDITOR_MANAGER__H */
