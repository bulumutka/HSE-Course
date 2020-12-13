#include "polygon.h"

#include "geometry_utils.h"
#include "line.h"
#include "point.h"

#include <cassert>
#include <cmath>
#include <optional>

namespace bulumutka {

namespace {

std::optional<double> findSimilarCoef(
    const std::vector<Point>& lhs, const std::vector<Point>& rhs
) {
    int size = lhs.size();
    if (lhs.size() != rhs.size()) {
        return std::nullopt;
    }

    for (int i = 0; i != size; ++i) {
        std::optional<double> similar_coef;
        for (int j = 0; j != size; ++j) {
            const auto& first_current = lhs[i];
            const auto& first_next = lhs[(i + 1) % size];
            const auto& second_current = rhs[j];
            const auto& second_next = rhs[(j + 1) % size];
            double coef = geometry_utils::dist(first_current, first_next) 
                / geometry_utils::dist(second_current, second_next);
            
            if (!similar_coef) {
                similar_coef = coef;
            } else if (!geometry_utils::closeEquals(*similar_coef, coef)) {
                break;
            }

            if (j + 1 == size) {
                return similar_coef;
            }
        }
    }

    return std::nullopt;
}

} // namespace

Polygon::Polygon(const std::vector<Point>& vertices) 
    : vertices_(vertices) 
{
    assert(vertices_.size() > 2);    
}

// Polygon
int Polygon::verticesCount() const {
    return vertices_.size();
}

std::vector<Point> Polygon::getVertices() const {
    return vertices_;
}

bool Polygon::isConvex() const {
    bool isAlwaysTop = true;
    bool isAlwayButtom = true;

    if (verticesCount() == 3) return true;

    for (size_t i = 0; i != verticesCount(); ++i) {
        const auto& current = vertices_[i];
        const auto& next = vertices_[(i + 1) % verticesCount()];
        const auto& target = vertices_[(i + 2) % verticesCount()];

        assert(current != next);
        assert(next != target);

        const auto cr = geometry_utils::crossProduct(next - current, target - next);
        if (geometry_utils::closeEquals(cr, 0.0) && !isAlwayButtom && !isAlwaysTop) {
            return false;
        } else if (cr > 0) {
            isAlwayButtom = false;
        } else {
            isAlwaysTop = false;
        }
    }

    return true;
}

// Shape
double Polygon::perimeter() const {
    double perimeter = 0.0;
    for (size_t i = 0; i != verticesCount(); ++i) {
        const auto next_ind = (i + 1) % verticesCount();
        perimeter += geometry_utils::dist(vertices_[i], vertices_[next_ind]);
    }
    return perimeter;
}

double Polygon::area() const {
    const auto inside = (vertices_[0] + vertices_[1]) / 2;
    double area_sum = 0;

    int size = vertices_.size();
    for (int i = 0; i < size; ++i) {
        const auto& current = vertices_[i];
        const auto& next = vertices_[(i + 1) % size];
        area_sum += std::abs(geometry_utils::crossProduct(current - inside, next - inside));
    }

    return area_sum / 2;
}

bool Polygon::operator==(const Shape& other) const {
    const auto other_ptr = dynamic_cast<const Polygon*>(&other);
    if (!other_ptr || verticesCount() != other_ptr->verticesCount()) {
        return false;
    }

    for (size_t i = 0; i != verticesCount(); ++i) {
        bool isCyclicShift = true;
        for (size_t j = 0; j != verticesCount(); ++j) {
            if (this->vertices_[j] != other_ptr->vertices_[(i + j) % verticesCount()]) {
                isCyclicShift = false;
                break;
            }
        }
        if (isCyclicShift) {
            return true;
        }
    }
    return false;
}

bool Polygon::isCongruentTo(const Shape& other) const {
    const auto polygon_ptr = dynamic_cast<const Polygon*>(&other);
    if (!polygon_ptr) {
        return false;
    }
    const auto result = findSimilarCoef(vertices_, polygon_ptr->vertices_);
    if (result.has_value()) {
        return geometry_utils::closeEquals(*result, 1); 
    }
    return false;
}

bool Polygon::isSimilarTo(const Shape& other) const {
    const auto polygon_ptr = dynamic_cast<const Polygon*>(&other);
    if (!polygon_ptr) {
        return false;
    }
    return findSimilarCoef(vertices_, polygon_ptr->vertices_).has_value();
}

bool Polygon::containsPoint(const Point& point) const {
    double angle_sum = 0;
    const int size = verticesCount();
    for (int i = 0; i != size; ++i) {
        const auto curr = vertices_[i] - point;
        const auto next = vertices_[(i + 1) % size] - point;
        const double scalar_product = geometry_utils::scalarProduct(curr, next);
        const double cross_product = geometry_utils::crossProduct(curr, next);
        if (geometry_utils::closeEquals(cross_product, 0.0) 
                && (geometry_utils::closeEquals(scalar_product, 0.0) || scalar_product < 0)) {
            return true;
        } else {
            angle_sum += std::atan2(cross_product, scalar_product);
        }
    }
    return geometry_utils::closeEquals(angle_sum, geometry_utils::PI(2));
}

void Polygon::rotate(const Point& center, double angle) {
    const double radians = geometry_utils::toRadians(angle);
    for (auto& point: vertices_) {
        point = geometry_utils::rotatePoint(point - center, radians) + center;
    }
}

void Polygon::reflex(const Point& center) {
    for (auto& point: vertices_) {
        point = geometry_utils::reflex(point, center);
    }
}

void Polygon::reflex(const Line& axis) {
    for (auto& point: vertices_) {
        point = geometry_utils::reflex(point, axis);
    }
}

void Polygon::scale(const Point& center, double coef) {
    for (auto& point: vertices_) {
        point = center + (point - center) * coef;
    }
}

}
