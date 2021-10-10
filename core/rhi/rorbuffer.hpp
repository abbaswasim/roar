// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2021
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

#include "rhi/rortypes.hpp"
#include "roar_export_import.hpp"

namespace rhi
{
/**
 * Device buffer encapsulation
 * Can be used for Vertex, Index, Instance, Constant as well as texture data
 */
class ROAR_ENGINE_ITEM Buffer        // : public Crtp<Buffer>
{
  public:
	Buffer();                                                                   //! Default constructor
	FORCE_INLINE Buffer(const Buffer &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Buffer(Buffer &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Buffer &operator=(const Buffer &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Buffer &operator=(Buffer &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~Buffer() noexcept                   = default;        //! Destructor // TODO: Remove if not to be used as polymorphic type

	declare_translation_unit_vtable();

	// FORCE_INLINE Buffer(uint32_t a_size)
	// {
	//	this->m_size_in_bytes = a_size;
	//	// TODO: Dispatch for device buffer creation
	// }

	/**
	 * Returns offset of the location available
	 */
	uint64_t request(uint64_t a_bytes);                         //! Returns offset of the location available
	uint64_t upload(uint8_t &a_data, uint64_t a_length);        //! Also returns the offset where the data is uploaded
	uint32_t handle();
	void     map();
	void     unmap();
	void     size(uint64_t a_size);
	uint64_t size() const noexcept;
	void     interleaved(bool a_interleaved);
	bool     interleaved() const noexcept;
	void     emplace_semantic(std::pair<ShaderSemantic, uint64_t> &&a_pair);

	std::pair<ShaderSemantic, uint64_t>                     semantic(size_t a_index) const noexcept;
	const std::vector<std::pair<ShaderSemantic, uint64_t>> &semantics() const noexcept;

  private:
	void _upload(uint8_t &a_data, uint64_t a_length);        // TODO: To be implemented in renderer or via CRTP

	// uint32_t                                         m_device_handle{0};               //! To be filled in by device buffer create calls, do I actually want this here?
	// uint8_t *                                        m_mapped_address{nullptr};        //! Mapped address for write out and read in operations
	// bool                                             m_mapped{false};                  //! Whether the buffer has been mapped into CPU address space
	uint64_t                                         m_filled_size{0};                 //! How much of the buffer is filled, this should be aligned(to_something)
	uint64_t                                         m_size_in_bytes{0};               //! This is the size in bytes
	std::vector<std::pair<ShaderSemantic, uint64_t>> m_semantics{};                    //! Pair of semantic and size required
	bool                                             m_interleaved_local{true};        //! Interleaved local means PNTPNTPNT, and otherwise its PPPNNNTTT
};

}        // namespace rhi
