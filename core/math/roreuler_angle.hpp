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
class ROAR_ENGINE_ITEM EulerAngle final
{
  public:
	/** this conversion uses NASA standard aeroplane conventions as described on page:
	 *   https://www.euclideanspace.com/maths/geometry/rotations/euler/index.htm
	 *   Coordinate System: right hand
	 *   Positive angle: right hand
	 *   Order of euler angles: heading first, then attitude, then bank
	 */
	_type m_heading  = 0;        //!< Angle around Y-axis in radians
	_type m_attitude = 0;        //!< Angle around Z-axis in radians
	_type m_bank     = 0;        //!< Angle around X-axis in radians

	FORCE_INLINE             EulerAngle()                              = default;        //! Default constructor
	FORCE_INLINE             EulerAngle(const EulerAngle &a_other)     = default;        //! Copy constructor
	FORCE_INLINE             EulerAngle(EulerAngle &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE EulerAngle &operator=(const EulerAngle &a_other)      = default;        //! Copy assignment operator
	FORCE_INLINE EulerAngle &operator=(EulerAngle &&a_other) noexcept  = default;        //! Move assignment operator
	FORCE_INLINE ~EulerAngle() noexcept                                = default;        //! Destructor

	FORCE_INLINE      EulerAngle(const Vector3<_type> &a_euler);
	FORCE_INLINE      EulerAngle(_type a_heading, _type a_attitude, _type a_bank);
	FORCE_INLINE void set(const Vector3<_type> &a_euler) noexcept;
	FORCE_INLINE void set(_type a_heading, _type a_attitude, _type a_bank) noexcept;

	FORCE_INLINE bool        operator==(const EulerAngle &a_other) const noexcept;
	FORCE_INLINE bool        operator!=(const EulerAngle &a_other) const noexcept;
	FORCE_INLINE EulerAngle &operator+=(const EulerAngle &a_other) noexcept;
	FORCE_INLINE EulerAngle &operator-=(const EulerAngle &a_other) noexcept;
	FORCE_INLINE EulerAngle  operator+(const EulerAngle &a_other) const noexcept;
	FORCE_INLINE EulerAngle  operator-(const EulerAngle &a_other) const noexcept;

	FORCE_INLINE _type heading() const noexcept;
	FORCE_INLINE _type attitude() const noexcept;
	FORCE_INLINE _type bank() const noexcept;
	FORCE_INLINE void  set_heading(_type a_heading) noexcept;
	FORCE_INLINE void  set_attitude(_type a_attitude) noexcept;
	FORCE_INLINE void  set_bank(_type a_bank) noexcept;

  protected:
  private:
};

}        // namespace ror

#include "roreuler_angle.hh"
