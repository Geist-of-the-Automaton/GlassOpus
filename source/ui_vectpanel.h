#ifndef UI_VECTPANEL_H
#define UI_VECTPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

#include <ui_symdialog.h>
#include <stdfuncs.h>
#include <graphics.h>
#include <splinevector.h>

QT_BEGIN_NAMESPACE

class Ui_VectPanel
{
public:
    QVBoxLayout *mainVert;

    QHBoxLayout *modeFilterDesc, *modeFilterBox;
    QLabel *modeLb, *filterLb;
    QComboBox *modeCB, *filterCB;

    QHBoxLayout *strengthDesc, *strengthBox;
    QLabel *strengthLb;
    QSpinBox *strengthSB;
    QSlider *strengthS;

    QHBoxLayout *colorDesc1, *colorBox1;
    QPushButton *colorAdv1;
    QLabel *colorLb1, *color1;
    QSpinBox *colorSB1;
    QSlider *colorS1;

    QHBoxLayout *colorDesc2, *colorBox2;
    QPushButton *colorAdv2;
    QLabel *colorLb2, *color2;
    QSpinBox *colorSB2;
    QSlider *colorS2;

    QHBoxLayout *taperDesc, *taperBox1, *taperBox2;
    QLabel *taperLb1, *taperLb2;
    QSpinBox *taperSB, *taperSb2;
    QSlider *taperS1, *taperS2;

    QHBoxLayout *gapDesc, *gapBox;
    QLabel *gapLb;
    QSpinBox *gapSB;
    QSlider *gapS;

    QHBoxLayout *bandDesc, *bandBox;
    QLabel *bandLb;
    QSpinBox *bandSB;
    QSlider *bandS;

    QHBoxLayout *symDesc, *symBox;
    QLabel *symLb;
    QComboBox *sym1, *sym2, *sym3;

    QHBoxLayout *otherDesc, *otherBox;
    QLabel *otherLb;
    QPushButton *cSwapPB, *tSwapPB;

    void setupUi(QWidget *vectPanel)
    {
        if (vectPanel->objectName().isEmpty())
            vectPanel->setObjectName("vectPanel");
        mainVert = new QVBoxLayout(vectPanel);
        mainVert->setObjectName("mainVert");
        vectPanel->setLayout(mainVert);
        vectPanel->setMaximumWidth(275);
        modeFilterDesc = new QHBoxLayout();
        modeFilterDesc->setObjectName("modeFilterDesc");
        modeLb = new QLabel(vectPanel);
        modeLb->setObjectName("modeLb");
        modeLb->setText("Mode");
        modeFilterDesc->addWidget(modeLb);
        modeFilterDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        filterLb = new QLabel(vectPanel);
        filterLb->setObjectName("filterLb");
        filterLb->setText("Filter");
        modeFilterDesc->addWidget(filterLb);
        modeFilterDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(modeFilterDesc);

        modeFilterBox = new QHBoxLayout();
        modeFilterBox->setObjectName("modeFilterBox");
        modeCB = new QComboBox(vectPanel);
        modeCB->addItems({"Color Vector", "Filter Vector"});
        modeCB->setObjectName("mode");
        modeFilterBox->addWidget(modeCB);
        filterCB = new QComboBox(vectPanel);
        for (string s : graphics::vectorFilters)
            filterCB->addItem(s.c_str());
        filterCB->setObjectName("filter");
        modeFilterBox->addWidget(filterCB);
        mainVert->addLayout(modeFilterBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        strengthDesc = new QHBoxLayout();
        strengthDesc->setObjectName("strengthDesc");
        strengthLb = new QLabel(vectPanel);
        strengthLb->setObjectName("strengthLb");
        strengthLb->setText("Filter Strength");
        strengthDesc->addWidget(strengthLb);
        strengthDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        strengthBox = new QHBoxLayout();
        mainVert->addLayout(strengthDesc);
        strengthSB = new QSpinBox(vectPanel);
        strengthSB->setRange(0, 1);
        strengthSB->setObjectName("strengthSB");
        strengthSB->setValue(255);
        strengthBox->addWidget(strengthSB);
        strengthS = new QSlider(vectPanel);
        strengthS->setRange(0, 1);
        strengthS->setObjectName("strengthS");
        strengthS->setValue(255);
        strengthS->setOrientation(Qt::Horizontal);
        strengthBox->addWidget(strengthS);
        mainVert->addLayout(strengthBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        colorDesc1 = new QHBoxLayout();
        colorDesc1->setObjectName("colorDesc");
        colorLb1 = new QLabel(vectPanel);
        colorLb1->setObjectName("colorLb");
        colorLb1->setText("Vector Colors");
        colorDesc1->addWidget(colorLb1);
        colorDesc1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        colorBox1 = new QHBoxLayout();
        mainVert->addLayout(colorDesc1);
        colorSB1 = new QSpinBox(vectPanel);
        colorSB1->setRange(0, 360);
        colorSB1->setObjectName("colorSB");
        colorSB1->setValue(120);
        colorBox1->addWidget(colorSB1);
        colorS1 = new QSlider(vectPanel);
        colorS1->setRange(0, 360);
        colorS1->setObjectName("colorS");
        colorS1->setValue(120);
        colorS1->setOrientation(Qt::Horizontal);
        colorS1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        colorBox1->addWidget(colorS1);
        colorAdv1 = new QPushButton(vectPanel);
        colorAdv1->setText("Advanced");
        colorAdv1->setObjectName("colorAdv");
        colorBox1->addWidget(colorAdv1);
        mainVert->addLayout(colorBox1);
        QImage img1(256, 128, QImage::Format_ARGB32_Premultiplied);
        QColor qc1;
        unsigned long time1 = stdFuncs::getTime();
        for (short y = 0; y <= 127; ++y) {
            QRgb *line = reinterpret_cast<QRgb *>(img1.scanLine(y));
            for (short x = 0; x <= 255; ++x) {
                qc1.setHsl(45, 255 - 2 * y, x);
                line[x] = qc1.rgba();
            }
        }
        qc1.setHsl(45, 0, 0);
        int hue1 = (qc1.hslHue() + 1) + 180;
        if (hue1 > 360)
            hue1 -= 360;
        --hue1;
        int cSize = 3;
        for (int y = 0; y < 2 * cSize + 1; ++y) {
            int Y = qc1.hslSaturation() + (y - cSize);
            if (Y < 0 || Y > 127)
                continue;
            for (int x = y == cSize ? 0 : cSize; x < 2 * cSize + 1; x += y == cSize ? 1 : 2 * cSize + 1) {
                int X = qc1.lightness() + (x - cSize);
                if ((y == cSize && x == cSize) || X < 0 || X > 255)
                    continue;
                QColor temp;
                temp.setHsl(hue1, 2 * Y, 255 - X);
                img1.setPixel(X, 127 - Y, temp.rgba());
            }
        }
        color1 = new QLabel(vectPanel);
        color1->setPixmap(QPixmap::fromImage(img1));
        cout << stdFuncs::getTime(time1) << endl;
        color1->setCursor(Qt::CrossCursor);
        color1->setMouseTracking(true);
        mainVert->addWidget(color1);
        colorBox2 = new QHBoxLayout();
        colorSB2 = new QSpinBox(vectPanel);
        colorSB2->setRange(0, 360);
        colorSB2->setObjectName("colorSB");
        colorSB2->setValue(120);
        colorBox2->addWidget(colorSB2);
        colorS2 = new QSlider(vectPanel);
        colorS2->setRange(0, 360);
        colorS2->setObjectName("colorS");
        colorS2->setValue(120);
        colorS2->setOrientation(Qt::Horizontal);
        colorS2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        colorBox2->addWidget(colorS2);
        colorAdv2 = new QPushButton(vectPanel);
        colorAdv2->setText("Advanced");
        colorAdv2->setObjectName("colorAdv");
        colorBox2->addWidget(colorAdv2);
        mainVert->addLayout(colorBox2);
        QImage img(256, 128, QImage::Format_ARGB32_Premultiplied);
        QColor qc;
        unsigned long time2 = stdFuncs::getTime();
        for (short y = 0; y <= 127; ++y) {
            QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(y));
            for (short x = 0; x <= 255; ++x) {
                qc.setHsl(120, 255 - 2 * y, x);
                line[x] = qc.rgba();
            }
        }
        qc.setHsl(120, 0, 0);
        int hue = (qc.hslHue() + 1) + 180;
        if (hue > 360)
            hue -= 360;
        --hue;
        cSize = 3;
        for (int y = 0; y < 2 * cSize + 1; ++y) {
            int Y = qc.hslSaturation() + (y - cSize);
            if (Y < 0 || Y > 127)
                continue;
            for (int x = y == cSize ? 0 : cSize; x < 2 * cSize + 1; x += y == cSize ? 1 : 2 * cSize + 1) {
                int X = qc.lightness() + (x - cSize);
                if ((y == cSize && x == cSize) || X < 0 || X > 255)
                    continue;
                QColor temp;
                temp.setHsl(hue, 2 * Y, 255 - X);
                img.setPixel(X, 127 - Y, temp.rgba());
            }
        }
        color2 = new QLabel(vectPanel);
        color2->setPixmap(QPixmap::fromImage(img));
        cout << stdFuncs::getTime(time2) << endl;
        color2->setCursor(Qt::CrossCursor);
        color2->setMouseTracking(true);
        mainVert->addWidget(color2);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        taperDesc = new QHBoxLayout();
        taperDesc->setObjectName("taperDesc");
        taperLb1 = new QLabel(vectPanel);
        taperLb1->setObjectName("taperLb");
        taperLb1->setText("Vector Tapers");
        taperDesc->addWidget(taperLb1);
        taperDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        taperBox1 = new QHBoxLayout();
        mainVert->addLayout(taperDesc);
        taperSB = new QSpinBox(vectPanel);
        taperSB->setRange(minTaper, maxTaper);
        taperSB->setObjectName("taperSB1");
        taperSB->setValue(0);
        taperBox1->addWidget(taperSB);
        taperS1 = new QSlider(vectPanel);
        taperS1->setRange(minTaper, maxTaper);
        taperS1->setObjectName("taperS1");
        taperS1->setValue(0);
        taperS1->setOrientation(Qt::Horizontal);
        taperBox1->addWidget(taperS1);
        mainVert->addLayout(taperBox1);

        taperBox2 = new QHBoxLayout();
        taperSB = new QSpinBox(vectPanel);
        taperSB->setRange(minTaper, maxTaper);
        taperSB->setObjectName("taperSB2");
        taperSB->setValue(0);
        taperBox2->addWidget(taperSB);
        taperS2 = new QSlider(vectPanel);
        taperS2->setRange(minTaper, maxTaper);
        taperS2->setObjectName("taperS2");
        taperS2->setValue(0);
        taperS2->setOrientation(Qt::Horizontal);
        taperBox2->addWidget(taperS2);
        mainVert->addLayout(taperBox2);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        gapDesc = new QHBoxLayout();
        gapDesc->setObjectName("gapDesc");
        gapLb = new QLabel(vectPanel);
        gapLb->setObjectName("gapLb");
        gapLb->setText("Gap Size");
        gapDesc->addWidget(gapLb);
        gapDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        gapBox = new QHBoxLayout();
        mainVert->addLayout(gapDesc);
        gapSB = new QSpinBox(vectPanel);
        gapSB->setRange(0, maxStyle);
        gapSB->setObjectName("gapSB");
        gapSB->setValue(0);
        gapBox->addWidget(gapSB);
        gapS = new QSlider(vectPanel);
        gapS->setRange(0, maxStyle);
        gapS->setObjectName("gapS");
        gapS->setValue(0);
        gapS->setOrientation(Qt::Horizontal);
        gapBox->addWidget(gapS);
        mainVert->addLayout(gapBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        bandDesc = new QHBoxLayout();
        bandDesc->setObjectName("bandDesc");
        bandLb = new QLabel(vectPanel);
        bandLb->setObjectName("bandLb");
        bandLb->setText("Band Size");
        bandDesc->addWidget(bandLb);
        bandDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        bandBox = new QHBoxLayout();
        mainVert->addLayout(bandDesc);
        bandSB = new QSpinBox(vectPanel);
        bandSB->setRange(minStyle, maxStyle);
        bandSB->setObjectName("bandSB");
        bandSB->setValue(minStyle);
        bandBox->addWidget(bandSB);
        bandS = new QSlider(vectPanel);
        bandS->setRange(minStyle, maxStyle);
        bandS->setObjectName("bandS");
        bandS->setValue(minStyle);
        bandS->setOrientation(Qt::Horizontal);
        bandBox->addWidget(bandS);
        mainVert->addLayout(bandBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        symDesc = new QHBoxLayout();
        symDesc->setObjectName("symDesc");
        symLb = new QLabel(vectPanel);
        symLb->setObjectName("symLb");
        symLb->setText("Symmetry");
        symDesc->addWidget(symLb);
        symDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        symBox = new QHBoxLayout();
        mainVert->addLayout(symDesc);
        sym1 = new QComboBox(vectPanel);
        sym1->setObjectName(QString::fromUtf8("sym1"));
        for (int i = minSym; i <= maxSym; ++i)
            sym1->addItem((to_string(i) + " Divisions").c_str());
        symBox->addWidget(sym1);
        sym2 = new QComboBox(vectPanel);
        sym2->setObjectName(QString::fromUtf8("sym2"));
        sym2->addItem("Of Every 1");
        symBox->addWidget(sym2);
        sym3 = new QComboBox(vectPanel);
        sym3->setObjectName(QString::fromUtf8("csym3"));
        sym3->addItem("Skip 0");
        symBox->addWidget(sym3);
        mainVert->addLayout(symBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        otherDesc = new QHBoxLayout();
        otherDesc->setObjectName("otherDesc");
        otherLb = new QLabel(vectPanel);
        otherLb->setObjectName("otherLb");
        otherLb->setText("Other");
        otherDesc->addWidget(otherLb);
        otherDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(otherDesc);
        otherBox = new QHBoxLayout();
        cSwapPB = new QPushButton(vectPanel);
        cSwapPB->setObjectName("cSwapPB");
        cSwapPB->setText("Swap Colors");
        otherBox->addWidget(cSwapPB);
        tSwapPB = new QPushButton(vectPanel);
        tSwapPB->setObjectName("tSwapPB");
        tSwapPB->setText("Swap Tapers");
        otherBox->addWidget(tSwapPB);
        mainVert->addLayout(otherBox);






        mainVert->addSpacerItem(new QSpacerItem(275, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

        retranslateUi(vectPanel);

        QMetaObject::connectSlotsByName(vectPanel);
    } // setupUi

    void retranslateUi(QWidget *brightnessAdj)
    {
        brightnessAdj->setWindowTitle(QCoreApplication::translate("Vect Settings", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VectPanel: public Ui_VectPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VECTPANEL_H
