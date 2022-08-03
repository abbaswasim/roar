#include "common.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector4.hpp"
#include "profiling/rorlog.hpp"
#include "renderer/rorrenderer.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>
#include <vector>

namespace ror_test
{

void test_render_targets(const std::vector<rhi::RenderTarget> a, std::vector<rhi::RenderTarget> b)
{
	EXPECT_EQ(a.size(), b.size());

	for (size_t i = 0; i < b.size(); ++i)
	{
		EXPECT_EQ(a[i].m_target_index, b[i].m_target_index);
		EXPECT_EQ(&a[i].m_target_reference.get(), &b[i].m_target_reference.get());
		EXPECT_EQ(a[i].m_load_action, b[i].m_load_action);
		EXPECT_EQ(a[i].m_store_action, b[i].m_store_action);
	}
}

void test_render_pass(rhi::Renderpass                       rdps,
                      std::vector<rhi::RenderTarget>        a_render_targets,
                      ror::Vector2ui                        a_dimensions,
                      ror::Vector4ui                        a_viewport,
                      std::vector<uint32_t>                 a_parents,
                      std::vector<std::string>              a_names,
                      std::vector<rhi::RenderpassTechnique> a_techniques,
                      std::vector<rhi::RenderpassType>      a_types,
                      std::vector<rhi::RenderpassState>     a_states,
                      std::vector<std::vector<uint32_t>>    a_rendered_inputs,
                      std::vector<std::vector<uint32_t>>    a_input_attachments,
                      std::vector<uint32_t>                 a_program_ids,
                      std::vector<bool>                     a_debug_outputs)
{
	auto dimensions     = rdps.dimensions();
	auto viewport       = rdps.viewport();
	auto parents        = rdps.parent_ids();
	auto render_targets = rdps.render_targets();

	test_vector2f_equal(dimensions, a_dimensions);
	test_vector4f_equal(viewport, a_viewport);

	EXPECT_EQ(parents.size(), a_parents.size());
	for (size_t i = 0; i < parents.size(); ++i)
		EXPECT_EQ(parents[i], a_parents[i]);

	test_render_targets(render_targets, a_render_targets);

	uint32_t iai = 0;
	for (auto rsps : rdps.subpasses())
	{
		auto name = rsps.name();
		ror::log_critical("Checking render pass {}", name.c_str());
		auto technique         = rsps.technique();
		auto type              = rsps.type();
		auto state             = rsps.state();
		auto program_id        = rsps.program_id();
		auto debug_output      = rsps.debug_output();
		auto input_attachments = rsps.input_attachment_ids();
		auto rendered_inputs   = rsps.rendered_input_ids();

		EXPECT_EQ(name, a_names[iai]);
		EXPECT_EQ(technique, a_techniques[iai]);
		EXPECT_EQ(type, a_types[iai]);
		EXPECT_EQ(state, a_states[iai]);
		EXPECT_EQ(program_id, a_program_ids[iai]);
		EXPECT_EQ(debug_output, a_debug_outputs[iai]);

		EXPECT_EQ(input_attachments.size(), a_input_attachments[iai].size());
		for (size_t i = 0; i < input_attachments.size(); ++i)
			EXPECT_EQ(input_attachments[i], a_input_attachments[iai][i]);

		EXPECT_EQ(rendered_inputs.size(), a_rendered_inputs[iai].size());
		for (size_t i = 0; i < rendered_inputs.size(); ++i)
			EXPECT_EQ(rendered_inputs[i], a_rendered_inputs[iai][i]);

		iai++;
	}
}

TEST(RendererTest, config_load)
{
	ror::Renderer rdr;

	auto &rtgs  = rdr.render_targets();
	auto &fgphs = rdr.frame_graphs();

	std::vector<std::vector<uint32_t>> empty_indices{};
	// Test forward
	{
		auto rdpses = fgphs["forward"];

		ror::Vector4ui viewport{0, 0, 1024, 768};
		ror::Vector2ui dimensions{1024, 768};

		uint32_t index = 0;
		for (auto rdps : rdpses)
		{
			switch (index)
			{
				case 0:
					test_render_pass(rdps,
					                 {{2, rtgs[2], rhi::LoadAction::clear, rhi::StoreAction::store}},
					                 dimensions,
					                 viewport,
					                 {},        // Parents
					                 {"shadowing"},
					                 {rhi::RenderpassTechnique::fragment},
					                 {rhi::RenderpassType::shadow},
					                 {rhi::RenderpassState::transient},
					                 {{}},
					                 {{}},
					                 {0},
					                 {true});
					break;
				case 1:
					test_render_pass(rdps,
					                 {{0, rtgs[0], rhi::LoadAction::clear, rhi::StoreAction::store}},
					                 dimensions,
					                 viewport,
					                 {0},        // Parents
					                 {"forward_lighting", "tonemap"},
					                 {rhi::RenderpassTechnique::fragment, rhi::RenderpassTechnique::fragment},
					                 {rhi::RenderpassType::forward_light, rhi::RenderpassType::tone_mapping},
					                 {rhi::RenderpassState::transient, rhi::RenderpassState::transient},
					                 {{2}, {}},
					                 {{}, {0}},
					                 {1, 0},
					                 {true, true});
					break;
			}
			index++;
		}
	}

	// Test deferred
	{
		auto rdpses = fgphs["deferred"];

		ror::Vector4ui viewport{0, 0, 1024, 768};
		ror::Vector2ui dimensions{1024, 768};

		uint32_t index = 0;
		for (auto rdps : rdpses)
		{
			switch (index)
			{
				case 0:
					test_render_pass(rdps,
					                 {{3, rtgs[3], rhi::LoadAction::clear, rhi::StoreAction::store}},
					                 dimensions,
					                 viewport,
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
					                 {{4, rtgs[4], rhi::LoadAction::clear, rhi::StoreAction::store}},
					                 dimensions,
					                 viewport,
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
					                 {{0, rtgs[0], rhi::LoadAction::clear, rhi::StoreAction::store}},
					                 dimensions,
					                 viewport,
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
					                 {{0, rtgs[0], rhi::LoadAction::clear, rhi::StoreAction::store}},
					                 dimensions,
					                 viewport,
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
					                 {{0, rtgs[0], rhi::LoadAction::clear, rhi::StoreAction::store}},
					                 dimensions,
					                 viewport,
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
					                 {{2, rtgs[2], rhi::LoadAction::clear, rhi::StoreAction::store}, {3, rtgs[3], rhi::LoadAction::clear, rhi::StoreAction::store}},
					                 dimensions,
					                 viewport,
					                 {0, 1},        // Parents
					                 {"depth pre-pass", "g-buffer", "g-buffer-resolve-lighting", "render"},
					                 {rhi::RenderpassTechnique::fragment, rhi::RenderpassTechnique::fragment, rhi::RenderpassTechnique::fragment, rhi::RenderpassTechnique::fragment},
					                 {rhi::RenderpassType::depth, rhi::RenderpassType::deferred_gbuffer, rhi::RenderpassType::post_process, rhi::RenderpassType::main},
					                 {rhi::RenderpassState::transient, rhi::RenderpassState::transient, rhi::RenderpassState::transient, rhi::RenderpassState::transient},
					                 {{}, {}, {}, {}},
					                 {{}, {0, 1}, {0, 1}, {0, 1}},
					                 {1, 1, 1, 0},
					                 {true, true, true, true});

					break;
				case 6:
					test_render_pass(rdps,
					                 {{2, rtgs[2], rhi::LoadAction::clear, rhi::StoreAction::store}, {3, rtgs[3], rhi::LoadAction::clear, rhi::StoreAction::store}},
					                 dimensions,
					                 viewport,
					                 {0},        // Parents
					                 {"bloom"},
					                 {rhi::RenderpassTechnique::fragment},
					                 {rhi::RenderpassType::post_process},
					                 {rhi::RenderpassState::transient},
					                 {{}},
					                 {{0}},
					                 {1},
					                 {false});
					break;
				case 7:
					test_render_pass(rdps,
					                 {{2, rtgs[2], rhi::LoadAction::clear, rhi::StoreAction::store}, {3, rtgs[3], rhi::LoadAction::clear, rhi::StoreAction::store}},
					                 dimensions,
					                 viewport,
					                 {0},        // Parents
					                 {"tonemap"},
					                 {rhi::RenderpassTechnique::fragment},
					                 {rhi::RenderpassType::post_process},
					                 {rhi::RenderpassState::transient},
					                 {{}},
					                 {{0}},
					                 {1},
					                 {false});
					break;
			}
			index++;
		}
	}

	// Test render targets
	auto &trgts = rdr.render_targets();
	{
		{
			EXPECT_EQ(trgts[0].name(), "swapchain");
			EXPECT_EQ(trgts[0].format(), rhi::PixelFormat::b8g8r8a8_uint32_norm_srgb);
		}
		{
			EXPECT_EQ(trgts[1].name(), "renderable");
			EXPECT_EQ(trgts[1].format(), rhi::PixelFormat::r8g8b8a8_uint32_norm);
		}
		{
			EXPECT_EQ(trgts[2].name(), "Shadow");
			EXPECT_EQ(trgts[2].format(), rhi::PixelFormat::depth32_float32);
		}
		{
			EXPECT_EQ(trgts[3].name(), "Texture1");
			EXPECT_EQ(trgts[3].format(), rhi::PixelFormat::r8_uint8);
			EXPECT_EQ(trgts[3].target(), rhi::TextureTarget::texture_2D);
		}
		{
			EXPECT_EQ(trgts[4].name(), "Texture2");
			EXPECT_EQ(trgts[4].format(), rhi::PixelFormat::r8_uint8);
		}
		{
			EXPECT_EQ(trgts[5].name(), "Depth_Stencil");
			EXPECT_EQ(trgts[5].format(), rhi::PixelFormat::depth24_norm_stencil8_uint32);
		}
		{
			EXPECT_EQ(trgts[6].name(), "Texture3");
			EXPECT_EQ(trgts[6].format(), rhi::PixelFormat::b5g6r5_uint16_norm);
			EXPECT_EQ(trgts[6].target(), rhi::TextureTarget::texture_cube);
		}
		{
			EXPECT_EQ(trgts[7].name(), "Texture4");
			EXPECT_EQ(trgts[7].format(), rhi::PixelFormat::r8g8b8a8_uint32);
		}
		{
			EXPECT_EQ(trgts[8].name(), "Texture6");
			EXPECT_EQ(trgts[8].format(), rhi::PixelFormat::r8g8b8a8_uint32_norm_srgb);
		}
		{
			EXPECT_EQ(trgts[9].name(), "Texture8");
			EXPECT_EQ(trgts[9].format(), rhi::PixelFormat::r8g8b8a8_uint32);
		}
		{
			EXPECT_EQ(trgts[10].name(), "Texture9");
			EXPECT_EQ(trgts[10].format(), rhi::PixelFormat::r8g8b8a8_int32);
		}
	}

	// Test render buffers
	auto &bufs = rdr.render_buffers();
	{
		(void) bufs;
	}

	// Test shaders
	auto &shdrs = rdr.shaders();

	auto ends_with = [](std::string &&value, const std::string &ending) {
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	};

	{
		EXPECT_TRUE(ends_with(shdrs[0].shader_path().filename(), "default_pbr.vert"));
		EXPECT_TRUE(ends_with(shdrs[1].shader_path().filename(), "default_pbr.frag"));
		EXPECT_TRUE(ends_with(shdrs[2].shader_path().filename(), "shadow.vert"));
		EXPECT_TRUE(ends_with(shdrs[3].shader_path().filename(), "shadow.frag"));
		EXPECT_TRUE(ends_with(shdrs[4].shader_path().filename(), "depth.vert"));
		EXPECT_TRUE(ends_with(shdrs[5].shader_path().filename(), "depth.frag"));
		EXPECT_TRUE(ends_with(shdrs[6].shader_path().filename(), "blur.kern"));
		EXPECT_TRUE(ends_with(shdrs[7].shader_path().filename(), "explode.comp"));
	}
	// Test programs
	auto &prg = rdr.programs();
	{
		{
			EXPECT_EQ(prg[0].vertex_id(), 0);
			EXPECT_EQ(prg[0].fragment_id(), 1);
		}
		{
			EXPECT_EQ(prg[1].vertex_id(), 2);
			EXPECT_EQ(prg[1].fragment_id(), 3);
		}
		{
			EXPECT_EQ(prg[2].compute_id(), 7);
		}
	}
}

}        // namespace ror_test
