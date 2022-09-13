#include "histodialog.h"
#include "ui_histodialog.h"

HistoDialog::HistoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoDialog)
{
    ui->setupUi(this);
    setWindowTitle("Histograms");
    connect(ui->qcb1, SIGNAL(currentIndexChanged(int)), this, SLOT(process()));
    connect(ui->qcb2, SIGNAL(currentIndexChanged(int)), this, SLOT(process()));
    connect(ui->qcb3, SIGNAL(currentIndexChanged(int)), this, SLOT(matchChanged()));
}

HistoDialog::~HistoDialog() {
    delete ui;
}

void HistoDialog::setWork(QImage *toProcess) {
    qi = toProcess;
    //processed = qi->scaled(qi->width() / 3, qi->height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //proDisp = processed.copy();
    ui->qcb3->setEnabled(false);
    for (int i = 0; i < 4; ++i)
        histos[i] = graphics::Color::Histogram(qi, graphics::eType(i));
    ui->label->setPixmap(QPixmap::fromImage(histos[0]));
    ui->clipS->setValue(150);
    ui->clipSB->setValue(1.5);
}

void HistoDialog::matchChanged() {
    int index = ui->qcb3->currentIndex();
    processed = qi->copy();
    graphics::Color::matchHistogram(&processed, toMatch, graphics::mType(index));
    processed = processed.scaled(qi->width() / 3, qi->height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(QPixmap::fromImage(processed));
}

void HistoDialog::process() {
    int index1 = ui->qcb1->currentIndex();
    int index2 = ui->qcb2->currentIndex();
    if (index2 != 1) {
        ui->qcb3->setCurrentIndex(0);
        ui->qcb3->setEnabled(false);
    }
    else
        ui->qcb3->setEnabled(true);
    if (index2 != 3) {
        ui->clipS->setValue(15);
        ui->clipSB->setValue(1.5);
        ui->xdivS->setValue(1);
        ui->xdivSB->setValue(1);
        ui->ydivS->setValue(1);
        ui->ydivSB->setValue(1);
        ui->clipS->setEnabled(false);
        ui->clipSB->setEnabled(false);
        ui->xdivS->setEnabled(false);
        ui->xdivSB->setEnabled(false);
        ui->ydivS->setEnabled(false);
        ui->ydivSB->setEnabled(false);
        ui->note->setText("");
    }
    else {
        ui->clipS->setEnabled(true);
        ui->clipSB->setEnabled(true);
        ui->xdivS->setEnabled(true);
        ui->xdivSB->setEnabled(true);
        ui->ydivS->setEnabled(true);
        ui->ydivSB->setEnabled(true);
        ui->note->setText("X or Y CLAHE divisions of 1 will use auto-divisions in that axis.");
    }
    if (index2 == 0)
        ui->label->setPixmap(QPixmap::fromImage(histos[index1]));
    else {
        processed = qi->copy();
        if (index2 == 1) {
            vector <string> acceptedImportImageFormats = {"bmp", "jpg", "jpeg", "png", "ppm", "xbm", "xpm", "gif", "pbm", "pgm"};
            string formats = "";
            for (string s : acceptedImportImageFormats)
                formats += " *." + s;
            formats = "Image Files (" + formats.substr(1) + ")";
            QString fileName = QFileDialog::getOpenFileName(this, tr("Import"), "/", tr(formats.c_str()));
            if (fileName == "") {
                ui->qcb2->setCurrentIndex(0);
                return;
            }
            string fn = fileName.toStdString();
            size_t index = fn.find_last_of('.');
            string fileType = fn.substr(index + 1);
            if (std::find(acceptedImportImageFormats.begin(), acceptedImportImageFormats.end(), fileType) != acceptedImportImageFormats.end())
                toMatch = QImage(fileName);
            graphics::Color::matchHistogram(&processed, toMatch, graphics::mType(ui->qcb3->currentIndex()));
        }
        else if (index2 == 2)
            graphics::Color::equalizeHistogramTo(&processed, graphics::eType(index1));
        else if (index2 == 3)
            graphics::Color::claheTo(&processed, graphics::eType(index1), ui->clipSB->value(), ui->xdivSB->value(), ui->ydivSB->value());
        processed = processed.scaled(qi->width() / 3, qi->height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->label->setPixmap(QPixmap::fromImage(processed));
    }
}

void HistoDialog::on_clipSB_valueChanged(double value) {
    ui->clipS->setValue(static_cast<int>(value * 10.0));
    process();
}

void HistoDialog::on_clipS_valueChanged(int value) {
    ui->clipSB->setValue(static_cast<float>(value) / 10.0);
    process();
}

void HistoDialog::on_xdivS_valueChanged(int value) {
    ui->xdivSB->setValue(value);
    process();
}

void HistoDialog::on_xdivSB_valueChanged(int value) {
    ui->xdivS->setValue(value);
    process();
}

void HistoDialog::on_ydivS_valueChanged(int value) {
    ui->ydivSB->setValue(value);
    process();
}

void HistoDialog::on_ydivSB_valueChanged(int value) {
    ui->ydivS->setValue(value);
    process();
}

void HistoDialog::on_buttonBox_accepted() {
    int index1 = ui->qcb1->currentIndex();
    int index2 = ui->qcb2->currentIndex();
    if (index2 == 1)
        graphics::Color::matchHistogram(qi, toMatch, graphics::mType(ui->qcb3->currentIndex()));
    else if (index2 == 2)
        graphics::Color::equalizeHistogramTo(qi, graphics::eType(index1));
    else if (index2 == 3)
        graphics::Color::claheTo(qi, graphics::eType(index1), ui->clipSB->value(), ui->xdivSB->value(), ui->ydivSB->value());
    done(1);
}

void HistoDialog::on_buttonBox_rejected() {
    done(0);
}
