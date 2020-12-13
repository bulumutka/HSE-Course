#include "geometry_utils.h"

#include "line.h"

#include <cmath>

namespace bulumutka::geometry_utils {

namespace {

const double EPS = 1e-6;
const double PI_CONST = 3.1415926;

double getCoef(const Line& line) {
    if (closeEquals(line.end.y - line.start.y, 0)) {
        return 0;
    } else {
        return (line.end.x - line.start.x) / (line.end.y - line.start.y);
    }
}

double isPointOnAxis(const Point& point, const Line& axis) {
    return closeEquals(
        (point.x - axis.start.x) * (axis.end.y - axis.start.y),
        (point.y - axis.start.y) * (axis.end.x - axis.start.x));
}

} // namespace

double PI(double coef) {
    return PI_CONST * coef;
}

bool closeEquals(double x, double y) {
    return std::abs(x - y) < EPS; 
}

double crossProduct(const Point& lhs, const Point& rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x; 
}

double scalarProduct(const Point& lhs, const Point& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

double dist(const Point& lhs, const Point& rhs) {
    return std::sqrt((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y));
}

const Point rotatePoint(const Point& point, double angle) {
    const double cos_a = std::cos(angle);
    const double sin_a = std::sin(angle);
    return { 
        cos_a * point.x - sin_a * point.y,
        sin_a * point.x + cos_a * point.y 
    };
}

double toRadians(double angle) {
    return PI() * angle / 180;
}

const Point reflex(const Point& point, const Point& center) {
    return center * 2 - point;
}

const Point reflex(const Point& point, const Line& axis) {
    const auto center = geometry_utils::findNormal(point, axis);
    return reflex(point, center);
}

const Point findNormal(const Point& point, const Line& axis) {
    if (isPointOnAxis(point, axis)) {
        return point;
    }

    const auto u = point - axis.start;
    const auto v = axis.end - axis.start;
    const double cos_a = geometry_utils::scalarProduct(u, v) 
        / geometry_utils::dist(u) 
        / geometry_utils::dist(v);
    const auto w = v / dist(v) * cos_a * dist(u);
    return axis.start + w;
}

const std::optional<Point> findLinesIntersection(const Line& lhs, const Line& rhs) {    
    const double k1 = getCoef(lhs);
    const double k2 = getCoef(rhs);

    if (closeEquals(k1, k2)) {
        return std::nullopt; // Lines are collinear
    }

    const double y = (rhs.start.x - lhs.start.x 
        + lhs.start.y * k1 - rhs.start.y * k2) / (k1 - k2);
    const double x = rhs.start.x + (y - rhs.start.y) * k2;
    return std::make_optional<Point>(x, y);
}

}
