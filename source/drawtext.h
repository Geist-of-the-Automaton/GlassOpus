#ifndef DRAWTEXT_H
#define DRAWTEXT_H

#include <QPoint>
#include <QStaticText>
#include <QColor>
#include <QFontDatabase>
#include <QKeyEvent>
#include <QVector>

using Qt::Key;
using std::min;
using std::max;

const double rad2deg = 180.0 / 3.14159265359;
const int ptSize = 7;

class DrawText
{
public:
    DrawText();
    DrawText(const DrawText &txt);
    DrawText& operator = (const DrawText &txt);
    QPoint getShowPoint();
    QStaticText getText();
    QTransform getTransform();
    QFont getFont();
    QColor getColor();
    QPoint getCorner();
    void updateFont(QFont qf);
    void updateColor(QColor qc);
    bool updateText(Qt::Key key, bool shiftActive);
    void updateText(QStaticText qsText);
    void updateScale(float x, float y);
    void updatePos(QPoint loc);
    void updateRotate(float rot);
    QVector <float> getVals();

private:
    QColor color;
    float angle, xscale, yscale;
    QPoint pos;
    QStaticText sText;
    QTransform trans;
    QFont font;

};

#endif // DRAWTEXT_H
