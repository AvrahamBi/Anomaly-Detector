// 20593794

#ifndef EX2DEMO_MINCIRCLE_H
#define EX2DEMO_MINCIRCLE_H

#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "anomaly_detection_util.h"

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
    Circle(): center(Point()), radius(0){}
};


class minCircle {
public:
    float getDistanceBetweenPoints(const Point& p1, const Point& p2);
    Circle createCircleFromPoints(const Point& p1, const Point& p2);

    Circle createCircleFromPoints(const Point& p1, const Point& p2, const Point& p3);

    Circle createMinTrivialCircle(std::vector<Point>& points);

    bool isPointInsideCircle(const Circle& circle, const Point& point);

    void swapPoints(std::vector<Point>& points, int indexOfP1, int indexOfP2);

    Circle welzlRecursiveFunc(std::vector<Point>& points, std::vector<Point> pointsOnBoundaryOfTheMinCircle, int numOfPoints);

    std::vector<Point> getCopyVectorOfPoints(Point** &points, int size);

    Circle welzlAlgorithm(Point** points, int size);

    Circle findMinCircle(Point** points,int size);
};

#endif //EX2DEMO_MINCIRCLE_H
