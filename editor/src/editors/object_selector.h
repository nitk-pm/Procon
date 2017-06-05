#ifndef OBJECT_SELECTOR__H
#define OBJECT_SELECTOR__H

#include "editors/editor.h"

#include <QtCore/QList>

class ObjectModel;
class QAction;

class ObjectSelector : public Editor {
    Q_OBJECT

public:
    void sceneEvent(QGraphicsSceneMouseEvent *event);

protected:
    void connectAction(Ui::MainWindow *ui);

public slots:
    void finish();

private:
    void select(ObjectModel *obj);
    void reselect(ObjectModel *obj);
    void deselect();

private slots:
    void remove();

signals:
    void setDeleteActionFlag(bool flag);

private:
    QList<ObjectModel*> object_list;
};

#endif /* end of include guard: OBJECT_SELECTOR__H */
