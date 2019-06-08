cmake_minimum_required(VERSION 2.8)
set(src_base
    Condition.cpp
    CountDownLatch.cpp
    CurrentThread.cpp
    Exception.cpp
    Timestamp.cpp
    Thread.cpp
    )
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_definitions(-std=c++11 -g)
add_library(spero_base STATIC ${src_base})
target_link_libraries(spero_base pthread)

add_subdirectory(tests)