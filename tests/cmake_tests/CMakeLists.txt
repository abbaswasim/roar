cmake_minimum_required (VERSION 3.12)

project(RoarEngine VERSION 1.1.0.0 LANGUAGES CXX C)
list(APPEND CMAKE_MESSAGE_CONTEXT Roar_Engine)

add_executable(cmake_test ${CMAKE_SOURCE_DIR}/main.cpp)
