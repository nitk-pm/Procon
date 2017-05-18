#ifndef DATA_CONTAINER__H
#define DATA_CONTAINER__H

#include "common/scene.h"

#include <QtCore/QHash>
#include <QtCore/QPointF>
#include <QtGui/QPolygonF>

// class QGraphicsPixmapItem;
// class QGraphicsEllipseItem;
class QGraphicsItemGroup;

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
    // QGraphicsEllipseItem* getVertex(const QPoint &pos) const;
    QGraphicsEllipseItem* addVertex(const QPoint &pos);

private:
    QGraphicsItemGroup *vertex_group;

/* 作成されたポリゴンの情報 */
public:
    // QGraphicsPolygonItem* getPolygon(const QPoint &pos) const;
    QGraphicsPolygonItem* addPolygon(const QPolygonF &polygon);

private:
    QGraphicsItemGroup *polygon_group;
};

#endif /* end of include guard: DATA_CONTAINER__H */
