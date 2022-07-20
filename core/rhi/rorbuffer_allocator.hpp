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

#include "profiling/rorlog.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include "rhi/rortypes.hpp"
#include <cstddef>

// #define WITHOUT_BUFFER_ALLOCATOR

namespace rhi
{
/**
 * A very simple std::vector allocator, it shouldn't be used with anything else
 * It allocates a static block, that can never be released or reused or resized
 * It kind of works like a placement new for vectors
 * This allocator has global state, it works directly on global buffer_pack
 * For each type of allocations the allocator knows about, it tries to find a buffer
 * from that type semantic and uses that for allocation
 */
#if defined(WITHOUT_BUFFER_ALLOCATOR)
template <class _type>
class ROAR_ENGINE_ITEM BufferAllocator : public std::allocator<_type>
{};
#else
template <class _type>
class ROAR_ENGINE_ITEM BufferAllocator
{
  public:
	FORCE_INLINE                  BufferAllocator()                                   = default;        //! Default constructor
	FORCE_INLINE                  BufferAllocator(const BufferAllocator &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                  BufferAllocator(BufferAllocator &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE BufferAllocator &operator=(const BufferAllocator &a_other)           = default;        //! Copy assignment operator
	FORCE_INLINE BufferAllocator &operator=(BufferAllocator &&a_other) noexcept       = default;        //! Move assignment operator
	FORCE_INLINE virtual ~BufferAllocator() noexcept                                  = default;        //! Destructor

	typedef _type value_type;

	template <class _other_type>
	FORCE_INLINE constexpr BufferAllocator(const BufferAllocator<_other_type> &) noexcept
	{
		assert(0 && "Type cast constructor for allocator not defined.");
	}

	[[nodiscard]] FORCE_INLINE _type *allocate(std::size_t a_count) const
	{
		auto  semantic = get_format_shader_semantic<_type>();
		auto &bp       = rhi::get_buffers_pack();
		auto &b        = bp.buffer(semantic);

		if constexpr (ror::get_build() == ror::BuildType::build_debug)
		{
			if (b.interleaved())        // Can't use sequential allocators for interleaved buffers // TODO: Find a solution
				throw std::bad_alloc();
		}

		// This is thread safe because Buffer is thread safe
		// This is only valid for Static Buffers, b.request will change if its dynamic and this won't work
		return reinterpret_cast<_type *>(b.request(static_cast<ptrdiff_t>(a_count * sizeof(_type))));
	}

	FORCE_INLINE void deallocate(_type *a_pointer, std::size_t a_count) noexcept
	{
		(void) a_pointer;
		(void) a_count;

		if constexpr (ror::get_build() == ror::BuildType::build_debug)
		{
			const auto &bp = rhi::get_buffers_pack();
			if (!bp.ready_to_free())
				assert(0 && "BufferAllocator can't deallocate at the moment, this shouldn't happen, something went wrong");
		}
	}

  protected:
  private:
};

template <class _type_0, class _type_1>
bool operator==(const BufferAllocator<_type_0> &, const BufferAllocator<_type_1> &)
{
	assert(0 && "Equality operator not defined for BufferAllocator");
	return true;
}

template <class _type_0, class _type_1>
bool operator!=(const BufferAllocator<_type_0> &, const BufferAllocator<_type_1> &)
{
	assert(0 && "Inequality operator not defined for BufferAllocator");
	return false;
}
#endif

// A failed atempt to create a buffer view based allocator
// template <class _type>
// class ROAR_ENGINE_ITEM BufferAllocator final
// {
//   public:
//	FORCE_INLINE                  BufferAllocator()                                   = delete;         //! Default constructor
//	FORCE_INLINE                  BufferAllocator(const BufferAllocator &a_other)     = default;        //! Copy constructor
//	FORCE_INLINE                  BufferAllocator(BufferAllocator &&a_other) noexcept = default;        //! Move constructor
//	FORCE_INLINE BufferAllocator &operator=(const BufferAllocator &a_other) = default;                  //! Copy assignment operator
//	FORCE_INLINE BufferAllocator &operator=(BufferAllocator &&a_other) noexcept = default;              //! Move assignment operator
//	FORCE_INLINE ~BufferAllocator() noexcept                                    = default;              //! Destructor

//	typedef _type value_type;

//	FORCE_INLINE explicit BufferAllocator(BufferView &a_buffer_view) :
//		m_buffer_view(a_buffer_view)
//	{}

//	template <class _other_type>
//	constexpr BufferAllocator(const BufferAllocator<_other_type> &) noexcept
//	{
//		assert(0 && "Type cast constructor for allocator not defined.");
//	}

//	[[nodiscard]] FORCE_INLINE _type *allocate(std::size_t a_count)
//	{
//		// if (a_count > (this->m_bytes_owned - this->m_allocated_bytes) / sizeof(_type))
//		if (a_count > this->m_buffer_view.m_count - this->m_allocated_count)
//			throw std::bad_array_new_length();

//		if constexpr (ror::get_build() == ror::BuildType::build_debug)
//		{
//			const auto &bp = rhi::get_buffers_pack();
//			const auto &b  = bp.buffer(this->m_index);

//			if (b.interleaved())        // Can't use sequential allocators for interleaved buffers // TODO: Find a solution
//				throw std::bad_alloc();
//		}

//		// if (auto pointer = static_cast<_type *>(std::malloc(a_count * sizeof(_type))))
//		auto pointer = static_cast<_type *>(a_count * sizeof(_type));

//		if (auto pointe = static_cast<_type *>((a_count * sizeof(_type))))
//		{
//			return pointer;
//		}

//		throw std::bad_alloc();
//	}

//	FORCE_INLINE void deallocate(_type *a_pointer, std::size_t a_count) noexcept
//	{
//		(void) a_pointer;
//		(void) a_count;
//		assert(0 && "BufferAllocator can't deallocate at the moment, something went wrong");
//		// std::free(a_pointer);
//	}

//   protected:
//   private:
//	const BufferView &m_buffer_view;               //! Which buffer is used for allocations
//	size_t            m_allocated_count{0};        //! How many of the count items from m_buffer_view we can use are used already

//	// const size_t      m_bytes_owned{0};            //! How many of the bytes from m_buffer we can use for allocations
//	// size_t            m_allocated_bytes{0};        //! How many of the bytes from m_buffer we can use for allocations are used already
// };

// template <class _type_0, class _type_1>
// bool operator==(const BufferAllocator<_type_0> &, const BufferAllocator<_type_1> &)
// {
//	assert(0 && "Equality operator not defined for BufferAllocator");
//	return true;
// }

// template <class _type_0, class _type_1>
// bool operator!=(const BufferAllocator<_type_0> &, const BufferAllocator<_type_1> &)
// {
//	assert(0 && "Inequality operator not defined for BufferAllocator");
//	return false;
// }
}        // namespace rhi
