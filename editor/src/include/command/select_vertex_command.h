#ifndef SELECT_VERTEX_COMMAND__H
#define SELECT_VERTEX_COMMAND__H

#include "command/command.h"
#include <QtCore/QPointF>

class Scene;

class SelectVertexCommand : public Command {
public:
    SelectVertexCommand(Scene *scene, const QPointF &pos, QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();

private:
    Scene  *scene;
    QPointF pos;
};

#endif /* end of include guard: SELECT_VERTEX_COMMAND__H */
