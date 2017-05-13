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
    void addVertex(const QPointF &pos);
    void removeVertex(const QPointF &pos);
    void pushSelectVertex(const QPointF &pos);
    void popSelectVertex();
    int  createPolygon();
    void destroyPolygon(int index);

    const QVector<QPointF>& selectVartexes() const;
    QGraphicsPolygonItem* polygonItem(int id) const;


protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QSize background_size;
    QGraphicsPixmapItem *background;
    QMap<int, QGraphicsEllipseItem*> vertex_map;
    QVector<QPointF> select_vertexes;
    QVector<QGraphicsLineItem*> edge_list;
    QMap<int, QGraphicsPolygonItem*> polygon_list;
};

#endif // SCENE__H
