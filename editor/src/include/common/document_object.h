#ifndef DOCUMENT_OBJECT__H
#define DOCUMENT_OBJECT__H

#include <QtWidgets/QGraphicsItem>

class DocumentObject : public QGraphicsItem {
public:
    DocumentObject(QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void draw(QPainter *painter) = 0;
    virtual void drawSelected(QPainter *painter) = 0;

    bool isClicked() const;
    void setClicked(bool clicked);

private:
    bool _clicked;
};

#endif /* end of include guard: DOCUMENT_OBJECT__H */
