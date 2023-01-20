cmake_minimum_required(VERSION 3.20)

# set the project name
project(ACS_Proj1 VERSION 0.0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set startup project for sln files
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

add_subdirectory(zstd/build/cmake/lib)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE zstd/lib)
target_link_libraries(${PROJECT_NAME} zstd)