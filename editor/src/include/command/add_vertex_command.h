#ifndef ADD_VERTEX_COMMAND__H
#define ADD_VERTEX_COMMAND__H

#include "command/command.h"
#include <QtCore/QPointF>

class Scene;

class AddVertexCommand : public Command {
public:
    AddVertexCommand(Scene *scene, const QPointF &pos, QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();

private:
    Scene *scene;
    QPointF pos;
};

#endif // ADD_VERTEX_COMMAND__H
