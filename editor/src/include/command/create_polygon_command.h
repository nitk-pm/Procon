#ifndef CREATE_POLYGON_COMMAND__H
#define CREATE_POLYGON_COMMAND__H

#include "command/command.h"
#include <QtCore/QVector>
#include <QtCore/QPointF>

class Scene;

class CreatePolygonCommand : public Command {
public:
    CreatePolygonCommand(Scene *scene, QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();

private:
    Scene *scene;
    int id;
};

#endif // CREATE_POLYGON_COMMAND__H
