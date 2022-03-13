#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include <list>
#include <QImage>
#include <QPainter>
#include <splinevector.h>
#include <triangle.h>

using std::vector;
using std::list;
using Qt::MouseButton;
using Qt::LeftButton;
using Qt::RightButton;
using Qt::TransformationMode::FastTransformation;
using Qt::TransformationMode::SmoothTransformation;

enum EditMode {Brush_Mode, Spline_Mode, Raster_Mode};
enum Selection {TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left, BodySelect, NoSelect};

const float ipolMin = 0.001f;
const float ipolMax = 0.1f;
const unsigned char ptSize = 5;
const double pi = 3.14159265359;
const unsigned char maxVects = UCHAR_MAX;
const int maxSymDiv = 36;
const int minSymDiv = 1;

enum sym2DivType {Rotate, Reflect};

class Layer {

public:

    Layer();
    Layer(QSize qs);
    Layer(QImage in, int alphaValue);
    Layer(const Layer &layer);
    ~Layer();
    Layer& operator = (const Layer &layer);
    QImage *getCanvas();
    QImage getRenderCanvas();
    vector <QPoint> getRasterEdges();
    void applyFilterToRaster(Filter f);
    vector <list <Triangle> > getTriangles();
    vector <SplineVector> getVectors();
    void pasteVectors(list <SplineVector> svs);
    void pasteRaster(QImage rasterIn, double angleIn, pair <QPoint, QPoint> bounds);
    QImage getRaster();
    QImage* getRasterPtr();
    double getAngle();
    bool isRotating();
    pair <QPoint, QPoint> getBounds();
    vector <unsigned char> getActiveVectors();
    void spinWheel(int dy);
    void release(QPoint qp, MouseButton button);
    void moveLeft(QPoint qp);
    void moveRight(QPoint qp);
    void pressLeft(QPoint qp);
    MouseButton pressRight(QPoint qp);
    void doubleClickLeft(QPoint qp, bool ctrlFlag);
    void doubleClickRight(QPoint qp);
    void setMode(EditMode m);
    void flipVert();
    void flipHori();
    void fillColor(QPoint qp, QColor qc);
    void patternFill(QPoint qp, QColor qc);
    void setShiftFlag(bool b);
    bool isShiftActive();
    void setAlpha(int a);
    int getAlpha();
    int getWidth();
    void setWidth(int w);
    void widthUp();
    void widthDown();
    void setVectorColor1(QRgb a);
    void setVectorColor2(QRgb b);
    void setVectorTaper1(int a);
    void setVectorTaper2(int b);
    unsigned char getVectorTaperType();
    void setVectorTaperType(Taper t);
    void setVectorFilter(string s);
    void setVectorMode(VectorMode vm);
    void setBand(int b);
    void setGap(int g);
    int getBand();
    int getGap();
    void swapColors();
    void swapTapers();
    pair <char, char> getVectorTapers();
    pair <QRgb, QRgb> getVectorColors();
    int getVectorFilterStrength();
    void setVectorFilterStrength(int str);
    void cleanUp();
    void selectAll();
    void deselect();
    void deleteSelected();
    void clearVectors();
    Filter getFilter();
    int getFilterStrength();
    void setFilterStrength(int str);
    void setFilter(string filterName);
    static float getipol(float a, float b, float ipol);
    void applyKernalToSelection(QProgressDialog *qpd, string fileName);
    void setSymDivPt(QPoint qp);
    void setSymDiv(int div);
    void setSymDivType(int type);
    int getSymDiv();
    int symActive();
    Filter filter;


private:

    void calcLine();
    void drawRasterSelection(QImage *img, Qt::TransformationMode tm);
    void findSelection(QPoint qp);

    EditMode mode;
    Selection selection;
    vector <SplineVector> vects;
    vector <list <Triangle>> tris;
    vector <unsigned char> activeVects;
    char activePt;
    QImage *qi, alphaLayer, rasterselectOg;
    float ipolPts, limiter = ipolMin, limitCnt = 2.0, postAngle;
    int alpha;
    bool shiftFlag, selectOgActive, selecting, symCreate;
    QPoint deltaMove, boundPt1, boundPt2, rotateAnchor;
    QPoint symPt;
    int symDiv;
    sym2DivType divType;

};

#endif // LAYER_H
