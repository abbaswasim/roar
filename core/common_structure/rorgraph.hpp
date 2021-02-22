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

#include "roar.hpp"
#include <mutex>
#include <queue>
#include <vector>

namespace ror
{
using VertexId = size_t;

/**
 * Node for Graph
 * Each node contains a list of children and parents
 */
class ROAR_ENGINE_ITEM GraphNode final
{
  public:
	GraphNode()                             = default;                   //! Default constructor
	GraphNode(const GraphNode &a_other)     = default;                   //! Copy constructor
	GraphNode(GraphNode &&a_other) noexcept = default;                   //! Move constructor
	GraphNode &operator=(const GraphNode &a_other) = default;            //! Copy assignment operator
	GraphNode &operator=(GraphNode &&a_other) noexcept = default;        //! Move assignment operator
	~GraphNode() noexcept                              = default;        //! Destructor

	FORCE_INLINE VertexId get_child(VertexId a_child) const;
	FORCE_INLINE VertexId get_parent(VertexId a_parent) const;
	FORCE_INLINE size_t   children_count() const;
	FORCE_INLINE size_t   parents_count() const;
	FORCE_INLINE void     add_child(VertexId a_child);
	FORCE_INLINE void     add_parent(VertexId a_parent);
	FORCE_INLINE void     remove_child(VertexId a_child);
	FORCE_INLINE void     remove_parent(VertexId a_parent);
	FORCE_INLINE void     clear_children();

  protected:
  private:
	std::vector<VertexId> m_children{};        //! Also describes link to child from current GraphNode
	std::vector<VertexId> m_parents{};         //! Also describes link from parent to current GraphNode
};

/**
 * @brief      Kahn's algorithm to find topological sorted list
 * @details    This method runs Kahn's algorithm to check if the graph is acyclic or not
 *             It then returns topological sorted list of the graph if its acyclic and empty list otherwise
 * @param      a_graph reference to the graph to use for calculation
 * @param      a_edge_count number of edges in the graph
 * @return     A toplogical sorted list of the graph or empty list if acyclic
 */
std::vector<VertexId> get_topologicaly_sorted_list_kahn(std::vector<GraphNode> &a_graph, VertexId a_edge_count);

template <bool>
class ConditionalMutex
{};

template <>
class ConditionalMutex<true>
{
  public:
	std::mutex m_mutex{};        //! Use to synchronize access from different threads
};

/**
 * Directed graph that might or might not be acyclic
 * It can be created from a set of vertices or consecutively added vertices one after the other
 * Each vertex carries a typed payload
 * This graph can be used as internally thread safe or not, Graph<T, true> is internally thread safe
 * while Graph<T, false> is not thread safe and can be made externally thread safe
 */
template <class _type, bool _thread_safe = false>
class ROAR_ENGINE_ITEM Graph final : public ConditionalMutex<_thread_safe>
{
  public:
	using Vertex = std::tuple<GraphNode, _type>;

	Graph()                         = default;                   //! Default constructor
	Graph(const Graph &a_other)     = default;                   //! Copy constructor
	Graph(Graph &&a_other) noexcept = default;                   //! Move constructor
	Graph &operator=(const Graph &a_other) = default;            //! Copy assignment operator
	Graph &operator=(Graph &&a_other) noexcept = default;        //! Move assignment operator
	~Graph() noexcept                          = default;        //! Destructor

	/**
	 * @brief      Appends a vertex (tuple<GraphNode, typename>) to the tail of the graph
	 * @details    Takes a tuple and appends it to the graph. a_vertex becomes child of the the last vertex
	 *             Also tries to find parent (last vertex) and adds it as parent for a_vertex
	 * @param      a_vertex The vertex to add into the graph
	 * @return     VertexId where the vertex was appended
	 */
	// TODO Does the const reference makes sense here or would I be abetter off with pass by value
	FORCE_INLINE VertexId append_vertex(const Vertex &a_vertex);

	/**
	 * @brief      Adds a vertex (tuple<GraphNode, typename>) to the tail of the graph
	 * @details    Takes a tuple and appends it to the graph. It then returns the index of the inserted tuple
	 *             this function does not do any linking, clients needs to call add_edge to link vertices inserted
	 *             this way
	 * @return     Index of the inserted vertex
	 */
	FORCE_INLINE VertexId add_vertex(const Vertex &a_vertex);

	/**
	 * @brief      Adds an edge between two vertices
	 * @details    Adds an edge between the two vertices identified by a_parent and a_child
	 *             The link is assumed to be from the parent to the child, hence it adds a_child into
	 *             to the list of children in a_parent, it also add a_prent to list of parents into a_child
	 * @param      a_parent Index of the parent vertex
	 * @param      a_child Index of the child vertex
	 */
	FORCE_INLINE void add_edge(VertexId a_parent, VertexId a_child);

	/**
	 * @brief      Removes an edge between two vertices
	 * @details    Removes an edge between the two vertices identified by a_parent and a_child
	 *             The link is assumed to be from the parent to the child but child also links to its parent
	 * @param      a_parent Index of the parent vertex
	 * @param      a_child Index of the child vertex
	 */
	FORCE_INLINE void remove_edge(VertexId a_parent, VertexId a_child);

	/**
	 * @brief      Returns vertex count
	 * @details    Returns vertex count
	 * @return     Total vertex count
	 */
	FORCE_INLINE size_t vertex_count() const;

	/**
	 * @brief      Clears the Graph
	 * @details    Clears the Graph by calling every elements destructors
	 */
	FORCE_INLINE void clear();

	/**
	 * @brief      Gets an element of the graph
	 * @details    Gets element at a specific index of the graph
	 * @param      a_index Element index, this is the order in which it was inserted
	 * @return     point to std::tuple<GraphNode, _type> at a_index if the index is valid and nullptr otherwise
	 */
	FORCE_INLINE Vertex *at(VertexId a_index);

	/**
	 * @brief      Checks if the graph is acylic
	 * @details    This function goes through all the vertices of the graph and their links
	 *             to determine if the graph is acyclic or not
	 * @return     true if the graph is acyclick and false otherwise
	 */
	bool is_acyclic();

	/**
	 * @brief      Returns the topological sorted list
	 * @details    Returns the topological sorted list by copying it into returned
	 * @return     std::vector<VertexId> with topological sorted list of the graph
	 */
	std::vector<VertexId> get_sorted_list();

  protected:
  private:
	/**
	 * @brief      Adds a leaf node
	 * @details    Adds a leaf node and increments m_leaf_count
	 * @param      a_vertex Vertex to be added as a leaf node
	 */
	FORCE_INLINE void add_leaf(const Vertex &a_vertex);

	/**
	 * @brief      Adds a children for a_parent
	 * @details    Adds a children for a_parent and decrements m_leaf_count if a_parent was a leaf before
	 * @param      a_parent Index of the parent node
	 * @param      a_child Index of the child node
	 */
	FORCE_INLINE void add_child(VertexId a_parent, VertexId a_child);

	/**
	 * @brief      Updates topological sorted list of the graph
	 * @details    Updates topological sorted list of the graph by first calling kahn algorithm to check for cycles
	 *             If the graph is acyclic the sorted list will contain all vertices otherwise empty
	 */
	void update_sorted_list();

	std::vector<Vertex>   m_nodes{};              //! List of vertices in the graph
	std::vector<VertexId> m_sorted_list{};        //! List of vertices in topographical sorted order of the graph
	VertexId              m_edge_count{0};        //! Total amount of edges in the graph at a time
};

}        // namespace ror

#include "rorgraph.hh"
