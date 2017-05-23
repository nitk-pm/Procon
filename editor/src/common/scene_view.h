#ifndef SCENE_VIEW_H
#define SCENE_VIEW_H

#include <QtWidgets/QGraphicsView>

class QGraphicsScene;
class QWheelEvent;
class Scene;

class SceneView : public QGraphicsView {
    Q_OBJECT

public:
    explicit SceneView(QWidget *parent = 0);
    ~SceneView();
    virtual void setScene(Scene *scene);

private:
    void zoom(int delta);

protected:
    virtual void wheelEvent(QWheelEvent *event);

private:
    int scheduled_scalings;

public slots:
    void scalingTime(qreal x);
    void animFinished();
};

#endif // SCENE_VIEW_H
