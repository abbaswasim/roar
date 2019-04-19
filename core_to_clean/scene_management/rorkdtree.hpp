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

// #define MAX_DIM  20             //max space dimension
// #define KD_MAX_POINTS 2000000    //max KD tree points number
// #define SD 10					//current space dimension
// #define RMAX 1000
// #define POINTS_NUM 10000
// #define TEST_NUM 50
//
// #define KDTEMPLATE	template
// #define KDNODE		KDNode
// #define KDTREE		KDTree

//
// namespace CoreEngine
// {
//
//	inline Xtype distance2(Xtype* x1, Xtype* x2, int dim)
//	{
//		Xtype S = 0;
//		for(int k=0; k < dim; k++)
//			S+= (x1[k]-x2[k])*(x1[k]-x2[k]);
//		return S;
//	}
//
//
//	bool equal(Xtype* x1, Xtype* x2, int dim)
//	{
//		for(int k=0; k < dim; k++)
//		{
//			if(x1[k] != x2[k])
//				return false;
//		}
//
//		return true ;
//	}
//
//	//KDTreeNode template class implementation
//	class KDNode
//	{
//		//member functions
//	public:
//		int axis ;
//		Xtype x[SD];
//		uint32_t id ;
//		bool checked ;
//		bool orientation ;
//
//		KDNode(Xtype* x0, int axis0);
//
//		KDNODE*	Insert(Xtype* x);
//		KDNODE*	FindParent(Xtype* x0);
//
//		KDNODE* Parent ;
//		KDNODE* Left ;
//		KDNODE* Right ;
//	};
//
//
//	KDNODE::KDNode(Xtype* x0, int axis0)
//	{
//		axis = axis0;
//		for(int k=0; kaxis  ;
//			parent = next ;
//			if(x0[split] > next->x[split])
//				next = next->Right ;
//			else
//				next = next->Left ;
//	}
//	return parent ;
// }
//
// KDNODE*	KDNODE::Insert(Xtype* p)
// {
//	KDNODE* parent = FindParent(p);
//	if(equal(p, parent->x, SD))
//		return nullptr ;
//
//	KDNODE* newNode = new KDNODE(p, parent->axis +1 < SD? parent->axis+1:0);
//	newNode->Parent = parent ;
//
//	if(p[parent->axis] > parent->x[parent->axis])
//	{
//		parent->Right = newNode ;
//		newNode->orientation = 1 ;
//	}
//	else
//	{
//		parent->Left = newNode ;
//		newNode->orientation = 0 ;
//	}
//
//	return newNode ;
// }
//
//
//
// class KDTree
// {
// public:
//	KDNODE*  Root ;
//	KDTree();
//
//	bool				add(Xtype* x);
//	KDNODE*				find_nearest(Xtype* x0);
//	KDNODE*				find_nearest_brute(Xtype* x) ;
//
//	inline	void		check_subtree(KDNODE* node, Xtype* x);
//	inline  void		set_bounding_cube(KDNODE* node, Xtype* x);
//	inline KDNODE*		search_parent(KDNODE* parent, Xtype* x);
//	void				uncheck();
//
//	LARGE_INTEGER		TimeStart, TimeFinish;
//	LARGE_INTEGER		CounterFreq;
//
//	void GetTimerFrequency(){
//		QueryPerformanceFrequency(&CounterFreq);
//	}
//
//	void StartTimer(){
//		QueryPerformanceCounter(&TimeStart);
//	}
//
//	void StopTimer(){
//		QueryPerformanceCounter(&TimeFinish);
//	}
//
//	double GetElapsedTime(){
//		double calc_time = (double)(TimeFinish.LowPart - TimeStart.LowPart)/(double)CounterFreq.LowPart;
//		return 1000*calc_time ;
//	}
//
// public:
//	Xtype				d_min ;
//	KDNODE*				nearest_neighbour ;
//
//	int					KD_id  ;
//
//	KDNODE*				List[KD_MAX_POINTS] ;
//	int					nList ;
//
//	KDNODE*				CheckedNodes[KD_MAX_POINTS] ;
//	int					checked_nodes ;
//
//	Xtype				x_min[SD], x_max[SD];
//	bool				max_boundary[SD], min_boundary[SD];
//	int					n_boundary ;
//
// };
//
// KDTREE::KDTree()
// {
//	Root = nullptr ;
//	KD_id = 1;
//	nList = 0;
// }
//
//
// bool KDTree::add(Xtype* x)
// {
//	if(nList >= KD_MAX_POINTS-1)
//		return 0; //can't add more points
//
//	if(!Root)
//	{
//		Root =  new KDNODE(x,0);
//		Root->id = KD_id++ ;
//		List[nList++] = Root ;
//	}
//	else
//	{
//		KDNODE* pNode ;
//		if((pNode=Root->Insert(x)))
//		{
//			pNode->id = KD_id++ ;
//			List[nList++] = pNode;
//		}
//	}
//
//	return true ;
// }
//
//
// //sequential nearest neighbour search
// KDNODE* KDTree::find_nearest_brute(Xtype* x)
// {
//	if(!Root)
//		return nullptr;
//
//	KDNODE* nearest = Root ;
//	Xtype d ;
//	d_min = distance2(Root->x, x, SD);
//	for(int n=1; nx, x, SD);
//	if(d < d_min)
//	{
//		nearest = List[n] ;
//		d_min = d;
//	}
// }
//
// return nearest ;
// }
//
//
// KDNODE* KDTREE::find_nearest(Xtype* x)
// {
//	if(!Root)
//		return nullptr ;
//
//	checked_nodes = 0;
//
//	KDNODE* parent = Root->FindParent(x);
//	nearest_neighbour = parent ;
//	d_min = distance2(x, parent->x, SD); ;
//
//	if(equal(x, parent->x, SD))
//		return nearest_neighbour ;
//
//	search_parent(parent, x);
//	uncheck();
//
//	return nearest_neighbour ;
// }
//
//
//
//
// void KDTREE::check_subtree(KDNODE* node, Xtype* x)
// {
//	if(!node || node->checked)
//		return ;
//
//	CheckedNodes[checked_nodes++] = node ;
//	node->checked = true ;
//	set_bounding_cube(node, x);
//
//	int dim = node->axis ;
//	Xtype d= node->x[dim] - x[dim];
//
//	if (d*d > d_min) {
//		if (node->x[dim] > x[dim])
//			check_subtree(node->Left, x);
//		else
//			check_subtree(node->Right, x);
//	}
//	// If the distance from the key to the current value is
//	// less than the nearest distance, we still need to look
//	// in both directions.
//	else {
//		check_subtree(node->Left, x);
//		check_subtree(node->Right, x);
//	}
// }
//
// void KDTREE::set_bounding_cube(KDNODE* node, Xtype* x)
// {
//	if(!node)
//		return ;
//	int d=0;
//	Xtype dx;
//	for(int k=0; kx[k]-x[k];
//		if(dx > 0)
//		{
//			dx *= dx ;
//			if(!max_boundary[k])
//			{
//				if(dx > x_max[k])
//					x_max[k] = dx;
//				if(x_max[k]>d_min)
//				{
//					max_boundary[k] =true;
//					n_boundary++ ;
//				}
//			}
//		}
//		else
//		{
//			dx *= dx ;
//			if(!min_boundary[k])
//			{
//				if(dx > x_min[k])
//					x_min[k] = dx;
//				if(x_min[k]>d_min)
//				{
//					min_boundary[k] =true;
//					n_boundary++ ;
//				}
//			}
//		}
//		d+=dx;
//		if(d>d_min)
//			return ;
//
// }
//
// if(dParent ;
//	}
//
//	return search_root ;
// }
//
// void KDTREE::uncheck()
// {
//	//for(int n=0; nchecked = false;)
// }
//
// } // CoreEngine
//
// //#include "ROROctree.inl"
