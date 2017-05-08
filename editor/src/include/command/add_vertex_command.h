#ifndef ADD_VERTEX_COMMAND__H
#define ADD_VERTEX_COMMAND__H

#include "command/command.h"
#include <QtCore/QPointF>

class PolygonDrawing;

class AddVertexCommand : public Command {
public:
    AddVertexCommand(PolygonDrawing *drawing, const QPointF &pos, QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();

private:
    PolygonDrawing *drawing;
    QPointF pos;
};

#endif // ADD_VERTEX_COMMAND__H
