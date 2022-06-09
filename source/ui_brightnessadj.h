#ifndef UI_BRIGHTNESSADJ_H
#define UI_BRIGHTNESSADJ_H

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
#include <QtWidgets/QComboBox>

QT_BEGIN_NAMESPACE

class Ui_BrightnessAdj
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_6;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_5;
    QDoubleSpinBox *spinbox;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_1;
    QSlider *slider;
    QVBoxLayout *verticalLayout;
    QDialogButtonBox *buttonBox;
    QComboBox *qcb;

    void setupUi(QWidget *brightnessAdj)
    {
        if (brightnessAdj->objectName().isEmpty())
            brightnessAdj->setObjectName(QString::fromUtf8("brightnessAdj"));
        brightnessAdj->resize(282, 244);
        verticalLayout_3 = new QVBoxLayout(brightnessAdj);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label = new QLabel(brightnessAdj);
        label->setObjectName(QString::fromUtf8("label"));
        label->setEnabled(true);
        label->setCursor(QCursor(Qt::CrossCursor));
        label->setMouseTracking(false);

        verticalLayout_6->addWidget(label);

        verticalLayout_3->addLayout(verticalLayout_6);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));

        verticalLayout_1 = new QVBoxLayout();
        verticalLayout_1->setObjectName(QString::fromUtf8("verticalLayout_1"));

        qcb = new QComboBox(brightnessAdj);
        qcb->setObjectName(QString::fromUtf8("combo"));
        qcb->addItems(QStringList({"RGB", "HSV", "HSL", "LAB"}));
        qcb->setCurrentIndex(1);

        verticalLayout_1->addWidget(qcb);

        horizontalLayout_3->addLayout(verticalLayout_1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));

        spinbox = new QDoubleSpinBox(brightnessAdj);
        spinbox->setObjectName(QString::fromUtf8("spinbox"));
        spinbox->setAlignment(Qt::AlignCenter);
        spinbox->setRange(-1.0, 1.0);
        spinbox->setDecimals(3);
        spinbox->setValue(0.0);
        spinbox->setSingleStep(0.01);

        verticalLayout_5->addWidget(spinbox);

        horizontalLayout_3->addLayout(verticalLayout_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));

        slider = new QSlider(brightnessAdj);
        slider->setObjectName(QString::fromUtf8("slider"));
        slider->setMinimum(-255);
        slider->setMaximum(255);
        slider->setValue(0);
        slider->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout_4->addWidget(slider);

        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonBox = new QDialogButtonBox(brightnessAdj);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_3->addLayout(horizontalLayout);

        retranslateUi(brightnessAdj);

        QMetaObject::connectSlotsByName(brightnessAdj);
    } // setupUi

    void retranslateUi(QWidget *brightnessAdj)
    {
        brightnessAdj->setWindowTitle(QCoreApplication::translate("Brightness Adjustment", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BrightnessAdj: public Ui_BrightnessAdj {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BRIGHTNESSADJ_H
