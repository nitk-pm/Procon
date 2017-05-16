#ifndef SELECT_MODE__H
#define SELECT_MODE__H

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtCore/QEvent>
#include <QtCore/QVector>

class DataContainer;
class QGraphicsSceneMouseEvent;
class QGraphicsItem;
class QAbstractGraphicsShapeItem;

class SelectMode : public QObject {
    Q_OBJECT

public:
    explicit SelectMode(DataContainer *container, QObject *parent = 0);
    virtual bool eventFilter(QObject *watched, QEvent *event);

    void selectEvent(QGraphicsSceneMouseEvent *event);
    void moveStartEvent(QGraphicsSceneMouseEvent *event);
    void moveEvent(QGraphicsSceneMouseEvent *event);

    void select(QGraphicsItem *item);
    void deselect();
    void reselect(QGraphicsItem *item);
    void remove();

signals:
    void setDeleteActionFlag(bool flag);

private:
    DataContainer                       *container;
    QVector<QAbstractGraphicsShapeItem*> select_items;
    QPointF                              prev_pos;
    bool                                 can_move_item;
};

#endif /* end of include guard: SELECT_MODE__H */
