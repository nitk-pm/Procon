#ifndef CONVEX_HULL__H
#define CONVEX_HULL__H

#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef std::pair<double, double> Point;
typedef std::pair<Point, Point> Edge;

namespace std {
    template<> class hash<Point> {
    public:
        size_t operator()(const Point &p) const {
            return hash<double>()(p.first) ^ hash<double>()(p.second);
        }
    };
}

class ConvexHull {
public:
    void addEdge(const Edge &edge);
    void addEdge(const Point &p1, const Point &p2);
    std::vector<Point> get() const;

private:
    double calc_angle(const Point &start, const Point &end);

private:
    std::unordered_map<Point, std::unordered_set<Point> > graph;
};

#endif /* end of include guard: CONVEX_HULL__H */
