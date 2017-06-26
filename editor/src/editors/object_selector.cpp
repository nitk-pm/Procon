#include "ui_window_layout.h"
#include "editors/object_selector.h"
#include "models/document.h"
#include "models/object_model.h"
#include "commands/remove_object.h"
#include "commands/command_manager.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QAction>

void ObjectSelector::sceneEvent(QGraphicsSceneMouseEvent *event) {
    if (event->type() == QEvent::GraphicsSceneMouseRelease) {
        auto obj = document()->getObject(event->scenePos());
        if (obj && !obj->isClicked()) {
            if (!object_list.empty() && event->modifiers() != Qt::ControlModifier) reselect(obj);
            else select(obj);
        }
        else deselect();
    }
}

void ObjectSelector::connectAction(Ui::MainWindow *ui) {
    connect(this, SIGNAL(setDeleteActionFlag(bool)), ui->action_delete, SLOT(setEnabled(bool)));
    connect(ui->action_delete, SIGNAL(triggered()), this, SLOT(remove()));
}

void ObjectSelector::finish() {
    deselect();
}

void ObjectSelector::select(ObjectModel *obj) {
    obj->setClicked(true);
    obj->setClickedPenColor(Qt::blue);
    object_list.append(obj);
    setDeleteActionFlag(true);
}

void ObjectSelector::reselect(ObjectModel *obj) {
    deselect();
    select(obj);
}

void ObjectSelector::deselect() {
    for (auto obj : object_list) obj->setClicked(false);
    object_list.clear();
    setDeleteActionFlag(false);
}

void ObjectSelector::remove() {
    commandManager()->registerCommand(new RemoveObject(document(), object_list));
    deselect();
}
