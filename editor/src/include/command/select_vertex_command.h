#ifndef SELECT_VERTEX_COMMAND__H
#define SELECT_VERTEX_COMMAND__H

#include "command/command.h"

class Scene;

class SelectVertexCommand : public Command {
public:
    SelectVertexCommand(Scene *scene, const QPointF &pos, QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();
};

#endif /* end of include guard: SELECT_VERTEX_COMMAND__H */
