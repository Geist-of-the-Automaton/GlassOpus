#ifndef GAMMAADJ_H
#define GAMMAADJ_H

#include <QDialog>
#include <QPainter>

#include <stdfuncs.h>
#include <graphics.h>

using graphics::Color;

namespace Ui {
class GammaAdj;
}

class GammaAdj : public QDialog
{
        Q_OBJECT
public:
    GammaAdj(QWidget *parent = nullptr);
    ~GammaAdj();
    void setWork(QImage *toProcess);

private slots:
    void on_spinbox_valueChanged(double value);
    void on_slider_valueChanged(int value);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    void process();

    Ui::GammaAdj *ui;
    QImage *qi, processed, proDisp;
    double val;

};

#endif // GAMMAADJ_H
