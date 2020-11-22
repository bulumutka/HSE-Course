#ifndef LINE_H_
#define LINE_H_

namespace bulumutka {

class Point;

struct Line {
    Line(const Point& start, const Point& end);

    bool operator==(const Line& other) const;

    const Point start;
    const Point end;
};

}

#endif
