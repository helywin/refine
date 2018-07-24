#-------------------------------------------------
#
# Project created by QtCreator 2018-06-09T11:25:15
#
#-------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_window
TEMPLATE = app

CONFIG += c++1z

SOURCES += \
        ./src/gui/Window.cpp \
        ./src/gui/About.cpp \
        ./src/gui/Curve.cpp \
        ./src/gui/CurveTable.cpp \
        ./src/gui/Middle.cpp \
        ./src/gui/Select.cpp \
        ./src/test/test_window.cpp

HEADERS += \
        ./src/gui/Window.h \
        ./src/gui/About.h \
        ./src/gui/Curve.h \
        ./src/gui/CurveTable.h \
        ./src/gui/Middle.h \
        ./src/gui/Select.h

RESOURCES += \
    res/ui.qrc

