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
FORCE_INLINE csgnode::csgnode(void)
{
	m_child_node  = new std::vector<csgnode *>;
	m_parent_node = m_left_node = m_right_node = nullptr;
}

FORCE_INLINE void csgnode::attach_parent(csgnode *a_new_parent_node)
{
	// Note: the Parent of a_ParentNode is not changed
	if (a_new_parent_node != nullptr)
	{
		if (this->m_parent_node != a_new_parent_node)        // Sanity Check
		{
			a_new_parent_node->m_child_node->push_back(this);
			// Also Remove the this from its Previous Parent List of Children
			if (this->m_parent_node != nullptr)
			{
				std::vector<csgnode *>::iterator index = std::find(this->m_parent_node->m_child_node->begin(),
				                                                   this->m_parent_node->m_child_node->end(),
				                                                   this);
				if (index != this->m_parent_node->m_child_node->end())
				{
					this->m_parent_node->m_child_node->erase(index);
				}
			}
			this->m_parent_node = a_new_parent_node;
		}
	}
}

FORCE_INLINE void csgnode::attach_child(csgnode *a_new_child_node)
{
	if (a_new_child_node != nullptr)
	{
		a_new_child_node->attach_parent(this);
	}
	/*
		// Note: the Children of a_ChildNode is not changed
		if (a_NewChildNode->m_ParentNode != this) // Sanity Check
		{
			this->m_ChildNode->push_back(a_NewChildNode);
			// Also Remove the a_NewChildNode from its Previous Parent List of Children
			if (a_NewChildNode->m_ParentNode != nullptr)
			{
				std::vector<CSGNode*>::iterator Index = std::find(a_NewChildNode->m_ParentNode->m_ChildNode->begin(),
																a_NewChildNode->m_ParentNode->m_ChildNode->end(),
																a_NewChildNode);
				if (Index != a_NewChildNode->m_ParentNode->m_ChildNode->end())
				{
					a_NewChildNode->m_ParentNode->m_ChildNode->erase(Index);
				}
			}
			a_NewChildNode->m_ParentNode = this;
		}
		*/
}

FORCE_INLINE void csgnode::attach_parent_in_middle(csgnode *a_new_parent_node)
{
	// Note : a_NewPrentNode will lose its parent
	if (a_new_parent_node != nullptr)
	{
		csgnode *this_node_parent = this->m_parent_node;
		this->attach_parent(a_new_parent_node);
		if (this_node_parent != nullptr)
		{
			a_new_parent_node->attach_parent(this_node_parent);
		}
	}
	/*
		if (this->m_ParentNode != a_NewParentNode)
		{
			a_NewParentNode->m_ChildNode->push_back(this);
			// if the Node already has other Parent then Maintain the Tree
			if (this->m_ParentNode)
			{
				a_NewParentNode->m_ParentNode = this->m_ParentNode;
				this->m_ParentNode->AttachChild(a_NewParentNode);//this->m_ParentNode->m_ChildNode = a_ParentNode;
			}
			this->m_ParentNode = a_NewParentNode;
		}
		*/
}

FORCE_INLINE void csgnode::attach_child_in_middle(csgnode *a_new_child_node)
{
	// Note : This will lose its parent
	if (a_new_child_node != nullptr)
	{
		a_new_child_node->attach_parent_in_middle(this);
	}
	/*
		if (a_NewChildNode->m_ParentNode != this)
		{
			a_NewChildNode->m_ParentNode = this;
			// if the Node already has other Children then Maintain the Tree
			if (!this->m_ChildNode->empty())
			{
				a_NewChildNode->m_ChildNode = this->m_ChildNode;
				this->m_ChildNode->m_ParentNode = a_NewChildNode;
			}
			this->m_ChildNode = a_NewChildNode;
		}
		*/
}

FORCE_INLINE void csgnode::insert_after(csgnode *a_new_node)
{
	// List Operations //
	if (a_new_node != nullptr)
	{
		a_new_node->m_left_node = this;
		if (this->m_right_node)
		{
			a_new_node->m_right_node        = this->m_right_node;
			this->m_right_node->m_left_node = a_new_node;
		}
		this->m_right_node = a_new_node;
	}
	// Tree Operations //
	// Nothing Since Tree Operations are handled with Attach* Functions
}

FORCE_INLINE void csgnode::insert_before(csgnode *a_new_node)
{
	// List Operations //
	if (a_new_node != nullptr)
	{
		a_new_node->m_right_node = this;
		if (this->m_left_node)
		{
			a_new_node->m_left_node         = this->m_left_node;
			this->m_left_node->m_right_node = a_new_node;
		}
		this->m_left_node = a_new_node;
	}
	// Tree Operations //
	// Nothing! Since Tree Operations are handled with Attach* Functions
}

FORCE_INLINE bool csgnode::has_parent()
{
	return (m_parent_node != nullptr);
}

FORCE_INLINE bool csgnode::has_child()
{
	return (!m_child_node->empty());
}

FORCE_INLINE csgnode *csgnode::get_child_at(uint32_t a_index)
{
	if (this->has_child())
	{
		assert(a_index < m_child_node->size() && "Child requested is out of bound");
		return (*this->m_child_node)[a_index];
	}
	return nullptr;
}

FORCE_INLINE csgnode *csgnode::get_parent_node() const
{
	return m_parent_node;
}

}        // namespace ror
