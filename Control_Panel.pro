#-------------------------------------------------
#
# Project created by QtCreator 2015-04-24T13:50:38
#
#-------------------------------------------------

QT       += core gui
QT       += network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Control_Panel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    parser.cpp \
    zoom.cpp \
    command.cpp \
    dialog_color.cpp \
    gscene.cpp \
    draw.cpp \
    qcustomplot.cpp \
    qledindicator.cpp \
    plc_omron.cpp \
    repair_data.cpp \
    tcp_client.cpp

HEADERS  += mainwindow.h \
    parser.h \
    zoom.h \
    command.h \
    dialog_color.h \
    gscene.h \
    draw.h \
    qcustomplot.h \
    qledindicator.h \
    plc_omron.h \
    repair_data.h \
    tcp_client.h

FORMS    += mainwindow.ui \
    dialog_color.ui

DISTFILES +=

RESOURCES += \
    icons.qrc
