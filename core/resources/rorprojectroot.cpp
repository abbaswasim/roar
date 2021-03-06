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

#include "profiling/rorlog.hpp"
#include "rorprojectroot.hpp"

namespace ror
{
// Convenience name for default initialized projects
const std::string project_path("roar_project");

ProjectRoot::ProjectRoot() :
	m_project_root(std::filesystem::absolute(project_path)),
	m_project_root_hash(std::hash<std::string>{}(project_path))
{
	std::filesystem::create_directory(this->m_project_root);        // its ok if already exists
}

ProjectRoot::ProjectRoot(std::filesystem::path a_project_path) :
	m_project_root(std::filesystem::absolute(a_project_path)),
	m_project_root_hash(std::hash<std::string>{}(a_project_path))
{
	if (a_project_path.empty())
		ror::log_error("ProjectRoot is initilaized with empty path.");

	std::filesystem::create_directory(this->m_project_root);        // its ok if already exists
}

void ProjectRoot::change_project(std::string a_root_path)
{
	this->m_project_root = std::filesystem::absolute(a_root_path);
	std::filesystem::create_directory(this->m_project_root);        // its ok if already exists

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
	return pr;
}

const ProjectRoot &get_default_project_root()
{
	static const ProjectRoot pr{};
	return pr;
}

}        // namespace ror
