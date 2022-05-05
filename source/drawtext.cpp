#include "drawtext.h"

DrawText::DrawText() {
    color = 0xFF000000;
    angle = 0.0;
    xscale = 1.0;
    yscale = 1.0;
    pos = QPoint(0, 0);
    trans = QTransform(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
    font = QFont();
    sText.setTextWidth(200);
}

DrawText::DrawText(const DrawText &txt) {
    *this = txt;
}

DrawText& DrawText::operator=(const DrawText &txt) {
    color = txt.color;
    angle = txt.angle;
    xscale = txt.xscale;
    yscale = txt.yscale;
    pos = txt.pos;
    sText = txt.sText;
    trans = txt.trans;
    font = txt.font;
    return *this;
}

QPoint DrawText::getShowPoint() {
    float s = sin(angle / rad2deg);
    float c = cos(angle / rad2deg);
    QPoint init = QPoint(-1 - ptSize / 2, -1 - ptSize / 2);
    return QPoint(init.x() * c - init.y() * s + pos.x(), init.x() * s + init.y() * c + pos.y());
}

QStaticText DrawText::getText() {
    return sText;
}

QTransform DrawText::getTransform() {
    return trans;
}

QFont DrawText::getFont() {
    return font;
}

QColor DrawText::getColor() {
    return color;
}

QPoint DrawText::getCorner() {
    return pos;
}

void DrawText::updateFont(QFont qf) {
    font = qf;
}

void DrawText::updateColor(QColor qc) {
    color = qc;
}

bool DrawText::updateText(Qt::Key key, bool shiftActive) {
    bool ret = false;
    if (key >= 0x20 && key <= 0x7E) {
        char c = static_cast<char>(key);
        if (!shiftActive && c >= 'A' && c <= 'Z')
            c += 32;
        sText.setText(sText.text() + c);
        ret = true;
    }
    else if (key == Qt::Key_Backspace) {
        QString qs = sText.text();
        sText.setText(sText.text().toStdString().substr(0, qs.length() - 1).c_str());
        ret = true;
    }
    return ret;
}

void DrawText::updateText(QStaticText qsText) {
    sText = qsText;
}

void DrawText::updateScale(float x, float y) {
    xscale += x;
    yscale += y;
    QTransform qt;
    qt.translate(pos.x(), pos.y());
    qt.rotate(angle);
    qt.scale(xscale, yscale);
    trans = qt;
}

void DrawText::updatePos(QPoint loc) {
    pos += loc;
    QTransform qt;
    qt.translate(pos.x(), pos.y());
    qt.rotate(angle);
    qt.scale(xscale, yscale);
    trans = qt;
}

void DrawText::updateRotate(float rot) {
    angle += rot;
    QTransform qt;
    qt.translate(pos.x(), pos.y());
    qt.rotate(angle);
    qt.scale(xscale, yscale);
    trans = qt;
}

QVector <float> DrawText::getVals() {
    return QVector <float> ({angle, xscale, yscale});
}

