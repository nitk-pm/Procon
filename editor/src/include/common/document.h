#ifndef DOCUMENT__H
#define DOCUMENT__H

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtCore/QHash>
#include <QtCore/QQList>
#include <QtGui/QPolygonF>

class QGraphicsItem;
class QGraphicsEllipseItem;
class QGraphicsPolygonItem;

class Document : public QObject {
    Q_OBJECT

public:
    Document(QObject *parent = 0) : QObject(parent) {}
    Document(int width, int height, QGraphicsItem *background, QObject *parent = 0);

    int width() const;
    int height() const;
    QGraphicsItem* background() const;

    void addVertex(const QPointF &pos);
    void addPolygon(const QList<QPointF> &pos_list);
    void remove(QGraphicsItem *item);

private:
    int _width;
    int _height;
    QGraphicsItem *_background;
};

#endif /* end of include guard: DOCUMENT__H */
