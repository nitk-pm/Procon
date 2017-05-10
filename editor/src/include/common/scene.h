#ifndef SCENE_H
#define SCENE_H

#include <QtWidgets/QGraphicsScene>
#include <QtCore/QVector>

class QGraphicsSceneMouseEvent;

class Scene : public QGraphicsScene {
    Q_OBJECT

public:
    static const int BASE_SIZE = 10;
    static const int OFFSET = BASE_SIZE * 0.3;
    static const int RECT_SIZE = BASE_SIZE * 0.4;

public:
    explicit Scene(QObject *parent = 0);
    void createBackground(int width, int height);
    QPointF modifyPos(const QPointF &pos);
    void selectVertex(const QPointF &pos);
    void deselectVertex(const QPointF &pos);
    void addVertex(const QPointF &pos);
    void removeVertex(const QPointF &pos);

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QGraphicsPixmapItem *background;
    QVector<QGraphicsEllipseItem*> select_vertex_list;
};

#endif // SCENE_H
