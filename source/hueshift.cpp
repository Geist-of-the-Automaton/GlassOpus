#include "hueshift.h"
#include "ui_hueshift.h"

HueShift::HueShift(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HueShift)
{
    this->setWindowFlag(Qt::WindowStaysOnTopHint, true);
    ui->setupUi(this);
    setWindowTitle("Contrast Adjustment");
}

HueShift::~HueShift() {
    delete ui;
}

void HueShift::setWork(QImage *toProcess) {
    qi = toProcess;
    processed = qi->scaled(qi->width() / 3, qi->height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    proDisp = processed.copy();
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void HueShift::on_spinbox_valueChanged(int value) {
    val = value;
    ui->slider->setValue(value);
    process();
}

void HueShift::on_slider_valueChanged(int value) {
    val = value;
    ui->spinbox->setValue(val);
    process();
}

void HueShift::process() {
    proDisp = processed.copy();
    Color::hueShift(&proDisp, val);
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void HueShift::on_buttonBox_accepted() {
    Color::hueShift(qi, val);
    done(1);
}

void HueShift::on_buttonBox_rejected() {
    done(0);
}
