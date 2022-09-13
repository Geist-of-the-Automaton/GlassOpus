#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include <list>
#include <QImage>
#include <QPainter>
#include <QProgressDialog>
#include <splinevector.h>
#include <triangle.h>
#include <polygon.h>
#include <drawtext.h>

using std::vector;
using std::list;
using Qt::MouseButton;
using Qt::LeftButton;
using Qt::RightButton;
using Qt::TransformationMode::FastTransformation;
using Qt::TransformationMode::SmoothTransformation;

enum EditMode {Brush_Mode, Spline_Mode, Raster_Mode, Polygon_Mode, Text_Mode, Mode_3D};
enum Selection {TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left, BodySelect, NoSelect};

const float ipolMin = 0.001f;
const float ipolMax = 0.1f;
const double pi = 3.14159265359;
const unsigned char maxVects = UCHAR_MAX;

const int maxSymDiv = 36;
const int minSymDiv = 1;

class Layer : public QWidget {

        Q_OBJECT

public:

    Layer();
    Layer(QSize qs);
    Layer(QImage in, int alphaValue);
    Layer(const Layer &layer);
    ~Layer();
    Layer& operator = (const Layer &layer);
    QImage *getCanvas();
    QImage getRenderCanvas();
    QImage render(int rerender = 0, QProgressDialog *progress = nullptr);
    vector <QPoint> getRasterEdges();
    void applyFilterToRaster(Filter f);
    vector <list <Triangle> > getTriangles();
    vector <SplineVector> getVectors();
    void pasteText(list <DrawText> dws);
    void pasteVectors(list <SplineVector> svs);
    void pastePolygons(list <Polygon> pgs);
    void pasteRaster(QImage rasterIn, double angleIn, pair <QPoint, QPoint> bounds);
    QImage getRaster();
    QImage* getRasterPtr();
    double getAngle();
    bool isRotating();
    pair <QPoint, QPoint> getBounds();
    vector <unsigned char> getActiveVectors();
    void spinWheel(int dy);
    void release(QPoint qp, MouseButton button);
    void moveLeft(QPointF qp, QPoint lastPos = QPoint());
    void moveRight(QPoint qp,  QPoint lastPos = QPoint());
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
    void clearPolygons();
    void clearTexts();
    void clearAll();
    void wipe();
    Filter getFilter();
    int getFilterStrength();
    void setFilterStrength(int str);
    void setFilter(string filterName);
    static float getipol(float a, float b, float ipol);
    void applyKernalToSelection(QProgressDialog *qpd, string fileName);
    void setSym(QPoint qp, int div, int ofEvery, int skip);
    int symActive();
    void polyToSelect();

    vector <Polygon> getPolgons();
    vector <unsigned char> getActiveGons();
    void reduceGonPts();
    void setDragDraw(bool dd);
    pair <QColor, QColor> getGonColor();
    void setGonColor(pair <QColor, QColor> qcs);
    int getEdgeSize();
    void setEdgeSize(int size);
    void setPolygonFilter(string s);
    void setPolyFilterStrength(int s);
    int getPolyFilterStrength();
    void setPolyMode(PolyMode mode);
    void createEllipse(QPoint cPt);
    void setShowDivs(bool b);

    //swap colors,

    vector <DrawText> getTexts();
    vector <unsigned char> getActiveTexts();
    QFont getFont();
    QStaticText getText();
    QColor getTextColor();
    void setFont(QFont font);
    void setText(QStaticText text);
    void setTextColor(QColor color);
    bool updateText(Qt::Key key, bool shiftFlag);
    void magicSelect(QPoint qp, vec4 vals);
    void invertSelection();

    void setVisibility(bool vis);
    bool isVisible();

    bool hasFilterItems();


    Filter filter;

signals:
    void visUpdated();
    void viewUpdated();

private:

    void calcLine();
    void drawRasterSelection(QImage *img, Qt::TransformationMode tm);
    void findSelection(QPoint qp);

    EditMode mode;
    Selection selection;
    vector <SplineVector> vects;
    vector <Polygon> gons;
    vector <DrawText> texts;
    vector <list <Triangle>> vectTris;
    vector <list <Triangle>> gonTris;
    vector <unsigned char> activeVects;
    vector <unsigned char> activeGons;
    vector <unsigned char> activeTexts;
    int activePt;
    QImage *qi, alphaLayer, rasterselectOg, rendered;
    float ipolPts, limiter = ipolMin, limitCnt = 2.0, postAngle;
    int alpha;
    bool shiftFlag, selectOgActive, selecting, symCreate, dragDraw, addOrSub, visible, hasFiltItems;
    QPoint deltaMove, boundPt1, boundPt2, rotateAnchor, ogLocation;
    QPoint symPt;
    int symDiv, symOfEvery, symSkip;

};


void renderLayer(QProgressDialog *qpd, QImage *toProcess, Layer *layer);
void calcLine(SplineVector sv, list <Triangle> *tris);
void fillTri(QImage *toProcess, Triangle t, QRgb color);
void filterTri(QImage *toProcess, Triangle t, Filter f);
void fillBTri(QImage *toProcess, QPoint a, QPoint b, QPoint c, QRgb color);
void fillTTri(QImage *toProcess, QPoint a, QPoint b, QPoint c, QRgb color);
void filterBTri(QImage *toProcess, QPoint a, QPoint b, QPoint c, Filter f);
void filterTTri(QImage *toProcess, QPoint a, QPoint b, QPoint c, Filter f);

#endif // LAYER_H
