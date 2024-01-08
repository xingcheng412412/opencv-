cmake_minimum_required(VERSION 3.0)

project(SOLIDERFIRE)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_BUILD_TYPE Debug)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(my_cmake_exe mian.cpp)
target_link_libraries(my_cmake_exe ${OpenCV_LIBS})