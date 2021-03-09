QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    brush.cpp \
    brushHandler.cpp \
    graphics.cpp \
    layer.cpp \
    main.cpp \
    mainwindow.cpp \
    dataIOHandler.cpp \
    radialprofiler.cpp \
    resizewindow.cpp \
    screenrender.cpp \
    splinevector.cpp \
    triangle.cpp \
    viewscroller.cpp

HEADERS += \
    brush.h \
    brushHandler.h \
    graphics.h \
    layer.h \
    mainwindow.h \
    radialprofiler.h \
    screenrender.h \
    splinevector.h \
    stdfuncs.h \
    dataIOHandler.h \
    resizewindow.h \
    triangle.h \
    ui_resizewindow.h \
    ui_radialprofiler.h \
    viewscroller.h

FORMS += \
    mainwindow.ui

LIBS += -LC:\opencv_build\install\x64\vc64\lib -lopencv_core450 -lopencv_imgproc450 -lopencv_highgui450 -lopencv_imgcodecs450 -lopencv_videoio450 -lopencv_video450 -lopencv_calib3d450 -lopencv_photo450 -lopencv_features2d450
  INCLUDEPATH += C:\opencv_build\install\include
  DEPENDPATH += C:\opencv_build\install\include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
