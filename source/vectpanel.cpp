#include "vectpanel.h"
#include "ui_vectpanel.h"

VectPanel::VectPanel(QWidget *parent) : QWidget(parent), ui(new Ui::VectPanel)
{
    ui->setupUi(this);
}

void VectPanel::updatePanel(pair <vType, unsigned int> dets) {
    QColor temp;
    string name;
    switch (dets.first) {
    case vMode:
        ui->modeCB->setCurrentIndex(dets.second);
        break;
    case vFilter:
        name = graphics::filterNames[dets.second];
        for (int i = 0; i < graphics::vectorFilters->length(); ++i)
            if (name == graphics::vectorFilters[i]) {
                ui->filterCB->setCurrentIndex(i);
                break;
            }
        break;
    case vStrength:
        ui->strengthS->setValue(dets.second);
        ui->strengthSB->setValue(dets.second);
        break;
    case vColor1:
        temp = dets.second;
        a = QPoint((255 - temp.hslSaturation()) / 2, 255 - temp.lightness());
        ui->colorS1->setValue(temp.hslHue());
        ui->colorSB1->setValue(temp.hslHue());
        changeColor(1);
        break;
    case vColor2:
        temp = dets.second;
        b = QPoint((255 - temp.hslSaturation()) / 2, 255 - temp.lightness());
        ui->colorS2->setValue(temp.hslHue());
        ui->colorSB2->setValue(temp.hslHue());
        changeColor(2);
        break;
    case vTaper1:
        ui->taperS1->setValue(dets.second);
        ui->taperSB1->setValue(dets.second);
        break;
    case vTaper2:
        ui->taperS2->setValue(dets.second);
        ui->taperSB2->setValue(dets.second);
        break;
    case vGap:
        ui->gapS->setValue(dets.second);
        ui->gapSB->setValue(dets.second);
        break;
    case vBand:
        ui->bandS->setValue(dets.second);
        ui->bandSB->setValue(dets.second);
        break;
    case vWidth:
        ui->widthS->setValue(dets.second);
        ui->widthSB->setValue(dets.second);
        break;
    case vStyle:
        ui->typeCB->setCurrentIndex(dets.second - 1);
        break;
    }
}

void VectPanel::updatePanel(SplineVector sv) {
    ui->modeCB->setCurrentIndex(sv.getMode());
    string name = graphics::filterNames[sv.getFilter().getFilterIndex()];
    for (int i = 0; i < graphics::vectorFilters->length(); ++i)
        if (name == graphics::vectorFilters[i]) {
            ui->filterCB->setCurrentIndex(i);
            break;
        }
    ui->strengthS->setValue(sv.getFilter().getStrength());
    ui->strengthSB->setValue(sv.getFilter().getStrength());
    QColor temp = sv.getColors().second;
    a = QPoint((255 - temp.hslSaturation()) / 2, 255 - temp.lightness());
    ui->colorS1->setValue(temp.hslHue());
    ui->colorSB1->setValue(temp.hslHue());
    changeColor(1);
    temp = sv.getColors().first;
    b = QPoint((255 - temp.hslSaturation()) / 2, 255 - temp.lightness());
    ui->colorS2->setValue(temp.hslHue());
    ui->colorSB2->setValue(temp.hslHue());
    changeColor(2);
    ui->taperS1->setValue(sv.getTaper().first);
    ui->taperSB1->setValue(sv.getTaper().first);
    ui->taperS2->setValue(sv.getTaper().second);
    ui->taperSB2->setValue(sv.getTaper().second);
    ui->gapS->setValue(sv.getGap());
    ui->gapSB->setValue(sv.getGap());
    ui->bandS->setValue(sv.getBand());
    ui->bandSB->setValue(sv.getBand());
    ui->widthS->setValue(sv.getWidth());
    ui->widthSB->setValue(sv.getWidth());
    ui->typeCB->setCurrentIndex(sv.getTaperType());
}

void VectPanel::resetPanel() {
    ui->bandS->setValue(ui->bandS->minimum());
    ui->bandSB->setValue(ui->bandSB->minimum());
    ui->gapS->setValue(ui->gapS->minimum());
    ui->gapS->setValue(ui->gapS->minimum());
    ui->modeCB->setCurrentIndex(0);
    ui->filterCB->setCurrentIndex(0);
    ui->strengthS->setValue(ui->strengthS->maximum());
    ui->strengthSB->setValue(ui->strengthSB->maximum());
    ui->colorS1->setValue(ui->colorS1->minimum());
    ui->colorSB1->setValue(ui->colorSB1->minimum());
    ui->colorS2->setValue(ui->colorS2->minimum());
    ui->colorSB2->setValue(ui->colorSB2->minimum());
    ui->taperS1->setValue(ui->taperS1->minimum());
    ui->taperSB1->setValue(ui->taperSB1->minimum());
    ui->taperS2->setValue(ui->taperS2->minimum());
    ui->taperSB2->setValue(ui->taperSB2->minimum());
    ui->widthS->setValue(minWidth);
    ui->widthSB->setValue(minWidth);
    ui->typeCB->setCurrentIndex(1);
    a = b = QPoint(0, 0);
    changeColor(1);
    changeColor(2);
}

pair <vType, unsigned int> VectPanel::getData() {
    return ret;
}

void VectPanel::mouseMoveEvent(QMouseEvent *event) {
    QPoint qp = event->pos();
    QPoint pt = ui->color1->pos();
    int lb = 0;
    if (qp.x() >= pt.x() && qp.x() < pt.x() + 256 && qp.y() >= pt.y() && qp.y() < pt.y() + 128)
        lb = 1;
    else {
        pt = ui->color2->pos();
        if (qp.x() >= pt.x() && qp.x() < pt.x() + 256 && qp.y() >= pt.y() && qp.y() < pt.y() + 128)
            lb = 2;
    }
    if (lb) {
        (lb == 1 ? a : b) = QPoint(qp.y() - pt.y(), 255 - (qp.x() - pt.x()));
        changeColor(lb);
        emit(actionGiven());
    }
}

void VectPanel::changeColor(int lb) {
    QImage img(256, 128, QImage::Format_ARGB32_Premultiplied);
    QColor qc;
    QPoint pt = (lb == 1 ? a : b);
    int hue = (lb == 1 ? ui->colorS1 : ui->colorS2)->value();
    int sat = 127 - pt.x();
    int lit = 255 - pt.y();
    qc.setHsl(hue - 1, 2 * sat, lit);
    ret = pair <vType, unsigned int> (vType(3 + lb), qc.rgba());
    for (short y = 0; y <= 127; ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(y));
        for (short x = 0; x <= 255; ++x) {
            qc.setHsl(hue - 1, 255 - 2 * y, x);
            line[x] = qc.rgba();
        }
    }
    qc.setHsl(hue - 1, sat, lit);
    int hue2 = hue + 180;
    if (hue2 > 360)
        hue2 -= 360;
    --hue2;
    int cSize = 4;
    for (int y = 0; y < 2 * cSize + 1; ++y) {
        int Y = sat + (y - cSize);
        if (Y < 0 || Y > 127)
            continue;
        for (int x = y == cSize ? 0 : cSize; x < 2 * cSize + 1; x += y == cSize ? 1 : 2 * cSize + 1) {
            int X = lit + (x - cSize);
            if ((y == cSize && x == cSize) || X < 0 || X > 255)
                continue;
            QColor temp;
            temp.setHsl(hue2, Y < 24 ? 255 - 2 * Y : Y, 255 - X);
            img.setPixel(X, 127 - Y, temp.rgba());
        }
    }
   (lb == 1 ? ui->color1 : ui->color2)->setPixmap(QPixmap::fromImage(img));
}

void VectPanel::on_modeCB_currentIndexChanged(int index) {
    ret = pair <vType, unsigned int> (vMode, index);
    emit(actionGiven());
}

void VectPanel::on_filterCB_currentIndexChanged(int index) {
    ret = pair <vType, unsigned int> (vFilter, index);
    emit(actionGiven());
}

void VectPanel::on_strengthSB_valueChanged(int value) {
    ret = pair <vType, unsigned int> (vStrength, value);
    ui->strengthS->setValue(value);
    emit(actionGiven());
}

void VectPanel::on_strengthS_valueChanged(int value) {
    ret = pair <vType, unsigned int> (vStrength, value);
    ui->strengthSB->setValue(value);
    emit(actionGiven());
}

void VectPanel::on_colorSB1_valueChanged(int value) {
    ui->colorS1->setValue(value);
    changeColor(1);
    emit(actionGiven());
}

void VectPanel::on_colorS1_valueChanged(int value) {
    ui->colorSB1->setValue(value);
    changeColor(1);
    emit(actionGiven());
}

void VectPanel::on_colorSB2_valueChanged(int value) {
    ui->colorS2->setValue(value);
    changeColor(2);
    emit(actionGiven());
}

void VectPanel::on_colorS2_valueChanged(int value) {
    ui->colorSB2->setValue(value);
    changeColor(2);
    emit(actionGiven());
}

void VectPanel::on_widthS_valueChanged(int value) {
    ret = pair <vType, unsigned int> (vWidth, value);
    ui->widthSB->setValue(value);
    emit(actionGiven());
}

void VectPanel::on_widthSB_valueChanged(int value) {
    ret = pair <vType, unsigned int> (vWidth, value);
    ui->widthS->setValue(value);
    emit(actionGiven());
}

void VectPanel::on_taperSB1_valueChanged(int value) {
    ret = pair <vType, unsigned int> (vTaper1, value);
    ui->taperS1->setValue(value);
    emit(actionGiven());
}

void VectPanel::on_taperS1_valueChanged(int value) {
    ret = pair <vType, unsigned int> (vTaper1, value);
    ui->taperSB1->setValue(value);
    emit(actionGiven());
}

void VectPanel::on_taperSB2_valueChanged(int value) {
    ret = pair <vType, unsigned int> (vTaper2, value);
    ui->taperS2->setValue(value);
    emit(actionGiven());
}

void VectPanel::on_taperS2_valueChanged(int value) {
    ret = pair <vType, unsigned int> (vTaper2, value);
    ui->taperSB2->setValue(value);
    emit(actionGiven());
}

void VectPanel::on_typeCB_currentIndexChanged(int index) {
    ret = pair <vType, unsigned int> (vStyle, index);
    emit(actionGiven());
}

void VectPanel::on_gapSB_valueChanged(int value) {
    ret = pair <vType, unsigned int> (vGap, value);
    ui->gapS->setValue(value);
    emit(actionGiven());
}

void VectPanel::on_gapS_valueChanged(int value) {
    ret = pair <vType, unsigned int> (vGap, value);
    ui->gapSB->setValue(value);
    emit(actionGiven());
}

void VectPanel::on_bandSB_valueChanged(int value) {
    ret = pair <vType, unsigned int> (vBand, value);
    ui->bandS->setValue(value);
    emit(actionGiven());
}

void VectPanel::on_bandS_valueChanged(int value) {
    ret = pair <vType, unsigned int> (vBand, value);
    ui->bandSB->setValue(value);
    emit(actionGiven());
}

void VectPanel::on_symPB_clicked() {
    ret = pair <vType, unsigned int> (vSym, 0);
    emit(actionGiven());
}

void VectPanel::on_cSwapPB_clicked() {
    int swap = ui->colorS1->value();
    QPoint pt = a;
    a = b;
    b = pt;
    ui->colorS1->setValue(ui->colorS2->value());
    ui->colorSB1->setValue(ui->colorS2->value());
    ui->colorS2->setValue(swap);
    ui->colorSB2->setValue(swap);
}

void VectPanel::on_tSwapPB_clicked() {
    int swap = ui->taperS1->value();
    ui->taperS1->setValue(ui->taperS2->value());
    ui->taperSB1->setValue(ui->taperS2->value());
    ui->taperS2->setValue(swap);
    ui->taperSB2->setValue(swap);
}
