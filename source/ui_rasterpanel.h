#ifndef UI_RASTERPANEL_H
#define UI_RASTERPANEL_H
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
#include <QtWidgets/QPushButton>

#include <stdfuncs.h>
#include <graphics.h>

QT_BEGIN_NAMESPACE

class Ui_RasterPanel
{
public:
    QVBoxLayout *mainVert;

    QHBoxLayout *colorDesc1, *colorBox1;
    QPushButton *colorAdv1;
    QLabel *colorLb1, *color1;
    QSpinBox *colorSB1;
    QSlider *colorS1;

    QHBoxLayout *otherColor1Desc, *otherColor1Box;
    QLabel *otherColor1Lb;
    QPushButton *takeColorPB, *transFillPB;

    QHBoxLayout *mWandDesc, *mWandBox;
    QLabel *mWandLb;
    QPushButton *mWandPB, *mWandSetPB;

    QHBoxLayout *flipDesc, *flipBox;
    QLabel *flipLb;
    QPushButton *hFlipPB, *vFlipPB;

    QHBoxLayout *adjDesc, *adj1Box, *adj2Box, *adj3Box;
    QLabel *adjLb;
    QPushButton *briPB, *conPB, *gamPB, *satPB, *huePB, *tarPB;

    QHBoxLayout *filtConvDesc, *filtConvBox;
    QLabel *filtConvLb;
    QPushButton *filtPB, *convPB;

    QHBoxLayout *otherColor2Desc, *otherColor2Box;
    QLabel *otherColor2Lb;
    QPushButton *ditherPB, *cTransPB;

    void setupUi(QWidget *rasterPanel)
    {
        if (rasterPanel->objectName().isEmpty())
            rasterPanel->setObjectName("rasterPanel");
        mainVert = new QVBoxLayout(rasterPanel);
        mainVert->setObjectName("mainVert");
        rasterPanel->setLayout(mainVert);
        rasterPanel->setMaximumWidth(275);

        colorDesc1 = new QHBoxLayout();
        colorDesc1->setObjectName("colorDesc");
        colorLb1 = new QLabel(rasterPanel);
        colorLb1->setObjectName("colorLb");
        colorLb1->setText("Fill Color");
        colorDesc1->addWidget(colorLb1);
        colorDesc1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        colorBox1 = new QHBoxLayout();
        mainVert->addLayout(colorDesc1);
        colorSB1 = new QSpinBox(rasterPanel);
        colorSB1->setRange(0, 360);
        colorSB1->setObjectName("colorSB");
        colorSB1->setValue(0);
        colorBox1->addWidget(colorSB1);
        colorS1 = new QSlider(rasterPanel);
        colorS1->setRange(0, 360);
        colorS1->setObjectName("colorS");
        colorS1->setValue(0);
        colorS1->setOrientation(Qt::Horizontal);
        colorS1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        colorBox1->addWidget(colorS1);
        colorAdv1 = new QPushButton(rasterPanel);
        colorAdv1->setText("Advanced");
        colorAdv1->setObjectName("colorAdv");
        colorBox1->addWidget(colorAdv1);
        mainVert->addLayout(colorBox1);
        QImage img1(256, 256, QImage::Format_ARGB32_Premultiplied);
        QColor qc1;
        unsigned long time1 = stdFuncs::getTime();
        for (short y = 0; y <= 255; ++y) {
            QRgb *line = reinterpret_cast<QRgb *>(img1.scanLine(y));
            for (short x = 0; x <= 255; ++x) {
                qc1.setHsl(-1, 255 - y, x);
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
            if (Y < 0 || Y > 255)
                continue;
            for (int x = y == cSize ? 0 : cSize; x < 2 * cSize + 1; x += y == cSize ? 1 : 2 * cSize + 1) {
                int X = qc1.lightness() + (x - cSize);
                if ((y == cSize && x == cSize) || X < 0 || X > 255)
                    continue;
                QColor temp;
                temp.setHsl(hue1, 2 * Y, 255 - X);
                img1.setPixel(X, 255 - Y, temp.rgba());
            }
        }
        color1 = new QLabel(rasterPanel);
        color1->setPixmap(QPixmap::fromImage(img1));
        cout << stdFuncs::getTime(time1) << endl;
        color1->setCursor(Qt::CrossCursor);
        color1->setMouseTracking(true);
        mainVert->addWidget(color1);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        otherColor1Desc = new QHBoxLayout();
        otherColor1Desc->setObjectName("otherColor1Desc");
        otherColor1Lb = new QLabel(rasterPanel);
        otherColor1Lb->setObjectName("otherColor1Lb");
        otherColor1Lb->setText("Other Fill Options");
        otherColor1Desc->addWidget(otherColor1Lb);
        otherColor1Desc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(otherColor1Desc);
        otherColor1Box = new QHBoxLayout();
        takeColorPB = new QPushButton(rasterPanel);
        takeColorPB->setObjectName("takeColorPB");
        takeColorPB->setText("Take Color");
        otherColor1Box->addWidget(takeColorPB);
        transFillPB = new QPushButton(rasterPanel);
        transFillPB->setObjectName("transFillPB");
        transFillPB->setText("Transparency Fill");
        otherColor1Box->addWidget(transFillPB);
        mainVert->addLayout(otherColor1Box);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        mWandDesc = new QHBoxLayout();
        mWandDesc->setObjectName("mWandDesc");
        mWandLb = new QLabel(rasterPanel);
        mWandLb->setObjectName("mWandLb");
        mWandLb->setText("Magic Wand");
        mWandDesc->addWidget(mWandLb);
        mWandDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(mWandDesc);
        mWandBox = new QHBoxLayout();
        mWandPB = new QPushButton(rasterPanel);
        mWandPB->setObjectName("mWandPB");
        mWandPB->setText("Magic Wand");
        mWandBox->addWidget(mWandPB);
        mWandSetPB = new QPushButton(rasterPanel);
        mWandSetPB->setObjectName("mWandSetPB");
        mWandSetPB->setText("Wand Settings");
        mWandBox->addWidget(mWandSetPB);
        mainVert->addLayout(mWandBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        flipDesc = new QHBoxLayout();
        flipDesc->setObjectName("flipDesc");
        flipLb = new QLabel(rasterPanel);
        flipLb->setObjectName("flipLb");
        flipLb->setText("Flip Raster");
        flipDesc->addWidget(flipLb);
        flipDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(flipDesc);
        flipBox = new QHBoxLayout();
        hFlipPB = new QPushButton(rasterPanel);
        hFlipPB->setObjectName("hFlipPB");
        hFlipPB->setText("Horizontal Flip");
        flipBox->addWidget(hFlipPB);
        vFlipPB = new QPushButton(rasterPanel);
        vFlipPB->setObjectName("vFlipPB");
        vFlipPB->setText("Vertical Flip");
        flipBox->addWidget(vFlipPB);
        mainVert->addLayout(flipBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        adjDesc = new QHBoxLayout();
        adjDesc->setObjectName("adjDesc");
        adjLb = new QLabel(rasterPanel);
        adjLb->setObjectName("adjLb");
        adjLb->setText("Adjustments");
        adjDesc->addWidget(adjLb);
        adjDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(adjDesc);
        adj1Box = new QHBoxLayout();
        briPB = new QPushButton(rasterPanel);
        briPB->setObjectName("briPB");
        briPB->setText("Brightness");
        adj1Box->addWidget(briPB);
        conPB = new QPushButton(rasterPanel);
        conPB->setObjectName("conPB");
        conPB->setText("Contrast");
        adj1Box->addWidget(conPB);
        mainVert->addLayout(adj1Box);
        adj1Box = new QHBoxLayout();
        gamPB = new QPushButton(rasterPanel);
        gamPB->setObjectName("gamPB");
        gamPB->setText("Horizontal Flip");
        adj1Box->addWidget(gamPB);
        satPB = new QPushButton(rasterPanel);
        satPB->setObjectName("satPB");
        satPB->setText("Saturation");
        adj1Box->addWidget(satPB);
        mainVert->addLayout(adj1Box);
        adj1Box = new QHBoxLayout();
        huePB = new QPushButton(rasterPanel);
        huePB->setObjectName("huePB");
        huePB->setText("Hue");
        adj1Box->addWidget(huePB);
        tarPB = new QPushButton(rasterPanel);
        tarPB->setObjectName("tarPB");
        tarPB->setText("Target Tool");
        adj1Box->addWidget(tarPB);
        mainVert->addLayout(adj1Box);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        filtConvDesc = new QHBoxLayout();
        filtConvDesc->setObjectName("filtConvDesc");
        filtConvLb = new QLabel(rasterPanel);
        filtConvLb->setObjectName("filtConvLb");
        filtConvLb->setText("Filtering and Convolution");
        filtConvDesc->addWidget(filtConvLb);
        filtConvDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(filtConvDesc);
        filtConvBox = new QHBoxLayout();
        filtPB = new QPushButton(rasterPanel);
        filtPB->setObjectName("filtPB");
        filtPB->setText("Filter");
        filtConvBox->addWidget(filtPB);
        convPB = new QPushButton(rasterPanel);
        convPB->setObjectName("convPB");
        convPB->setText("Convolve");
        filtConvBox->addWidget(convPB);
        mainVert->addLayout(filtConvBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        otherColor2Desc = new QHBoxLayout();
        otherColor2Desc->setObjectName("otherColor2Desc");
        otherColor2Lb = new QLabel(rasterPanel);
        otherColor2Lb->setObjectName("otherColor2Lb");
        otherColor2Lb->setText("Other Color Operations");
        otherColor2Desc->addWidget(otherColor2Lb);
        otherColor2Desc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(otherColor2Desc);
        otherColor2Box = new QHBoxLayout();
        ditherPB = new QPushButton(rasterPanel);
        ditherPB->setObjectName("ditherPB");
        ditherPB->setText("Dither");
        otherColor2Box->addWidget(ditherPB);
        cTransPB = new QPushButton(rasterPanel);
        cTransPB->setObjectName("cTransPB");
        cTransPB->setText("Color Transfer");
        otherColor2Box->addWidget(cTransPB);
        mainVert->addLayout(otherColor2Box);






        mainVert->addSpacerItem(new QSpacerItem(275, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

        retranslateUi(rasterPanel);

        QMetaObject::connectSlotsByName(rasterPanel);
    } // setupUi

    void retranslateUi(QWidget *brightnessAdj)
    {
        brightnessAdj->setWindowTitle(QCoreApplication::translate("Poly Settings", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RasterPanel: public Ui_RasterPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RASTERPANEL_H
