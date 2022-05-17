#include "layerfunc.h"
#include "ui_layerfunc.h"

LayerFunc::LayerFunc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LayerFunc)
{
    ui->setupUi(this);
    setWindowTitle("Layer Functions");
}

LayerFunc::~LayerFunc() {
    delete ui;
}

void LayerFunc::setWork(int layers) {
    numLayers = layers;
    for (int i = 0; i < layers; ++i) {
        ui->la->addItem(to_string(i).c_str());
        ui->lb->addItem(to_string(i).c_str());
    }
}

void LayerFunc::on_op_currentIndexChanged(int value) {
    ui->opType->setEnabled(value < 7);
    ui->lb->setEnabled(value != 7);
}

void LayerFunc::on_buttonBox_accepted() {
    done(1);
}

void LayerFunc::on_buttonBox_rejected() {
    done(0);
}

vector <int> LayerFunc::getChoice() {
    return vector <int> ({ui->la->currentIndex(), ui->lb->currentIndex(), ui->op->currentIndex(), ui->opType->currentIndex()});
}

