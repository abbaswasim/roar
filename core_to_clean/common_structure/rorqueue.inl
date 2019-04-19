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
template <class _type>
FORCE_INLINE Queue<_type>::Queue(void)
{
	//m_Queue = new std::deque<_Type>;
}

template <class _type>
FORCE_INLINE Queue<_type>::~Queue(void)
{
	rDELETE(m_queue);
}

template <class _type>
FORCE_INLINE _type Queue<_type>::front(void)
{
	// Warning : Client must check for Nulls
	return *(m_queue->begin());
}

template <class _type>
FORCE_INLINE _type Queue<_type>::deque(void)
{
	if (m_queue->empty())
	{
		// Warning : Client must check for Nulls
		return *(m_queue->begin());
	}
	_type front_element = *(m_queue->begin());
	m_queue->pop_front();
	return front_element;
}

template <class _type>
FORCE_INLINE void Queue<_type>::enque(_type a_value)
{
	m_queue->push_back(a_value);
}

template <class _type>
FORCE_INLINE bool Queue<_type>::empty(void)
{
	return m_queue->empty();
}

}        // namespace ror
