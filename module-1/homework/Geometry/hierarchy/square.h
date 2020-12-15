#ifndef SQUARE_H_
#define SQUARE_H_

#include "rectangle.h"

namespace bulumutka {

class Circle;
class Point;

class Square : public Rectangle {
public:
    Square(const Point& start, const Point& end);

    virtual ~Square() override = default;

    virtual Circle circumscribedCircle() const;
    virtual Circle inscribedCircle() const;
};

}

#endif
