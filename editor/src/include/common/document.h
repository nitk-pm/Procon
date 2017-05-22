#ifndef DOCUMENT__H
#define DOCUMENT__H

#include <QtCore/QObject>
#include <QtCore/QQList>

class Scene;
class VertexObject;
class PolygonObject;

class Document : public QObject {
    Q_OBJECT

public:
    Document(QObject *parent = 0) : QObject(parent), _scene(0) {}
    Document(Scene *scene, QObject *parent = 0);

    Scene* scene() const;
    void setScene(Scene *scene);

    void addVertexObject(VertexObject *vertex_obj) {}
    void addPolygonObject(PolygonObject *polygon_obj) {}

private:
    Scene *_scene;
};

#endif /* end of include guard: DOCUMENT__H */
