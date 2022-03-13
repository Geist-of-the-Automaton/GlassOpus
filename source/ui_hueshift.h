#ifndef UI_HUESHIFT_H
#define UI_HUESHIFT_H

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

QT_BEGIN_NAMESPACE

class Ui_HueShift
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
    QSpinBox *spinbox;
    QVBoxLayout *verticalLayout_4;
    QSlider *slider;
    QVBoxLayout *verticalLayout;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *hueshift)
    {
        if (hueshift->objectName().isEmpty())
            hueshift->setObjectName(QString::fromUtf8("hueshift"));
        hueshift->resize(282, 244);
        verticalLayout_3 = new QVBoxLayout(hueshift);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label = new QLabel(hueshift);
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
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));

        spinbox = new QSpinBox(hueshift);
        spinbox->setObjectName(QString::fromUtf8("spinbox"));
        spinbox->setAlignment(Qt::AlignCenter);
        spinbox->setRange(0, 359);
        spinbox->setValue(0);
        spinbox->setSingleStep(1);

        verticalLayout_5->addWidget(spinbox);

        horizontalLayout_3->addLayout(verticalLayout_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));

        slider = new QSlider(hueshift);
        slider->setObjectName(QString::fromUtf8("slider"));
        slider->setMinimum(0);
        slider->setMaximum(359);
        slider->setValue(0);
        slider->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout_4->addWidget(slider);

        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonBox = new QDialogButtonBox(hueshift);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_3->addLayout(horizontalLayout);

        retranslateUi(hueshift);

        QMetaObject::connectSlotsByName(hueshift);
    } // setupUi

    void retranslateUi(QWidget *hueshift)
    {
        hueshift->setWindowTitle(QCoreApplication::translate("hueshift", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HueShift: public Ui_HueShift {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HUESHIFT_H
