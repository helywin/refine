cmake_minimum_required(VERSION 3.12)
project(refine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MAJOR_VERSION 0)
set(CMAKE_MINOR_VERSION 0)
set(CMAKE_PATCH_VERSION 14)

#set(FILE_MD "README.md")
#add_custom_command(CONVERT_MD )

#add_subdirectory(${PROJECT_SOURCE_DIR}/src/core)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/test)
#add_subdirectory(${PROJECT_SOURCE_DIR}/src/gui)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib)
#add_subdirectory(${PROJECT_SOURCE_DIR}/src/tool)

