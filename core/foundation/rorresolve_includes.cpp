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

#include "foundation/rorresolve_includes.hpp"

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
bool resolve_includes(std::string &a_source, uint32_t &a_level, ror::ResourceSemantic a_semantic, bool a_with_markers)
{
	auto levels = ror::settings().m_resolve_includes_depth;

	if (a_level == levels)
	{
		ror::log_critical("Too many levels deep into resolving includes, is there a recursion?");
		return false;
	}

	a_level++;
	bool result{false};

	std::string out{a_source};        // Lets make a copy that we will work on, required otherwise includes_begin gets invalidated

	static const std::regex include_regex("#include *(<|\")[\\S]*\\.[\\S]*(>|\")[\\s]*\n", std::regex::ECMAScript | std::regex::icase);
	static const std::regex filename_regex("(<|\")[\\S]*(>|\")", std::regex::ECMAScript | std::regex::icase);

	auto includes_begin = std::sregex_iterator(a_source.begin(), a_source.end(), include_regex);
	auto includes_end   = std::sregex_iterator();

	for (auto i = includes_begin; i != includes_end; ++i)
	{
		result = true;

		std::smatch filename_matches;
		std::smatch match     = *i;
		std::string match_str = match.str();
		if (std::regex_search(match_str, filename_matches, filename_regex))
		{
			// The first filename_match is the whole string; next is the first parenthesized expression.
			assert(filename_matches.size() == 3 && "Include file name is not in expected format");
			std::string file_name{filename_matches[0].str()};
			file_name = file_name.substr(1, file_name.length() - 2);

			auto       &shader_resource = ror::load_resource(file_name, a_semantic);
			std::string shader_code{shader_resource.data_copy()};

			if (a_with_markers)
				shader_code = "// start " + file_name + "\n" + shader_code + "// end " + file_name + "\n\n";
			else
				shader_code += "\n";

			out = std::regex_replace(out, include_regex, shader_code, std::regex_constants::match_flag_type::format_first_only);        // this means the found item surrounded with [] "[$&]"
		}
		else
		{
			assert(0 && "Invalid include in the input file");        // Shouldn't reach here
		}
	}

	a_source = out;

	return result;
}

/**
 * @brief      Resolves includes in a source string
 * @details    Resolves includes in input source string. It works out internally how many levels iterations are required.
 *             This can be used if we already have a string, for example created from a Resource
 * @param      a_input           The source input
 * @param      a_semantic        The semantic of the input data type for searching around in the project folders
 * @param      a_with_markers    A flag for debugging purposes if we want to see which file was included where
 * @return     return void
 */
void resolve_includes(std::string &a_input, ror::ResourceSemantic a_semantic, bool a_with_markers)
{
	uint32_t level  = 0;
	bool     result = false;
	do
	{
		result = resolve_includes(a_input, level, a_semantic, a_with_markers);
	} while (result && level < 10);
}

// NOTE: We can't do the following because a Resource is a shared resource and can't be reset while other threads might be using it
// /**
//  * @brief      Resolves includes in a Resource
//  * @details    Resolves includes in input Resource, creating a string from it and updating the resource as well.
//  *             It works out internally how many levels iterations are required by calling resolve_includes(string)
//  * @param      a_input           The source input Resource
//  * @param      a_semantic        The semantic of the input data type for searching around in the project folders
//  * @param      a_with_markers    A flag for debugging purposes if we want to see which file was included where
//  * @return     return void
//  */
// FORCE_INLINE void resolve_includes(ror::Resource &a_resource, ror::ResourceSemantic a_semantic = ror::ResourceSemantic::shaders, bool a_with_markers = false)
// {
// 	std::string shader_code{reinterpret_cast<const char *>(a_resource.data().data()), a_resource.data().size()};
// 	resolve_includes(shader_code, a_semantic, a_with_markers);

// 	a_resource.update({shader_code.begin(), shader_code.end()});
// }
}        // namespace ror
