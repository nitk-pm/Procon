#ifndef CONVEX_HULL__H
#define CONVEX_HULL__H

#include <vector>

typedef std::pair<double, double> Point;

class ConvexHull {

public:
    void add(const Point &point);
    void add(double x, double y);
    std::vector<Point> get();

private:
    Point  minPoint();
    Point  minAnglePoint(const Point &point);
    double angle(const Point &start, const Point &end);

private:
    std::vector<Point> points;
    double border_angle;
};

#endif /* end of include guard: CONVEX_HULL__H */
