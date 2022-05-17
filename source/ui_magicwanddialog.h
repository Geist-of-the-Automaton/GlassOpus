#ifndef UI_MAGICWANDDIALOG_H
#define UI_MAGICWANDDIALOG_H

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

class Ui_WandDialog
{
public:
    QVBoxLayout *mainVert, *left, *right;
    QHBoxLayout *mainHori, *mainHori2, *hue, *sat, *lum, *bBox;
    QHBoxLayout *adv;
    QLabel *threshLB, *hueLB, *satLB, *lumLB, *advLB;
    QSpinBox *threshSB, *hueSB, *satSB, *lumSB;
    QSlider *threshS, *hueS, *satS, *lumS;
    QDialogButtonBox *buttonBox;
    QSpacerItem *spacer;

    void setupUi(QWidget *targetdialog)
    {
        if (targetdialog->objectName().isEmpty())
            targetdialog->setObjectName(QString::fromUtf8("targetdialog"));
        targetdialog->resize(400, 244);
        mainVert = new QVBoxLayout(targetdialog);
        mainVert->setObjectName(QString::fromUtf8("mainVert"));
        mainHori = new QHBoxLayout();
        mainHori->setObjectName(QString::fromUtf8("mainHori"));
        mainVert->addLayout(mainHori);
        adv = new QHBoxLayout();
        adv->setObjectName(QString::fromUtf8("adv"));
        advLB = new QLabel(targetdialog);
        advLB->setObjectName(QString::fromUtf8("advLB"));
        advLB->setText("- Advanced -");
        adv->addWidget(advLB, 0, Qt::AlignCenter);
        mainVert->addLayout(adv);
        mainHori2 = new QHBoxLayout();
        mainHori2->setObjectName(QString::fromUtf8("mainHori2"));
        mainVert->addLayout(mainHori2);
        left = new QVBoxLayout();
        left->setObjectName(QString::fromUtf8("left"));
        right = new QVBoxLayout();
        right->setObjectName(QString::fromUtf8("right"));
        threshLB = new QLabel(targetdialog);
        threshLB->setObjectName(QString::fromUtf8("threshLB"));
        threshLB->setText("Threshold");
        threshSB = new QSpinBox(targetdialog);
        threshSB->setObjectName(QString::fromUtf8("threshSB"));
        threshSB->setRange(0, 100);
        threshS = new QSlider(targetdialog);
        threshS->setObjectName(QString::fromUtf8("threshS"));
        threshS->setRange(0, 100);
        threshS->setOrientation(Qt::Horizontal);
        mainHori->addWidget(threshLB);
        mainHori->addWidget(threshSB);
        mainHori->addWidget(threshS);
        hueLB = new QLabel(targetdialog);
        hueLB->setObjectName(QString::fromUtf8("hueLB"));
        hueLB->setText("Hue Difference");
        left->addWidget(hueLB);
        satLB = new QLabel(targetdialog);
        satLB->setObjectName(QString::fromUtf8("satLB"));
        satLB->setText("Saturation Difference");
        left->addWidget(satLB);
        lumLB = new QLabel(targetdialog);
        lumLB->setObjectName(QString::fromUtf8("lumLB"));
        lumLB->setText("Luminance Difference");
        left->addWidget(lumLB);
        mainHori2->addLayout(left);
        mainHori2->addLayout(right);
        hueSB = new QSpinBox(targetdialog);
        hueSB->setObjectName(QString::fromUtf8("hueSB"));
        hueSB->setRange(0, 180);
        hueS = new QSlider(targetdialog);
        hueS->setObjectName(QString::fromUtf8("hueS"));
        hueS->setRange(0, 180);
        hueS->setOrientation(Qt::Horizontal);
        hue = new QHBoxLayout();
        hue->setObjectName(QString::fromUtf8("hue"));
        hue->addWidget(hueSB);
        hue->addWidget(hueS);
        right->addLayout(hue);
        satSB = new QSpinBox(targetdialog);
        satSB->setObjectName(QString::fromUtf8("satSB"));
        satSB->setRange(0, 100);
        satS = new QSlider(targetdialog);
        satS->setObjectName(QString::fromUtf8("satS"));
        satS->setRange(0, 100);
        satS->setOrientation(Qt::Horizontal);
        sat = new QHBoxLayout();
        sat->setObjectName(QString::fromUtf8("sat"));
        sat->addWidget(satSB);
        sat->addWidget(satS);
        right->addLayout(sat);
        lumSB = new QSpinBox(targetdialog);
        lumSB->setObjectName(QString::fromUtf8("lumSB"));
        lumSB->setRange(0, 100);
        lumS = new QSlider(targetdialog);
        lumS->setObjectName(QString::fromUtf8("lumS"));
        lumS->setRange(0, 100);
        lumS->setOrientation(Qt::Horizontal);
        lum = new QHBoxLayout();
        lum->setObjectName(QString::fromUtf8("lum"));
        lum->addWidget(lumSB);
        lum->addWidget(lumS);
        right->addLayout(lum);
        bBox = new QHBoxLayout();
        bBox->setObjectName(QString::fromUtf8("spacer"));
        spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        bBox->addSpacerItem(spacer);
        buttonBox = new QDialogButtonBox(targetdialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        bBox->addWidget(buttonBox);
        mainVert->addLayout(bBox);

        retranslateUi(targetdialog);

        QMetaObject::connectSlotsByName(targetdialog);
    } // setupUi

    void retranslateUi(QWidget *targetdialog)
    {
        targetdialog->setWindowTitle(QCoreApplication::translate("targetdialog", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MagicWandDialog: public Ui_WandDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TARGETDIALOG_H


#endif // UI_MAGICWANDDIALOG_H
