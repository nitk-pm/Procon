#ifndef DOCUMENT__H
#define DOCUMENT__H

#include <QtCore/QObject>
#include <QtCore/QList>

class Scene;
class ObjectModel;

class Document : public QObject {
    Q_OBJECT

public:
    Document(QObject *parent = 0) : QObject(parent), _scene(0), _updated(false) {}
    Document(Scene *scene, QObject *parent = 0);
    ~Document();

    Scene* scene() const;
    void setScene(Scene *scene);

    void addObject(ObjectModel *object);
    void removeObject(ObjectModel *object);
    ObjectModel* getObject(const QPointF &pos);
    QList<ObjectModel*> objectList() const;
    void clear();

    QString serialize() const;
    void deserialize(const QString &data);

    bool isUpdated() const;
    void setUpdated(bool updated);

private:
    bool _updated;
    Scene *_scene;
    QList<ObjectModel*> object_list;
};

#endif /* end of include guard: DOCUMENT__H */
