#ifndef VERTEX_OBJECT__H
#define VERTEX_OBJECT__H

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPen>
#include <QtCore/QRectF>

class VertexObject : public QGraphicsItem {
    Q_OBJECT

public:
    VertexObject(const QPointF &pos, QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPen pen;
};

#endif /* end of include guard: VERTEX_OBJECT__H */
