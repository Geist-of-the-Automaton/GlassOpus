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
            graphics::Color::claheTo(&processed, graphics::eType(index1));
        processed = processed.scaled(qi->width() / 3, qi->height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->label->setPixmap(QPixmap::fromImage(processed));
    }
}

void HistoDialog::on_buttonBox_accepted() {
    int index1 = ui->qcb1->currentIndex();
    int index2 = ui->qcb2->currentIndex();
    if (index2 == 1)
        graphics::Color::matchHistogram(qi, toMatch, graphics::mType(ui->qcb3->currentIndex()));
    else if (index2 == 2)
        graphics::Color::equalizeHistogramTo(qi, graphics::eType(index1));
    else if (index2 == 3)
        graphics::Color::claheTo(qi, graphics::eType(index1));
    done(1);
}

void HistoDialog::on_buttonBox_rejected() {
    done(0);
}
