#ifndef UI_TARGETDIALOG_H
#define UI_TARGETDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QComboBox>

QT_BEGIN_NAMESPACE

class Ui_TargetDialog
{
public:
    QVBoxLayout *mainVert, *secVertL, *secVertR;
    QHBoxLayout *mainHori, *bBox, *secHori;
    QHBoxLayout *imgHori, *hueTarHori, *hueBndHori, *satTarHori, *satBndHori, *litTarHori, *litBndHori, *opHori;
    QLabel *img, *hueTar, *hueBnd, *satTar, *satBnd, *litTar, *litBnd, *opLb;
    QSpinBox *hueTarSB, *hueBndSB, *satTarSB, *satBndSB, *litTarSB, *litBndSB, *opSB;
    QSlider *hueTarS, *hueBndS, *satTarS, *satBndS, *litTarS, *litBndS, *opS;
    QComboBox *op, *opType;
    QDialogButtonBox *buttonBox;
    QSpacerItem *spacer;

    void setupUi(QWidget *targetdialog)
    {
        if (targetdialog->objectName().isEmpty())
            targetdialog->setObjectName(QString::fromUtf8("targetdialog"));
        targetdialog->resize(282, 244);
        mainHori = new QHBoxLayout(targetdialog);
        mainHori->setObjectName(QString::fromUtf8("mainHori"));
        mainVert = new QVBoxLayout();
        mainVert->setObjectName(QString::fromUtf8("mainVert"));
        mainHori->addLayout(mainVert);
        img = new QLabel(targetdialog);
        img->setObjectName(QString::fromUtf8("img"));
        img->setCursor(QCursor(Qt::CrossCursor));
        img->setMouseTracking(true);
        imgHori = new QHBoxLayout();
        imgHori->setObjectName(QString::fromUtf8("imgHori"));
        imgHori->addWidget(img);
        mainVert->addLayout(imgHori);
        secHori = new QHBoxLayout();
        secHori->setObjectName(QString::fromUtf8("secHori"));
        mainVert->addLayout(secHori);
        secVertL = new QVBoxLayout();
        secVertL->setObjectName(QString::fromUtf8("secVertL"));
        secHori->addLayout(secVertL);
        secVertR = new QVBoxLayout();
        secVertR->setObjectName(QString::fromUtf8("secVertR"));
        secHori->addLayout(secVertR);

        hueTarHori = new QHBoxLayout();
        hueTarHori->setObjectName(QString::fromUtf8("hueTarHori"));
        hueTar = new QLabel(targetdialog);
        hueTar->setText("Target Hue");
        hueTar->setObjectName(QString::fromUtf8("hueTar"));
        secVertL->addWidget(hueTar);
        hueTarSB = new QSpinBox(targetdialog);
        hueTarSB->setObjectName(QString::fromUtf8("hueTarSB"));
        hueTarSB->setRange(0, 360);
        hueTarHori->addWidget(hueTarSB);
        hueTarS = new QSlider(targetdialog);
        hueTarS->setObjectName(QString::fromUtf8("hueTarS"));
        hueTarS->setRange(0, 360);
        hueTarS->setOrientation(Qt::Orientation::Horizontal);
        hueTarHori->addWidget(hueTarS);
        secVertR->addLayout(hueTarHori);

        hueBndHori = new QHBoxLayout();
        hueBndHori->setObjectName(QString::fromUtf8("hueBndHori"));
        hueBnd = new QLabel(targetdialog);
        hueBnd->setText("Target Bounds");
        hueBnd->setObjectName(QString::fromUtf8("hueBnd"));
        secVertL->addWidget(hueBnd);
        hueBndSB = new QSpinBox(targetdialog);
        hueBndSB->setObjectName(QString::fromUtf8("hueBndSB"));
        hueBndSB->setRange(0, 180);
        hueBndHori->addWidget(hueBndSB);
        hueBndS = new QSlider(targetdialog);
        hueBndS->setObjectName(QString::fromUtf8("hueBndS"));
        hueBndS->setRange(0, 180);
        hueBndS->setOrientation(Qt::Orientation::Horizontal);
        hueBndHori->addWidget(hueBndS);
        secVertR->addLayout(hueBndHori);

        satTarHori = new QHBoxLayout();
        satTarHori->setObjectName(QString::fromUtf8("satTarHori"));
        satTar = new QLabel(targetdialog);
        satTar->setText("Target Saturation");
        satTar->setObjectName(QString::fromUtf8("satTar"));
        secVertL->addWidget(satTar);
        satTarSB = new QSpinBox(targetdialog);
        satTarSB->setObjectName(QString::fromUtf8("satTarSB"));
        satTarSB->setRange(0, 255);
        satTarHori->addWidget(satTarSB);
        satTarS = new QSlider(targetdialog);
        satTarS->setObjectName(QString::fromUtf8("satTarS"));
        satTarS->setRange(0, 255);
        satTarS->setOrientation(Qt::Orientation::Horizontal);
        satTarHori->addWidget(satTarS);
        secVertR->addLayout(satTarHori);

        satBndHori = new QHBoxLayout();
        satBndHori->setObjectName(QString::fromUtf8("satBndHori"));
        satBnd = new QLabel(targetdialog);
        satBnd->setText("Target Bounds");
        satBnd->setObjectName(QString::fromUtf8("satBnd"));
        secVertL->addWidget(satBnd);
        satBndSB = new QSpinBox(targetdialog);
        satBndSB->setObjectName(QString::fromUtf8("satBndSB"));
        satBndSB->setRange(0, 255);
        satBndHori->addWidget(satBndSB);
        satBndS = new QSlider(targetdialog);
        satBndS->setObjectName(QString::fromUtf8("satBndS"));
        satBndS->setRange(0, 255);
        satBndS->setOrientation(Qt::Orientation::Horizontal);
        satBndHori->addWidget(satBndS);
        secVertR->addLayout(satBndHori);

        litTarHori = new QHBoxLayout();
        litTarHori->setObjectName(QString::fromUtf8("litTarHori"));
        litTar = new QLabel(targetdialog);
        litTar->setText("Target Luminance");
        litTar->setObjectName(QString::fromUtf8("litTar"));
        secVertL->addWidget(litTar);
        litTarSB = new QSpinBox(targetdialog);
        litTarSB->setObjectName(QString::fromUtf8("litTarSB"));
        litTarSB->setRange(0, 255);
        litTarHori->addWidget(litTarSB);
        litTarS = new QSlider(targetdialog);
        litTarS->setObjectName(QString::fromUtf8("litTarS"));
        litTarS->setRange(0, 255);
        litTarS->setOrientation(Qt::Orientation::Horizontal);
        litTarHori->addWidget(litTarS);
        secVertR->addLayout(litTarHori);

        litBndHori = new QHBoxLayout();
        litBndHori->setObjectName(QString::fromUtf8("litBndHori"));
        litBnd = new QLabel(targetdialog);
        litBnd->setText("Target Bounds");
        litBnd->setObjectName(QString::fromUtf8("litBnd"));
        secVertL->addWidget(litBnd);
        litBndSB = new QSpinBox(targetdialog);
        litBndSB->setObjectName(QString::fromUtf8("litBndSB"));
        litBndSB->setRange(0, 255);
        litBndHori->addWidget(litBndSB);
        litBndS = new QSlider(targetdialog);
        litBndS->setObjectName(QString::fromUtf8("litBndS"));
        litBndS->setRange(0, 255);
        litBndS->setOrientation(Qt::Orientation::Horizontal);
        litBndHori->addWidget(litBndS);
        secVertR->addLayout(litBndHori);

        opHori = new QHBoxLayout();
        opHori->setObjectName(QString::fromUtf8("opHori"));
        opLb = new QLabel(targetdialog);
        opLb->setText("Operation");
        opLb->setObjectName(QString::fromUtf8("opLb"));
        secVertL->addWidget(opLb);
        op = new QComboBox(targetdialog);
        QStringList qsl({"Opacity", "Blur", "Hue Shift", "Contrast Adjustment", "Saturation Adjustment", "Luminance Adjustment", "Gamma Adjustment", "Filter", "Dither"});
        op->addItems(qsl);
        op->setObjectName(QString::fromUtf8("op"));
        opHori->addWidget(op);
        opType = new QComboBox(targetdialog);
        opType->setObjectName(QString::fromUtf8("opType"));
        opHori->addWidget(opType);
        opSB = new QSpinBox(targetdialog);
        opSB->setObjectName(QString::fromUtf8("opSB"));
        opSB->setRange(0, 255);
        opHori->addWidget(opSB);
        opS = new QSlider(targetdialog);
        opS->setObjectName(QString::fromUtf8("opS"));
        opS->setRange(0, 255);
        opS->setOrientation(Qt::Orientation::Horizontal);
        opHori->addWidget(opS);
        secVertR->addLayout(opHori);

        bBox = new QHBoxLayout();
        bBox->setObjectName(QString::fromUtf8("spacer"));
        spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        bBox->addSpacerItem(spacer);
        buttonBox = new QDialogButtonBox(targetdialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        bBox->addWidget(buttonBox);
        mainVert->addLayout(bBox);

        op->setCurrentIndex(8);
        opType->addItems(QStringList({"Bayer", "Floyd Steinberg", "Sierra", "Sierra Lite", "Random", "Palette Reduction"}));

        retranslateUi(targetdialog);

        QMetaObject::connectSlotsByName(targetdialog);
    } // setupUi

    void retranslateUi(QWidget *targetdialog)
    {
        targetdialog->setWindowTitle(QCoreApplication::translate("targetdialog", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TargetTool: public Ui_TargetDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TARGETDIALOG_H
