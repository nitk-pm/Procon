#ifndef SCENE__H
#define SCENE__H

#include <QtWidgets/QGraphicsScene>

class QGraphicsPixmapItem;
class QGraphicsSceneMouseEvent;
class QAction;
class VertexObject;

class Scene : public QGraphicsScene {
    Q_OBJECT

public:
    static const int BASE_SZIE = 10;
    static const QPointF OFFSET = QPointF(BASE_SIZE, BASE_SIZE) / 2;
    static const QRectF VERTEX_RECT = QRectF(0, 0, BASE_SIZE * 0.4, BASE_SIZE * 0.4);

    explicit Scene(QObject *parent = 0);
    explicit Scene(int width, int height, QObject *parent = 0);
    void setBoardSize(int width, int height);
    QPointF modifyPos(const QPointF &pos);
    void drawVertex()

public slots:
    void changeEditor(QAction *action);

private:
    Editor *current_editor;
    QGraphicsPixmapItem *background;
};

#endif // SCENE__H
