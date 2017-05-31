#include "util/convex_hull_of_graph.h"
#define _USE_MATH_DEFINES
#include <cmath>

void ConvexHullOfGraph::addEdge(const Point &p1, const Point &p2) {
    if (p1 == p2) return;
    graph[p1].insert(p2);
    graph[p2].insert(p1);
}

void ConvexHullOfGraph::addEdge(double x1, double y1, double x2, double y2) {
    addEdge(Point(x1, y1), Point(x2, y2));
}

std::vector<Point> ConvexHullOfGraph::get() {
    std::vector<Point> result;

    border_angle = 0.0;
    result.push_back(minPoint());
    result.push_back(minAnglePoint(result.front()));

    while (result.front() != result.back()) result.push_back(minAnglePoint(result.back()));
    result.pop_back();

    return std::move(result);
}

Point ConvexHullOfGraph::minPoint() {
    Point min = graph.begin()->first;
    for (auto point : graph) {
        auto p = point.first;
        if (min.second > p.second) min = p;
        else if (min.second == p.second && min.first > p.first) min = p;
    }
    return min;
}

Point ConvexHullOfGraph::minAnglePoint(const Point &point) {
    Point min = point;
    double min_angle = 360.0;

    for (auto p : graph[point]) {
        double _angle = angle(point, p);
        if (min_angle > _angle && _angle >= border_angle) {
            min_angle = _angle;
            min = p;
        }
    }
    border_angle = min_angle;
    return min;
}

double ConvexHullOfGraph::angle(const Point &start, const Point &end) {
    double dx = end.first - start.first;
    double dy = end.second - start.second;
    double angle = std::atan(dy / dx) * (180.0 / M_PI);
    if ((dx < 0 && dy >= 0) || (dx < 0 && dy < 0)) angle += 180;
    else if (dx * dy < 0) angle += 360.0;
    return angle;
}
