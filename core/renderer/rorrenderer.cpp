
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
#include "resources/rorresource.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include <cassert>
#include <filesystem>
#include <string>
#include <vector>

namespace ror
{

define_translation_unit_vtable(Renderer)
{}

rhi::TextureTarget string_to_texture_target(const std::string &a_target)
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
		std::vector<std::string> shaders = this->m_json_file["shaders"];
		for (auto &shader : shaders)
		{
			auto s_path = std::filesystem::path(shader);
			auto type = rhi::string_to_shader_type(s_path.extension());

			this->m_shaders.emplace_back(shader, type, ror::ResourceAction::load);
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
			texture.usage(rhi::TextureUsage::render_target);        // Is also changed/updated later in reading framegraphs

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
			// TODO: Add implementation here
			if (render_buffer.contains("name")) {}
			this->m_render_buffers.emplace_back();
		}
	}
}

rhi::RenderpassType string_to_renderpass_type(const std::string &a_type)
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

void read_render_pass(json &a_render_pass, rhi::Renderpass &render_pass, std::vector<rhi::TextureImage> &a_render_targets)
{
	ror::Vector2ui dims = render_pass.dimensions();

	if (a_render_pass.contains("width"))
		dims.x = (a_render_pass["width"]);

	if (a_render_pass.contains("height"))
		dims.y = (a_render_pass["height"]);

	render_pass.dimensions(dims);

	if (a_render_pass.contains("viewport"))
	{
		auto vip = a_render_pass["viewport"];

		ror::Vector4i viewport;

		viewport.x = vip["x"];
		viewport.y = vip["y"];
		viewport.z = vip["w"];
		viewport.w = vip["h"];

		render_pass.viewport(viewport);
	}

	if (a_render_pass.contains("depends_on"))
	{
		auto parents = a_render_pass["depends_on"];
		render_pass.parent_ids(parents);
	}

	if (a_render_pass.contains("background"))
	{
		auto color = a_render_pass["background"];
		assert(color.size() == 4 && "Renderpass background color is not correctly defined");
		render_pass.background({color[0], color[1], color[2], color[3]});
	}

	assert(a_render_pass.contains("render_targets") && "Render pass must have render targets");

	{
		auto render_targets = a_render_pass["render_targets"];

		std::vector<rhi::RenderTarget> rts;

		for (auto &rt : render_targets)
		{
			assert(rt.contains("index") && rt.contains("load_action") && rt.contains("store_action") && "Render Target must contain all index, load and store actions");

			uint32_t index       = rt["index"];
			auto     loadaction  = rt["load_action"];
			auto     storeaction = rt["store_action"];

			rhi::LoadAction  load_action{rhi::LoadAction::dont_care};
			rhi::StoreAction store_action{rhi::StoreAction::dont_care};

			if (loadaction == "load")
				load_action = rhi::LoadAction::load;
			else if (loadaction == "clear")
				load_action = rhi::LoadAction::clear;
			else if (loadaction == "dont_care")
				load_action = rhi::LoadAction::dont_care;
			else
				assert(0 && "Invalid load action string provided");

			if (storeaction == "store")
				store_action = rhi::StoreAction::store;
			else if (storeaction == "discard")
				store_action = rhi::StoreAction::discard;
			else if (storeaction == "dont_care")
				store_action = rhi::StoreAction::dont_care;
			else
				assert(0 && "Invalid store action string provided");

			// Emplaces a RenderTarget
			assert(index < a_render_targets.size() && "Index is out of bound for render targets provided");

			rts.emplace_back(index, a_render_targets[index], load_action, store_action);
		}

		render_pass.render_targets(std::move(rts));
	}

	assert(a_render_pass.contains("subpasses") && "There must be atleast one subpass in a render pass");

	auto subpasses = a_render_pass["subpasses"];

	std::vector<rhi::Rendersubpass> rsps;
	for (auto &subpass : subpasses)
	{
		rhi::Rendersubpass rsp{};

		if (subpass.contains("name"))
			rsp.name(subpass["name"]);

		if (subpass.contains("technique"))
		{
			rsp.technique(subpass["technique"] == "fragment" ? rhi::RenderpassTechnique::fragment : rhi::RenderpassTechnique::compute);
		}

		if (subpass.contains("type"))
			rsp.type(string_to_renderpass_type(subpass["type"]));

		if (subpass.contains("state"))
			rsp.state(subpass["state"] == "transient" ? rhi::RenderpassState::transient : rhi::RenderpassState::persistent);

		if (subpass.contains("debug_output"))
			rsp.debug_output(subpass["debug_output"]);

		if (subpass.contains("program"))
			rsp.program_id(subpass["program"]);

		std::vector<rhi::RenderTarget> input_attachments{};
		std::vector<rhi::RenderTarget> output_attachments{};

		if (subpass.contains("rendered_inputs"))
		{
			std::vector<uint32_t> rendered_inputs = subpass["rendered_inputs"];
			for (auto &rt_index : rendered_inputs)
			{
				assert(rt_index < a_render_targets.size() && "This rendered input doesn't exist in the render targets");
				a_render_targets[rt_index].usage(rhi::TextureUsage::render_target_read);
			}
			rsp.rendered_input_ids(std::move(rendered_inputs));
		}

		if (subpass.contains("subpass_inputs"))
		{
			std::vector<uint32_t> subpass_inputs = subpass["subpass_inputs"];
			for (auto &rt_index : subpass_inputs)
			{
				assert(rt_index < a_render_targets.size() && "This subpass input doesn't exist in the render targets");
				a_render_targets[rt_index].usage(rhi::TextureUsage::render_target_read);
			}
			rsp.input_attachment_ids(std::move(subpass_inputs));
		}

		rsps.emplace_back(std::move(rsp));
	}

	render_pass.subpasses(std::move(rsps));
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

	assert((frame_graph.contains("forward") || frame_graph.contains("deferred")) && "There must be at least one frame graph provided");

	if (frame_graph.contains("forward"))
	{
		auto forward_passes = frame_graph["forward"];
		for (auto &forward_pass : forward_passes)
		{
			rhi::Renderpass rp;

			rp.viewport(this->m_viewport);
			rp.dimensions(this->m_dimensions);
			read_render_pass(forward_pass, rp, this->m_render_targets);
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
			read_render_pass(deferred_pass, rp, this->m_render_targets);
			this->m_frame_graphs["deferred"].emplace_back(std::move(rp));
		}
	}

	if (frame_graph.contains("current"))
		this->m_current_frame_graph = &this->m_frame_graphs[frame_graph["current"]];
	else
	{
		if (frame_graph.contains("forward"))
			this->m_current_frame_graph = &this->m_frame_graphs["forward"];
		else
			this->m_current_frame_graph = &this->m_frame_graphs["deferred"];
	}
}

void Renderer::setup_references()
{
	// Lets setup all the refrences, we have to do this last because hopefully everything is loaded by now
	// All reference are set here except RenderTargets in subpasses which are done earlier
	for (auto &graph : this->m_frame_graphs)
	{
		for (auto &pass : graph.second)
		{
			// All the parent_ids into parents (refrences)
			{
				auto &pids = pass.parent_ids();

				rhi::Renderpass::Renderpasses ps{};

				assert(pids.size() <= graph.second.size() && "Parent Ids and number of render passes in this graph doesn't match");

				for (auto pid : pids)
					ps.emplace_back(std::ref(graph.second[pid]));

				pass.parents(std::move(ps));
			}

			for (auto &subpass : pass.subpasses())
			{
				// All the render input ids into render input references
				{
					auto &rids = subpass.rendered_input_ids();

					rhi::Rendersubpass::RenderTargets rts{};

					assert(rids.size() <= pass.render_targets().size() && "Rendered Ids and number of render targets in the renderer doesn't match");

					for (auto rid : rids)
						rts.emplace_back(std::ref(pass.render_targets()[rid]));

					subpass.rendered_inputs(std::move(rts));
				}
				// All the subpass input ids into subpass input references
				{
					auto &iads = subpass.input_attachment_ids();

					rhi::Rendersubpass::Rendersubpasses rsps{};

					assert(iads.size() <= pass.subpasses().size() && "Subpass input attachment Ids and number of subpasses in this render pass doesn't match");

					for (auto rid : iads)
						rsps.emplace_back(std::ref(pass.subpasses()[rid]));

					subpass.input_attachments(std::move(rsps));
				}
			}
		}
	}
}

void Renderer::load_specific()
{
	// Order is important don't re-order
	this->load_programs();
	this->load_render_targets();
	this->load_render_buffers();
	this->load_frame_graphs();
	this->setup_references();
}

void Renderer::upload(rhi::Device &a_device)
{
	for (auto &shader : this->m_shaders)
		shader.upload(a_device);

	for (auto &program : this->m_programs)
		program.upload();

	// TODO: Fix uploads in the buffer
	// for (auto &render_buffer : this->m_render_buffers)
	//     render_buffer.upload();

	for (auto &graph : this->m_frame_graphs)
	{
		for (auto &pass : graph.second)
		{
			auto this_m_render_targets = pass.render_targets();
			for (auto &render_target : this_m_render_targets)
			{
				auto &texture = render_target.m_target_reference.get();
				texture.width(pass.dimensions().x);
				texture.height(pass.dimensions().y);
				texture.upload(a_device);        // Doesn't necessarily upload a texture to the GPU but creates the texture in the GPU for later use
			}

			pass.upload(a_device);
		}
	}
}

std::vector<rhi::RenderpassType> Renderer::render_passes()
{
	assert(this->m_current_frame_graph);
	return this->render_passes(*this->m_current_frame_graph);
}

std::vector<rhi::RenderpassType> Renderer::all_render_passes()
{
	std::vector<rhi::RenderpassType> passes;
	for (auto &rps : this->m_frame_graphs)
	{
		auto rpees = this->render_passes(rps.second);
		passes.insert(passes.end(), std::make_move_iterator(rpees.begin()), std::make_move_iterator(rpees.end()));
	}

	return passes;
}
}        // namespace ror
