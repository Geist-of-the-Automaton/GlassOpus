#ifndef UI_LAYERFUNC_H
#define UI_LAYERFUNC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <graphics.h>

QT_BEGIN_NAMESPACE

class Ui_LayerFunc
{
public:
    QVBoxLayout *mainVert;
    QHBoxLayout *mainHori, *bBox;
    QComboBox *op, *opType, *la, *lb;
    QLabel *text, *alb, *blb;
    QDialogButtonBox *buttonBox;
    QSpacerItem *spacer;

    void setupUi(QWidget *targetdialog)
    {
        if (targetdialog->objectName().isEmpty())
            targetdialog->setObjectName(QString::fromUtf8("targetdialog"));
        targetdialog->resize(282, 244);
        mainVert = new QVBoxLayout(targetdialog);
        mainVert->setObjectName(QString::fromUtf8("mainVert"));
        mainHori = new QHBoxLayout();
        mainHori->setObjectName(QString::fromUtf8("mainHori"));
        mainVert->addLayout(mainHori);

        op = new QComboBox(targetdialog);
        QStringList qsl {"ADD", "SUBTRACT", "DIFFERENCE", "MAX", "MIN", "AVERAGE", "REMOVE", "NOT", "AND", "NAND", "OR", "NOR", "XOR", "XNOR"};
        op->addItems(qsl);
        op->setObjectName(QString::fromUtf8("op"));
        opType = new QComboBox(targetdialog);
        opType->setObjectName(QString::fromUtf8("opType"));
        opType->addItems(graphics::eTypes);
        alb = new QLabel(targetdialog);
        alb->setObjectName(QString::fromUtf8("alb"));
        alb->setText("Layer A");
        mainHori->addWidget(alb);
        la = new QComboBox(targetdialog);
        la->setObjectName(QString::fromUtf8("la"));
        mainHori->addWidget(la);
        mainHori->addWidget(op);
        mainHori->addWidget(opType);
        blb = new QLabel(targetdialog);
        blb->setObjectName(QString::fromUtf8("blb"));
        blb->setText("Layer B");
        mainHori->addWidget(blb);
        lb = new QComboBox(targetdialog);
        lb->setObjectName(QString::fromUtf8("lb"));
        mainHori->addWidget(lb);

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
    class LayerFunc: public Ui_LayerFunc {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAYERFUNC_H
