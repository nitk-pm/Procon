#include <iostream>

#include "geometry/geometry.h"

using namespace std;

void test(const geometry::Polygon &a, const geometry::Polygon &b) {
    for (int i = 0; i < a.size(); i++) {
        cout << "check vertex " << i << ": ";
        if (a[i] == b[i]) cout << "ok";
        else cout << "failed";
        cout << endl;
    }
}

int main(void) {
    geometry::Polygon polygon;

    polygon.add(0, 0);
    polygon.add(10, 0);
    polygon.add(10, 10);

    geometry::Polygon flip_horizontal;
    flip_horizontal.add(10, 0);
    flip_horizontal.add(0, 0);
    flip_horizontal.add(0, 10);

    test(polygon.flipHorizontal(), flip_horizontal);

    geometry::Polygon rotate;
    rotate.add(10, 0);
    rotate.add(10, 10);
    rotate.add(0, 10);

    auto p = polygon.rotate(90);
    for (int i = 0; i < p.size(); i++) {
        cout << p[i].x() << " " << p[i].y() << endl;
    }

    test(polygon.rotate(90), rotate);

    return 0;
}