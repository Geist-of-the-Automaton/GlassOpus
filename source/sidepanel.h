#ifndef SIDEPANEL_H
#define SIDEPANEL_H


#include <QScrollArea>
#include <QTimer>
#include <chrono>
#include <QEasingCurve>
#include <QPushButton>
#include <functional> // for std::function
#include <QApplication>
#include <QEvent>
#include <QResizeEvent>
#include <QScroller>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGraphicsEffect>
#include <QScrollBar>
#include <frametab.h>

using std::chrono::milliseconds;
using std::chrono::duration;
using std::chrono::system_clock;


enum SidePanelState {Opening, Opened, Closing, Closed};

class SidePanel : public QScrollArea
{
    Q_OBJECT

public:
    explicit SidePanel(QWidget *parent = nullptr);
    ~SidePanel() override;
    void openPanel();
    void closePanel();
    void setOffsets(int topOff, int botOff, int leftOff);
    void stopMoving();
    void remove(int delLayer);
    void add(Layer *layer);
    void setActive(int layer);
    int getActive();
    pair <int, int> getToSwap();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    void updateHandler(const SidePanelState state, QPushButton* btn);
    void updateHandlerRect(const qreal progress, const QRect& geom);
    void setState(const SidePanelState new_state);
    static QPointF lerp(const qreal t, const QPointF& a, const QPointF& b);
    static QRectF lerp(const qreal t, const QRectF& a, const QRectF& b);
    QRect rectOpened(const int width, const QRect& parent_rect);
    QRect rectClosed(const int width, const QRect& parent_rect);
    static double scale(const double& valueIn, const double& baseMin, const double& baseMax, const double& limitMin, const double& limitMax);
    QRect parentRect();

    SidePanelState state;
    milliseconds _duration {1000};
    system_clock::time_point timeStart;
    QEasingCurve curveOnOpen, curveOnClose;
    QTimer *timer;
    QPushButton *handler;
    qreal animProgress;
    QPoint offset, localPos;
    int ogY, panelSize, top, bottom, left;
    QTimer moveTimer;
    vector <FrameTab *> tabs;
    QVBoxLayout *mainVert;
    QWidget *proxy;
    QHBoxLayout *headers;
    QSpacerItem *spacer;
    int selectedLayer, activeLayer, toSwap;
    Qt::MouseButton lastBtn;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

signals:
    void stateChanged(SidePanelState state);
    void isMoving();
    void notMoving();
    void layerSet();
    void layerMoved();

protected:
    void resizeEvent(QResizeEvent* event) override;

public slots:
    void moveSlot();
};

#endif // SIDEPANEL_H
