#ifndef VERTEX_OBJECT__H
#define VERTEX_OBJECT__H

#include "models/object_model.h"

class VertexObject : public ObjectModel {
public:
    VertexObject(const QPointF &pos, QGraphicsItem *parent = 0);
    void draw(QPainter *painter);
};

#endif /* end of include guard: VERTEX_OBJECT__H */
