#ifndef UI_POLYPANEL_H
#define UI_POLYPANEL_H

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

class Ui_PolyPanel
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

    QHBoxLayout *bandDesc, *bandBox;
    QLabel *bandLb;
    QSpinBox *bandSB;
    QSlider *bandS;

    QHBoxLayout *tViewDesc, *tViewBox;
    QLabel *tViewLb;
    QRadioButton *tViewRB;

    QHBoxLayout *otherDesc, *otherBox;
    QLabel *otherLb;
    QPushButton *cSwapPB, *rPtPB;

    void setupUi(QWidget *polyPanel)
    {
        if (polyPanel->objectName().isEmpty())
            polyPanel->setObjectName("polyPanel");
        mainVert = new QVBoxLayout(polyPanel);
        mainVert->setObjectName("mainVert");
        polyPanel->setLayout(mainVert);
        polyPanel->setMaximumWidth(275);
        modeFilterDesc = new QHBoxLayout();
        modeFilterDesc->setObjectName("modeFilterDesc");
        modeLb = new QLabel(polyPanel);
        modeLb->setObjectName("modeLb");
        modeLb->setText("Mode");
        modeFilterDesc->addWidget(modeLb);
        modeFilterDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        filterLb = new QLabel(polyPanel);
        filterLb->setObjectName("filterLb");
        filterLb->setText("Filter");
        modeFilterDesc->addWidget(filterLb);
        modeFilterDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(modeFilterDesc);

        modeFilterBox = new QHBoxLayout();
        modeFilterBox->setObjectName("modeFilterBox");
        modeCB = new QComboBox(polyPanel);
        modeCB->addItems({"Color Polygon", "Filter Polygon"});
        modeCB->setObjectName("modeCB");
        modeFilterBox->addWidget(modeCB);
        filterCB = new QComboBox(polyPanel);
        for (string s : graphics::vectorFilters)
            filterCB->addItem(s.c_str());
        filterCB->setObjectName("filterCB");
        modeFilterBox->addWidget(filterCB);
        mainVert->addLayout(modeFilterBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        strengthDesc = new QHBoxLayout();
        strengthDesc->setObjectName("strengthDesc");
        strengthLb = new QLabel(polyPanel);
        strengthLb->setObjectName("strengthLb");
        strengthLb->setText("Filter Strength");
        strengthDesc->addWidget(strengthLb);
        strengthDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        strengthBox = new QHBoxLayout();
        mainVert->addLayout(strengthDesc);
        strengthSB = new QSpinBox(polyPanel);
        strengthSB->setRange(0, 255);
        strengthSB->setObjectName("strengthSB");
        strengthSB->setValue(255);
        strengthSB->setSingleStep(1);
        strengthBox->addWidget(strengthSB);
        strengthS = new QSlider(polyPanel);
        strengthS->setRange(0, 255);
        strengthS->setObjectName("strengthS");
        strengthS->setValue(255);
        strengthS->setOrientation(Qt::Horizontal);
        strengthS->setTickInterval(1);
        strengthBox->addWidget(strengthS);
        mainVert->addLayout(strengthBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        colorDesc1 = new QHBoxLayout();
        colorDesc1->setObjectName("colorDesc");
        colorLb1 = new QLabel(polyPanel);
        colorLb1->setObjectName("colorLb");
        colorLb1->setText("Polygon Colors");
        colorDesc1->addWidget(colorLb1);
        colorDesc1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        colorBox1 = new QHBoxLayout();
        mainVert->addLayout(colorDesc1);
        colorSB1 = new QSpinBox(polyPanel);
        colorSB1->setRange(0, 360);
        colorSB1->setObjectName("colorSB1");
        colorSB1->setValue(0);
        colorSB1->setSingleStep(1);
        colorBox1->addWidget(colorSB1);
        colorS1 = new QSlider(polyPanel);
        colorS1->setRange(0, 360);
        colorS1->setObjectName("colorS1");
        colorS1->setValue(0);
        colorS1->setOrientation(Qt::Horizontal);
        colorS1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        colorS1->setSingleStep(1);
        colorBox1->addWidget(colorS1);
        colorAdv1 = new QPushButton(polyPanel);
        colorAdv1->setText("Advanced");
        colorAdv1->setObjectName("colorAdv");
        colorBox1->addWidget(colorAdv1);
        mainVert->addLayout(colorBox1);
        QImage img1(256, 128, QImage::Format_ARGB32_Premultiplied);
        QColor qc1;
        for (short y = 0; y <= 127; ++y) {
            QRgb *line = reinterpret_cast<QRgb *>(img1.scanLine(y));
            for (short x = 0; x <= 255; ++x) {
                qc1.setHsl(-1, 255 - 2 * y, x);
                line[x] = qc1.rgba();
            }
        }
        qc1.setHsl(-1, 0, 0);
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
        color1 = new QLabel(polyPanel);
        color1->setPixmap(QPixmap::fromImage(img1));
        color1->setCursor(Qt::CrossCursor);
        color1->setMouseTracking(true);
        mainVert->addWidget(color1);
        colorBox2 = new QHBoxLayout();
        colorSB2 = new QSpinBox(polyPanel);
        colorSB2->setRange(0, 360);
        colorSB2->setObjectName("colorSB2");
        colorSB2->setValue(0);
        colorSB2->setSingleStep(1);
        colorBox2->addWidget(colorSB2);
        colorS2 = new QSlider(polyPanel);
        colorS2->setRange(0, 360);
        colorS2->setObjectName("colorS2");
        colorS2->setValue(0);
        colorS2->setOrientation(Qt::Horizontal);
        colorS2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        colorS2->setSingleStep(1);
        colorBox2->addWidget(colorS2);
        colorAdv2 = new QPushButton(polyPanel);
        colorAdv2->setText("Advanced");
        colorAdv2->setObjectName("colorAdv");
        colorBox2->addWidget(colorAdv2);
        mainVert->addLayout(colorBox2);
        QImage img(256, 128, QImage::Format_ARGB32_Premultiplied);
        QColor qc;
        for (short y = 0; y <= 127; ++y) {
            QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(y));
            for (short x = 0; x <= 255; ++x) {
                qc.setHsl(-1, 255 - 2 * y, x);
                line[x] = qc.rgba();
            }
        }
        qc.setHsl(-1, 0, 0);
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
        color2 = new QLabel(polyPanel);
        color2->setPixmap(QPixmap::fromImage(img));
        color2->setCursor(Qt::CrossCursor);
        color2->setMouseTracking(true);
        mainVert->addWidget(color2);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        bandDesc = new QHBoxLayout();
        bandDesc->setObjectName("bandDesc");
        bandLb = new QLabel(polyPanel);
        bandLb->setObjectName("bandLb");
        bandLb->setText("Edge Size");
        bandDesc->addWidget(bandLb);
        bandDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        bandBox = new QHBoxLayout();
        mainVert->addLayout(bandDesc);
        bandSB = new QSpinBox(polyPanel);
        bandSB->setRange(0, maxWidth);
        bandSB->setObjectName("bandSB");
        bandSB->setValue(0);
        bandSB->setSingleStep(1);
        bandBox->addWidget(bandSB);
        bandS = new QSlider(polyPanel);
        bandS->setRange(0, maxWidth);
        bandS->setObjectName("bandS");
        bandS->setValue(0);
        bandS->setOrientation(Qt::Horizontal);
        bandS->setTickInterval(1);
        bandBox->addWidget(bandS);
        mainVert->addLayout(bandBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        tViewDesc = new QHBoxLayout();
        tViewDesc->setObjectName("tViewDesc");
        tViewLb = new QLabel(polyPanel);
        tViewLb->setObjectName("tViewLb");
        tViewLb->setText("View Triangles");
        tViewDesc->addWidget(tViewLb);
        tViewDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        tViewBox = new QHBoxLayout();
        mainVert->addLayout(tViewDesc);
        tViewRB = new QRadioButton(polyPanel);
        tViewRB->setObjectName("tViewRB");
        tViewRB->setChecked(false);
        tViewBox->addWidget(tViewRB);
        mainVert->addLayout(tViewBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        otherDesc = new QHBoxLayout();
        otherDesc->setObjectName("otherDesc");
        otherLb = new QLabel(polyPanel);
        otherLb->setObjectName("otherLb");
        otherLb->setText("Other");
        otherDesc->addWidget(otherLb);
        otherDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(otherDesc);
        otherBox = new QHBoxLayout();
        cSwapPB = new QPushButton(polyPanel);
        cSwapPB->setObjectName("cSwapPB");
        cSwapPB->setText("Swap Colors");
        otherBox->addWidget(cSwapPB);
        rPtPB = new QPushButton(polyPanel);
        rPtPB->setObjectName("rPtPB");
        rPtPB->setText("Reduce Points");
        otherBox->addWidget(rPtPB);
        mainVert->addLayout(otherBox);






        mainVert->addSpacerItem(new QSpacerItem(275, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

        retranslateUi(polyPanel);

        QMetaObject::connectSlotsByName(polyPanel);
    } // setupUi

    void retranslateUi(QWidget *brightnessAdj)
    {
        brightnessAdj->setWindowTitle(QCoreApplication::translate("Poly Settings", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PolyPanel: public Ui_PolyPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POLYPANEL_H
