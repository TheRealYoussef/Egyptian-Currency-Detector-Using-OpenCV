#ifndef CIRCLE_H
#define CIRCLE_H

class Circle {

public:

    int x, y;

    Circle(int _x, int _y);

    bool operator==(const Circle& circle) const;

    bool operator<(const Circle& circle) const;
};

#endif // CIRCLE_H
