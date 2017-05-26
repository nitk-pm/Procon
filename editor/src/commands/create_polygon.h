#ifndef CREATE_POLYGON__H
#define CREATE_POLYGON__H

#include <QtWidgets/QUndoCommand>
#include <QtCore/QList>

class Document;
class PolygonObject;

class CreatePolygon : public QUndoCommand {
public:
    CreatePolygon(Document *doc, PolygonObject *poly, QUndoCommand *parent = 0);
    void undo();
    void redo();

private:
    Document *document;
    PolygonObject *polygon;
};

#endif /* end of include guard: CREATE_POLYGON__H */
