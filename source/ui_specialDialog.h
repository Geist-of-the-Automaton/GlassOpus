#ifndef UI_SPECIALDIALOG_H
#define UI_SPECIALDIALOG_H

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

#include <graphics.h>

QT_BEGIN_NAMESPACE

class Ui_Special
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
    QVBoxLayout *verticalLayout_1;
    QSlider *slider;
    QVBoxLayout *verticalLayout;
    QDialogButtonBox *buttonBox;
    QComboBox *qcb, *qcb2;

    void setupUi(QWidget *special)
    {
        if (special->objectName().isEmpty())
            special->setObjectName(QString::fromUtf8("special"));
        special->resize(282, 244);
        verticalLayout_3 = new QVBoxLayout(special);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label = new QLabel(special);
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

        qcb2 = new QComboBox(special);
        qcb2->setObjectName("combo2");
        for (string s : graphics::specFilterNames)
            qcb2->addItem(QString(s.c_str()));
        qcb2->setCurrentIndex(0);

        horizontalLayout_3->addWidget(qcb2);

        qcb = new QComboBox(special);
        qcb->setObjectName(QString::fromUtf8("combo"));
        qcb->addItems(QStringList({"RGB", "HSV", "HSL", "LAB"}));
        qcb->setCurrentIndex(0);

        verticalLayout_1->addWidget(qcb);

        horizontalLayout_3->addLayout(verticalLayout_1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));

        spinbox = new QSpinBox(special);
        spinbox->setObjectName(QString::fromUtf8("spinbox"));
        spinbox->setAlignment(Qt::AlignCenter);
        spinbox->setRange(2, 60);
        spinbox->setValue(2);
        spinbox->setValue(2);

        verticalLayout_5->addWidget(spinbox);

        horizontalLayout_3->addLayout(verticalLayout_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));

        slider = new QSlider(special);
        slider->setObjectName(QString::fromUtf8("slider"));
        slider->setMinimum(2);
        slider->setMaximum(60);
        slider->setValue(2);
        slider->setOrientation(Qt::Orientation::Horizontal);

        verticalLayout_4->addWidget(slider);

        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonBox = new QDialogButtonBox(special);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_3->addLayout(horizontalLayout);

        retranslateUi(special);

        QMetaObject::connectSlotsByName(special);
    } // setupUi

    void retranslateUi(QWidget *special)
    {
        special->setWindowTitle(QCoreApplication::translate("Special Filters", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Special: public Ui_Special {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPECIALDIALOG_H
