#ifndef POLYGON_H
#define POLYGON_H

#include <QPoint>
#include <QColor>

#include <list>
#include <vector>

#include <triangle.h>
#include <graphics.h>

using std::list;
using std::vector;
using std::min;
using std::max;

using graphics::Filter;

const int maxEdgeSize = 100;
static bool drawType = false;

enum PolyMode {ColorGon, FilterGon};

static int det (QPoint a, QPoint b, QPoint c) {
    QPoint u = b - a;
    QPoint v = c - b;
    return u.x() * v.y() - u.y() * v.x();
}

class Polygon
{
public:
    Polygon();
    Polygon(const Polygon &gon);
    Polygon& operator = (const Polygon &gon);
    list <Triangle> getTris();
    vector <QPoint> getPts();
    void setEdgeSize(int size);
    int getEdgeSize();
    void setPolyColor(QRgb qc);
    void setEdgeColor(QRgb qc);
    QRgb getPolyColor();
    QRgb getEdgeColor();
    void setFilter(string s);
    Filter getFilter();
    void setFilterStrength(int s);
    void removePt(int index);
    void addPt(QPoint qp, int index = -1);
    void movePt(QPoint loc, size_t index);
    void translate(QPoint qp);
    void scale(QPoint qp);
    void rotate(QPoint qp);
    void cleanup();
    int inPoly(QPoint qp);
    QPoint getCenter();
    void setRPt1(QPoint qp);
    void setRPt2(QPoint qp);
    void makeBackup();
    void reducePts();
    void setPolyMode(PolyMode mode);
    PolyMode getPolyMode();
    static Polygon ellipse(QPoint cPt);
    void setShowDivs(bool b);
    bool getShowDivs();


   /*
    * filter
    * filter strength
    * mode
    * transparency
    */

private:
    void triangulate();

    PolyMode pm;
    list <Triangle> tris;
    vector <QPoint> pts, backup;
    int edgeSize;
    QRgb pColor, eColor;
    QPoint rPt1, rPt2;
    Filter f;
    bool showDivs;

};

#endif // POLYGON_H
