#include "line.h"
#include <cmath>

Line::Line(int a, int r) : angle(a), ro(r) {}

bool Line::operator==(const Line& line) const {
    return std::abs(angle - line.angle) <= 87 && std::abs(ro - line.ro) <= 40;
}

bool Line::operator<(const Line& line) const {
    if (angle != line.angle)
        return angle < line.angle;
    return ro < line.ro;
}
