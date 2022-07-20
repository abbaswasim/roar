
// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2022
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

#include "configuration/rorconfiguration.hpp"
#include "foundation/rormacros.hpp"
#include "math/rorvector2.hpp"
#include "profiling/rorlog.hpp"
#include "renderer/rorrenderer.hpp"
#include "rhi/crtp_interfaces/rorrenderpass.hpp"
#include "rhi/metal/rorrenderpass.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include <vector>

namespace ror
{

define_translation_unit_vtable(Renderer)
{}

rhi::TextureTarget string_to_texture_target(std::string a_target)
{
	if (a_target == "2D")
		return rhi::TextureTarget::texture_2D;
	else if (a_target == "3D")
		return rhi::TextureTarget::texture_3D;
	else if (a_target == "CUBE")
		return rhi::TextureTarget::texture_cube;
	else
		assert(0 && "Implement other texture targets support");

	return rhi::TextureTarget::texture_2D;
}

void Renderer::load_programs()
{
	if (this->m_json_file.contains("shaders"))
	{
		// Read all the shaders
		auto shaders = this->m_json_file["shaders"];
		for (auto &shader : shaders)
		{
			this->m_shaders.emplace_back(shader);
		}
	}

	if (this->m_json_file.contains("programs"))
	{
		auto programs = this->m_json_file["programs"];
		for (auto &program : programs)
		{
			if (program.contains("vertex"))
			{
				assert(program.contains("fragment") && "Program must contain both vertex and fragment ids");

				int32_t vid{-1};
				int32_t fid{-1};

				vid = program["vertex"];
				fid = program["fragment"];

				this->m_programs.emplace_back(vid, fid);
			}
			else if (program.contains("compute"))
			{
				assert(!program.contains("fragment") && !program.contains("vertex") && "Program can't have both compute and vertex,fragment ids");

				int32_t cid{-1};

				cid = program["compute"];
				this->m_programs.emplace_back(cid);
			}
		}
	}
}

void Renderer::load_render_targets()
{
	if (this->m_json_file.contains("render_targets"))
	{
		auto render_targets = this->m_json_file["render_targets"];
		for (auto &render_target : render_targets)
		{
			rhi::TextureImage texture;
			texture.push_empty_mip();

			assert(render_target.contains("format") && "Render_target must specifiy format");
			texture.format(rhi::string_to_pixel_format(render_target["format"]));

			// Not allocated render target, will be allocated when needed in render passes and properties set
			// texture.allocate();
			// texture.reset();
			// texture.width();
			// texture.height();
			// texture.depth(1u);
			// texture.bytes_per_pixel();

			if (render_target.contains("target"))
				texture.target(string_to_texture_target(render_target["target"]));

			if (render_target.contains("name"))
				texture.name(render_target["name"]);

			// TODO: test if this is causing copy elision issues
			this->m_render_targets.emplace_back(std::move(texture));
		}
	}
	else
	{
		ror::log_critical("Renderer config should contain render_targets description but found nothing");
	}
}

void Renderer::load_render_buffers()
{
	if (this->m_json_file.contains("render_buffers"))
	{
		auto render_buffers = this->m_json_file["render_buffers"];
		for (auto &render_buffer : render_buffers)
		{
			if (render_buffer.contains("name")) {}
			this->m_render_buffers.emplace_back();
		}
	}
}

rhi::RenderpassType string_to_renderpass_type(std::string a_type)
{
	// clang-format off
	if      (a_type == "lut")                return rhi::RenderpassType::lut;
	else if (a_type == "main")               return rhi::RenderpassType::main;
	else if (a_type == "depth")              return rhi::RenderpassType::depth;
	else if (a_type == "shadow")             return rhi::RenderpassType::shadow;
	else if (a_type == "light_bin")          return rhi::RenderpassType::light_bin;
	else if (a_type == "reflection")         return rhi::RenderpassType::reflection;
	else if (a_type == "refraction")         return rhi::RenderpassType::refraction;
	else if (a_type == "tone_mapping")       return rhi::RenderpassType::tone_mapping;
	else if (a_type == "forward_light")      return rhi::RenderpassType::forward_light;
	else if (a_type == "post_process")       return rhi::RenderpassType::post_process;
	else if (a_type == "deferred_gbuffer")   return rhi::RenderpassType::deferred_gbuffer;
	else if (a_type == "reflection_probes")  return rhi::RenderpassType::reflection_probes;
	else if (a_type == "image_based_light")  return rhi::RenderpassType::image_based_light;
	else if (a_type == "ambient_occlusion")  return rhi::RenderpassType::ambient_occlusion;
	else if (a_type == "deferred_clustered") return rhi::RenderpassType::deferred_clustered;
	// clang-format on

	assert(0);
	return rhi::RenderpassType::main;
}

void read_render_pass(json &a_render_pass, rhi::Renderpass& rp)
{
	if (a_render_pass.contains("name"))
		rp.name(a_render_pass["name"]);

	if (a_render_pass.contains("technique"))
	{
		rp.technique(a_render_pass["technique"] == "fragment" ? rhi::RenderpassTechnique::fragment : rhi::RenderpassTechnique::compute);
	}

	if (a_render_pass.contains("type"))
		rp.type(string_to_renderpass_type(a_render_pass["type"]));

	if (a_render_pass.contains("state"))
		rp.state(a_render_pass["state"] == "transient" ? rhi::RenderpassState::transient : rhi::RenderpassState::presistent);

	if (a_render_pass.contains("debug_output"))
		rp.debug_output(a_render_pass["debug_output"]);

	ror::Vector2ui dims = rp.dimensions();

	if (a_render_pass.contains("width"))
		dims.x = (a_render_pass["width"]);

	if (a_render_pass.contains("height"))
		dims.y = (a_render_pass["height"]);

	rp.dimensions(dims);

	if (a_render_pass.contains("viewport"))
	{
		auto vip = a_render_pass["viewport"];

		ror::Vector4i viewport;

		viewport.x = vip["x"];
		viewport.y = vip["y"];
		viewport.z = vip["w"];
		viewport.w = vip["h"];

		rp.viewport(viewport);
	}

	if (a_render_pass.contains("depends_on"))
	{
		auto parents = a_render_pass["depends_on"];
		rp.parents(parents);
	}

	if (a_render_pass.contains("program"))
		rp.program_id(a_render_pass["program"]);

	std::vector<rhi::Attachment<rhi::LoadAction>>  input_attachments{};
	std::vector<rhi::Attachment<rhi::StoreAction>> output_attachments{};

	if (a_render_pass.contains("inputs"))
	{
		auto inputs = a_render_pass["inputs"];
		for (auto &input : inputs)
		{
			assert(input.contains("index") && "Input must contain a render target/buffer index");

			rhi::Attachment<rhi::LoadAction> attachment;
			attachment.m_target_index = input["index"];

			if (input.contains("action"))
			{
				auto action = input["action"];
				if (action == "load")
					attachment.m_action = rhi::LoadAction::load;
				else if (action == "clear")
					attachment.m_action = rhi::LoadAction::clear;
				else if (action == "dont_care")
					attachment.m_action = rhi::LoadAction::dont_care;
				else
					assert(0);
			}

			input_attachments.emplace_back(std::move(attachment));
		}
	}
	assert(a_render_pass.contains("outputs") && "There must be some outputs specified in render passes");

	auto outputs = a_render_pass["outputs"];
	for (auto &output : outputs)
	{
		assert(output.contains("index") && "Input must contain a render target/buffer index");

		rhi::Attachment<rhi::StoreAction> attachment;
		attachment.m_target_index = output["index"];

		if (output.contains("action"))
		{
			auto action = output["action"];
			if (action == "store")
				attachment.m_action = rhi::StoreAction::store;
			else if (action == "discard")
				attachment.m_action = rhi::StoreAction::discard;
			else if (action == "dont_care")
				attachment.m_action = rhi::StoreAction::dont_care;
			else
				assert(0);
		}

		output_attachments.emplace_back(std::move(attachment));
	}

	rp.input_attachments(input_attachments);
	rp.output_attachments(output_attachments);
}

void Renderer::load_frame_graphs()
{
	assert(this->m_json_file.contains("frame_graph") && "There are no framegraphs provided, can't continue");
	auto frame_graph = this->m_json_file["frame_graph"];

	if (frame_graph.contains("width"))
	{
		this->m_dimensions.x = frame_graph["width"];
	}

	if (frame_graph.contains("height"))
	{
		this->m_dimensions.y = frame_graph["height"];
	}

	if (frame_graph.contains("viewport"))
	{
		auto viewport      = frame_graph["viewport"];
		this->m_viewport.x = viewport["x"];
		this->m_viewport.y = viewport["y"];
		this->m_viewport.z = viewport["w"];
		this->m_viewport.w = viewport["h"];
	}

	if (frame_graph.contains("forward"))
	{
		auto forward_passes = frame_graph["forward"];
		for (auto &forward_pass : forward_passes)
		{
			rhi::Renderpass rp;

			rp.viewport(this->m_viewport);
			rp.dimensions(this->m_dimensions);
			read_render_pass(forward_pass, rp);
			this->m_frame_graphs["forward"].emplace_back(std::move(rp));
		}
	}
	if (frame_graph.contains("deferred"))
	{
		auto deferred_passes = frame_graph["deferred"];
		for (auto &deferred_pass : deferred_passes)
		{
			rhi::Renderpass rp;

			rp.viewport(this->m_viewport);
			rp.dimensions(this->m_dimensions);
			read_render_pass(deferred_pass, rp);
			this->m_frame_graphs["deferred"].emplace_back(std::move(rp));
		}
	}

	if (frame_graph.contains("current"))
		this->m_current_frame_graph = &this->m_frame_graphs[frame_graph["current"]];
}

void Renderer::load_specific()
{
	// Order is important don't re-order
	this->load_programs();
	this->load_render_targets();
	this->load_render_buffers();
	this->load_frame_graphs();
}

}        // namespace ror
