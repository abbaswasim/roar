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

#include "bounds/rorbounding_sphere.h"
#include "foundation/rorobject_information.h"

namespace ror
{
// Doubly Linked List Type Tree which give both		//
// flavors of Tree traversal and Doubly Linked List	//

class ROAR_ENGINE_ITEM csgnode : public ObjectInformation
{
  public:
	// Default Constructor leaves the Node nondeterminate
	FORCE_INLINE csgnode(void);
	virtual ~csgnode(void);

	FORCE_INLINE bool has_parent();
	FORCE_INLINE bool has_child();
	FORCE_INLINE csgnode *get_child_at(uint32_t a_index);
	FORCE_INLINE csgnode *get_parent_node() const;
	// Tree Functions (Scene Graph Building Functions)
	// Makes a_ParentNode the Parent of This Node
	FORCE_INLINE void attach_parent(csgnode *a_parent_node);
	// Makes a_ChildNode the Child of This Node
	FORCE_INLINE void attach_child(csgnode *a_child_node);

	// Makes a_ParentNode the Parent of This Node and maintain Tree
	FORCE_INLINE void attach_parent_in_middle(csgnode *a_parent_node);
	// Makes a_ChildNode the Child of This Node and maintain Tree
	FORCE_INLINE void attach_child_in_middle(csgnode *a_child_node);

  protected:
	csgnode *               m_parent_node;        // A node can only have one parent
	std::vector<csgnode *> *m_child_node;         // A Node can have more then one children
	csgnode *               m_left_node;          // Previous in the list // Do i really need this ???
	csgnode *               m_right_node;         // Next in the list		// Do i really need this ???

  private:
	// Linked List functions (Scene Graph Creating Functions)
	// Makes a_NewNode the Next of This Node
	FORCE_INLINE void insert_after(csgnode *a_new_node);
	// Makes a_NewNode the Previous of This Node
	FORCE_INLINE void insert_before(csgnode *a_new_node);
};
}        // namespace ror

#include "rorsg_node.hh"
