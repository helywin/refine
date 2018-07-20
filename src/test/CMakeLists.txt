set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/core CORE)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/gui GUI)
include_directories(${PROJECT_SOURCE_DIR}/src/core)
include_directories(${PROJECT_SOURCE_DIR}/src/gui)

set(LOG ${PROJECT_SOURCE_DIR}/src/core/Log.cpp)
set(LIBCAN ${PROJECT_SOURCE_DIR}/lib/ControlCAN.LIB)
set(LIBGSL D:/msys32/mingw32/lib/libgsl.a)

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

#add_executable(test_collect test_collect.cpp ${CORE})
#target_link_libraries(test_collect Qt5Core)
#target_link_libraries(test_collect ${LIBCAN})

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


add_executable(test_json test_json.cpp)
target_link_libraries(test_json Qt5Core)
add_executable(test_right test_right.cpp my_right.h my_right.cpp)
target_link_libraries(test_right Qt5Core)

add_executable(test_window test_window.cpp ${GUI})
target_link_libraries(test_window Qt5Widgets Qt5Core Qt5Gui)