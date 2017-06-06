#include "editors/polygon_creator.h"
#include "models/document.h"
#include "models/polygon_object.h"
#include "commands/command_manager.h"
#include "commands/create_polygon.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QMessageBox>
#include <QtCore/QDebug>

void PolygonCreator::sceneEvent(QGraphicsSceneMouseEvent *event) {
    if (event->type() == QEvent::GraphicsSceneMousePress) {
        auto obj = document()->getObject(event->scenePos());
        if (obj && obj->id() == ObjectID::Vertex) {
            if (obj == object_list.first()) {
                QPolygonF polygon;
                for (auto p : object_list) {
                    polygon << (p->pos() + p->boundingRect().center());
                }
                commandManager()->registerCommand(new CreatePolygon(document(), new PolygonObject(polygon)));
                finish();
            }
            else if (obj->isClicked()) {
                obj->setClicked(false);
                object_list.removeOne(obj);
            }
            else if (object_list.count() >= 16) {
                QMessageBox box;
                box.setIcon(QMessageBox::Warning);
                box.setWindowTitle(tr("warning"));
                box.setText(tr("それ以上は選択できません。"));
                box.setStandardButtons(QMessageBox::Ok);
                box.exec();
            }
            else if (!obj->isClicked()) {
                obj->setClicked(true);
                obj->setClickedPenColor(Qt::green);
                object_list.append(obj);
            }
        }
    }
}

void PolygonCreator::finish() {
    for (auto obj : object_list) {
        obj->setClicked(false);
    }
    object_list.clear();
}
