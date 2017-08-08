#ifndef NO_FIT_POLYGON__H
#define NO_FIT_POLYGON__H

#include "geometry.h"

namespace geometry {

class Nfp {
public:
    Nfp() = default;
    Nfp(const Polygon &p, const Polygon &q) {
        set(p, q);
    }

    void set(const Polygon &p, const Polygon &q) {
        this->p = p;
        this->q = q;
        regenerate = true;
    }

    Polygon get() {
        if (!regenerate) return nfp;
        nfp.clear();
        Point pmin = minPoint(p.points());
        Point qmin = minPoint(q.points());

        return nfp;
    }

private:
    Point minPoint(const Polygon &polygon) const {
        Point min = polygon[0];
        for (int i = 1; i < polygon.count(); i++) {
            if (min > polygon[i]) min = polygon[i];
        }
        return min;
    }

private:
    bool regenerate;
    Polygon p;
    Polygon q;
    Polygon nfp;
};

} // namespace geometry

#endif