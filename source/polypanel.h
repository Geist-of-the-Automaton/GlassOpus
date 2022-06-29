#ifndef POLYPANEL_H
#define POLYPANEL_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QMouseEvent>
#include <polygon.h>


enum pType {pMode = 1, pFilter = 2, pStrength = 3, pColor1 = 4, pColor2 = 5, pBand = 6, pReduce = 7, pTriView = 8};
using std::pair;

namespace Ui {
class PolyPanel;
}

class PolyPanel : public QWidget
{
    Q_OBJECT
public:
    explicit PolyPanel(QWidget *parent = nullptr);
    void updatePanel(pair <pType, unsigned int> dets);
    void updatePanel(Polygon gon);
    void resetPanel();
    pair <pType, unsigned int> getData();
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
    void on_bandSB_valueChanged(int value);
    void on_bandS_valueChanged(int value);
    void on_tViewRB_clicked();
    void on_cSwapPB_clicked();
    void on_rPtPB_clicked();

signals:
    void actionGiven();

private:
    void changeColor(int lb);

    Ui::PolyPanel *ui;

    pair <pType, unsigned int> ret;
    QPoint a, b;

};

#endif // POLYPANEL_H
