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
#include "rorgui_graphics_element.h"

namespace ror
{
class ROAR_ENGINE_ITEM cguimanager : public ror::Singleton<cguimanager>
{
  public:
	typedef std::vector<ror::cguielement *>          elements_vector;
	typedef std::unordered_map<std::string, int32_t> elements_names_map;

	FORCE_INLINE elements_vector *get_elements() const;

	FORCE_INLINE bool add_element(cguielement *a_element, std::string a_name);
	FORCE_INLINE cguielement *get_element(uint32_t a_index);
	FORCE_INLINE int32_t get_element(cguielement *a_element);

	FORCE_INLINE void release_element(uint32_t a_index);
	FORCE_INLINE void release_element(cguielement *a_element);
	FORCE_INLINE void release_all_elements();

	FORCE_INLINE void render();
	FORCE_INLINE bool initialize();
	FORCE_INLINE void check_for_mouse_events(ror::MouseEvent &a_event);
	FORCE_INLINE void check_for_keyboard_events(ror::KeyboardEvent &a_event);
	FORCE_INLINE void check_for_window_events(ror::WindowEvent &a_event);

	static bool ZOrderComparison(const cguielement *a_1, const cguielement *a_2);

  protected:
	FORCE_INLINE void set_elements(elements_vector *a_elements);
	// Make CSingleton<CGUIManager> as friend
	friend class Singleton<cguimanager>;

	FORCE_INLINE cguimanager(void);
	FORCE_INLINE ~cguimanager(void);

	elements_vector *m_elements;        // Elements in the Scene, CGUIManager Takes Ownership of all the Elements

  private:
	//static uint32_t				 m_NextElementId;	// ID of the Elements
	static elements_names_map m_elements_names;        // Names of the Elements which are used to generate IDs
};
}        // namespace ror

#include "rorgui_manager.hh"
