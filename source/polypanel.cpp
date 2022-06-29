#include "polypanel.h"
#include "ui_polypanel.h"

PolyPanel::PolyPanel(QWidget *parent) : QWidget(parent), ui(new Ui::PolyPanel) {
    ui->setupUi(this);
}

void PolyPanel::updatePanel(pair <pType, unsigned int> dets) {
    QColor temp;
    string name;
    switch (dets.first) {
    case pMode:
        ui->modeCB->setCurrentIndex(dets.second);
        break;
    case pFilter:
        name = graphics::filterNames[dets.second];
        for (int i = 0; i < graphics::vectorFilters->length(); ++i)
            if (name == graphics::vectorFilters[i]) {
                ui->filterCB->setCurrentIndex(i);
                break;
            }
        break;
    case pStrength:
        ui->strengthS->setValue(dets.second);
        ui->strengthSB->setValue(dets.second);
        break;
    case pColor1:
        temp = dets.second;
        a = QPoint((255 - temp.hslSaturation()) / 2, 255 - temp.lightness());
        ui->colorS1->setValue(temp.hslHue());
        ui->colorSB1->setValue(temp.hslHue());
        changeColor(1);
        break;
    case pColor2:
        temp = dets.second;
        b = QPoint((255 - temp.hslSaturation()) / 2, 255 - temp.lightness());
        ui->colorS2->setValue(temp.hslHue());
        ui->colorSB2->setValue(temp.hslHue());
        changeColor(2);
        break;
    case pBand:
        ui->bandS->setValue(dets.second);
        ui->bandSB->setValue(dets.second);
        break;
    case pTriView:
        ui->tViewRB->setChecked(dets.second);
        break;
    }
}

void PolyPanel::updatePanel(Polygon gon) {
    ui->modeCB->setCurrentIndex(gon.getPolyMode());
    string name = graphics::filterNames[gon.getFilter().getFilterIndex()];
    for (int i = 0; i < graphics::vectorFilters->length(); ++i)
        if (name == graphics::vectorFilters[i]) {
            ui->filterCB->setCurrentIndex(i);
            break;
        }
    ui->strengthS->setValue(gon.getFilter().getStrength());
    ui->strengthSB->setValue(gon.getFilter().getStrength());
    QColor temp = gon.getPolyColor();
    a = QPoint((255 - temp.hslSaturation()) / 2, 255 - temp.lightness());
    ui->colorS1->setValue(temp.hslHue());
    ui->colorSB1->setValue(temp.hslHue());
    changeColor(1);
    temp = gon.getEdgeColor();
    b = QPoint((255 - temp.hslSaturation()) / 2, 255 - temp.lightness());
    ui->colorS2->setValue(temp.hslHue());
    ui->colorSB2->setValue(temp.hslHue());
    changeColor(2);
    ui->bandS->setValue(gon.getEdgeSize());
    ui->bandSB->setValue(gon.getEdgeSize());
}

void PolyPanel::resetPanel() {
    ui->bandS->setValue(ui->bandS->minimum());
    ui->bandSB->setValue(ui->bandSB->minimum());
    ui->modeCB->setCurrentIndex(0);
    ui->filterCB->setCurrentIndex(0);
    ui->strengthS->setValue(ui->strengthS->maximum());
    ui->strengthSB->setValue(ui->strengthSB->maximum());
    ui->colorS1->setValue(ui->colorS1->minimum());
    ui->colorSB1->setValue(ui->colorSB1->minimum());
    ui->colorS2->setValue(ui->colorS2->minimum());
    ui->colorSB2->setValue(ui->colorSB2->minimum());
    a = b = QPoint(0, 0);
    changeColor(1);
    changeColor(2);
}

pair <pType, unsigned int> PolyPanel::getData() {
    return ret;
}

void PolyPanel::mouseMoveEvent(QMouseEvent *event) {
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

void PolyPanel::changeColor(int lb) {
    QImage img(256, 128, QImage::Format_ARGB32_Premultiplied);
    QColor qc;
    QPoint pt = (lb == 1 ? a : b);
    int hue = (lb == 1 ? ui->colorS1 : ui->colorS2)->value();
    int sat = 127 - pt.x();
    int lit = 255 - pt.y();
    qc.setHsl(hue - 1, 2 * sat, lit);
    ret = pair <pType, unsigned int> (pType(3 + lb), qc.rgba());
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

void PolyPanel::on_modeCB_currentIndexChanged(int index) {
    ret = pair <pType, unsigned int> (pMode, index);
    emit(actionGiven());
}

void PolyPanel::on_filterCB_currentIndexChanged(int index) {
    ret = pair <pType, unsigned int> (pFilter, index);
    emit(actionGiven());
}

void PolyPanel::on_strengthSB_valueChanged(int value) {
    ret = pair <pType, unsigned int> (pStrength, value);
    ui->strengthS->setValue(value);
    emit(actionGiven());
}

void PolyPanel::on_strengthS_valueChanged(int value) {
    ret = pair <pType, unsigned int> (pStrength, value);
    ui->strengthSB->setValue(value);
    emit(actionGiven());
}

void PolyPanel::on_colorSB1_valueChanged(int value) {
    ui->colorS1->setValue(value);
    changeColor(1);
    emit(actionGiven());
}

void PolyPanel::on_colorS1_valueChanged(int value) {
    ui->colorSB1->setValue(value);
    changeColor(1);
    emit(actionGiven());
}

void PolyPanel::on_colorSB2_valueChanged(int value) {
    ui->colorS2->setValue(value);
    changeColor(2);
    emit(actionGiven());
}

void PolyPanel::on_colorS2_valueChanged(int value) {
    ui->colorSB2->setValue(value);
    changeColor(2);
    emit(actionGiven());
}

void PolyPanel::on_bandSB_valueChanged(int value) {
    ret = pair <pType, unsigned int> (pBand, value);
    ui->bandS->setValue(value);
    emit(actionGiven());
}

void PolyPanel::on_bandS_valueChanged(int value) {
    ret = pair <pType, unsigned int> (pBand, value);
    ui->bandSB->setValue(value);
    emit(actionGiven());
}

void PolyPanel::on_tViewRB_clicked() {
    ret = pair <pType, unsigned int> (pTriView, ui->tViewRB->isChecked() ? 1 : 0);
    emit(actionGiven());
}

void PolyPanel::on_cSwapPB_clicked() {
    int swap = ui->colorS1->value();
    QPoint pt = a;
    a = b;
    b = pt;
    ui->colorS1->setValue(ui->colorS2->value());
    ui->colorSB1->setValue(ui->colorS2->value());
    ui->colorS2->setValue(swap);
    ui->colorSB2->setValue(swap);
}

void PolyPanel::on_rPtPB_clicked() {
    ret = pair <pType, unsigned int> (pReduce, 0);
    emit(actionGiven());
}

