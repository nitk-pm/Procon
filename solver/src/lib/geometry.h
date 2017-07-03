#ifndef GEOMETRY__H
#define GEOMETRY__H

#include <vector>
#include <algorithm>
#include <cmath>

namespace geometry {

static const double EPS = 1e-10;
static const double PI  = 3.14159265358979;

/* 二つの値が等しいかどうか（許容誤差を考慮） */
inline bool equal(double a, double b) {
    return std::abs(a - b) < EPS;
}

/* 2次元平面上の座標を表すクラス */
class Point {
public:
    Point() = default;
    Point(const Point &p) = default;
    Point& operator=(const Point &other) = default;

    Point(double x, double y) {
        set(x, y);
    }

    void setX(double x) {
        _x = x;
    }

    void setY(double y) {
        _y = y;
    }

    void set(double x, double y) {
        setX(x);
        setY(y);
    }

    double x() const {
        return _x;
    }

    double y() const {
        return _y;
    }

    /* 大きさ */
    double length() const {
        return std::sqrt(_x * _x + _y * _y);
    }

    Point rotate(double angle) const {
        const double s = std::sin(angle * PI / 180.0);
        const double c = std::cos(angle * PI / 180.0);
        return Point(_x * c - _y * s, _x * s + _y * c);
    }

    Point& operator+=(const Point &p) {
        _x += p._x;
        _y += p._y;
        return *this;
    }

    Point& operator-=(const Point &p) {
        _x -= p._x;
        _y -= p._y;
        return *this;
    }

    Point& operator*=(double factor) {
        _x *= factor;
        _y *= factor;
        return *this;
    }

    Point& operator/=(double divisor) {
        _x /= divisor;
        _y /= divisor;
        return *this;
    }

private:
    double _x;
    double _y;
};

inline bool operator==(const Point &p, const Point &q) {
    return equal(p.x(), q.x()) && equal(p.y(), q.y());
}

inline bool operator!=(const Point &p, const Point &q) {
    return !equal(p.x(), q.x()) || !equal(p.y(), q.y());
}

inline bool operator>(const Point &p, const Point &q) {
    if (p.y() > q.y()) return true;
    else if (equal(p.y(), q.y()) && p.x() > q.x()) return true;
    return false;
}

inline bool operator<(const Point &p, const Point &q) {
    if (p.y() < q.y()) return true;
    else if (equal(p.y(), q.y()) && p.x() < q.x()) return true;
    return false;
}

inline const Point operator+(const Point &p, const Point &q) {
    return Point(p.x() + q.x(), p.y() + q.y());
}

inline const Point operator-(const Point &p, const Point &q) {
    return Point(p.x() - q.x(), p.y() - q.y());
}

inline const Point operator*(const Point &p, double factor) {
    return Point(p.x() * factor, p.y() * factor);
}

inline const Point operator*(double factor, const Point &p) {
    return Point(p.x() * factor, p.y() * factor);
}

inline const Point operator/(const Point &p, double divisor) {
    return Point(p.x() / divisor, p.y() / divisor);
}

inline const Point operator/(double divisor, const Point &p) {
    return Point(p.x() / divisor, p.y() / divisor);
}

/* 内積 */
inline double dot(const Point &p, const Point &q) {
    return p.x() * q.x() + p.y() * q.y();
}

/* 外積 */
inline double cross(const Point &p, const Point &q) {
    return p.x() * q.y() - p.y() * q.x();
}


/* 点と点の距離 */
inline double distance(const Point &p, const Point &q) {
    return (q - p).length();
}

/* 直線と点の距離 */
inline double distance(const Point &p1, const Point &p2, const Point &q) {
    return cross(p2 - p1, q - p1) / (p2 - p1).length();
}

/* 直線と直線の交差判定 */
inline bool intersect(const Point &p1, const Point &p2, const Point &q1, const Point &q2) {
    double t1 = cross(p2 - p1, q1 - p1) * cross(p2 - p1, q2 - p1);
    double t2 = cross(q2 - q1, p1 - q1) * cross(q2 - q1, p2 - q1);
    return t1 <= 0 && t2 <= 0;
}

/* 角度 */
inline double radian(const Point &p1, const Point &p2, const Point &p3) {
    Point  p   = p2 - p1;
    Point  q   = p3 - p1;
    double rad = std::acos(dot(p, q) / (p.length() * q.length()));

    if (cross(p, q) < 0) rad = 2 * PI - rad;
    return rad;
}

/* 多角形を表したクラス */
class Polygon {
public:
    Polygon() = default;
    Polygon(const Polygon &other) = default;
    Polygon& operator=(const Polygon &polygon) = default;

    Polygon(const std::vector<Point> &points) {
        set(points);
    }

    void set(const std::vector<Point> &points) {
        _points = points;
    }

    void add(const std::vector<Point> &points) {
        _points.insert(_points.end(), points.begin(), points.end());
    }

    void add(const Point &point) {
        _points.push_back(point);
    }

    void add(double x, double y) {
        add(Point(x, y));
    }

    void clear() {
        _points.clear();
    }

    Point& operator[](int index) {
        return _points[index];
    }

    const Point& operator[](int index) const {
        return _points.at(index);
    }

    int size() const {
        return _points.size();
    }

    std::vector<Point> points() const {
        return _points;
    }

    /* 符号付面積 */
    double signedArea() const {
        const int count  = _points.size();
        double    result = 0.0;
        for (int i = 0; i < count; i++) {
            result += cross(_points[i], _points[(i + 1) % count]);
        }
        return result / 2;
    }

    /* 面積 */
    double area() const {
        return std::abs(signedArea());
    }

    /* 多角形と点の内外判定 */
    bool containsPoint(const Point &point) const {
        const int count = _points.size();
        int       wn    = 0;
        for (int i = 0; i < count; i++) {
            if (_points[i].y() <= point.y() && _points[(i + 1) % count].y() > point.y()) {
                double vt = (point.y() - _points[i].y()) / (_points[(i + 1) % count].y() - _points[i].y());
                if (point.x() < (_points[i].x() + (vt * (_points[(i + 1) % count].x() - _points[i].x())))) wn++;
            }
            else if (_points[i].y() > point.y() && _points[(i + 1) % count].y() <= point.y()) {
                double vt = (point.y() - _points[i].y()) / (_points[(i + 1) % count].y() - _points[i].y());
                if (point.x() < (_points[i].x() + (vt * (_points[(i + 1) % count].x() - _points[i].x())))) wn--;
            }
        }
        return wn != 0;
    }

    /* 多角形のループの向き（時計回り：-1, 反時計回り：1, 不定：0） */
    int loopWise() const {
        double _area = signedArea();
        if (equal(_area, 0.0)) return 0;
        else if (_area > 0) return 1;
        return -1;
    }

    /* 多角形のループの向きを反転する */
    Polygon flipLoopWise() const {
        Polygon polygon = *this;
        std::reverse(polygon._points.begin() + 1, polygon._points.end());
        return polygon;
    }

    /* 左右反転した多角形を返す */
    Polygon flipHorizontal() const {
        Polygon polygon = *this;
        double max = polygon[0].x();
        double min = polygon[0].x();
        for (int i = 0; i < polygon.size(); i++) {
            max = std::max(max, polygon[i].x());
            min = std::min(min, polygon[i].x());
        }

        const double center = (max - min);
        for (int i = 0; i < polygon.size(); i++) {
            polygon[i].setX(center - polygon[i].x());
        }
        return polygon;
    }

    /* 原点中心に回転させ、座標補正する */
    Polygon rotate(double angle) const {
        const double s = std::sin(angle * PI / 180.0);
        const double c = std::cos(angle * PI / 180.0);
        Polygon polygon = *this;
        Point offset(polygon[0].x(), polygon[0].y());
        for (int i = 1; i < polygon.size(); i++) {
            polygon[i].setX(polygon[i].x() * c - polygon[i].y() * s);
            polygon[i].setY(polygon[i].x() * s + polygon[i].y() * c);
            offset.setX(std::min(offset.x(), polygon[i].x()));
            offset.setY(std::min(offset.y(), polygon[i].y()));
        }
        // for (int i = 0; i < polygon.size(); i++) {
        //     polygon[i] -= offset;
        // }
        return polygon;
    }

    /* 引数で渡した多角形が完全に包含されているかチェックする */
    bool containsPolygon(const Polygon &other) const {
        for (int i = 0; i < other.size(); i++) {
            if (!containsPoint(other[i])) return false;
        }
        return true;
    }

    /* オフセット値で補正したポリゴンを返す */
    Polygon offset(const Point &p) const {
        Polygon polygon = *this;
        for (auto &it : polygon._points) {
            it += p;
        }
        return polygon;
    }

    /* 多角形を包含する矩形を計算して返す */
    Polygon boundingBox() const {
        Polygon bounding_box;
        bounding_box.add(_points[0]);
        bounding_box.add(_points[0]);
        bounding_box.add(_points[0]);
        bounding_box.add(_points[0]);
        for (auto &it : _points) {
            if (it.x() < bounding_box[0].x()) {
                bounding_box[0].setX(it.x());
                bounding_box[2].setX(it.x());
            }
            if (it.x() > bounding_box[1].x()) {
                bounding_box[1].setX(it.x());
                bounding_box[3].setX(it.x());
            }
            if (it.y() < bounding_box[0].y()) {
                bounding_box[0].setY(it.y());
                bounding_box[1].setY(it.y());
            }
            if (it.y() > bounding_box[2].y()) {
                bounding_box[2].setY(it.y());
                bounding_box[3].setY(it.y());
            }
        }
        return bounding_box;
    }

private:
    std::vector<Point> _points;
};

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
        result.push_back(minPoint());
        result.push_back(minRadianPoint(result.back()));
        while (result.front() != result.back()) result.push_back(minRadianPoint(result.back()));
        result.pop_back();
        return std::move(result);
    }

private:
    Point minPoint() const {
        Point min = _points.front();
        for (auto &p : _points) {
            if (min > p) min = p;
        }
        return min;
    }

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

};

#endif /* end of include guard: GEOMETRY__H */
