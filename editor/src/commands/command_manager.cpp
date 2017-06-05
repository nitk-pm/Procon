#include "commands/command_manager.h"

#include <QtWidgets/QAction>

CommandManager* CommandManager::instance() {
    static CommandManager manager;
    return &manager;
}

CommandManager::CommandManager() : QObject(nullptr) {
}

QUndoStack* CommandManager::undoStack() const {
    return _stack;
}

QAction* CommandManager::undoAction() const {
    return _undo_action;
}

void CommandManager::setUndoAction(QAction *undo_action) {
    _undo_action = undo_action;
    connect(_undo_action, SIGNAL(triggered()), _stack, SLOT(undo()));
    connect(_stack, SIGNAL(canUndoChanged(bool)), _undo_action, SLOT(setEnabled(bool)));
}

QAction* CommandManager::redoAction() const {
    return _redo_action;
}

void CommandManager::setRedoAction(QAction *redo_action) {
    _redo_action = redo_action;
    connect(_redo_action, SIGNAL(triggered()), _stack, SLOT(redo()));
    connect(_stack, SIGNAL(canRedoChanged(bool)), _redo_action, SLOT(setEnabled(bool)));
}

void CommandManager::registerCommand(QUndoCommand *command) {
    _stack->push(command);
}
