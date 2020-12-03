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

function(print_target_properties tgt)
	if(NOT TARGET ${tgt})
		message("There is no target named '${tgt}'")
		return()
	endif()

	# this list of properties can be extended as needed
	set(CMAKE_PROPERTY_LIST SOURCE_DIR BINARY_DIR COMPILE_DEFINITIONS CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG SYSTEM_INCLUDE_DIRECTORIES
			 COMPILE_OPTIONS INCLUDE_DIRECTORIES INTERFACE_INCLUDE_DIRECTORIES LINK_LIBRARIES DEFINE_SYMBOL)

	message("Configuration for target ${tgt}")

	foreach (prop ${CMAKE_PROPERTY_LIST})
		get_property(propval TARGET ${tgt} PROPERTY ${prop} SET)
		if (propval)
			get_target_property(propval ${tgt} ${prop})
			message (STATUS "${prop} = ${propval}")
		endif()
	endforeach(prop)

endfunction(print_target_properties)

# MESSAGE( STATUS "CMAKE_CXX_FLAGS that you need are: " ${CMAKE_CXX_FLAGS} )

print_target_properties(${ROAR_NAME})

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
	message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

message("Here starts all the properties")

# Get all propreties that cmake supports
execute_process(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)

# Convert command output into a CMake list
STRING(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
STRING(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")

function(print_properties)
	message ("CMAKE_PROPERTY_LIST = ${CMAKE_PROPERTY_LIST}")
endfunction(print_properties)

function(print_target_propertie tgt)
	if(NOT TARGET ${tgt})
	  message("There is no target named '${tgt}'")
	  return()
	endif()

	foreach (prop ${CMAKE_PROPERTY_LIST})
		string(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" prop ${prop})
	# Fix https://stackoverflow.com/questions/32197663/how-can-i-remove-the-the-location-property-may-not-be-read-from-target-error-i
	if(prop STREQUAL "LOCATION" OR prop MATCHES "^LOCATION_" OR prop MATCHES "_LOCATION$")
		continue()
	endif()
		# message ("Checking ${prop}")
		get_property(propval TARGET ${tgt} PROPERTY ${prop} SET)
		if (propval)
			get_target_property(propval ${tgt} ${prop})
			message ("${tgt} ${prop} = ${propval}")
		endif()
	endforeach(prop)
endfunction(print_target_propertie)

print_target_propertie(${ROAR_NAME})
