# Roar Source Code
# Wasim Abbas
# http://www.waZim.com
# Copyright (c) 2008-2019
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the 'Software'),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the Software
# is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
# OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# Version: 1.0.0

set(ROAR_TEST_NAME roar_test)

set(ROAR_TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(ROAR_TEST_HEADERS
  ${ROAR_TEST_SOURCE_DIR}/math/vector2.hpp
  ${ROAR_TEST_SOURCE_DIR}/math/vector3.hpp
  ${ROAR_TEST_SOURCE_DIR}/math/vector4.hpp
  ${ROAR_TEST_SOURCE_DIR}/math/matrix2.hpp
  ${ROAR_TEST_SOURCE_DIR}/math/matrix3.hpp
  ${ROAR_TEST_SOURCE_DIR}/math/matrix4.hpp
  ${ROAR_TEST_SOURCE_DIR}/math/matrix3x4.hpp
  ${ROAR_TEST_SOURCE_DIR}/math/quaternion.hpp
  ${ROAR_TEST_SOURCE_DIR}/math/axis_angle.hpp
  ${ROAR_TEST_SOURCE_DIR}/math/euler_angle.hpp
  ${ROAR_TEST_SOURCE_DIR}/math/plane.hpp
  ${ROAR_TEST_SOURCE_DIR}/math/ray.hpp
  ${ROAR_TEST_SOURCE_DIR}/math/vector2.inl
  ${ROAR_TEST_SOURCE_DIR}/math/vector3.inl
  ${ROAR_TEST_SOURCE_DIR}/math/vector4.inl
  ${ROAR_TEST_SOURCE_DIR}/math/matrix2.inl
  ${ROAR_TEST_SOURCE_DIR}/math/matrix3.inl
  ${ROAR_TEST_SOURCE_DIR}/math/matrix4.inl
  ${ROAR_TEST_SOURCE_DIR}/math/matrix3x4.inl
  ${ROAR_TEST_SOURCE_DIR}/math/quaternion.inl
  ${ROAR_TEST_SOURCE_DIR}/math/axis_angle.inl
  ${ROAR_TEST_SOURCE_DIR}/math/euler_angle.inl
  ${ROAR_TEST_SOURCE_DIR}/math/plane.inl
  ${ROAR_TEST_SOURCE_DIR}/math/ray.inl)

set(ROAR_TEST_SOURCES
  ${ROAR_TEST_SOURCE_DIR}/roartests.cpp
  ${ROAR_TEST_SOURCE_DIR}/common.cpp
  ${ROAR_TEST_SOURCE_DIR}/graph.cpp
  ${ROAR_TEST_SOURCE_DIR}/log.cpp
  ${ROAR_TEST_SOURCE_DIR}/carray_vs_stdarray.cpp)

add_executable(${ROAR_TEST_NAME} ${ROAR_TEST_HEADERS} ${ROAR_TEST_SOURCES})

target_include_directories(${ROAR_TEST_NAME} PRIVATE
  ${ROAR_ROOT_DIR}/thirdparty
  ${ROAR_ROOT_DIR}/thirdparty/gtest/include
  ${ROAR_ROOT_DIR}/core
  ${ROAR_ROOT_DIR}/tests)

add_subdirectory(${ROAR_ROOT_DIR}/thirdparty/gtest ${CMAKE_CURRENT_BINARY_DIR}/gtest_binaries)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/gtest_binaries)

set(ROAR_REQUIRED_TEST_LIBRARIES ${ROAR_REQUIRED_LIBRARIES} roar spdlog gtest gtest_main)
target_link_libraries(${ROAR_TEST_NAME} ${ROAR_REQUIRED_TEST_LIBRARIES})

debug_options(${ROAR_TEST_NAME})

target_compile_definitions(${ROAR_TEST_NAME} PUBLIC ROAR_IMPORTING_SYMBOLS)
