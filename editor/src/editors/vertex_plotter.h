#ifndef VERTEX_PLOTTER__H
#define VERTEX_PLOTTER__H

#include "editors/editor.h"

class VertexPlotter : public Editor {
public:
    VertexPlotter(Document *document, QObject *parent = 0);
    void sceneEvent(QGraphicsSceneMouseEvent *event);
};

#endif /* end of include guard: VERTEX_PLOTTER__H */
