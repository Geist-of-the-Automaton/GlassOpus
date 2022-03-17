#ifndef UI_SYMDIALOG_H
#define UI_SYMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <string>
using std::to_string;

const int maxSym = 36;
const int minSym = 1;

QT_BEGIN_NAMESPACE

class Ui_SymDialog
{
public:
    QVBoxLayout *verticalLayout_3;
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

    void setupUi(QWidget *symDialog)
    {
        if (symDialog->objectName().isEmpty())
            symDialog->setObjectName(QString::fromUtf8("symDialog"));
        symDialog->resize(350, 150);
        symDialog->setFixedSize(350, 150);
        verticalLayout_3 = new QVBoxLayout(symDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));

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

        qcb1 = new QComboBox(symDialog);
        qcb1->setObjectName(QString::fromUtf8("combo1"));
        for (int i = minSym; i <= maxSym; ++i)
            qcb1->addItem((to_string(i) + " Divisions").c_str());
        qcb1->setCurrentIndex(0);

        verticalLayout_1->addWidget(qcb1);

        horizontalLayout_3->addLayout(verticalLayout_1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));

        qcb2 = new QComboBox(symDialog);
        qcb2->setObjectName(QString::fromUtf8("combo3"));
        qcb2->addItem("Of Every 1");
        qcb2->setCurrentIndex(0);

        verticalLayout_5->addWidget(qcb2);

        horizontalLayout_3->addLayout(verticalLayout_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));

        qcb3 = new QComboBox(symDialog);
        qcb3->setObjectName(QString::fromUtf8("combo4"));
        qcb3->addItem("Skip 0");
        qcb3->setCurrentIndex(0);

        verticalLayout_4->addWidget(qcb3);

        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonBox = new QDialogButtonBox(symDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);


        //horizontalLayout->addLayout(verticalLayout);

        verticalLayout_3->addLayout(horizontalLayout);
        verticalLayout_3->addWidget(buttonBox);

        retranslateUi(symDialog);

        QMetaObject::connectSlotsByName(symDialog);
    } // setupUi

    void retranslateUi(QWidget *symDialog)
    {
        symDialog->setWindowTitle(QCoreApplication::translate("symDialog", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SymDialog: public Ui_SymDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYMDIALOG_H
