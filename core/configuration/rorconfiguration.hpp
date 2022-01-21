// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2020
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

#include "foundation/rorcrtp.hpp"
#include "nlohmann/json.hpp"
#include "resources/rorresource.hpp"
#include <foundation/rortypes.hpp>

// for convenience
using json = nlohmann::json;

namespace ror
{
template <typename _type>
class ROAR_ENGINE_ITEM Configuration : public Crtp<_type, Configuration>
{
  public:
	FORCE_INLINE                Configuration()                                 = default;        //! Default constructor
	FORCE_INLINE                Configuration(const Configuration &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                Configuration(Configuration &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE Configuration &operator=(const Configuration &a_other) = default;                //! Copy assignment operator
	FORCE_INLINE Configuration &operator=(Configuration &&a_other) noexcept = default;            //! Move assignment operator
	FORCE_INLINE virtual ~Configuration() noexcept override                 = default;            //! Destructor

	void load(std::filesystem::path a_config_path)
	{
		const auto& resource    = load_resource(a_config_path, ResourceSemantic::configs);
		this->m_json_file = json::parse(resource.data());

		this->underlying().load_specific();
	}

	// template<typename _type_inner>
	// bool contains(_type_inner &&a_key) const
	// {
	//	return this->m_json_file.contains(std::forward(a_key));
	// }

	// template <typename _type_inner>
	// auto integer(_type_inner &&a_key) const
	// {
	//	return this->m_json_file.integer(std::forward(a_key));
	// }

  protected:
  private:
	json m_json_file{};
	friend _type;        //! Any type derived from Configuration will have access to constructor and private members
};
}        // namespace ror

#include "rorconfiguration.hh"
