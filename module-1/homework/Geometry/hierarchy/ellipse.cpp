#include "ellipse.h"

#include "geometry_utils.h"
#include "line.h"

#include <cmath>
#include <vector>

namespace bulumutka {

Ellipse::Ellipse(
    const Point& first,
    const Point& second,
    double dist
) : first_(first),
    second_(second),
    a_(dist / 2)
{}

std::pair<Point, Point> Ellipse::focuses() const {
    return { first_, second_ };
}

std::pair<Line, Line> Ellipse::directrices() const {
    const auto coef = a_ / eccentricity();
    const auto center = (first_ + second_) / 2;
    auto s = (first_ - center);
    s = s / geometry_utils::dist(s) * coef;
    const auto first_d_point = center + s;
    
    auto u = first_d_point - center;
    u = geometry_utils::rotatePoint(u, geometry_utils::toRadians(90));
    const auto second_d_point = first_d_point + u;

    return {
        { first_d_point, second_d_point },
        { geometry_utils::reflex(first_d_point, center),
          geometry_utils::reflex(second_d_point, center )}
    };
}

double Ellipse::eccentricity() const {
    return linearEccentricity() / a_;
}

Point Ellipse::center() const {
    return (first_ + second_) / 2;
}

double Ellipse::perimeter() const {
    const double b = semiMinorAxis();
    return 4 * (geometry_utils::PI() * a_ * b + (a_ - b)) / (a_ + b);
}

double Ellipse::area() const {
    return semiMinorAxis() * a_ * geometry_utils::PI();
}

bool Ellipse::operator==(const Shape& other) const {
    const auto* other_ptr = dynamic_cast<const Ellipse*>(&other);
    if (!other_ptr) {
        return false;
    }
    return geometry_utils::closeEquals(a_, other_ptr->a_) 
        && ((first_ == other_ptr->first_ && second_ == other_ptr->second_)
            || (first_ == other_ptr->second_ && second_ == other_ptr->first_)); 
}

bool Ellipse::isCongruentTo(const Shape& other) const {
    const auto* other_ptr = dynamic_cast<const Ellipse*>(&other);
    if (!other_ptr) {
        return false;
    }
    const auto is_dist_equals = geometry_utils::closeEquals(
        geometry_utils::dist(first_, second_),
        geometry_utils::dist(other_ptr->first_, other_ptr->second_)
    );
    return geometry_utils::closeEquals(a_, other_ptr->a_) && is_dist_equals;
}

bool Ellipse::isSimilarTo(const Shape& other) const {
    const auto* other_ptr = dynamic_cast<const Ellipse*>(&other);
    if (!other_ptr) {
        return false;
    }
    const auto scale = geometry_utils::dist(first_, second_) 
        / geometry_utils::dist(other_ptr->first_, other_ptr->second_);
    return geometry_utils::closeEquals(a_, scale * other_ptr->a_);
}

bool Ellipse::containsPoint(const Point& point) const {
    const auto dist = geometry_utils::dist(first_, point)
        + geometry_utils::dist(second_, point);
    return geometry_utils::closeEquals(dist, 2 * a_) || dist < 2 * a_;
}

void Ellipse::rotate(const Point& center, double angle) {
    const double radians = geometry_utils::toRadians(angle);
    std::vector<Point*> focuses { &first_, &second_ };
    for (auto* focus_ptr: focuses) {
        auto& focus = *focus_ptr;
        focus = geometry_utils::rotatePoint(focus - center, radians) + center;
    }
}

void Ellipse::reflex(const Point& center) {
    std::vector<Point*> focuses { &first_, &second_ };
    for (auto* focus_ptr: focuses) {
        auto& focus = *focus_ptr;
        focus = geometry_utils::reflex(focus, center);
    }
}

void Ellipse::reflex(const Line& axis) {
    std::vector<Point*> focuses { &first_, &second_ };
    for (auto* focus_ptr: focuses) {
        auto& focus = *focus_ptr;
        focus = geometry_utils::reflex(focus, axis);
    }
}

void Ellipse::scale(const Point& center, double coef) {
    std::vector<Point*> focuses { &first_, &second_ };
    for (auto& focus_ptr: focuses) {
        auto& focus = *focus_ptr;
        focus = center + (focus - center) * coef;
    }
}

double Ellipse::linearEccentricity() const {
    return geometry_utils::dist(first_, second_) / 2;
}

double Ellipse::semiMinorAxis() const {
    return a_ * std::sqrt(1 - eccentricity() * eccentricity());
}

}
