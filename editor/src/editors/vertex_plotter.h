#ifndef VERTEX_PLOTTER__H
#define VERTEX_PLOTTER__H

#include "editors/editor.h"

class VertexPlotter : public Editor {
public:
    VertexPlotter() {}
    void sceneEvent(QGraphicsSceneMouseEvent *event);
};

#endif /* end of include guard: VERTEX_PLOTTER__H */
