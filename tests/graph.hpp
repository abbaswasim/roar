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

#include "common_structure/rorgraph.hpp"
#include <gtest/gtest.h>
#include <string>

namespace ror_test
{
class GraphTest : public testing::Test
{
  public:
	GraphTest()                             = default;                   //! Default constructor
	GraphTest(const GraphTest &a_other)     = delete;                    //! Copy constructor
	GraphTest(GraphTest &&a_other) noexcept = delete;                    //! Move constructor
	GraphTest &operator=(const GraphTest &a_other) = delete;             //! Copy assignment operator
	GraphTest &operator=(GraphTest &&a_other) noexcept = delete;         //! Move assignment operator
	virtual ~GraphTest() noexcept                      = default;        //! Destructor

	ror::Graph<std::string, false> *get_graph1();
	ror::Graph<std::string, false> *get_graph2();

  protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

  private:
	ror::Graph<std::string, false> m_graph1;
	ror::Graph<std::string, false> m_graph2;
};

class GraphTSTest : public testing::Test
{
  public:
	GraphTSTest()                               = default;                   //! Default constructor
	GraphTSTest(const GraphTSTest &a_other)     = delete;                    //! Copy constructor
	GraphTSTest(GraphTSTest &&a_other) noexcept = delete;                    //! Move constructor
	GraphTSTest &operator=(const GraphTSTest &a_other) = delete;             //! Copy assignment operator
	GraphTSTest &operator=(GraphTSTest &&a_other) noexcept = delete;         //! Move assignment operator
	virtual ~GraphTSTest() noexcept                        = default;        //! Destructor

	ror::Graph<std::string, true> *get_graph1();
	ror::Graph<std::string, true> *get_graph2();

  protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

  private:
	ror::Graph<std::string, true> m_graph1;
	ror::Graph<std::string, true> m_graph2;
};

}        // namespace ror_test
