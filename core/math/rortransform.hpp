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

#include "foundation/rortypes.hpp"
#include "rorquaternion.hpp"
#include "rorvector3.hpp"

namespace ror
{
/**
 * Similar to Matrix4 but has rotation, translation and scaling components
 */
template <class _type>
class ROAR_ENGINE_ITEM Transform final
{
  public:
	Quaternion<_type> m_rotation{};                  //!< Rotation component of the transform
	Vector3<_type>    m_translation{0, 0, 0};        //!< Translation or origin of the transform
	Vector3<_type>    m_scale{1, 1, 1};              //!< Scaling component of the transform

	FORCE_INLINE            Transform()                             = default;        //! Default constructor
	FORCE_INLINE            Transform(const Transform &a_other)     = default;        //! Copy constructor
	FORCE_INLINE            Transform(Transform &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE Transform &operator=(const Transform &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Transform &operator=(Transform &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Transform() noexcept                              = default;        //! Destructor

	FORCE_INLINE Transform(const Quaternion<_type> &a_rotation, const Vector3<_type> &a_translation, const Vector3<_type> &a_scale);
	FORCE_INLINE Transform(const Matrix3<_type> &a_matrix) noexcept;
	FORCE_INLINE Transform(const Matrix4<_type> &a_matrix) noexcept;
	FORCE_INLINE Transform(const Matrix3x4<_type> &a_matrix) noexcept;

	FORCE_INLINE void set(const Quaternion<_type> &a_rotation, const Vector3<_type> &a_translation, const Vector3<_type> &a_scale) noexcept;
	FORCE_INLINE void set(const Matrix3<_type> &a_matrix) noexcept;
	FORCE_INLINE void set(const Matrix4<_type> &a_matrix) noexcept;
	FORCE_INLINE void set(const Matrix3x4<_type> &a_matrix) noexcept;

	FORCE_INLINE Quaternion<_type> rotation() const;
	FORCE_INLINE Vector3<_type> translation() const;
	FORCE_INLINE Vector3<_type> scale() const;

	FORCE_INLINE void rotation(const Quaternion<_type> &a_rotation) noexcept;
	FORCE_INLINE void translation(const Vector3<_type> &a_translation) noexcept;
	FORCE_INLINE void scale(const Vector3<_type> &a_scale) noexcept;

  protected:
  private:
};

static_assert(sizeof(Transform<float32_t>) == sizeof(float32_t) * 10, "Size of transform is wrong");

using Transformf = Transform<float32_t>;
using Transformd = Transform<double64_t>;

}        // namespace ror

#include "rortransform.hh"
