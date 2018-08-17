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
include_directories(${PROJECT_SOURCE_DIR}/src/core)
include_directories(${PROJECT_SOURCE_DIR}/src/gui)

set(SRC ${PROJECT_SOURCE_DIR}/res/ui.qrc)
set(LIBLOG ${PROJECT_SOURCE_DIR}/src/core/Log.cpp)
set(LIBCAN ${PROJECT_SOURCE_DIR}/lib/ControlCAN.LIB)
set(LIBGSL D:/msys32/mingw32/lib/libgsl.a)
set(LIBOMP D:/msys32/mingw32/lib/gcc/i686-w64-mingw32/7.3.0/libgomp.a)

#add_executable(test_can test_can.cpp ${CORE})
#target_link_libraries(test_can Qt5Core)
#target_link_libraries(test_can ${LIBCAN})
#
#add_executable(test_config test_config.cpp ${CORE})
#target_link_libraries(test_config Qt5Core)
#target_link_libraries(test_config ${LIBCAN})
#
#add_executable(test_slot test_slot.cpp my_head.cpp ../core/Log.cpp)
#target_link_libraries(test_slot Qt5Core)

add_executable(test_collect test_collect.cpp ${CORE} ${SRC})
target_link_libraries(test_collect Qt5Core)
target_link_libraries(test_collect ${LIBCAN} ${LIBGSL} ${LIBOMP})

#add_executable(test_constructure test_constructure.cpp ${CORE})
#target_link_libraries(test_constructure ${LIBCAN})
#target_link_libraries(test_constructure Qt5Core)
#
#add_executable(test_bit test_bit.cpp ${LOG})
#target_link_libraries(test_bit Qt5Core)
#target_link_libraries(test_bit ${LIBCAN})
#
#add_executable(test_nas test_nas.cpp ${LOG})
#target_link_libraries(test_nas Qt5Core)

#add_executable(test_filter test_filter.cpp)
#target_link_libraries(test_filter D:/msys32/mingw32/lib/libgsl.a)

#add_executable(test_vector test_vector.cpp)
#target_link_libraries(test_vector Qt5Core)

add_executable(test_refine test_refine.cpp ${CORE})
target_link_libraries(test_refine
        Qt5Core
        ${LIBCAN}
        ${LIBGSL})


#add_executable(test_delete test_delete.cpp)
#target_link_libraries(test_str Qt5Core)

#add_executable(test_yaml test_yaml.cpp ${LOG})
#target_link_libraries(test_yaml Qt5Core)
#target_link_libraries(test_yaml D:/msys32/mingw32/lib/yaml-cpp.dll.a)


#add_executable(test_json test_json.cpp)
#target_link_libraries(test_json Qt5Core)
#add_executable(test_right test_right.cpp my_right.h my_right.cpp)
#target_link_libraries(test_right Qt5Core)

add_executable(test_window test_window.cpp ${GUI} ${SRC})
target_link_libraries(test_window Qt5Widgets Qt5Core Qt5Gui Qt5OpenGL)

#add_executable(test_gl test_gl.cpp)
#target_link_libraries(test_gl D:/msys32/mingw32/lib/libglfw3.a
#        D:/software/mingw32/i686-w64-mingw32/lib/libopengl32.a
#        D:/software/mingw32/i686-w64-mingw32/lib/libgcc_s.a
#        D:/software/mingw32/i686-w64-mingw32/lib/libpthread.a)
#set_target_properties(test_gl PROPERTIES LINK_FLAGS "-Wl,-s -static -Wl,-subsystem,windows -mthreads")
#add_executable(test_release test_release.cpp)
#target_link_libraries(test_release Qt5Core)
#set_target_properties(TARGET test_release PROPERTY )