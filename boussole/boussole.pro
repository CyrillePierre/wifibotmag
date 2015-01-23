#-------------------------------------------------
#
# Project created by QtCreator 2014-12-09T16:42:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = boussole
TEMPLATE = app

LIBS += -L/usr/local/qwt/lib/ -lqwt
INCLUDEPATH += /usr/local/qwt/include/

SOURCES += main.cpp \
    Thread.cpp \
    window.cpp

HEADERS  += \
    Thread.h \
    window.h

FORMS    += \
    window.ui

CONFIG   += qwt
