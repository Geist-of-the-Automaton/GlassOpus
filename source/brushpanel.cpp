#include "brushpanel.h"
#include "ui_brushpanel.h"

BrushPanel::BrushPanel(QWidget *parent) : QWidget(parent), ui(new Ui::BrushPanel)
{
    ui->setupUi(this);
    bh = nullptr;
}

void BrushPanel::setWork(brushHandler *bhandler, patternProfiler *pprofiler, brushShape *bshape, RadialProfiler *rprofiler) {
    bh = bhandler;
    pp = pprofiler;
    bs = bshape;
    rp = rprofiler;
}

void BrushPanel::updateFromBH() {
    int temp;
    temp = bh->getDensity();
    ui->sprayS->setValue(temp);
    ui->spraySB->setValue(temp);
    temp = bh->getSize();
    ui->radiusS->setValue(temp);
    ui->radiusSB->setValue(temp);
    temp = bh->getSize();
    ui->radiusS->setValue(temp);
    ui->radiusSB->setValue(temp);
    temp = bh->getStength();
    ui->strengthS->setValue(temp);
    ui->strengthSB->setValue(temp);
    QColor qc = bh->getBrushColor();
    ui->colorS->setValue(qc.hslHue() + 1);
    ui->colorSB->setValue(qc.hslHue() + 1);
    a = QPoint(255 - qc.hslSaturation(), 255 - qc.lightness());
    changeColor();
    ui->patternRB->setDown(bh->getPatternInUse());
    ui->filterCB->setCurrentIndex(bh->getFilterIndex());
    ui->methodCB->setCurrentIndex(bh->getMethodIndex());
    ui->shapeCB->setCurrentIndex(bh->getBrushShape());

}

void BrushPanel::mouseMoveEvent(QMouseEvent *event) {
    if (bh != nullptr) {
        QPoint qp = event->pos();
        QPoint pt = ui->color->pos();
        if (qp.x() >= pt.x() && qp.x() < pt.x() + 256 && qp.y() >= pt.y() && qp.y() < pt.y() + 256) {
            a = QPoint(qp.y() - pt.y(), 255 - (qp.x() - pt.x()));
            changeColor();
        }
    }
}

void BrushPanel::on_methodCB_currentIndexChanged(int index) {
    if (bh != nullptr)
        bh->setAppMethod(ui->methodCB->currentText().toStdString());
}

void BrushPanel::on_shapeCB_currentIndexChanged(int index) {
    if (bh != nullptr) {
        string text = ui->shapeCB->currentText().toStdString();
        if (text == "Custom")
            bh->setShape(text, bs->getShapeSize(bh->getSize()));
        else
            bh->setShape(text);
        emit(updateSizeShape());
    }
}

void BrushPanel::on_radiusSB_valueChanged(int value) {
    ui->radiusS->setValue(value);
    if (bh != nullptr) {
        rp->updateSize(value);
        string text = ui->shapeCB->currentText().toStdString();
        if (text == "custom")
            bh->setShape(text, bs->getShapeSize(bh->getSize()));
        emit(updateSizeShape());
    }
}

void BrushPanel::on_radiusS_valueChanged(int value) {
    ui->radiusSB->setValue(value);
    if (bh != nullptr) {
        rp->updateSize(value);
        string text = ui->shapeCB->currentText().toStdString();
        if (text == "custom")
            bh->setShape(text, bs->getShapeSize(bh->getSize()));
        emit(updateSizeShape());
    }
}

void BrushPanel::on_strengthSB_valueChanged(int value) {
    ui->strengthS->setValue(value);
    if (bh != nullptr)
        bh->setStrength(value);
}

void BrushPanel::on_strengthS_valueChanged(int value) {
    ui->strengthSB->setValue(value);
    if (bh != nullptr)
        bh->setStrength(value);
}

void BrushPanel::on_colorSB_valueChanged(int value) {
    ui->colorS->setValue(value);
    if (bh != nullptr)
        changeColor();
}

void BrushPanel::on_colorS_valueChanged(int value) {
    ui->colorSB->setValue(value);
    if (bh != nullptr)
        changeColor();
}

void BrushPanel::changeColor() {
    QImage img(256, 256, QImage::Format_ARGB32_Premultiplied);
    QColor qc;
    int hue = ui->colorS->value();
    int sat = 255 - a.x();
    int lit = 255 - a.y();
    qc.setHsl(hue - 1, sat, lit);
    bh->setBrushColor(qc);
    for (short y = 0; y <= 255; ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(y));
        for (short x = 0; x <= 255; ++x) {
            qc.setHsl(hue - 1, 255 - y, x);
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
        if (Y < 0 || Y > 255)
            continue;
        for (int x = y == cSize ? 0 : cSize; x < 2 * cSize + 1; x += y == cSize ? 1 : 2 * cSize + 1) {
            int X = lit + (x - cSize);
            if ((y == cSize && x == cSize) || X < 0 || X > 255)
                continue;
            QColor temp;
            temp.setHsl(hue2, Y < 48 ? 255 - Y : Y, 255 - X);
            img.setPixel(X, 255 - Y, temp.rgba());
        }
    }
    ui->color->setPixmap(QPixmap::fromImage(img));
}

void BrushPanel::on_filterCB_currentIndexChanged(int index) {
    if (bh != nullptr)
        bh->setFilter(ui->filterCB->currentText().toStdString());
}

void BrushPanel::on_spraySB_valueChanged(int value) {
    ui->sprayS->setValue(value);
    if (bh != nullptr)
        bh->setDensity(value);
}

void BrushPanel::on_sprayS_valueChanged(int value) {
    ui->spraySB->setValue(value);
    if (bh != nullptr)
        bh->setDensity(value);
}

void BrushPanel::on_patternRB_clicked() {
    if (bh != nullptr)
        bh->setPatternInUse(ui->patternRB->isChecked());
}

void BrushPanel::on_patternPB_clicked() {
    pp->exec();
    bh->setPattern(pp->getPattern());
}

void BrushPanel::on_shapePB_clicked() {
    bs->exec();
    bh->setShape(brushShapes[bh->getBrushShape()], bs->getShapeSize(bh->getSize()));
    emit(updateSizeShape());
}

void BrushPanel::on_radialPB_clicked() {
    rp->showRelative();
}

void BrushPanel::on_symPB_clicked() {
    emit(symExec());
}


