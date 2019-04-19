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

#include "bounds/rorbounding_box.h"
#include "memory_manager/rormemory_check.h"

namespace ror
{
template <class _type>
class ROAR_ENGINE_ITEM Octree
{
  public:
	class OctreeNode
	{
	  public:
		enum EOctreeNodeID
		{
			rTOP_FRONT_LEFT,
			rTOP_FRONT_RIGHT,
			rTOP_BACK_LEFT,
			rTOP_BACK_RIGHT,
			rBOTTOM_FRONT_LEFT,
			rBOTTOM_FRONT_RIGHT,
			rBOTTOM_BACK_LEFT,
			rBOTTOM_BACK_RIGHT
		};

		OctreeNode()
		{}
		~OctreeNode()
		{}

	  private:
		EOctreeNodeID m_node_id;                  // Node Position ID
		OctreeNode *  m_parent_node;              // Pointer to Parent
		OctreeNode *  m_children_nodes[8];        // Pointers to children
		Vector3f      m_center_point;             // Center of the Node
		bool          m_is_leaf;                  // Only true for Leaf Nodes
		BoundingBox   m_bound;                    // Bounding Box of the Node
	};

	FORCE_INLINE Octree(void);
	~Octree(void);

	FORCE_INLINE void initialize();

  private:
	OctreeNode *m_root_node;
	bool        m_enabled;
};
}        // namespace ror

#include "roroctree.inl"
