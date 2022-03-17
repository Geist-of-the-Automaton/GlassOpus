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
    QVBoxLayout *verticalLayout;
    QDialogButtonBox *buttonBox;
    QComboBox *qcb1, *qcb2, *qcb3;

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

        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonBox = new QDialogButtonBox(histoDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        horizontalLayout->addLayout(verticalLayout);

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
