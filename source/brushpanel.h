#ifndef BRUSHPANEL_H
#define BRUSHPANEL_H

#include <QObject>
#include <QWidget>
#include <brushhandler.h>
#include <patternprofiler.h>
#include <brushshape.h>
#include <radialprofiler.h>

namespace Ui {
class BrushPanel;
}

class BrushPanel : public QWidget
{
    Q_OBJECT
public:
    explicit BrushPanel(QWidget *parent = nullptr);
    void setWork(brushHandler *bhandler, patternProfiler *pprofiler, brushShape *bshape, RadialProfiler *rprofiler);
    void mouseMoveEvent(QMouseEvent *event);
    void updateFromBH();

signals:
    void updateSizeShape();
    void symExec();


public slots:

    void on_methodCB_currentIndexChanged(int index);
    void on_shapeCB_currentIndexChanged(int index);
    void on_radiusSB_valueChanged(int value);
    void on_radiusS_valueChanged(int value);
    void on_strengthSB_valueChanged(int value);
    void on_strengthS_valueChanged(int value);
    void on_colorSB_valueChanged(int value);
    void on_colorS_valueChanged(int value);
    void on_filterCB_currentIndexChanged(int index);
    void on_spraySB_valueChanged(int value);
    void on_sprayS_valueChanged(int value);
    void on_patternRB_clicked();
    void on_patternPB_clicked();
    void on_symPB_clicked();
    void on_shapePB_clicked();
    void on_radialPB_clicked();

private:
    void changeColor();

    Ui::BrushPanel *ui;
    brushHandler *bh;
    QPoint a;
    patternProfiler *pp;
    RadialProfiler *rp;
    brushShape *bs;

};

#endif // BRUSHPANEL_H
