#ifndef CONTRASTADJ_H
#define CONTRASTADJ_H

#include <QDialog>
#include <QPainter>

#include <stdfuncs.h>
#include <graphics.h>

using graphics::Color;

namespace Ui {
class ContrastAdj;
}

class ContrastAdj : public QDialog
{
        Q_OBJECT
public:
    ContrastAdj(QWidget *parent = nullptr);
    ~ContrastAdj();
    void setWork(QImage *toProcess);

private slots:
    void on_spinbox_valueChanged(double value);
    void on_slider_valueChanged(int value);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    void process();

    Ui::ContrastAdj *ui;
    QImage *qi, processed, proDisp;
    double val;

};

#endif // CONTRASTADJ_H
