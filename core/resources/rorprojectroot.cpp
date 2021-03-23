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

#include "rorprojectroot.hpp"

namespace ror
{
// TODO: To be changed later on with proper abstraction
const std::string project_path("roar_project/");

hash_64_t ProjectRoot::m_project_root_hash = std::hash<std::string>{}(project_path);

ProjectRoot::ProjectRoot() :
	m_project_root(std::filesystem::absolute(project_path))
{
	std::filesystem::create_directory(this->m_project_root);
}

void ProjectRoot::change_project_root(std::string a_root_path)
{
	this->m_project_root = std::filesystem::absolute(a_root_path);
	std::filesystem::create_directory(this->m_project_root);

	// Update the hash
	this->m_project_root_hash = static_cast<size_t>(std::hash<std::string>{}(this->m_project_root));
}

const std::filesystem::path &ProjectRoot::get_project_root() const
{
	return this->m_project_root;
}

uint64_t ProjectRoot::get_project_root_hash()
{
	return m_project_root_hash;
}
}        // namespace ror
