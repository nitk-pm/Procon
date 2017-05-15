#ifndef SELECT_MODE__H
#define SELECT_MODE__H

#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtCore/QVector>

class DataContainer;
class QGraphicsSceneMouseEvent;
class QAbstractGraphicsShapeItem;

class SelectMode : public QObject {
public:
    explicit SelectMode(DataContainer *container, QObject *parent = 0);
    virtual bool eventFilter(QObject *watched, QEvent *event);

    void select(QGraphicsSceneMouseEvent *event);

private:
    DataContainer *container;
    QVector<QAbstractGraphicsShapeItem*> select_items;
};

#endif /* end of include guard: SELECT_MODE__H */
