#ifndef DATA_CONTAINER__H
#define DATA_CONTAINER__H

#include "common/scene.h"

#include <QtCore/QHash>
#include <QtCore/QPointF>
#include <QtGui/QPolygonF>

// class QGraphicsPixmapItem;
// class QGraphicsEllipseItem;
// class QGraphicsPolygonItem;

class DataContainer {

/* 定数 */
public:
    static const int BASE_SIZE = 10;
    static const int RECT_SIZE = BASE_SIZE * 0.4;

/* コンストラクター */
public:
    DataContainer() {}
    DataContainer(Scene *scene, int width, int height);

/* 基本メソッド */
public:
    void                 set(Scene *scene, int width, int height);
    Scene*               scene() const;
    QGraphicsPixmapItem* background() const;
    int                  width() const;
    int                  height() const;

    QPoint  convertRealPosToVirtualPos(const QPointF &pos);
    QPointF convertVirtualPosToRealPos(const QPoint &pos);
    QPointF convertVirtualPosToRealPosCenter(const QPoint &pos);
    QPointF modifyPosCenter(const QPointF &pos);

private:
    Scene               *_scene;
    QGraphicsPixmapItem *_background;
    int                  _width;
    int                  _height;

/* プロットされた頂点の情報 */
public:
    QGraphicsEllipseItem* getVertex(const QPoint &pos) const;
    QGraphicsEllipseItem* addVertex(const QPoint &pos);
    void                  removeVertex(const QPoint &pos);
    bool                  containsVertex(const QPoint &pos) const;

private:
    QHash<QPoint, QGraphicsEllipseItem*> vertex_map;

/* 作成されたポリゴンの情報 */
public:
    QGraphicsPolygonItem* getPolygon(int id) const;
    int                   addPolygon(const QPolygonF &polygon);
    void                  removePolygon(int id);

private:
    QHash<int, QGraphicsPolygonItem*> polygon_list;
};

inline uint qHash(const QPoint &key) {
    return qHash(static_cast<qint64>(key.x() << 32 | key.y()));
}

#endif /* end of include guard: DATA_CONTAINER__H */
