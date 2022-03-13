#ifndef SATURATIONADJ_H
#define SATURATIONADJ_H

#include <QDialog>
#include <QPainter>

#include <stdfuncs.h>
#include <graphics.h>

using graphics::eType;
using graphics::Color;

namespace Ui {
class SaturationAdj;
}

class SaturationAdj : public QDialog
{
        Q_OBJECT
public:
    SaturationAdj(QWidget *parent = nullptr);
    ~SaturationAdj();
    void setWork(QImage *toProcess);

private slots:
    void on_spinbox_valueChanged(double value);
    void on_slider_valueChanged(int value);
    void process();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::SaturationAdj *ui;
    QImage *qi, processed, proDisp;
    double val;

};

#endif // SATURATIONADJ_H
