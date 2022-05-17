#ifndef MAGICWANDDIALOG_H
#define MAGICWANDDIALOG_H


#include <QDialog>

#include <vec_mat_maths.h>

namespace Ui {
class MagicWandDialog;
}

class MagicWandDialog : public QDialog
{
        Q_OBJECT
public:
    MagicWandDialog(QWidget *parent = nullptr);
    ~MagicWandDialog();
    vec4 getVals();
    void setVals(vec4 vals);

private slots:

    void on_threshSB_valueChanged(int value);
    void on_threshS_valueChanged(int value);
    void on_hueSB_valueChanged(int value);
    void on_hueS_valueChanged(int value);
    void on_satSB_valueChanged(int value);
    void on_satS_valueChanged(int value);
    void on_lumSB_valueChanged(int value);
    void on_lumS_valueChanged(int value);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:

    Ui::MagicWandDialog *ui;
    double hue, sat, lum;

};

#endif // MAGICWANDDIALOG_H
