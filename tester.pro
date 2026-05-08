QT       += core gui widgets serialbus printsupport
CONFIG   += c++17
TARGET   = tester
TEMPLATE = app

msvc {
    QMAKE_CXXFLAGS += /utf-8
    QMAKE_CFLAGS += /utf-8
}


INCLUDEPATH += $$PWD \
               $$PWD/third_party/qcustomplot

SOURCES += \
    control/controlpanel.cpp \
    control/curveselector.cpp \
    curve/curveview.cpp \
    curve/yrangedialog.cpp \
    data/csvlogger.cpp \
    data/dataparser.cpp \
    main.cpp \
    config/configeditdialog.cpp \
    config/configmanager.cpp \
    communication/modbuscomm.cpp \
    mainwindow/connectdialog.cpp \
    mainwindow/mainwindow.cpp \
    third_party/qcustomplot/qcustomplot.cpp

HEADERS += \
    communication/modbuscomm.h \
    config/configeditdialog.h \
    config/configmanager.h \
    config/point_config.h \
    control/controlpanel.h \
    control/curveselector.h \
    curve/curveview.h \
    curve/yrangedialog.h \
    data/csvlogger.h \
    data/dataparser.h \
    data/point_value.h \
    mainwindow/connectdialog.h \
    mainwindow/mainwindow.h \
    third_party/qcustomplot/qcustomplot.h

FORMS += \
    config/configeditdialog.ui \
    control/controlpanel.ui \
    curve/yrangedialog.ui \
    mainwindow/connectdialog.ui \
    mainwindow/mainwindow.ui