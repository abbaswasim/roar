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
	ror::cube(size, origin , output);
#endif

	if constexpr (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		{
			const uint32_t                       indices[] = {0, 1, 2, 2, 1, 3, 4, 5, 0, 0, 5, 1, 0, 1, 6, 6, 1, 7, 4, 0, 6, 6, 0, 2, 0, 5, 3, 3, 5, 7, 5, 4, 7, 7, 4, 6};
			ror::Vector3<TypeParam>              size{1.0f, 1.0f, 1.0f}, origin{0.0f, 0.0f, 0.0f};
			std::vector<ror::Vector3<TypeParam>> res = {{-0.5f, -0.5f, -0.5f},
														{0.5f, -0.5f, -0.5f},
														{-0.5f, 0.5f, -0.5f},
														{0.5f, 0.5f, -0.5f},
														{-0.5f, -0.5f, 0.5f},
														{0.5f, -0.5f, 0.5f},
														{-0.5f, 0.5f, 0.5f},
														{0.5f, 0.5f, 0.5f}};

			{
				std::vector<ror::Vector3<TypeParam>> output;
				ror::cube(size, origin, output);
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector3_equal(res[i], output[i]);
				}
			}

			{
				std::vector<ror::Vector3<TypeParam>> output;
				// Use overload
				ror::cube(static_cast<float32_t>(size.x), origin, output);
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector3_equal(res[i], output[i]);
				}
			}
			{
				std::vector<ror::Vector3<TypeParam>> output;
				// Use overload
				ror::cube(static_cast<float32_t>(size.x), output);
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector3_equal(res[i], output[i]);
				}
			}
			{
				// Use overload
				std::vector<ror::Vector3<TypeParam>> output = ror::cube<ror::Vector3<TypeParam>>(static_cast<float32_t>(size.x));
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector3_equal(res[i], output[i]);
				}
			}
			{
				ror::Vector3<TypeParam> minimum{-0.5f, -0.5f, -0.5f}, maximum{0.5f, 0.5f, 0.5f};
				// Use min/max overload
				std::vector<ror::Vector3<TypeParam>> output;
				ror::cube(output, minimum, maximum);
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector3_equal(res[i], output[i]);
				}
			}
			{
				std::vector<ror::Vector3<TypeParam>> output;
				std::vector<ror::Vector3<TypeParam>> index_buffer;
				// Use overload
				ror::cube(size, origin, output, index_buffer);
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector3_equal(res[i], output[i]);
					test_vector3_equal(ror::Vector3<TypeParam>(indices[3 * i], indices[3 * i + 1], indices[3 * i + 2]), index_buffer[i]);
				}
			}
			{
				std::vector<ror::Vector3<TypeParam>> output;
				std::vector<ror::Vector3<TypeParam>> index_buffer;

				// Use overload for unit cube
				ror::cube(output, index_buffer);

				for (size_t i = 0; i < 8; ++i)
				{
					test_vector3_equal(res[i], output[i]);
					test_vector3_equal(ror::Vector3<TypeParam>(indices[3 * i], indices[3 * i + 1], indices[3 * i + 2]), index_buffer[i]);
				}
			}
		}
		{
			ror::Vector3<TypeParam>              size{1.0f, 1.0f, 1.0f}, origin{0.5f, 0.5f, 0.5f};
			std::vector<ror::Vector3<TypeParam>> res = {{0.0f, 0.0f, 0.0f},
														{1.0f, 0.0f, 0.0f},
														{0.0f, 1.0f, 0.0f},
														{1.0f, 1.0f, 0.0f},
														{0.0f, 0.0f, 1.0f},
														{1.0f, 0.0f, 1.0f},
														{0.0f, 1.0f, 1.0f},
														{1.0f, 1.0f, 1.0f}};

			{
				std::vector<ror::Vector3<TypeParam>> output;
				ror::cube(size, origin, output);
				for (size_t i = 0; i < 8; ++i)
				{
					test_vector3_equal(res[i], output[i]);
				}
			}
		}
		{
			ror::Vector3<TypeParam>              size{1.0f, 1.0f, 1.0f}, origin{-0.5f, -0.5f, -0.5f};
			std::vector<ror::Vector3<TypeParam>> output;
			std::vector<ror::Vector3<TypeParam>> res = {{-1.0f, -1.0f, -1.0f},
														{0.0f, -1.0f, -1.0f},
														{-1.0f, 0.0f, -1.0f},
														{0.0f, 0.0f, -1.0f},
														{-1.0f, -1.0f, 0.0f},
														{0.0f, -1.0f, 0.0f},
														{-1.0f, 0.0f, 0.0f},
														{0.0f, 0.0f, 0.0f}};

			ror::cube(size, origin, output);
			for (size_t i = 0; i < 8; ++i)
			{
				test_vector3_equal(res[i], output[i]);
			}
		}
	}

	{
		ror::Vector3<TypeParam>              size{10, 20, 30}, origin{0, 0, 0};
		std::vector<ror::Vector3<TypeParam>> output;
		std::vector<ror::Vector3<TypeParam>> res = {{-5, -10, -15}, {5, -10, -15}, {-5, 10, -15}, {5, 10, -15}, {-5, -10, 15}, {5, -10, 15}, {-5, 10, 15}, {5, 10, 15}};

		ror::cube(size, origin, output);
		for (size_t i = 0; i < 8; ++i)
		{
			test_vector3_equal(res[i], output[i]);
		}
	}

	{
		ror::Vector3<TypeParam>              size{10, 20, 30}, origin{5, 15, 25};
		std::vector<ror::Vector3<TypeParam>> output;
		std::vector<ror::Vector3<TypeParam>> res = {{0, 5, 10}, {10, 5, 10}, {0, 25, 10}, {10, 25, 10}, {0, 5, 40}, {10, 5, 40}, {0, 25, 40}, {10, 25, 40}};

		ror::cube(size, origin, output);
		for (size_t i = 0; i < 8; ++i)
		{
			test_vector3_equal(res[i], output[i]);
		}
	}

	{
		ror::Vector3<TypeParam>              size{10, 20, 30}, origin{10, 20, 30};
		std::vector<ror::Vector3<TypeParam>> output;
		std::vector<ror::Vector3<TypeParam>> res = {{5, 10, 15}, {15, 10, 15}, {5, 30, 15}, {15, 30, 15}, {5, 10, 45}, {15, 10, 45}, {5, 30, 45}, {15, 30, 45}};

		ror::cube(size, origin, output);
		for (size_t i = 0; i < 8; ++i)
		{
			test_vector3_equal(res[i], output[i]);
		}
	}
}

TYPED_TEST(GeometryTest, cube_test_2d)
{
	if constexpr (std::is_same<TypeParam, float32_t>::value || std::is_same<TypeParam, double64_t>::value)
	{
		{
			ror::Vector2<TypeParam>              size{1.0f, 1.0f}, origin{0.0f, 0.0f};
			std::vector<ror::Vector2<TypeParam>> res = {{-0.5f, -0.5f},
														{0.5f, -0.5f},
														{-0.5f, 0.5f},
														{0.5f, 0.5f}};

			{
				std::vector<ror::Vector2<TypeParam>> output;
				ror::cube(size, origin, output);
				for (size_t i = 0; i < 4; ++i)
				{
					test_vector2_equal(res[i], output[i]);
				}
			}

			{
				std::vector<ror::Vector2<TypeParam>> output;
				// Use overload
				ror::cube(static_cast<float32_t>(size.x), origin, output);
				for (size_t i = 0; i < 4; ++i)
				{
					test_vector2_equal(res[i], output[i]);
				}
			}
			{
				std::vector<ror::Vector2<TypeParam>> output;
				// Use overload
				ror::cube(static_cast<float32_t>(size.x), output);
				for (size_t i = 0; i < 4; ++i)
				{
					test_vector2_equal(res[i], output[i]);
				}
			}
			{
				// Use overload
				std::vector<ror::Vector2<TypeParam>> output = ror::cube<ror::Vector2<TypeParam>>(static_cast<float32_t>(size.x));
				for (size_t i = 0; i < 4; ++i)
				{
					test_vector2_equal(res[i], output[i]);
				}
			}
		}
		{
			ror::Vector2<TypeParam>              size{1.0f, 1.0f}, origin{0.5f, 0.5f};
			std::vector<ror::Vector2<TypeParam>> res = {{0.0f, 0.0f},
														{1.0f, 0.0f},
														{0.0f, 1.0f},
														{1.0f, 1.0f}};

			{
				std::vector<ror::Vector2<TypeParam>> output;
				ror::cube(size, origin, output);
				for (size_t i = 0; i < 4; ++i)
				{
					test_vector2_equal(res[i], output[i]);
				}
			}
		}
		{
			ror::Vector2<TypeParam>              size{1.0f, 1.0f}, origin{-0.5f, -0.5f};
			std::vector<ror::Vector2<TypeParam>> output;
			std::vector<ror::Vector2<TypeParam>> res = {{-1.0f, -1.0f},
														{0.0f, -1.0f},
														{-1.0f, 0.0f},
														{0.0f, 0.0f}};

			ror::cube(size, origin, output);
			for (size_t i = 0; i < 4; ++i)
			{
				test_vector2_equal(res[i], output[i]);
			}
		}
	}

	{
		ror::Vector2<TypeParam>              size{10, 20}, origin{0, 0};
		std::vector<ror::Vector2<TypeParam>> output;
		std::vector<ror::Vector2<TypeParam>> res = {{-5, -10}, {5, -10}, {-5, 10}, {5, 10}};

		ror::cube(size, origin, output);
		for (size_t i = 0; i < 4; ++i)
		{
			test_vector2_equal(res[i], output[i]);
		}
	}

	{
		ror::Vector2<TypeParam>              size{10, 20}, origin{5, 15};
		std::vector<ror::Vector2<TypeParam>> output;
		std::vector<ror::Vector2<TypeParam>> res = {{0, 5}, {10, 5}, {0, 25}, {10, 25}};

		ror::cube(size, origin, output);
		for (size_t i = 0; i < 4; ++i)
		{
			test_vector2_equal(res[i], output[i]);
		}
	}

	{
		ror::Vector2<TypeParam>              size{10, 20}, origin{10, 20};
		std::vector<ror::Vector2<TypeParam>> output;
		std::vector<ror::Vector2<TypeParam>> res = {{5, 10}, {15, 10}, {5, 30}, {15, 30}};

		ror::cube(size, origin, output);
		for (size_t i = 0; i < 4; ++i)
		{
			test_vector2_equal(res[i], output[i]);
		}
	}
}

}        // namespace ror_test
