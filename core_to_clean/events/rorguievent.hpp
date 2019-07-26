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

#include "memory_manager/rormemory_check.h"

namespace ror
{
class ROAR_ENGINE_ITEM cguielement;
class ROAR_ENGINE_ITEM cguievent
{
  public:
	FORCE_INLINE cguievent(void);
	FORCE_INLINE cguievent(cguielement *a_guielement);
	FORCE_INLINE cguievent(const cguievent &a_other);
	FORCE_INLINE ~cguievent(void);

	FORCE_INLINE cguievent &operator=(const cguievent &a_other);
	FORCE_INLINE cguielement *get_source() const;
	FORCE_INLINE void         set_source(cguielement *a_source);

  protected:
  private:
	ror::cguielement *m_source;        // Source who triggered event, CGUIEvent Does not take ownership
	                                   //CoreEngine::IFunctor			*m_CallBack;	// Callback information
};
}        // namespace ror

#include "rorguievent.hh"
