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
  set(METAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp)

  find_library(MTL_FOUNDATION_LIBRARY Foundation)
  find_library(MTL_METAL_LIBRARY Metal)
  find_library(MTL_QUARTZCORE_LIBRARY QuartzCore)

  add_library(metalcpp ${CMAKE_CURRENT_SOURCE_DIR}/rormetal_common.cpp
	${METAL_INCLUDE_DIR}/Foundation/Foundation.hpp
	${METAL_INCLUDE_DIR}/Metal/Metal.hpp
	${METAL_INCLUDE_DIR}/QuartzCore/QuartzCore.hpp)
  target_link_libraries(metalcpp PUBLIC ${MTL_FOUNDATION_LIBRARY} ${MTL_METAL_LIBRARY} ${MTL_QUARTZCORE_LIBRARY})
  target_include_directories(metalcpp PUBLIC ${METAL_INCLUDE_DIR})
elseif (${ROR_RENDER_TYPE_DX12})
elseif (${ROR_RENDER_TYPE_GLES3})
endif()

# Add glslang
set(ENABLE_SPVREMAPPER OFF)
set(ENABLE_GLSLANG_BINARIES ON)
set(ENABLE_HLSL ON)
set(ENABLE_OPT OFF)
set(BUILD_TESTING OFF)
set(BUILD_EXTERNAL OFF)

cmake_policy(PUSH)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# We need glslang to build as static library
# if someone somewhere, i.e. another dependency in this case xxHash, sets BUILD_SHARED_LIBS to ON glslang will build as shared library
# but it doesn't work if used the way I am using it, linked into another shared library (roar) and then used by clients (editor)
# For some reason at Editor point, there are multiple copies of glslang loaded and some TLS variables PoolIndex in glslang is unset for the second run
add_subdirectory(glslang)
build_options(glslang false)                               # Set common build options

cmake_policy(POP)

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

# Add par
add_library(par INTERFACE)
set(PAR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/par)
target_sources(par INTERFACE ${PAR_INCLUDE_DIR}/par_octasphere.h)
target_include_directories(par INTERFACE ${PAR_INCLUDE_DIR})

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

# Add Dear ImGui
set(IMGUI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(IMGUI_HEADER_FILES
    ${IMGUI_INCLUDE_DIR}/imconfig.h
    ${IMGUI_INCLUDE_DIR}/imgui.h
    ${IMGUI_INCLUDE_DIR}/imgui_internal.h
    ${IMGUI_INCLUDE_DIR}/imstb_rectpack.h
    ${IMGUI_INCLUDE_DIR}/imstb_textedit.h
    ${IMGUI_INCLUDE_DIR}/imstb_truetype.h)
set(IMGUI_SOURCE_FILES
	${IMGUI_INCLUDE_DIR}/imgui.cpp
	${IMGUI_INCLUDE_DIR}/imgui_demo.cpp
	${IMGUI_INCLUDE_DIR}/imgui_draw.cpp
	${IMGUI_INCLUDE_DIR}/imgui_tables.cpp
	${IMGUI_INCLUDE_DIR}/imgui_widgets.cpp)

add_library(imgui STATIC ${IMGUI_SOURCE_FILES})
target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR})
target_sources(imgui PRIVATE ${IMGUI_SOURCE_FILES})

# Add Dear ImGuizmo
set(IMGUIZMO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imguizmo)
set(IMGUIZMO_HEADER_FILES
    ${IMGUIZMO_INCLUDE_DIR}/ImGuizmo.h)
set(IMGUIZMO_SOURCE_FILES
	${IMGUIZMO_INCLUDE_DIR}/ImGuizmo.cpp)

add_library(imguizmo STATIC ${IMGUIZMO_SOURCE_FILES})
target_include_directories(imguizmo PUBLIC imgui ${IMGUIZMO_INCLUDE_DIR})
target_sources(imguizmo PRIVATE ${IMGUIZMO_SOURCE_FILES})

# SPIRV-Cross
set(SPIRV_CROSS_STATIC ON)
set(SPIRV_CROSS_ENABLE_MSL  ON)
set(SPIRV_CROSS_ENABLE_REFLECT  ON)

set(SPIRV_CROSS_ENABLE_UTIL  OFF)
set(SPIRV_CROSS_SHARED OFF)
set(SPIRV_CROSS_CLI OFF)
set(SPIRV_CROSS_ENABLE_TESTS OFF)
set(SPIRV_CROSS_ENABLE_HLSL  OFF)
set(SPIRV_CROSS_ENABLE_CPP  OFF)
set(SPIRV_CROSS_ENABLE_C_API  OFF)
set(SPIRV_CROSS_SKIP_INSTALL ON)

add_subdirectory(spirv-cross)

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
