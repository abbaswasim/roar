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

# Add spdlog library
add_subdirectory(spdlog)
target_compile_options(spdlog INTERFACE "-Wno-float-equal")

set_target_properties(spdlog
  PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Add json library
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann)

# Add xxHash hashing library
cmake_policy (SET CMP0042 NEW)
set(XXHASH_BUILD_XXHSUM 0) # Don't need to build the cli
add_subdirectory(xxHash/cmake_unofficial)
# target_compile_options(xxhash PRIVATE XXH_INLINE_ALL)

# add_subdirectory(convhull_3d)
# target_compile_options(convhull_3d PRIVATE
#   "-Wno-gnu-anonymous-struct"
#   "-Wno-nested-anon-types")

# add_library(vulkan INTERFACE)
# set(VULKAN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vulkan/include)
# target_sources(vulkan INTERFACE ${VULKAN_INCLUDE_DIR}/vulkan/vulkan.h)
# target_include_directories(vulkan INTERFACE ${VULKAN_INCLUDE_DIR})

# target_compile_definitions(vulkan INTERFACE VK_NO_PROTOTYPES)
