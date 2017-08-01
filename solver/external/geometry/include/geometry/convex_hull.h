#ifndef CONVEX_HULL__H
#define CONVEX_HULL__H

#include "geometry.h"

namespace geometry {

/* 凸包の計算 */
class ConvexHull {
public:
    void add(const Point &point) {
        _points.push_back(point);
    }

    void add(double x, double y) {
        add(Point(x, y));
    }

    void add(const std::vector<Point> &points) {
        _points.insert(_points.end(), points.begin(), points.end());
    }

    std::vector<Point> get() const {
        std::vector<Point> result;
        result.push_back(minPoint(_points));
        result.push_back(minRadianPoint(result.back()));
        while (result.front() != result.back()) result.push_back(minRadianPoint(result.back()));
        result.pop_back();
        return std::move(result);
    }

private:
    Point minRadianPoint(const Point &point) const {
        Point min = point;
        for (auto &p : _points) {
            if (min == point) min = p;
            else {
                double v = cross(min - point, p - point);
                if (v > 0 || (equal(v, 0) && (p - point).length() > (min - point).length())) min = p;
            }
        }
        return min;
    }

private:
    std::vector<Point> _points;
};

} // namespace geometry

#endif