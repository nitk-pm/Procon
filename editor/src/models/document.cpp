#include "models/document.h"
#include "models/object_model.h"
#include "models/polygon_object.h"
#include "common/scene.h"
#include "util/geometry.h"
#include "util/serialize.h"

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
    if (!item_list.empty()) {
        item_list.pop_back();
        for (auto item : item_list) {
            ObjectModel *obj = static_cast<ObjectModel*>(item);
            if (obj->id() == ObjectID::Vertex) return obj;
            if (obj->id() == ObjectID::Polygon) {
                auto polygon_object = static_cast<PolygonObject*>(obj);
                if (polygon_object->containsPoint(pos)) return obj;
            }
        }
    }
    return nullptr;
}

QList<ObjectModel*> Document::objectList() const {
    return object_list;
}

QString Document::serialize() const {
    using namespace rucm;
    geometry::ConvexHull convex_hull;

    for (auto obj : object_list) {
        if (obj->id() == ObjectID::Polygon) {
            auto polygon = static_cast<PolygonObject*>(obj)->polygon();
            int size = polygon.count();
            for (int i = 0; i < size; i++) {
                QPointF p = _scene->modifyDataPos(polygon[i]);
                convex_hull.add(p.x(), p.y());
            }
        }
    }

    auto frame = convex_hull.get();

    qDebug("frame point");
    for (auto p : frame) {
        qDebug("x = %lf, y = %lf", p.x(), p.y());
    }
    auto serialize = rucm::Serialize(rucm::geometry::Polygon(frame));
    qDebug() << QString::fromStdString(serialize.get());
    return QString();
}

void Document::deserialize(const QString &data) {

}
