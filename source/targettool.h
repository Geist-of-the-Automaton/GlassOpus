#ifndef TARGETTOOL_H
#define TARGETTOOL_H


#include <QDialog>
#include <QPainter>
#include <QMouseEvent>

#include <stdfuncs.h>
#include <graphics.h>

using graphics::Color;

namespace Ui {
class TargetTool;
}

class TargetTool : public QDialog
{
        Q_OBJECT
public:
    TargetTool(QWidget *parent = nullptr);
    ~TargetTool();
    void setWork(QImage *toProcess);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_hueTarSB_valueChanged(int value);
    void on_hueTarS_valueChanged(int value);
    void on_hueBndSB_valueChanged(int value);
    void on_hueBndS_valueChanged(int value);
    void on_satTarSB_valueChanged(int value);
    void on_satTarS_valueChanged(int value);
    void on_satBndSB_valueChanged(int value);
    void on_satBndS_valueChanged(int value);
    void on_litTarSB_valueChanged(int value);
    void on_litTarS_valueChanged(int value);
    void on_litBndSB_valueChanged(int value);
    void on_litBndS_valueChanged(int value);
    void on_opSB_valueChanged(int value);
    void on_opS_valueChanged(int value);
    void on_op_currentIndexChanged(int value);
    void on_opType_currentIndexChanged(int value);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    void processMouse(QPoint qp);
    void process();
    void preprocess();

    Ui::TargetTool *ui;
    QImage *qi, processed, processed2, proDisp;
    int hueTar, hueBnd, satTar, satBnd, litTar, litBnd, opVal;

};

#endif // TARGETTOOL_H
