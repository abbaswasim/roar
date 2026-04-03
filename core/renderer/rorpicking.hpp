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

namespace ror
{

struct PickHit
{
	size_t    m_resolved_node_index{0};        // Node index in the resolved nodes_models array (matches nodes_offsets.node_offset.x)
	size_t    m_scene_node_id{0};              // Scene node instance
	int32_t   m_model_id{-1};                  // Scene node's model id
	size_t    m_model_node_id{0};              // Node id within the model
	int32_t   m_mesh_id{-1};                   // Mesh id within the model
	size_t    m_primitive_id{0};               // Primitive within the mesh
	float32_t m_local_ray_t{0.0f};
	float32_t m_ray_t{0.0f};
	float32_t m_distance_to_eye{0.0f};
};

class ROAR_ENGINE_ITEM Picking final
{
  public:
	FORCE_INLINE          Picking()                             = default;        //! Default constructor
	FORCE_INLINE          Picking(const Picking &a_other)       = default;        //! Copy constructor
	FORCE_INLINE          Picking(Picking &&a_other) noexcept   = default;        //! Move constructor
	FORCE_INLINE Picking &operator=(const Picking &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Picking &operator=(Picking &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Picking() noexcept                            = default;        //! Destructor

  protected:
  private:
	std::vector<PickHit> m_frames{};
};

}        // namespace ror

#include "rorpicking.hh"
