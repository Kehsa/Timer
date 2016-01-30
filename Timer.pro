#-------------------------------------------------
#
# Project created by QtCreator 2016-01-29T21:46:05
#
#-------------------------------------------------

QT      += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Timer
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    mydatabase.cpp \
    dialogtimersettings.cpp \
    mytimerkeeper.cpp

HEADERS  += \
    mainwindow.h \
    mydatabase.h \
    dialogtimersettings.h \
    mytimerkeeper.h

FORMS    += \
    mainwindow.ui \
    dialogtimersettings.ui

QMAKE_CXXFLAGS += -std=gnu++14 -O3

RESOURCES += \
    resourse.qrc
