#ifndef GEOMETRY__H
#define GEOMETRY__H

#include <complex>
#include <vector>
#include <cmath>

namespace rucm {
    namespace geometry {
        const int EPS = 1e-10;
        const double PI = 3.14159265358979;
        typedef std::complex<double> Point;

        /* 二つの値が等しいかどうか（許容誤差を考慮） */
        bool equal(double a, double b) {
            return std::abs(a - b) < EPS;
        }

        double x(const Point &p) {
            return p.real();
        }

        double y(const Point &p) {
            return p.imag();
        }

        /* 内積 */
        double dot(const Point &p, const Point &q) {
            return p.real() * q.real() + p.imag() * q.imag();
        }

        /* 外積 */
        double cross(const Point &p, const Point &q) {
            return p.real() * q.imag() - p.imag() * q.real();
        }

        /* 大きさ */
        double length(const Point &point) {
            return std::sqrt(point.real() * point.real() + point.imag() * point.imag());
        }

        /* 点と点の距離 */
        double distance(const Point &p, const Point &q) {
            return length(q - p);
        }

        /* 直線と点の距離 */
        double distance(const Point &p1, const Point &p2, const Point &q) {
            return cross(p2 - p1, q - p1) / length(p2 - p1);
        }

        /* 直線と直線の交差判定 */
        bool intersect(const Point &p1, const Point &p2, const Point &q1, const Point &q2) {
            double t1 = cross(p2 - p1, q1 - p1) * cross(p2 - p1, q2 - p1);
            double t2 = cross(q2 - q1, p1 - q1) * cross(q2 - q1, p2 - q1);
            return t1 <= 0 && t2 <= 0;
        }

        /* 正規化 */
        Point norm(const Point &point) {
            return point / length(point);
        }

        /* 角度 */
        double radian(const Point &p1, const Point &p2, const Point &p3) {
            Point p = p2 - p1;
            Point q = p3 - p1;
            double rad = std::acos(dot(p, q) / (length(p) * length(q)));

            if (cross(p, q) < 0) rad = 2 * PI - rad;
            return rad;
        }

        /* 多角形を表したクラス */
        class Polygon {
        public:
            Polygon() {}
            Polygon(const std::vector<Point> &points) {
                set(points);
            }

            void set(const std::vector<Point> &points) {
                _points = points;
            }

            std::vector<Point> points() const {
                return _points;
            }

            /* 符号付面積 */
            double signedArea() const {
                const int count = _points.size();
                double result = 0.0;
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
                int wn = 0;
                for (int i = 0; i < count; i++) {
                    if (_points[i].imag() <= point.imag() && _points[(i + 1) % count].imag() > point.imag()) {
                        double vt = (point.imag() - _points[i].imag()) / (_points[(i + 1) % count].imag() - _points[i].imag());
                        if (point.real() < (_points[i].real() + (vt * (_points[(i + 1) % count].real() - _points[i].real())))) wn++;
                    }
                    else if (_points[i].imag() > point.imag() && _points[(i + 1) % count].imag() <= point.imag()) {
                        double vt = (point.imag() - _points[i].imag()) / (_points[(i + 1) % count].imag() - _points[i].imag());
                        if (point.real() < (_points[i].real() + (vt * (_points[(i + 1) % count].real() - _points[i].real())))) wn--;
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
                    if (min.imag() > p.imag() || (equal(min.imag(), p.imag()) && min.real() > p.real())) min = p;
                }
                return min;
            }

            Point minRadianPoint(const Point &point) const {
                Point min = point;
                for (auto &p : _points) {
                    if (min == point) min = p;
                    else {
                        double v = cross(min - point, p - point);
                        if (v > 0 || (equal(v, 0) && length(p - point) > length(min - point))) min = p;
                    }
                }
                return min;
            }

        private:
            std::vector<Point> _points;
        };
    };
};

#endif /* end of include guard: GEOMETRY__H */
