#include "util/convex_hull.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

void ConvexHull::add(const Point &point) {
    if (std::find(points.begin(), points.end(), point) != points.end()) return;
    points.push_back(point);
}

void ConvexHull::add(double x, double y) {
    add(Point(y, x));
}

std::vector<Point> ConvexHull::get() {
    std::vector<Point> result;
    
    border_angle = 0.0;
    result.push_back(minPoint());
    result.push_back(minAnglePoint(result.front()));

    while (result.front() != result.back()) result.push_back(minAnglePoint(result.back()));
    result.pop_back();

    return std::move(result);
}

Point ConvexHull::minPoint() {
    Point min = points.front();

    for (auto &point : points) {
        if (min > point) min = point;
    }
    return min;
}

Point ConvexHull::minAnglePoint(const Point &point) {
    Point min = point;
    double min_angle = 360.0;

    for (auto p : points) {
        double _angle = angle(point, p);
        if (min_angle > _angle && _angle >= border_angle) {
            min_angle = _angle;
            min = p;
        }
    }
    border_angle = min_angle;
    return min;
}

double ConvexHull::angle(const Point &start, const Point &end) {
    double dx = end.second - start.second;
    double dy = end.first - start.first;
    double angle = std::atan(dy / dx) * (180.0 / M_PI);
    if ((dx < 0 && dy >= 0) || (dx < 0 && dy < 0)) angle += 180;
    else if (dx * dy < 0) angle += 360.0;
    return angle;
}
