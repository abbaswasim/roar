#include "common.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector4.hpp"
#include "profiling/rorlog.hpp"
#include "renderer/rorrenderer.hpp"
#include "rhi/crtp_interfaces/rorrenderpass.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>
#include <vector>

namespace ror_test
{
using RenderTargets = std::vector<rhi::RenderOutputRef<rhi::RenderTarget>>;

void test_render_target(const rhi::RenderTarget &a, const rhi::RenderTarget &b)
{
	EXPECT_EQ(a.m_target_index, b.m_target_index);
	EXPECT_EQ(&a.m_target_reference.get(), &b.m_target_reference.get());
	EXPECT_EQ(a.m_load_action, b.m_load_action);
	EXPECT_EQ(a.m_store_action, b.m_store_action);
	EXPECT_EQ(a.m_type , b.m_type);
}

void test_render_targets(const std::vector<rhi::RenderTarget> a, std::vector<rhi::RenderTarget> b)
{
	ASSERT_EQ(a.size(), b.size());

	for (size_t i = 0; i < b.size(); ++i)
		test_render_target(a[i], b[i]);
}

void test_render_target_refs(const RenderTargets a, RenderTargets b)
{
	ASSERT_EQ(a.size(), b.size());

	for (size_t i = 0; i < b.size(); ++i)
	{
		EXPECT_EQ(a[i].m_index, b[i].m_index);
		EXPECT_EQ(a[i].m_stage, b[i].m_stage);
	}

	for (size_t i = 0; i < b.size(); ++i)
		test_render_target(*a[i].m_render_output, *b[i].m_render_output);
}

void test_render_pass(const rhi::Renderpass                 rdps,
                      std::vector<rhi::RenderTarget>        a_render_targets,
                      ror::Vector2ui                        a_dimensions,
                      std::vector<uint32_t>                 a_parents,
                      std::vector<std::string>              a_names,
                      std::vector<rhi::RenderpassTechnique> a_techniques,
                      std::vector<rhi::RenderpassType>      a_types,
                      std::vector<rhi::RenderpassState>     a_states,
                      std::vector<RenderTargets>            a_rendered_inputs,
                      std::vector<RenderTargets>            a_input_attachments,
                      std::vector<int32_t>                  a_program_ids,
                      std::vector<bool>                     a_debug_outputs)
{
	auto dimensions     = rdps.dimensions();
	auto parents        = rdps.parent_ids();
	auto render_targets = rdps.render_targets();

	test_vector2f_equal(dimensions, a_dimensions);

	EXPECT_EQ(parents.size(), a_parents.size());
	for (size_t i = 0; i < parents.size(); ++i)
		EXPECT_EQ(parents[i], a_parents[i]);

	test_render_targets(render_targets, a_render_targets);

	uint32_t iai = 0;
	for (auto rsps : rdps.subpasses())
	{
		auto name = rsps.name();
		// Enable the logs for debugging
		// ror::log_info("Checking render pass {}", name.c_str());
		auto technique         = rsps.technique();
		auto type              = rsps.type();
		auto state             = rsps.state();
		auto program_id        = rsps.program_id();
		auto debug_output      = rsps.debug_output();
		auto input_attachments = rsps.input_attachments();
		auto rendered_inputs   = rsps.rendered_inputs();

		EXPECT_EQ(name, a_names[iai]);
		EXPECT_EQ(technique, a_techniques[iai]);
		EXPECT_EQ(type, a_types[iai]);
		EXPECT_EQ(state, a_states[iai]);
		EXPECT_EQ(program_id, a_program_ids[iai]);
		EXPECT_EQ(debug_output, a_debug_outputs[iai]);

		// ror::log_info("Checking render pass attachments {}", name.c_str());
		EXPECT_EQ(input_attachments.size(), a_input_attachments[iai].size());
		test_render_target_refs(input_attachments, a_input_attachments[iai]);

		// ror::log_info("Checking render pass outputs {}", name.c_str());
		EXPECT_EQ(rendered_inputs.size(), a_rendered_inputs[iai].size());
		test_render_target_refs(rendered_inputs, a_rendered_inputs[iai]);

		// ror::log_info("Checked render pass {}", name.c_str());

		iai++;
	}
}

TEST(RendererTest, config_load)
{
	ror::Renderer rdr;

	auto &rtgs  = rdr.images();
	auto &fgphs = rdr.frame_graphs();

	std::vector<std::vector<uint32_t>> empty_indices{};
	// Test forward
	{
		auto rdpses = fgphs.at("forward");

		ror::Vector2ui dimensions{1024, 768};

		// const_cast is only allowed in tests
		rhi::RenderTarget rt0{0, const_cast<rhi::TextureImage &>(rtgs[0]), rhi::LoadAction::clear, rhi::StoreAction::store, rhi::RenderOutputType::color};
		rhi::RenderTarget rt1{3, const_cast<rhi::TextureImage &>(rtgs[3]), rhi::LoadAction::clear, rhi::StoreAction::discard, rhi::RenderOutputType::depth};
		rhi::RenderTarget rt2{2, const_cast<rhi::TextureImage &>(rtgs[2]), rhi::LoadAction::clear, rhi::StoreAction::store, rhi::RenderOutputType::color};

		std::vector<RenderTargets> rirfs{};
		rirfs.resize(2);
		rirfs[0].emplace_back(&rt2, 2, rhi::ShaderStage::fragment);

		std::vector<RenderTargets> iarfs{};
		iarfs.resize(2);
		iarfs[1].emplace_back(&rt0, 0, rhi::ShaderStage::fragment);

		uint32_t index = 0;
		for (auto rdps : rdpses)
		{
			switch (index)
			{
				case 0:
					test_render_pass(rdps,
					                 {},
					                 dimensions,
					                 {},        // Parents
					                 {"node_compute_traversal"},
					                 {rhi::RenderpassTechnique::compute},
					                 {rhi::RenderpassType::node_transform},
					                 {rhi::RenderpassState::transient},
					                 {{}},        // renderered inputs
					                 {{}},        // input attachments
					                 {0},
					                 {true});
					break;
				case 1:
					test_render_pass(rdps,
					                 {rt0, rt1},
					                 dimensions,
					                 {0},        // Parents
					                 {"forward_lighting", "tonemap"},
					                 {rhi::RenderpassTechnique::fragment, rhi::RenderpassTechnique::fragment},
					                 {rhi::RenderpassType::forward_light, rhi::RenderpassType::tone_mapping},
					                 {rhi::RenderpassState::transient, rhi::RenderpassState::transient},
					                 rirfs,
					                 iarfs,
					                 {-1, -1},
					                 {true, true});
					break;
			}
			index++;
		}
	}

	// Test deferred
	{
		auto rdpses = fgphs.at("deferred");

		ror::Vector2ui dimensions{1024, 768};

		// const_cast is only allowed in tests
		rhi::RenderTarget rt0{0, const_cast<rhi::TextureImage &>(rtgs[0]), rhi::LoadAction::clear, rhi::StoreAction::store, rhi::RenderOutputType::color};
		rhi::RenderTarget rt1{1, const_cast<rhi::TextureImage &>(rtgs[1]), rhi::LoadAction::clear, rhi::StoreAction::store, rhi::RenderOutputType::color};
		rhi::RenderTarget rt2{2, const_cast<rhi::TextureImage &>(rtgs[2]), rhi::LoadAction::clear, rhi::StoreAction::store, rhi::RenderOutputType::color};
		rhi::RenderTarget rt3{3, const_cast<rhi::TextureImage &>(rtgs[3]), rhi::LoadAction::clear, rhi::StoreAction::store, rhi::RenderOutputType::color};
		rhi::RenderTarget rt4{4, const_cast<rhi::TextureImage &>(rtgs[4]), rhi::LoadAction::clear, rhi::StoreAction::store, rhi::RenderOutputType::color};
		rhi::RenderTarget rt5{5, const_cast<rhi::TextureImage &>(rtgs[5]), rhi::LoadAction::clear, rhi::StoreAction::store, rhi::RenderOutputType::color};

		// rhi::RenderTarget rt0d{0, const_cast<rhi::TextureImage &>(rtgs[0]), rhi::LoadAction::clear, rhi::StoreAction::store, rhi::RenderOutputType::depth};
		rhi::RenderTarget rt3d{3, const_cast<rhi::TextureImage &>(rtgs[3]), rhi::LoadAction::clear, rhi::StoreAction::store, rhi::RenderOutputType::depth};

		std::vector<RenderTargets> rirfs{};
		rirfs.resize(2);
		rirfs[0].emplace_back(&rt0, 2, rhi::ShaderStage::fragment);

		std::vector<RenderTargets> iarfs5{};
		iarfs5.resize(4);
		iarfs5[1].emplace_back(&rt0, 0, rhi::ShaderStage::fragment);
		iarfs5[2].emplace_back(&rt0, 0, rhi::ShaderStage::fragment);
		iarfs5[3].emplace_back(&rt0, 0, rhi::ShaderStage::fragment);

		std::vector<RenderTargets> iarfs67{};
		iarfs67.resize(1);
		iarfs67[0].emplace_back(&rt0, 0, rhi::ShaderStage::fragment);

		uint32_t index = 0;
		for (auto rdps : rdpses)
		{
			switch (index)
			{
				case 0:
					test_render_pass(rdps,
					                 {rt4},
					                 dimensions,
					                 {},        // Parents
					                 {"lut"},
					                 {rhi::RenderpassTechnique::compute},
					                 {rhi::RenderpassType::lut},
					                 {rhi::RenderpassState::persistent},
					                 {{}},
					                 {{}},
					                 {0},
					                 {true});
					break;
				case 1:
					test_render_pass(rdps,
					                 {rt5},
					                 dimensions,
					                 {},        // Parents
					                 {"compute"},
					                 {rhi::RenderpassTechnique::compute},
					                 {rhi::RenderpassType::reflection},
					                 {rhi::RenderpassState::transient},
					                 {{}},
					                 {{}},
					                 {3},
					                 {false});
					break;
				case 2:
					test_render_pass(rdps,
					                 {rt0},
					                 dimensions,
					                 {},        // Parents
					                 {"light-bin"},
					                 {rhi::RenderpassTechnique::compute},
					                 {rhi::RenderpassType::light_bin},
					                 {rhi::RenderpassState::transient},
					                 {{}},
					                 {{}},
					                 {3},
					                 {false});
					break;
				case 3:
					test_render_pass(rdps,
					                 {rt0},
					                 dimensions,
					                 {},        // Parents
					                 {"main"},
					                 {rhi::RenderpassTechnique::fragment},
					                 {rhi::RenderpassType::main},
					                 {rhi::RenderpassState::transient},
					                 {{}},
					                 {{}},
					                 {3},
					                 {false});
					break;
				case 4:
					test_render_pass(rdps,
					                 {rt0},
					                 dimensions,
					                 {1},        // Parents
					                 {"shadowing"},
					                 {rhi::RenderpassTechnique::fragment},
					                 {rhi::RenderpassType::shadow},
					                 {rhi::RenderpassState::transient},
					                 {{}},
					                 {{}},
					                 {2},
					                 {true});
					break;
				case 5:
					test_render_pass(rdps,
					                 {rt0, rt3d},
					                 dimensions,
					                 {0, 1},        // Parents
					                 {"depth pre-pass", "g-buffer", "g-buffer-resolve-lighting", "render"},
					                 {rhi::RenderpassTechnique::fragment, rhi::RenderpassTechnique::fragment, rhi::RenderpassTechnique::fragment, rhi::RenderpassTechnique::fragment},
					                 {rhi::RenderpassType::depth, rhi::RenderpassType::deferred_gbuffer, rhi::RenderpassType::post_process, rhi::RenderpassType::main},
					                 {rhi::RenderpassState::transient, rhi::RenderpassState::transient, rhi::RenderpassState::transient, rhi::RenderpassState::transient},
					                 {{}, {}, {}, {}},
					                 iarfs5,
					                 {1, 1, 1, 0},
					                 {true, true, true, true});

					break;
				case 6:
					test_render_pass(rdps,
					                 {rt0, rt3d},
					                 dimensions,
					                 {0},        // Parents
					                 {"bloom"},
					                 {rhi::RenderpassTechnique::fragment},
					                 {rhi::RenderpassType::post_process},
					                 {rhi::RenderpassState::transient},
					                 {{}},
					                 iarfs67,
					                 {1},
					                 {false});
					break;
				case 7:
					test_render_pass(rdps,
					                 {rt0, rt3d},
					                 dimensions,
					                 {0},        // Parents
					                 {"tonemap"},
					                 {rhi::RenderpassTechnique::fragment},
					                 {rhi::RenderpassType::post_process},
					                 {rhi::RenderpassState::transient},
					                 {{}},
					                 iarfs67,
					                 {1},
					                 {false});
					break;
			}
			index++;
		}
	}

	// Test render targets
	auto &trgts = rdr.images();
	{
		{
			EXPECT_EQ(trgts[0].name(), "before_swapchain");
			EXPECT_EQ(trgts[0].format(), rhi::PixelFormat::b8g8r8a8_uint32_norm);
		}
		{
			EXPECT_EQ(trgts[1].name(), "renderable");
			EXPECT_EQ(trgts[1].format(), rhi::PixelFormat::r8g8b8a8_uint32_norm);
		}
		{
			EXPECT_EQ(trgts[2].name(), "shadow");
			EXPECT_EQ(trgts[2].format(), rhi::PixelFormat::depth32_float32);
		}
		{
			EXPECT_EQ(trgts[3].name(), "depth");
			EXPECT_EQ(trgts[3].format(), rhi::PixelFormat::depth32_float32);
			EXPECT_EQ(trgts[3].target(), rhi::TextureTarget::texture_2D);
		}
		{
			EXPECT_EQ(trgts[4].name(), "Texture1");
			EXPECT_EQ(trgts[4].format(), rhi::PixelFormat::r8_uint8);
		}
		{
			EXPECT_EQ(trgts[5].name(), "Texture2");
			EXPECT_EQ(trgts[5].format(), rhi::PixelFormat::r8_uint8);
		}
		{
			EXPECT_EQ(trgts[6].name(), "Depth_Stencil");
			EXPECT_EQ(trgts[6].format(), rhi::PixelFormat::depth24_norm_stencil8_uint32);
		}
		{
			EXPECT_EQ(trgts[7].name(), "Texture3");
			EXPECT_EQ(trgts[7].format(), rhi::PixelFormat::b5g6r5_uint16_norm);
			EXPECT_EQ(trgts[7].target(), rhi::TextureTarget::texture_cube);
		}
		{
			EXPECT_EQ(trgts[8].name(), "Texture4");
			EXPECT_EQ(trgts[8].format(), rhi::PixelFormat::r8g8b8a8_uint32);
		}
		{
			EXPECT_EQ(trgts[9].name(), "Texture6");
			EXPECT_EQ(trgts[9].format(), rhi::PixelFormat::r8g8b8a8_uint32_norm);
		}
		{
			EXPECT_EQ(trgts[10].name(), "Texture8");
			EXPECT_EQ(trgts[10].format(), rhi::PixelFormat::r8g8b8a8_uint32);
		}
		{
			EXPECT_EQ(trgts[11].name(), "Texture9");
			EXPECT_EQ(trgts[11].format(), rhi::PixelFormat::r8g8b8a8_int32);
		}
	}

	// Test render buffers
	auto &bufs = rdr.buffers();
	{
		(void) bufs;
	}

	// Test shaders
	auto &shdrs = rdr.shaders();

	auto ends_with = [](std::string &&value, const std::string &ending) {
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	};

	ASSERT_GT(shdrs.size(), 4);

	{
		EXPECT_TRUE(ends_with(shdrs[0].shader_path().filename(), "node_transform.glsl.comp"));
		EXPECT_TRUE(ends_with(shdrs[1].shader_path().filename(), "position.glsl.vert"));
		EXPECT_TRUE(ends_with(shdrs[2].shader_path().filename(), "position.glsl.frag"));
		EXPECT_TRUE(ends_with(shdrs[3].shader_path().filename(), "tonemap.glsl.vert"));
		EXPECT_TRUE(ends_with(shdrs[4].shader_path().filename(), "tonemap.glsl.frag"));
	}

	// Test programs
	auto &prg = rdr.programs();
	{
		ASSERT_GT(prg.size(), 1);
		{
			EXPECT_EQ(prg[0].compute_id(), 0);
		}
		{
			EXPECT_EQ(prg[1].vertex_id(), 1);
			EXPECT_EQ(prg[1].fragment_id(), 2);
		}
	}
}

}        // namespace ror_test
