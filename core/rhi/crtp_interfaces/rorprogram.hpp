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
#include "core/foundation/rorcrtp.hpp"

namespace rhi
{
template <class _type>
class ProgramCrtp : public ror::Crtp<_type, ProgramCrtp>
{
  public:
	FORCE_INLINE              ProgramCrtp(const ProgramCrtp &a_other)     = default;        //! Copy constructor
	FORCE_INLINE              ProgramCrtp(ProgramCrtp &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ProgramCrtp &operator=(const ProgramCrtp &a_other)       = default;        //! Copy assignment operator
	FORCE_INLINE ProgramCrtp &operator=(ProgramCrtp &&a_other) noexcept   = default;        //! Move assignment operator
	FORCE_INLINE virtual ~ProgramCrtp() noexcept override                 = default;        //! Destructor

	FORCE_INLINE ProgramCrtp(int32_t a_vert_id, int32_t a_frag_id) :
	    m_vertex_id(a_vert_id), m_fragment_id(a_frag_id)
	{}

	FORCE_INLINE explicit ProgramCrtp(int32_t a_compute_id) :
	    m_compute_id(a_compute_id)
	{}

	// clang-format off
	FORCE_INLINE constexpr void vertex_id(int32_t        a_id)   { this->m_vertex_id = a_id;    }
	FORCE_INLINE constexpr void fragment_id(int32_t      a_id)   { this->m_fragment_id = a_id;  }
	FORCE_INLINE constexpr void compute_id(int32_t       a_id)   { this->m_compute_id = a_id;   }
	FORCE_INLINE constexpr void tile_id(int32_t          a_id)   { this->m_tile_id = a_id;      }
	FORCE_INLINE constexpr void mesh_id(int32_t          a_id)   { this->m_mesh_id = a_id;      }
	// FORCE_INLINE constexpr void upload()                         { this->underlying().upload(); }

	FORCE_INLINE constexpr auto vertex_id()     const noexcept { return this->m_vertex_id;   }
	FORCE_INLINE constexpr auto fragment_id()   const noexcept { return this->m_fragment_id; }
	FORCE_INLINE constexpr auto compute_id()    const noexcept { return this->m_compute_id;  }
	FORCE_INLINE constexpr auto tile_id()       const noexcept { return this->m_tile_id;     }
	FORCE_INLINE constexpr auto mesh_id()       const noexcept { return this->m_mesh_id;     }
	// clang-format on

  protected:
	FORCE_INLINE ProgramCrtp() = default;        //! Default constructor
  private:
	int32_t m_vertex_id{-1};
	int32_t m_fragment_id{-1};
	int32_t m_compute_id{-1};
	int32_t m_tile_id{-1};
	int32_t m_mesh_id{-1};
	// NOTE: If any other types are added make sure to add it into ShaderUpdater shaders mapping in rorshader_update.cpp
};

}        // namespace rhi

#include "rhi/crtp_interfaces/rorprogram.hh"
