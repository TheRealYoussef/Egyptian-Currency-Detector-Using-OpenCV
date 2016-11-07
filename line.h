#ifndef LINE_H
#define LINE_H

class Line {

public:

    int angle, ro;

    Line(int a, int r);

    bool operator==(const Line& line) const;

    bool operator<(const Line& line) const;
};

#endif // LINE_H
