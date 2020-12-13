#include "rectangle.h"

#include "geometry_utils.h"
#include "line.h"
#include "point.h"

#include <cmath>

namespace bulumutka {

namespace {

std::vector<Point> createVertices(
    const Point& start,
    const Point& end,
    double coef
) {
    const auto radians = std::atan(coef);
    const auto edge = geometry_utils::dist(start, end) / std::sqrt(coef * coef + 1);

    const auto u = end - start;
    const auto w = geometry_utils::rotatePoint(u, radians);
    const auto v = w / geometry_utils::dist(w) * edge;
    const auto second_point = start + v;
    const auto forth_point = geometry_utils::reflex(second_point, (start + end) / 2);

    return {
        start,
        second_point,
        end,
        forth_point
    };
}

} // namespace

Rectangle::Rectangle(
    const Point& start,
    const Point& end,
    double coef
) : Polygon(createVertices(start, end, coef)) {}

// Rectangle
Point Rectangle::center() const {
    const auto& vertices = getVertices();
    return (vertices[0] + vertices[2]) / 2;
}

std::pair<Line, Line> Rectangle::diagonals() const {
    const auto& vertices = getVertices();
    return {
        { vertices[0], vertices[2] },
        { vertices[1], vertices[3] }
    };
}

}
