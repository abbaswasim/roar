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
#include "profiling/rorlog.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <istream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "foundation/rorcache.hpp"
#include "foundation/rorhash.hpp"

namespace ror
{
enum class ResourceSemantic
{
	materials,
	textures,        // Perhaps distinguish between 2D/3D/Cube textures
	shaders,
	scripts,
	objects,
	configs,
	models,        // Also a mesh
	caches,
	audio,
	cubemap,
	animation,
	font,
	logs,
	misc
};

std::string get_resource_semantic_string(ResourceSemantic a_semantic);

// A hash function object to work with unordered_* containers:
struct PathHash
{
	std::size_t operator()(std::filesystem::path const &a_path) const noexcept
	{
		return std::filesystem::hash_value(a_path);
	}
};

std::filesystem::path get_cache_path();
std::filesystem::path find_resource(const std::filesystem::path &, ResourceSemantic);        // Tries very hard to find the resource in the paths it knows, it has an order to it

// TODO: Use pools and/or shared mem for all resources, something encapsulating Resource(s)
// Infact make sure to use a separate pool for each ResourceSemantic type of resource, this will add in bindless descriptor assignment

class ROAR_ENGINE_ITEM Resource
{
  public:
	FORCE_INLINE           Resource(const Resource &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE           Resource(Resource &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE Resource &operator=(const Resource &a_other) = delete;            //! Copy assignment operator
	FORCE_INLINE Resource &operator=(Resource &&a_other) noexcept = delete;        //! Move assignment operator

	virtual ~Resource() noexcept;        //! Destructor

	Resource(std::filesystem::path a_absolute_path, bool a_binary = false, bool a_read_only = true, bool a_mapped = false);
	Resource(std::filesystem::path a_path, ResourceSemantic a_resource_semantic, bool a_binary = false, bool a_read_only = true, bool a_mapped = false);

	using data_ptr = std::shared_ptr<std::vector<uint8_t>>;

	// TODO: Need to work out how this works. Can one change vector via this const pointer?
	// What will be the best way to send it back in to update data
	const data_ptr get_data() const;
	void           update_data(data_ptr a_data);

	virtual void temp();

  protected:
  private:
	void load();                 // Loads the resource
	void load_or_mmap();         // Loads or mmaps the resource depending on whether its read only or not
	void generate_uuid();        // Generates or Reads UUID for the resource

	std::filesystem::path m_absolute_path{};           // Path to the resource
	data_ptr              m_data{nullptr};             // Pointer to its data
	bool                  m_binary_file{false};        // True if its a binary file and false if its text file
	bool                  m_read_only{true};           // If readonly we can optimise synchronisation and perhaps map it instead
	bool                  m_mapped{false};             // True if data is mmapped
	hash_64_t             m_path_hash{0};              // Hash of the path of the resource
	hash_64_t             m_data_hash{0};              // Hash of the contents of the resource
	hash_128_t            m_uuid{0, 0};                // The UUID of the resource, if it doesn't have one, one will be generated for it
	std::mutex            m_mutex{};                   // Mutex to lock resource load/unload and existence, this
													   // is required because we don't know if the generated filenames are used by other jobs

	// ResourceSemantic      m_semantic{ResourceSemantic::misc};        // What's the default semantic of the resource
	// std::filesystem::path m_cached_path{};                           // Cached path to the resource
};


Resource &load_resource(const std::filesystem::path &a_path, ResourceSemantic a_semantic);

// static_assert(std::is_trivially_copyable_v<Resource>, "Resource is not trivially copyable");
// static_assert(std::is_standard_layout_v<Resource>, "Resource is not standard layout");

}        // namespace ror
