cmake_minimum_required(VERSION 3.11)
project(refine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
#if(CMAKE_BUILD_TYPE MATCHES Release)
##    QT_QMAKE_EXECUTABLE(D:/msys32/mingw32/qt5-static/bin/qmake.exe)
##    QT_MKSPECS_DIR(D:/msys32/mingw32/qt5-static/share/qt5/mkspecs)
#    link_directories(D:/msys32/mingw32/qt5-static/lib)
#    set(Qt5Core_DIR D:/msys32/mingw32/qt5-static/lib/cmake/Qt5Core)
#    set(Qt5Widgets_DIR D:/msys32/mingw32/qt5-static/lib/cmake/Qt5Widgets)
#    set(Qt5Gui_DIR D:/msys32/mingw32/qt5-static/lib/cmake/Qt5Gui)
#endif(CMAKE_BUILD_TYPE MATCHES Release)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5OpenGL CONFIG REQUIRED)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)
set(DEFINES "-DUNICODE -D_UNICODE -DWIN32 -DQT_DEPRECATED_WARNINGS")
set(CMAKE_C_FLAGS "-fno-keep-inline-dllexport -fopenmp -march=i686 -mtune=core2 -Wa,-mbig-obj -O2 -Wall -W -Wextra ${DEFINES}")
set(CMAKE_CXX_FLAGS "-fno-keep-inline-dllexport -fopenmp -O2 -g -Wall -W -Wextra -fexceptions -mthreads ${DEFINES}")

if (CMAKE_BUILD_TYPE MATCHES Release)
    message("release compile!!")
    #    set(LFLAG "-lmingw32 -LD:/msys32/mingw32/qt5-static/lib D:/msys32/mingw32/qt5-static/lib/libqtmain.a -LD:/msys32/mingw32/qt5-static/share/qt5/plugins/styles D:/msys32/mingw32/qt5-static/share/qt5/plugins/styles/libqwindowsvistastyle.a -LD:/msys32/mingw32/qt5-static/share/qt5/plugins/platforms D:/msys32/mingw32/qt5-static/share/qt5/plugins/platforms/libqwindows.a -lwinspool -lshlwapi D:/msys32/mingw32/qt5-static/lib/libQt5EventDispatcherSupport.a D:/msys32/mingw32/qt5-static/lib/libQt5FontDatabaseSupport.a D:/msys32/mingw32/qt5-static/lib/libqtfreetype.a D:/msys32/mingw32/qt5-static/lib/libQt5ThemeSupport.a D:/msys32/mingw32/qt5-static/lib/libQt5AccessibilitySupport.a D:/msys32/mingw32/qt5-static/lib/libQt5VulkanSupport.a D:/msys32/mingw32/qt5-static/lib/libQt5WindowsUIAutomationSupport.a -LD:/msys32/mingw32/qt5-static/share/qt5/plugins/imageformats D:/msys32/mingw32/qt5-static/share/qt5/plugins/imageformats/libqgif.a D:/msys32/mingw32/qt5-static/share/qt5/plugins/imageformats/libqicns.a D:/msys32/mingw32/qt5-static/share/qt5/plugins/imageformats/libqico.a D:/msys32/mingw32/qt5-static/share/qt5/plugins/imageformats/libqjp2.a -ljasper D:/msys32/mingw32/qt5-static/share/qt5/plugins/imageformats/libqjpeg.a D:/msys32/mingw32/qt5-static/share/qt5/plugins/imageformats/libqtga.a D:/msys32/mingw32/qt5-static/share/qt5/plugins/imageformats/libqtiff.a D:/msys32/mingw32/qt5-static/share/qt5/plugins/imageformats/libqwbmp.a D:/msys32/mingw32/qt5-static/share/qt5/plugins/imageformats/libqwebp.a D:/msys32/mingw32/qt5-static/lib/libQt5Widgets.a -luxtheme -ldwmapi D:/msys32/mingw32/qt5-static/lib/libQt5Gui.a -lcomdlg32 -loleaut32 -limm32 -lglu32 -lopengl32 -lgdi32 D:/msys32/mingw32/qt5-static/lib/libqtlibpng.a D:/msys32/mingw32/qt5-static/lib/libqtharfbuzz.a D:/msys32/mingw32/qt5-static/lib/libQt5Core.a -lmpr -lnetapi32 -luserenv -lversion -lws2_32 -lkernel32 -luser32 -lshell32 -luuid -lole32 -ladvapi32 -lwinmm D:/msys32/mingw32/qt5-static/lib/libqtpcre2.a")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-Wl,-s -Wl,-subsystem,windows -mthreads")
    #    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-static -Wl,-s -static -Wl,-subsystem,windows -mthreads ${LFLAG}")
    #    include_directories(D:/msys32/mingw32/qt5-static/include)
    #    set(QT_STATIC D:/msys32/mingw32/qt5-static)
    #    set(Qt5Core ${QT_STATIC}/liblibQt5Core.a)
    #    set(Qt5Widgets ${QT_STATIC}/libQt5Widgets.a)
    #    set(Qt5CoreGui ${QT_STATIC}/liblibQtGui.a)
    #    set(Qt5Other ${QT_STATIC}/liblibQtGui.a)
    add_definitions(-DQT_NO_DEBUG)
endif (CMAKE_BUILD_TYPE MATCHES Release)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message("debug compile!!")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-mthreads")
endif (CMAKE_BUILD_TYPE MATCHES Debug)

set(LIBCAN ${PROJECT_SOURCE_DIR}/lib/ControlCAN.LIB)

aux_source_directory(. SRC)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(main main.cpp ${SRC})
target_link_libraries(main
        Qt5Core
        Qt5Widgets
        Qt5Gui
        Qt5OpenGL
        ${LIBCAN})