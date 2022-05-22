#ifndef DATAIO_H
#define DATAIO_H

#include <thread>
#include <vector>
#include <list>
#include <atomic>
#include <mutex>
#include <fstream>
#include <QImageReader>
#include <QFileDialog>
#include <QProgressDialog>
#include <QImage>
#include <QPainter>
#include <layer.h>
#include <graphics.h>
#include <base85.h>
#include <string>

using std::thread;
using std::vector;
using std::list;
using std::pair;
using std::iter_swap;
using std::swap;
using std::atomic_int;
using std::mutex;
using std::ofstream;
using std::ifstream;
using std::getline;
using graphics::Filter;

enum scaleType {dontScale, bestFit, aspectRatio, toWidth, toHeight};

struct RGB {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

static mutex locker;
static atomic_int progressMarker;
const unsigned char maxLayer = 64;

class DataIOHandler : public QWidget {

    Q_OBJECT

public:

    DataIOHandler(QProgressDialog *progress);
    ~DataIOHandler();
    static void renderFrame(QProgressDialog *fqpd, QImage *ret, vector <Layer *> layers);
    void setDims(QSize size);
    QSize getdims();
    void scale(scaleType option);
    bool importImage(QString fileName);
    void exportImage(QString fileName);
    void compileLayer();
    void compileFrame();
    void setActiveLayer(int i, EditMode mode);
    int getNumLayers();
    int getActiveLayer();
    bool wasUpdated();
    void copyText();
    void cutText();
    void pasteText();
    void deleteText();
    void copyVectors();
    void cutVectors();
    void deleteVectors();
    void pasteVectors();
    void copyPolygons();
    void cutPolygons();
    void deletePolygons();
    void pastePolygons();
    void copyRaster();
    void cutRaster();
    void deleteRaster();
    void pasteRaster();
    void addLayer();
    void copyLayer();
    void pasteLayer();
    void deleteLayer();
    void moveBackward();
    void moveForward();
    void moveToBack();
    void moveToFront();
    void swapLayers(int a, int b);
    Layer *getWorkingLayer();
    QImage getBackground();
    QImage getForeground();
    void save(QString projectName);
    int load(QString projectName);
    void clearFrame();
    vector <Layer *> backup();
    void layerFunc(vector <int> choices);

    vector<int> findPoints(QImage *qi);
    void vectorCheck(SplineVector sv);

    void setSym(QPoint qp, int div, int ofEvery, int skip);

signals:
    void hasUpdate();

private:

    QSize dims;
    vector <Layer *> frame;
    list <SplineVector> vectorCopySlot;
    list <Polygon> polygonCopySlot;
    list <DrawText> textCopySlot;
    Layer layerCopySlot;
    QString file;
    unsigned char activeLayer;
    QImage importImg, rasterCopySlot;
    bool updated;
    double angleCopySlot;
    pair <QPoint, QPoint> boundCopySlot;
    QProgressDialog *progress;
};

#endif // DATAIO_H
