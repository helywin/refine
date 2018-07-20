set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/core CORE)
include_directories(${PROJECT_SOURCE_DIR}/src/core)