// Generated by Qt UI editor. Reformated after.

#ifndef UI_RADIALPROFILER_H
#define UI_RADIALPROFILER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RadialProfiler {

public:

    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RadialProfiler) {
        if (RadialProfiler->objectName().isEmpty())
            RadialProfiler->setObjectName(QString::fromUtf8("RadialProfiler"));
        RadialProfiler->resize(800, 600);
        centralwidget = new QWidget(RadialProfiler);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        RadialProfiler->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RadialProfiler);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        RadialProfiler->setMenuBar(menubar);
        statusbar = new QStatusBar(RadialProfiler);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        RadialProfiler->setStatusBar(statusbar);
        retranslateUi(RadialProfiler);
        QMetaObject::connectSlotsByName(RadialProfiler);
    }

    void retranslateUi(QMainWindow *RadialProfiler) {
        RadialProfiler->setWindowTitle(QCoreApplication::translate("RadialProfiler", "RadialProfiler", nullptr));
    }
};

namespace Ui {
    class RadialProfiler: public Ui_RadialProfiler {};
}

QT_END_NAMESPACE

#endif // UI_RADIALPROFILER_H