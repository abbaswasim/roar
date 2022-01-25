#include "graph.hpp"
#include <iostream>

namespace ror_test
{

void GraphTest::SetUp()
{
	ror::Graph<std::string>::Vertex A = std::make_tuple(ror::GraphNode(), "A");
	ror::Graph<std::string>::Vertex B = std::make_tuple(ror::GraphNode(), "B");
	ror::Graph<std::string>::Vertex C = std::make_tuple(ror::GraphNode(), "C");
	ror::Graph<std::string>::Vertex D = std::make_tuple(ror::GraphNode(), "D");
	ror::Graph<std::string>::Vertex E = std::make_tuple(ror::GraphNode(), "E");

	{
		ror::VertexId Ai = this->m_graph1.add_vertex(A);
		ror::VertexId Bi = this->m_graph1.add_vertex(B);
		ror::VertexId Ci = this->m_graph1.add_vertex(C);
		ror::VertexId Di = this->m_graph1.add_vertex(D);
		ror::VertexId Ei = this->m_graph1.add_vertex(E);

		ror::VertexId Fi = this->m_graph1.add_vertex(std::make_tuple(ror::GraphNode(), "F"));
		ror::VertexId Gi = this->m_graph1.add_vertex(std::make_tuple(ror::GraphNode(), "G"));
		ror::VertexId Hi = this->m_graph1.add_vertex(std::make_tuple(ror::GraphNode(), "H"));

		this->m_graph1.add_edge(Ai, Ci);
		this->m_graph1.add_edge(Bi, Ci);
		this->m_graph1.add_edge(Bi, Di);
		this->m_graph1.add_edge(Bi, Ei);
		this->m_graph1.add_edge(Ci, Ei);
		this->m_graph1.add_edge(Di, Ei);
		this->m_graph1.add_edge(Ei, Fi);
		this->m_graph1.add_edge(Ei, Gi);
		this->m_graph1.add_edge(Hi, Fi);
	}

	{
		this->m_graph2.append_vertex(A);
		this->m_graph2.append_vertex(B);

		ror::VertexId Ci = this->m_graph2.append_vertex(C);

		this->m_graph2.append_vertex(E);

		ror::VertexId Fi = this->m_graph2.append_vertex(std::make_tuple(ror::GraphNode(), "F"));

		ror::VertexId Di = this->m_graph2.add_vertex(D);
		ror::VertexId Gi = this->m_graph2.append_vertex(std::make_tuple(ror::GraphNode(), "G"));

		ror::VertexId Hi = this->m_graph2.add_vertex(std::make_tuple(ror::GraphNode(), "H"));

		this->m_graph2.add_edge(Ci, Hi);
		this->m_graph2.add_edge(Ci, Di);
		this->m_graph2.add_edge(Fi, Gi);
		this->m_graph2.add_edge(Gi, Ci);
	}
}

ror::Graph<std::string> *GraphTest::get_graph1()
{
	return &this->m_graph1;
}

ror::Graph<std::string> *GraphTest::get_graph2()
{
	return &this->m_graph2;
}

void GraphTest::TearDown()
{
	this->m_graph1.clear();
	this->m_graph2.clear();
}

TEST_F(GraphTest, GraphTest_parents_remove)
{
	ror::GraphNode gn;

	gn.add_parent(0);
	gn.add_parent(3);
	gn.add_parent(4);
	gn.add_parent(6);
	gn.add_parent(2);

	EXPECT_EQ(gn.get_parent(0), size_t(0));
	EXPECT_EQ(gn.get_parent(1), size_t(3));
	EXPECT_EQ(gn.get_parent(2), size_t(4));
	EXPECT_EQ(gn.get_parent(3), size_t(6));
	EXPECT_EQ(gn.get_parent(4), size_t(2));

	gn.remove_parent(4);

	EXPECT_EQ(gn.get_parent(0), size_t(0));
	EXPECT_EQ(gn.get_parent(1), size_t(3));
	EXPECT_EQ(gn.get_parent(2), size_t(6));
	EXPECT_EQ(gn.get_parent(3), size_t(2));

	gn.remove_parent(3);
	gn.add_parent(8);

	EXPECT_EQ(gn.get_parent(0), size_t(0));
	EXPECT_EQ(gn.get_parent(1), size_t(6));
	EXPECT_EQ(gn.get_parent(2), size_t(2));
	EXPECT_EQ(gn.get_parent(3), size_t(8));
}

TEST_F(GraphTest, GraphTest_confirm_nodes)
{
	{
		auto pnode = this->get_graph1()->at(0);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("A").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(1);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("B").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(2);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("C").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(3);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("D").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(4);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("E").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(5);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("F").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(6);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("G").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(7);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("H").c_str());
	}

	{
		auto pnode = this->get_graph2()->at(0);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("A").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(1);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("B").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(2);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("C").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(3);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("E").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(4);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("F").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(5);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("D").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(6);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("G").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(7);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("H").c_str());
	}
}

TEST_F(GraphTest, GraphTest_confirm_links)
{
	{
		auto pnode  = this->get_graph1()->at(0);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("A").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(1);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(3));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("B").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(2);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("C").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(3);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("D").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(4);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(2));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("E").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(5);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(0));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("F").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(6);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(0));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("G").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(7);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("H").c_str());
	}

	{
		auto pnode  = this->get_graph2()->at(0);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("A").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(1);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("B").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(2);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(3));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("C").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(3);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("E").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(4);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("F").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(5);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("D").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(6);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("G").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(7);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(0));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("H").c_str());
	}
}

TEST_F(GraphTest, GraphTest_confirm_acyclic)
{
	ASSERT_TRUE(this->get_graph1()->is_acyclic());
	ASSERT_FALSE(this->get_graph2()->is_acyclic());
}

TEST_F(GraphTest, GraphTest_confirm_acyclic_change)
{
	ASSERT_TRUE(this->get_graph1()->is_acyclic());
	this->get_graph1()->add_edge(5, 2);        // Add edge between F -> B
	ASSERT_FALSE(this->get_graph1()->is_acyclic());
}

TEST_F(GraphTest, GraphTest_confirm_acyclic_clear)
{
	ASSERT_TRUE(this->get_graph1()->is_acyclic());
	this->get_graph1()->clear();
	ASSERT_TRUE(this->get_graph1()->is_acyclic());

	ASSERT_FALSE(this->get_graph2()->is_acyclic());
	this->get_graph2()->clear();
	ASSERT_TRUE(this->get_graph2()->is_acyclic());
}

TEST_F(GraphTest, GraphTest_confirm_topological_sorted_list)
{
	{
		ASSERT_TRUE(this->get_graph1()->is_acyclic());
		std::vector<ror::VertexId> sorteds_list = this->get_graph1()->get_sorted_list();
		std::vector<ror::VertexId> ground_truth{0, 1, 7, 2, 3, 4, 5, 6};

		ASSERT_EQ(sorteds_list.size(), ground_truth.size());

		size_t i = 0;
		for (auto sl : sorteds_list)
		{
			ASSERT_EQ(sl, ground_truth[i++]);
		}
	}

	{
		ASSERT_FALSE(this->get_graph2()->is_acyclic());
		std::vector<ror::VertexId> sorteds_list = this->get_graph2()->get_sorted_list();
		ASSERT_EQ(sorteds_list.size(), size_t(0));
	}

	{
		ror::VertexId C(2);
		ror::VertexId D(5);
		ror::VertexId F(4);
		ror::VertexId G(6);

		this->get_graph2()->remove_edge(C, D);
		this->get_graph2()->remove_edge(F, G);

		std::vector<ror::VertexId> sorteds_list = this->get_graph2()->get_sorted_list();
		std::vector<ror::VertexId> ground_truth{0, 5, 1, 6, 2, 3, 7, 4};

		ASSERT_EQ(sorteds_list.size(), ground_truth.size());

		size_t i = 0;
		for (auto sl : sorteds_list)
		{
			ASSERT_EQ(sl, ground_truth[i++]);
		}
	}
}

void GraphTSTest::SetUp()
{
	ror::Graph<std::string>::Vertex A = std::make_tuple(ror::GraphNode(), "A");
	ror::Graph<std::string>::Vertex B = std::make_tuple(ror::GraphNode(), "B");
	ror::Graph<std::string>::Vertex C = std::make_tuple(ror::GraphNode(), "C");
	ror::Graph<std::string>::Vertex D = std::make_tuple(ror::GraphNode(), "D");
	ror::Graph<std::string>::Vertex E = std::make_tuple(ror::GraphNode(), "E");

	{
		ror::VertexId Ai = this->m_graph1.add_vertex(A);
		ror::VertexId Bi = this->m_graph1.add_vertex(B);
		ror::VertexId Ci = this->m_graph1.add_vertex(C);
		ror::VertexId Di = this->m_graph1.add_vertex(D);
		ror::VertexId Ei = this->m_graph1.add_vertex(E);

		ror::VertexId Fi = this->m_graph1.add_vertex(std::make_tuple(ror::GraphNode(), "F"));
		ror::VertexId Gi = this->m_graph1.add_vertex(std::make_tuple(ror::GraphNode(), "G"));
		ror::VertexId Hi = this->m_graph1.add_vertex(std::make_tuple(ror::GraphNode(), "H"));

		this->m_graph1.add_edge(Ai, Ci);
		this->m_graph1.add_edge(Bi, Ci);
		this->m_graph1.add_edge(Bi, Di);
		this->m_graph1.add_edge(Bi, Ei);
		this->m_graph1.add_edge(Ci, Ei);
		this->m_graph1.add_edge(Di, Ei);
		this->m_graph1.add_edge(Ei, Fi);
		this->m_graph1.add_edge(Ei, Gi);
		this->m_graph1.add_edge(Hi, Fi);
	}

	{
		this->m_graph2.append_vertex(A);
		this->m_graph2.append_vertex(B);

		ror::VertexId Ci = this->m_graph2.append_vertex(C);

		this->m_graph2.append_vertex(E);

		ror::VertexId Fi = this->m_graph2.append_vertex(std::make_tuple(ror::GraphNode(), "F"));

		ror::VertexId Di = this->m_graph2.add_vertex(D);
		ror::VertexId Gi = this->m_graph2.append_vertex(std::make_tuple(ror::GraphNode(), "G"));

		ror::VertexId Hi = this->m_graph2.add_vertex(std::make_tuple(ror::GraphNode(), "H"));

		this->m_graph2.add_edge(Ci, Hi);
		this->m_graph2.add_edge(Ci, Di);
		this->m_graph2.add_edge(Fi, Gi);
		this->m_graph2.add_edge(Gi, Ci);
	}
}

ror::Graph<std::string> *GraphTSTest::get_graph1()
{
	return &this->m_graph1;
}

ror::Graph<std::string> *GraphTSTest::get_graph2()
{
	return &this->m_graph2;
}

void GraphTSTest::TearDown()
{
	this->m_graph1.clear();
	this->m_graph2.clear();
}

TEST_F(GraphTSTest, GraphTest_parents_remove)
{
	ror::GraphNode gn;

	gn.add_parent(0);
	gn.add_parent(3);
	gn.add_parent(4);
	gn.add_parent(6);
	gn.add_parent(2);

	EXPECT_EQ(gn.get_parent(0), size_t(0));
	EXPECT_EQ(gn.get_parent(1), size_t(3));
	EXPECT_EQ(gn.get_parent(2), size_t(4));
	EXPECT_EQ(gn.get_parent(3), size_t(6));
	EXPECT_EQ(gn.get_parent(4), size_t(2));

	gn.remove_parent(4);

	EXPECT_EQ(gn.get_parent(0), size_t(0));
	EXPECT_EQ(gn.get_parent(1), size_t(3));
	EXPECT_EQ(gn.get_parent(2), size_t(6));
	EXPECT_EQ(gn.get_parent(3), size_t(2));

	gn.remove_parent(3);
	gn.add_parent(8);

	EXPECT_EQ(gn.get_parent(0), size_t(0));
	EXPECT_EQ(gn.get_parent(1), size_t(6));
	EXPECT_EQ(gn.get_parent(2), size_t(2));
	EXPECT_EQ(gn.get_parent(3), size_t(8));
}

TEST_F(GraphTSTest, GraphTest_confirm_nodes)
{
	{
		auto pnode = this->get_graph1()->at(0);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("A").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(1);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("B").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(2);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("C").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(3);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("D").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(4);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("E").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(5);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("F").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(6);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("G").c_str());
	}
	{
		auto pnode = this->get_graph1()->at(7);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("H").c_str());
	}

	{
		auto pnode = this->get_graph2()->at(0);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("A").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(1);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("B").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(2);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("C").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(3);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("E").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(4);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("F").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(5);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("D").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(6);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("G").c_str());
	}
	{
		auto pnode = this->get_graph2()->at(7);
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("H").c_str());
	}
}

TEST_F(GraphTSTest, GraphTest_confirm_links)
{
	{
		auto pnode  = this->get_graph1()->at(0);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("A").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(1);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(3));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("B").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(2);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("C").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(3);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("D").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(4);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(2));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("E").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(5);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(0));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("F").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(6);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(0));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("G").c_str());
	}
	{
		auto pnode  = this->get_graph1()->at(7);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("H").c_str());
	}

	{
		auto pnode  = this->get_graph2()->at(0);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("A").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(1);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("B").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(2);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(3));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("C").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(3);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("E").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(4);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("F").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(5);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("D").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(6);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(1));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("G").c_str());
	}
	{
		auto pnode  = this->get_graph2()->at(7);
		auto vertex = std::get<0>(*pnode);
		EXPECT_EQ(vertex.children_count(), size_t(0));
		ASSERT_STREQ(std::get<1>(*pnode).c_str(), std::string("H").c_str());
	}
}

TEST_F(GraphTSTest, GraphTest_confirm_acyclic)
{
	ASSERT_TRUE(this->get_graph1()->is_acyclic());
	ASSERT_FALSE(this->get_graph2()->is_acyclic());
}

TEST_F(GraphTSTest, GraphTest_confirm_acyclic_change)
{
	ASSERT_TRUE(this->get_graph1()->is_acyclic());
	this->get_graph1()->add_edge(5, 2);        // Add edge between F -> B
	ASSERT_FALSE(this->get_graph1()->is_acyclic());
}

TEST_F(GraphTSTest, GraphTest_confirm_acyclic_clear)
{
	ASSERT_TRUE(this->get_graph1()->is_acyclic());
	this->get_graph1()->clear();
	ASSERT_TRUE(this->get_graph1()->is_acyclic());

	ASSERT_FALSE(this->get_graph2()->is_acyclic());
	this->get_graph2()->clear();
	ASSERT_TRUE(this->get_graph2()->is_acyclic());
}

TEST_F(GraphTSTest, GraphTest_confirm_topological_sorted_list)
{
	{
		ASSERT_TRUE(this->get_graph1()->is_acyclic());
		std::vector<ror::VertexId> sorteds_list = this->get_graph1()->get_sorted_list();
		std::vector<ror::VertexId> ground_truth{0, 1, 7, 2, 3, 4, 5, 6};

		ASSERT_EQ(sorteds_list.size(), ground_truth.size());

		size_t i = 0;
		for (auto sl : sorteds_list)
		{
			ASSERT_EQ(sl, ground_truth[i++]);
		}
	}

	{
		ASSERT_FALSE(this->get_graph2()->is_acyclic());
		std::vector<ror::VertexId> sorteds_list = this->get_graph2()->get_sorted_list();
		ASSERT_EQ(sorteds_list.size(), size_t(0));
	}

	{
		ror::VertexId C(2);
		ror::VertexId D(5);
		ror::VertexId F(4);
		ror::VertexId G(6);

		this->get_graph2()->remove_edge(C, D);
		this->get_graph2()->remove_edge(F, G);

		std::vector<ror::VertexId> sorteds_list = this->get_graph2()->get_sorted_list();
		std::vector<ror::VertexId> ground_truth{0, 5, 1, 6, 2, 3, 7, 4};

		ASSERT_EQ(sorteds_list.size(), ground_truth.size());

		size_t i = 0;
		for (auto sl : sorteds_list)
		{
			ASSERT_EQ(sl, ground_truth[i++]);
		}
	}
}

}        // namespace ror_test
