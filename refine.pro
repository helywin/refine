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

TARGET = scope
QMAKE_LFLAGS = -static
QMAKE_LFLAGS_QT_DLL = -static
TEMPLATE = app

CONFIG += c++1z

SUBDIRS = src lib

INCLUDEPATH += ./src/gui \
               ./src/core

SOURCES += \
        ./src/gui/Display.cpp \
        ./src/gui/FrameViewer.cpp \
        ./src/gui/Scope.cpp \
        ./src/core/Buffer.cpp \
        ./src/core/Can.cpp \
        ./src/core/Collect.cpp \
        ./src/core/Csv.cpp \
        ./src/core/Curve.cpp \
        ./src/core/Despatch.cpp \
        ./src/core/File.cpp \
        ./src/core/Log.cpp \
        ./src/core/Revolve.cpp \
        ./src/core/Transform.cpp \
        ./src/core/Tribe.cpp \
        ./src/core/Trigger.cpp \
        ./src/core/Version.cpp \
        ./src/test/test_scope.cpp

HEADERS += \
        ./src/gui/Display.hpp \
        ./src/gui/FrameViewer.hpp \
        ./src/gui/Scope.hpp \
        ./src/core/Buffer.hpp \
        ./src/core/Can.hpp \
        ./src/core/Collect.hpp \
        ./src/core/Csv.hpp \
        ./src/core/Curve.hpp \
        ./src/core/Despatch.hpp \
        ./src/core/File.hpp \
        ./src/core/Log.hpp \
        ./src/core/Revolve.hpp \
        ./src/core/Transform.hpp \
        ./src/core/Tribe.hpp \
        ./src/core/Trigger.hpp \
        ./src/core/Version.hpp \

RESOURCES += \
    res/ui.qrc

RC_FILE += res/refine.rc



win32: LIBS += -L$$PWD/lib/ -lControlCAN


