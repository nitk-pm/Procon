#include "models/document.h"
#include "models/object_model.h"
#include "models/polygon_object.h"
#include "common/scene.h"
#include "util/geometry.h"
#include "util/serialize.h"

#include <QtCore/QDebug>

Document::Document(Scene *scene, QObject *parent) : QObject(parent) {
    setScene(scene);
    setUpdated(false);
}

Document::~Document() {
    for (auto obj : object_list) {
        _scene->removeItem(obj);
        delete obj;
    }
    object_list.clear();
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
    setUpdated(true);
}

void Document::removeObject(ObjectModel *object) {
    if (object_list.removeOne(object)) {
        _scene->removeItem(object);
        setUpdated(true);
    }
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

void Document::clear() {
    for (auto &obj : object_list) {
        _scene->removeItem(obj);
    }
    object_list.clear();
    setUpdated(true);
}

QString Document::serialize() const {
    util::geometry::ConvexHull convex_hull;
    util::Serialize serialize;
    QStringList data_list;
    int count = 0;

    for (auto obj : object_list) {
        if (obj->id() == ObjectID::Polygon) {
            auto polygon = static_cast<PolygonObject*>(obj)->polygon();
            int size = polygon.count();
            util::geometry::Polygon poly;
            for (int i = 0; i < size; i++) {
                QPointF p = _scene->modifyDataPos(polygon[i]);
                poly.add(p.x(), p.y());
                convex_hull.add(p.x(), p.y());
            }
            serialize.set(poly);
            data_list.append(QString::fromStdString(serialize.get()));
            count++;
        }
    }
    serialize.set(util::geometry::Polygon(convex_hull.get()));
    data_list.append(QString::fromStdString(serialize.get()));

    data_list.insert(0, QString("%1").arg(count));
    QString result = data_list.join(":");
    qDebug() << result;
    return result;
}

void Document::deserialize(const QString &data) {

}

bool Document::isUpdated() const {
    return _updated;
}

void Document::setUpdated(bool updated) {
    _updated = updated;
}
