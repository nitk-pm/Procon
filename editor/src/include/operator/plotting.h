#ifndef PLOTTING__H
#define PLOTTING__H

#include "command/command.h"

#include <QtCore/QObject>
#include <QtCore/QPointF>

class Scene;

class Plotting : public QObject {
    Q_OBJECT

public:
    explicit Plotting(Scene *scene, QObject *parent = 0);
    virtual bool eventFilter(QObject *watched, QEvent *event);

private:
    class AddVertex : public Command {
    public:
        explicit AddVertex(Plotting *plotting, QUndoCommand *parent = 0);
        virtual void undo();
        virtual void redo();

    private:
        Plotting *plotting;
    };

private:
    Scene *scene;
};

#endif /* end of include guard: PLOTTING__H */
