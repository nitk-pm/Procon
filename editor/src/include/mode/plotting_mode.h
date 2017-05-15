#ifndef PLOTTING_MODE__H
#define PLOTTING_MODE__H

#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtCore/QPointF>

class DataContainer;
class QGraphicsSceneMouseEvent;

class PlottingMode : public QObject {
    Q_OBJECT

public:
    explicit PlottingMode(DataContainer *container, QObject *parent = 0);
    virtual bool eventFilter(QObject *watched, QEvent *event);

    void plotting(QGraphicsSceneMouseEvent *event);

private:
    DataContainer *container;
};

#endif /* end of include guard: PLOTTING_MODE__H */
