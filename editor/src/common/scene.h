#ifndef SCENE__H
#define SCENE__H

#include <QtWidgets/QGraphicsScene>

class QGraphicsPixmapItem;
class QGraphicsSceneMouseEvent;
class QAction;

class Scene : public QGraphicsScene {
    Q_OBJECT

public:
    static const int BASE_SIZE = 10;

    Scene(QObject *parent = 0);
    Scene(int width, int height, QObject *parent = 0);
    void setBoardSize(int width, int height);
    QPointF modifyPos(const QPointF &pos);

public slots:
    void changeEditor(QAction *action);

private:
    QObject *current_editor;
    QGraphicsPixmapItem *background;
};

#endif // SCENE__H
