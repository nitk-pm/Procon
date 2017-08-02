#include "models/object_model.h"

#include <QtGui/QPainter>

ObjectModel::ObjectModel(QGraphicsItem *parent) : QGraphicsItem(parent) {
    _clicked = false;
    _protect = false;
}

void ObjectModel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    draw(painter);
    if (isClicked()) {
        QPen click_pen = QPen(clickedPenColor());
        click_pen.setStyle(Qt::DotLine);
        click_pen.setWidth(2);
        painter->setPen(click_pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());
    }
}

QRectF ObjectModel::boundingRect() const {
    return bounding_rect;
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

QColor ObjectModel::clickedPenColor() const {
    return clicked_color;
}

void ObjectModel::setClickedPenColor(const QColor &color) {
    clicked_color = color;
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
    if (_clicked) setZValue(zValue() + 1);
    else setZValue(zValue() - 1);
    update(bounding_rect);
}

bool ObjectModel::isProtect() const {
    return _protect;
}

void ObjectModel::enableProtect(bool protect) {
    _protect = protect;
}

void ObjectModel::setBoundingRect(const QRectF &rect) {
    bounding_rect = rect;
}
