#include "square.h"

#include "circle.h"
#include "point.h"

#include <cmath>

namespace bulumutka {

namespace {

const double SQUARE_COEF = 1.0;

} // namespace

Square::Square(
    const Point& start,
    const Point& end
) : Rectangle(start, end, SQUARE_COEF) {}

Circle Square::circumscribedCircle() const {
    const auto& vertices = getVertices();
    const auto center = (vertices[0] + vertices[2]) / 2;
    const auto radius = std::abs(vertices[0].x - vertices[2].x) / std::sqrt(2); 
    return Circle(center, radius);
}

Circle Square::inscribedCircle() const {
    const auto& vertices = getVertices();
    const auto center = (vertices[0] + vertices[2]) / 2;
    const auto radius = std::abs(vertices[0].x - vertices[2].x) / 2; 
    return Circle(center, radius);
}

}
