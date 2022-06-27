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

set(ROAR_TEST_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

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
  ${ROAR_TEST_SOURCE_DIR}/math/segment.hpp
  ${ROAR_TEST_SOURCE_DIR}/geometry/geometry.hpp
  ${ROAR_TEST_SOURCE_DIR}/math/vector2.hh
  ${ROAR_TEST_SOURCE_DIR}/math/vector3.hh
  ${ROAR_TEST_SOURCE_DIR}/math/vector4.hh
  ${ROAR_TEST_SOURCE_DIR}/math/matrix2.hh
  ${ROAR_TEST_SOURCE_DIR}/math/matrix3.hh
  ${ROAR_TEST_SOURCE_DIR}/math/matrix4.hh
  ${ROAR_TEST_SOURCE_DIR}/math/matrix3x4.hh
  ${ROAR_TEST_SOURCE_DIR}/math/quaternion.hh
  ${ROAR_TEST_SOURCE_DIR}/math/axis_angle.hh
  ${ROAR_TEST_SOURCE_DIR}/math/euler_angle.hh
  ${ROAR_TEST_SOURCE_DIR}/math/plane.hh
  ${ROAR_TEST_SOURCE_DIR}/math/ray.hh
  ${ROAR_TEST_SOURCE_DIR}/math/segment.hh
  ${ROAR_TEST_SOURCE_DIR}/geometry/geometry.hh)

set(ROAR_TEST_SOURCES
  ${ROAR_TEST_SOURCE_DIR}/roartests.cpp
  ${ROAR_TEST_SOURCE_DIR}/common.cpp
  ${ROAR_TEST_SOURCE_DIR}/general.cpp
  ${ROAR_TEST_SOURCE_DIR}/graph.cpp
  ${ROAR_TEST_SOURCE_DIR}/log.cpp
  ${ROAR_TEST_SOURCE_DIR}/watchcat.cpp
  ${ROAR_TEST_SOURCE_DIR}/jobsystem.cpp
  ${ROAR_TEST_SOURCE_DIR}/eventsystem.cpp
  ${ROAR_TEST_SOURCE_DIR}/configuration/configuration.cpp
  ${ROAR_TEST_SOURCE_DIR}/rhi/shader_buffer.cpp
  ${ROAR_TEST_SOURCE_DIR}/vertex_description/vertex_description.cpp
  ${ROAR_TEST_SOURCE_DIR}/carray_vs_stdarray.cpp)

add_executable(${ROAR_TEST_NAME} ${ROAR_TEST_HEADERS} ${ROAR_TEST_SOURCES})

target_include_directories(${ROAR_TEST_NAME} PRIVATE
  ${ROAR_ROOT_DIR}
  ${ROAR_ROOT_DIR}/thirdparty
  ${ROAR_ROOT_DIR}/core
  ${ROAR_ROOT_DIR}/tests)

# target_include_directories_system(${ROAR_TEST_NAME} PUBLIC ${ROAR_ROOT_DIR}/thirdparty/gtest/googletest/include/)

add_subdirectory(${ROAR_ROOT_DIR}/thirdparty/gtest ${CMAKE_CURRENT_BINARY_DIR}/gtest_binaries)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/gtest_binaries)

set(ROAR_REQUIRED_TEST_LIBRARIES ${ROAR_REQUIRED_LIBRARIES} roar spdlog)
set(ROAR_REQUIRED_TEST_SYSTEM_LIBRARIES gtest_main)

target_link_libraries(${ROAR_TEST_NAME} PRIVATE ${ROAR_REQUIRED_TEST_LIBRARIES})
target_link_libraries_system(${ROAR_TEST_NAME} PRIVATE ${ROAR_REQUIRED_TEST_SYSTEM_LIBRARIES})

# Does this make any different, first tests didn't find any, explore some day
target_precompile_headers(${ROAR_TEST_NAME} PRIVATE <gtest/gtest.h> <vector> <array> <complex> <map> <optional> <iostream> <iomanip> <memory>)

build_options(${ROAR_TEST_NAME})

target_compile_definitions(${ROAR_TEST_NAME} PUBLIC ROAR_IMPORTING_SYMBOLS)
