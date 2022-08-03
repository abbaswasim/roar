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

#include "configuration/rorsettings_configuration.hpp"
#include "foundation/rorhash.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rorrandom.hpp"
#include "foundation/rorutilities.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorprojectroot.hpp"
#include "rhi/rortypes.hpp"
#include "rorresource.hpp"
#include "settings/rorsettings.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <memory>
#include <string>
#include <utility>
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
		case ResourceSemantic::scenes:
			return "scenes";
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

ResourceExtension get_resource_extension(const std::filesystem::path &a_path)
{
	auto extension = a_path.extension();

	// clang-format off
	if (extension == ".jpeg")  return ResourceExtension::texture_jpeg;
	if (extension == ".png")   return ResourceExtension::texture_png;
	if (extension == ".bmp")   return ResourceExtension::texture_bmp;
	if (extension == ".psd")   return ResourceExtension::texture_psd;
	if (extension == ".tga")   return ResourceExtension::texture_tga;
	if (extension == ".gif")   return ResourceExtension::texture_gif;
	if (extension == ".hdr")   return ResourceExtension::texture_hdr;
	if (extension == ".pic")   return ResourceExtension::texture_pic;
	if (extension == ".pnm")   return ResourceExtension::texture_pnm;
	if (extension == ".astc")  return ResourceExtension::texture_astc;
	if (extension == ".ktx")   return ResourceExtension::texture_ktx;
	if (extension == ".ktx2")  return ResourceExtension::texture_ktx2;
	if (extension == ".basis") return ResourceExtension::texture_basis;
	if (extension == ".vert")  return ResourceExtension::shaders_vertex;
	if (extension == ".frag")  return ResourceExtension::shaders_fragment;
	if (extension == ".comp")  return ResourceExtension::shaders_compute;
	if (extension == ".mesh")  return ResourceExtension::shaders_mesh;
	if (extension == ".py")    return ResourceExtension::scripts_python;
	if (extension == ".c")     return ResourceExtension::scripts_c;
	if (extension == ".cpp")   return ResourceExtension::scripts_cpp;
	if (extension == ".lua")   return ResourceExtension::scripts_lua;
	if (extension == ".json")  return ResourceExtension::configs_json;
	if (extension == ".ini")   return ResourceExtension::configs_ini;
	if (extension == ".gltf")  return ResourceExtension::models_gltf;
	if (extension == ".obj")   return ResourceExtension::models_obj;
	if (extension == ".fbx")   return ResourceExtension::models_fbx;
	if (extension == ".ogg")   return ResourceExtension::audio_ogg;
	if (extension == ".ttf")   return ResourceExtension::font_ttf;
	if (extension == ".otf")   return ResourceExtension::font_otf;
	if (extension == ".txt")   return ResourceExtension::logs_txt;
	// clang-format on

	return ResourceExtension::unknown;
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
 project_root/astro_boy/assets/textures/boy10.jpg
 project_root/textures/astro_boy/materials/boy10.jpg
 project_root/textures/astro_boy/textures/boy10.jpg
 project_root/textures/astro_boy/shaders/boy10.jpg
 project_root/textures/astro_boy/scripts/boy10.jpg
 project_root/textures/astro_boy/objects/boy10.jpg
 project_root/textures/astro_boy/configs/boy10.jpg
 project_root/textures/astro_boy/models/boy10.jpg
 project_root/textures/astro_boy/misc/boy10.jpg
*/

std::filesystem::path find_resource(const std::filesystem::path &a_path, ResourceSemantic a_semantic)
{
	// If absolute path or has no filename, just return as it is
	if (a_path.is_absolute() || !a_path.has_filename())
	{
		return a_path;
	}

	// Here we just create a path so if two threads do the same thing thats fine at least there is no contention

	std::filesystem::path semantic_path{get_resource_semantic_string(a_semantic)};
	std::filesystem::path resource_semantic_path{semantic_path / a_path};

	auto &project_root_path = get_project_root().path();        // Here calling get_project_root without any arguments relies on clients who must have called and initalized project_root

	// Is it at the root of the project?
	std::filesystem::path p{project_root_path / a_path};

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
			std::filesystem::path r{project_root_path / "assets" / a_path};

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
					auto parent_path = a_path.parent_path();
					auto file_name   = a_path.filename();

					std::filesystem::path t{project_root_path / parent_path / "assets" / file_name};

					if (std::filesystem::exists(t))
					{
						return t;
					}
					else
					{
						std::filesystem::path u{project_root_path / parent_path / "assets" / resource_semantic_path};

						if (std::filesystem::exists(u))
						{
							return u;
						}
						else
						{
							std::filesystem::path v{project_root_path / parent_path / semantic_path / file_name};

							if (std::filesystem::exists(v))
							{
								return v;
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
								                  ResourceSemantic::scenes,
								                  ResourceSemantic::misc})        // Different way of dealing with caches and logs
								{
									auto items_path = get_resource_semantic_string(item);

									std::filesystem::path w{project_root_path /
									                        semantic_path / parent_path / items_path / file_name};

									if (std::filesystem::exists(w))
									{
										return w;
									}
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

	if (!std::strftime(gen_filename, sizeof(gen_filename), "%d_%m_%Y_%H_%M_%S", std::localtime(&t)))
	{
		auto temp = project_root_path / resource_semantic_path;
		log_error("File name generation failed using {} as a file name", temp.c_str());

		return temp;
	}

	std::string file_with_timestamp{resource_semantic_path.replace_filename(std::string(gen_filename) + "_" + resource_semantic_path.filename().string())};

	// If std::strftime returned successfully use the generated filename
	return project_root_path / file_with_timestamp;
}

static Resource &cache_resource(const std::filesystem::path &a_absolute_path)
{
	using ResourceCache = Cache<std::filesystem::path, std::shared_ptr<Resource>, PathHash>;        // Thread Safe resource cache
	static ResourceCache resource_cache{};

	assert(a_absolute_path != "" && "Path can't be empty");

	// NOTE: resource_cache is thread safe but from find() till insert() another thread might interfere, we can't make this whole opp atomic so multiple insertion attempts might happen
	// Which is fine, we ignore our generated shared_ptr in that case
	auto found = resource_cache.find(a_absolute_path);
	if (found.second)
	{
		log_trace("Resource cache hit for {}", a_absolute_path.c_str());
		return *found.first;
	}

	auto pointer = std::make_shared<Resource>(a_absolute_path);
	assert(pointer);

	auto result = resource_cache.insert(a_absolute_path, pointer);

	if (result.second)        // Means newly inserted
		return *pointer;
	else        // Means we thought we _can_ insert it at "find()" stage but at "insert()" stage it was inserted by someone else, so lets use the already inserted one
		return *result.first->second;
}

/**
 * Can be used to load resources relative to project_root anywhere in different folers
 */
Resource &load_resource(const std::filesystem::path &a_path, ResourceSemantic a_semantic)
{
	assert(a_path != "" && "load_resource is provided empty path");

	auto  absolute_path = find_resource(a_path, a_semantic);
	auto &resource      = cache_resource(absolute_path);
	resource.load();
	return resource;
}

/**
 * Can be used to create absolute paths using the project_root
 * For example if "boy10.jpg" is provided as assets path with semantic of "textures" while parent path is "astro_boy" will result in
 * project_root/astro_boy/textures/boy10.jpg
 * if "astro_boy/boy10.jpg" is provided as assets path with semantic of "textures" while parent path is "" will result in
 * project_root/textures/astro_boy/boy10.jpg
 */
std::filesystem::path make_resource_path(const std::filesystem::path &a_path, ResourceSemantic a_semantic, const std::filesystem::path &a_parent_path)
{
	assert(a_path != "" && "create_resource is provided empty path");
	auto &project_root_path = get_project_root().path();        // Here calling get_project_root without any arguments relies on clients who must have called and initalized project_root

	std::filesystem::path semantic_path{get_resource_semantic_string(a_semantic)};
	std::filesystem::path absolute_path{project_root_path / a_parent_path / semantic_path / a_path};

	return absolute_path;
}

/**
 * Can be used to create resources relative to project_root if they don't already exist
 * Requires the path to resource that to be created and semantic and will try to create that, parent_path is optional
 * For example if "boy10.jpg" is provided as assets path with semantic of "textures" while parent path is "astro_boy" will result in
 * project_root/astro_boy/textures/boy10.jpg

 * if "astro_boy/boy10.jpg" is provided as assets path with semantic of "textures" while parent path is "" will result in
 * project_root/textures/astro_boy/boy10.jpg
*/
Resource &create_resource(const std::filesystem::path &a_path, ResourceSemantic a_semantic, const std::filesystem::path &a_parent_path)
{
	auto  absolute_path = make_resource_path(a_path, a_semantic, a_parent_path);
	auto &resource      = cache_resource(absolute_path);

	resource.create();
	return resource;
}

/**
 * Can be used to make a resource in memory relative to project_root if they don't already exist
 * Requires the path to resource that to be created and semantic and will create a Resource object corresponding to that path
 * NOTE: You need to call write on the resource at someone if you want to write it to desk
 */
Resource &make_resource(const std::filesystem::path &a_path, ResourceSemantic a_semantic, const std::filesystem::path &a_parent_path)
{
	auto  absolute_path = make_resource_path(a_path, a_semantic, a_parent_path);
	auto &resource      = cache_resource(absolute_path);

	return resource;
}

Resource &resource(const std::filesystem::path &a_path, ResourceSemantic a_semantic, ResourceAction a_action, const std::filesystem::path &a_parent_path)
{
	switch (a_action)
	{
		case ResourceAction::load:
			return load_resource(a_path, a_semantic);
		case ResourceAction::create:
			return create_resource(a_path, a_semantic, a_parent_path);
		case ResourceAction::make:
			return make_resource(a_path, a_semantic, a_parent_path);
	}
}

Resource::~Resource() noexcept
{
	this->write_or_unmap();
}

Resource::Resource(std::filesystem::path a_absolute_path, bool a_binary, bool a_read_only, bool a_mapped) :
    m_absolute_path(a_absolute_path), m_binary_file(a_binary), m_read_only(a_read_only), m_mapped(a_mapped)
{
	if (!this->m_absolute_path.is_absolute())
	{
		log_critical("{} path is not absolute, Resource only accepts absolute filename ", this->m_absolute_path.c_str());
	}

	this->m_extension = get_resource_extension(this->m_absolute_path);
}

void Resource::create()
{
	if (!std::filesystem::exists(this->m_absolute_path))
	{
		ror::log_info("Creating resource at location {}", this->m_absolute_path.c_str());

		std::ios_base::openmode mode = std::ios::ate | std::ios::out;

		if (this->m_binary_file)
			mode |= std::ios::binary;

		// Create the file, and the directories, if we tried our best but couldn't find it
		if (this->m_absolute_path.has_filename())
		{
			std::filesystem::create_directories(this->m_absolute_path.parent_path());

			if (this->m_absolute_path.has_extension())
				std::ofstream of(this->m_absolute_path, mode);
			else
				std::filesystem::create_directory(this->m_absolute_path);
		}
		else
			std::filesystem::create_directories(this->m_absolute_path);

		this->update_hashes();
	}
	else
	{
		log_critical("Trying to create resource but it already exists {}", this->m_absolute_path.c_str());
	}

	this->m_dirty = true;
}

void Resource::remove()
{
	ror::log_info("Removing resource {}", this->m_absolute_path.c_str());

	if (std::filesystem::exists(this->m_absolute_path))
	{
		auto project_root = get_project_root().path();
		assert(project_root.is_absolute() && "Project root isn't absolute, can't delete files in this path");
		assert(this->m_absolute_path.is_absolute() && "Resource path isn't absolute");
		if (this->m_absolute_path.string().find(project_root) != std::string::npos)        // Make sure its relative to the project we are working in
			std::filesystem::remove_all(this->m_absolute_path);
		else
			log_critical("Trying to delete folder {} which is outside project {}", this->m_absolute_path.c_str(), project_root.c_str());
	}

	this->m_data.clear();
	this->update_hashes();
	this->m_dirty = false;
}

void Resource::load()
{
	// Create cache or load cached file name
	this->load_or_mmap();
	this->update_hashes();
	ror::log_info("Loaded cached resource file {}", this->m_absolute_path.c_str());
}

void Resource::update_hashes()
{
	this->generate_uuid();

	// Create Path hash from absolute path as compared to cached path
	this->m_path_hash = std::filesystem::hash_value(this->m_absolute_path);

	// Create Data hash from cached data as compared to actual file data
	if (!this->m_data.empty())
		this->m_data_hash = ror::hash_64(this->m_data.data(), this->m_data.size());
	else
		this->m_data_hash = 0;
}

void Resource::load_or_mmap()
{
	// If we are asked to create mmaped file or the resource is readonly, lets mmap it
	if (this->m_mapped)        // || this->m_read_only)
	{
		assert(0 && "mmap me and consider m_read_only");
	}
	else
	{
		this->m_read_only            = true;        // TODO: Remove, although already true its here because its not used otherwise and I get warnings
		std::ios_base::openmode mode = std::ios::ate | std::ios::in;

		if (this->m_binary_file)
			mode |= std::ios::binary;

		std::ifstream as_file(this->m_absolute_path, mode);
		if (!as_file.is_open())
		{
			ror::log_critical("Can't open file, it probably doesn't exist {}", this->m_absolute_path.c_str());
			return;
		}

		// No point to synchronise here because some other process might be writing into the file
		std::streampos bytes_count = as_file.tellg();
		as_file.seekg(0, std::ios::beg);

		if (bytes_count <= 0)
		{
			ror::log_critical("Error! reading file size, it seems to be empty {}", this->m_absolute_path.c_str());
			return;
		}

		// Cast is ok because if byte_count is bigger than size_t range, we have a bigger problem
		this->m_data.resize(static_cast<size_t>(bytes_count));
		as_file.read(reinterpret_cast<char *>(this->m_data.data()), bytes_count);        // Weird that int8_t is 'signed char' and can't be converted to 'char'

		as_file.close();
	}
}

void Resource::write_or_unmap()
{
	if (this->m_dirty)
	{
		std::ios_base::openmode mode = std::ios::ate | std::ios::out;

		if (this->m_binary_file)
			mode |= std::ios::binary;

		std::ofstream as_file(this->m_absolute_path, mode);
		if (!as_file.is_open())
		{
			ror::log_critical("Can't open file for writing {}", this->m_absolute_path.c_str());
			return;
		}

		// No point to synchronise here because some other process might be writing into the file, if this is the case, expect UB

		// Cast is ok because if byte_count is bigger than size_t range, we have a bigger problem
		as_file.write(reinterpret_cast<char *>(this->m_data.data()), static_cast<long>(this->m_data.size()));
		as_file.close();

		this->m_dirty = false;
	}

	// TODO: Use the mmap feature too if (this->m_maped == true)
}

void Resource::flush()
{
	this->write_or_unmap();
}

void Resource::generate_uuid()
{
	// TODO: Implement proper UUID generation
	// Read UUID config file or generate it
	// For now using a hash
	std::string path{this->m_absolute_path};
	this->m_uuid = ror::hash_128(path.c_str(), path.size());
}

const std::vector<uint8_t> &Resource::data() const
{
	return this->m_data;
}

const std::filesystem::path &Resource::absolute_path() const
{
	return this->m_absolute_path;
}

ResourceExtension Resource::extension()
{
	return this->m_extension;
}

void Resource::update(bytes_vector &&a_data, bool a_append, bool a_mark_dirty)
{
	if (a_append)
	{
		auto psize = this->m_data.size();
		this->m_data.resize(this->m_data.size() + a_data.size());
		std::copy(a_data.begin(), a_data.end(), this->m_data.begin() + static_cast_safe<long>(psize));
	}
	else
		this->m_data = std::move(a_data);

	this->update_hashes();
	this->m_dirty = a_mark_dirty;
}

std::filesystem::path get_cache_path()
{
	auto &project_root_path = get_project_root().path();        // Calling get_project_root without any arguments relies on clients who must call to initalized project_root

	return project_root_path / ror::settings().m_roar_cache;
}

}        // namespace ror
