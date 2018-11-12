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
               ./src/core \
               ./src/mod \
               ./src/del \

SOURCES += \
        ./src/gui/About.cpp \
        ./src/gui/ChangeLog.cpp \
        ./src/gui/ColorPicker.cpp \
        ./src/gui/CurveBox.cpp \
        ./src/gui/CurveEditor.cpp \
        ./src/gui/CurvePanel.cpp \
        ./src/gui/CurveView.cpp \
        ./src/gui/CurveViewer.cpp \
        ./src/gui/Display.cpp \
        ./src/gui/FilePicker.cpp \
        ./src/gui/FrameViewer.cpp \
        ./src/gui/LostFocus.cpp \
        ./src/gui/MarkBox.cpp \
        ./src/gui/MessagerPanel.cpp \
        ./src/gui/Output.cpp \
        ./src/gui/Refine.cpp \
        ./src/gui/Settings.cpp \
        ./src/gui/Sketch.cpp \
        ./src/gui/StatusBar.cpp \
        ./src/gui/Toolbox.cpp \
        ./src/gui/TribeView.cpp \
        ./src/core/Buffer.cpp \
        ./src/core/Can.cpp \
        ./src/core/Collect.cpp \
        ./src/core/Csv.cpp \
        ./src/core/Curve.cpp \
        ./src/core/File.cpp \
        ./src/core/FramePool.cpp \
        ./src/core/Initializer.cpp \
        ./src/core/Mark.cpp \
        ./src/core/Packer.cpp \
        ./src/core/Record.cpp \
        ./src/core/Revolve.cpp \
        ./src/core/Softcan.cpp \
        ./src/core/Transform.cpp \
        ./src/core/Transmit.cpp \
        ./src/core/Tribe.cpp \
        ./src/core/Trigger.cpp \
        ./src/core/Version.cpp \
        ./src/mod/CurveModel.cpp \
        ./src/mod/TribeModel.cpp \
        ./src/del/ByteInput.cpp \
        ./src/del/ByteRangeFrame.cpp \
        ./src/del/ColorInput.cpp \
        ./src/del/ComboInput.cpp \
        ./src/del/RangeWidget.cpp \
        ./src/del/SpecInput.cpp \
        ./src/test/main.cpp \

HEADERS += \
        ./src/gui/About.hpp \
        ./src/gui/ChangeLog.hpp \
        ./src/gui/ColorPicker.hpp \
        ./src/gui/CurveBox.hpp \
        ./src/gui/CurveEditor.hpp \
        ./src/gui/CurvePanel.hpp \
        ./src/gui/CurveView.hpp \
        ./src/gui/CurveViewer.hpp \
        ./src/gui/Display.hpp \
        ./src/gui/FilePicker.hpp \
        ./src/gui/FrameViewer.hpp \
        ./src/gui/LostFocus.hpp \
        ./src/gui/MarkBox.hpp \
        ./src/gui/MessagerPanel.hpp \
        ./src/gui/Output.hpp \
        ./src/gui/Refine.hpp \
        ./src/gui/Settings.hpp \
        ./src/gui/Sketch.hpp \
        ./src/gui/StatusBar.hpp \
        ./src/gui/Toolbox.hpp \
        ./src/gui/TribeView.hpp \
        ./src/core/Buffer.hpp \
        ./src/core/Can.hpp \
        ./src/core/Collect.hpp \
        ./src/core/Csv.hpp \
        ./src/core/Curve.hpp \
        ./src/core/File.hpp \
        ./src/core/FramePool.cpp \
        ./src/core/Initializer.hpp \
        ./src/core/Mark.hpp \
        ./src/core/Packer.hpp \
        ./src/core/Record.hpp \
        ./src/core/Revolve.hpp \
        ./src/core/Softcan.hpp \
        ./src/core/Transform.hpp \
        ./src/core/Transmit.hpp \
        ./src/core/Tribe.hpp \
        ./src/core/Trigger.hpp \
        ./src/core/Version.hpp \
        ./src/mod/CurveModel.hpp \
        ./src/mod/TribeModel.hpp \
        ./src/del/ByteInput.hpp \
        ./src/del/ByteRangeFrame.hpp \
        ./src/del/ColorInput.hpp \
        ./src/del/ComboInput.hpp \
        ./src/del/RangeWidget.hpp \
        ./src/del/SpecInput.hpp \

RESOURCES += \
    res/text.qrc \
    res/ui.qrc \
    res/trans.qrc

RC_FILE += res/refine.rc

TRANSLATIONS += \
        ./lang/zh.ts \
        ./lang/en.ts \


win32: LIBS += -L$$PWD/lib/ -lControlCAN
