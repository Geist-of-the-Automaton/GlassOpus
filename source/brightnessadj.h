#ifndef BRIGHTNESSADJ_H
#define BRIGHTNESSADJ_H

#include <QDialog>
#include <QPainter>

#include <stdfuncs.h>
#include <graphics.h>
using graphics::eType;
using graphics::Color;

namespace Ui {
class BrightnessAdj;
}

class BrightnessAdj : public QDialog
{
        Q_OBJECT
public:
    BrightnessAdj(QWidget *parent = nullptr);
    ~BrightnessAdj();
    void setWork(QImage *toProcess);

private slots:
    void on_spinbox_valueChanged(double value);
    void on_slider_valueChanged(int value);
    void process();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::BrightnessAdj *ui;
    QImage *qi, processed, proDisp;
    double val;

};

#endif // BRIGHTNESSADJ_H


