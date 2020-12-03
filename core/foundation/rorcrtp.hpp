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

#include "rormacros.hpp"

namespace ror
{
/**
 * Helper class for all crtp types
 * A crtp class has two main functionalities, adding functionality and static interfaces
 * Borrowed from https://www.fluentcpp.com/2017/05/19/crtp-helper/
 */
template <typename _type, template <typename> class _derived_type>
class ROAR_ENGINE_ITEM Crtp
{
  public:
	FORCE_INLINE Crtp(const Crtp &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Crtp(Crtp &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Crtp &operator=(const Crtp &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Crtp &operator=(Crtp &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~Crtp() noexcept                 = default;        //! Destructor

	// clang-format off
	_type       &underlying()       { return static_cast<_type &>(*this);       }
	_type const &underlying() const { return static_cast<_type const &>(*this); }
	// clang-format on

  private:
	FORCE_INLINE Crtp() = default;        //! Default constructor
	friend _derived_type<_type>;          //! Any type derives from Crtp will have access to constructor
};

/**
* An example client for Crtp usage
* This one is an interface, you still need to create an implementation
*/

template <class _type>
class ROAR_ENGINE_ITEM CrtpClient : public Crtp<_type, CrtpClient>
{
  public:
	FORCE_INLINE CrtpClient()                              = default;                   //! Default constructor
	FORCE_INLINE CrtpClient(const CrtpClient &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE CrtpClient(CrtpClient &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE CrtpClient &operator=(const CrtpClient &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE CrtpClient &operator=(CrtpClient &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~CrtpClient() noexcept                       = default;        //! Destructor

	void common_func()
	{
		this->underlying().common_func_provider();
	}

  protected:
  private:
};

/**
* And here is an implementation of the CrtpClient interface
*/
class ROAR_ENGINE_ITEM CrtpClientImpl : public CrtpClient<CrtpClientImpl>
{
  public:
	FORCE_INLINE CrtpClientImpl()                                  = default;                   //! Default constructor
	FORCE_INLINE CrtpClientImpl(const CrtpClientImpl &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE CrtpClientImpl(CrtpClientImpl &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE CrtpClientImpl &operator=(const CrtpClientImpl &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE CrtpClientImpl &operator=(CrtpClientImpl &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~CrtpClientImpl() noexcept                           = default;        //! Destructor

	void common_func_provider()
	{}

  protected:
  private:
};

/**
* Here is a usecase of CrtpClient with all its implementations
template<typename _type>
void print(CrtpClient<_type> const& clients)
{
	clients.common_func();
}

CrtpClientImpl1 impl1{};
CrtpClientImpl2 impl2{};

print(impl1);
print(impl2);
*/

/**
 * Here is a mixin helper, the main difference to crtp is its deriving from T
 * So the base class is provided as template. In Crtp case derived class is provided as template
 * Full example http://www.thinkbottomup.com.au/site/blog/C%20%20_Mixins_-_Reuse_through_inheritance_is_good
 */
template <class _type>
class Mixin : public _type
{};

}        // namespace ror
