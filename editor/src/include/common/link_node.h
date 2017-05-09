#ifndef LINKED_NODE__H
#define LINKED_NODE__H

#include <QtCore/QMetaType>
#include <QtCore/QPointF>
#include <QtCore/QList>

class LinkNode {
public:
    LinkNode() {}
    LinkNode(const QPointF &pos);
    ~LinkNode();
    void link(LinkNode* other);
    void unlink(LinkNode* other);
    QPointF pos() const;

private:
    QList<LinkNode*> linked_list;
    QPointF _pos;
};

Q_DECLARE_METATYPE(LinkNode)

#endif /* end of include guard: LINKED_NODE__H */
