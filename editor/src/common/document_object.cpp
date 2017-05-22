#include "common/document_object.h"

DocumentObject::DocumentObject(QGraphicsItem *parent) : QGraphicsItem(parent) {
    _clicked = false;
}

void DocumentObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    draw(painter);
    if (_clicked) drawSelected(painter);
}

bool DocumentObject::isClicked() const {
    return _clicked;
}

void DocumentObject::setClicked(bool clicked) {
    _clicked = clicked;
}
