#include "models/document.h"
#include "models/object_model.h"
#include "models/polygon_object.h"
#include "common/scene.h"
#include "util/convex_hull_of_graph.h"

#include <QtCore/QDebug>

Document::Document(Scene *scene, QObject *parent) : QObject(parent) {
    setScene(scene);
}

Scene* Document::scene() const {
    return _scene;
}

void Document::setScene(Scene *scene) {
    _scene = scene;
}

void Document::addObject(ObjectModel *object) {
    object_list.append(object);
    _scene->addItem(object);
}

void Document::removeObject(ObjectModel *object) {
    object_list.removeOne(object);
    _scene->removeItem(object);
}

ObjectModel* Document::getObject(const QPointF &pos) {
    auto item_list = _scene->items(pos);
    item_list.pop_back();
    for (auto item : item_list) {
        ObjectModel *obj = static_cast<ObjectModel*>(item);
        if (obj->id() == ObjectID::Vertex) return obj;
        if (obj->id() == ObjectID::Polygon) {
            auto polygon_object = static_cast<PolygonObject*>(obj);
            if (polygon_object->containsPoint(pos)) return obj;
        }
    }
    return nullptr;
}

QList<ObjectModel*> Document::objectList() const {
    return object_list;
}

QString Document::serialize() const {
    ConvexHullOfGraph convex_hull;

    for (auto obj : object_list) {
        if (obj->id() == ObjectID::Polygon) {
            auto polygon = static_cast<PolygonObject*>(obj)->polygon();
            int size = polygon.count();
            for (int i = 0; i < size; i++) {
                QPointF p1 = _scene->modifyDataPos(polygon[i]);
                QPointF p2 = _scene->modifyDataPos(polygon[(i + 1) % size]);
                convex_hull.addEdge(p1.x(), p1.y(), p2.x(), p2.y());
            }
        }
    }

    auto frame = convex_hull.get();
    for (auto p : frame) {
        qDebug("x = %lf, y = %lf", p.first, p.second);
    }
    return QString();
}

void Document::deserialize(const QString &data) {

}
