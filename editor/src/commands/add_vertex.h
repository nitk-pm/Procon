#ifndef ADD_VERTEX__H
#define ADD_VERTEX__H

#include <QtWidgets/QUndoCommand>

class Document;
class VertexObject;

class AddVertex : public QUndoCommand {
public:
    AddVertex(Document *doc, VertexObject *obj, QUndoCommand *parent = 0);
    ~AddVertex();
    void undo();
    void redo();

private:
    Document *document;
    VertexObject *object;
};

#endif /* end of include guard: ADD_VERTEX__H */
