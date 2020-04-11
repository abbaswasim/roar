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

#include "rorgraph.hpp"
#include <utility>

namespace ror
{
void GraphNode::add_child(VertexId a_child)
{
	this->m_children.emplace_back(a_child);
}

void GraphNode::add_parent(VertexId a_parent)
{
	this->m_parents.emplace_back(a_parent);
}

void GraphNode::remove_child(VertexId a_child)
{
	this->m_children.erase(
		std::remove_if(this->m_children.begin(), this->m_children.end(),
					   [&](const VertexId &n) { return (n == a_child); }),
		this->m_children.end());
}

void GraphNode::remove_parent(VertexId a_parent)
{
	this->m_parents.erase(
		std::remove_if(this->m_parents.begin(), this->m_parents.end(),
					   [&](const VertexId &n) { return (n == a_parent); }),
		this->m_parents.end());
}

void GraphNode::clear_children()
{
	this->m_children.clear();
}

size_t GraphNode::children_count() const
{
	return this->m_children.size();
}

size_t GraphNode::parents_count() const
{
	return this->m_parents.size();
}

VertexId GraphNode::get_child(VertexId a_child) const
{
	const VertexId s = this->m_children.size();
	if (a_child < s)
		return this->m_children[a_child];
	else
		throw std::runtime_error("Error out of bound index");
}

VertexId GraphNode::get_parent(VertexId a_parent) const
{
	const VertexId s = this->m_parents.size();
	if (a_parent < s)
		return this->m_parents[a_parent];
	else
		throw std::runtime_error("Error out of bound index");
}

template <class _type, bool _thread_safe>
VertexId Graph<_type, _thread_safe>::append_vertex(const Vertex &a_vertex)
{
	// std::scoped_lock<std::mutex> mtx(this->m_mutex);
	if constexpr (_thread_safe)
		std::lock_guard<std::mutex> mtx(this->m_mutex);

	this->add_leaf(a_vertex);

	const VertexId s = this->m_nodes.size();

	if (s > 1)        // If we have more than 1 item then set children for parent
	{
		this->add_child(s - 2, s - 1);
	}

	return VertexId(s - 1);
}

template <class _type, bool _thread_safe>
VertexId Graph<_type, _thread_safe>::add_vertex(const Vertex &a_vertex)
{
	if constexpr (_thread_safe)
		std::lock_guard<std::mutex> mtx(this->m_mutex);

	this->add_leaf(a_vertex);

	return VertexId(this->m_nodes.size() - 1);
}

template <class _type, bool _thread_safe>
void Graph<_type, _thread_safe>::add_edge(VertexId a_parent, VertexId a_child)
{
	if constexpr (_thread_safe)
		std::lock_guard<std::mutex> mtx(this->m_mutex);

	const VertexId s = this->m_nodes.size();
	if ((a_parent >= 0 && a_parent < s) && (a_child >= 0 && a_child < s))
	{
		this->add_child(a_parent, a_child);
	}
}

template <class _type, bool _thread_safe>
void Graph<_type, _thread_safe>::remove_edge(VertexId a_parent, VertexId a_child)
{
	if constexpr (_thread_safe)
		std::lock_guard<std::mutex> mtx(this->m_mutex);

	const VertexId s = this->m_nodes.size();
	if ((a_parent >= 0 && a_parent < s) && (a_child >= 0 && a_child < s))
	{
		std::get<0>(this->m_nodes[a_parent]).remove_child(a_child);
		std::get<0>(this->m_nodes[a_child]).remove_parent(a_parent);
		--this->m_edge_count;
	}
}

template <class _type, bool _thread_safe>
void Graph<_type, _thread_safe>::add_leaf(const Vertex &a_vertex)
{
	this->m_nodes.emplace_back(a_vertex);
}

template <class _type, bool _thread_safe>
void Graph<_type, _thread_safe>::add_child(VertexId a_parent, VertexId a_child)
{
	auto &[parent, payload] = this->m_nodes[a_parent];
	parent.add_child(a_child);

	auto &[child, payload1] = this->m_nodes[a_child];
	child.add_parent(a_parent);

	++this->m_edge_count;
}

template <class _type, bool _thread_safe>
size_t Graph<_type, _thread_safe>::vertex_count() const
{
	if constexpr (_thread_safe)
		std::lock_guard<std::mutex> mtx(this->m_mutex);

	return this->m_nodes.size();
}

template <class _type, bool _thread_safe>
void Graph<_type, _thread_safe>::clear()
{
	if constexpr (_thread_safe)
		std::lock_guard<std::mutex> mtx(this->m_mutex);

	this->m_nodes.clear();
}

template <class _type, bool _thread_safe>
std::tuple<GraphNode, _type> *Graph<_type, _thread_safe>::at(const VertexId a_index)
{
	if constexpr (_thread_safe)
		std::lock_guard<std::mutex> mtx(this->m_mutex);

	const VertexId s = this->m_nodes.size();
	if (a_index >= 0 && a_index < s)
		return &this->m_nodes[a_index];

	return nullptr;
}

template <class _type, bool _thread_safe>
void Graph<_type, _thread_safe>::update_sorted_list()
{
	std::vector<GraphNode> graph_copy;

	for (auto n : this->m_nodes)
	{
		graph_copy.emplace_back(std::get<0>(n));
	}

	this->m_sorted_list = get_topologicaly_sorted_list_kahn(graph_copy, this->m_edge_count);
}

template <class _type, bool _thread_safe>
bool Graph<_type, _thread_safe>::is_acyclic()
{
	if constexpr (_thread_safe)
		std::lock_guard<std::mutex> mtx(this->m_mutex);

	if (this->m_edge_count == 0 || this->m_nodes.empty())
		return true;

	this->update_sorted_list();

	if (this->m_sorted_list.size() == 0)
		return false;

	return true;
}

template <class _type, bool _thread_safe>
std::vector<VertexId> Graph<_type, _thread_safe>::get_sorted_list()
{
	if constexpr (_thread_safe)
		std::lock_guard<std::mutex> mtx(this->m_mutex);

	if (this->m_sorted_list.empty())
	{
		this->update_sorted_list();
	}
	return this->m_sorted_list;
}

// https://en.wikipedia.org/wiki/Topological_sorting
std::vector<VertexId> get_topologicaly_sorted_list_kahn(std::vector<GraphNode> &a_graph, VertexId a_edge_count)
{
	const VertexId s = a_graph.size();

	std::vector<VertexId> sorted_list;
	sorted_list.reserve(s);
	std::queue<VertexId> q;

	// Add all nodes with no incoming edges (nodes with no parents)
	for (VertexId i = 0; i < s; ++i)
	{
		if (a_graph[i].parents_count() == 0)
		{
			q.emplace(i);
		}
	}

	while (!q.empty())
	{
		VertexId vid = q.front();
		sorted_list.emplace_back(vid);
		q.pop();

		// For each children remove links (both to parent(vid) and all children)
		GraphNode &    gn = a_graph[vid];
		const VertexId sz = gn.children_count();
		for (size_t i = 0; i < sz; ++i)
		{
			auto  ci = gn.get_child(i);
			auto &c  = a_graph[ci];
			c.remove_parent(vid);
			if (c.parents_count() == 0)
			{
				q.emplace(ci);
			}
		}
		a_edge_count -= gn.children_count();
		gn.clear_children();
	}

	if (a_edge_count > 0)
	{
		return std::vector<VertexId>();
	}

	return sorted_list;
}
}        // namespace ror
