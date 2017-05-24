#include "models/object_model.h"

ObjectModel::ObjectModel(QGraphicsItem *parent) : QGraphicsItem(parent) {
    _clicked = false;
}

void ObjectModel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    draw(painter);
    if (_clicked) drawClicked(painter);
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
    update(bounding_rect);
}
