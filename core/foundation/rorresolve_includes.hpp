// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2023
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the 'Software'),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Version: 1.0.0

#include "foundation/rormacros.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorresource.hpp"
#include "settings/rorsettings.hpp"
#include <cassert>
#include <regex>
#include <string>
#include <vector>

namespace ror
{

/**
 * @brief      Resolves includes in a source string at one level
 * @details    Resolves includes in input source string only at one level at a time. It takes levels as argument.
 *             This shouldn't be used directly but via other methods to resurse for inner includes.
 * @param      a_input           The source input
 * @param      a_level           Which level are we currently iterating on.
 * @param      a_semantic        The semantic of the input data type for searching around in the project folders
 * @param      a_with_markers    A flag for debugging purposes if we want to see which file was included where
 * @return     return            true if we managed to replace the a_source with data from its includes, false other
 */
// bool resolve_includes(std::string &a_source, uint32_t &a_level, ror::ResourceSemantic a_semantic = ror::ResourceSemantic::shaders, bool a_with_markers = false);
bool resolve_includes(std::string &a_source, uint32_t &a_level, ror::ResourceSemantic a_semantic, bool a_with_markers);

/**
 * @brief      Resolves includes in a source string
 * @details    Resolves includes in input source string. It works out internally how many levels iterations are required.
 *             This can be used if we already have a string, for example created from a Resource
 * @param      a_input           The source input
 * @param      a_semantic        The semantic of the input data type for searching around in the project folders
 * @param      a_with_markers    A flag for debugging purposes if we want to see which file was included where
 * @return     return void
 */
// void resolve_includes(std::string &a_input, ror::ResourceSemantic a_semantic = ror::ResourceSemantic::shaders, bool a_with_markers = false);
void resolve_includes(std::string &a_input, ror::ResourceSemantic a_semantic, bool a_with_markers);

}        // namespace ror
