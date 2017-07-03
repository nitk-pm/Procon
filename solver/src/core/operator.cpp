#include "core/operator.h"
#include "core/solver.h"
#include "core/datastore.h"
#include "lib/geometry.h"

using namespace core;

void Operator::initialize() {
    datastore = parent()->getComponent<Datastore>();
}

bool Operator::canPut(short id, short type, short pos) {
    geometry::Point p(double(pos >> 8), double(pos & 255));
    auto polygon = datastore->polygons[id].offset(p);
    return datastore->frame.containsPolygon(polygon);
}