#include "models/document.h"
#include "models/object_model.h"
#include "common/scene.h"

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
