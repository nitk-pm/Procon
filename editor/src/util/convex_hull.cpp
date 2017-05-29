#include "util/convex_hull.h"
#define _USE_MATH_DEFINES
#include <cmath>

void ConvexHull::addEdge(const Edge &edge) {
    graph[edge.first].insert(edge.second);
    graph[edge.second].insert(edge.first);
}

void ConvexHull::addEdge(const Point &p1, const Point &p2) {
    addEdge(Edge(p1, p2));
}

std::vector<Point> ConvexHull::get() const {
    return std::vector<Point>();
}

double ConvexHull::calc_angle(const Point &start, const Point &end) {
    double dx = end.first - start.first;
    double dy = end.second - start.second;
    double angle = std::atan(dy / dx) * (180.0 / M_PI);
    if ((dx < 0 && dy >= 0) || (dx < 0 && dy < 0)) angle += 180;
    else if (dx * dy < 0) angle += 360.0;
}
