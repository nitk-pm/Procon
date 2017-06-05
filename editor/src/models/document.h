#ifndef DOCUMENT__H
#define DOCUMENT__H

#include <QtCore/QObject>
#include <QtCore/QList>

class Scene;
class ObjectModel;

class Document : public QObject {
    Q_OBJECT

public:
    Document(Scene *scene, QObject *parent = 0);
    ~Document();

    QString filename() const;
    void setFilename(QString filename);

    bool isUpdated() const;
    void setUpdated(bool updated);

    Scene* scene() const;
    void setScene(Scene *scene);

    void addObject(ObjectModel *object);
    void removeObject(ObjectModel *object);
    ObjectModel* getObject(const QPointF &pos);
    QList<ObjectModel*> objectList() const;
    void clear();

    QString serialize(bool cut_offset = true) const;
    void deserialize(const QString &data);

private:
    QString _filename;
    bool _updated;
    Scene *_scene;
    QList<ObjectModel*> object_list;
};

#endif /* end of include guard: DOCUMENT__H */
