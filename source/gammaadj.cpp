#include "gammaadj.h"
#include "ui_gammaadj.h"

GammaAdj::GammaAdj(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GammaAdj)
{
    this->setWindowFlag(Qt::WindowStaysOnTopHint, true);
    ui->setupUi(this);
    setWindowTitle("Contrast Adjustment");
}

GammaAdj::~GammaAdj() {
    delete ui;
}

void GammaAdj::setWork(QImage *toProcess) {
    qi = toProcess;
    processed = qi->scaled(qi->width() / 3, qi->height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    proDisp = processed.copy();
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void GammaAdj::on_spinbox_valueChanged(double value) {
    val = value;
    ui->slider->setValue(static_cast<int>(1000.0 * value));
    process();
}

void GammaAdj::on_slider_valueChanged(int value) {
    val = static_cast<double>(value) / 1000.0;
    ui->spinbox->setValue(val);
    process();
}

void GammaAdj::process() {
    proDisp = processed.copy();
    Color::gammaAdjust(&proDisp, val);
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void GammaAdj::on_buttonBox_accepted() {
    Color::gammaAdjust(qi, val);
    done(1);
}

void GammaAdj::on_buttonBox_rejected() {
    done(0);
}
