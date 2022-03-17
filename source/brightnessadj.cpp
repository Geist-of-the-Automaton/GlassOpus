#include "brightnessadj.h"
#include "ui_brightnessadj.h"

BrightnessAdj::BrightnessAdj(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrightnessAdj)
{
    ui->setupUi(this);
    setWindowTitle("Brightness Adjustment");
    connect(ui->qcb, SIGNAL(currentIndexChanged(int)), this, SLOT(process()));
}

BrightnessAdj::~BrightnessAdj() {
    delete ui;
}

void BrightnessAdj::setWork(QImage *toProcess) {
    qi = toProcess;
    processed = qi->scaled(qi->width() / 3, qi->height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    proDisp = processed.copy();
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
    ui->slider->setValue(0);
    ui->spinbox->setValue(0.0);
    ui->qcb->setCurrentIndex(0);
}

void BrightnessAdj::on_spinbox_valueChanged(double value) {
    val = value;
    ui->slider->setValue(static_cast<int>(255.0 * value));
    process();
}

void BrightnessAdj::on_slider_valueChanged(int value) {
    val = static_cast<double>(value) / 255.0;
    ui->spinbox->setValue(val);
    process();
}

void BrightnessAdj::process() {
    proDisp = processed.copy();
    Color::brightnessAdjust(&proDisp, val, eType(ui->qcb->currentIndex()));
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void BrightnessAdj::on_buttonBox_accepted() {
    Color::brightnessAdjust(qi, val, eType(ui->qcb->currentIndex()));
    done(1);
}

void BrightnessAdj::on_buttonBox_rejected() {
    done(0);
}
