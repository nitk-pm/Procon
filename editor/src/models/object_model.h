#ifndef OBJECT_MODEL__H
#define OBJECT_MODEL__H

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtCore/QRectF>

enum ObjectID {
    Vertex,
    Polygon
};

class ObjectModel : public QGraphicsItem {
public:
    ObjectModel(QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void draw(QPainter *painter) = 0;
    
    QRectF boundingRect() const;

    ObjectID id() const;
    void setId(ObjectID id);

    QPen pen() const;
    void setPen(const QPen &pen);

    QColor clickedPenColor() const;
    void setClickedPenColor(const QColor &color);

    QBrush brush() const;
    void setBrush(const QBrush &brush);

    bool isClicked() const;
    void setClicked(bool clicked);

    bool isProtect() const;
    void enableProtect(bool protect);

protected:
    void setBoundingRect(const QRectF &rect);

private:
    QRectF bounding_rect;
    QColor clicked_color;
    ObjectID _id;
    QPen _pen;
    QBrush _brush;
    bool _clicked;
    bool _protect;
};

#endif /* end of include guard: OBJECT_MODEL__H */
