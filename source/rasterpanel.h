#ifndef RASTERPANEL_H
#define RASTERPANEL_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QMouseEvent>

enum rType {rColor = 0, rTake = 1, rTransFill = 2, rWand = 3, rWandSet = 4, rFlipH = 5, rFlipV = 6, rBri = 7, rCon = 8, rGam = 9, rSat = 10, rHue = 11, rTar = 12, rFilt = 13, rConv = 14, rDith = 15, rTran = 16};
using std::pair;

namespace Ui {
class RasterPanel;
}

class RasterPanel : public QWidget
{
    Q_OBJECT
public:
    explicit RasterPanel(QWidget *parent = nullptr);
    void updatePanel(QColor color);
    void resetPanel();
    pair <rType, unsigned int> getData();
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void on_colorSB_valueChanged(int value);
    void on_colorS_valueChanged(int value);
    void on_takeColorPB_clicked();
    void on_transFillPB_clicked();
    void on_mWandPB_clicked();
    void on_mWandSetPB_clicked();
    void on_hFlipPB_clicked();
    void on_vFlipPB_clicked();
    void on_briPB_clicked();
    void on_conPB_clicked();
    void on_gamPB_clicked();
    void on_satPB_clicked();
    void on_huePB_clicked();
    void on_tarPB_clicked();
    void on_filtPB_clicked();
    void on_convPB_clicked();
    void on_ditherPB_clicked();
    void on_cTransPB_clicked();

signals:
    void actionGiven();

private:
    void changeColor();

    Ui::RasterPanel *ui;

    pair <rType, unsigned int> ret;
    QPoint a;

};

#endif // RASTERPANEL_H
