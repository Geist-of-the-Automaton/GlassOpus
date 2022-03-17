#include "filterdialog.h"
#include "ui_filterdialog.h"

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    setWindowTitle("Filter Selection");
    connect(ui->qcb1, SIGNAL(currentIndexChanged(int)), this, SLOT(comboChange()));
}

FilterDialog::~FilterDialog() {
    delete ui;
}

void FilterDialog::setWork(QImage *toProcess) {
    qi = toProcess;
    processed = qi->scaled(qi->width() / 3, qi->height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    proDisp = processed.copy();
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
    process();
}

void FilterDialog::on_spinbox_valueChanged(double value) {
    val = static_cast<int>(255.0 * value);
    ui->slider->setValue(val);
    process();
}

void FilterDialog::on_slider_valueChanged(int value) {
    val = value;
    ui->spinbox->setValue(static_cast<float>(val / 255.0));
    process();
}

void FilterDialog::comboChange() {
    int index = ui->qcb1->currentIndex();
    val = graphics::filterPresets[index];
    ui->slider->setValue(val);
    ui->spinbox->setValue(static_cast<float>(val / 255.0));
    process();
}

void FilterDialog::process() {
    proDisp = processed.copy();
    int index = ui->qcb1->currentIndex();
    Filter f(val, graphics::filterNames[index]);
    f.setStrength(val);
    f.applyTo(&proDisp);
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void FilterDialog::on_buttonBox_accepted() {
    int index = ui->qcb1->currentIndex();
    Filter f(val, graphics::filterNames[index]);
    f.setStrength(val);
    f.applyTo(qi);
    done(1);
}

void FilterDialog::on_buttonBox_rejected() {
    done(0);
}
