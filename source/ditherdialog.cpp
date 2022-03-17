#include "ditherdialog.h"
#include "ui_ditherdialog.h"

DitherDialog::DitherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DitherDialog)
{
    ui->setupUi(this);
    setWindowTitle("Dither Selection");
    connect(ui->qcb1, SIGNAL(currentIndexChanged(int)), this, SLOT(process()));
    connect(ui->qcb2, SIGNAL(currentIndexChanged(int)), this, SLOT(process()));
    connect(ui->qcb3, SIGNAL(currentIndexChanged(int)), this, SLOT(process()));
}

DitherDialog::~DitherDialog() {
    delete ui;
}

void DitherDialog::setWork(QImage *toProcess) {
    qi = toProcess;
    processed = qi->scaled(qi->width() / 3, qi->height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    proDisp = processed.copy();
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
    ui->qcb1->setCurrentIndex(0);
    ui->qcb2->setCurrentIndex(6);
    ui->qcb3->setEnabled(true);
    ui->qcb3->setCurrentIndex(4);
    process();
}

void DitherDialog::process() {
    if (ui->qcb1->currentIndex() == 0)
        ui->qcb3->setEnabled(true);
    else
        ui->qcb3->setDisabled(true);
    proDisp = processed.copy();
    int bpps[] = {1, 2, 3, 4, 5, 6, 7};
    int matSizes[] = {2, 4, 8, 16, 32};
    int bpp = bpps[ui->qcb2->currentIndex()];
    int matSize = matSizes[ui->qcb3->currentIndex()];
    switch (ui->qcb1->currentIndex()) {
        case 0:
            Color::ditherBayer(&proDisp, bpp, matSize);
        break;
    case 1:
        Color::ditherFloydSteinberg(&proDisp, bpp);
        break;
    case 2:
        Color::ditherSierra(&proDisp, bpp);
        break;
    case 3:
        Color::ditherSierraLite(&proDisp, bpp);
        break;
    case 4:
        Color::ditherRandom(&proDisp, bpp);
        break;
    case 5:
        Color::paletteReduction(&proDisp, bpp);
        break;
    }
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void DitherDialog::on_buttonBox_accepted() {
    int bpps[] = {1, 2, 3, 4, 5, 6, 7};
    int matSizes[] = {2, 4, 8, 16, 32};
    int bpp = bpps[ui->qcb2->currentIndex()];
    int matSize = matSizes[ui->qcb3->currentIndex()];
    switch (ui->qcb1->currentIndex()) {
        case 0:
            Color::ditherBayer(qi, bpp, matSize);
        break;
    case 1:
        Color::ditherFloydSteinberg(qi, bpp);
        break;
    case 2:
        Color::ditherSierra(qi, bpp);
        break;
    case 3:
        Color::ditherSierraLite(qi, bpp);
        break;
    case 4:
        Color::ditherRandom(qi, bpp);
        break;
    case 5:
        Color::paletteReduction(qi, bpp);
        break;
    }
    done(1);
}

void DitherDialog::on_buttonBox_rejected() {
    done(0);
}
