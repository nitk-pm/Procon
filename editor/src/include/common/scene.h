#ifndef SCENE_H
#define SCENE_H

#include <QtWidgets/QGraphicsScene>

class QGraphicsSceneMouseEvent;
class QGraphicsPixmapItem;
class PolygonDrawing;

class Scene : public QGraphicsScene {
    Q_OBJECT

public:
    static const int BASE_SIZE = 10;
    static const int OFFSET = BASE_SIZE * 0.3;
    static const int RECT_SIZE = BASE_SIZE * 0.4;

public:
    explicit Scene(QObject *parent = 0);
    void setVisibleCurrentVertex(bool flag);
    void createBackground(int width, int height);

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    PolygonDrawing *drawing;
    QGraphicsPixmapItem *background;
};

#endif // SCENE_H
