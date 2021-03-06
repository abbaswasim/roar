// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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

#include "foundation/rorhash.hpp"
#include "foundation/rorrandom.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorprojectroot.hpp"
#include "rorresource.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <memory>
#include <vector>

namespace ror
{
std::string get_resource_semantic_string(ResourceSemantic a_semantic)
{
	switch (a_semantic)
	{
		case ResourceSemantic::materials:
			return "materials";
		case ResourceSemantic::textures:
			return "textures";
		case ResourceSemantic::shaders:
			return "shaders";
		case ResourceSemantic::scripts:
			return "scripts";
		case ResourceSemantic::objects:
			return "objects";
		case ResourceSemantic::configs:
			return "configs";
		case ResourceSemantic::models:
			return "models";
		case ResourceSemantic::caches:
			return "caches";
		case ResourceSemantic::audio:
			return "audio";
		case ResourceSemantic::cubemap:
			return "cubemap";
		case ResourceSemantic::animation:
			return "animation";
		case ResourceSemantic::font:
			return "font";
		case ResourceSemantic::logs:
			return "logs";
		case ResourceSemantic::misc:
			return "misc";
	}

	return "";
}

Resource::~Resource() noexcept
{
	if (this->m_mapped)
	{
		// TODO: Unmap
	}
}

Resource::Resource(std::filesystem::path a_absolute_path, bool a_binary, bool a_read_only, bool a_mapped) :
	m_absolute_path(a_absolute_path), m_binary_file(a_binary), m_read_only(a_read_only), m_mapped(a_mapped)
{
	if (!this->m_absolute_path.is_absolute())
	{
		log_error("{} path is not absolute, either use absolute filename or use the relative constructor", this->m_absolute_path.c_str());
	}

	this->load();
}

Resource::Resource(std::filesystem::path a_relative_path, ResourceSemantic a_resource_semantic, bool a_binary, bool a_read_only, bool a_mapped) :
	m_absolute_path(a_relative_path), m_semantic(a_resource_semantic), m_binary_file(a_binary), m_read_only(a_read_only), m_mapped(a_mapped)
{
	this->m_absolute_path = this->find_resource();
	this->load();
}

/**
 * Tries hard to find resource in the paths we know.
 * For example if "astro_boy/boy10.jpg" is provided as assets path, it will search in the following places in that order
 project_root/astro_boy/boy10.jpg
 project_root/textures/astro_boy/boy10.jpg
 project_root/assets/astro_boy/boy10.jpg
 project_root/assets/textures/astro_boy/boy10.jpg
 project_root/astro_boy/assets/boy10.jpg
 project_root/astro_boy/textures/boy10.jpg
 project_root/textures/astro_boy/materials/boy10.jpg
 project_root/textures/astro_boy/textures/boy10.jpg
 project_root/textures/astro_boy/shaders/boy10.jpg
 project_root/textures/astro_boy/scripts/boy10.jpg
 project_root/textures/astro_boy/objects/boy10.jpg
 project_root/textures/astro_boy/configs/boy10.jpg
 project_root/textures/astro_boy/models/boy10.jpg
 project_root/textures/astro_boy/misc/boy10.jpg
*/

std::filesystem::path Resource::find_resource()
{
	// If absolute path or has no filename, just return as it is
	if (this->m_absolute_path.is_absolute() || !this->m_absolute_path.has_filename())
	{
		return this->m_absolute_path;
	}

	// Here we just create a path so if two threads do the same thing thats fine at least there is no contention
	// std::lock_guard<std::mutex> mtx(this->m_mutex);        // You don't want any other thread to claim this resource

	std::filesystem::path semantic_path{get_resource_semantic_string(this->m_semantic)};
	std::filesystem::path resource_semantic_path{semantic_path / this->m_absolute_path};

	auto project_root_path = get_project_root().path();        // Here calling get_project_root without any arguments relies one clients who must have called and initalized project_root

	// Is it at the root of the project?
	std::filesystem::path p{project_root_path / this->m_absolute_path};

	// If the file exists don't muck around just return
	if (std::filesystem::exists(p))
	{
		return p;
	}
	else        // Try to find it on the filesystem inside the project root
	{
		std::filesystem::path q{project_root_path / resource_semantic_path};

		if (std::filesystem::exists(q))
		{
			return q;
		}
		else
		{
			// Some times projects have "assets" folder, lets search that too
			std::filesystem::path r{project_root_path / "assets" / this->m_absolute_path};

			if (std::filesystem::exists(r))
			{
				return r;
			}
			else
			{
				std::filesystem::path s{project_root_path / "assets" / resource_semantic_path};

				if (std::filesystem::exists(s))
				{
					return s;
				}
				else
				{
					// Split the path for parent and file name, its ok if these don't exist and return ""
					auto parent_path = this->m_absolute_path.parent_path();
					auto file_name   = this->m_absolute_path.filename();

					std::filesystem::path t{project_root_path / parent_path / "assets" / file_name};

					if (std::filesystem::exists(t))
					{
						return t;
					}
					else
					{
						std::filesystem::path u{project_root_path / parent_path / semantic_path / file_name};

						if (std::filesystem::exists(u))
						{
							return u;
						}
						else
						{
							// Now we are desperate, trying hard to find this resource
							for (auto item : {ResourceSemantic::materials,
											  ResourceSemantic::textures,
											  ResourceSemantic::shaders,
											  ResourceSemantic::scripts,
											  ResourceSemantic::objects,
											  ResourceSemantic::configs,
											  ResourceSemantic::models,
											  ResourceSemantic::misc})        // Different way of dealing with caches and logs
							{
								auto items_path = get_resource_semantic_string(item);

								std::filesystem::path v{project_root_path /
														semantic_path / parent_path / items_path / file_name};

								if (std::filesystem::exists(v))
								{
									return v;
								}
							}
						}
					}
				}
			}
		}
	}

	// TODO: Try a recursive search if still hasnt' found the resource

	// If no valid path provided and we have exhausted all posibilites just create a file with current time
	std::time_t t = std::time(nullptr);
	char        gen_filename[30];

	if (std::strftime(gen_filename, sizeof(gen_filename), "%d_%m_%Y_%H_%M_%S", std::localtime(&t)))
	{
		auto temp = project_root_path / resource_semantic_path;
		log_error("File name generation failed using {} as a file name", temp.c_str());

		return temp;
	}

	// If std::strftime failes return what was expected without anything more we can do
	return project_root_path / semantic_path / gen_filename;
}

void Resource::load()
{
	{
		std::lock_guard<std::mutex> mtx(this->m_mutex);
		if (!std::filesystem::exists(this->m_absolute_path))
		{
			std::ios_base::openmode mode = std::ios::ate | std::ios::out;

			if (this->m_binary_file)
				mode |= std::ios::binary;

			ror::log_error("{} file does not exit, creating now, but contents will be empty", this->m_absolute_path.c_str());

			// Create the file, and the directories, if we tried our best but couldn't find it
			std::filesystem::create_directory(this->m_absolute_path.parent_path());
			std::ofstream of(this->m_absolute_path, mode);                            // Maybe this is not such a good idea, for textures/shaders you will have empty files created
			of << get_resource_semantic_string(this->m_semantic) << std::endl;        // Just insert the semantic type into the file so readers know what failed
		}
	}

	// Create cache or load cached file name
	this->load_or_generate_uuid();
	this->cache();
	this->load_or_mmap();

	ror::log_info("Loaded cached resource {} for {} file", this->m_cached_path.c_str(), this->m_absolute_path.c_str());

	std::string path_string{this->m_absolute_path};
	// Create Path hash from absolute path as compared to cached path
	this->m_path_hash = ror::hash_64(path_string.c_str(), path_string.size());

	// Create Data hash from cached data as compared to actual file data
	if (!this->m_data->empty())
		this->m_data_hash = ror::hash_64(this->m_data->data(), this->m_data->size());
	else
		ror::log_error("Loaded cached resource {} for {} file has nothing in it.", this->m_cached_path.c_str(), this->m_absolute_path.c_str());
}

void Resource::load_or_mmap()
{
	// If we are asked to create mmaped file or the resource is readonly, lets mmap it
	if (this->m_mapped)        // || this->m_read_only)
	{
		// TODO: mmap me and consider m_read_only
	}
	else
	{
		std::ios_base::openmode mode = std::ios::ate | std::ios::in;

		if (this->m_binary_file)
			mode |= std::ios::binary;

		// std::basic_ifstream<uint8_t> as_file(std::string(this->m_cached_path), mode);
		std::ifstream as_file(this->m_cached_path, mode);
		if (!as_file.is_open())
		{
			log_critical("Can't open file {}", this->m_cached_path.c_str());
			return;
		}

		// No point to synchronise here because some other process might be writing into the file
		std::streampos bytes_count = as_file.tellg();
		as_file.seekg(0, std::ios::beg);

		this->m_data = std::make_shared<std::vector<std::uint8_t>>();
		// Cast is ok because if byte_count is bigger than size_t range, we have a bigger problem
		this->m_data->resize(static_cast<size_t>(bytes_count));
		as_file.read(reinterpret_cast<char *>(this->m_data->data()), bytes_count);        // Weird that int8_t is 'signed char' and can't be converted to 'char'

		as_file.close();
	}
}

void Resource::cache()
{
	// TODO: Implement proper cacheing
	this->m_cached_path = this->m_absolute_path;
}

void Resource::load_or_generate_uuid()
{
	// TODO: Implement proper UUID generation
	// Read UUID config file or generate it
	// For now using a hash
	std::string path = this->m_absolute_path.c_str();
	this->m_uuid     = ror::hash_128(path.c_str(), path.size());
}

const std::shared_ptr<std::vector<uint8_t>> Resource::get_data() const
{
	return this->m_data;
}

void Resource::temp()
{}
}        // namespace ror
