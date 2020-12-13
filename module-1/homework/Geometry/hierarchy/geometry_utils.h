#ifndef GEOMETRY_UTILS_H_
#define GEOMETRY_UTILS_H_

#include "point.h"

#include <optional>

namespace bulumutka {

struct Line;

namespace geometry_utils {

/**
 * Returns pi * coef
 */
double PI(double coef = 1);

/**
 *  Use this function to compare two double values
 */
bool closeEquals(double x, double y);

double crossProduct(const Point& lhs, const Point& rhs);

double scalarProduct(const Point& lhs, const Point& rhs);

double dist(const Point& lhs, const Point& rhs = Point(0, 0));

const Point rotatePoint(const Point& point, double angle);

double toRadians(double angle);

/**
 * Reflex by point
 */
const Point reflex(const Point& point, const Point& center);

/**
 * Reflex by axis
 */
const Point reflex(const Point& point, const Line& axis);

/**
 * Finds the nearest point on the axis, to the {@code point}
 */
const Point findNormal(const Point& point, const Line& axis);

const std::optional<Point> findLinesIntersection(const Line& lhs, const Line& rhs);

}}

#endif
