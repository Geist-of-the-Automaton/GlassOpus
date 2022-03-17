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
    lab = toProcess->copy();
    rgb = toProcess->copy();
    Color::colorTransfer(&lab, from, tType::LAB);
    Color::colorTransfer(&rgb, from, tType::sRGB);
    proDisp = lab.scaled(lab.width() / 3, lab.height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
    ui->qcb->setCurrentIndex(1);
}

void TransferDialog::process() {
    proDisp = (ui->qcb->currentIndex() == 0 ? rgb : lab).scaled(lab.width() / 3, lab.height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void TransferDialog::on_buttonBox_accepted() {
    *qi = (ui->qcb->currentIndex() == 0 ? rgb : lab);
    done(1);
}

void TransferDialog::on_buttonBox_rejected() {
    done(0);
}
