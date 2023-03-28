cmake_minimum_required(VERSION 3.10)

project(main VERSION 1.0)

find_package(Boost 1.71 REQUIRED)

message(STATUS ${Boost_FOUND})
if (${Boost_FOUND})
    message(STATUS "Boost version: " ${Boost_VERSION_STRING})
    message(STATUS "Boost inlcude dir: " ${Boost_INCLUDE_DIRS})
    message(STATUS "Boost library dir: " ${Boost_LIBRARY_DIRS})
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} main.cpp)
else()
    message(STATUS "Boost not found!")
endif()

message(STATUS "")
