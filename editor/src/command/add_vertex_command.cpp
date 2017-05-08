#include "command/add_vertex_command.h"
#include "common/polygon_drawing.h"
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsScene>
#include <QtCore/QDebug>


AddVertexCommand::AddVertexCommand(PolygonDrawing *drawing, const QPointF &pos, QUndoCommand *parent) : Command(parent), drawing(drawing), pos(pos) {

}

void AddVertexCommand::undo() {
    drawing->pop();
}

void AddVertexCommand::redo() {
    drawing->push(pos);
}
