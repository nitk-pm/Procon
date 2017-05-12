#ifndef SCENE__H
#define SCENE__H

#include <QtWidgets/QGraphicsScene>
#include <QtCore/QVector>
#include <QtCore/QMap>

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
    int  convertPosToIndex(const QPointF &pos);
    void selectVertex(const QPointF &pos);
    void deselectVertex();
    void addVertex(const QPointF &pos);
    void removeVertex(const QPointF &pos);
    int  createPolygon();
    void destroyPolygon(int index);

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QSize background_size;
    QGraphicsPixmapItem *background;
    QMap<int, QGraphicsEllipseItem*> vertex_map;
    QVector<QGraphicsEllipseItem*> select_vertex_list;
    QVector<QGraphicsLineItem*> line_list;
    QVector<QGraphicsPolygonItem*> polygon_list;
};

#endif // SCENE__H
