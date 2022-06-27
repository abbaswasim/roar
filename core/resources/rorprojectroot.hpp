// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2019
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

#pragma once

#include "foundation/rortypes.hpp"
#include "roar.hpp"
#include <filesystem>
#include <string>

namespace ror
{
class ROAR_ENGINE_ITEM ProjectRoot final
{
  public:
	FORCE_INLINE ProjectRoot(const ProjectRoot &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE ProjectRoot(ProjectRoot &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE ProjectRoot &operator=(const ProjectRoot &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE ProjectRoot &operator=(ProjectRoot &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~ProjectRoot() noexcept                                = default;        //! Destructor

	/**
	 * Creates a project with the hardcoded project name "roar_project"
	 * in some directory depending on the OS provided working dir
	 */
	ProjectRoot();

	/**
	 * Creates a project with the provided root dir
	 */
	ProjectRoot(std::filesystem::path a_project_path);

	void                         change_project(std::string a_root_path);
	const std::filesystem::path &path() const noexcept;
	uint64_t                     hash() const noexcept;

  protected:
  private:
	std::filesystem::path m_project_root{};                    //! Root folder copy
	hash_64_t             m_project_root_hash{};               //! Root folder hash used for seeding ror_hash
};

/**
* One shot project root creation utility, can be used later to query global root project multiple times
* Calling get_project_root with a different path will have no effect
* If not called with a valid path the first time, ProjectRoot constructor will report error,
* any preceding calls can be with empty path
*/
const ProjectRoot &get_project_root(std::filesystem::path a_path = "");

/**
* Default project root creation utility
*/
const ProjectRoot &get_default_project_root();
}        // namespace ror
