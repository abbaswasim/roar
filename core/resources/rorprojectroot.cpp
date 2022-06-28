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

#include "configuration/rorsettings_configuration.hpp"
#include "profiling/rorlog.hpp"
#include "rorprojectroot.hpp"
#include <filesystem>
#include <string>

namespace ror
{

ProjectRoot::ProjectRoot(std::filesystem::path a_project_path) :
	m_project_root(std::filesystem::absolute(a_project_path)),
	m_project_root_hash(std::hash<std::string>{}(m_project_root))
{
	if (a_project_path.empty())
		ror::log_error("ProjectRoot is initilaized with empty path.");

	ror::log_info("Creating roar project root {}", this->m_project_root.c_str());
	std::filesystem::create_directory(this->m_project_root);        // its ok if already exists
}

void ProjectRoot::change_project(std::string a_root_path)
{
	this->m_project_root = std::filesystem::absolute(a_root_path);        // This will make it absolute according to current_path, which is where you run the exe from
	std::filesystem::create_directory(this->m_project_root);              // its ok if already exists

	// Update the hash
	this->m_project_root_hash = std::hash<std::string>{}(this->m_project_root);
}

const std::filesystem::path &ProjectRoot::path() const noexcept
{
	return this->m_project_root;
}

uint64_t ProjectRoot::hash() const noexcept
{
	return m_project_root_hash;
}

// One shot project root creation utility, can be used later to query global root project multiple times
const ProjectRoot &get_project_root(std::filesystem::path a_path)
{
	static ProjectRoot pr{a_path};
	assert(a_path != pr.path() && "Trying to re-initialize project root");
	assert(pr.path() != "" && "Project root can't be empty");
	return pr;
}

}        // namespace ror
