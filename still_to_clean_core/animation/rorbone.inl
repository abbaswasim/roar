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

namespace ror
{
FORCE_INLINE Bone::Bone(void)
{
	this->m_id        = -1;
	this->m_children  = new std::vector<int32_t>();
	this->m_parent_id = -1;
	//this->m_Keyframes				= new CoreEngine::CKeyframe*;
	this->m_keyframes           = nullptr;
	this->m_child_count         = 0;
	this->m_no_of_keyframes     = 0;
	this->m_joint_matrix        = rNew        Matrix4f();
	this->m_world_matrix        = rNew        Matrix4f();
	this->m_skinning_matrix     = rNew     Matrix4f();
	this->m_inverse_bind_matrix = rNew Matrix4f();
	//		this->m_InfluencedVertices		= new std::vector<CWeightedVertex>();
	//		this->m_InfluencedVertexCount	= 0;
}

FORCE_INLINE bool Bone::has_parent() const
{
	return (this->m_parent_id != -1);
}

FORCE_INLINE bool Bone::has_children() const
{
	return (this->m_child_count > 0);
}

FORCE_INLINE uint32_t Bone::get_child_count() const
{
	return m_child_count;
}

FORCE_INLINE void Bone::set_child_count(uint32_t a_child_count)
{
	m_child_count = a_child_count;
}

FORCE_INLINE uint32_t Bone::get_child_at(uint32_t a_index) const
{
	assert(a_index < this->m_child_count && "Bone index Out of bound");
	return (*m_children)[a_index];
}

FORCE_INLINE void Bone::update_world_matrix()
{}

FORCE_INLINE Matrix4f *Bone::get_bind_matrix() const
{
	return this->m_joint_matrix;
}

FORCE_INLINE void Bone::set_bind_matrix(ror::Matrix4f *a_bind_matrix)
{
	this->m_joint_matrix = a_bind_matrix;
}

FORCE_INLINE Matrix4f *Bone::get_world_matrix() const
{
	return m_world_matrix;
}

FORCE_INLINE void Bone::set_world_matrix(ror::Matrix4f *a_world_matrix)
{
	*this->m_world_matrix = *a_world_matrix;
}

FORCE_INLINE Matrix4f *Bone::get_runtime_matrix() const
{
	return m_skinning_matrix;
}

FORCE_INLINE void Bone::set_runtime_matrix(ror::Matrix4f *a_runtime_matrix)
{
	*this->m_skinning_matrix = *a_runtime_matrix;
}

FORCE_INLINE Matrix4f *Bone::get_inverse_bind_matrix() const
{
	return this->m_inverse_bind_matrix;
}

FORCE_INLINE void Bone::set_inverse_bind_matrix(ror::Matrix4f *a_inverse_bind_matrix)
{
	this->m_inverse_bind_matrix = a_inverse_bind_matrix;
}

FORCE_INLINE int32_t Bone::get_parent_id() const
{
	return m_parent_id;
}

FORCE_INLINE void Bone::reset() const
{
	*this->m_skinning_matrix = IDENTITY;        ///**this->m_JointMatrix;*/*this->m_WorldMatrix;
}

}        // namespace ror
