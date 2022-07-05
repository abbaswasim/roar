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

#include "rorvector3.hpp"

namespace ror
{
template <class _type>
class ROAR_ENGINE_ITEM AxisAngle final
{
  public:
	Vector3<_type> m_axis          = Vector3<_type>(0, 0, 0);        //!< Normalized axis
	_type          m_angle_radians = 0;                              //!< Angle in radians

	FORCE_INLINE            AxisAngle()                             = default;        //! Default constructor
	FORCE_INLINE            AxisAngle(const AxisAngle &a_other)     = default;        //! Copy constructor
	FORCE_INLINE            AxisAngle(AxisAngle &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE AxisAngle &operator=(const AxisAngle &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE AxisAngle &operator=(AxisAngle &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~AxisAngle() noexcept                              = default;        //! Destructor

	FORCE_INLINE      AxisAngle(const Vector3<_type> &a_axis, _type a_radians);
	FORCE_INLINE      AxisAngle(_type a_axis_x, _type a_axis_y, _type a_axis_z, _type a_radians);
	FORCE_INLINE void set(const Vector3<_type> &a_axis, _type a_radians) noexcept;
	FORCE_INLINE void set(_type a_axis_x, _type a_axis_y, _type a_axis_z, _type a_radians) noexcept;

	FORCE_INLINE bool operator==(const AxisAngle &a_other) const noexcept;
	FORCE_INLINE bool operator!=(const AxisAngle &a_other) const noexcept;

	FORCE_INLINE Vector3<_type> axis() const noexcept;
	FORCE_INLINE _type          angle() const noexcept;
	FORCE_INLINE void           set_axis(const Vector3<_type> &a_axis) noexcept;
	FORCE_INLINE void           set_angle(_type a_radians) noexcept;

  protected:
  private:
};

}        // namespace ror

#include "roraxis_angle.hh"
