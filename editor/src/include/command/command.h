#ifndef COMMAND__H
#define COMMAND__H

#include <QtWidgets/QUndoCommand>

class QUndoStack;

class Command : public QUndoCommand {
public:
    static QUndoStack *stack;
    Command(QUndoCommand *parent = 0);
    virtual void undo() {}
    virtual void redo() {}
};

#endif // COMMAND__H
