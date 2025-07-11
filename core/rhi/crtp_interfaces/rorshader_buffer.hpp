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
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "rhi/rortypes.hpp"
#include <string>
#include <unordered_map>
#include <utility>

namespace rhi
{
template <class _type>
class ShaderBufferCrtp : public ror::Crtp<_type, ShaderBufferCrtp>
{
  public:
	FORCE_INLINE                   ShaderBufferCrtp(const ShaderBufferCrtp &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                   ShaderBufferCrtp(ShaderBufferCrtp &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ShaderBufferCrtp &operator=(const ShaderBufferCrtp &a_other)            = default;        //! Copy assignment operator
	FORCE_INLINE ShaderBufferCrtp &operator=(ShaderBufferCrtp &&a_other) noexcept        = default;        //! Move assignment operator
	FORCE_INLINE virtual ~ShaderBufferCrtp() noexcept override                           = default;        //! Destructor

	FORCE_INLINE bool operator==(const ShaderBufferCrtp &a_rhs) const
	{
		return this->m_shader_buffer_template == a_rhs.m_shader_buffer_template;
	}

	struct Entry
	{
		std::string       m_name{"empty_entry"};
		rhi::VertexFormat m_format{rhi::VertexFormat::float32_4};
		uint32_t          m_count{1};

		// clang-format off
		Entry() {}
		Entry(std::string a_name, rhi::VertexFormat a_format) : m_name(a_name), m_format(a_format){}
		Entry(std::string a_name, rhi::VertexFormat a_format, uint32_t a_count) : m_name(a_name), m_format(a_format), m_count(a_count) {}
		// clang-format on
	};

	struct Struct : public Entry
	{
		// clang-format off
		Struct() {}
		Struct(std::string a_name, rhi::VertexFormat a_format) : Entry(a_name, a_format) {}
		Struct(std::string a_name, rhi::VertexFormat a_format, uint32_t a_count) : Entry(a_name, a_format, a_count) {}
		// clang-format on
	};

	using entry_variant        = std::variant<std::string, rhi::VertexFormat, uint32_t, Struct>;
	using entry_variant_vector = std::vector<entry_variant>;

	template <class... _types>
	FORCE_INLINE ShaderBufferCrtp(std::string a_buffer_name, rhi::ShaderBufferType a_type, rhi::ShaderBufferFrequency a_frequency, rhi::Layout a_layout, uint32_t a_set, uint32_t a_binding, _types... a_others) :
	    m_shader_buffer_template(a_buffer_name, a_type, a_frequency, a_layout, a_set, a_binding)
	{
		entry_variant_vector others{a_others...};

		assert(others.size() >= 2 && "Not enough values for a single entry");

		bool in_struct = false;

		rhi::ShaderBufferTemplate::Struct empty_strct;
		rhi::ShaderBufferTemplate::Struct temp_struct;

		for (size_t i = 0; i < others.size();)
		{
			auto entry = get_entry(others, i, in_struct);
			if (entry.m_format == rhi::VertexFormat::struct_1)
			{
				temp_struct         = empty_strct;
				temp_struct.m_name  = entry.m_name;
				temp_struct.m_type  = entry.m_format;
				temp_struct.m_count = entry.m_count;
			}
			else
			{
				if (in_struct)
					temp_struct.add_entry(entry.m_name, entry.m_format, rhi::Layout::std140, entry.m_count);
				else
				{
					// If we were in_struct before this lets add that first and reset the temp_struct
					if (temp_struct.m_entries.size() > 0)
					{
						this->m_shader_buffer_template.add_struct(temp_struct);
						temp_struct = empty_strct;
					}
					else
						this->m_shader_buffer_template.add_entry(entry.m_name, entry.m_format, entry.m_count);
				}
			}
		}

		// If we have finished and didn't add the struct yet, so add it
		if (temp_struct.m_entries.size() > 0)
			this->m_shader_buffer_template.add_struct(temp_struct);

		this->update_variables();
	}

	FORCE_INLINE ShaderBufferCrtp(std::string a_buffer_name, rhi::ShaderBufferType a_type, rhi::ShaderBufferFrequency a_frequency, rhi::Layout a_layout, uint32_t a_set, uint32_t a_binding) :
	    m_shader_buffer_template(a_buffer_name, a_type, a_frequency, a_layout, a_set, a_binding)
	{
		this->update_variables();
	}

	// clang-format off
	FORCE_INLINE constexpr auto &shader_buffer()                                   noexcept  { return this->m_shader_buffer_template;                                              }
	FORCE_INLINE constexpr auto  variables_updated()                               noexcept  { return this->m_variables.size() > 0;                                                }
	FORCE_INLINE constexpr auto  to_glsl_string()                            const           { return this->m_shader_buffer_template.to_glsl_string();                             }
	FORCE_INLINE constexpr auto  to_glsl_string(std::string && a_modifier)   const           { return this->m_shader_buffer_template.to_glsl_string(std::move(a_modifier));        }
	FORCE_INLINE constexpr auto  set()                                       const noexcept  { return this->m_shader_buffer_template.set();                                        }
	FORCE_INLINE constexpr auto  binding()                                   const noexcept  { return this->m_shader_buffer_template.binding();                                    }
	FORCE_INLINE constexpr auto  type()                                      const noexcept  { return this->m_shader_buffer_template.type();                                       }
	FORCE_INLINE constexpr auto  frequency()                                 const noexcept  { return this->m_shader_buffer_template.frequency();                                  }
	FORCE_INLINE constexpr auto  layout()                                    const noexcept  { return this->m_shader_buffer_template.layout();                                     }
	FORCE_INLINE constexpr auto  offset()                                    const noexcept  { return this->m_shader_buffer_template.offset();                                     }
	FORCE_INLINE constexpr void  set(uint32_t a_set)                               noexcept  {        this->m_shader_buffer_template.set(a_set);                                   }
	FORCE_INLINE constexpr void  binding(uint32_t a_binding)                       noexcept  {        this->m_shader_buffer_template.binding(a_binding);                           }
	FORCE_INLINE           void  shader_buffer(rhi::ShaderBufferTemplate a_buffer) noexcept  {        this->m_shader_buffer_template = a_buffer;                                   }
	// clang-format on

	FORCE_INLINE constexpr auto stride(const std::string &a_name)
	{
		auto entry = this->m_variables[a_name];
		assert(entry && "Entry is null");
		return entry->m_stride;
	}

	FORCE_INLINE constexpr auto stride()
	{
		return this->m_shader_buffer_template.stride();
	}

	FORCE_INLINE constexpr void add_entry(const std::string &a_name, Format a_type, uint32_t a_count = 1)
	{
		this->m_shader_buffer_template.add_entry(a_name, a_type, a_count);

		this->update_variables();
	}

	FORCE_INLINE constexpr void add_struct(rhi::ShaderBufferTemplate::Struct &a_struct)
	{
		this->m_shader_buffer_template.add_struct(a_struct);

		this->update_variables();
	}

	template <typename _data_type>
	FORCE_INLINE constexpr void update(const std::string &a_variable, const _data_type *a_value, uint32_t a_index, uint32_t a_stride)
	{
		this->update(a_variable, reinterpret_cast<const uint8_t *>(a_value), a_index, a_stride);
	}

	FORCE_INLINE constexpr void update(const std::string &a_variable, const uint8_t *a_value, uint32_t a_index, uint32_t a_stride)
	{
		auto entry = this->m_variables[a_variable];
		this->buffer_copy(a_value, (a_stride * a_index) + entry->m_offset, entry->m_size);
	}

	template <typename _data_type>
	FORCE_INLINE constexpr void update(const std::string &a_variable, const _data_type *a_value)
	{
		this->update(a_variable, reinterpret_cast<const uint8_t *>(a_value));
	}

	FORCE_INLINE constexpr void update(const std::string &a_variable, const uint8_t *a_value)
	{
		auto entry = this->m_variables[a_variable];
		this->buffer_copy(a_value, entry->m_offset, entry->m_size);
	}

	// The following 4 are bulk update options, use with care
	template <typename _data_type>
	FORCE_INLINE constexpr void update(const std::string &a_variable, const _data_type *a_value, uint32_t a_size)
	{
		this->update(a_variable, reinterpret_cast<const uint8_t *>(a_value), a_size);
	}

	FORCE_INLINE constexpr void update(const std::string &a_variable, const uint8_t *a_value, uint32_t a_size)
	{
		auto entry = this->m_variables[a_variable];
		this->buffer_copy(a_value, entry->m_offset, a_size);
	}

	template <typename _data_type>
	FORCE_INLINE constexpr void update(const std::string &a_variable, uint32_t a_offset, const _data_type *a_value, uint32_t a_size)
	{
		this->update(a_variable, a_offset, reinterpret_cast<const uint8_t *>(a_value), a_size);
	}

	FORCE_INLINE constexpr void update(const std::string &a_variable, uint32_t a_offset, const uint8_t *a_value, uint32_t a_size)
	{
		auto entry = this->m_variables[a_variable];
		this->buffer_copy(a_value, entry->m_offset + a_offset, a_size);
	}

	void upload(const rhi::Device &a_device, rhi::ResourceStorageOption a_mode = rhi::ResourceStorageOption::managed)
	{
		// Alignment on size is Metal requirement but won't hurt in Vulkan either (https://github.com/gpuweb/gpuweb/issues/425)
		auto aligned_size = ror::static_cast_safe<uint32_t>(ror::align16(this->m_shader_buffer_template.size()));
		this->update_variables();
		this->buffer_init(a_device, aligned_size, a_mode);
	}

	FORCE_INLINE auto &top_level()
	{
		return this->m_shader_buffer_template.top_level();
	}

	FORCE_INLINE void update_count(const std::string &a_entry_name, uint32_t a_new_count)
	{
		this->m_shader_buffer_template.update_count(a_entry_name, a_new_count);

		this->update_variables();
	}

  protected:
	FORCE_INLINE ShaderBufferCrtp() = default;        //! Default constructor
  private:
	// clang-format off
	FORCE_INLINE constexpr void  buffer_copy(const uint8_t *a_data, size_t a_offset, size_t a_length) noexcept                { this->underlying().buffer_copy(a_data, a_offset, a_length);    }
	FORCE_INLINE           void  buffer_init(const rhi::Device& a_device, uint32_t a_size, rhi::ResourceStorageOption a_mode) { this->underlying().buffer_init(a_device, a_size, a_mode);      }
	// clang-format on

	FORCE_INLINE Entry get_entry(const entry_variant_vector &a_data, size_t &a_index, bool &a_in_struct)
	{
		Entry e;

		if (auto struct_entry = std::get_if<Struct>(&a_data[a_index]))
		{
			e.m_name   = struct_entry->m_name;
			e.m_format = struct_entry->m_format;
			e.m_count  = struct_entry->m_count;
			a_index++;
			assert(a_in_struct && "No struct entry before starting struct entries");
		}
		else
		{
			auto name   = std::get_if<std::string>(&a_data[a_index++]);
			auto format = std::get_if<rhi::VertexFormat>(&a_data[a_index++]);

			assert(name && format && "Invalid entry");

			if (*format == rhi::VertexFormat::struct_1)
				a_in_struct = true;

			if (*format == rhi::VertexFormat::struct_0)
				a_in_struct = false;

			e.m_name   = *name;
			e.m_format = *format;

			if (a_index < a_data.size())
			{
				auto count = std::get_if<uint32_t>(&a_data[a_index]);
				if (count)
				{
					e.m_count = *count;
					a_index++;
				}
			}
		}

		return e;
	}

  protected:
	FORCE_INLINE void update_variables()
	{
		this->m_variables.clear();

		// Now lets create the variables and their mapping
		auto entries = this->m_shader_buffer_template.entries_structs();
		this->m_variables.reserve(entries.size());
		for (auto entry : entries)
			this->m_variables[entry->m_name] = entry;
	}

  private:
	ShaderBufferTemplate                                                      m_shader_buffer_template{};        //! Template for this ShaderBuffer
	std::unordered_map<std::string, const rhi::ShaderBufferTemplate::Entry *> m_variables{};                     //! All the variables in this input buffer
};

template <typename _api_buffer>
class ShaderBufferBase : public ShaderBufferCrtp<ShaderBufferBase<_api_buffer>>, public _api_buffer        // _api_buffer is BufferVulkan, BufferMetal etc
{
  public:
	FORCE_INLINE                   ShaderBufferBase()                                    = delete;         //! Default constructor
	FORCE_INLINE                   ShaderBufferBase(const ShaderBufferBase &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                   ShaderBufferBase(ShaderBufferBase &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ShaderBufferBase &operator=(const ShaderBufferBase &a_other)            = delete;         //! Copy assignment operator
	FORCE_INLINE ShaderBufferBase &operator=(ShaderBufferBase &&a_other) noexcept        = delete;         //! Move assignment operator
	FORCE_INLINE virtual ~ShaderBufferBase() noexcept override                           = default;        //! Destructor

	template <class... _types>
	FORCE_INLINE ShaderBufferBase(const std::string a_buffer_name, rhi::ShaderBufferType a_type, rhi::ShaderBufferFrequency a_frequency, rhi::Layout a_layout, uint32_t a_set, uint32_t a_binding, _types... a_others) :
	    ShaderBufferCrtp<ShaderBufferBase<_api_buffer>>(a_buffer_name, a_type, a_frequency, a_layout, a_set, a_binding, a_others...)
	{}

	FORCE_INLINE ShaderBufferBase(const std::string a_buffer_name, rhi::ShaderBufferType a_type, rhi::ShaderBufferFrequency a_frequency, rhi::Layout a_layout, uint32_t a_set, uint32_t a_binding) :
	    ShaderBufferCrtp<ShaderBufferBase<_api_buffer>>(a_buffer_name, a_type, a_frequency, a_layout, a_set, a_binding)
	{}

	FORCE_INLINE bool operator==(const ShaderBufferBase &a_rhs) const
	{
		return ShaderBufferCrtp<ShaderBufferBase<_api_buffer>>::operator==(a_rhs);
	}

	declare_translation_unit_vtable() override;

	// clang-format off
	template<typename _encoder_type>
	FORCE_INLINE constexpr void  buffer_bind(_encoder_type& a_encoder, rhi::ShaderStage a_stage)                              { this->bind(a_encoder, a_stage, this->offset(), this->binding());  }
	template<typename _encoder_type>
	FORCE_INLINE constexpr void  buffer_bind(_encoder_type& a_encoder, rhi::ShaderStage a_stage, uintptr_t a_binding)         { this->bind(a_encoder, a_stage, this->offset(), a_binding);        }
	FORCE_INLINE constexpr void  buffer_unmap()                                                         noexcept              { this->unmap(); this->m_mapped_address = nullptr;                  }
	FORCE_INLINE constexpr void  buffer_map()                                                           noexcept              { this->m_mapped_address = this->map();                             }
	FORCE_INLINE constexpr void  buffer_init(const rhi::Device& a_device, uint32_t a_size, rhi::ResourceStorageOption a_mode) { this->init(a_device, a_size, a_mode);                             }
	// clang-format on

	FORCE_INLINE void buffer_copy(const uint8_t *a_data, size_t a_offset, size_t a_length)
	{
		assert(this->m_mapped_address && "Need to map the shader buffer first before copy is called");
		assert(a_data && "Data is null need to provide valid data before copy is called");

		std::memcpy(this->m_mapped_address + a_offset, a_data, a_length);
	}

	ShaderBufferBase deep_copy()
	{
		ShaderBufferBase shaderbuffer{this->top_level().m_name, this->type(), this->frequency(), this->layout(), this->set(), this->binding()};

		auto temp_sb = this->shader_buffer();

		shaderbuffer.shader_buffer(temp_sb);

		if (shaderbuffer.variables_updated())
			shaderbuffer.update_variables();

		this->m_mapped_address = nullptr;

		return shaderbuffer;
	}

	// Forbids using uploads from _api_buffer (BufferVulkan or BufferMetal)
	using ShaderBufferCrtp<ShaderBufferBase>::upload;

  protected:
  private:
	uint8_t *m_mapped_address{nullptr};        //! For batch copy the buffer will be mapped once in here and unmapped once
};

}        // namespace rhi

#include "rhi/crtp_interfaces/rorshader_buffer.hh"
