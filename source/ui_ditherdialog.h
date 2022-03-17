#ifndef UI_DITHERDIALOG_H
#define UI_DITHERDIALOG_H

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
#include <string>
using std::to_string;

QT_BEGIN_NAMESPACE

class Ui_DitherDialog
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
    QVBoxLayout *verticalLayout;
    QDialogButtonBox *buttonBox;
    QComboBox *qcb1, *qcb2, *qcb3;

    void setupUi(QWidget *ditherDialog)
    {
        if (ditherDialog->objectName().isEmpty())
            ditherDialog->setObjectName(QString::fromUtf8("ditherDialog"));
        ditherDialog->resize(300, 244);
        verticalLayout_3 = new QVBoxLayout(ditherDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label = new QLabel(ditherDialog);
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

        qcb1 = new QComboBox(ditherDialog);
        qcb1->setObjectName(QString::fromUtf8("combo1"));
        qcb1->addItems(QStringList({"Bayer", "Floyd Steinberg", "Sierra", "Sierra Lite", "Random", "Palette Reduction"}));
        qcb1->setCurrentIndex(0);

        verticalLayout_1->addWidget(qcb1);

        horizontalLayout_3->addLayout(verticalLayout_1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));

        qcb2 = new QComboBox(ditherDialog);
        qcb2->setObjectName(QString::fromUtf8("combo2"));
        int bpps[] = {1, 2, 3, 4, 5, 6, 7};
        QStringList strs;
        for (int i : bpps)
            strs.push_back((to_string(3 * i) + " BPP (" + to_string(i) + " per channel").c_str());
        qcb2->addItems(strs);
        qcb2->setCurrentIndex(0);

        verticalLayout_5->addWidget(qcb2);

        horizontalLayout_3->addLayout(verticalLayout_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));

        qcb3 = new QComboBox(ditherDialog);
        qcb3->setObjectName(QString::fromUtf8("combo3"));
        int matSizes[] = {2, 4, 8, 16, 32};
        strs.clear();
        for (int i : matSizes)
            strs.push_back((to_string(i) + "x" + to_string(i)).c_str());
        qcb3->addItems(strs);
        qcb3->setCurrentIndex(0);

        verticalLayout_4->addWidget(qcb3);

        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonBox = new QDialogButtonBox(ditherDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_3->addLayout(horizontalLayout);

        retranslateUi(ditherDialog);

        QMetaObject::connectSlotsByName(ditherDialog);
    } // setupUi

    void retranslateUi(QWidget *ditherDialog)
    {
        ditherDialog->setWindowTitle(QCoreApplication::translate("DitherDialog", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DitherDialog: public Ui_DitherDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DITHERDIALOG_H
