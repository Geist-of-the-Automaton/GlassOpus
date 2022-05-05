#TARGET = Roto

QT = core gui widgets

RC_ICONS = execIco.ico

HEADERS = \
    $$PWD/brush.h \
    $$PWD/brushhandler.h \
    $$PWD/brushshape.h \
    $$PWD/dataIOHandler.h \
    $$PWD/graphics.h \
    $$PWD/layer.h \
    $$PWD/mainwindow.h \
    $$PWD/patternprofiler.h \
    $$PWD/radialprofiler.h \
    $$PWD/screenrender.h \
    $$PWD/splinevector.h \
    $$PWD/stdfuncs.h \
    $$PWD/resizewindow.h \
    $$PWD/triangle.h \
    $$PWD/ui_mainwindow.h \
    $$PWD/ui_radialprofiler.h \
    $$PWD/ui_resizewindow.h \
    $$PWD/viewscroller.h\
    $$PWD/ui_brushShape.h\
    $$PWD/ui_patternprofiler.h\
    $$PWD/brushShape.h\
    $$PWD/patternprofiler.h \
    brightnessadj.h \
    contrastadj.h \
    ditherdialog.h \
    drawtext.h \
    filterdialog.h \
    gammaadj.h \
    histodialog.h \
    hueshift.h \
    kerneldialog.h \
    polygon.h \
    saturationadj.h \
    symdialog.h \
    transferdialog.h \
    ui_brightnessadj.h \
    ui_contrastadj.h \
    ui_ditherdialog.h \
    ui_filterdialog.h \
    ui_gammaadj.h \
    ui_histodialog.h \
    ui_hueshift.h \
    ui_kerneldialog.h \
    ui_saturationadj.h \
    ui_symdialog.h \
    ui_transfer.h \
    vec_mat_maths.h

SOURCES = \
    $$PWD/brush.cpp \
    $$PWD/brushhandler.cpp \
    $$PWD/brushshape.cpp \
    $$PWD/dataIOHandler.cpp \
    $$PWD/graphics.cpp \
    $$PWD/layer.cpp \
    $$PWD/main.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/mainwindow.ui \
    $$PWD/patternprofiler.cpp \
    $$PWD/radialprofiler.cpp \
    $$PWD/resizewindow.cpp \
    $$PWD/screenrender.cpp \
    $$PWD/splinevector.cpp \
    $$PWD/viewscroller.cpp\
    $$PWD/brushShape.cpp\
    $$PWD/patternprofiler.cpp \
    brightnessadj.cpp \
    contrastadj.cpp \
    ditherdialog.cpp \
    drawtext.cpp \
    filterdialog.cpp \
    gammaadj.cpp \
    histodialog.cpp \
    hueshift.cpp \
    kerneldialog.cpp \
    polygon.cpp \
    saturationadj.cpp \
    symdialog.cpp \
    transferdialog.cpp

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
