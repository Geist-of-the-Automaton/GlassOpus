#ifndef SPECIAL_H
#define SPECIAL_H
#include <QDialog>
#include <QImage>
#include <QPainter>

#include <stdfuncs.h>
#include <graphics.h>
using graphics::eType;
using graphics::Color;

namespace Ui {
    class Special;
}

class Special : public QDialog
{
    Q_OBJECT

public:
    Special(QWidget *parent = nullptr);
    ~Special();
    void setWork(QImage *toProcess);

private slots:
    void on_spinbox_valueChanged(int value);
    void on_slider_valueChanged(int value);
    void on_combo2_currentIndexChanged(int value);
    void process();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::Special *ui;
    QImage *qi, processed, proDisp;
    int radius, which;

};

#endif // SPECIAL_H

