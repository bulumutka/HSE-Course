#include "circle.h"

#include "geometry_utils.h"
#include "point.h"

namespace bulumutka {

Circle::Circle(
    const Point& center,
    double radius
) : Ellipse(center, center, 2 * radius) {}

double Circle::radius() const {
    return a_;
}

}
