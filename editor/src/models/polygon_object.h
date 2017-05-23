#ifndef POLYGON_OBJECT__H
#define POLYGON_OBJECT__H

#include "models/object_model.h"
#include <QtGui/QPolygonF>

class PolygonObject : public ObjectModel {
public:
    PolygonObject(const QPolygonF &polygon, QGraphicsItem *parent = 0);

    QRectF boundingRect() const;
    void draw(QPainter *painter);
    void drawClicked(QPainter *painter);

    QPolygonF polygon() const;
    void setPolygon(const QPolygonF &polygon);

private:
    QPolygonF _polygon;
};

#endif /* end of include guard: POLYGON_OBJECT__H */
