#ifndef LAYERFUNC_H
#define LAYERFUNC_H

#include <QDialog>
#include <QPainter>
#include <QMouseEvent>

#include <stdfuncs.h>
#include <graphics.h>

using graphics::ImgSupport;

namespace Ui {
class LayerFunc;
}

class LayerFunc : public QDialog
{
        Q_OBJECT
public:
    LayerFunc(QWidget *parent = nullptr);
    ~LayerFunc();
    void setWork(int layers);
    vector <int> getChoice();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_op_currentIndexChanged(int value);

private:
    Ui::LayerFunc *ui;
    int numLayers;

};

#endif // LAYERFUNC_H
