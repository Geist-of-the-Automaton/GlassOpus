#ifndef UI_HISTODIALOG_H
#define UI_HISTODIALOG_H

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
#include <QtWidgets/QSpacerItem>
#include <string>
using std::to_string;

#include <graphics.h>

QT_BEGIN_NAMESPACE

class Ui_HistoDialog
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
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_1;
    QHBoxLayout *horiLayout1, *horiLayout2, *horiLayout3;
    QVBoxLayout *verticalLayout;
    QDialogButtonBox *buttonBox;
    QComboBox *qcb1, *qcb2, *qcb3;
    QSpinBox *xdivSB, *ydivSB;
    QDoubleSpinBox *clipSB;
    QSlider *clipS, *xdivS, *ydivS;
    QLabel *matchLB, *clipLB, *xdivLB, *ydivLB, *note;
    QSpacerItem *spacer;


    void setupUi(QWidget *histoDialog)
    {
        if (histoDialog->objectName().isEmpty())
            histoDialog->setObjectName(QString::fromUtf8("histoDialog"));
        histoDialog->resize(300, 244);
        verticalLayout_3 = new QVBoxLayout(histoDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label = new QLabel(histoDialog);
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

        qcb1 = new QComboBox(histoDialog);
        qcb1->setObjectName(QString::fromUtf8("combo1"));
        qcb1->addItems(graphics::eTypes);
        qcb1->setCurrentIndex(0);

        verticalLayout_1->addWidget(qcb1);

        horizontalLayout_3->addLayout(verticalLayout_1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));

        qcb2 = new QComboBox(histoDialog);
        qcb2->setObjectName(QString::fromUtf8("combo2"));
        qcb2->addItems(QStringList({"View Histogram", "Match Histogram", "Equalize Histogram", "CLAHE"}));
        qcb2->setCurrentIndex(0);

        verticalLayout_5->addWidget(qcb2);

        horizontalLayout_3->addLayout(verticalLayout_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));

        qcb3 = new QComboBox(histoDialog);
        qcb3->setObjectName(QString::fromUtf8("combo3"));
        qcb3->addItems(graphics::mTypes);
        qcb3->setCurrentIndex(0);

        verticalLayout_4->addWidget(qcb3);

        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_2->addLayout(horizontalLayout_3);

        horiLayout1 = new QHBoxLayout();
        horiLayout1->setObjectName("horiLayout1");
        clipLB = new QLabel(histoDialog);
        clipLB->setText("Clipping Limit");
        clipLB->setObjectName("clipLB");
        horiLayout1->addWidget(clipLB);
        clipSB = new QDoubleSpinBox(histoDialog);
        clipSB->setRange(0.0, 10.0);
        clipSB->setValue(1.5);
        clipSB->setSingleStep(0.1);
        clipSB->setObjectName("clipSB");
        clipS = new QSlider(histoDialog);
        clipS->setRange(0, 100);
        clipS->setValue(15);
        clipS->setSingleStep(1);
        clipS->setObjectName("clipS");
        clipS->setOrientation(Qt::Horizontal);
        horiLayout1->addWidget(clipSB);
        horiLayout1->addWidget(clipS);
        verticalLayout_2->addLayout(horiLayout1);

        horiLayout2 = new QHBoxLayout();
        horiLayout2->setObjectName("horiLayout2");
        xdivLB = new QLabel(histoDialog);
        xdivLB->setText("X Divisions");
        xdivLB->setObjectName("xdivLB");
        horiLayout2->addWidget(xdivLB);
        xdivSB = new QSpinBox(histoDialog);
        xdivSB->setRange(1, 64);
        xdivSB->setValue(2);
        xdivSB->setSingleStep(1);
        xdivSB->setObjectName("xdivSB");
        xdivS = new QSlider(histoDialog);
        xdivS->setRange(1, 64);
        xdivS->setValue(2);
        xdivS->setSingleStep(1);
        xdivS->setObjectName("xdivS");
        xdivS->setOrientation(Qt::Horizontal);
        horiLayout2->addWidget(xdivSB);
        horiLayout2->addWidget(xdivS);
        verticalLayout_2->addLayout(horiLayout2);

        horiLayout3 = new QHBoxLayout();
        horiLayout3->setObjectName("horiLayout3");
        ydivLB = new QLabel(histoDialog);
        ydivLB->setText("Y Divisions");
        ydivLB->setObjectName("ydivLB");
        horiLayout3->addWidget(ydivLB);
        ydivSB = new QSpinBox(histoDialog);
        ydivSB->setRange(1, 64);
        ydivSB->setValue(2);
        ydivSB->setSingleStep(1);
        ydivSB->setObjectName("ydivSB");
        ydivS = new QSlider(histoDialog);
        ydivS->setRange(1, 64);
        ydivS->setValue(2);
        ydivS->setSingleStep(1);
        ydivS->setObjectName("ydivS");
        ydivS->setOrientation(Qt::Horizontal);
        horiLayout3->addWidget(ydivSB);
        horiLayout3->addWidget(ydivS);
        verticalLayout_2->addLayout(horiLayout3);

        horizontalLayout->addLayout(verticalLayout_2);

        note = new QLabel(histoDialog);
        note->setText("X or Y CLAHE divisions of 1 will use auto-divisions in that axis.");
        note->setObjectName("note");
        verticalLayout_2->addWidget(note);

        spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
        verticalLayout_2->addSpacerItem(spacer);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonBox = new QDialogButtonBox(histoDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);

        //horizontalLayout->addLayout(verticalLayout);

        verticalLayout_3->addLayout(horizontalLayout);

        retranslateUi(histoDialog);

        QMetaObject::connectSlotsByName(histoDialog);
    } // setupUi

    void retranslateUi(QWidget *histoDialog)
    {
        histoDialog->setWindowTitle(QCoreApplication::translate("histoDialog", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HistoDialog: public Ui_HistoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTODIALOG_H
