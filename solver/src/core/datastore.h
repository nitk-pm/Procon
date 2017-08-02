#ifndef DATA_STORE__H
#define DATA_STORE__H

#include "core/component.h"
#include "geometry/geometry.h"

#include <vector>
#include <string>
#include <bitset>

namespace core {

constexpr unsigned int MAX_POLYGONS = 50;
constexpr unsigned int MAX_PIECES = MAX_POLYGONS * 8;

typedef std::bitset<101 * 65> Board;

class Datastore : public Component {
public:
    Datastore() : count(0) {}

    geometry::Polygon frame;
    int               count;
    geometry::Polygon polygons[MAX_POLYGONS];
    geometry::Polygon pieces[MAX_PIECES];
    geometry::Polygon nfp[MAX_PIECES][MAX_PIECES];

    const geometry::Polygon& pieceAt(short id, short type) const;

    void processingPolygons();
};

} // namespace core

#endif // DATA_STORE__H