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

#include "animation/rorkeyframe.h"
#include "geometry/rorweighted_vertex.h"
#include "math/rormatrix4.h"
#include "memory_manager/rormemory_check.h"

namespace ror
{
class ROAR_ENGINE_ITEM CCBA;
class ROAR_ENGINE_ITEM Bone
{
  public:
	FORCE_INLINE Bone(void);
	~Bone(void);

	FORCE_INLINE void reset() const;
	FORCE_INLINE bool has_parent() const;
	FORCE_INLINE uint32_t get_child_at(uint32_t a_index) const;
	FORCE_INLINE bool     has_children() const;
	FORCE_INLINE uint32_t get_child_count() const;
	FORCE_INLINE int32_t get_parent_id() const;
	FORCE_INLINE void    set_child_count(uint32_t a_child_count);
	FORCE_INLINE void    update_world_matrix();
	FORCE_INLINE Matrix4f *get_bind_matrix() const;
	FORCE_INLINE void      set_bind_matrix(ror::Matrix4f *a_world_matrix);
	FORCE_INLINE Matrix4f *get_world_matrix() const;
	FORCE_INLINE void      set_world_matrix(ror::Matrix4f *a_world_matrix);
	FORCE_INLINE Matrix4f *get_runtime_matrix() const;
	FORCE_INLINE void      set_runtime_matrix(ror::Matrix4f *a_runtime_matrix);
	FORCE_INLINE Matrix4f *get_inverse_bind_matrix() const;
	FORCE_INLINE void      set_inverse_bind_matrix(ror::Matrix4f *a_world_matrix);

  private:
	// CBA should be able to access the data
	friend class ROAR_ENGINE_ITEM CCBA;

	int32_t               m_id;                         // ID of this bone
	int32_t               m_parent_id;                  // Parent ID of this bone
	ror::Matrix4f *       m_joint_matrix;               // The Bind Pose Matrix
	ror::Matrix4f *       m_inverse_bind_matrix;        // The Inverse Bind Pose Matrix
	ror::Matrix4f *       m_world_matrix;               // The World Matrix
	ror::Matrix4f *       m_skinning_matrix;            // The Matrix Used for calculations
	uint32_t              m_child_count;                // Number of Children
	std::vector<int32_t> *m_children;                   // Children of this bone
	                                                    //			uint32_t							 m_InfluencedVertexCount;	// Number of Vertices effected by this bone
	                                                    //			std::vector<CWeightedVertex>		*m_InfluencedVertices;		// Vertices effected by this bone
	uint32_t        m_no_of_keyframes;                  // No Of key frames of animation for this bone
	ror::Keyframe **m_keyframes;                        // All Key frames for this skeleton animation
};
}        // namespace ror

#include "rorbone.inl"
