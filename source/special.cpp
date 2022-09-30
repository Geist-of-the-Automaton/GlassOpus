#include "special.h"
#include "ui_specialDialog.h"

Special::Special(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Special)
{
    which = 0;
    radius = 2;
    ui->setupUi(this);
    connect(ui->qcb, SIGNAL(currentIndexChanged(int)), this, SLOT(process()));
}

Special::~Special() {
    delete ui;
}

void Special::setWork(QImage *toProcess) {
    qi = toProcess;
    processed = qi->scaled(qi->width() / 3, qi->height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    proDisp = processed.copy();
    process();
}

void Special::on_spinbox_valueChanged(int value) {
    ui->slider->setValue(value);
    radius = value;
    process();
}

void Special::on_slider_valueChanged(int value) {
    ui->spinbox->setValue(value);
    radius = value;
    process();
}

void Special::on_combo2_currentIndexChanged(int value) {
    which = value;
    ui->slider->setRange(graphics::specMins[which], graphics::specMaxs[which]);
    ui->slider->setValue(graphics::specMins[which]);
    ui->qcb->setEnabled(graphics::needsEType[which]);
}

void Special::process() {
    proDisp = processed.copy();
    graphics::specFilters[which](&proDisp, radius, eType(ui->qcb->currentIndex()));
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void Special::on_buttonBox_accepted() {
    graphics::specFilters[which](qi, radius, eType(ui->qcb->currentIndex()));
    done(1);
}

void Special::on_buttonBox_rejected() {
    done(0);
}
