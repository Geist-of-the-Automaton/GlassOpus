#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QPainter>

#include <graphics.h>

using graphics::Filter;
using graphics::Filtering;

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
        Q_OBJECT
public:
    FilterDialog(QWidget *parent = nullptr);
    ~FilterDialog();
    void setWork(QImage *toProcess);

private slots:
    void comboChange();
    void on_spinbox_valueChanged(double value);
    void on_slider_valueChanged(int value);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    void process();
    Ui::FilterDialog *ui;
    QImage *qi, processed, proDisp;
    int val;

};

#endif // FILTERDIALOG_H
