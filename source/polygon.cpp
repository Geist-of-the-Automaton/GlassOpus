#include "polygon.h"

Polygon::Polygon() {
    edgeSize = 0;
    eColor = 0xFFAAAAAA;
    pColor = 0xFF000000;
    pm = ColorGon;
    showDivs = false;
    f.setFilter("Negative");
}

Polygon::Polygon(const Polygon &gon) {
    *this = gon;
}

Polygon& Polygon::operator=(const Polygon &gon) {
    tris = gon.tris;
    pts = gon.pts;
    edgeSize = gon.edgeSize;
    pColor = gon.pColor;
    eColor = gon.eColor;
    f = gon.f;
    pm = gon.pm;
    showDivs = gon.showDivs;
    return *this;
}

list <Triangle> Polygon::getTris() {
    return tris;
}

vector <QPoint> Polygon::getPts() {
    return pts;
}

void Polygon::setEdgeSize(int size) {
    if (size >= 0 && size <= maxEdgeSize)
        edgeSize = size;
}

int Polygon::getEdgeSize() {
    return edgeSize;
}

void Polygon::setPolyColor(QRgb qc) {
    pColor = qc;
}

void Polygon::setEdgeColor(QRgb qc) {
    eColor = qc;
}

QRgb Polygon::getPolyColor() {
    return pColor;
}

QRgb Polygon::getEdgeColor() {
    return eColor;
}

void Polygon::setFilter(string s) {
    f.setFilter(s);
}

Filter Polygon::getFilter() {
    return f;
}

void Polygon::setFilterStrength(int s) {
    f.setStrength(s);
}

void Polygon::removePt(int index) {
    pts.erase(pts.begin() + index);
    triangulate();
}

void Polygon::addPt(QPoint qp, int index) {
    if (index == -1 || index + 1 == pts.size())
        pts.push_back(qp);
    else
        pts.insert(pts.begin() + index + 1, qp);
    triangulate();
}

void Polygon::movePt(QPoint loc, size_t index) {
    pts[index] = loc;
    triangulate();
}

void Polygon::translate(QPoint qp) {
    QPoint change = qp - rPt1;
    for (int i = 0; i < pts.size(); ++i)
        pts[i] += change;
    rPt1 = qp;
    triangulate();
}

void Polygon::scale(QPoint qp) {
    pts = backup;
    for (int i = 0; i < pts.size(); ++i)
        pts[i] -= rPt1;
    if (rPt2.x() == 0 || rPt2.y() == 0)
        return;
    float xChange = static_cast<float>(qp.x()) / static_cast<float>(rPt2.x());
    float yChange = static_cast<float>(qp.y()) / static_cast<float>(rPt2.y());
    for (int i = 0; i < pts.size(); ++i)
        pts[i] = QPoint(static_cast<float>(pts[i].x()) * xChange, static_cast<float>(pts[i].y()) * yChange) + rPt1;
    triangulate();
}

void Polygon::rotate(QPoint qp) {
    pts = backup;
    float angle = -(atan2(rPt2.y() - rPt1.y(), rPt2.x() - rPt1.x()) - atan2(qp.y() - rPt1.y(), qp.x() - rPt1.x()));
    float s = sin(angle);
    float c = cos(angle);
    for (int i = 0; i < pts.size(); ++i) {
        pts[i] -= rPt1;
        pts[i] = QPoint(pts[i].x() * c - pts[i].y() * s, pts[i].x() * s + pts[i].y() * c);
        pts[i] += rPt1;
    }
    triangulate();
}

void Polygon::cleanup() {
    backup.clear();
}

int Polygon::inPoly(QPoint qp) {
    for (Triangle &t : tris)
        if (inTri(qp, t.a, t.b, t.c))
            return 1;
    return 0;
}

QPoint Polygon::getCenter() {
    int x = 0, y = 0;
    for (QPoint pt : pts) {
        x += pt.x();
        y += pt.y();
    }
    x /= pts.size();
    y /= pts.size();
    rPt1 = rPt2 = QPoint(x, y);
    return rPt1;
}

void Polygon::setRPt1(QPoint qp) {
    rPt1 = qp;
}

void Polygon::setRPt2(QPoint qp) {
    rPt2 = qp;
}

void Polygon::makeBackup() {
    backup = pts;
}

void Polygon::triangulate() {
    vector <QPoint> pts2 = pts;
    vector <QPoint> processed;
    int isCCW = -1;
    int ccw = 0, cw = 0;
    for (int i = 0; i < pts2.size(); ++i) {
        int i1 = (i + 1) % pts2.size();
        int i2 = (i + 2) % pts2.size();
        int d = det(pts2[i], pts2[i1], pts2[i2]);
        if (d < 0)
            ++ccw;
        else if (d > 0)
            ++cw;
    }
    isCCW = ccw > cw ? 1 : 0;
    tris.clear();
    while (pts2.size() >= 3) {
        int size = tris.size();
        for (int i = 0; i < pts2.size(); ++i) {
            int flag = 1;
            int i1 = (i + 1) % pts2.size();
            int i2 = (i + 2) % pts2.size();
            if ((det(pts2[i], pts2[i1], pts2[i2]) < 0 && isCCW) || (det(pts2[i], pts2[i1], pts2[i2]) > 0 && !isCCW)) {
                if (i2 < i) {
                    for (int j = i2 + 1; j < i; ++j)
                        if (inTri(pts2[j], pts2[i], pts2[i1], pts2[i2])) {
                            flag = 0;
                            break;
                        }
                }
                else {
                    for (int j = 0; j < i; ++j)
                        if (inTri(pts2[j], pts2[i], pts2[i1], pts2[i2])) {
                            flag = 0;
                            break;
                        }
                    if (flag)
                        for (int j = i2 + 1; j < pts2.size(); ++j)
                            if (inTri(pts2[j], pts2[i], pts2[i1], pts2[i2])) {
                                flag = 0;
                                break;
                            }
                }
                if (flag)
                    for (QPoint qp : processed)
                        if (inTri(qp, pts2[i], pts2[i1], pts2[i2])) {
                            flag = 0;
                            break;
                        }
                if (flag) {
                    tris.push_back(Triangle(pts2[i], pts2[i1], pts2[i2]));
                    processed.push_back(pts2[i1]);
                    pts2.erase(pts2.begin() + i1);
                    //break;
                }
            }
        }
        if (tris.size() == size)
            break;
    }
}

void Polygon::reducePts() {
    vector <QPoint> res;
    for (int i = 0; i < pts.size(); i += 2) {
        QPoint center = pts[i] + pts[(i + 1) % pts.size()];
        res.push_back(QPoint(center.x() / 2, center.y() / 2));
    }
    if (res.size() >= 3)
        pts = res;
    triangulate();
}

void Polygon::setPolyMode(PolyMode mode) {
    pm = mode;
}

PolyMode Polygon::getPolyMode() {
    return pm;
}

Polygon Polygon::ellipse(QPoint cPt) {
    int xrad = cPt.x() - 2;
    int yrad = cPt.y() - 2;
    Polygon p;
    for (float angle = 180.0; angle > -180.0; angle -= 1.0) {
        float radAng = angle * 0.0174533;
        float s = sin(radAng);
        float c = cos(radAng);
        QPoint qp = QPoint(xrad * c + xrad, yrad * s + yrad);
        p.addPt(qp);
    }
    while (p.getPts().size() - 2 != p.getTris().size())
        p.reducePts();
    return p;
}

void Polygon::setShowDivs(bool b) {
    showDivs = b;
}

bool Polygon::getShowDivs() {
    return showDivs;
}

