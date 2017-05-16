#ifndef DATA_CONTAINER__H
#define DATA_CONTAINER__H

#include <QtCore/QMap>
#include <QtCore/QPointF>
#include <QtGui/QPolygonF>

class Scene;
class QGraphicsEllipseItem;
class QGraphicsPolygonItem;

class DataContainer {

/* コンストラクター */
public:
    DataContainer() {}
    DataContainer(Scene *scene, int width, int height);

/* 基本設定 */
public:
    void   set(Scene *scene, int width, int height);
    Scene* scene() const;
    int    width() const;
    int    height() const;
    int    convertPosToIndex(const QPointF &pos) const;

private:
    Scene *_scene;
    int    _width;
    int    _height;

/* プロットされた頂点の情報 */
public:
    QGraphicsEllipseItem* getVertex(const QPointF &pos) const;
    QGraphicsEllipseItem* addVertex(const QPointF &pos);
    void                  removeVertex(const QPointF &pos);
    bool                  containsVertex(const QPointF &pos) const;

private:
    QMap<int, QGraphicsEllipseItem*> vertex_map;

/* 作成されたポリゴンの情報 */
public:
    QGraphicsPolygonItem* getPolygon(int id) const;
    int                   addPolygon(const QPolygonF &polygon);
    void                  removePolygon(int id);

private:
    QMap<int, QGraphicsPolygonItem*> polygon_list;
};

#endif /* end of include guard: DATA_CONTAINER__H */
