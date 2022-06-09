#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H


#include <QDialog>
#include <QPainter>

#include <stdfuncs.h>
#include <graphics.h>

using graphics::Color;
using graphics::tType;

namespace Ui {
class TransferDialog;
}

class TransferDialog : public QDialog
{
        Q_OBJECT
public:
    TransferDialog(QWidget *parent = nullptr);
    ~TransferDialog();
    void setWork(QImage *toProcess, QImage from);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void process();

private:

    Ui::TransferDialog *ui;
    QImage *qi, rgb, clab, hlab, proDisp;

};


#endif // TRANSFERDIALOG_H
