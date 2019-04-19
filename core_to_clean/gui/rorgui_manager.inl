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

namespace ror
{
FORCE_INLINE cguimanager::cguimanager(void)
{
	this->m_elements = new elements_vector();
}

FORCE_INLINE cguimanager::~cguimanager(void)
{
	//this->ReleaseAllElements();
	rDELETE(this->m_elements);
}

FORCE_INLINE void cguimanager::render()
{
	static elements_vector::reverse_iterator end;
	static elements_vector::reverse_iterator begin;

	end   = this->m_elements->rend();
	begin = this->m_elements->rbegin();

	for (elements_vector::reverse_iterator index = begin; index < end; ++index)
	{
		if ((*index)->is_visible())
			(*index)->render();
	}
}

FORCE_INLINE bool cguimanager::initialize()
{
	// Here we sort the GUI Elements by Z-Order
	std::sort(this->m_elements->begin(), this->m_elements->end(), ZOrderComparison);

	//		uint32_t ListSize = 0;
	//		ListSize = (uint32_t)this->m_Elements->size();
	//		for(uint32_t i = 0; i < ListSize; i++)
	//		{
	//			//(*this->m_Elements)[i]->Initialize(0);
	//		}

	return true;
}

FORCE_INLINE void cguimanager::check_for_mouse_events(ror::MouseEvent &a_event)
{
	static uint32_t list_size = 0;
	list_size                 = (uint32_t) this->m_elements->size();
	for (uint32_t i = 0; i < list_size; i++)
	{
		if (true == (*this->m_elements)[i]->check_mouse_events(a_event))
			break;
	}
}

FORCE_INLINE void cguimanager::check_for_keyboard_events(ror::KeyboardEvent &a_event)
{}

FORCE_INLINE void cguimanager::check_for_window_events(ror::WindowEvent &a_event)
{
	static elements_vector::reverse_iterator end;
	static elements_vector::reverse_iterator begin;

	end   = this->m_elements->rend();
	begin = this->m_elements->rbegin();

	for (elements_vector::reverse_iterator index = begin; index < end; ++index)
	{
		(*index)->check_window_events(a_event);
	}
}

FORCE_INLINE void cguimanager::set_elements(std::vector<cguielement *> *a_elements)
{
	this->m_elements = a_elements;
}

FORCE_INLINE std::vector<cguielement *> *cguimanager::get_elements() const
{
	return this->m_elements;
}

FORCE_INLINE bool cguimanager::add_element(ror::cguielement *a_element, std::string a_name)
{
	if (nullptr == a_element)
	{
		return false;
	}
	this->m_elements->push_back(a_element);
	a_element->m_id = (uint32_t) this->m_elements->size() - 1;
	this->m_elements_names.insert(std::make_pair(a_name, a_element->m_id));
	return true;
}

FORCE_INLINE cguielement *cguimanager::get_element(uint32_t a_index)
{
	return (a_index >= this->m_elements->size() ? nullptr : (*this->m_elements)[a_index]);
}

FORCE_INLINE int32_t cguimanager::get_element(cguielement *a_element)
{
	if (nullptr == a_element)
		return -1;

	static uint32_t list_size = 0;
	list_size                 = (uint32_t) this->m_elements->size();
	for (uint32_t i = 0; i < list_size; i++)
	{
		if ((*this->m_elements)[i] == a_element)
		{
			return int32_t(i);
		}
	}
	return -1;
}

FORCE_INLINE void cguimanager::release_element(uint32_t a_index)
{
	assert(a_index < (uint32_t) this->m_elements->size() && "Index Out of Bound, FileName : RORGUIManager.inl");
	rDELETE((*this->m_elements)[a_index]);
	this->m_elements->erase(this->m_elements->begin() + a_index);
}

FORCE_INLINE void cguimanager::release_element(cguielement *a_element)
{
	this->release_element(this->get_element(a_element));
}

FORCE_INLINE void cguimanager::release_all_elements()
{
	uint32_t list_size1 = 0;
	list_size1          = (uint32_t) this->m_elements->size();
	for (uint32_t i = 0; i < list_size1; i++)
	{
		rDELETE((*this->m_elements)[i]);
	}
	this->m_elements->clear();
}

}        // namespace ror
