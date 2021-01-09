cmake_minimum_required(VERSION 3.17)
project(CalTrain C)

set(CMAKE_C_STANDARD 99)
set (SOURCE_EXE caltrain.c caltrain-runner.c caltrain.h)
add_executable(CalTrain ${SOURCE_EXE})
find_package(Threads REQUIRED)
target_link_libraries(CalTrain pthread)