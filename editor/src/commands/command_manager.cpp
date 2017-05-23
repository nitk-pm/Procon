#include "commands/command_manager.h"

#include <QtWidgets/QAction>

CommandManager* CommandManager::instance() {
    static CommandManager manager;
    return &manager;
}

CommandManager::CommandManager() : QObject(nullptr) {
    stack = new QUndoStack();
}

QAction* CommandManager::undoAction() const {
    return _undo_action;
}

void CommandManager::setUndoAction(QAction *undo_action) {
    _undo_action = undo_action;
    connect(_undo_action, SIGNAL(triggered()), stack, SLOT(undo()));
    connect(stack, SIGNAL(canUndoChanged(bool)), _undo_action, SLOT(setEnabled(bool)));
}

QAction* CommandManager::redoAction() const {
    return _redo_action;
}

void CommandManager::setRedoAction(QAction *redo_action) {
    _redo_action = redo_action;
    connect(_redo_action, SIGNAL(triggered()), stack, SLOT(undo()));
    connect(stack, SIGNAL(canUndoChanged(bool)), _redo_action, SLOT(setEnabled(bool)));
}

void CommandManager::registerCommand(QUndoCommand *command) {
    stack->push(command);
}
