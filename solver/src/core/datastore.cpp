#include "core/datastore.h"

using namespace core;

const geometry::Polygon& Datastore::pieceAt(short id, short type) const {
    return pieces[id * 8 + type];
}

void Datastore::processingPolygons() {
    for (int i = 0; i < count; i++) {
        pieces[i * 8 + 0] = polygons[i];
        pieces[i * 8 + 1] = polygons[i].rotate(90);
        pieces[i * 8 + 2] = polygons[i].rotate(180);
        pieces[i * 8 + 3] = polygons[i].rotate(270);
        pieces[i * 8 + 4] = pieces[i * 8 + 0].flipHorizontal();
        pieces[i * 8 + 5] = pieces[i * 8 + 1].flipHorizontal();
        pieces[i * 8 + 6] = pieces[i * 8 + 2].flipHorizontal();
        pieces[i * 8 + 7] = pieces[i * 8 + 3].flipHorizontal();
    }
}