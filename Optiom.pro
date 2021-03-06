#-------------------------------------------------
#
# Project created by QtCreator 2014-12-07T13:24:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Optiom
TEMPLATE = app

VERSION = 0.1
DEFINES += VER=$$VERSION

QMAKE_CXXFLAGS += -std=c++11 -Wall -pedantic

SOURCES += main.cpp\
    Mainwindow.cpp \
    KeyInputBox.cpp \

HEADERS  += Mainwindow.h \
    KeyStrings.h \
    KeyInputBox.h \

FORMS    += Mainwindow.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS += \
    optiom_ru.ts \
