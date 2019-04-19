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

#include "foundation/rorfont_manager.h"
#include "interfaces/rorievent_listener.h"
#include "interfaces/rorifunctor.h"
#include "interfaces/roriguievent_listener.h"
#include "io/rorkeyboard.h"
#include "io/rormouse.h"
#include "math/rorvector4.h"
#include "profiling/rorlog.h"

namespace ror
{
class ROAR_ENGINE_ITEM cguimanager;
class ROAR_ENGINE_ITEM cguielement
{
  public:
	enum EAlignment
	{
		rLEFT,
		rRIGHT,
		rCENTER,
		rTOP_LEFT,
		rTOP_RIGHT,
		rTOP_CENTER,
		rBOTTOM_LEFT,
		rBOTTOM_RIGHT,
		rBOTTOM_CENTER,
		rLEFT_CENTER,
		rRIGHT_CENTER,
		rALIGNMENT_MAX
	};

	enum EElementType
	{
		rNONE,
		rPANEL,
		rBUTTON,
		rLABEL,
		rMENU,
		rTAB,
		rTEXTBOX,
		rCONTAINER,
		rCOMBOBOX,
		rSLIDER,
		rELEMENT_TYPE_MAX
	};

	FORCE_INLINE cguielement(void);
	FORCE_INLINE cguielement(cguielement *a_parent, Vector3i &a_position, Vector4f *a_background, Vector3f *a_foreground, Vector2i &a_bounds, IFunctor *a_call_back, EElementType a_element_type, int32_t a_font_index, EAlignment a_alighment = rCENTER);
	virtual ~cguielement(void);

	FORCE_INLINE bool operator()(cguielement *a_one, cguielement *a_two);
	FORCE_INLINE int32_t get_id() const;
	FORCE_INLINE void    set_id(int32_t a_id);
	FORCE_INLINE cguielement *get_parent() const;
	FORCE_INLINE void         set_parent(cguielement *a_parent);
	FORCE_INLINE Vector3i get_position() const;
	FORCE_INLINE Vector3i get_own_position() const;
	FORCE_INLINE void     set_position(Vector3i &a_position);
	FORCE_INLINE Vector3i get_absolute_position() const;
	FORCE_INLINE void     set_absolute_position(ror::Vector3i &a_absolute_position);
	FORCE_INLINE Vector4i get_non_clien_area() const;
	FORCE_INLINE void     set_non_clien_area(Vector4i &a_ncareas);
	FORCE_INLINE Vector2i get_bounds() const;
	FORCE_INLINE void     set_bounds(Vector2i &a_bounds);
	FORCE_INLINE Vector2i get_text_position() const;
	FORCE_INLINE void     set_text_position(Vector2i &a_position);
	FORCE_INLINE std::string get_text() const;
	FORCE_INLINE void        set_text(std::string &a_text);
	FORCE_INLINE IFunctor *get_call_back() const;
	FORCE_INLINE void      set_call_back(IFunctor *a_callback);
	FORCE_INLINE bool      get_dirty() const;
	FORCE_INLINE void      set_dirty(bool a_dirty);
	FORCE_INLINE bool      is_visible() const;
	FORCE_INLINE bool      get_draw_caption() const;
	FORCE_INLINE void      set_draw_caption(bool a_draw_caption);
	FORCE_INLINE bool      get_visible() const;
	FORCE_INLINE void      set_visible(bool a_visible);
	FORCE_INLINE void      hide_me();
	FORCE_INLINE int32_t get_zorder() const;
	FORCE_INLINE void    set_zorder(int32_t a_zorder);
	FORCE_INLINE EElementType get_element_type() const;
	FORCE_INLINE void         set_element_type(EElementType a_element_type);
	FORCE_INLINE Vector4f *get_background() const;
	FORCE_INLINE void      set_background(Vector4f *a_background);
	FORCE_INLINE Vector3f *get_foreground() const;
	FORCE_INLINE void      set_foreground(Vector3f *a_foreground);
	FORCE_INLINE int32_t get_sequence_in_parent() const;
	FORCE_INLINE void    set_sequence_in_parent(int32_t a_sequence_in_parent);
	FORCE_INLINE void    increment_sequence_in_parent(cguielement *a_child);
	FORCE_INLINE bool    get_pressed() const;
	FORCE_INLINE void    set_pressed(bool a_pressed);
	FORCE_INLINE int32_t get_font_index() const;
	FORCE_INLINE void    set_font_index(int32_t a_font_index);

	virtual bool initialize(uint32_t a_turn)
	{
		return true;
	};
	virtual void                render(){};
	virtual iguievent_listener *get_event_listener();
	virtual bool                check_mouse_events(MouseEvent &a_event);
	virtual bool                check_keyboard_events(KeyboardEvent &a_event);
	virtual bool                check_window_events(WindowEvent &a_event);

  protected:
	friend class ROAR_ENGINE_ITEM cguimanager;

	int32_t        m_id;                        // Unique Id Of the Element, this is not the Index of the element in the List
	cguielement *  m_parent;                    // Parent of the Element
	ror::Vector4i  m_non_clien_area;            // The Border of the Element, Left, Right, Top, Bottom
	ror::Vector3i  m_position;                  // The Position of the Element with respect to parent
	ror::Vector3i  m_absolute_position;         // The Position of the Element on Screen
	ror::Vector4f *m_background;                // The Background color of the element
	ror::Vector3f *m_foreground;                // The Foreground color of the element
	ror::Vector2i  m_bounds;                    // The Size of the Element (x = Width, y = Height)
	ror::Vector2i  m_text_position;             // The Position of the text where it will be drawn
	std::string    m_text;                      // The String Caption of the Element
	ror::IFunctor *m_call_back;                 // The call back for the Element
	bool           m_pressed;                   // If The Mouse is Pressed But Not released Yet
	bool           m_dirty;                     // Should be updated or not?
	bool           m_draw_caption;              // Should be the Caption be Shown?
	bool           m_visible;                   // Should be Shown or not?
	int32_t        m_font_index;                // Font Index from the List of Loaded Fonts
	int32_t        m_sequence_in_parent;        // Sequence in the elements relative to parent
	EElementType   m_element_type;              // What type of GUI element it is
	EAlignment     m_alignment;                 // What is the Alignment of the element

	static ror::Mouse *   m_mouse_device;           // Mouse Device
	static ror::Keyboard *m_keyboard_device;        // Keyboard Device
};
}        // namespace ror

#include "rorgui_element.inl"
