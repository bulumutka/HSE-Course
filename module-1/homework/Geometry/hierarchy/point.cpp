#include "point.h"

#include "geometry_utils.h"

#include <cmath>

namespace bulumutka {

Point::Point(double x, double y) 
    : x(x), y(y) 
{}

Point& Point::operator=(const Point& other) {
    x = other.x;
    y = other.y;
    return *this;
}

bool Point::operator==(const Point& other) const {
    return geometry_utils::closeEquals(x, other.x) 
        && geometry_utils::closeEquals(y, other.y);
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}

const Point Point::operator-() const {
    return { -x, -y };
}

const Point Point::operator*(double coef) const {
    return { x * coef, y * coef };
}

const Point Point::operator/(double coef) const {
    return { x / coef, y / coef };
}

const Point Point::operator+(const Point& other) const {
    return { x + other.x, y + other.y };
}

const Point Point::operator-(const Point& other) const {
    return { x - other.x, y - other.y };
}

}
