// 20593794

#include "minCircle.h"

float minCircle::getDistanceBetweenPoints(const Point& p1, const Point& p2){
    return (float)sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

Circle minCircle::createCircleFromPoints(const Point& p1, const Point& p2){
    //the center is the middle point of the 2 given points.
    //the radius is the distance between the points / 2
    Point center = Point((p1.x + p2.x)/2, (p1.y + p2.y)/2);
    float radius = getDistanceBetweenPoints(p1,p2)/2;
    return {center,radius};
}

Circle minCircle::createCircleFromPoints(const Point& p1, const Point& p2, const Point& p3){
    //calculate repetitive expressions
    float x1_squared = pow(p1.x, 2); float y1_squared = pow(p1.y, 2);
    float x2_squared = pow(p2.x, 2); float y2_squared = pow(p2.y, 2);
    float x3_squared = pow(p3.x, 2); float y3_squared = pow(p3.y, 2);
    float x1_squared_minus_x3_squared = x1_squared - x3_squared;
    float y1_squared_minus_y3_squared = y1_squared - y3_squared;
    float x2_squared_minus_x1_squared = x2_squared - x1_squared;
    float y2_squared_minus_y1_squared = y2_squared - y1_squared;

    //calculating t at 5 steps:
    //step 1: ((x1^2–x3^2)(x1–x2)
    float t1 = x1_squared_minus_x3_squared * (p1.x - p2.x);
    //step 2: (y1^2–y3^2)(x1–x2)
    float t2 = y1_squared_minus_y3_squared * (p1.x - p2.x);
    //step 3: (x2^2–x1^2)(x1–x3)
    float t3 = x2_squared_minus_x1_squared * (p1.x - p3.x);
    //step 4: (y2^2–y1^2)(x1–x3)
    float t4 = y2_squared_minus_y1_squared * (p1.x - p3.x);
    //step 5: 2*((y3–y1)(x1–x2)–(y2–y1)(x1–x3))
    float t5 = 2 * ((p3.y-p1.y)*(p1.x-p2.x) - (p2.y-p1.y)*(p1.x-p3.x));
    //connecting all parts:
    float t = (t1+t2+t3+t4)/t5;

    //calculating s at 5 steps:
    //step 1: (x1^2–x3^2)(y1–y2)
    float s1 = x1_squared_minus_x3_squared * (p1.y - p2.y);
    //step 2: (y1^2–y3^2)(y1–y2)
    float s2 = y1_squared_minus_y3_squared * (p1.y - p2.y);
    //step 3: (x2^2–x1^2)(y1–y3)
    float s3 = x2_squared_minus_x1_squared * (p1.y - p3.y);
    //step 4: (y2^2–y1^2)(y1–y3)
    float s4 = (y2_squared - y1_squared) * (p1.y - p3.y);
    //step 5: 2*((x3-x1)(y1–y2)–(x2–x1)(y1–y3)
    float s5 = 2* ((p3.x-p1.x)*(p1.y-p2.y) - (p2.x-p1.x)*(p1.y-p3.y));
    //connecting all parts:
    float s = (s1+s2+s3+s4)/s5;

    //c = -x1^2 – y1^2 – 2sx1 – 2ty1
    float c = -1*x1_squared - y1_squared -2*s*p1.x -2*t*p1.y;

    //calculate a,b,radius
    float a = -1*s;
    float b = -1*t;
    auto radius = (float)sqrt(pow(a,2) + pow(b,2) - c);

    return {Point(a,b),radius};
}

Circle minCircle::createMinTrivialCircle(std::vector<Point>& points){
    if(points.empty()){
        return {Point(0,0), 0};
    }else if(points.size() == 1){
        return {points[0], 0};
    }else if(points.size() == 2){
        return createCircleFromPoints(points[0], points[1]);
    }
    return createCircleFromPoints(points[0], points[1], points[2]);
}

bool minCircle::isPointInsideCircle(const Circle& circle, const Point& point){
    if(getDistanceBetweenPoints(circle.center, point) <= circle.radius){
        return true;
    }
    return false;
}

void minCircle::swapPoints(std::vector<Point>& points, int indexOfP1, int indexOfP2){
    Point tmp = points[indexOfP1];
    points[indexOfP1] = points[indexOfP2];
    points[indexOfP2] = tmp;
}

Circle minCircle::welzlRecursiveFunc(std::vector<Point>& points, std::vector<Point> pointsOnBoundaryOfTheMinCircle, int numOfPoints){
    //stop condition
    if(numOfPoints == 0 || pointsOnBoundaryOfTheMinCircle.size() == 3){

        return createMinTrivialCircle(pointsOnBoundaryOfTheMinCircle);
    }
    // choose a random point
    int indexOfRandomPoint = rand() % numOfPoints;
    Point randomPoint = points[indexOfRandomPoint];

    //for efficiency instead of removing the point - put it at the end of the vector and ignore it.
    swapPoints(points, indexOfRandomPoint, numOfPoints - 1);

    //get recursively the minimum circle of points - {randomPoint}.
    Circle minCircleOfPointsBesidesTheRandomSelectedPoint =
            welzlRecursiveFunc(points, pointsOnBoundaryOfTheMinCircle, numOfPoints - 1);

    if(isPointInsideCircle(minCircleOfPointsBesidesTheRandomSelectedPoint, randomPoint)){
        return minCircleOfPointsBesidesTheRandomSelectedPoint;
    }
    pointsOnBoundaryOfTheMinCircle.push_back(randomPoint);
    return welzlRecursiveFunc(points, pointsOnBoundaryOfTheMinCircle, numOfPoints-1);
}

std::vector<Point> minCircle::getCopyVectorOfPoints(Point** &points, int size){
    std::vector<Point> copy;
    copy.reserve(size);
    for(int i = 0; i < size; i++){
        copy.push_back(*(points[i]));
    }
    return copy;
}

Circle minCircle::welzlAlgorithm(Point** points, int size){
    //copying the given points for manipulate it during the algorithm.
    std::vector<Point> copyOfTheGivenPoints = getCopyVectorOfPoints(points, size);
    std::vector<Point> pointsOnBoundaryOfTheMinCircle;
    return welzlRecursiveFunc(copyOfTheGivenPoints, pointsOnBoundaryOfTheMinCircle, size);
}

// The implementation is based on welzl algorithm.
 Circle minCircle::findMinCircle(Point** points,int size){
    return  welzlAlgorithm(points, size);
}










