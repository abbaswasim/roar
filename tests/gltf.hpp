// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2022
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

#include "graphics/rormodel.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/rorbuffers_pack.hpp"
#include <functional>
#include <gtest/gtest.h>
#include <memory>

namespace ror_test
{
class GLTFTest : public testing::Test
{
  public:
	GLTFTest()                                       = default;        //! Default constructor
	GLTFTest(const GLTFTest &a_other)                = delete;         //! Copy constructor
	GLTFTest(GLTFTest &&a_other) noexcept            = delete;         //! Move constructor
	GLTFTest &operator=(const GLTFTest &a_other)     = delete;         //! Copy assignment operator
	GLTFTest &operator=(GLTFTest &&a_other) noexcept = delete;         //! Move assignment operator
	virtual ~GLTFTest() noexcept override            = default;        //! Destructor

	static rhi::BuffersPack           *bp;        //! Non-owning raw pointer to buffers pack
	static std::shared_ptr<ror::Model> fox_model;

	std::unordered_map<hash_64_t, uint32_t> hashes_of_vs{};
	std::unordered_map<hash_64_t, uint32_t> hashes_of_fs{};
	std::unordered_map<hash_64_t, uint32_t> hashes_of_ps{};
	std::unordered_map<hash_64_t, uint32_t> hashes_of_mats{};
	std::unordered_map<hash_64_t, uint32_t> hashes_of_meshs{};
	std::unordered_map<hash_64_t, uint32_t> hashes_of_prims{};

	std::unordered_map<hash_64_t, std::string> unique_vs{};
	std::unordered_map<hash_64_t, std::string> unique_fs{};
	std::unordered_map<hash_64_t, std::string> unique_ps{};

	std::unordered_map<hash_64_t, uint32_t> unique_mesh_vs{};
	std::unordered_map<hash_64_t, uint32_t> unique_mesh_fs{};
	std::unordered_map<hash_64_t, uint32_t> unique_mesh_ps{};

	std::unordered_map<hash_64_t, std::pair<size_t, size_t>> unique_vattrib{};

	void load_model(std::string path);
	void print_hashes();
	void write_shaders();

  protected:
	static void SetUpTestSuite()
	{
		ror::log_critical("Calling constructor of the thing");
		bp        = &rhi::get_buffers_pack();
		fox_model = std::make_shared<ror::Model>();
	}

	static void TearDownTestSuite()
	{
		ror::log_critical("Calling destructor of the thing");
		bp->free();
	}

	virtual void SetUp() override;
	virtual void TearDown() override;

  private:
};

}        // namespace ror_test
