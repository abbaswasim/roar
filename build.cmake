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

include_guard()

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE RelWithDebInfo) # good compromise
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD 20)  # Ask for Cxx20 but not required
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS false)

function(build_options target_name extra_pedantic)
  if (CMAKE_BUILD_TYPE MATCHES "Debug")
	target_compile_definitions(${target_name} PRIVATE RORDEBUG)
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
	set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
  else()
	set(CMAKE_CXX_FLAGS_DEBUG "-O3")
	set(CMAKE_C_FLAGS_DEBUG "-O3")
  endif (CMAKE_BUILD_TYPE MATCHES "Debug")

  # Be slightly more pedantic
  if (extra_pedantic)
	target_compile_options(${target_name} PRIVATE
	  -Wall
	  -Wextra
	  -Werror
	  -Wconversion
	  -Wsign-conversion
	  -Wunreachable-code
	  -Wuninitialized
	  -pedantic-errors
	  -Wold-style-cast
	  -Wshadow
	  -Weffc++
	  # -Wno-covered-switch-default
	  # -Wno-switch-enum
	  # -Wno-duplicate-enum
	  # -Wno-mismatched-tags
	  # -Wno-documentation-unknown-command
	  -Wfloat-equal)
  endif()

  if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
	if (extra_pedantic)
	  target_compile_options(${target_name} PRIVATE
		-Wmost
		-Wno-c++98-compat-pedantic
		-Wno-c++98-compat
		-Wno-switch-default # This is fine because I am enforcing having all options handled in the each switch
		# -fno-omit-frame-pointer
		# -fno-optimize-sibling-calls
		-Wno-poison-system-directories
		-Wno-gnu-zero-variadic-macro-arguments) # TODO: Find a solution to this for gtest

	  target_compile_options(${target_name} PRIVATE
		# The following warnings after everyting are enabled by -Weverything but are not practical to fix hence ignoring
		-Weverything
		-Wno-exit-time-destructors        # worth enabling if getting crashes at exit time
		-Wno-global-constructors          # worth enabling if getting crashes at exit time
		-Wno-c++2a-compat
		-Wno-missing-prototypes
		-Wno-padded                       # TODO: This one is interesting, enable to fix all issues
		-Wno-double-promotion             # re-enable this later
		-Wno-documentation                # I don't understand why this is passed through to thirdparty deps still, investigate later
	  )
	endif()
	# target_compile_options(${target_name} PRIVATE -Wno-gnu-zero-variadic-macro-arguments)

	if (ROAR_BUILD_SANITIZED)
	  target_compile_options(${target_name} PRIVATE
		# -fsanitize=memory
		# -fsanitize=thread
		# -fsanitize=undefined
		# -fno-sanitize-recover=all
		# -fsanitize=float-divide-by-zero
		# -fsanitize=float-cast-overflow
		# -fno-sanitize=null
		# -fno-sanitize=alignment
		-fsanitize=address)

	  target_link_options(${target_name} PRIVATE
		# -fsanitize=undefined
		# -fsanitize=memory
		# -fsanitize=thread
		-fsanitize=address)
	endif()
  endif()

  if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
      target_compile_options(${target_name} PRIVATE -Wno-invalid-constexpr -Wno-c++20-extensions -Wno-c++20-compat -Wno-return-type -Wno-attributes)
  endif()

endfunction(build_options)

# Inspired by https://stackoverflow.com/questions/52135983/cmake-target-link-libraries-include-as-system-to-suppress-compiler-warnings
# Important: This only works if there is a target created that uses this link command, but make sure all includes are via the target
# Standalone includes if not using the target link will still produce warnings/errors
# For example "CTPL/ctpl_stl.h" is a target include but "thirdparty/CTPL/ctpl_stl.h" is not and will produce warnings
function(target_link_libraries_system target_name scope)
  set(dependencies ${ARGN})
  foreach(dependency ${dependencies})
	get_target_property(dependency_include_dirs ${dependency} INTERFACE_INCLUDE_DIRECTORIES)
	if(dependency_include_dirs)
	  target_include_directories(${target_name} SYSTEM ${scope} ${dependency_include_dirs})
	else()
	  message("Warning: ${dependency} doesn't set INTERFACE_INCLUDE_DIRECTORIES. No INCLUDE_DIRECTORIES set.")
	endif()
	target_link_libraries(${target_name} ${scope} ${dependency})
  endforeach(dependency)
endfunction(target_link_libraries_system)

# Use this to hide your library header warnings from clients
# Inspired by https://foonathan.net/2018/10/cmake-warnings/
function(target_include_directories_system target_name)
  set(include_dirs ${ARGN})
  foreach(include_dir ${include_dirs})
	target_include_directories(${target_name} PRIVATE ${include_dir})
	target_include_directories(${target_name} SYSTEM INTERFACE ${include_dir})
  endforeach(include_dir)
endfunction(target_include_directories_system)
