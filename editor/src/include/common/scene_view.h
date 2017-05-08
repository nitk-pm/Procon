#ifndef SCENE_VIEW_H
#define SCENE_VIEW_H

#include <QtWidgets/QGraphicsView>

class QGraphicsScene;
class QWheelEvent;
class QMouseEvent;
class Scene;

class SceneView : public QGraphicsView {
    Q_OBJECT

public:
    explicit SceneView(QWidget *parent = 0);
    ~SceneView();

    virtual void setScene(Scene *scene);

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

private:
    int scheduled_scalings;
    qreal translate_scalings;
    QPointF old_mouse_pos;

public slots:
    void scalingTime(qreal x);
    void animFinished();
    void changeModeMoving();
    void changeModeEditPiece();
};

#endif // SCENE_VIEW_H
