#ifndef PIECE__H
#define PIECE__H

#include <QtCore/QVector>

class Piece {
public:
    void addVertex(int x, int y);
    void remove(int x, int y);

private:
    QVector<QPair<int, int> > vertexes;
};

#endif // PIECE__H
