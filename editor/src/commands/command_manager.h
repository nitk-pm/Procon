#ifndef COMMAND_MANAGER__H
#define COMMAND_MANAGER__H

#include <QtCore/QObject>
#include <QtWidgets/QUndoStack>

class QUndoCommand;
class QAction;

class CommandManager : public QObject {
    Q_OBJECT

public:
    CommandManager(QUndoStack *stack);
    ~CommandManager();

    QUndoStack* undoStack() const;

    QAction* undoAction() const;
    void setUndoAction(QAction *undo_action);

    QAction* redoAction() const;
    void setRedoAction(QAction *redo_action);

    void registerCommand(QUndoCommand *command);

private:
    QUndoStack *_stack;
    QAction *_undo_action;
    QAction *_redo_action;
};

#endif /* end of include guard: COMMAND_MANAGER__H */
