#include "common/link_node.h"

LinkNode::LinkNode(const QPointF &pos) : _pos(pos) {}

LinkNode::~LinkNode() {}

void LinkNode::link(LinkNode *other) {
}

void LinkNode::unlink(LinkNode *other) {}

QPointF LinkNode::pos() const {
    return _pos;
}
