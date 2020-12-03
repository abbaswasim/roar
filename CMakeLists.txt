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

cmake_minimum_required (VERSION 3.12)

project(RoarEngine VERSION 1.1.0.0 LANGUAGES CXX C)
list(APPEND CMAKE_MESSAGE_CONTEXT Roar_Engine)

set(ROAR_NAME roar)

set(ROAR_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}) # Always prefer the _LIST_ version FILES/DIRS instead of the _SOURCE_
set(ROAR_SOURCE_DIR ${ROAR_ROOT_DIR}/core)

include(${ROAR_ROOT_DIR}/build.cmake)

# Always generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
add_subdirectory(${ROAR_ROOT_DIR}/tests)
add_subdirectory(${ROAR_ROOT_DIR}/thirdparty)
add_subdirectory(${ROAR_SOURCE_DIR})

# add_custom_command(
  # TARGET ${ROAR_NAME} POST_BUILD
  # COMMENT "Copying compile_commands.json to root of the target so that ycmd can see it"
  # COMMAND ${CMAKE_COMMAND} -E copy_if_different compile_commands.json ${ROAR_ROOT_DIR}
  # VERBATIM)

# Copy compile_commands.json to root so that ycmd can see it, maybe better fix how ycmd picks this?
# configure_file(${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json /tmp/roar_build COPYONLY)
