# Roar Source Code
# Wasim Abbas
# http://www.waZim.com
# Copyright (c) 2022
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

set(ROAR_EDITOR_NAME "RoarEditor")
set(ROAR_EDITOR_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(ROAR_EDITOR_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(ROAR_EDITOR_HEADERS
  ${ROAR_EDITOR_SOURCE_DIR}/roreditor.hpp)

set(ROAR_EDITOR_SOURCES
  ${ROAR_EDITOR_SOURCE_DIR}/roreditor.cpp)

add_executable(${ROAR_EDITOR_NAME} ${ROAR_EDITOR_SOURCES} ${ROAR_EDITOR_HEADERS})
set_target_properties(${ROAR_EDITOR_NAME} PROPERTIES VERSION 1.0.0.0 SOVERSION 1)          # TODO: Fix the hardcoded version
set_target_properties(${ROAR_EDITOR_NAME} PROPERTIES CMAKE_VISIBILITY_INLINES_HIDDEN true) # Don't export all of those C++ template generated symbols
set_target_properties(${ROAR_EDITOR_NAME} PROPERTIES CMAKE_CXX_VISIBILITY_PRESET hidden)   # Set default to hide symbols visibility for Roar Editor
set_target_properties(${ROAR_EDITOR_NAME} PROPERTIES CMAKE_C_VISIBILITY_PRESET hidden)     # Set default to hide symbols visibility for Roar Editor

target_include_directories(${ROAR_EDITOR_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories(${ROAR_EDITOR_NAME} PUBLIC ${ROAR_EDITOR_SOURCE_DIR})
target_include_directories(${ROAR_EDITOR_NAME} PUBLIC ${ROAR_EDITOR_ROOT_DIR})

set(ROAR_EDITOR_REQUIRED_PUBLIC_LIBRARIES "")
set(ROAR_EDITOR_REQUIRED_PRIVATE_LIBRARIES ${ROAR_NAME})

target_link_libraries(${ROAR_EDITOR_NAME} PRIVATE
  ${ROAR_EDITOR_REQUIRED_PRIVATE_LIBRARIES}
  ${ROAR_EDITOR_REQUIRED_PUBLIC_LIBRARIES})

build_options(${ROAR_EDITOR_NAME} true)                               # Set common build options
