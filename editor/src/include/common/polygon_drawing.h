#ifndef POLYGON_DRAWING__H
#define POLYGON_DRAWING__H

#include <QtCore/QVector>
#include <QtCore/QPointF>

class QGraphicsItem;
class QGraphicsLineItem;
class QGraphicsEllipseItem;
class QGraphicsScene;

class PolygonDrawing {
public:
    PolygonDrawing(QGraphicsItem *background);
    ~PolygonDrawing();

    void setVisibleCurrentVertex(bool flag);
    void move(const QPointF &pos);
    void push(const QPointF &pos);
    void pop();
    void clear();
    bool empty() const;
    int  count() const;
    QVector<QPoint> get();

private:
    QGraphicsItem *background;
    QGraphicsEllipseItem *current_vertex;
    QGraphicsLineItem *current_line;
    QVector<QPair<QGraphicsEllipseItem*, QGraphicsLineItem*> > vertexes;
};

#endif /* end of include guard: POLYGON_DRAWING__H */
