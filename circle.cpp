#include "circle.h"
#include <cmath>

Circle::Circle(int _x, int _y) : x(_x), y(_y) {}

bool Circle::operator==(const Circle& circle) const {
    double dist = std::sqrt((x - circle.x) * (x - circle.x) + (y - circle.y) * (y - circle.y));
    return dist < 64;
}

bool Circle::operator<(const Circle& circle) const {
    if (x != circle.x)
        return x < circle.x;
    return y < circle.y;
}
