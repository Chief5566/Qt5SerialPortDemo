#-------------------------------------------------
#
# Project created by QtCreator 2015-06-18T13:36:03
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialCom
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    comlogic.cpp \
    custom_label.cpp

HEADERS  += widget.h \
    custom_label.h

FORMS    += widget.ui
