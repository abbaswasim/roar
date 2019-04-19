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
// Matrix2 operators and functions
template <class _type>
FORCE_INLINE Matrix2<_type> operator+(const Matrix2<_type> &a_lhs, const Matrix2<_type> &a_rhs)
{
	return Matrix2<_type>(a_lhs.m_values[0] + a_rhs.m_values[0], a_lhs.m_values[1] + a_rhs.m_values[1],
						  a_lhs.m_values[2] + a_rhs.m_values[2], a_lhs.m_values[3] + a_rhs.m_values[3]);
}

template <class _type>
FORCE_INLINE Matrix2<_type> operator-(const Matrix2<_type> &a_lhs, const Matrix2<_type> &a_rhs)
{
	return Matrix2<_type>(a_lhs.m_values[0] - a_rhs.m_values[0], a_lhs.m_values[1] - a_rhs.m_values[1],
						  a_lhs.m_values[2] - a_rhs.m_values[2], a_lhs.m_values[3] - a_rhs.m_values[3]);
}

template <class _type>
FORCE_INLINE Matrix2<_type> operator*(const Matrix2<_type> &a_lhs, const Matrix2<_type> &a_rhs)
{
	return Matrix2<_type>(a_lhs.m_values[0] * a_rhs.m_values[0] + a_lhs.m_values[2] * a_rhs.m_values[1],
						  a_lhs.m_values[1] * a_rhs.m_values[0] + a_lhs.m_values[3] * a_rhs.m_values[1],

						  a_lhs.m_values[0] * a_rhs.m_values[2] + a_lhs.m_values[2] * a_rhs.m_values[3],
						  a_lhs.m_values[1] * a_rhs.m_values[2] + a_lhs.m_values[3] * a_rhs.m_values[3]);
}

template <class _type>
FORCE_INLINE bool operator==(const Matrix2<_type> &a_left, const Matrix2<_type> &a_right)
{
	constexpr uint32_t matrix_size = 4;

	for (uint32_t index = 0; index < matrix_size; ++index)
	{
		if (!decimal_equal(a_left.m_values[index], a_right.m_values[index]))
		{
			return false;
		}
	}

	return true;
}

template <class _type>
FORCE_INLINE bool operator!=(const Matrix2<_type> &a_left, const Matrix2<_type> &a_right)
{
	return !(a_left == a_right);
}

template <class _type>
FORCE_INLINE Vector2<_type> operator*(const Matrix2<_type> &a_matrix, const Vector2<_type> &a_vector)
{
	return Vector2<_type>(a_matrix.m_values[0] * a_vector.x + a_matrix.m_values[2] * a_vector.y,
						  a_matrix.m_values[1] * a_vector.x + a_matrix.m_values[3] * a_vector.y);
}

// Not the same as above M * v != v * M
template <class _type>
FORCE_INLINE Vector2<_type> operator*(const Vector2<_type> &a_vector, const Matrix2<_type> &a_matrix)
{
	return Vector2<_type>(a_matrix.m_values[0] * a_vector.x + a_matrix.m_values[1] * a_vector.y,
						  a_matrix.m_values[2] * a_vector.x + a_matrix.m_values[3] * a_vector.y);
}

template <class _type>
FORCE_INLINE Matrix2<_type> operator*(const Matrix2<_type> &a_matrix, _type a_value)
{
	return Matrix2<_type>(a_matrix.m_values[0] * a_value, a_matrix.m_values[1] * a_value,
						  a_matrix.m_values[2] * a_value, a_matrix.m_values[3] * a_value);
}

template <class _type>
FORCE_INLINE Matrix2<_type> operator+(const Matrix2<_type> &a_matrix, _type a_value)
{
	return Matrix2<_type>(a_matrix.m_values[0] + a_value, a_matrix.m_values[1] + a_value,
						  a_matrix.m_values[2] + a_value, a_matrix.m_values[3] + a_value);
}

template <class _type>
FORCE_INLINE Matrix2<_type> operator-(const Matrix2<_type> &a_matrix, _type a_value)
{
	return Matrix2<_type>(a_matrix.m_values[0] - a_value, a_matrix.m_values[1] - a_value,
						  a_matrix.m_values[2] - a_value, a_matrix.m_values[3] - a_value);
}

template <class _type>
FORCE_INLINE Matrix2<_type> operator/(const Matrix2<_type> &a_matrix, _type a_value)
{
	return Matrix2<_type>(a_matrix.m_values[0] / a_value, a_matrix.m_values[1] / a_value,
						  a_matrix.m_values[2] / a_value, a_matrix.m_values[3] / a_value);
}

template <class _type>
FORCE_INLINE Matrix2<_type> matrix2_rotation_around_z(_type a_angle_in_radians)
{
	Matrix2<_type> matrix;

	matrix.m_values[0] = std::cos(a_angle_in_radians);
	matrix.m_values[1] = std::sin(a_angle_in_radians);
	matrix.m_values[2] = -matrix.m_values[1];
	matrix.m_values[3] = matrix.m_values[0];

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix2<_type> matrix2_transformation(const Matrix2<_type> &a_rotation, const Vector2<_type> &a_scaling)
{
	return a_rotation * matrix2_scaling(a_scaling);
}

template <class _type>
FORCE_INLINE Matrix2<_type> matrix2_scaling(_type a_x, _type a_y)
{
	Matrix2<_type> matrix;

	matrix.m_values[0] = a_x;
	matrix.m_values[3] = a_y;

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix2<_type> matrix2_scaling(const Vector2<_type> &a_vector)
{
	return matrix2_scaling(a_vector.x, a_vector.y);
}

template <class _type>
FORCE_INLINE void decompose_into_rotation_scaling(const Matrix2<_type> &a_matrix, Matrix2<_type> &a_rotation, Vector2<_type> &a_scaling)
{
	// https://erkaman.github.io/posts/model_matrix_recover.html
	Vector2<_type> x = a_matrix.x_axis();
	Vector2<_type> y = a_matrix.y_axis();

	a_scaling.x = x.length();
	a_scaling.y = y.length();

	a_rotation.set_x_axis(x.normalized());
	a_rotation.set_y_axis(y.normalized());
}

template <class _type>
FORCE_INLINE Matrix2<_type> matrix2_interpolate(const Matrix2<_type> &a_matrix1, const Matrix2<_type> &a_matrix2, _type a_t)
{
	// TODO Find a better way of doing this use quaternions perhaps
	Matrix2<_type> temp;

	for (uint32_t i = 0; i < 4; ++i)
	{
		temp.m_values[i] = interpolate(a_matrix1.m_values[i], a_matrix2.m_values[i], a_t);
	}

	return temp;
}

}        // namespace ror
