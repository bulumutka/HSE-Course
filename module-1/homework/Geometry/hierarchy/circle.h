#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "ellipse.h"

namespace bulumutka {

class Point;

class Circle : public Ellipse {
public:
    Circle(const Point& center, double radius);
    virtual ~Circle() override = default;

    virtual double radius() const;
};

}

#endif
