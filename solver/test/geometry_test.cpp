#include "gtest/gtest.h"
#include "geometry/geometry.h"

TEST(Intersect, Test1) {
    geometry::Point p(0, 10);
    geometry::Point q(10, 0);

    EXPECT_EQ(false, geometry::intersect(p, q, p, q));
}

TEST(Intersect, Test2) {
    geometry::Point p(0, 10);
    geometry::Point q(10, 0);
    geometry::Point r(10, 10);

    EXPECT_EQ(false, geometry::intersect(p, q, p, r));
}

TEST(Intersect, Test3) {
    geometry::Point p(0, 10);
    geometry::Point q(10, 0);
    geometry::Point r(10, 10);
    geometry::Point s(0, 0);

    EXPECT_EQ(true, geometry::intersect(p, q, r, s));
}

TEST(Polygon, FlipHorizontalTest) {
    geometry::Polygon polygon, ans;
    polygon.add(0, 0);
    polygon.add(5, 0);
    polygon.add(0, 5);
    ans.add(5, 0);
    ans.add(0, 0);
    ans.add(5, 5);

    EXPECT_EQ(polygon.flipHorizontal(), ans);
}

TEST(Polygon, RotateTest1) {
    geometry::Polygon polygon, ans;
    polygon.add(0, 0);
    polygon.add(5, 0);
    polygon.add(0, 5);
    ans.add(5, 0);
    ans.add(5, 5);
    ans.add(0, 0);

    EXPECT_EQ(polygon.rotate(90), ans);
}

TEST(Polygon, RotateTest2) {
    geometry::Polygon polygon, ans;
    polygon.add(0, 0);
    polygon.add(5, 0);
    polygon.add(0, 5);
    ans.add(5, 5);
    ans.add(0, 5);
    ans.add(5, 0);

    EXPECT_EQ(polygon.rotate(180), ans);
}

TEST(Polygon, RotateTest3) {
    geometry::Polygon polygon, ans;
    polygon.add(0, 0);
    polygon.add(5, 0);
    polygon.add(0, 5);
    ans.add(0, 5);
    ans.add(0, 0);
    ans.add(5, 5);

    EXPECT_EQ(polygon.rotate(270), ans);
}