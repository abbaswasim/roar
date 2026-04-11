// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2026
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
#include "math/rorvector2.hpp"
#include <array>
#include <vector>

namespace ror
{
struct PickHit
{
	uint32_t m_scene_node_id{0};        //! Scene node id
	int32_t  m_model_id{-1};            //! Scene node's model id
	uint32_t m_model_node_id{0};        //! Node id within the model
	int32_t  m_mesh_id{-1};             //! Mesh id within the model
	uint32_t m_primitive_id{0};         //! Primitive id within the mesh
};

template <size_t _frame_count>
class ROAR_ENGINE_ITEM Picking final
{
  public:
	FORCE_INLINE          Picking(const Picking &a_other)       = default;        //! Copy constructor
	FORCE_INLINE          Picking(Picking &&a_other) noexcept   = default;        //! Move constructor
	FORCE_INLINE Picking &operator=(const Picking &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Picking &operator=(Picking &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Picking() noexcept                            = default;        //! Destructor

	// clang-format off
	FORCE_INLINE void register_hit(PickHit a_hit)                                 { this->m_hits.emplace_back(a_hit);               }
	FORCE_INLINE void register_target(ror::Vector2f a_target, uint32_t a_frame)   { this->m_targets[a_frame] = a_target;            }
	FORCE_INLINE constexpr auto &hits(uint32_t a_frame)                           { return this->m_hits[a_frame];                   }
	FORCE_INLINE constexpr auto &target(uint32_t a_frame)                  const  { return this->m_targets[a_frame];                }
	FORCE_INLINE constexpr void  clear()                                          { return this->m_hits.clear();                    }
	FORCE_INLINE          Picking()                                               { for (auto& h: this->m_hits) h.reserve(20);      }        //! Default constructor
	// clang-format on

  protected:
  private:
	// clang-format off
	FORCE_INLINE constexpr auto &hits()                   const {    return this->m_hits;                            }
	// clang-format on

	std::array<ror::Vector2f, _frame_count>        m_targets{};        //! All the hit location in the framebuffer
	std::array<std::vector<PickHit>, _frame_count> m_hits{};           //! All the hits for all the frames
};

}        // namespace ror

#include "rorpicking.hh"
