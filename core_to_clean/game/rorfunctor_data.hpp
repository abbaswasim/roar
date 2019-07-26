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

#include "gui/rorgui_element.h"

namespace ror
{
class ROAR_ENGINE_ITEM FunctorData
{
  public:
	FORCE_INLINE FunctorData();
	FORCE_INLINE FunctorData(cguielement *a_pointer, std::string a_name, std::string a_class_id, std::string a_function);
	FORCE_INLINE ~FunctorData();

	ror::cguielement *m_pointer;         // Pointer to the Element
	std::string       m_name;            // Name of the Functor
	std::string       m_class_id;        // Class ID
	std::string       m_function;        // Function to Bound to
};

typedef std::vector<FunctorData> functor_vector;
}        // namespace ror

#include "rorfunctor_data.hh"
