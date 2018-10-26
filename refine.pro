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
        ./src/gui/colorpicker.cpp \
        ./src/gui/curvebox.cpp \
        ./src/gui/curveeditor.cpp \
        ./src/gui/curveviewer.cpp \
        ./src/gui/display.cpp \
        ./src/gui/filepicker.cpp \
        ./src/gui/frameviewer.cpp \
        ./src/gui/markbox.cpp \
        ./src/gui/messager.cpp \
        ./src/gui/output.cpp \
        ./src/gui/refine.cpp \
        ./src/gui/sketch.cpp \
        ./src/gui/statusbar.cpp \
        ./src/gui/toolbox.cpp \
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
        ./src/gui/colorpicker.hpp \
        ./src/gui/curvebox.hpp \
        ./src/gui/curveeditor.hpp \
        ./src/gui/curveviewer.hpp \
        ./src/gui/display.hpp \
        ./src/gui/filepicker.hpp \
        ./src/gui/frameviewer.hpp \
        ./src/gui/markbox.hpp \
        ./src/gui/messager.hpp \
        ./src/gui/output.hpp \
        ./src/gui/refine.hpp \
        ./src/gui/sketch.hpp \
        ./src/gui/statusbar.hpp \
        ./src/gui/toolbox.hpp \
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
        ./src/core/Version.hpp

RESOURCES += \
    res/ui.qrc

RC_FILE += res/refine.rc

TRANSLATIONS += \
        ./lang/zh.ts \
        ./lang/en.ts \


win32: LIBS += -L$$PWD/lib/ -lControlCAN
