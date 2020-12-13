#ifndef POINT_H_
#define POINT_H_

namespace bulumutka {

struct Point {
    Point(double x, double y);

    Point& operator=(const Point& other);
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    const Point operator-() const;

    const Point operator*(double coef) const;
    const Point operator/(double coef) const;
    const Point operator+(const Point& point) const;
    const Point operator-(const Point& other) const;

    double x;
    double y;
};

}

#endif
