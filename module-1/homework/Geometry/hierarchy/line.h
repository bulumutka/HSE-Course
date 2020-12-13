#ifndef LINE_H_
#define LINE_H_

#include "point.h"

namespace bulumutka {

struct Line {
    Line(const Point& start, const Point& end);

    bool operator==(const Line& other) const;

    Point start;
    Point end;
};

}

#endif
