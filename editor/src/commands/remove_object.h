#ifndef REMOVE_OBJECT__H
#define REMOVE_OBJECT__H

#include <QtWidgets/QUndoCommand>
#include <QtCore/QList>

class Document;
class ObjectModel;

class RemoveObject : public QUndoCommand {
public:
    RemoveObject(Document *doc, QList<ObjectModel*> list);
    void undo();
    void redo();

private:
    Document *document;
    QList<ObjectModel*> remove_list;
};

#endif /* end of include guard: REMOVE_OBJECT__H */
