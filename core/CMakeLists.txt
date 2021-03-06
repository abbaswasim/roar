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

# Configure version source file roar_version.cpp
configure_file(${ROAR_SOURCE_DIR}/roar_version.cpp.in roar_version.cpp @ONLY)
add_library(roar_version STATIC ${CMAKE_CURRENT_BINARY_DIR}/roar_version.cpp)
target_include_directories(roar_version PUBLIC ${CMAKE_CURRENT_LIST_DIR})

set(ROAR_HEADERS
  ${ROAR_SOURCE_DIR}/profiling/rorlog.hpp
  ${ROAR_SOURCE_DIR}/profiling/rorlog.hh
  ${ROAR_SOURCE_DIR}/profiling/rortimer.hpp
  ${ROAR_SOURCE_DIR}/profiling/rortimer.hh
  ${ROAR_SOURCE_DIR}/foundation/rormacros.hpp
  ${ROAR_SOURCE_DIR}/foundation/rorcompiler_workarounds.hpp
  ${ROAR_SOURCE_DIR}/foundation/rorjobsystem.hpp
  ${ROAR_SOURCE_DIR}/foundation/rorjobsystem.hh
  ${ROAR_SOURCE_DIR}/foundation/rortypes.hpp
  ${ROAR_SOURCE_DIR}/foundation/rorutilities.hpp
  ${ROAR_SOURCE_DIR}/foundation/rorsystem.hpp
  ${ROAR_SOURCE_DIR}/foundation/rorrandom.hpp
  ${ROAR_SOURCE_DIR}/foundation/rorrandom.hh
  ${ROAR_SOURCE_DIR}/foundation/rorcommon.hpp
  ${ROAR_SOURCE_DIR}/common_structure/rorgraph.hpp
  ${ROAR_SOURCE_DIR}/common_structure/rorgraph.hh
  ${ROAR_SOURCE_DIR}/math/rorvector2.hpp
  ${ROAR_SOURCE_DIR}/math/rorvector2.hh
  ${ROAR_SOURCE_DIR}/math/rorvector3.hpp
  ${ROAR_SOURCE_DIR}/math/rorvector3.hh
  ${ROAR_SOURCE_DIR}/math/rorvector4.hpp
  ${ROAR_SOURCE_DIR}/math/rorvector4.hh
  ${ROAR_SOURCE_DIR}/math/rorvector_functions.hpp
  ${ROAR_SOURCE_DIR}/math/rorvector_functions.hh
  ${ROAR_SOURCE_DIR}/math/rorray.hpp
  ${ROAR_SOURCE_DIR}/math/rorray.hh
  ${ROAR_SOURCE_DIR}/math/rormatrix2.hpp
  ${ROAR_SOURCE_DIR}/math/rormatrix2.hh
  ${ROAR_SOURCE_DIR}/math/rormatrix3.hpp
  ${ROAR_SOURCE_DIR}/math/rormatrix3.hh
  ${ROAR_SOURCE_DIR}/math/rormatrix4.hpp
  ${ROAR_SOURCE_DIR}/math/rormatrix4.hh
  ${ROAR_SOURCE_DIR}/math/rormatrix3x4.hpp
  ${ROAR_SOURCE_DIR}/math/rormatrix3x4.hh
  ${ROAR_SOURCE_DIR}/math/rormatrix2_functions.hpp
  ${ROAR_SOURCE_DIR}/math/rormatrix2_functions.hh
  ${ROAR_SOURCE_DIR}/math/rormatrix3_functions.hpp
  ${ROAR_SOURCE_DIR}/math/rormatrix3_functions.hh
  ${ROAR_SOURCE_DIR}/math/rormatrix4_functions.hpp
  ${ROAR_SOURCE_DIR}/math/rormatrix4_functions.hh
  ${ROAR_SOURCE_DIR}/math/rormatrix3x4_functions.hpp
  ${ROAR_SOURCE_DIR}/math/rormatrix3x4_functions.hh
  ${ROAR_SOURCE_DIR}/math/rormatrix.hpp
  ${ROAR_SOURCE_DIR}/math/rorvector.hpp
  ${ROAR_SOURCE_DIR}/math/rorquaternion.hpp
  ${ROAR_SOURCE_DIR}/math/rorquaternion.hh
  ${ROAR_SOURCE_DIR}/math/roraxis_angle.hpp
  ${ROAR_SOURCE_DIR}/math/roraxis_angle.hh
  ${ROAR_SOURCE_DIR}/math/roreuler_angle.hpp
  ${ROAR_SOURCE_DIR}/math/roreuler_angle.hh
  ${ROAR_SOURCE_DIR}/math/rorplane.hpp
  ${ROAR_SOURCE_DIR}/math/rorplane.hh
  ${ROAR_SOURCE_DIR}/configuration/rorconfiguration.hpp
  ${ROAR_SOURCE_DIR}/configuration/rorconfiguration.hh
  ${ROAR_SOURCE_DIR}/geometry/rorgeometry_utilities.hpp
  ${ROAR_SOURCE_DIR}/geometry/rorgeometry_utilities.hh
  ${ROAR_SOURCE_DIR}/watchcat/rorwatchcat.hpp
  ${ROAR_SOURCE_DIR}/rhi/rorrhi_types.hpp
  ${ROAR_SOURCE_DIR}/rhi/rorbuffers_format.hpp
  ${ROAR_SOURCE_DIR}/rhi/rorbuffer.hpp
  ${ROAR_SOURCE_DIR}/resources/rorresource.hpp)

set(ROAR_SOURCES
  ${ROAR_SOURCE_DIR}/profiling/rorlog.cpp
  ${ROAR_SOURCE_DIR}/profiling/rortimer.cpp
  ${ROAR_SOURCE_DIR}/foundation/rorrandom.cpp
  ${ROAR_SOURCE_DIR}/foundation/rorjobsystem.cpp
  ${ROAR_SOURCE_DIR}/watchcat/rorwatchcat.cpp
  ${ROAR_SOURCE_DIR}/resources/rorresource.cpp
  ${ROAR_SOURCE_DIR}/rhi/rorrhi_types.cpp
  ${ROAR_SOURCE_DIR}/rhi/rorbuffers_format.cpp
  ${ROAR_SOURCE_DIR}/rhi/rorbuffer.cpp
  ${ROAR_SOURCE_DIR}/resources/rorprojectroot.cpp)

add_library(${ROAR_NAME} SHARED ${ROAR_SOURCES} ${ROAR_HEADERS})
set_target_properties(${ROAR_NAME} PROPERTIES VERSION 1.0.0.0 SOVERSION 1)          # TODO: Fix the hardcoded version
set_target_properties(${ROAR_NAME} PROPERTIES CMAKE_VISIBILITY_INLINES_HIDDEN true) # Don't export all of those C++ template generated symbols
set_target_properties(${ROAR_NAME} PROPERTIES CMAKE_CXX_VISIBILITY_PRESET hidden)   # Set default to hide symbols visibility for Roar
set_target_properties(${ROAR_NAME} PROPERTIES CMAKE_C_VISIBILITY_PRESET hidden)     # Set default to hide symbols visibility for Roar

set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)    # Set default to hide symbols visibility for all targets
set(CMAKE_C_VISIBILITY_PRESET hidden)      # Set default to hide symbols visibility for all targets

message(STATUS "Now generating roar_export_import.hpp file")
include(GenerateExportHeader)
generate_export_header(${ROAR_NAME}
  EXPORT_FILE_NAME roar_export_import.hpp
  EXPORT_MACRO_NAME ROAR_ENGINE_ITEM
  NO_EXPORT_MACRO_NAME ROAR_ENGINE_PRIVATE)

# If using roar as library, make sure you define ROAR_AS_SYSTEM if you don't want warnings from roar polluting your build logs
if (ROAR_AS_LIBRARY)
  # Using target_include_directories_system so that clients don't have issues with warnings in roar
  target_include_directories_system(${ROAR_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
  target_include_directories_system(${ROAR_NAME} PUBLIC ${ROAR_SOURCE_DIR})
  target_include_directories_system(${ROAR_NAME} PUBLIC ${ROAR_ROOT_DIR})
else()
  target_include_directories(${ROAR_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
  target_include_directories(${ROAR_NAME} PUBLIC ${ROAR_SOURCE_DIR})
  target_include_directories(${ROAR_NAME} PUBLIC ${ROAR_ROOT_DIR})
endif()

set(ROAR_REQUIRED_PLATFORM_LIBRARIES "")

if (APPLE)
  find_library(CORE_SERVICES CoreServices)
  set(ROAR_REQUIRED_PLATFORM_LIBRARIES ${CORE_SERVICES})
elseif (LINUX)
elseif (WIN32)
else ()
endif()

set(ROAR_REQUIRED_PRIVATE_LIBRARIES
  roar_version)

set(ROAR_REQUIRED_PUBLIC_LIBRARIES
  cimage
  stb
  spdlog
  nlohmann_json
  xxhash)

target_link_libraries(${ROAR_NAME} PRIVATE ${ROAR_REQUIRED_PLATFORM_LIBRARIES})
target_link_libraries(${ROAR_NAME} PRIVATE ${ROAR_REQUIRED_PRIVATE_LIBRARIES})

# To make sure project warning levels are not transferred to dependencies, one can link to these as SYSTEM libraries
# This still seems like a hack, target_link_libraries_system is defined in build.cmake
target_link_libraries_system(${ROAR_NAME} PUBLIC ${ROAR_REQUIRED_PUBLIC_LIBRARIES})

build_options(${ROAR_NAME})                               # Set common build options
