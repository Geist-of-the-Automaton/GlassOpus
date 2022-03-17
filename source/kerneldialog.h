#ifndef KERNELDIALOG_H
#define KERNELDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QProgressDialog>
#include <QDir>

#include <graphics.h>
using graphics::ImgSupport;
using graphics::Filtering;

const QString Kernel_Loc = "/Kernels/";

namespace Ui {
class KernelDialog;
}

class KernelDialog : public QDialog
{
        Q_OBJECT
public:
    KernelDialog(QWidget *parent = nullptr);
    ~KernelDialog();
    void setWork(QImage *toProcess);

private slots:
    void comboChange();
    void on_spinbox_valueChanged(int value);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    void process();
    Ui::KernelDialog *ui;
    QImage *qi, processed, proDisp;
    int val, lock;
    KernelData kd;

};

#endif // KERNELDIALOG_H
