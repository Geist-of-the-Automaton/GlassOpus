#ifndef HUESHIFT_H
#define HUESHIFT_H

#include <QDialog>
#include <QPainter>

#include <stdfuncs.h>
#include <graphics.h>

using graphics::Color;

namespace Ui {
class HueShift;
}

class HueShift : public QDialog
{
        Q_OBJECT
public:
    HueShift(QWidget *parent = nullptr);
    ~HueShift();
    void setWork(QImage *toProcess);

private slots:
    void on_spinbox_valueChanged(int value);
    void on_slider_valueChanged(int value);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    void process();

    Ui::HueShift *ui;
    QImage *qi, processed, proDisp;
    int val;

};

#endif // HUESHIFT_H
