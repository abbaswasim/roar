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

#include "configuration/rorsettings_configuration.hpp"
#include "foundation/rorutilities.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/rortypes.hpp"
#include "roar_export_import.hpp"
#include <cassert>
#include <memory>
#include <mutex>
#include <vector>

namespace rhi
{

template <bool>
class DynamicBuffer
{};

template <>
class DynamicBuffer<true>
{};

using Dynamic = DynamicBuffer<true>;
using Static  = DynamicBuffer<false>;

/**
 * Device buffer encapsulation
 * Not copyable but moveable
 * Can be used for Vertex, Index, Instance, Constant as well as texture data
 */
template <typename _type = Static>
class ROAR_ENGINE_ITEM Buffer final
{
  public:
	Buffer();                                                                   //! Default constructor
	FORCE_INLINE         Buffer(const Buffer &a_other)        = delete;         //! Copy constructor
	FORCE_INLINE         Buffer(Buffer &&a_other) noexcept    = default;        //! Move constructor
	FORCE_INLINE Buffer &operator=(const Buffer &a_other)     = delete;         //! Copy assignment operator
	FORCE_INLINE Buffer &operator=(Buffer &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~Buffer() noexcept                           = default;        //! Destructor

	// TODO: Remove the following, no need for individual semantic size, turn semantic into uint64_t value instead of vector
	using BufferSemanticPair    = std::pair<BufferSemantic, uint64_t>;
	using BufferSemanticPairVec = std::vector<BufferSemanticPair>;

	/**
	 * Returns offset of the location available
	 */
	ptrdiff_t                    offset(ptrdiff_t a_bytes);                                               //! Returns offset of the location available
	void                         upload(const uint8_t *a_data, size_t a_size, ptrdiff_t a_offset);        //! Also returns the offset where the data is uploaded
	void                         upload(const std::vector<uint8_t> &a_data, ptrdiff_t a_offset);          //! Also returns the offset where the data is uploaded
	uint32_t                     handle() noexcept;
	void                         map() noexcept;
	void                         unmap() noexcept;
	void                         size(ptrdiff_t a_size) noexcept;
	ptrdiff_t                    size() const noexcept;
	void                         interleaved(bool a_interleaved) noexcept;
	bool                         interleaved() const noexcept;
	void                         emplace_semantic(BufferSemanticPair &&a_pair);
	BufferSemanticPair           semantic(size_t a_index) const noexcept;
	const BufferSemanticPairVec &semantics() const noexcept;
	void                         gpu_upload() noexcept;        //! Uploads the buffer to the GPU
	const std::vector<uint8_t>  &data() const;                 //! Pointer to the data store, shouldn't be manipulated directly

	template <typename U = _type, std::enable_if_t<std::is_same<U, Static>::value, bool> = true>
	uint8_t *request(ptrdiff_t a_bytes);

  private:
	void _upload();                                                      // TODO: To be implemented in renderer or via CRTP
	void _partial_upload(ptrdiff_t a_offset, ptrdiff_t a_length);        // TODO: To be implemented in renderer or via CRTP

	template <typename U = _type, std::enable_if_t<std::is_same<U, Dynamic>::value, bool> = true>
	ptrdiff_t _offset(ptrdiff_t a_bytes);        //! Returns offset of the location available

	template <typename U = _type, std::enable_if_t<std::is_same<U, Static>::value, bool> = true>
	ptrdiff_t _offset(ptrdiff_t a_bytes);        //! Returns offset of the location available

	// uint32_t                    m_device_handle{0};               //! To be filled in by device buffer create calls, do I actually want this here?
	// uint8_t                    *m_mapped_address{nullptr};        //! Mapped address for write out and read in operations
	// bool                        m_mapped{false};                  //! Whether the buffer has been mapped into CPU address space
	ptrdiff_t                   m_filled_size{0};                 //! How much of the buffer is filled, this should be aligned, also this needs synchronising
	ptrdiff_t                   m_size_in_bytes{0};               //! This is the total size in bytes
	BufferSemanticPairVec       m_semantics{};                    //! Pair of semantic and size required
	bool                        m_interleaved_local{true};        //! Interleaved local means PNTPNTPNT, and otherwise its PPPNNNTTT
	std::vector<uint8_t>        m_data{};                         //! Data block of the buffer
	std::shared_ptr<std::mutex> m_mutex{};                        //! Mutex to lock _offset() calls with, its shared_ptr and not unique_ptr or std::mutex because I need the ctors
};

// Template deduction guide CATD for Buffer static
template <class _type>
Buffer(_type) -> Buffer<Static>;

}        // namespace rhi

#include "rorbuffer.hh"
