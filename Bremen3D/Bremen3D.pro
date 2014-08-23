#-------------------------------------------------
#
# Project created by QtCreator 2013-12-29T03:03:07
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Bremen3D
TEMPLATE = app

HEADERS += \
    adc.h \
    adctab.h \
    dactab.h \
    eqtab.h \
    graphwindow.h \
    maintab.h \
    mainwindow.h \
    sendudp.h \
    UDPcommands.h \
    widget.h \
    calcfilt.h \
    classes/knob.h \
    classes/qcustomplot.h \
    classes/transform.h \
    centralwidget.h \
    eqchannel.h \
    eqsection.h \
    defines.h \
    mixertab.h

SOURCES += \
    adc.cpp \
    adctab.cpp \
    dactab.cpp \
    eqtab.cpp \
    graphwindow.cpp \
    main.cpp \
    maintab.cpp \
    mainwindow.cpp \
    sendudp.cpp \
    widget.cpp \
    calcfilt.cpp \
    classes/knob.cpp \
    classes/qcustomplot.cpp \
    classes/transform.cpp \
    centralwidget.cpp \
    eqchannel.cpp \
    eqsection.cpp \
    mixertab.cpp

RESOURCES += \
    graphics.qrc

FORMS += \



