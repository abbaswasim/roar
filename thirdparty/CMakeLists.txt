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

set_target_properties(xxhash
  PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Add CImg library as interface
add_library(cimage INTERFACE)
target_sources(cimage INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/CImg/CImg.h)
target_include_directories(cimage SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Add stb library as interface
# TODO: Check if SSE2 support is triggered or not
# TODO: For ARM Neon must be explicitely requested for stb image
add_library(stb INTERFACE)
target_sources(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.h)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

if (${ROR_RENDER_TYPE_VULKAN})
  # Add vulkan
  add_library(vulkan_headers INTERFACE)
  set(VULKAN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vulkan/include)
  target_sources(vulkan_headers INTERFACE ${VULKAN_INCLUDE_DIR}/vulkan/vulkan.h)
  target_include_directories(vulkan_headers INTERFACE ${VULKAN_INCLUDE_DIR})
elseif (${ROR_RENDER_TYPE_METAL})
  # Add MetalCPP
  add_library(metalcpp_headers INTERFACE)
  set(METAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp)
  target_sources(metalcpp_headers INTERFACE ${METAL_INCLUDE_DIR}/Metal/Metal.hpp)
  target_include_directories(metalcpp_headers INTERFACE ${METAL_INCLUDE_DIR})
elseif (${ROR_RENDER_TYPE_DX12})
elseif (${ROR_RENDER_TYPE_GLES3})
endif()

# Add basis
set(BASIS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/basis_universal)
set(BASIS_SOURCE_FILES
	${BASIS_DIR}/transcoder/basisu_transcoder.cpp
	${BASIS_DIR}/transcoder/basisu_transcoder.h
	${BASIS_DIR}/zstd/zstddeclib.c)

add_library(basisu STATIC ${BASIS_SOURCE_FILES})
target_include_directories(basisu PUBLIC ${BASIS_DIR})
target_include_directories(basisu PUBLIC ${BASIS_DIR}/transcoder)
target_compile_options(basisu PUBLIC "-fno-strict-aliasing")
set_property(TARGET basisu PROPERTY POSITION_INDEPENDENT_CODE ON)

# Add cgltf
add_library(cgltf INTERFACE)
set(CGLTF_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cgltf)
target_sources(cgltf INTERFACE ${CGLTF_INCLUDE_DIR}/cgltf.h)
target_include_directories(cgltf INTERFACE ${CGLTF_INCLUDE_DIR})

# Add CTPL
add_library(ctpl INTERFACE)
set(CTPL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CTPL)
target_sources(ctpl INTERFACE ${CTPL_INCLUDE_DIR}/ctpl_stl.h)
target_include_directories(ctpl INTERFACE ${CTPL_INCLUDE_DIR})

# add_subdirectory(convhull_3d)
# target_compile_options(convhull_3d PRIVATE
#   "-Wno-gnu-anonymous-struct"
#   "-Wno-nested-anon-types")

# add_library(vulkan INTERFACE)
# set(VULKAN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vulkan/include)
# target_sources(vulkan INTERFACE ${VULKAN_INCLUDE_DIR}/vulkan/vulkan.h)
# target_include_directories(vulkan INTERFACE ${VULKAN_INCLUDE_DIR})

# target_compile_definitions(vulkan INTERFACE VK_NO_PROTOTYPES)

# Add glfw for windowing
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
