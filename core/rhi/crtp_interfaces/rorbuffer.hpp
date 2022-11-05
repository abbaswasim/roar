// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2022
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
#include "foundation/rorcrtp.hpp"
#include "foundation/rorutilities.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/rortypes.hpp"
#include "roar_export_import.hpp"
#include "settings/rorsettings.hpp"
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
 * CPU side buffer encapsulation. Usually a static big buffer for a bunch of stuff. Not copyable but moveable
 * Can be used for any GPU visible data like * Vertex, Index, Instance, Constant as well as texture data etc
 * But requires a device buffer for upload something like BufferMetal or BufferVulkan
 */
template <typename _type = Static>
class ROAR_ENGINE_ITEM BufferCrtp
{
  public:
	FORCE_INLINE             BufferCrtp();                                               //! Default constructor
	FORCE_INLINE             BufferCrtp(BufferCrtp &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE             BufferCrtp(const BufferCrtp &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE BufferCrtp &operator=(const BufferCrtp &a_other)      = delete;         //! Copy assignment operator
	FORCE_INLINE BufferCrtp &operator=(BufferCrtp &&a_other) noexcept  = delete;         //! Move assignment operator
	FORCE_INLINE virtual ~BufferCrtp() noexcept                        = default;        //! Destructor

	using BufferSemanticPair    = std::pair<rhi::BufferSemantic, uint64_t>;
	using BufferSemanticPairVec = std::vector<BufferSemanticPair>;

	void        copy(const uint8_t *a_data, size_t a_size, size_t a_offset);        //! Copys contents a_size bytes from data into the buffer at a_offset
	void        copy(const std::vector<uint8_t> &a_data, size_t a_offset);          //! Copys contents from the whole of data into the buffer at a_offset
	void        size(size_t a_size) noexcept;
	void        interleaved(bool a_interleaved) noexcept;
	void        emplace_semantic(BufferSemanticPair &&a_pair);
	auto        offset(size_t a_bytes);        //! Returns offset of the location available
	auto        size() const noexcept;
	auto        filled_size() const noexcept;
	auto        interleaved() const noexcept;
	auto        semantic(size_t a_index) const noexcept;
	const auto &semantics() const noexcept;
	const auto &data() const;        //! Pointer to the data store, shouldn't be manipulated directly

	template <typename U = _type, std::enable_if_t<std::is_same<U, Static>::value, bool> = true>
	auto *request(size_t a_bytes);

  protected:
  private:
	template <typename U = _type, std::enable_if_t<std::is_same<U, Dynamic>::value, bool> = true>
	auto _offset(size_t a_bytes);        //! Returns offset of the location available

	template <typename U = _type, std::enable_if_t<std::is_same<U, Static>::value, bool> = true>
	auto _offset(size_t a_bytes);        //! Returns offset of the location available

	size_t                      m_filled_size{0};            //! How much of the buffer is filled, this should be aligned, also this needs synchronising
	size_t                      m_size_in_bytes{0};          //! This is the total size in bytes
	BufferSemanticPairVec       m_semantics{};               //! Pair of semantic and size required
	bool                        m_interleaved{false};        //! Interleaved true means PNTPNTPNT, and otherwise its PPPNNNTTT
	std::vector<uint8_t>        m_data{};                    //! Data block of the buffer
	std::shared_ptr<std::mutex> m_mutex{};                   //! Mutex to lock _offset() calls with, its shared_ptr and not unique_ptr or std::mutex because I need the ctors
};

// Template deduction guide CATD for Buffer static
template <class _type>
BufferCrtp(_type) -> BufferCrtp<Static>;

}        // namespace rhi

#include "rorbuffer.hh"
