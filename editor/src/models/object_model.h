#ifndef OBJECT_MODEL__H
#define OBJECT_MODEL__H

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtCore/QRectF>

class ObjectModel : public QGraphicsItem {
public:
    enum Type {
        Vertex,
        Polygon
    };

    ObjectModel(QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual QRectF boundingRect() const = 0;
    virtual void draw(QPainter *painter) = 0;
    virtual void drawClicked(QPainter *painter) = 0;

    QPen pen() const;
    void setPen(const QPen &pen);

    QBrush brush() const;
    void setBrush(const QBrush &brush);

    bool isClicked() const;
    void setClicked(bool clicked);

protected:
    QRectF bounding_rect;

private:
    QPen _pen;
    QBrush _brush;
    bool _clicked;
};

#endif /* end of include guard: OBJECT_MODEL__H */
