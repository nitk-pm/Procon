#ifndef SCENE__H
#define SCENE__H

#include <QtWidgets/QGraphicsScene>
#include <QtCore/QVector>
#include <QtCore/QMap>

class QGraphicsSceneMouseEvent;
class QAction;

class Scene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    QGraphicsPixmapItem* createBackground(int width, int height);

public slots:
    void changeMode(QAction *action);

private:
    QObject *current_mode;
};

#endif // SCENE__H
