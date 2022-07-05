// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2022
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

#include "foundation/rormacros.hpp"
#include "foundation/rorutilities.hpp"
#include "rhi/rortypes.hpp"
#include <cstddef>
#include <unordered_map>
#include <variant>
#include <vector>

namespace rhi
{
// Types and their base alignment according to std140 and std430? (confirm std430)

enum class Layout : uint32_t
{
	std140,
	std430
};

/**
 * Shader input Buffer encapsulation
 * Could be used for UBOs, and SSBOs etc
 */
// TODO: Have a simplified version of this, with no nest structs allowed in it
class ROAR_ENGINE_ITEM ShaderBuffer final
{
  public:
	FORCE_INLINE               ShaderBuffer();                                                 //! Default constructor
	FORCE_INLINE               ShaderBuffer(const ShaderBuffer &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               ShaderBuffer(ShaderBuffer &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ShaderBuffer &operator=(const ShaderBuffer &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE ShaderBuffer &operator=(ShaderBuffer &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE ~ShaderBuffer() noexcept                                    = default;        //! Destructor

	struct Entry
	{
		std::string m_name{};                         //! Name of the variable in the Buffer Object
		Format      m_type{Format::float32_4};        //! Data type of the variable in the Buffer Object
		uint32_t    m_count{1};                       //! How many of this Entries are there, a way to define arrays
		uint32_t    m_stride{0};                      //! Gape between elements if its an array
		uint32_t    m_offset{0};                      //! Where in the buffer would this entry be, offset from start of the ShaderBuffer
		uint32_t    m_size{0};                        //! Whats the size of this entry in machine units

		Entry(std::string a_name, Format a_type, uint32_t a_count, uint32_t a_stride, uint32_t a_offset, uint32_t a_size) :
		    m_name(a_name), m_type(a_type), m_count(a_count), m_stride(a_stride), m_offset(a_offset), m_size(a_size)
		{}

		Entry()                                    = default;        //! Default constructor
		Entry(const Entry &a_other)                = default;        //! Copy constructor
		Entry(Entry &&a_other) noexcept            = default;        //! Move constructor
		Entry &operator=(const Entry &a_other)     = default;        //! Copy assignment operator
		Entry &operator=(Entry &&a_other) noexcept = default;        //! Move assignment operator
		virtual ~Entry() noexcept                  = default;        //! Destructor

		declare_translation_unit_vtable();
	};

	struct Struct;

	using entry = std::variant<Struct, Entry>;

	struct Struct : public Entry
	{
		uint32_t           m_alignment{16};        //! Alignment of the struct, it could be alignof(vec4) or something else
		std::vector<entry> m_entries{};            //! Data block of the struct entries

		Struct(std::string a_name, uint32_t a_count);

		void add_entry(std::string a_name, Format a_type, Layout a_layout = Layout::std140, uint32_t a_count = 1);
		void add_struct(Struct a_struct);

		declare_translation_unit_vtable() override;
	};

	FORCE_INLINE ShaderBuffer(std::string a_name, Layout a_layout = rhi::Layout::std140, uint32_t a_set = 0, uint32_t a_binding = 0) :
	    m_layout(a_layout), m_set(a_set), m_binding(a_binding), m_entries(a_name, 1)
	{}

	FORCE_INLINE void add_entry(std::string a_name, Format a_type, uint32_t a_count = 1)
	{
		this->m_entries.add_entry(a_name, a_type, this->m_layout, a_count);
	}

	FORCE_INLINE void add_struct(Struct a_struct)
	{
		this->m_entries.add_struct(a_struct);
	}

	/**
	 * Use this as an iterator to get the entries in the buffer
	 * This should only be used with buffers that are "simple" and
	 * has no structs or nested structs in it.
	 * For those use entries_structs() instead
	 */
	std::vector<const Entry *> entries();

	/**
	 * Use this as an iterator to get the entries and flattend struct entries in the buffer
	 * This should only be used with buffers that are NOT "simple" and
	 * has structs or nested structs in it.
	 * For those "simple" buffers use entries() instead
	 */
	std::vector<const Entry *> entries_structs();

  private:
	Layout   m_layout{Layout::std140};        //! Default layout
	uint32_t m_set{0};                        //! Which set does this buffer belongs in
	uint32_t m_binding{0};                    //! Which binding does this buffer belongs in
	Struct   m_entries;                       //! All the entries in the buffer, Raw entries in this struct or linked-list of structures
};

}        // namespace rhi
