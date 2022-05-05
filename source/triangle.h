#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QPoint>


static int sign (QPoint p1, QPoint p2, QPoint p3) {
    return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
}

static int inTri (QPoint qp, QPoint a, QPoint b, QPoint c) {
    int d1 = sign(qp, a, b);
    int d2 = sign(qp, b, c);
    int d3 = sign(qp, c, a);
    return ((d1 >= 0) && (d2 >= 0) && (d3 >= 0)) || ((d1 <= 0) && (d2 <= 0) && (d3 <= 0));
}


struct Triangle {
    QPoint a, b, c;

    Triangle(QPoint A, QPoint B, QPoint C) {
        a = A;
        b = B;
        c = C;
    }

    Triangle(const Triangle &t) {
        *this = t;
    }

    Triangle& operator = (const Triangle &t) {
        a = t.a;
        b = t.b;
        c = t.c;
        return *this;
    }
};

#endif // TRIANGLE_H
