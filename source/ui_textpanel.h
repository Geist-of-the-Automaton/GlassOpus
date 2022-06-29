#ifndef UI_TEXTPANEL_H
#define UI_TEXTPANEL_H
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
#include <QtWidgets/QPlainTextEdit>
#include <QFontDatabase>

#include <stdfuncs.h>

QT_BEGIN_NAMESPACE

class Ui_TextPanel
{
public:
    QVBoxLayout *mainVert;

    QHBoxLayout *contentDesc;
    QLabel *contentLb;
    QPlainTextEdit *content;

    QHBoxLayout *fontDesc;
    QLabel *fontLb;
    QComboBox *fonts;

    QHBoxLayout *sizeDesc, *sizeBox;
    QLabel *sizeLb;
    QSpinBox *sizeSB;
    QSlider *sizeS;

    QHBoxLayout *colorDesc1, *colorBox1;
    QPushButton *colorAdv1;
    QLabel *colorLb1, *color1;
    QSpinBox *colorSB1;
    QSlider *colorS1;

    QHBoxLayout *tawDesc, *tawBox;
    QLabel *tawLb;
    QSpinBox *tawSB;
    QSlider *tawS;

    QHBoxLayout *wordDesc, *wordBox;
    QLabel *wordLb;
    QSpinBox *wordSB;
    QSlider *wordS;

    QHBoxLayout *letterDesc, *letterBox;
    QLabel *letterLb;
    QSpinBox *letterSB;
    QSlider *letterS;

    QHBoxLayout *mainStyle;
    QVBoxLayout *style1v, *style2v, *style3v;
    QLabel *boldLb, *italLb, *undrLb, *overLb, *strkLb;
    QRadioButton *bold, *ital, *undr, *over, *strk;

    void setupUi(QWidget *vectPanel)
    {
        if (vectPanel->objectName().isEmpty())
            vectPanel->setObjectName("vectPanel");
        mainVert = new QVBoxLayout(vectPanel);
        mainVert->setObjectName("mainVert");
        vectPanel->setLayout(mainVert);
        vectPanel->setMaximumWidth(275);
        contentDesc = new QHBoxLayout();
        contentDesc->setObjectName("contentDesc");
        contentLb = new QLabel(vectPanel);
        contentLb->setObjectName("contentLb");
        contentLb->setText("Content");
        contentDesc->addWidget(contentLb);
        contentDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        content = new QPlainTextEdit(vectPanel);
        content->setObjectName("content");
        content->setWordWrapMode(QTextOption::WordWrap);
        mainVert->addLayout(contentDesc);
        mainVert->addWidget(content);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        fontDesc = new QHBoxLayout();
        fontDesc->setObjectName("fontDesc");
        fontLb = new QLabel(vectPanel);
        fontLb->setObjectName("fontLb");
        fontLb->setText("Filter font");
        fontDesc->addWidget(fontLb);
        fontDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        mainVert->addLayout(fontDesc);
        fonts = new QComboBox(vectPanel);
        fonts->setObjectName("fonts");
        QFontDatabase qfd;
        fonts->addItems(qfd.families());
        mainVert->addWidget(fonts);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        sizeDesc = new QHBoxLayout();
        sizeDesc->setObjectName("sizeDesc");
        sizeLb = new QLabel(vectPanel);
        sizeLb->setObjectName("sizeLb");
        sizeLb->setText("Font Size");
        sizeDesc->addWidget(sizeLb);
        sizeDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        sizeBox = new QHBoxLayout();
        mainVert->addLayout(sizeDesc);
        sizeSB = new QSpinBox(vectPanel);
        sizeSB->setRange(1, 1000);
        sizeSB->setObjectName("sizeSB");
        sizeSB->setValue(12);
        sizeBox->addWidget(sizeSB);
        sizeS = new QSlider(vectPanel);
        sizeS->setRange(1, 1000);
        sizeS->setObjectName("sizeS");
        sizeS->setValue(12);
        sizeS->setOrientation(Qt::Horizontal);
        sizeBox->addWidget(sizeS);
        mainVert->addLayout(sizeBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        colorDesc1 = new QHBoxLayout();
        colorDesc1->setObjectName("colorDesc");
        colorLb1 = new QLabel(vectPanel);
        colorLb1->setObjectName("colorLb");
        colorLb1->setText("Text Color");
        colorDesc1->addWidget(colorLb1);
        colorDesc1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        colorBox1 = new QHBoxLayout();
        mainVert->addLayout(colorDesc1);
        colorSB1 = new QSpinBox(vectPanel);
        colorSB1->setRange(0, 360);
        colorSB1->setObjectName("colorSB1");
        colorSB1->setValue(0);
        colorBox1->addWidget(colorSB1);
        colorS1 = new QSlider(vectPanel);
        colorS1->setRange(0, 360);
        colorS1->setObjectName("colorS1");
        colorS1->setValue(0);
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
        color1 = new QLabel(vectPanel);
        color1->setPixmap(QPixmap::fromImage(img1));
        color1->setCursor(Qt::CrossCursor);
        color1->setMouseTracking(true);
        mainVert->addWidget(color1);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        tawDesc = new QHBoxLayout();
        tawDesc->setObjectName("tawDesc");
        tawLb = new QLabel(vectPanel);
        tawLb->setObjectName("tawLb");
        tawLb->setText("Text Area Width");
        tawDesc->addWidget(tawLb);
        tawDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        tawBox = new QHBoxLayout();
        mainVert->addLayout(tawDesc);
        tawSB = new QSpinBox(vectPanel);
        tawSB->setRange(-1, 5000);
        tawSB->setObjectName("tawSB");
        tawSB->setValue(-1);
        tawBox->addWidget(tawSB);
        tawS = new QSlider(vectPanel);
        tawS->setRange(-1, 5000);
        tawS->setObjectName("tawS");
        tawS->setValue(-1);
        tawS->setOrientation(Qt::Horizontal);
        tawBox->addWidget(tawS);
        mainVert->addLayout(tawBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        wordDesc = new QHBoxLayout();
        wordDesc->setObjectName("wordDesc");
        wordLb = new QLabel(vectPanel);
        wordLb->setObjectName("wordLb");
        wordLb->setText("Word Spacing");
        wordDesc->addWidget(wordLb);
        wordDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        wordBox = new QHBoxLayout();
        mainVert->addLayout(wordDesc);
        wordSB = new QSpinBox(vectPanel);
        wordSB->setRange(-100, 100);
        wordSB->setObjectName("wordSB");
        wordSB->setValue(0);
        wordBox->addWidget(wordSB);
        wordS = new QSlider(vectPanel);
        wordS->setRange(-100, 100);
        wordS->setObjectName("wordS");
        wordS->setValue(0);
        wordS->setOrientation(Qt::Horizontal);
        wordBox->addWidget(wordS);
        mainVert->addLayout(wordBox);

        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));

        letterDesc = new QHBoxLayout();
        letterDesc->setObjectName("letterDesc");
        letterLb = new QLabel(vectPanel);
        letterLb->setObjectName("letterLb");
        letterLb->setText("Letter Spacing");
        letterDesc->addWidget(letterLb);
        letterDesc->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
        letterBox = new QHBoxLayout();
        mainVert->addLayout(letterDesc);
        letterSB = new QSpinBox(vectPanel);
        letterSB->setRange(-100, 100);
        letterSB->setObjectName("letterSB");
        letterSB->setValue(0);
        letterBox->addWidget(letterSB);
        letterS = new QSlider(vectPanel);
        letterS->setRange(-100, 100);
        letterS->setObjectName("letterS");
        letterS->setValue(0);
        letterS->setOrientation(Qt::Horizontal);
        letterBox->addWidget(letterS);
        mainVert->addLayout(letterBox);

        //mainVert->addSpacerItem(new QSpacerItem(275, 5, QSizePolicy::Minimum, QSizePolicy::Fixed));

        mainStyle = new QHBoxLayout();
        mainStyle->setObjectName("mainStyle");
        style1v = new QVBoxLayout();
        style1v->setObjectName("style1v");
        boldLb = new QLabel(vectPanel);
        boldLb->setObjectName("boldLb");
        boldLb->setText("Bold");
        style1v->addWidget(boldLb);
        bold = new QRadioButton(vectPanel);
        bold->setChecked(false);
        bold->setObjectName("bold");
        bold->setAutoExclusive(false);
        style1v->addWidget(bold);
        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));
        overLb = new QLabel(vectPanel);
        overLb->setObjectName("overLb");
        overLb->setText("Overline");
        style1v->addWidget(overLb);
        over = new QRadioButton(vectPanel);
        over->setObjectName("over");
        over->setAutoExclusive(false);
        style1v->addWidget(over);
        mainStyle->addLayout(style1v);
        mainStyle->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));

        style2v = new QVBoxLayout();
        style2v->setObjectName("style2v");
        italLb = new QLabel(vectPanel);
        italLb->setObjectName("italLb");
        italLb->setText("Italic");
        style2v->addWidget(italLb);
        ital = new QRadioButton(vectPanel);
        ital->setObjectName("ital");
        ital->setAutoExclusive(false);
        style2v->addWidget(ital);
        mainVert->addSpacerItem(new QSpacerItem(275, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));
        strkLb = new QLabel(vectPanel);
        strkLb->setObjectName("strkLb");
        strkLb->setText("Strikeout");
        style2v->addWidget(strkLb);
        strk = new QRadioButton(vectPanel);
        strk->setObjectName("strk");
        strk->setAutoExclusive(false);
        style2v->addWidget(strk);
        mainStyle->addLayout(style2v);
        mainStyle->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));

        style3v = new QVBoxLayout();
        style3v->setObjectName("style3v");
        undrLb = new QLabel(vectPanel);
        undrLb->setObjectName("undrLb");
        undrLb->setText("Underline");
        style3v->addWidget(undrLb);
        undr = new QRadioButton(vectPanel);
        undr->setObjectName("undr");
        undr->setAutoExclusive(false);
        style3v->addWidget(undr);
        style3v->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
        mainStyle->addLayout(style3v);
        mainVert->addLayout(mainStyle);


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
    class TextPanel: public Ui_TextPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTPANEL_H
