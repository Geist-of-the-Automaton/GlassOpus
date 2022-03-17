#ifndef DITHERDIALOG_H
#define DITHERDIALOG_H

#include <QDialog>
#include <QPainter>

#include <graphics.h>

using graphics::Color;

namespace Ui {
class DitherDialog;
}

class DitherDialog : public QDialog
{
        Q_OBJECT
public:
    DitherDialog(QWidget *parent = nullptr);
    ~DitherDialog();
    void setWork(QImage *toProcess);

private slots:
    void process();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::DitherDialog *ui;
    QImage *qi, processed, proDisp;

};

#endif // DITHERDIALOG_H
