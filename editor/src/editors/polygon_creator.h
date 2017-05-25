#ifndef POLYGON_CREATOR__H
#define POLYGON_CREATOR__H

#include "editors/editor.h"

#include <QtCore/QList>

class ObjectModel;

class PolygonCreator : public Editor {
public:
    PolygonCreator() {}
    void sceneEvent(QGraphicsSceneMouseEvent *event);

    void finish();

private:
    QList<ObjectModel*> object_list;
};

#endif /* end of include guard: POLYGON_CREATOR__H */
