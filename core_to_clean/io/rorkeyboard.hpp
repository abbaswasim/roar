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

#include "foundation/rorsingleton.h"
#include "interfaces/roriinput_device.h"

namespace ror
{
// SingleTon Class of Keyboard, only one keyboard can be used a time
class ROAR_ENGINE_ITEM Keyboard : public Singleton<Keyboard>, public IInputDevice
{
  public:
	// Base Class Functions
	FORCE_INLINE void initialize();
	FORCE_INLINE bool update();
	FORCE_INLINE void shutdown();

	// Keyboard functions
	FORCE_INLINE bool is_key_press(rUsChar a_key);        // Not Implemented Yet
	FORCE_INLINE bool is_key_down(rUsChar a_key);
	FORCE_INLINE bool is_key_up(rUsChar a_key);

	FORCE_INLINE void setkey_press(rUsChar a_key);        // Not Implemented Yet
	FORCE_INLINE void setkey_down(rUsChar a_key);
	FORCE_INLINE void setkey_up(rUsChar a_key);

  protected:
  private:
	FORCE_INLINE Keyboard();
	~Keyboard();

	// Make CSingleton<CKeyboard> a friend as well
	friend class Singleton<Keyboard>;

	// Keyboard data
	bool m_keys[256];            // Keys Status
	bool m_use_key_board;        // Keyboard Flag
};

}        // namespace ror
#include "rorkeyboard.hh"
