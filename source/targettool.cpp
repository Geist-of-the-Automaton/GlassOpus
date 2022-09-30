#include "targettool.h"
#include "ui_targetdialog.h"

TargetTool::TargetTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TargetTool)
{
    ui->setupUi(this);
    setWindowTitle("Target Tool");
}

TargetTool::~TargetTool() {
    delete ui;
}

void TargetTool::setWork(QImage *toProcess) {
    qi = toProcess;
    processed = qi->scaled(qi->width() / 2, qi->height() / 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    proDisp = processed.copy();
    ui->img->setPixmap(QPixmap::fromImage(proDisp));
    on_op_currentIndexChanged(8);
}

void TargetTool::on_hueTarSB_valueChanged(int value) {
    ui->hueTarS->setValue(value);
    hueTar = value;
    process();
}

void TargetTool::on_hueTarS_valueChanged(int value) {
    ui->hueTarSB->setValue(value);
    hueTar = value;
    process();
}

void TargetTool::on_hueBndSB_valueChanged(int value) {
    ui->hueBndS->setValue(value);
    hueBnd = value;
    process();
}

void TargetTool::on_hueBndS_valueChanged(int value) {
    ui->hueBndSB->setValue(value);
    hueBnd = value;
    process();
}

void TargetTool::on_satTarSB_valueChanged(int value) {
    ui->satTarS->setValue(value);
    satTar = value;
    process();
}

void TargetTool::on_satTarS_valueChanged(int value) {
    ui->satTarSB->setValue(value);
    satTar = value;
    process();
}

void TargetTool::on_satBndSB_valueChanged(int value) {
    ui->satBndS->setValue(value);
    satBnd = value;
    process();
}

void TargetTool::on_satBndS_valueChanged(int value) {
    ui->satBndSB->setValue(value);
    satBnd = value;
    process();
}

void TargetTool::on_litTarSB_valueChanged(int value) {
    ui->litTarS->setValue(value);
    litTar = value;
    process();
}

void TargetTool::on_litTarS_valueChanged(int value) {
    ui->litTarSB->setValue(value);
    litTar = value;
    process();
}

void TargetTool::on_litBndSB_valueChanged(int value) {
    ui->litBndS->setValue(value);
    litBnd = value;
    process();
}

void TargetTool::on_litBndS_valueChanged(int value) {
    ui->litBndSB->setValue(value);
    litBnd = value;
    process();
}

void TargetTool::on_opSB_valueChanged(int value) {
    ui->opS->setValue(value);
    opVal = value;
    preprocess();
    process();
}

void TargetTool::on_opS_valueChanged(int value) {
    ui->opSB->setValue(value);
    opVal = value;
    preprocess();
    process();
}

void TargetTool::preprocess() {
    int op = ui->op->currentIndex();
    int opType = ui->opType->currentIndex();
    processed2 = processed.copy();
    if (op == 0) {
       processed2 = QImage(processed.size(), QImage::Format_ARGB32);
       processed2.fill(0x00000000);
    }
    else if (op == 1) {
        Kernel k = opType == 0 ? graphics::Filtering::getBoxBlur(opVal) : graphics::Filtering::getConeBlur(opVal);
        KernelData data(pair<bool, Kernel>(false, k));
        graphics::Filtering::applyKernel(nullptr, &processed2, data);
    }
    else if (op == 2) {
        graphics::Color::hueShift(&processed2, opVal);
    }
    else if (op == 3) {
        if (opVal != 0.0)
            graphics::Color::contrastAdjust(&processed2, static_cast<double>(opVal) / 255.0);
    }
    else if (op == 4) {
        graphics::eType type = graphics::eType(opType);
        if (opVal != 0.0)
            graphics::Color::saturationAdjust(&processed2, static_cast<double>(opVal) / 255.0, type);
    }
    else if (op == 5) {
        graphics::eType type = graphics::eType(opType);
        if (opVal != 0.0)
            graphics::Color::brightnessAdjust(&processed2, static_cast<double>(opVal) / 255.0, type);
    }
    else if (op == 6) {
        if (opVal != 0.0)
            graphics::Color::gammaAdjust(&processed2, static_cast<double>(opVal) / 255.0);
    }
    else if (op == 7) {
        graphics::Filter f(opVal, ui->opType->currentText().toStdString());
        f.setStrength(opVal);
        f.applyTo(&processed2);
    }
    else if (op == 8) {
        if (opType == 0)
            graphics::Color::ditherBayer(&processed2, opVal, 32);
        else if (opType == 1)
            graphics::Color::ditherFloydSteinberg(&processed2, opVal);
        else if (opType == 2)
            graphics::Color::ditherSierra(&processed2, opVal);
        else if (opType == 3)
            graphics::Color::ditherSierraLite(&processed2, opVal);
        else if (opType == 4)
            graphics::Color::ditherRandom(&processed2, opVal);
        else if (opType == 5)
            graphics::Color::paletteReduction(&processed2, opVal);
    }
}

void TargetTool::on_op_currentIndexChanged(int value) {
    ui->opType->clear();
    ui->opType->setEnabled(true);
    if (value == 1) {
        ui->opType->addItems(QStringList({"Box", "Cone"}));
        ui->opS->setRange(1, 20);
        ui->opSB->setRange(1, 20);
        ui->opS->setValue(1);
        ui->opSB->setValue(1);
    }
    else if (value == 2) {
        ui->opS->setRange(0, 360);
        ui->opSB->setRange(0, 360);
        ui->opS->setValue(0);
        ui->opSB->setValue(0);
    }
    else if (value == 3 || value == 6) {
        ui->opS->setRange(-255, 255);
        ui->opSB->setRange(-255, 255);
        ui->opS->setValue(0);
        ui->opSB->setValue(0);
    }
    else if (value == 4) {
        ui->opType->addItems(QStringList({"HSL / HSI", "HSV / HSB"}));
        ui->opS->setRange(-255, 255);
        ui->opSB->setRange(-255, 255);
        ui->opS->setValue(0);
        ui->opSB->setValue(0);
    }
    else if (value == 5) {
        ui->opType->addItems(graphics::eTypes);
        ui->opS->setRange(-255, 255);
        ui->opSB->setRange(-255, 255);
        ui->opS->setValue(0);
        ui->opSB->setValue(0);
    }
    else if (value == 7) {
        for (int i = 0; i < graphics::numFilters; ++i)
            ui->opType->addItem(graphics::filterNames[i].c_str());
        ui->opS->setRange(0, 255);
        ui->opSB->setRange(0, 255);
        ui->opS->setValue(255);
        ui->opSB->setValue(255);
    }
    else if (value == 8) {
        ui->opType->addItems(QStringList({"Bayer", "Floyd Steinberg", "Sierra", "Sierra Lite", "Random", "Palette Reduction"}));
        ui->opS->setRange(1, 7);
        ui->opSB->setRange(1, 7);
        ui->opS->setValue(7);
        ui->opSB->setValue(7);
    }
    else {
        ui->opType->setEnabled(false);
        ui->opS->setRange(0, 255);
        ui->opSB->setRange(0, 255);
        ui->opS->setValue(255);
        ui->opSB->setValue(255);
    }
    ui->opType->setCurrentIndex(0);
    opVal = ui->opS->value();
    preprocess();
    process();
}

void TargetTool::on_opType_currentIndexChanged(int value) {
    preprocess();
    process();
}

void TargetTool::mousePressEvent(QMouseEvent *event) {
    processMouse(event->pos());
}

void TargetTool::mouseMoveEvent(QMouseEvent *event) {
    processMouse(event->pos());
}

void TargetTool::processMouse(QPoint qp) {
    if (qp.x() < 0 || qp.y() < 0 || qp.x() >= proDisp.width() || qp.y() >= proDisp.height())
        return;
    QColor qc = processed.pixelColor(qp);
    hueTar = qc.hslHue();
    satTar = qc.hslSaturation();
    litTar = qc.lightness();
    ui->hueTarS->setValue(hueTar);
    ui->hueTarSB->setValue(hueTar);
    ui->satTarS->setValue(satTar);
    ui->satTarSB->setValue(satTar);
    ui->litTarS->setValue(litTar);
    ui->litTarSB->setValue(litTar);
    process();
}

void TargetTool::process() {
    proDisp = processed.copy();
    for (int j = 0; j < proDisp.height(); ++j) {
        QRgb *line = reinterpret_cast<QRgb *>(proDisp.scanLine(j));
        QRgb *line2 = reinterpret_cast<QRgb *>(processed2.scanLine(j));
        for (int i = 0; i < proDisp.width(); ++i) {
            QColor qc(line[i]);
            int lHue = hueTar - hueBnd;
            int hHue = hueTar + hueBnd;
            int hue = qc.hslHue() + 1;
            bool flag = hue >= hueTar - hueBnd && hue <= hueTar + hueBnd;
            if (!flag)
                flag = (lHue < 0 && hue >= lHue + 360) || (hHue > 360 && hue < hHue - 360);
            if (flag && qc.hslSaturation() >= satTar - satBnd && qc.saturation() <= satTar + satBnd && qc.lightness() >= litTar - litBnd && qc.lightness() <= litTar + litBnd)
                line[i] = line2[i];
        }
    }
    ui->img->setPixmap(QPixmap::fromImage(proDisp));
}

void TargetTool::on_buttonBox_accepted() {
    processed = qi->copy();
    preprocess();
    for (int j = 0; j < processed2.height(); ++j) {
        QRgb *line = reinterpret_cast<QRgb *>(processed.scanLine(j));
        QRgb *line2 = reinterpret_cast<QRgb *>(processed2.scanLine(j));
        for (int i = 0; i < processed2.width(); ++i) {
            QColor qc(line[i]);
            int lHue = hueTar - hueBnd;
            int hHue = hueTar + hueBnd;
            int hue = qc.hslHue() + 1;
            bool flag = hue >= hueTar - hueBnd && hue <= hueTar + hueBnd;
            if (!flag)
                flag = (lHue < 0 && hue >= lHue + 360) || (hHue > 360 && hue < hHue - 360);
            if (flag && qc.hslSaturation() >= satTar - satBnd && qc.saturation() <= satTar + satBnd && qc.lightness() >= litTar - litBnd && qc.lightness() <= litTar + litBnd)
                line[i] = line2[i];
        }
    }
    *qi = processed;
    done(1);
}

void TargetTool::on_buttonBox_rejected() {
    done(0);
}

