set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(SRC ../core/Can.cpp ../core/CanConfig.cpp
        ../core/CanBuffer.cpp ../core/Log.cpp
        ../core/CurveConfig.cpp ../core/Collect.cpp
        ../core/Revolve.cpp ../core/Kebab.cpp
        ../core/Sublime.cpp ../core/Keep.cpp
        ../core/Abstract.cpp ../core/Tribe.cpp
        ../core/Rate.cpp ../core/Criteria.cpp
        ../core/Refine.cpp ../core/Modes.cpp
        ../core/Constant.cpp)

set(LOG ../core/Log.cpp)

include_directories(../core)

#add_executable(test_can test_can.cpp ${SRC})
#target_link_libraries(test_can Qt5Core)
#target_link_libraries(test_can ${PROJECT_SOURCE_DIR}/lib/ControlCAN.LIB)
#
#add_executable(test_config test_config.cpp ${SRC})
#target_link_libraries(test_config Qt5Core)
#target_link_libraries(test_config ${PROJECT_SOURCE_DIR}/lib/ControlCAN.LIB)
#
#add_executable(test_slot test_slot.cpp my_head.cpp ../core/Log.cpp)
#target_link_libraries(test_slot Qt5Core)

#add_executable(test_collect test_collect.cpp ${SRC})
#target_link_libraries(test_collect Qt5Core)
#target_link_libraries(test_collect ${PROJECT_SOURCE_DIR}/lib/ControlCAN.LIB)

#add_executable(test_constructure test_constructure.cpp ${SRC})
#target_link_libraries(test_constructure ${PROJECT_SOURCE_DIR}/lib/ControlCAN.LIB)
#target_link_libraries(test_constructure Qt5Core)
#
#add_executable(test_bit test_bit.cpp ${LOG})
#target_link_libraries(test_bit Qt5Core)
#target_link_libraries(test_bit ${PROJECT_SOURCE_DIR}/lib/ControlCAN.LIB)
#
#add_executable(test_nas test_nas.cpp ${LOG})
#target_link_libraries(test_nas Qt5Core)

#add_executable(test_filter test_filter.cpp)
#target_link_libraries(test_filter D:/msys32/mingw32/lib/libgsl.a)

#add_executable(test_vector test_vector.cpp)
#target_link_libraries(test_vector Qt5Core)

add_executable(test_refine test_refine.cpp ${SRC})
target_link_libraries(test_refine Qt5Core)
target_link_libraries(test_refine ${PROJECT_SOURCE_DIR}/lib/ControlCAN.LIB)
target_link_libraries(test_refine D:/msys32/mingw32/lib/libgsl.a)

#add_executable(test_delete test_delete.cpp)
#target_link_libraries(test_str Qt5Core)

#add_executable(test_yaml test_yaml.cpp ${LOG})
#target_link_libraries(test_yaml Qt5Core)
#target_link_libraries(test_yaml D:/msys32/mingw32/lib/yaml-cpp.dll.a)

add_executable(test_json test_json.cpp)
target_link_libraries(test_json Qt5Core)

add_executable(test_right test_right.cpp my_right.h my_right.cpp)
target_link_libraries(test_right Qt5Core)
