#ifndef VECTPANEL_H
#define VECTPANEL_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include <splinevector.h>

enum vType {vSym = 0, vMode = 1, vFilter = 2, vStrength = 3, vColor1 = 4, vColor2 = 5, vTaper1 = 6, vTaper2 = 7, vGap = 8, vBand = 9, vWidth = 10, vStyle = 11};
using std::pair;

namespace Ui {
class VectPanel;
}

class VectPanel : public QWidget
{
    Q_OBJECT
public:
    explicit VectPanel(QWidget *parent = nullptr);
    void updatePanel(pair <vType, unsigned int> dets);
    void updatePanel(SplineVector sv);
    void resetPanel();
    pair <vType, unsigned int> getData();
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void on_modeCB_currentIndexChanged(int index);
    void on_filterCB_currentIndexChanged(int index);
    void on_strengthSB_valueChanged(int value);
    void on_strengthS_valueChanged(int value);
    void on_colorSB1_valueChanged(int value);
    void on_colorS1_valueChanged(int value);
    void on_colorSB2_valueChanged(int value);
    void on_colorS2_valueChanged(int value);
    void on_widthS_valueChanged(int value);
    void on_widthSB_valueChanged(int value);
    void on_taperSB1_valueChanged(int value);
    void on_taperS1_valueChanged(int value);
    void on_taperSB2_valueChanged(int value);
    void on_taperS2_valueChanged(int value);
    void on_typeCB_currentIndexChanged(int index);
    void on_gapSB_valueChanged(int value);
    void on_gapS_valueChanged(int value);
    void on_bandSB_valueChanged(int value);
    void on_bandS_valueChanged(int value);
    void on_symPB_clicked();
    void on_cSwapPB_clicked();
    void on_tSwapPB_clicked();

signals:
    void actionGiven();

private:
    void changeColor(int lb);

    Ui::VectPanel *ui;

    pair <vType, unsigned int> ret;
    QPoint a, b;

};

#endif // VECTPANEL_H
