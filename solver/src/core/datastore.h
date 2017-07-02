#ifndef DATA_STORE__H
#define DATA_STORE__H

#include <vector>
#include <string>

#include "component.h"
#include "lib/geometry.h"

namespace core {

constexpr unsigned int MAX_POLYGONS = 50;
constexpr unsigned int MAX_PIECES = MAX_POLYGONS * 8;

class Datastore : public Component {
public:
    Datastore() : count(0) {}

    geometry::Polygon frame;
    int               count;
    geometry::Polygon polygons[MAX_POLYGONS];
    geometry::Polygon pieces[MAX_PIECES];

    void processingPolygons();
};

} // namespace core

#endif // DATA_STORE__H