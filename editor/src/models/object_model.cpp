#include "models/object_model.h"

ObjectModel::ObjectModel(QGraphicsItem *parent) : QGraphicsItem(parent) {
    _clicked = false;
}

void ObjectModel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    draw(painter);
}

ObjectID ObjectModel::id() const {
    return _id;
}

void ObjectModel::setId(ObjectID id) {
    _id = id;
}

QPen ObjectModel::pen() const {
    return _pen;
}

void ObjectModel::setPen(const QPen &pen) {
    _pen = pen;
}

QBrush ObjectModel::brush() const {
    return _brush;
}

void ObjectModel::setBrush(const QBrush &brush) {
    _brush = brush;
}

bool ObjectModel::isClicked() const {
    return _clicked;
}

void ObjectModel::setClicked(bool clicked) {
    _clicked = clicked;
    if (_clicked) {
        setZValue(zValue() + 1);
    }
    else {
        setZValue(zValue() - 1);
    }
    update(bounding_rect);
}
