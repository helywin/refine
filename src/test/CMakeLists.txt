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
find_package(Qt5PrintSupport CONFIG REQUIRED)
find_package(Qt5TextToSpeech CONFIG REQUIRED)
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
#add_definitions(-DQT_DEPRECATED_WARNINGS)
#add_definitions(-DUNICODE)
#add_definitions(-D_UNICODE)
#add_definitions(-DWIN32)
#set(CMAKE_CXX_FLAGS "-DQT_NO_DEBUG -c -O3")
#set(CMAKE_BUILD_TYPE Release CACHE STRING "set build type to debug")

#if(CMAKE_BUILD_TYPE MATCHES Release)
#    set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
#    set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
#    set_target_properties(Qt5::Widgets PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
#    set_target_properties(Qt5::Gui PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
#endif(CMAKE_BUILD_TYPE MATCHES Release)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/core CORE)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/gui GUI)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/mod MOD)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/del DEL)
include_directories(${PROJECT_SOURCE_DIR}/src/core)
include_directories(${PROJECT_SOURCE_DIR}/src/gui)
include_directories(${PROJECT_SOURCE_DIR}/src/mod)
include_directories(${PROJECT_SOURCE_DIR}/src/del)
#include_directories("C:/Program Files/Microsoft SDKs/Speech/v11.0/Include")

set(UI ${PROJECT_SOURCE_DIR}/res/ui.qrc)
set(TEXT ${PROJECT_SOURCE_DIR}/res/text.qrc)
set(TRANS ${PROJECT_SOURCE_DIR}/res/trans.qrc)
set(ICONS ${PROJECT_SOURCE_DIR}/res/icons.qrc)
set(RC ${PROJECT_SOURCE_DIR}/res/refine.rc)


set(LIBCAN ${PROJECT_SOURCE_DIR}/lib/ControlCAN.LIB)
set(LIBGSL D:/msys32/mingw32/lib/libgsl.a)
set(LIBOPENGL D:/msys32/mingw32/i686-w64-mingw32/lib/libopengl32.a)
set(LIBOMP D:/msys32/mingw32/lib/gcc/i686-w64-mingw32/7.3.0/libgomp.dll.a)
set(LIBZIP D:/msys32/mingw32/lib/libzip.dll.a)
set(LIBZ D:/msys32/mingw32/lib/libz.dll.a)
set(LIBBZ2 D:/msys32/mingw32/lib/libbz2.dll.a)
set(LIBFREEGLUT D:/msys32/mingw32/lib/libfreeglut.dll.a)
set(LIBNETTLE D:/msys32/mingw32/lib/libnettle.dll.a)
set(LIBGNUTLS D:/msys32/mingw32/lib/libgnutls.dll.a)

add_executable(file test_file.cpp)
target_link_libraries(file
        Qt5Core)

add_executable(test_gl test_gl.cpp MyOpenGLWidget.cpp MyOpenGLWidget.hpp)
target_link_libraries(test_gl
        Qt5Core
        Qt5Widgets
        Qt5OpenGL
        Qt5Gui
        ${LIBOPENGL})

#add_executable(collect test_collect.cpp ${CORE})
#target_link_libraries(collect ${LIBCAN}
#        Qt5Core)
#
#add_executable(frame frame_viewer_main.cpp ${CORE} ${GUI})
#target_link_libraries(frame ${LIBCAN}
#        Qt5Core Qt5Widgets Qt5Gui Qt5OpenGL ${LIBOPENGL})
#
#add_executable(scope test_scope.cpp ${CORE} ${GUI})
#target_link_libraries(scope ${LIBCAN}
#        Qt5Core Qt5Widgets Qt5Gui Qt5OpenGL ${LIBOPENGL})
#
#add_executable(buffer test_buffer.cpp ${CORE} ${GUI})
#target_link_libraries(buffer ${LIBCAN}
#        Qt5Core Qt5Widgets Qt5Gui Qt5OpenGL ${LIBOPENGL})
#
#add_executable(thread test_thread.cpp TestThread.h)
#target_link_libraries(thread Qt5Core)
#
#add_executable(setting test_setting.cpp ../core/Initializer.cpp ../core/Initializer.hpp)
#target_link_libraries(setting Qt5Core)

#add_executable(test_zip test_zip.cpp ../core/Packer.cpp ../core/Packer.hpp)
#target_link_libraries(test_zip Qt5Core
#        ${LIBZIP}
#        ${LIBZ}
#        ${LIBBZ2}
#        ${LIBNETTLE}
#        ${LIBGNUTLS})

add_executable(refine main.cpp ${RC} ${CORE} ${GUI} ${MOD} ${DEL}
        ${UI} ${TEXT} ${TRANS} ${ICONS})
target_link_libraries(refine
        ${LIBCAN}
        ${LIBOPENGL}
        ${LIBOMP}
        ${LIBFREEGLUT}
        Qt5Core
        Qt5Widgets
        Qt5Gui
        Qt5OpenGL)


if (CMAKE_BUILD_TYPE MATCHES Release)
    set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/release)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/debug)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
endif ()