#include "rasterpanel.h"
#include "ui_rasterpanel.h"

RasterPanel::RasterPanel(QWidget *parent) : QWidget(parent), ui(new Ui::RasterPanel) {
    ui->setupUi(this);
}

void RasterPanel::updatePanel(QColor color) {
    a = QPoint(255 - color.hslSaturation(), 255 - color.lightness());
    ui->colorS1->setValue(color.hslHue());
    ui->colorSB1->setValue(color.hslHue());
    changeColor();
}

void RasterPanel::resetPanel() {
    updatePanel(0xFF000000);
}

pair<rType, unsigned int> RasterPanel::getData() {
    return ret;
}

void RasterPanel::mouseMoveEvent(QMouseEvent *event) {
    QPoint qp = event->pos();
    QPoint pt = ui->color1->pos();
    if (qp.x() >= pt.x() && qp.x() < pt.x() + 256 && qp.y() >= pt.y() && qp.y() < pt.y() + 256) {
        a = QPoint(qp.y() - pt.y(), 255 - (qp.x() - pt.x()));
        changeColor();
        emit(actionGiven());
    }
}

void RasterPanel::on_colorSB_valueChanged(int value) {
    ui->colorS1->setValue(value);
    changeColor();
    emit(actionGiven());
}

void RasterPanel::on_colorS_valueChanged(int value) {
    ui->colorSB1->setValue(value);
    changeColor();
    emit(actionGiven());
}

void RasterPanel::on_takeColorPB_clicked() {
    ret = pair <rType, unsigned int> (rTake, 0);
    emit(actionGiven());
}

void RasterPanel::on_transFillPB_clicked() {
    ret = pair <rType, unsigned int> (rTransFill, 0);
    emit(actionGiven());
}

void RasterPanel::on_mWandPB_clicked() {
    ret = pair <rType, unsigned int> (rWand, 0);
    emit(actionGiven());
}

void RasterPanel::on_mWandSetPB_clicked() {
    ret = pair <rType, unsigned int> (rWandSet, 0);
    emit(actionGiven());
}

void RasterPanel::on_hFlipPB_clicked() {
    ret = pair <rType, unsigned int> (rFlipH, 0);
    emit(actionGiven());
}

void RasterPanel::on_vFlipPB_clicked() {
    ret = pair <rType, unsigned int> (rFlipV, 0);
    emit(actionGiven());
}

void RasterPanel::on_briPB_clicked() {
    ret = pair <rType, unsigned int> (rBri, 0);
    emit(actionGiven());
}

void RasterPanel::on_conPB_clicked() {
    ret = pair <rType, unsigned int> (rCon, 0);
    emit(actionGiven());
}

void RasterPanel::on_gamPB_clicked() {
    ret = pair <rType, unsigned int> (rGam, 0);
    emit(actionGiven());
}

void RasterPanel::on_satPB_clicked() {
    ret = pair <rType, unsigned int> (rSat, 0);
    emit(actionGiven());
}

void RasterPanel::on_huePB_clicked() {
    ret = pair <rType, unsigned int> (rHue, 0);
    emit(actionGiven());
}

void RasterPanel::on_tarPB_clicked() {
    ret = pair <rType, unsigned int> (rTar, 0);
    emit(actionGiven());
}

void RasterPanel::on_filtPB_clicked() {
    ret = pair <rType, unsigned int> (rFilt, 0);
    emit(actionGiven());
}

void RasterPanel::on_convPB_clicked() {
    ret = pair <rType, unsigned int> (rConv, 0);
    emit(actionGiven());
}

void RasterPanel::on_ditherPB_clicked() {
    ret = pair <rType, unsigned int> (rDith, 0);
    emit(actionGiven());

}

void RasterPanel::on_cTransPB_clicked() {
    ret = pair <rType, unsigned int> (rTran, 0);
    emit(actionGiven());
}

void RasterPanel::changeColor() {
    QImage img(256, 256, QImage::Format_ARGB32_Premultiplied);
    QColor qc;
    QPoint pt = a;
    int hue = ui->colorS1->value();
    int sat = 255 - pt.x();
    int lit = 255 - pt.y();
    qc.setHsl(hue - 1, sat, lit);
    ret = pair <rType, unsigned int> (rColor, qc.rgba());
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
            temp.setHsl(hue2, Y < 24 ? 255 - Y : Y, 255 - X);
            img.setPixel(X, 255 - Y, temp.rgba());
        }
    }
    ui->color1->setPixmap(QPixmap::fromImage(img));
}

