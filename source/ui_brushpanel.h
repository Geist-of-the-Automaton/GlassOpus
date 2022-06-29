#ifndef UI_BRUSHPANEL_H
#define UI_BRUSHPANEL_H

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

#include <brushhandler.h>
#include <ui_symdialog.h>
#include <stdfuncs.h>

QT_BEGIN_NAMESPACE

class Ui_BrushPanel
{
public:
    QVBoxLayout *mainVert;

    QHBoxLayout *methodShapeDesc, *methodShapeBox;
    QLabel *methodLb, *shapeLb;
    QComboBox *methodCB, *shapeCB;

    QHBoxLayout *radiusDesc, *radiusBox;
    QLabel *radiusLb;
    QSpinBox *radiusSB;
    QSlider *radiusS;

    QHBoxLayout *strengthDesc, *strengthBox;
    QLabel *strengthLb;
    QSpinBox *strengthSB;
    QSlider *strengthS;

    QHBoxLayout *colorDesc, *colorBox;
    QPushButton *colorAdv;
    QLabel *colorLb, *color;
    QSpinBox *colorSB;
    QSlider *colorS;

    QHBoxLayout *filterDesc;
    QLabel *filterLb;
    QComboBox *filterCB;

    QHBoxLayout *sprayDesc, *sprayBox;
    QLabel *sprayLb;
    QSpinBox *spraySB;
    QSlider *sprayS;

    QHBoxLayout *patternDesc, *patternBox;
    QLabel *patternLb;
    QRadioButton *patternRB;
    QPushButton *patternPB;

    QHBoxLayout *symBox;
    QPushButton *symPB;

    QHBoxLayout *otherDesc, *otherBox;
    QLabel *otherLb;
    QPushButton *shapePB, *radialPB;

    void setupUi(QWidget *brushPanel)
    {
        if (brushPanel->objectName().isEmpty())
            brushPanel->setObjectName("brushPanel");
        mainVert = new QVBoxLayout(brushPanel);
        mainVert->setObjectName("mainVert");
        brushPanel->setLayout(mainVert);
        brushPanel->setMaximumWidth(275);
        brushPanel->setMouseTracking(false);
        methodShapeDesc = new QHBoxLayout();
        methodShapeDesc->setObjectName("methodShapeDesc");
        //methodShapeDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        methodLb = new QLabel(brushPanel);
        methodLb->setObjectName("methodLb");
        methodLb->setText("Method");
        methodShapeDesc->addWidget(methodLb);
        methodShapeDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        shapeLb = new QLabel(brushPanel);
        shapeLb->setObjectName("shapeLb");
        shapeLb->setText("Shape");
        methodShapeDesc->addWidget(shapeLb);
        methodShapeDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(methodShapeDesc);

        methodShapeBox = new QHBoxLayout();
        methodShapeBox->setObjectName("methodShapeBox");
        methodCB = new QComboBox(brushPanel);
        for (string s : appMethods)
            methodCB->addItem(s.c_str());
        methodCB->setObjectName("methodCB");
        methodShapeBox->addWidget(methodCB);
        shapeCB = new QComboBox(brushPanel);
        for (string s : brushShapes)
            shapeCB->addItem(s.c_str());
        shapeCB->setObjectName("shapeCB");
        methodShapeBox->addWidget(shapeCB);
        mainVert->addLayout(methodShapeBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        filterDesc = new QHBoxLayout();
        filterDesc->setObjectName("filterDesc");
        filterLb = new QLabel(brushPanel);
        filterLb->setObjectName("filterLb");
        filterLb->setText("Brush Filter");
        filterDesc->addWidget(filterLb);
        filterDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(filterDesc);
        filterCB = new QComboBox(brushPanel);
        filterCB->setObjectName("filterCB");
        for (string s : graphics::filterNames)
            filterCB->addItem(s.c_str());
        mainVert->addWidget(filterCB);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        strengthDesc = new QHBoxLayout();
        strengthDesc->setObjectName("strengthDesc");
        strengthLb = new QLabel(brushPanel);
        strengthLb->setObjectName("strengthLb");
        strengthLb->setText("Brush Strength");
        strengthDesc->addWidget(strengthLb);
        strengthDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        strengthBox = new QHBoxLayout();
        mainVert->addLayout(strengthDesc);
        strengthSB = new QSpinBox(brushPanel);
        strengthSB->setRange(minStrength, maxStrength);
        strengthSB->setObjectName("strengthSB");
        strengthSB->setValue(255);
        strengthBox->addWidget(strengthSB);
        strengthS = new QSlider(brushPanel);
        strengthS->setRange(minStrength, maxStrength);
        strengthS->setObjectName("strengthS");
        strengthS->setValue(255);
        strengthS->setOrientation(Qt::Horizontal);
        strengthBox->addWidget(strengthS);
        mainVert->addLayout(strengthBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        colorDesc = new QHBoxLayout();
        colorDesc->setObjectName("colorDesc");
        colorLb = new QLabel(brushPanel);
        colorLb->setObjectName("colorLb");
        colorLb->setText("Brush Color");
        colorDesc->addWidget(colorLb);
        colorDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        colorBox = new QHBoxLayout();
        mainVert->addLayout(colorDesc);
        colorSB = new QSpinBox(brushPanel);
        colorSB->setRange(0, 360);
        colorSB->setObjectName("colorSB");
        colorSB->setValue(0);
        colorBox->addWidget(colorSB);
        colorS = new QSlider(brushPanel);
        colorS->setRange(0, 360);
        colorS->setObjectName("colorS");
        colorS->setValue(0);
        colorS->setOrientation(Qt::Horizontal);
        colorS->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        colorBox->addWidget(colorS);
        colorAdv = new QPushButton(brushPanel);
        colorAdv->setText("Advanced");
        colorAdv->setObjectName("colorAdv");
        colorBox->addWidget(colorAdv);
        mainVert->addLayout(colorBox);
        QImage img(256, 256, QImage::Format_ARGB32_Premultiplied);
        QColor qc;
        unsigned long time = stdFuncs::getTime();
        for (short y = 0; y <= 255; ++y) {
            QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(y));
            for (short x = 0; x <= 255; ++x) {
                qc.setHsl(-1, 255 - y, x);
                line[x] = qc.rgba();
            }
        }
        qc.setHsl(-1, 0, 0);
        int hue = (qc.hslHue() + 1) + 180;
        if (hue > 360)
            hue -= 360;
        --hue;
        int cSize = 3;
        for (int y = 0; y < 2 * cSize + 1; ++y) {
            int Y = qc.hslSaturation() + (y - cSize);
            if (Y < 0 || Y > 255)
                continue;
            for (int x = y == cSize ? 0 : cSize; x < 2 * cSize + 1; x += y == cSize ? 1 : 2 * cSize + 1) {
                int X = qc.lightness() + (x - cSize);
                if ((y == cSize && x == cSize) || X < 0 || X > 255)
                    continue;
                QColor temp;
                temp.setHsl(hue, Y, 255 - X);
                img.setPixel(X, 255 - Y, temp.rgba());
            }
        }
        color = new QLabel(brushPanel);
        color->setPixmap(QPixmap::fromImage(img));
        cout << stdFuncs::getTime(time) << endl;
        color->setCursor(Qt::CrossCursor);
        color->setMouseTracking(true);
        mainVert->addWidget(color);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        radiusDesc = new QHBoxLayout();
        radiusDesc->setObjectName("radiusDesc");
        radiusLb = new QLabel(brushPanel);
        radiusLb->setObjectName("radiusLb");
        radiusLb->setText("Brush Radius");
        radiusDesc->addWidget(radiusLb);
        radiusDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        radiusBox = new QHBoxLayout();
        mainVert->addLayout(radiusDesc);
        radiusSB = new QSpinBox(brushPanel);
        radiusSB->setRange(minRadius, maxRadius);
        radiusSB->setObjectName("radiusSB");
        radiusSB->setValue(10);
        radiusBox->addWidget(radiusSB);
        radiusS = new QSlider(brushPanel);
        radiusS->setRange(minRadius, maxRadius);
        radiusS->setObjectName("radiusS");
        radiusS->setValue(10);
        radiusS->setOrientation(Qt::Horizontal);
        radiusBox->addWidget(radiusS);
        mainVert->addLayout(radiusBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        sprayDesc = new QHBoxLayout();
        sprayDesc->setObjectName("sprayDesc");
        sprayLb = new QLabel(brushPanel);
        sprayLb->setObjectName("sprayLb");
        sprayLb->setText("Spray Density");
        sprayDesc->addWidget(sprayLb);
        sprayDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        sprayBox = new QHBoxLayout();
        mainVert->addLayout(sprayDesc);
        spraySB = new QSpinBox(brushPanel);
        spraySB->setRange(minDensity, maxDensity);
        spraySB->setObjectName("spraySB");
        spraySB->setValue(0);
        sprayBox->addWidget(spraySB);
        sprayS = new QSlider(brushPanel);
        sprayS->setRange(minDensity, maxDensity);
        sprayS->setObjectName("sprayS");
        sprayS->setValue(0);
        sprayS->setOrientation(Qt::Horizontal);
        sprayBox->addWidget(sprayS);
        mainVert->addLayout(sprayBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        patternDesc = new QHBoxLayout();
        patternDesc->setObjectName("patternDesc");
        patternLb = new QLabel(brushPanel);
        patternLb->setObjectName("patternLb");
        patternLb->setText("Patterning");
        patternDesc->addWidget(patternLb);
        patternDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        patternBox = new QHBoxLayout();
        mainVert->addLayout(patternDesc);
        patternRB = new QRadioButton(brushPanel);
        patternRB->setObjectName("patternRB");
        patternRB->setChecked(false);
        patternBox->addWidget(patternRB);
        patternPB = new QPushButton(brushPanel);
        patternPB->setText("Pattern Profiler");
        patternPB->setObjectName("patternPB");
        patternBox->addWidget(patternPB);
        mainVert->addLayout(patternBox);
        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        otherDesc = new QHBoxLayout();
        otherDesc->setObjectName("otherDesc");
        otherLb = new QLabel(brushPanel);
        otherLb->setObjectName("otherLb");
        otherLb->setText("Other");
        otherDesc->addWidget(otherLb);
        otherDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(otherDesc);
        otherBox = new QHBoxLayout();
        shapePB = new QPushButton(brushPanel);
        shapePB->setObjectName("shapePB");
        shapePB->setText("Shape Profiler");
        otherBox->addWidget(shapePB);
        radialPB = new QPushButton(brushPanel);
        radialPB->setObjectName("radialPB");
        radialPB->setText("Radial Profiler");
        otherBox->addWidget(radialPB);
        mainVert->addLayout(otherBox);

        symBox = new QHBoxLayout();
        symPB = new QPushButton(brushPanel);
        symPB->setObjectName(QString::fromUtf8("symPB"));
        symPB->setMinimumWidth(123);
        symPB->setText("Symmetry Tool");
        symBox->addWidget(symPB);
        symBox->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(symBox);






        mainVert->addSpacerItem(new QSpacerItem(275, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

        retranslateUi(brushPanel);

        QMetaObject::connectSlotsByName(brushPanel);
    } // setupUi

    void retranslateUi(QWidget *brightnessAdj)
    {
        brightnessAdj->setWindowTitle(QCoreApplication::translate("Brush Settings", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BrushPanel: public Ui_BrushPanel {};
} // namespace Ui

QT_END_NAMESPACE
#endif // UI_BRUSHPANEL_H
