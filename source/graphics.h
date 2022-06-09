#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <list>
#include <functional>
#include <vector>
#include <fstream>
#include <QImage>
#include <QLabel>
#include <QProgressDialog>
#include <QCoreApplication>
#include <stdfuncs.h>
#include <vec_mat_maths.h>
#include <cmath>
#include <thread>

using std::max;
using std::min;
using std::string;
using std::list;
using std::function;
using std::vector;
using std::fstream;
using std::ios;
using std::pair;
using std::to_string;
using std::string;
using std::atan;
using std::thread;

typedef vector<vector<float>> Kernel;
typedef pair<bool,Kernel> KernelData;

namespace graphics {

const string filterNames[] =   {"Normal (rgb)", "Greyscale", "Polarize", "Negative",   "Burn",      "Dodge",        "Enshadow",    "Red Channel", "Green Channel", "Blue Channel",  "Yellow Channel", "Cyan Channel", "Magenta Channel", "Hue Channel", "Saturation Channel", "Value Channel", "Lightness Channel", "Red Pass", "Green Pass", "Blue Pass", "Yellow Pass", "Cyan Pass", "Magenta Pass", "Red Filter", "Green Filter", "Blue Filter", "Yellow Filter", "Cyan Filter", "Magenta Filter", "Burn Red", "Burn Green", "Burn Blue", "Burn Yellow", "Burn Cyan", "Burn Magenta", "Dodge Red", "Dodge Green", "Dodge Blue", "Dodge Yellow", "Dodge Cyan", "Dodge Magenta", "RBG", "GRB", "GBR", "BRG", "BGR", "Color Noise", "Grey Noise"};
const string vectorFilters[] = {"Greyscale",    "Polarize",  "Red Pass", "Green Pass", "Blue Pass", "Magenta Pass", "Yellow Pass", "Cyan Pass",   "Red Filter",    "Green Filter",  "Blue Filter", "Color Noise", "Grey Noise"};
const int filterPresets[] =    {0             , 255        ,  128      , 0           , 20          , 20           , 64           ,  0           , 0              , 0             , 0                , 0             , 0                , 0            , 0                   , 0              , 0                  ,  255         ,  255     , 255        , 255           , 255          , 255        , 255         , 255           , 255          , 255            , 20           , 20              , 20        , 20          , 20         , 20           , 20         , 20            , 20         , 20           , 20          , 20            , 20          , 20             , 0    , 0    , 0    , 0    , 0    , 4            , 4           };
const int numFilters = 48;
const double minZoom = 1.0 / 4.0;
const double maxZoom = 8.0;
const int minColor = 0;
const int maxColor = 255;
const int bins = 256;
const double pi = 3.14159265359;
const float minsLAB[3] {0.0f, -86.1847f, -107.864f};
const float scalesLAB[3] {2.55f, 1.3825f, 1.2602f};

const QStringList eTypes {"RGB", "HSV / HSB", "HSL / HSI", "CIELAB"};
enum eType {RGB, HSV, HSL, LAB};
const QStringList mTypes {"RGB", "Hue", "Saturation", "Value", "Lightness"};
enum mType {RGB_I, HUE, SAT, HSV_V, HSL_L};
enum bType {NOT, AND, NAND, OR, NOR, XOR, XNOR};

enum tType {sRGB, HunterLAB, CIELAB};

class Filter {

public:

    Filter(int strength = filterPresets[0], string filterName = filterNames[0]);
    Filter (const Filter &filter);
    Filter& operator = (const Filter &lhs);
    void applyTo(QImage *qi);
    QRgb applyTo(QColor qc);
    int getStrength();
    void setStrength(int value);
    void strengthUp();
    void strengthDown();
    void setFilter(string filterName);
    int getFilterIndex();

private:

    function <QRgb (QColor, int)> filterApplicator;
    int strength;
    size_t filterIndex;
};

class Filtering {

public:

    static QRgb toRGB (int a, int r, int g, int b);
    static QRgb greyscale (QColor qc, int strength);
    static QRgb polarize (QColor qc, int strength);
    static QRgb negative (QColor qc, int strength);
    static QRgb burn (QColor qc, int strength);
    static QRgb dodge (QColor qc, int strength);
    static QRgb enshadow(QColor qc, int strength);
    static QRgb redChannel (QColor qc, int strength);
    static QRgb greenChannel (QColor qc, int strength);
    static QRgb blueChannel (QColor qc, int strength);
    static QRgb yellowChannel(QColor qc, int strength);
    static QRgb cyanChannel(QColor qc, int strength);
    static QRgb magentaChannel(QColor qc, int strength);
    static QRgb hueChannel (QColor qc, int strength);
    static QRgb satChannel (QColor qc, int strength);
    static QRgb valChannel (QColor qc, int strength);
    static QRgb litChannel (QColor qc, int strength);
    static QRgb redPass (QColor qc, int strength);
    static QRgb greenPass (QColor qc, int strength);
    static QRgb bluePass (QColor qc, int strength);
    static QRgb magentaPass (QColor qc, int strength);
    static QRgb yellowPass (QColor qc, int strength);
    static QRgb cyanPass (QColor qc, int strength);
    static QRgb redFilter (QColor qc, int strength);
    static QRgb greenFilter (QColor qc, int strength);
    static QRgb blueFilter (QColor qc, int strength);
    static QRgb yellowFilter (QColor qc, int strength);
    static QRgb cyanFilter (QColor qc, int strength);
    static QRgb magentaFilter (QColor qc, int strength);
    static QRgb burnRed (QColor qc, int strength);
    static QRgb burnGreen (QColor qc, int strength);
    static QRgb burnBlue (QColor qc, int strength);
    static QRgb burnYellow (QColor qc, int strength);
    static QRgb burnCyan (QColor qc, int strength);
    static QRgb burnMagenta (QColor qc, int strength);
    static QRgb dodgeRed (QColor qc, int strength);
    static QRgb dodgeGreen (QColor qc, int strength);
    static QRgb dodgeBlue (QColor qc, int strength);
    static QRgb dodgeYellow (QColor qc, int strength);
    static QRgb dodgeCyan (QColor qc, int strength);
    static QRgb dodgeMagenta (QColor qc, int strength);
    static QRgb rgb (QColor qc, int strength);
    static QRgb rbg (QColor qc, int strength);
    static QRgb grb (QColor qc, int strength);
    static QRgb gbr (QColor qc, int strength);
    static QRgb brg (QColor qc, int strength);
    static QRgb bgr (QColor qc, int strength);
    static QRgb greyFilmGrain (QColor qc, int strength);
    static QRgb colorFilmGrain (QColor qc, int strength);
    static void applyKernel(QProgressDialog *qpd, QImage *qi, KernelData kernalInfo);
    static Kernel getConeBlur(int radius);
    static Kernel getBoxBlur(int radius);

private:

    static int Burn(int color, int strength);
    static int Dodge(int color, int strength);
};

class Color {
public:
    static void brightnessAdjust(QImage *qi, double val, eType type);
    static void contrastAdjust(QImage *qi, double val);
    static void gammaAdjust(QImage *qi, double val);
    static void saturationAdjust(QImage *qi, double val, eType type);
    static void hueShift(QImage *qi, int val);
    static void ditherFloydSteinberg(QImage *qi, int bpp);
    static void ditherSierra(QImage *qi, int bpp);
    static void ditherSierraLite(QImage *qi, int bpp);
    static void ditherBayer(QImage *qi, int bpp, int matrixSize);
    static void ditherRandom(QImage *qi, int bpp);
    static void paletteReduction(QImage *qi, int bpp);
    static void colorTransfer(QImage *to, QImage from, tType type);
    static QImage Histogram(QImage *in, eType type);
    static void equalizeHistogramTo(QImage *qi, eType type);
    static void claheTo(QImage *qi, eType type, float clipLimit = 0.5, int divisonX = 0, int divisionY = 0);
    static void matchHistogram(QImage *qi, QImage toMatch, mType type);
    static vec4 rgb2lab(QColor qc);
    static vec4 getLabScaled(vec4 lab);
    static vec4 getLabDescaled(vec4 lab);
    static vec4 lab2rgb(vec4 lab);
    static QColor toQColor(vec4 rgb);
};

class ImgSupport {

public:

    ImgSupport();
    QImage zoomImg(QImage qi);
    double getZoom();
    void setZoom(double Zoom);
    void zoomIn();
    void zoomOut();
    QPoint getZoomCorrected(QPoint qp);
    QSize getZoomCorrected(QSize qs);
    static void flipVertical(QImage *qi);
    static void flipHorizontal(QImage *qi);
    static list <QImage *> resize(QSize reqSize);
    static KernelData loadKernel(string fileName);
    static void applyAlpha(QImage *qi, int yStart, int yEnd, unsigned int alpha);
    static QImage addLayers(QImage a, QImage b, eType type);
    static QImage subLayers(QImage a, QImage b, eType type);
    static QImage diffLayers(QImage a, QImage b, eType type);
    static QImage maxLayers(QImage a, QImage b, eType type);
    static QImage minLayers(QImage a, QImage b, eType type);
    static QImage avgLayers(QImage a, QImage b, eType type);
    static QImage remLayers(QImage a, QImage b, eType type);
    static QImage bitLayers(QImage a, QImage b, bType type);

private:

    static int getSize(double dim, double zoom);

    double zoom;
};


const function <QRgb (QColor, int)> filters[] = {Filtering::rgb, Filtering::greyscale, Filtering::polarize, Filtering::negative, Filtering::burn, Filtering::dodge, Filtering::enshadow, Filtering::redChannel, Filtering::greenChannel, Filtering::blueChannel, Filtering::yellowChannel, Filtering::cyanChannel, Filtering::magentaChannel, Filtering::hueChannel, Filtering::satChannel, Filtering::valChannel, Filtering::litChannel, Filtering::redPass, Filtering::greenPass, Filtering::bluePass, Filtering::yellowPass, Filtering::cyanPass, Filtering::magentaPass, Filtering::redFilter, Filtering::greenFilter, Filtering::blueFilter, Filtering::yellowFilter, Filtering::cyanFilter, Filtering::magentaFilter, Filtering::burnRed, Filtering::burnGreen, Filtering::burnBlue, Filtering::burnYellow, Filtering::burnCyan, Filtering::burnMagenta, Filtering::dodgeRed, Filtering::dodgeGreen, Filtering::dodgeBlue, Filtering::dodgeYellow, Filtering::dodgeCyan, Filtering::dodgeMagenta, Filtering::rbg, Filtering::grb, Filtering::gbr, Filtering::brg, Filtering::bgr, Filtering::colorFilmGrain, Filtering::greyFilmGrain};
}

#endif // GRAPHICS_H
