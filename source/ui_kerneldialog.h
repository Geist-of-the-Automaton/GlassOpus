#ifndef UI_KERNELDIALOG_H
#define UI_KERNELDIALOG_H

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
#include <QtWidgets/QSlider>
#include <string>
using std::to_string;

#include <graphics.h>

QT_BEGIN_NAMESPACE

class Ui_KernelDialog
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
    QVBoxLayout *verticalLayout_1;
    QVBoxLayout *verticalLayout;
    QDialogButtonBox *buttonBox;
    QComboBox *qcb1;
    QSpinBox *spinbox;

    void setupUi(QWidget *kernelDialog)
    {
        if (kernelDialog->objectName().isEmpty())
            kernelDialog->setObjectName(QString::fromUtf8("kernelDialog"));
        kernelDialog->resize(282, 244);
        verticalLayout_3 = new QVBoxLayout(kernelDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label = new QLabel(kernelDialog);
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

        qcb1 = new QComboBox(kernelDialog);
        qcb1->setObjectName(QString::fromUtf8("combo1"));
        for (string s : graphics::filterNames)
            qcb1->addItem(s.c_str());
        qcb1->setCurrentIndex(0);

        verticalLayout_1->addWidget(qcb1);

        horizontalLayout_3->addLayout(verticalLayout_1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));


        spinbox = new QSpinBox(kernelDialog);
        spinbox->setObjectName(QString::fromUtf8("spinbox"));
        spinbox->setAlignment(Qt::AlignCenter);
        spinbox->setRange(1, 15);
        spinbox->setValue(1);

        verticalLayout_5->addWidget(spinbox);

        horizontalLayout_3->addLayout(verticalLayout_5);

        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonBox = new QDialogButtonBox(kernelDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_3->addLayout(horizontalLayout);

        retranslateUi(kernelDialog);

        QMetaObject::connectSlotsByName(kernelDialog);
    } // setupUi

    void retranslateUi(QWidget *kernelDialog)
    {
        kernelDialog->setWindowTitle(QCoreApplication::translate("kernelDialog", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class KernelDialog: public Ui_KernelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KERNELDIALOG_H
