#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <list>
#include <functional>
#include <unordered_map>
#include <fstream>
#include <thread>
#include <algorithm>
#include <QMainWindow>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMenuBar>
#include <QColorDialog>
#include <QInputDialog>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QErrorMessage>
#include <QFileSystemWatcher>
#include <QDir>
#include <QScrollBar>
#include <QProcess>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QScreen>
#include <QLabel>
#include <QDropEvent>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QProgressDialog>
#include <QTextEdit>
#include <QToolBar>
#include <QDockWidget>

#include <dataIOHandler.h>
#include <brushhandler.h>
#include <graphics.h>
#include <stdfuncs.h>
#include <resizewindow.h>
#include <screenrender.h>
#include <radialprofiler.h>
#include <viewscroller.h>
#include <algorithm>
#include <brushshape.h>
#include <patternprofiler.h>
#include <brightnessadj.h>
#include <contrastadj.h>
#include <gammaadj.h>
#include <saturationadj.h>
#include <hueshift.h>
#include <ditherdialog.h>
#include <transferdialog.h>
#include <symdialog.h>
#include <filterdialog.h>
#include <kerneldialog.h>
#include <histodialog.h>
#include <targettool.h>
#include <magicwanddialog.h>
#include <layerfunc.h>
#include <sidepanel.h>
#include <brushpanel.h>
#include <vectpanel.h>
#include <polypanel.h>
#include <rasterpanel.h>
#include <textpanel.h>
#include <special.h>
#include <Windows.h>

using std::string;
using std::to_string;
using std::list;
using std::to_string;
using std::function;
using std::unordered_map;
using std::fstream;
using std::ios;
using std::find;
using std::pair;
using Qt::MouseButton;
using Qt::NoButton;
using Qt::LeftButton;
using Qt::RightButton;
using Qt::Key_Shift;
using Qt::Key_Control;
using Qt::Key_Up;
using Qt::Key_Down;
using Qt::Key_Left;
using Qt::Key_Right;
using Qt::Key_Escape;
using Qt::Key_Delete;
using Qt::Key_Backspace;
using Qt::Key_X;
using Qt::Key_C;
using Qt::Key_V;
using Qt::Key_A;
using Qt::Key_Y;
using Qt::Key_Z;
using Qt::Key_D;
using Qt::Key_F7;
using Qt::Key_Alt;

using graphics::vectorFilters;
using graphics::filterNames;
using graphics::Filter;

const QString UI_FileName = "mainMenubar.txt";
const QString Doc_FileName = "Glass_Opus_Documentation.pdf";
const QString WinIco_FileName = "execIco.png";
const QString Logo_FileName = "Logo.png";
const QString UI_Loc = "/Menus/";
const QString Icon_Loc = UI_Loc + "Icons/";
const QString Doc_Loc = "/Documentation/";
const QString FetchLink = "https://github.com/SCCapstone/SnakyBusiness/raw/master";
const vector <string> acceptedImportImageFormats = {"bmp", "jpg", "jpeg", "png", "ppm", "xbm", "xpm", "gif", "pbm", "pgm"};
const vector <string> acceptedExportImageFormats = {"bmp", "jpg", "jpeg", "png", "ppm", "xbm", "xpm"};
static mutex scrollLock;

enum downloadAction {DownloadThenRestart, DownLoadThenOpen};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(string startPath, string projectFile, QWidget *parent = nullptr);
    ~MainWindow();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
    void hoverEvent(QHoverEvent *event);

public slots:
    void changeVectorFilter(string s);
    void changePolygonFilter(string s);
    void changeScreenFilter(string s);
    void changeBrushFilter(string s);
    void changeBrushMethod(string s);
    void changeBrushShape(string s);
    void doSomething(string btnPress);
    void downloadFinished();
    void downloadTimeout();
    void textChanged();

private:
    void log(string title, QObject *obj);
    bool createMenubar();
    void addItems(QMenu *menu, string items);
    void addAction(QMenu *menu, string s);
    void addItems(QToolBar *toolbar, string items);
    void refresh();
    void setShiftFlag(bool b);
    void setSamplePt(QPoint qp);
    void downloadItem(QString subfolder, QString fileName, downloadAction action, QString promptTitle, QString promptText);
    void createDocImgs();
    void setMode(EditMode emode);
    void changeOffsets();
    void setupDock(QDockWidget *qdw, viewScroller *vsw, QWidget *qw);

    Ui::MainWindow *ui;
    screenRender *sr;
    viewScroller *vs, *objScroller, *brushScroller, *vectScroller, *polyScroller, *rasterScroller, *textScroller;
    EditMode mode;
    bool shiftFlag, ctrlFlag, onePress, altFlag;
    MouseButton lastButton;
    DataIOHandler *ioh;
    brushHandler bh;
    resizeWindow *resizeCheck;
    RadialProfiler *radialProfiler;
    QColorDialog cd;
    QFileSystemWatcher downloadWatcher;
    QTimer downloadTimer;
    QErrorMessage *qme;
    unordered_map <string, QObject *> objFetch;
    list <QObject *> toDel;
    QString dSubfolder, dFileName;
    downloadAction dAction;
    bool takeFlag, magicFlag, layerMenuInteract;
    QString saveFileName;
    appMethod tempMethod = overwrite;
    QProgressDialog *progress;
    brushShape *brushProlfiler;
    patternProfiler *pp;
    bool lock;
    QLabel *histograms;
    SymDialog *symDialog;
    QPoint lastPos;
    QTextEdit qte;
    MagicWandDialog *mwd;
    QToolBar *toolbar;
    QMenuBar *menubar;
    SidePanel *LayerMenu;
    QDockWidget *objDetails, *brushDetails, *vectDetails, *polyDetails, *rasterDetails, *textDetails, *statBarTest;
    BrushPanel *brushPanel;
    VectPanel *vectPanel;
    PolyPanel *polyPanel;
    RasterPanel *rasterPanel;
    TextPanel *textPanel;
    QPoint leastPt, mostPt;


};

#endif // MAINWINDOW_H
