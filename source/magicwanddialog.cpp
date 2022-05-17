#include "magicwanddialog.h"
#include "ui_magicwanddialog.h"


MagicWandDialog::MagicWandDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MagicWandDialog)
{
    ui->setupUi(this);
    setWindowTitle("Magic Wand Settings");
    ui->threshS->setValue(50);
    ui->threshSB->setValue(50);
    ui->hueS->setValue(23);
    ui->hueSB->setValue(23);
    ui->satS->setValue(50);
    ui->satSB->setValue(50);
    ui->lumS->setValue(50);
    ui->lumSB->setValue(50);
}

MagicWandDialog::~MagicWandDialog() {
    delete ui;
}

void MagicWandDialog::on_threshSB_valueChanged(int value) {
    ui->threshS->setValue(value);
    double h = 45.0 * static_cast<double>(value) / 100.0;
    double s = 100.0 * static_cast<double>(value) / 100.0;
    double l =100.0 * static_cast<double>(value) / 100.0;
    ui->hueS->setValue(h);
    ui->hueSB->setValue(h);
    ui->satS->setValue(s);
    ui->satSB->setValue(s);
    ui->lumS->setValue(l);
    ui->lumSB->setValue(l);
}

void MagicWandDialog::on_threshS_valueChanged(int value) {
    ui->threshSB->setValue(value);
    double h = 45.0 * static_cast<double>(value) / 100.0;
    double s = 100.0 * static_cast<double>(value) / 100.0;
    double l = 100.0 * static_cast<double>(value) / 100.0;
    ui->hueS->setValue(h);
    ui->hueSB->setValue(h);
    ui->satS->setValue(s);
    ui->satSB->setValue(s);
    ui->lumS->setValue(l);
    ui->lumSB->setValue(l);
    hue = h;
    sat = s;
    lum = l;
}

void MagicWandDialog::on_hueSB_valueChanged(int value) {
    hue = value;
    ui->hueS->setValue(value);
}

void MagicWandDialog::on_hueS_valueChanged(int value) {
    hue = value;
    ui->hueSB->setValue(value);
}

void MagicWandDialog::on_satSB_valueChanged(int value) {
    sat = value;
    ui->satS->setValue(value);
}

void MagicWandDialog::on_satS_valueChanged(int value) {
    sat = value;
    ui->satSB->setValue(value);
}

void MagicWandDialog::on_lumSB_valueChanged(int value) {
    lum = value;
    ui->lumS->setValue(value);
}

void MagicWandDialog::on_lumS_valueChanged(int value) {
    lum = value;
    ui->lumSB->setValue(value);
}

vec4 MagicWandDialog::getVals() {
    return vec4(hue, sat, lum, ui->threshS->value());
}

void MagicWandDialog::setVals(vec4 vals) {
    ui->hueS->setValue(vals._X);
    ui->hueSB->setValue(vals._X);
    ui->satS->setValue(vals._Y);
    ui->satSB->setValue(vals._Y);
    ui->lumS->setValue(vals._Z);
    ui->lumSB->setValue(vals._Z);
    ui->threshS->setValue(vals._W);
    ui->threshSB->setValue(vals._W);
    hue = vals._X;
    sat = vals._Y;
    lum = vals._Z;
}

void MagicWandDialog::on_buttonBox_accepted() {
    done(1);
}

void MagicWandDialog::on_buttonBox_rejected() {
    done(0);
}


