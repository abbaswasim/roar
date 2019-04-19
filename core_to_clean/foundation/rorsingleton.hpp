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

#include "foundation/rormacros.h"

namespace ror
{
// Template Singleton Class Interfaces all Singletons in Roar
// Don't Forget to put Singleton <_Type> as Friend in derived Classes
template <class _type>
class ROAR_ENGINE_ITEM Singleton
{
  public:
	static _type *get_instance()
	{
		static _type instance;
		return &instance;
	}

  protected:
	// Protected to prevent accidental creation
	Singleton(void)
	{}
	virtual ~Singleton(void)
	{}

  private:
	// Not defined, to prevent copy, move and assignments
	Singleton(const Singleton &) = delete;
	Singleton(const _type &&)    = delete;
	Singleton &operator=(const Singleton &other) = delete;
	Singleton &operator=(const Singleton &&other) = delete;
};

}        // namespace ror
