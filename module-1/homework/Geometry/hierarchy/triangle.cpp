#include "triangle.h"

#include "circle.h"
#include "geometry_utils.h"
#include "line.h"
#include "point.h"

#include <vector>

namespace bulumutka {

Triangle::Triangle(
    const Point& first,
    const Point& second,
    const Point& third
) : Polygon({ first, second, third }) {}

Circle Triangle::circumscribedCircle() const {
    const auto& vertices = getVertices();
    const auto first_middle = (vertices[0] + vertices[2]) / 2;
    const auto second_middle = (vertices[0] + vertices[1]) / 2;
    const auto center = geometry_utils::findLinesIntersection(
        { vertices[1], first_middle },
        { vertices[2], second_middle }
    );

    assert(center.has_value());
    
    const auto radius = geometry_utils::dist(first_middle, *center);
    return Circle(*center, radius);
}

Circle Triangle::inscribedCircle() const {
    const auto& vertices = getVertices();
    const auto first_norm = geometry_utils::findNormal(vertices[1],
        { vertices[0], vertices[2] });
    const auto second_norm = geometry_utils::findNormal(vertices[2],
        { vertices[0], vertices[1] });
    const auto center = geometry_utils::findLinesIntersection(
        { vertices[1], first_norm },
        { vertices[2], second_norm }
    );

    assert(center.has_value());
    
    const auto radius = geometry_utils::dist(first_norm, *center);
    return Circle(*center, radius);
}

}
