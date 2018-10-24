#-------------------------------------------------
#
# Project created by QtCreator 2018-06-09T11:25:15
#
#-------------------------------------------------

#TEMPLATE = subdirs
#SUBDIRS = src
#CONFIG = ordered
QT       += core gui widgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = refine
QMAKE_LFLAGS = -static
QMAKE_LFLAGS_QT_DLL = -static
TEMPLATE = app

CONFIG += c++1z

SUBDIRS = src lib

INCLUDEPATH += ./src/gui \
               ./src/core

SOURCES += \
        ./src/gui/ColorPicker.cpp \
        ./src/gui/CurveBox.cpp \
        ./src/gui/CurveEditor.cpp \
        ./src/gui/Displayer.cpp \
        ./src/gui/FilePicker.cpp \
        ./src/gui/FrameViewer.cpp \
        ./src/gui/Messager.cpp \
        ./src/gui/Output.cpp \
        ./src/gui/Refine.cpp \
        ./src/gui/Splitter.cpp \
        ./src/gui/StatusBar.cpp \
        ./src/gui/Toolbox.cpp \
        ./src/core/Buffer.cpp \
        ./src/core/Can.cpp \
        ./src/core/Collect.cpp \
        ./src/core/Csv.cpp \
        ./src/core/Curve.cpp \
        ./src/core/Despatch.cpp \
        ./src/core/File.cpp \
        ./src/core/Initializer.cpp \
        ./src/core/Log.cpp \
        ./src/core/Mark.cpp \
        ./src/core/Record.cpp \
        ./src/core/Revolve.cpp \
        ./src/core/Softcan.cpp \
        ./src/core/Transform.cpp \
        ./src/core/Transmit.cpp \
        ./src/core/Tribe.cpp \
        ./src/core/Trigger.cpp \
        ./src/core/Version.cpp \
        ./src/test/main.cpp

HEADERS += \
        ./src/gui/ColorPicker.hpp \
        ./src/gui/CurveBox.hpp \
        ./src/gui/CurveEditor.hpp \
        ./src/gui/Displayer.hpp \
        ./src/gui/FilePicker.hpp \
        ./src/gui/FrameViewer.hpp \
        ./src/gui/Messager.hpp \
        ./src/gui/Output.hpp \
        ./src/gui/Refine.hpp \
        ./src/gui/Splitter.hpp \
        ./src/gui/StatusBar.hpp \
        ./src/gui/Toolbox.hpp \
        ./src/core/Buffer.hpp \
        ./src/core/Can.hpp \
        ./src/core/Collect.hpp \
        ./src/core/Csv.hpp \
        ./src/core/Curve.hpp \
        ./src/core/Despatch.hpp \
        ./src/core/File.hpp \
        ./src/core/Initializer.hpp \
        ./src/core/Log.hpp \
        ./src/core/Mark.hpp \
        ./src/core/Record.hpp \
        ./src/core/Revolve.hpp \
        ./src/core/Softcan.hpp \
        ./src/core/Transform.hpp \
        ./src/core/Transmit.hpp \
        ./src/core/Tribe.hpp \
        ./src/core/Trigger.hpp \
        ./src/core/Version.hpp \

RESOURCES += \
    res/ui.qrc

RC_FILE += res/refine.rc

TRANSLATIONS += \
        ./lang/zh.ts \
        ./lang/en.ts \


win32: LIBS += -L$$PWD/lib/ -lControlCAN
