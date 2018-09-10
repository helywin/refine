#-------------------------------------------------
#
# Project created by QtCreator 2018-06-09T11:25:15
#
#-------------------------------------------------
QT       += core gui widgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_window
QMAKE_LFLAGS = -static
QMAKE_LFLAGS_QT_DLL = -static
TEMPLATE = app

CONFIG += c++1z

SOURCES += \
        ./src/gui/About.cpp \
        ./src/gui/CurveDialog.cpp \
        ./src/gui/CurveTable.cpp \
        ./src/gui/Middle.cpp \
        ./src/gui/Painter.cpp \
        ./src/gui/PaintTest.cpp \
        ./src/gui/Select.cpp \
        ./src/gui/Sketch.cpp \
        ./src/gui/TableTips.cpp \
        ./src/gui/Window.cpp \
        ./src/gui/ModePanel.cpp \
        ./src/test/test_window.cpp

HEADERS += \
        ./src/gui/About.h \
        ./src/gui/CurveDialog.h \
        ./src/gui/CurveTable.h \
        ./src/gui/Middle.h \
        ./src/gui/Painter.h \
        ./src/gui/PaintTest.h \
        ./src/gui/Select.h \
        ./src/gui/Sketch.h \
        ./src/gui/TableTips.h \
        ./src/gui/Window.h \
        ./src/gui/ModePanel.h \
        ./src/core/Version.h

RESOURCES += \
    res/ui.qrc

RC_FILE += res/refine.rc

