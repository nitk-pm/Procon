#ifndef COMMAND_MANAGER__H
#define COMMAND_MANAGER__H

#include <QtCore/QObject>
#include <QtWidgets/QUndoStack>

class QUndoCommand;
class QAction;

class CommandManager : public QObject {
    Q_OBJECT

public:
    static CommandManager* instance();

    QAction* undoAction() const;
    void setUndoAction(QAction *undo_action);

    QAction* redoAction() const;
    void setRedoAction(QAction *redo_action);

    void registerCommand(QUndoCommand *command);

private:
    Q_DISABLE_COPY(CommandManager);
    CommandManager();

    QAction *_undo_action;
    QAction *_redo_action;
    QUndoStack *stack;
};

#endif /* end of include guard: COMMAND_MANAGER__H */
