#include "transferdialog.h"
#include "ui_transfer.h"

TransferDialog::TransferDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransferDialog)
{
    ui->setupUi(this);
    setWindowTitle("Color Transfer");
    connect(ui->qcb, SIGNAL(currentIndexChanged(int)), this, SLOT(process()));
}

TransferDialog::~TransferDialog() {
    delete ui;
}

void TransferDialog::setWork(QImage *toProcess, QImage from) {
    qi = toProcess;
    clab = toProcess->copy();
    hlab = toProcess->copy();
    rgb = toProcess->copy();
    Color::colorTransfer(&clab, from, tType::CIELAB);
    Color::colorTransfer(&hlab, from, tType::HunterLAB);
    Color::colorTransfer(&rgb, from, tType::sRGB);
    proDisp = clab.scaled(clab.width() / 3, clab.height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
    ui->qcb->setCurrentIndex(1);
}

void TransferDialog::process() {
    switch (ui->qcb->currentIndex()) {
    case 0:
        proDisp = rgb;
        break;
    case 1:
        proDisp = hlab;
        break;
    case 2:
        proDisp = clab;
        break;
    }
    proDisp = proDisp.scaled(clab.width() / 3, clab.height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void TransferDialog::on_buttonBox_accepted() {
    switch (ui->qcb->currentIndex()) {
    case 0:
        proDisp = rgb;
        break;
    case 1:
        proDisp = hlab;
        break;
    case 2:
        proDisp = clab;
        break;
    }
    *qi = proDisp;
    done(1);
}

void TransferDialog::on_buttonBox_rejected() {
    done(0);
}
