#include "textpanel.h"
#include "ui_textpanel.h"

TextPanel::TextPanel(QWidget *parent) : QWidget(parent), ui(new Ui::TextPanel) {
    ui->setupUi(this);
}

void TextPanel::updatePanel(pair<textType, unsigned int> dets) {
    QFontDatabase qfd;
    QColor qc;
    switch (dets.first) {
    case tFont:
        ui->fonts->setCurrentIndex(dets.second == -1 || dets.second > ui->fonts->count() ? 0 : dets.second);
        break;
    case tSize:
        ui->sizeS->setValue(dets.second);
        ui->sizeSB->setValue(dets.second);
        break;
    case tTaw:
        ui->tawS->setValue(dets.second);
        ui->tawSB->setValue(dets.second);
        break;
    case tWord:
        ui->wordS->setValue(dets.second);
        ui->wordSB->setValue(dets.second);
        break;
    case tLetter:
        ui->letterS->setValue(dets.second);
        ui->letterSB->setValue(dets.second);
        break;
    case tBold:
        ui->bold->setChecked(dets.second == 0 ? false : true);
        break;
    case tItal:
        ui->ital->setChecked(dets.second == 0 ? false : true);
        break;
    case tUndr:
        ui->undr->setChecked(dets.second == 0 ? false : true);
        break;
    case tOver:
        ui->over->setChecked(dets.second == 0 ? false : true);
        break;
    case tStrk:
        ui->strk->setChecked(dets.second == 0 ? false : true);
        break;
    case tColor:
        QColor color = dets.second;
        a = QPoint((255 - color.hslSaturation()) / 2, 255 - color.lightness());
        ui->colorS1->setValue(color.hslHue());
        ui->colorSB1->setValue(color.hslHue());
        changeColor();
        break;
    }
}

void TextPanel::updatePanel(QString text) {
    ui->content->setPlainText(text);
}

void TextPanel::updatePanel(DrawText text) {
    ui->content->setPlainText(text.getText().text());
    QFontDatabase qfd;
    int index = qfd.families().indexOf(text.getFont().family());
    ui->fonts->setCurrentIndex(index == -1 || index > ui->fonts->count() ? 0 : index);
    ui->sizeS->setValue(text.getFont().pointSize());
    ui->sizeSB->setValue(text.getFont().pointSize());
    QColor color = text.getColor();
    a = QPoint((255 - color.hslSaturation()) / 2, 255 - color.lightness());
    ui->colorS1->setValue(color.hslHue());
    ui->colorSB1->setValue(color.hslHue());
    changeColor();
    ui->tawS->setValue(text.getText().textWidth());
    ui->tawSB->setValue(text.getText().textWidth());
    ui->wordS->setValue(text.getFont().wordSpacing());
    ui->wordSB->setValue(text.getFont().wordSpacing());
    ui->letterS->setValue(text.getFont().letterSpacing());
    ui->letterSB->setValue(text.getFont().letterSpacing());
    ui->bold->setChecked(text.getFont().bold());
    ui->ital->setChecked(text.getFont().italic());
    ui->undr->setChecked(text.getFont().underline());
    ui->over->setChecked(text.getFont().overline());
    ui->strk->setChecked(text.getFont().strikeOut());
}

void TextPanel::resetPanel() {
    ui->content->setPlainText("");
    ui->fonts->setCurrentIndex(0);
    ui->sizeS->setValue(ui->sizeS->minimum());
    ui->sizeSB->setValue(ui->sizeSB->minimum());
    ui->colorS1->setValue(ui->colorS1->minimum());
    ui->colorSB1->setValue(ui->colorSB1->minimum());
    ui->tawS->setValue(ui->tawS->minimum());
    ui->tawSB->setValue(ui->tawSB->minimum());
    ui->wordS->setValue(ui->wordS->minimum());
    ui->wordSB->setValue(ui->wordSB->minimum());
    ui->letterS->setValue(ui->letterS->minimum());
    ui->letterSB->setValue(ui->letterSB->minimum());
    ui->bold->setChecked(false);
    ui->ital->setChecked(false);
    ui->undr->setChecked(false);
    ui->over->setChecked(false);
    ui->strk->setChecked(false);
}

pair<textType, unsigned int> TextPanel::getData() {
    return ret;
}

QString TextPanel::getText() {
    return ui->content->toPlainText();
}

void TextPanel::mouseMoveEvent(QMouseEvent *event) {
    QPoint qp = event->pos();
    QPoint pt = ui->color1->pos();
    if (qp.x() >= pt.x() && qp.x() < pt.x() + 256 && qp.y() >= pt.y() && qp.y() < pt.y() + 128) {
        a = QPoint(qp.y() - pt.y(), 255 - (qp.x() - pt.x()));
        changeColor();
        emit(actionGiven());
    }
}

void TextPanel::on_content_textChanged() {
    ret = pair <textType, unsigned int> (tText, 0);
    emit(actionGiven());
}

void TextPanel::on_fonts_currentIndexChanged(int index) {
    ret = pair <textType, unsigned int> (tFont, index);
    emit(actionGiven());
}

void TextPanel::on_sizeSB_valueChanged(int value) {
    ui->sizeS->setValue(value);
    ret = pair <textType, unsigned int> (tSize, value);
    emit(actionGiven());
}

void TextPanel::on_sizeS_valueChanged(int value) {
    ui->sizeSB->setValue(value);
    ret = pair <textType, unsigned int> (tSize, value);
    emit(actionGiven());
}

void TextPanel::on_colorSB1_valueChanged(int value) {
    ui->colorS1->setValue(value);
    changeColor();
    emit(actionGiven());
}

void TextPanel::on_colorS1_valueChanged(int value) {
    ui->colorSB1->setValue(value);
    changeColor();
    emit(actionGiven());
}

void TextPanel::on_tawSB_valueChanged(int value) {
    ui->tawS->setValue(value);
    ret = pair <textType, unsigned int> (tTaw, value);
    emit(actionGiven());
}

void TextPanel::on_tawS_valueChanged(int value) {
    ui->tawSB->setValue(value);
    ret = pair <textType, unsigned int> (tTaw, value);
    emit(actionGiven());
}

void TextPanel::on_wordSB_valueChanged(int value) {
    ui->wordS->setValue(value);
    ret = pair <textType, unsigned int> (tWord, value);
    emit(actionGiven());
}

void TextPanel::on_wordS_valueChanged(int value) {
    ui->wordSB->setValue(value);
    ret = pair <textType, unsigned int> (tWord, value);
    emit(actionGiven());
}

void TextPanel::on_letterSB_valueChanged(int value) {
    ui->letterS->setValue(value);
    ret = pair <textType, unsigned int> (tLetter, value);
    emit(actionGiven());
}

void TextPanel::on_letterS_valueChanged(int value) {
    ui->letterSB->setValue(value);
    ret = pair <textType, unsigned int> (tLetter, value);
    emit(actionGiven());
}

void TextPanel::on_bold_toggled(bool which) {
    ret = pair <textType, unsigned int> (tBold, which);
    emit(actionGiven());
}

void TextPanel::on_ital_toggled(bool which) {
    ret = pair <textType, unsigned int> (tItal, which);
    emit(actionGiven());
}

void TextPanel::on_undr_toggled(bool which) {
    ret = pair <textType, unsigned int> (tUndr, which);
    emit(actionGiven());
}

void TextPanel::on_over_toggled(bool which) {
    ret = pair <textType, unsigned int> (tOver, which);
    emit(actionGiven());
}

void TextPanel::on_strk_toggled(bool which) {
    ret = pair <textType, unsigned int> (tStrk, which);
    emit(actionGiven());
}

void TextPanel::changeColor() {
    QImage img(256, 128, QImage::Format_ARGB32_Premultiplied);
    QColor qc;
    QPoint pt = a;
    int hue = ui->colorS1->value();
    int sat = 127 - pt.x();
    int lit = 255 - pt.y();
    qc.setHsl(hue - 1, 2 * sat, lit);
    ret = pair <textType, unsigned int> (tColor, qc.rgba());
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
    ui->color1->setPixmap(QPixmap::fromImage(img));
}
