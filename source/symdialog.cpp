#include "symdialog.h"
#include "ui_symdialog.h"

SymDialog::SymDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SymDialog)
{
    ui->setupUi(this);
    setWindowTitle("Symmetry Dialog");
    connect(ui->qcb1, SIGNAL(currentIndexChanged(int)), this, SLOT(divChange()));
    connect(ui->qcb2, SIGNAL(currentIndexChanged(int)), this, SLOT(ofEveryChange()));
    div = 1;
    skip = 0;
    ofEvery = 1;
    divChange();
}

SymDialog::~SymDialog() {
    delete ui;
}

void SymDialog::setIndicies(int s1, int s2, int s3) {
    ui->qcb1->setCurrentIndex(s1);
    div = s1;
    divChange();
    ui->qcb2->setCurrentIndex(s2);
    ofEvery = s2;
    ofEveryChange();
    ui->qcb3->setCurrentIndex(s3);
    skip = s3;
}

void SymDialog::divChange() {
    int qcb1_index = ui->qcb1->currentIndex();
    QComboBox *qcb2 = ui->qcb2;
    qcb2->clear();
    QComboBox *qcb3 = ui->qcb3;
    qcb3->clear();
    for (int i = 0; i <= qcb1_index; ++i)
        qcb2->addItem(("Of Every " + to_string(i + 1)).c_str());
    qcb2->setCurrentIndex(0);
    qcb3->addItem("Skip 0");
    qcb3->setCurrentIndex(0);
    if (qcb1_index != 0) {
        qcb2->setEnabled(true);
        qcb3->setEnabled(true);
    }
    else {
        qcb2->setEnabled(false);
        qcb3->setEnabled(false);
    }
}

void SymDialog::ofEveryChange() {
    QComboBox *qcb2 = ui->qcb2;
    QComboBox *qcb3 = ui->qcb3;
    qcb3->clear();
    for (int i = 0; i <= qcb2->currentIndex(); ++i)
        qcb3->addItem(("Skip " + to_string(i)).c_str());
}

void SymDialog::on_buttonBox_accepted() {
    div = ui->qcb1->currentIndex() + 1;
    ofEvery = ui->qcb2->currentIndex() + 1;
    skip = ui->qcb3->currentIndex();
    done(1);
}

void SymDialog::on_buttonBox_rejected() {
    done(0);
}

int SymDialog::getDiv() {
    return div;
}

int SymDialog::getOfEvery() {
    return ofEvery;
}

int SymDialog::getSkip() {
    return skip;
}

