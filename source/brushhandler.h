#ifndef brushHANDLER_H
#define brushHANDLER_H

#include <vector>
#include <list>
#include <QPoint>
#include <QImage>
#include <brush.h>
#include <graphics.h>
#include <stdfuncs.h>
#include <layer.h>

using std::vector;
using std::list;
using std::min;
using std::max;
using graphics::Filter;

enum appMethod {overwrite, additive, subtractive, filter, radial, sample};

const string appMethods[] = {"Overwrite", "Additive", "Subtractive", "Filter", "Radial", "Sample"};
const int numMethods = 6;
const int maxDensity = RAND_MAX / 256;
const int minDensity = 0;
const unsigned char maxStrength = 255;
const unsigned char minStrength = 1;

class brushHandler {

public:

    brushHandler(unsigned char str = 128, int size = 10, int density = 0, string type = appMethods[0], QColor qc = QColor(0xFF000000));
    ~brushHandler();
    void setAppMethod(string type);
    void setPoint(QPoint qp);
    void setDensity(int density);
    void setStrength(int str);
    void setShape(string shape, vector <vector <unsigned char> > custom = vector <vector <unsigned char> > ());
    void setBrushColor(QColor qc);
    void setFillColor(QColor qc);
    void setFilter(string filterName);
    void setSamplePoint(QPoint sPnt);
    void setRelativePoint(QPoint rPnt);
    const unsigned char *const *const getBrushMap();
    void setPattern(vector <vector <unsigned char> > pattern);
    void setPatternInUse(int used);
    void radialUpdate(int size, vector <int> pts);
    vector <vector <unsigned char> > getPatternMap();
    bool getPatternInUse();
    int getMethodIndex();
    int getFilterIndex();
    QColor getBrushColor();
    QColor getFillColor();
    int getDensity();
    int getStength();
    int getSize();
    int getFullSize();
    void densityUp();
    void densityDown();
    void strengthUp();
    void strengthDown();
    void applyBrush(QImage *qi, QPoint qp);
    void erase(QImage *qi, QPoint qp);
    void setInterpolationActive(bool flag);
    void setSize(int size);
    Shape getBrushShape();
    void setSym(QPoint qp, int div, int ofEvery, int skip);
    QPoint getSymPt();
    void setCanvasSize(int w, int h);
    void resetCheck2(QPoint least, QPoint most);

private:

    void resetPoint();
    int onScreen(int x, int y, int xMax, int yMax);
    void overwrite(QImage *qi);
    void additive(QImage *qi);
    void subractive(QImage *qi);
    void filter(QImage *qi);
    void radial(QImage *qi);
    void sample(QImage *qi);
    QColor getAffected();
    void shift();
    void shiftLeft();
    void shiftRight();
    void shiftUp();
    void shiftDown();

    bool ipolActive, patternInUse;
    int sprayDensity;
    unsigned char strength;
    unsigned char checkMap[3 * maxRadius + 1][3 * maxRadius + 1];
    vector <vector <unsigned char>> checkMap2;
    appMethod method;
    Brush brush;
    QColor brushColor, fillColor;
    Filter brushFilter;
    QPoint currPnt, lastPnt, relativityPoint, samplePoint;
    list <QPoint> toProcess;
    float radialMap[2 * maxRadius + 1][2 * maxRadius + 1];
    vector <int> radialValues;
    vector <vector <unsigned char> > patternMap;
    QPoint symPt;
    int symDiv, symOfEvery, symSkip;
};

#endif // brushHANDLER_H
