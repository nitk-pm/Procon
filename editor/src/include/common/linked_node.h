#ifndef LINKED_NODE__H
#define LINKED_NODE__H

#include <QtCore/QPointF>
#include <QtCore/QList>

class QGraphicsEllipseItem;
class QGraphicsLineItem;

class LinkedNode {
public:
    void link(LinkedNode* other);

private:
    QList<LinkedNode*> linked_list;
    QGraphicsEllipseItem *vertex_item;
};

#endif /* end of include guard: LINKED_NODE__H */
