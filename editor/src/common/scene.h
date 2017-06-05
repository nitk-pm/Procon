#ifndef SCENE__H
#define SCENE__H

#include <QtWidgets/QGraphicsScene>

class QGraphicsPixmapItem;
class QGraphicsSceneMouseEvent;

class Scene : public QGraphicsScene {
    Q_OBJECT

public:
    static const int BASE_SIZE = 10;

    Scene(QObject *parent = 0);
    Scene(int width, int height, QObject *parent = 0);
    void setBoardSize(int width, int height);
    QPointF modifyDataPos(const QPointF &pos);
    QPointF modifyPos(const QPointF &pos);
    QPointF convertDataPos(int x, int y);
    QGraphicsPixmapItem* background() const;

    void changeEditor(QObject *editor);

signals:
    void beginEditor();
    void finishEditor();

private:
    QObject *current_editor;
    QGraphicsPixmapItem *_background;
};

#endif // SCENE__H
