#include "saturationadj.h"
#include "ui_saturationadj.h"

SaturationAdj::SaturationAdj(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaturationAdj)
{
    ui->setupUi(this);
    setWindowTitle("Saturation Adjustment");
    connect(ui->qcb, SIGNAL(currentIndexChanged(int)), this, SLOT(process()));
}

SaturationAdj::~SaturationAdj() {
    delete ui;
}

void SaturationAdj::setWork(QImage *toProcess) {
    qi = toProcess;
    processed = qi->scaled(qi->width() / 3, qi->height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    proDisp = processed.copy();
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
    ui->slider->setValue(0);
    ui->spinbox->setValue(0.0);
    ui->qcb->setCurrentIndex(0);
}

void SaturationAdj::on_spinbox_valueChanged(double value) {
    val = value;
    ui->slider->setValue(static_cast<int>(255.0 * value));
    process();
}

void SaturationAdj::on_slider_valueChanged(int value) {
    val = static_cast<double>(value) / 255.0;
    ui->spinbox->setValue(val);
    process();
}

void SaturationAdj::process() {
    proDisp = processed.copy();
    Color::saturationAdjust(&proDisp, val, eType(ui->qcb->currentIndex() + 1));
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void SaturationAdj::on_buttonBox_accepted() {
    Color::saturationAdjust(qi, val, eType(ui->qcb->currentIndex() + 1));
    done(1);
}

void SaturationAdj::on_buttonBox_rejected() {
    done(0);
}
