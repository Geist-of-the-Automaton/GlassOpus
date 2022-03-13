#include "contrastadj.h"
#include "ui_contrastadj.h"

ContrastAdj::ContrastAdj(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContrastAdj)
{
    this->setWindowFlag(Qt::WindowStaysOnTopHint, true);
    ui->setupUi(this);
    setWindowTitle("Contrast Adjustment");
}

ContrastAdj::~ContrastAdj() {
    delete ui;
}

void ContrastAdj::setWork(QImage *toProcess) {
    qi = toProcess;
    processed = qi->scaled(qi->width() / 3, qi->height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    proDisp = processed.copy();
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void ContrastAdj::on_spinbox_valueChanged(double value) {
    val = value;
    ui->slider->setValue(static_cast<int>(1000.0 * value));
    process();
}

void ContrastAdj::on_slider_valueChanged(int value) {
    val = static_cast<double>(value) / 1000.0;
    ui->spinbox->setValue(val);
    process();
}

void ContrastAdj::process() {
    proDisp = processed.copy();
    Color::contrastAdjust(&proDisp, val);
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void ContrastAdj::on_buttonBox_accepted() {
    Color::contrastAdjust(qi, val);
    done(1);
}

void ContrastAdj::on_buttonBox_rejected() {
    done(0);
}
