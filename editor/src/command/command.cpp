#include "command/command.h"
#include <QtWidgets/QUndoStack>

QUndoStack *Command::stack = new QUndoStack();
Command::Command(QUndoCommand *parent) : QUndoCommand(parent) {}
