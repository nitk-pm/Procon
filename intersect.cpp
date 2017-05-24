#include <iostream>
#include <complex>
using namespace std;

typedef complex<double> Point;

double dot(Point a, Point b) {
    return (conj(a) * b).real();
}

double cross(Point a, Point b) {
    return (conj(a) * b).imag();
}

bool intersect(Point a, Point b, Point c, Point d) {
    double t_ab = cross(b - a, c - a) * cross(b - a, d - a);
    double t_cd = cross(d - c, a - c) * cross(d - c, b - c);
    return  t_ab <= 0 && t_cd <= 0;
}

void test(Point a, Point b, Point c, Point d, bool check) {
    bool result = intersect(a, b, c, d);
    if (result == check) cout << "intersect" << endl;
    else cout << "not intersect" << endl;
}

int main(void) {
    test(Point(0, 0), Point(0, 10), Point(0, 5), Point(-5, 5), true);
    return 0;
}
