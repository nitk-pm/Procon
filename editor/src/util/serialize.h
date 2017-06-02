#ifndef SERIALIZE__H
#define SERIALIZE__H

#include "util/geometry.h"
#include <string>

namespace rucm {

class Serialize {
public:
    Serialize() {}
    Serialize(const geometry::Polygon &polygon) {
        set(polygon);
    }

    void set(const geometry::Polygon &polygon) {
        geometry::Polygon poly;
        if (polygon.loopWise() == -1) poly = polygon.reverse();
        else poly = polygon;

        calcOffset(poly);

        for (auto &p : poly.points()) {
            _polygon.add(p - _offset);
        }
    }

    std::string get() const {
        std::string result;
        result += std::to_string(_polygon.points().size()) + ' ';
        for (auto &p : _polygon.points()) {
            result += std::to_string(int(p.x())) + ' ';
            result += std::to_string(int(p.y())) + ' ';
        }
        result.pop_back();
        return result;
    }

    geometry::Point offset() const {
        return _offset;
    }

private:
    void calcOffset(const geometry::Polygon &polygon) {
        _offset = polygon.points().front();
        for (auto &p : polygon.points()) {
            if (_offset.x() > p.x()) _offset.setX(p.x());
            if (_offset.y() > p.y()) _offset.setY(p.y());
        }
    }

private:
    geometry::Polygon _polygon;
    geometry::Point _offset;
};

};

#endif /* end of include guard: SERIALIZE__H */
