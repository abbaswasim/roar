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

#include "geometry.hpp"

namespace ror_test
{
template <class _type>
void GeometryTest<_type>::SetUp()
{}

template <class _type>
void GeometryTest<_type>::TearDown()
{}

TYPED_TEST(GeometryTest, cube_test_3d)
{
#if 0
	// Proves Vector4 not allowed
	ror::Vector4<TypeParam> size{10, 20, 30, 40}, origin{5, 15, 25, 35};
	std::vector<ror::Vector4<TypeParam>> output;
	ror::make_box_triangles(size, origin , output);
#endif

	const uint32_t indices_tris[]  = {0, 1, 2, 2, 1, 3, 4, 5, 0, 0, 5, 1, 0, 1, 6, 6, 1, 7, 4, 0, 6, 6, 0, 2, 0, 5, 3, 3, 5, 7, 5, 4, 7, 7, 4, 6};
	const size_t   indices_lines[] = {0, 1, 1, 3, 3, 2, 2, 0, 0, 4, 1, 5, 2, 6, 3, 7, 4, 5, 5, 7, 7, 6, 6, 4};

	if constexpr (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		{
			ror::Vector3<TypeParam>              size{1.0f, 1.0f, 1.0f}, origin{0.0f, 0.0f, 0.0f};
			std::vector<ror::Vector3<TypeParam>> res_points = {{-0.5f, -0.5f, -0.5f},
															   {0.5f, -0.5f, -0.5f},
															   {-0.5f, 0.5f, -0.5f},
															   {0.5f, 0.5f, -0.5f},
															   {-0.5f, -0.5f, 0.5f},
															   {0.5f, -0.5f, 0.5f},
															   {-0.5f, 0.5f, 0.5f},
															   {0.5f, 0.5f, 0.5f}};
			std::vector<ror::Vector3<TypeParam>> res_tris;
			res_tris.reserve(36);

			for (uint32_t i = 0; i < 12; ++i)
			{
				res_tris.emplace_back(res_points[indices_tris[3 * i]]);
				res_tris.emplace_back(res_points[indices_tris[3 * i + 1]]);
				res_tris.emplace_back(res_points[indices_tris[3 * i + 2]]);
			}

			std::vector<ror::Vector3<TypeParam>> res_lines;
			res_lines.reserve(24);

			for (uint32_t i = 0; i < 12; ++i)
			{
				res_lines.emplace_back(res_points[indices_lines[2 * i]]);
				res_lines.emplace_back(res_points[indices_lines[2 * i + 1]]);
			}

			{
				std::vector<ror::Vector3<TypeParam>> output;
				ror::make_box_triangles(size, origin, output);
				for (size_t i = 0; i < 36; ++i)
				{
					test_vector3_equal(res_tris[i], output[i]);
				}
			}

			{
				std::vector<ror::Vector3<TypeParam>> output;
				// Use overload
				ror::make_box_triangles(static_cast<float32_t>(size.x), origin, output);
				for (size_t i = 0; i < 36; ++i)
				{
					test_vector3_equal(res_tris[i], output[i]);
				}
			}
			{
				std::vector<ror::Vector3<TypeParam>> output;
				// Use overload
				ror::make_box_triangles(static_cast<float32_t>(size.x), output);
				for (size_t i = 0; i < 36; ++i)
				{
					test_vector3_equal(res_tris[i], output[i]);
				}
			}
			{
				// Use overload
				std::vector<ror::Vector3<TypeParam>> output = ror::make_box_triangles<ror::Vector3<TypeParam>>(static_cast<float32_t>(size.x));
				for (size_t i = 0; i < 36; ++i)
				{
					test_vector3_equal(res_tris[i], output[i]);
				}
			}
			{
				ror::Vector3<TypeParam> minimum{-0.5f, -0.5f, -0.5f}, maximum{0.5f, 0.5f, 0.5f};
				// Use min/max overload
				std::vector<ror::Vector3<TypeParam>> output;
				ror::make_box_triangles(output, minimum, maximum);
				for (size_t i = 0; i < 36; ++i)
				{
					test_vector3_equal(res_tris[i], output[i]);
				}
			}
			{
				std::vector<ror::Vector3<TypeParam>> output;
				std::vector<ror::Vector3<TypeParam>> index_buffer;
				// Use overload
				ror::make_box_triangles_indexed(size, origin, output, index_buffer);
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector3_equal(res_points[i], output[i]);
				}
				for (size_t i = 0; i < 12; ++i)
				{
					test_vector3_equal(ror::Vector3<TypeParam>(indices_tris[3 * i], indices_tris[3 * i + 1], indices_tris[3 * i + 2]), index_buffer[i]);
				}
			}
			{
				std::vector<ror::Vector3<TypeParam>> output;
				std::vector<ror::Vector3<TypeParam>> index_buffer;

				// Use overload for unit cube
				ror::make_box_triangles_indexed(output, index_buffer);

				for (size_t i = 0; i < 8; ++i)
				{
					test_vector3_equal(res_points[i], output[i]);
				}

				for (size_t i = 0; i < 12; ++i)
				{
					test_vector3_equal(ror::Vector3<TypeParam>(indices_tris[3 * i], indices_tris[3 * i + 1], indices_tris[3 * i + 2]), index_buffer[i]);
				}
			}
			// Lines version of the tests
			{
				std::vector<ror::Vector3<TypeParam>> output;
				ror::make_box_lines(size, origin, output);
				for (size_t i = 0; i < 24; ++i)
				{
					test_vector3_equal(res_lines[i], output[i]);
				}
			}

			{
				std::vector<ror::Vector3<TypeParam>> output;
				// Use overload
				ror::make_box_lines(static_cast<float32_t>(size.x), origin, output);
				for (size_t i = 0; i < 24; ++i)
				{
					test_vector3_equal(res_lines[i], output[i]);
				}
			}
			{
				std::vector<ror::Vector3<TypeParam>> output;
				// Use overload
				ror::make_box_lines(static_cast<float32_t>(size.x), output);
				for (size_t i = 0; i < 24; ++i)
				{
					test_vector3_equal(res_lines[i], output[i]);
				}
			}
			{
				// Use overload
				std::vector<ror::Vector3<TypeParam>> output = ror::make_box_lines<ror::Vector3<TypeParam>>(static_cast<float32_t>(size.x));
				for (size_t i = 0; i < 24; ++i)
				{
					test_vector3_equal(res_lines[i], output[i]);
				}
			}
			{
				ror::Vector3<TypeParam> minimum{-0.5f, -0.5f, -0.5f}, maximum{0.5f, 0.5f, 0.5f};
				// Use min/max overload
				std::vector<ror::Vector3<TypeParam>> output;
				ror::make_box_lines(output, minimum, maximum);
				for (size_t i = 0; i < 24; ++i)
				{
					test_vector3_equal(res_lines[i], output[i]);
				}
			}
			{
				std::vector<ror::Vector3<TypeParam>> output;
				std::vector<ror::Vector2<TypeParam>> index_buffer;
				// Use overload
				ror::make_box_lines_indexed(size, origin, output, index_buffer);
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector3_equal(res_points[i], output[i]);
				}
				for (size_t i = 0; i < 12; ++i)
				{
					test_vector2_equal(ror::Vector2<TypeParam>(indices_lines[2 * i], indices_lines[2 * i + 1]), index_buffer[i]);
				}
			}
			{
				std::vector<ror::Vector3<TypeParam>> output;
				std::vector<ror::Vector2<TypeParam>> index_buffer;

				// Use overload for unit cube
				ror::make_box_lines_indexed(output, index_buffer);

				for (size_t i = 0; i < 8; ++i)
				{
					test_vector3_equal(res_points[i], output[i]);
				}

				for (size_t i = 0; i < 12; ++i)
				{
					test_vector2_equal(ror::Vector2<TypeParam>(indices_lines[2 * i], indices_lines[2 * i + 1]), index_buffer[i]);
				}
			}
		}
		{
			ror::Vector3<TypeParam>              size{1.0f, 1.0f, 1.0f}, origin{0.5f, 0.5f, 0.5f};
			std::vector<ror::Vector3<TypeParam>> res_points = {{0.0f, 0.0f, 0.0f},
															   {1.0f, 0.0f, 0.0f},
															   {0.0f, 1.0f, 0.0f},
															   {1.0f, 1.0f, 0.0f},
															   {0.0f, 0.0f, 1.0f},
															   {1.0f, 0.0f, 1.0f},
															   {0.0f, 1.0f, 1.0f},
															   {1.0f, 1.0f, 1.0f}};

			std::vector<ror::Vector3<TypeParam>> res_tris;
			res_tris.reserve(36);

			for (uint32_t i = 0; i < 12; ++i)
			{
				res_tris.emplace_back(res_points[indices_tris[3 * i]]);
				res_tris.emplace_back(res_points[indices_tris[3 * i + 1]]);
				res_tris.emplace_back(res_points[indices_tris[3 * i + 2]]);
			}

			std::vector<ror::Vector3<TypeParam>> res_lines;
			res_lines.reserve(24);

			for (uint32_t i = 0; i < 12; ++i)
			{
				res_lines.emplace_back(res_points[indices_lines[2 * i]]);
				res_lines.emplace_back(res_points[indices_lines[2 * i + 1]]);
			}

			{
				std::vector<ror::Vector3<TypeParam>> output;
				ror::make_box_triangles(size, origin, output);
				for (size_t i = 0; i < 36; ++i)
				{
					test_vector3_equal(res_tris[i], output[i]);
				}
			}
			{
				std::vector<ror::Vector3<TypeParam>> output;
				ror::make_box_lines(size, origin, output);
				for (size_t i = 0; i < 24; ++i)
				{
					test_vector3_equal(res_lines[i], output[i]);
				}
			}
		}
		{
			ror::Vector3<TypeParam>              size{1.0f, 1.0f, 1.0f}, origin{-0.5f, -0.5f, -0.5f};
			std::vector<ror::Vector3<TypeParam>> output;
			std::vector<ror::Vector3<TypeParam>> res_points = {{-1.0f, -1.0f, -1.0f},
															   {0.0f, -1.0f, -1.0f},
															   {-1.0f, 0.0f, -1.0f},
															   {0.0f, 0.0f, -1.0f},
															   {-1.0f, -1.0f, 0.0f},
															   {0.0f, -1.0f, 0.0f},
															   {-1.0f, 0.0f, 0.0f},
															   {0.0f, 0.0f, 0.0f}};

			std::vector<ror::Vector3<TypeParam>> res_tris;
			res_tris.reserve(36);

			for (uint32_t i = 0; i < 12; ++i)
			{
				res_tris.emplace_back(res_points[indices_tris[3 * i]]);
				res_tris.emplace_back(res_points[indices_tris[3 * i + 1]]);
				res_tris.emplace_back(res_points[indices_tris[3 * i + 2]]);
			}

			std::vector<ror::Vector3<TypeParam>> res_lines;
			res_lines.reserve(24);

			for (uint32_t i = 0; i < 12; ++i)
			{
				res_lines.emplace_back(res_points[indices_lines[2 * i]]);
				res_lines.emplace_back(res_points[indices_lines[2 * i + 1]]);
			}

			ror::make_box_triangles(size, origin, output);
			for (size_t i = 0; i < 36; ++i)
			{
				test_vector3_equal(res_tris[i], output[i]);
			}

			output.clear();
			ror::make_box_lines(size, origin, output);
			for (size_t i = 0; i < 24; ++i)
			{
				test_vector3_equal(res_lines[i], output[i]);
			}
		}
	}

	{
		ror::Vector3<TypeParam>              size{10, 20, 30}, origin{0, 0, 0};
		std::vector<ror::Vector3<TypeParam>> output;
		std::vector<ror::Vector3<TypeParam>> res_points = {{-5, -10, -15}, {5, -10, -15}, {-5, 10, -15}, {5, 10, -15}, {-5, -10, 15}, {5, -10, 15}, {-5, 10, 15}, {5, 10, 15}};

		std::vector<ror::Vector3<TypeParam>> res_tris;
		res_tris.reserve(36);

		for (uint32_t i = 0; i < 12; ++i)
		{
			res_tris.emplace_back(res_points[indices_tris[3 * i]]);
			res_tris.emplace_back(res_points[indices_tris[3 * i + 1]]);
			res_tris.emplace_back(res_points[indices_tris[3 * i + 2]]);
		}

		std::vector<ror::Vector3<TypeParam>> res_lines;
		res_lines.reserve(24);

		for (uint32_t i = 0; i < 12; ++i)
		{
			res_lines.emplace_back(res_points[indices_lines[2 * i]]);
			res_lines.emplace_back(res_points[indices_lines[2 * i + 1]]);
		}

		ror::make_box_triangles(size, origin, output);
		for (size_t i = 0; i < 36; ++i)
		{
			test_vector3_equal(res_tris[i], output[i]);
		}

		output.clear();

		ror::make_box_lines(size, origin, output);
		for (size_t i = 0; i < 24; ++i)
		{
			test_vector3_equal(res_lines[i], output[i]);
		}
	}

	{
		ror::Vector3<TypeParam>              size{10, 20, 30}, origin{5, 15, 25};
		std::vector<ror::Vector3<TypeParam>> output;
		std::vector<ror::Vector3<TypeParam>> res_points = {{0, 5, 10}, {10, 5, 10}, {0, 25, 10}, {10, 25, 10}, {0, 5, 40}, {10, 5, 40}, {0, 25, 40}, {10, 25, 40}};

		std::vector<ror::Vector3<TypeParam>> res_tris;
		res_tris.reserve(36);

		for (uint32_t i = 0; i < 12; ++i)
		{
			res_tris.emplace_back(res_points[indices_tris[3 * i]]);
			res_tris.emplace_back(res_points[indices_tris[3 * i + 1]]);
			res_tris.emplace_back(res_points[indices_tris[3 * i + 2]]);
		}
		std::vector<ror::Vector3<TypeParam>> res_lines;
		res_lines.reserve(24);

		for (uint32_t i = 0; i < 12; ++i)
		{
			res_lines.emplace_back(res_points[indices_lines[2 * i]]);
			res_lines.emplace_back(res_points[indices_lines[2 * i + 1]]);
		}

		ror::make_box_triangles(size, origin, output);
		for (size_t i = 0; i < 36; ++i)
		{
			test_vector3_equal(res_tris[i], output[i]);
		}

		output.clear();

		ror::make_box_lines(size, origin, output);
		for (size_t i = 0; i < 24; ++i)
		{
			test_vector3_equal(res_lines[i], output[i]);
		}
	}

	{
		ror::Vector3<TypeParam>              size{10, 20, 30}, origin{10, 20, 30};
		std::vector<ror::Vector3<TypeParam>> output;
		std::vector<ror::Vector3<TypeParam>> res_points = {{5, 10, 15}, {15, 10, 15}, {5, 30, 15}, {15, 30, 15}, {5, 10, 45}, {15, 10, 45}, {5, 30, 45}, {15, 30, 45}};

		std::vector<ror::Vector3<TypeParam>> res_tris;
		res_tris.reserve(36);

		for (uint32_t i = 0; i < 12; ++i)
		{
			res_tris.emplace_back(res_points[indices_tris[3 * i]]);
			res_tris.emplace_back(res_points[indices_tris[3 * i + 1]]);
			res_tris.emplace_back(res_points[indices_tris[3 * i + 2]]);
		}
		std::vector<ror::Vector3<TypeParam>> res_lines;
		res_lines.reserve(24);

		for (uint32_t i = 0; i < 12; ++i)
		{
			res_lines.emplace_back(res_points[indices_lines[2 * i]]);
			res_lines.emplace_back(res_points[indices_lines[2 * i + 1]]);
		}

		ror::make_box_triangles(size, origin, output);
		for (size_t i = 0; i < 36; ++i)
		{
			test_vector3_equal(res_tris[i], output[i]);
		}

		output.clear();

		ror::make_box_lines(size, origin, output);
		for (size_t i = 0; i < 24; ++i)
		{
			test_vector3_equal(res_lines[i], output[i]);
		}
	}
}

TYPED_TEST(GeometryTest, cube_test_2d)
{
	const uint32_t indices_tris[]  = {0, 1, 2, 2, 1, 3};
	const uint32_t indices_lines[] = {0, 1, 1, 3, 3, 2, 2, 0};
	if constexpr (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		{
			ror::Vector2<TypeParam>              size{1.0f, 1.0f}, origin{0.0f, 0.0f};
			std::vector<ror::Vector2<TypeParam>> res_points = {{-0.5f, -0.5f},
															   {0.5f, -0.5f},
															   {-0.5f, 0.5f},
															   {0.5f, 0.5f}};

			std::vector<ror::Vector3<TypeParam>> res_tris;
			res_tris.reserve(6);

			for (uint32_t i = 0; i < 2; ++i)
			{
				res_tris.emplace_back(res_points[indices_tris[3 * i]]);
				res_tris.emplace_back(res_points[indices_tris[3 * i + 1]]);
				res_tris.emplace_back(res_points[indices_tris[3 * i + 2]]);
			}

			std::vector<ror::Vector3<TypeParam>> res_lines;
			res_lines.reserve(8);

			for (uint32_t i = 0; i < 4; ++i)
			{
				res_lines.emplace_back(res_points[indices_lines[2 * i]]);
				res_lines.emplace_back(res_points[indices_lines[2 * i + 1]]);
			}

			{
				std::vector<ror::Vector2<TypeParam>> output;
				ror::make_box_triangles(size, origin, output);
				for (size_t i = 0; i < 6; ++i)
				{
					test_vector2_equal(res_tris[i], output[i]);
				}
			}

			{
				std::vector<ror::Vector2<TypeParam>> output;
				// Use overload
				ror::make_box_triangles(static_cast<float32_t>(size.x), origin, output);
				for (size_t i = 0; i < 6; ++i)
				{
					test_vector2_equal(res_tris[i], output[i]);
				}
			}
			{
				std::vector<ror::Vector2<TypeParam>> output;
				// Use overload
				ror::make_box_triangles(static_cast<float32_t>(size.x), output);
				for (size_t i = 0; i < 6; ++i)
				{
					test_vector2_equal(res_tris[i], output[i]);
				}
			}
			{
				// Use overload
				std::vector<ror::Vector2<TypeParam>> output = ror::make_box_triangles<ror::Vector2<TypeParam>>(static_cast<float32_t>(size.x));
				for (size_t i = 0; i < 6; ++i)
				{
					test_vector2_equal(res_tris[i], output[i]);
				}
			}
			{
				ror::Vector2<TypeParam> minimum{-0.5f, -0.5f}, maximum{0.5f, 0.5f};
				// Use min/max overload
				std::vector<ror::Vector2<TypeParam>> output;
				ror::make_box_triangles(output, minimum, maximum);
				for (size_t i = 0; i < 6; ++i)
				{
					test_vector2_equal(res_tris[i], output[i]);
				}
			}
			{
				std::vector<ror::Vector2<TypeParam>> output;
				std::vector<ror::Vector3<TypeParam>> index_buffer;
				// Use overload
				ror::make_box_triangles_indexed(size, origin, output, index_buffer);
				for (size_t i = 0; i < 4; ++i)
				{
					test_vector2_equal(res_points[i], output[i]);
				}
				for (size_t i = 0; i < 2; ++i)
				{
					test_vector2_equal(ror::Vector3<TypeParam>(indices_tris[3 * i], indices_tris[3 * i + 1], indices_tris[3 * i + 2]), index_buffer[i]);
				}
			}
			{
				std::vector<ror::Vector2<TypeParam>> output;
				std::vector<ror::Vector3<TypeParam>> index_buffer;

				// Use overload for unit cube
				ror::make_box_triangles_indexed(output, index_buffer);

				for (size_t i = 0; i < 4; ++i)
				{
					test_vector2_equal(res_points[i], output[i]);
				}

				for (size_t i = 0; i < 2; ++i)
				{
					test_vector2_equal(ror::Vector3<TypeParam>(indices_tris[3 * i], indices_tris[3 * i + 1], indices_tris[3 * i + 2]), index_buffer[i]);
				}
			}

			// Lines version of the boxes
			{
				std::vector<ror::Vector2<TypeParam>> output;
				ror::make_box_lines(size, origin, output);
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector2_equal(res_lines[i], output[i]);
				}
			}

			{
				std::vector<ror::Vector2<TypeParam>> output;
				// Use overload
				ror::make_box_lines(static_cast<float32_t>(size.x), origin, output);
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector2_equal(res_lines[i], output[i]);
				}
			}
			{
				std::vector<ror::Vector2<TypeParam>> output;
				// Use overload
				ror::make_box_lines(static_cast<float32_t>(size.x), output);
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector2_equal(res_lines[i], output[i]);
				}
			}
			{
				// Use overload
				std::vector<ror::Vector2<TypeParam>> output = ror::make_box_lines<ror::Vector2<TypeParam>>(static_cast<float32_t>(size.x));
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector2_equal(res_lines[i], output[i]);
				}
			}
			{
				ror::Vector2<TypeParam> minimum{-0.5f, -0.5f}, maximum{0.5f, 0.5f};
				// Use min/max overload
				std::vector<ror::Vector2<TypeParam>> output;
				ror::make_box_lines(output, minimum, maximum);
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector2_equal(res_lines[i], output[i]);
				}
			}
			{
				std::vector<ror::Vector2<TypeParam>> output;
				std::vector<ror::Vector2<TypeParam>> index_buffer;
				// Use overload
				ror::make_box_lines_indexed(size, origin, output, index_buffer);
				for (size_t i = 0; i < 4; ++i)
				{
					test_vector2_equal(res_points[i], output[i]);
				}
				for (size_t i = 0; i < 4; ++i)
				{
					test_vector2_equal(ror::Vector2<TypeParam>(indices_lines[2 * i], indices_lines[2 * i + 1]), index_buffer[i]);
				}
			}
			{
				std::vector<ror::Vector2<TypeParam>> output;
				std::vector<ror::Vector2<TypeParam>> index_buffer;

				// Use overload for unit cube
				ror::make_box_lines_indexed(output, index_buffer);

				for (size_t i = 0; i < 4; ++i)
				{
					test_vector2_equal(res_points[i], output[i]);
				}

				for (size_t i = 0; i < 4; ++i)
				{
					test_vector2_equal(ror::Vector2<TypeParam>(indices_lines[2 * i], indices_lines[2 * i + 1]), index_buffer[i]);
				}
			}
		}
		{
			ror::Vector2<TypeParam>              size{1.0f, 1.0f}, origin{0.5f, 0.5f};
			std::vector<ror::Vector2<TypeParam>> res_points = {{0.0f, 0.0f},
															   {1.0f, 0.0f},
															   {0.0f, 1.0f},
															   {1.0f, 1.0f}};

			std::vector<ror::Vector3<TypeParam>> res_tris;
			res_tris.reserve(6);

			for (uint32_t i = 0; i < 2; ++i)
			{
				res_tris.emplace_back(res_points[indices_tris[3 * i]]);
				res_tris.emplace_back(res_points[indices_tris[3 * i + 1]]);
				res_tris.emplace_back(res_points[indices_tris[3 * i + 2]]);
			}

			std::vector<ror::Vector3<TypeParam>> res_lines;
			res_lines.reserve(8);

			for (uint32_t i = 0; i < 4; ++i)
			{
				res_lines.emplace_back(res_points[indices_lines[2 * i]]);
				res_lines.emplace_back(res_points[indices_lines[2 * i + 1]]);
			}

			{
				std::vector<ror::Vector2<TypeParam>> output;
				ror::make_box_triangles(size, origin, output);
				for (size_t i = 0; i < 6; ++i)
				{
					test_vector2_equal(res_tris[i], output[i]);
				}
			}
			{
				std::vector<ror::Vector2<TypeParam>> output;
				ror::make_box_lines(size, origin, output);
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector2_equal(res_lines[i], output[i]);
				}
			}
		}
		{
			ror::Vector2<TypeParam>              size{1.0f, 1.0f}, origin{-0.5f, -0.5f};
			std::vector<ror::Vector2<TypeParam>> output;
			std::vector<ror::Vector2<TypeParam>> res_points = {{-1.0f, -1.0f},
															   {0.0f, -1.0f},
															   {-1.0f, 0.0f},
															   {0.0f, 0.0f}};

			std::vector<ror::Vector3<TypeParam>> res_tris;
			res_tris.reserve(6);

			for (uint32_t i = 0; i < 2; ++i)
			{
				res_tris.emplace_back(res_points[indices_tris[3 * i]]);
				res_tris.emplace_back(res_points[indices_tris[3 * i + 1]]);
				res_tris.emplace_back(res_points[indices_tris[3 * i + 2]]);
			}

			std::vector<ror::Vector3<TypeParam>> res_lines;
			res_lines.reserve(8);

			for (uint32_t i = 0; i < 4; ++i)
			{
				res_lines.emplace_back(res_points[indices_lines[2 * i]]);
				res_lines.emplace_back(res_points[indices_lines[2 * i + 1]]);
			}

			{
				ror::make_box_triangles(size, origin, output);
				for (size_t i = 0; i < 6; ++i)
				{
					test_vector2_equal(res_tris[i], output[i]);
				}
			}
			{
				output.clear();
				ror::make_box_lines(size, origin, output);

				for (size_t i = 0; i < 8; ++i)
				{
					test_vector2_equal(res_lines[i], output[i]);
				}
			}
		}
	}

	{
		ror::Vector2<TypeParam>              size{10, 20}, origin{0, 0};
		std::vector<ror::Vector2<TypeParam>> output;
		std::vector<ror::Vector2<TypeParam>> res_points = {{-5, -10}, {5, -10}, {-5, 10}, {5, 10}};

		std::vector<ror::Vector3<TypeParam>> res_tris;
		res_tris.reserve(6);

		for (uint32_t i = 0; i < 2; ++i)
		{
			res_tris.emplace_back(res_points[indices_tris[3 * i]]);
			res_tris.emplace_back(res_points[indices_tris[3 * i + 1]]);
			res_tris.emplace_back(res_points[indices_tris[3 * i + 2]]);
		}

		std::vector<ror::Vector3<TypeParam>> res_lines;
		res_lines.reserve(8);

		for (uint32_t i = 0; i < 4; ++i)
		{
			res_lines.emplace_back(res_points[indices_lines[2 * i]]);
			res_lines.emplace_back(res_points[indices_lines[2 * i + 1]]);
		}

		{
			ror::make_box_triangles(size, origin, output);
			for (size_t i = 0; i < 6; ++i)
			{
				test_vector2_equal(res_tris[i], output[i]);
			}
		}
		{
			output.clear();
			ror::make_box_lines(size, origin, output);
			for (size_t i = 0; i < 8; ++i)
			{
				test_vector2_equal(res_lines[i], output[i]);
			}
		}
	}

	{
		ror::Vector2<TypeParam>              size{10, 20}, origin{5, 15};
		std::vector<ror::Vector2<TypeParam>> output;
		std::vector<ror::Vector2<TypeParam>> res_points = {{0, 5}, {10, 5}, {0, 25}, {10, 25}};

		std::vector<ror::Vector3<TypeParam>> res_tris;
		res_tris.reserve(6);

		for (uint32_t i = 0; i < 2; ++i)
		{
			res_tris.emplace_back(res_points[indices_tris[3 * i]]);
			res_tris.emplace_back(res_points[indices_tris[3 * i + 1]]);
			res_tris.emplace_back(res_points[indices_tris[3 * i + 2]]);
		}

		std::vector<ror::Vector3<TypeParam>> res_lines;
		res_lines.reserve(8);

		for (uint32_t i = 0; i < 4; ++i)
		{
			res_lines.emplace_back(res_points[indices_lines[2 * i]]);
			res_lines.emplace_back(res_points[indices_lines[2 * i + 1]]);
		}

		{
			ror::make_box_triangles(size, origin, output);
			for (size_t i = 0; i < 6; ++i)
			{
				test_vector2_equal(res_tris[i], output[i]);
			}
		}
		{
			output.clear();
			ror::make_box_lines(size, origin, output);
			for (size_t i = 0; i < 8; ++i)
			{
				test_vector2_equal(res_lines[i], output[i]);
			}
		}
	}
	{
		ror::Vector2<TypeParam>              size{10, 20}, origin{10, 20};
		std::vector<ror::Vector2<TypeParam>> output;
		std::vector<ror::Vector2<TypeParam>> res_points = {{5, 10}, {15, 10}, {5, 30}, {15, 30}};

		std::vector<ror::Vector3<TypeParam>> res_tris;
		res_tris.reserve(6);

		for (uint32_t i = 0; i < 2; ++i)
		{
			res_tris.emplace_back(res_points[indices_tris[3 * i]]);
			res_tris.emplace_back(res_points[indices_tris[3 * i + 1]]);
			res_tris.emplace_back(res_points[indices_tris[3 * i + 2]]);
		}

		std::vector<ror::Vector3<TypeParam>> res_lines;
		res_lines.reserve(8);

		for (uint32_t i = 0; i < 4; ++i)
		{
			res_lines.emplace_back(res_points[indices_lines[2 * i]]);
			res_lines.emplace_back(res_points[indices_lines[2 * i + 1]]);
		}

		{
			ror::make_box_triangles(size, origin, output);
			for (size_t i = 0; i < 6; ++i)
			{
				test_vector2_equal(res_tris[i], output[i]);
			}
		}
		{
			output.clear();
			ror::make_box_lines(size, origin, output);
			for (size_t i = 0; i < 8; ++i)
			{
				test_vector2_equal(res_lines[i], output[i]);
			}
		}
	}
}

}        // namespace ror_test
