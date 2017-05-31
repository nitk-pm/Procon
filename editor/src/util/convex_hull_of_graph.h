#ifndef CONVEX_HULL_OF_GRAPH__H
#define CONVEX_HULL_OF_GRAPH__H

#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef std::pair<double, double> Point;

namespace std {
    template<> class hash<Point> {
    public:
        size_t operator()(const Point &p) const {
            return hash<double>()(p.first) ^ hash<double>()(p.second);
        }
    };
}

class ConvexHullOfGraph {

public:
    void addEdge(const Point &p1, const Point &p2);
    void addEdge(double x1, double y1, double x2, double y2);
    std::vector<Point> get();

private:
    Point  minPoint();
    Point  minAnglePoint(const Point &point);
    double angle(const Point &start, const Point &end);

private:
    std::unordered_map<Point, std::unordered_set<Point> > graph;
    double border_angle;
};

#endif /* end of include guard: CONVEX_HULL_OF_GRAPH__H */
