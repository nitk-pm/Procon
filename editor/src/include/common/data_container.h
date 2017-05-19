#ifndef DATA_CONTAINER__H
#define DATA_CONTAINER__H

#include "common/scene.h"

#include <QtCore/QHash>
#include <QtCore/QPointF>
#include <QtGui/QPolygonF>

class DataContainer {

/* 定数 */
public:
    static const int BASE_SIZE = 10;
    static const int RECT_SIZE = BASE_SIZE * 0.4;

/* コンストラクタ・デストラクタ */
public:
    DataContainer() {}
    DataContainer(Scene *scene, int width, int height);
    ~DataContainer();

/* 基本メソッド */
public:
    void                 set(Scene *scene, int width, int height);
    Scene*               scene() const;
    QGraphicsPixmapItem* background() const;
    int                  width() const;
    int                  height() const;

    QPoint  convertRealPosToVirtualPos(const QPointF &pos);
    QPointF convertVirtualPosToRealPos(const QPoint &pos);
    QPointF modifyPosCenter(const QPointF &pos);

private:
    Scene               *_scene;
    QGraphicsPixmapItem *_background;
    int                  _width;
    int                  _height;

/* 頂点及びポリゴンの追加と取得 */
public:
    QGraphicsEllipseItem* addVertex(const QPoint &pos);
    QGraphicsPolygonItem* addPolygon(const QPolygonF &polygon);

    QGraphicsItem* getItem(const QPoint &pos);
};

#endif /* end of include guard: DATA_CONTAINER__H */
