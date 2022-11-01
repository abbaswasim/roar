
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
#include "foundation/rorhash.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector4.hpp"
#include "profiling/rorlog.hpp"
#include "renderer/rorrenderer.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "rhi/rorshader_input.hpp"
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
			auto      s_path = std::filesystem::path(shader);
			auto      type   = rhi::string_to_shader_type(s_path.extension());
			hash_64_t hash   = hash_64(s_path.c_str(), s_path.string().length());
			this->m_shaders.emplace_back(shader, hash, type, ror::ResourceAction::load);
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

				int32_t vid = program["vertex"];
				int32_t fid = program["fragment"];

				this->m_programs.emplace_back(vid, fid);
			}
			else if (program.contains("compute"))
			{
				assert(!program.contains("fragment") && !program.contains("vertex") && "Program can't have both compute and vertex,fragment ids");

				int32_t cid = program["compute"];

				this->m_programs.emplace_back(cid);
			}
		}
	}
}

void Renderer::load_textures()
{
	if (this->m_json_file.contains("textures"))
	{
		auto textures = this->m_json_file["textures"];
		for (auto &texture : textures)
		{
			rhi::TextureImage texture_image;
			texture_image.push_empty_mip();

			assert(texture.contains("format") && "Texture must specifiy format");
			texture_image.format(rhi::string_to_pixel_format(texture["format"]));
			texture_image.usage(rhi::TextureUsage::render_target);        // Is also changed/updated later in reading framegraphs

			// Not allocated render target, will be allocated when needed in render passes and properties set
			// texture.allocate();
			// texture.reset();
			// texture.width();
			// texture.height();
			// texture.depth(1u);
			// texture.bytes_per_pixel();

			if (texture.contains("target"))
				texture_image.target(string_to_texture_target(texture["target"]));

			if (texture.contains("name"))
				texture_image.name(texture["name"]);

			this->m_textures.emplace_back(std::move(texture_image));
		}
	}
	else
	{
		ror::log_critical("Renderer config should contain textures description but found nothing");
	}
}

void Renderer::load_buffers()
{
	if (this->m_json_file.contains("buffers"))
	{
		auto buffers = this->m_json_file["buffers"];
		for (auto &buffer : buffers)
		{
			// Unused properties name and format
			if (buffer.contains("name")) {}
			if (buffer.contains("format")) {}

			this->m_buffers.emplace_back();
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
	else if (a_type == "pre_process")        return rhi::RenderpassType::pre_process;
	else if (a_type == "post_process")       return rhi::RenderpassType::post_process;
	else if (a_type == "tone_mapping")       return rhi::RenderpassType::tone_mapping;
	else if (a_type == "forward_light")      return rhi::RenderpassType::forward_light;
	else if (a_type == "node_transform")      return rhi::RenderpassType::node_transform;
	else if (a_type == "deferred_gbuffer")   return rhi::RenderpassType::deferred_gbuffer;
	else if (a_type == "reflection_probes")  return rhi::RenderpassType::reflection_probes;
	else if (a_type == "image_based_light")  return rhi::RenderpassType::image_based_light;
	else if (a_type == "ambient_occlusion")  return rhi::RenderpassType::ambient_occlusion;
	else if (a_type == "skeletal_transform")  return rhi::RenderpassType::skeletal_transform;
	else if (a_type == "deferred_clustered") return rhi::RenderpassType::deferred_clustered;
	// clang-format on

	assert(0);
	return rhi::RenderpassType::main;
}

rhi::LoadAction to_load_action(nlohmann::json a_loadaction)
{
	rhi::LoadAction load_action;
	if (a_loadaction == "load")
		load_action = rhi::LoadAction::load;
	else if (a_loadaction == "clear")
		load_action = rhi::LoadAction::clear;
	else if (a_loadaction == "dont_care")
		load_action = rhi::LoadAction::dont_care;
	else
		assert(0 && "Invalid load/store action string provided");

	return load_action;
}

rhi::StoreAction to_store_action(nlohmann::json a_storeaction)
{
	rhi::StoreAction store_action;
	if (a_storeaction == "store")
		store_action = rhi::StoreAction::store;
	else if (a_storeaction == "discard")
		store_action = rhi::StoreAction::discard;
	else if (a_storeaction == "dont_care")
		store_action = rhi::StoreAction::dont_care;
	else
		assert(0 && "Invalid store action string provided");

	return store_action;
}

void read_render_pass(json &a_render_pass, std::vector<rhi::Renderpass> &a_frame_graph, ror::Vector4i a_viewport, ror::Vector2ui a_dimensions,
                      std::vector<rhi::TextureImage>        &a_textures,
                      std::vector<rhi::Buffer<rhi::Static>> &a_buffers)
{
	rhi::Renderpass render_pass;

	if (a_render_pass.contains("disabled"))
	{
		auto disabled = a_render_pass["disabled"];
		render_pass.enabled(!disabled);
	}

	render_pass.viewport(a_viewport);
	render_pass.dimensions(a_dimensions);

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

	assert((a_render_pass.contains("render_targets") || a_render_pass.contains("render_buffers")) && "Render pass must have render targets");

	if (a_render_pass.contains("render_targets"))
	{
		auto render_targets = a_render_pass["render_targets"];

		std::vector<rhi::RenderTarget> rts;

		for (auto &rt : render_targets)
		{
			assert(rt.contains("index") && rt.contains("load_action") && rt.contains("store_action") && "Render Target must contain all index, load and store actions");

			uint32_t index       = rt["index"];
			auto     loadaction  = rt["load_action"];
			auto     storeaction = rt["store_action"];

			rhi::LoadAction  load_action  = to_load_action(loadaction);
			rhi::StoreAction store_action = to_store_action(storeaction);

			// Emplaces a RenderTarget
			assert(index < a_textures.size() && "Index is out of bound for render targets provided");

			rts.emplace_back(index, a_textures[index], load_action, store_action);
		}

		render_pass.render_targets(std::move(rts));
	}

	if (a_render_pass.contains("render_buffers"))
	{
		auto render_buffers = a_render_pass["render_buffers"];

		std::vector<rhi::RenderBuffer> rbs;

		for (auto &rb : render_buffers)
		{
			assert(rb.contains("index") && rb.contains("load_action") && rb.contains("store_action") && "Render Buffer must contain all index, load and store actions");

			uint32_t index       = rb["index"];
			auto     loadaction  = rb["load_action"];
			auto     storeaction = rb["store_action"];

			rhi::LoadAction  load_action  = to_load_action(loadaction);
			rhi::StoreAction store_action = to_store_action(storeaction);

			// Emplaces a RenderTarget
			assert(index < a_buffers.size() && "Index is out of bound for render targets provided");

			rbs.emplace_back(index, a_buffers[index], load_action, store_action);
		}

		render_pass.render_buffers(std::move(rbs));
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
				assert(rt_index < a_textures.size() && "This rendered input doesn't exist in the render targets");
				a_textures[rt_index].usage(rhi::TextureUsage::render_target_read);
			}
			rsp.rendered_input_ids(std::move(rendered_inputs));
		}

		if (subpass.contains("buffer_inputs"))
		{
			std::vector<uint32_t> buffer_inputs = subpass["buffer_inputs"];
			for (auto &bi_index : buffer_inputs)
			{
				assert(bi_index < a_buffers.size() && "This buffer input doesn't exist in the buffer targets");
				// a_render_buffers[bi_index].usage(rhi::TextureUsage::render_target_read);
			}
			rsp.buffer_input_ids(std::move(buffer_inputs));
		}

		if (subpass.contains("subpass_inputs"))
		{
			std::vector<uint32_t> subpass_inputs = subpass["subpass_inputs"];
			for (auto &rt_index : subpass_inputs)
			{
				assert(rt_index < a_textures.size() && "This subpass input doesn't exist in the render targets");
				a_textures[rt_index].usage(rhi::TextureUsage::render_target_read);
			}
			rsp.input_attachment_ids(std::move(subpass_inputs));
		}

		rsps.emplace_back(std::move(rsp));
	}

	render_pass.subpasses(std::move(rsps));

	a_frame_graph.emplace_back(std::move(render_pass));
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
			read_render_pass(forward_pass, this->m_frame_graphs["forward"], this->m_viewport, this->m_dimensions, this->m_textures, this->m_buffers);
		}
	}

	if (frame_graph.contains("deferred"))
	{
		auto deferred_passes = frame_graph["deferred"];
		for (auto &deferred_pass : deferred_passes)
		{
			read_render_pass(deferred_pass, this->m_frame_graphs["deferred"], this->m_viewport, this->m_dimensions, this->m_textures, this->m_buffers);
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

std::reference_wrapper<const rhi::RenderTarget> Renderer::find_rendertarget_reference(const std::vector<rhi::Renderpass> &a_renderpasses, uint32_t a_index)
{
	for (auto &pass : a_renderpasses)
	{
		for (auto &rts : pass.render_targets())
		{
			if (a_index == rts.m_target_index)
				return std::ref(rts);
		}
	}

	// If we don't have this render target lets create one
	assert(a_index < this->m_textures.size() && "Index is out of bound in the textures array");
	rhi::LoadAction  load_action{rhi::LoadAction::clear};
	rhi::StoreAction store_action{rhi::StoreAction::store};

	if (this->m_input_render_targets.size() == 0)
		this->m_input_render_targets.reserve(20);        // Should be enough otherwise an error will happen which I will know about

	this->m_input_render_targets.emplace_back(a_index, this->m_textures[a_index], load_action, store_action);

	return std::ref(this->m_input_render_targets.back());        // back is ok here because this vector can't be reallocated
}

std::reference_wrapper<const rhi::RenderBuffer> Renderer::find_renderbuffer_reference(const std::vector<rhi::Renderpass> &a_renderpasses, uint32_t a_index)
{
	for (auto &pass : a_renderpasses)
	{
		for (auto &rts : pass.render_buffers())
		{
			if (a_index == rts.m_target_index)
				return std::ref(rts);
		}
	}

	// If we don't have this render buffers lets create one
	assert(a_index < this->m_buffers.size() && "Index is out of bound in the render buffers array");
	rhi::LoadAction  load_action{rhi::LoadAction::clear};
	rhi::StoreAction store_action{rhi::StoreAction::store};

	if (this->m_input_render_buffers.size() == 0)
		this->m_input_render_buffers.reserve(20);        // Should be enough otherwise an error will happen which I will know about

	this->m_input_render_buffers.emplace_back(a_index, this->m_buffers[a_index], load_action, store_action);

	return std::ref(this->m_input_render_buffers.back());        // back is ok here because this vector can't be reallocated
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
				{
					assert(pid < graph.second.size() && "Parent Id is out of bound");
					ps.emplace_back(std::ref(graph.second[pid]));
				}

				pass.parents(std::move(ps));
			}

			for (auto &subpass : pass.subpasses())
			{
				// All the render input ids into render input references
				{
					auto &rids = subpass.rendered_input_ids();

					rhi::Rendersubpass::RenderTargets rts{};

					assert(rids.size() <= this->m_textures.size() && "Rendered Ids and number of render targets in the renderer doesn't match");

					for (auto rid : rids)
					{
						assert(rid < this->m_textures.size() && "Render input Id is out of bound");
						rts.emplace_back(find_rendertarget_reference(graph.second, rid));
					}

					subpass.rendered_inputs(std::move(rts));
				}
				// All the subpass input ids into subpass input references
				{
					auto &iads = subpass.input_attachment_ids();

					rhi::Rendersubpass::RenderTargets rsps{};

					assert(iads.size() <= this->m_textures.size() && "Subpass input attachment Ids and number of subpasses in this render pass doesn't match");

					for (auto rid : iads)
					{
						assert(rid < this->m_textures.size() && "Input attachment Id is out of bound");
						rsps.emplace_back(find_rendertarget_reference(graph.second, rid));
					}

					subpass.input_attachments(std::move(rsps));
				}
				// All the buffer input ids into buffer input references
				{
					auto &biid = subpass.buffer_input_ids();

					rhi::Rendersubpass::BufferTargets bfts{};

					for (auto bid : biid)
					{
						assert(bid < this->m_buffers.size() && "Input attachment Id is out of bound");
						bfts.emplace_back(find_renderbuffer_reference(graph.second, bid));
					}

					subpass.buffer_inputs(std::move(bfts));
				}
			}
		}
	}
}

void Renderer::load_specific()
{
	// Order is important don't re-order
	this->load_programs();
	this->load_textures();
	this->load_buffers();
	this->load_frame_graphs();
	this->setup_references();
}

void Renderer::upload(rhi::Device &a_device)
{
	for (auto &shader : this->m_shaders)
	{
		shader.compile();
		shader.upload(a_device);
	}

	for (auto &program : this->m_programs)
		program.upload(a_device, this->m_shaders);

	for (auto &render_target : this->m_input_render_targets)
		render_target.m_target_reference.get().upload(a_device);

	for (auto &render_buffer : this->m_input_render_buffers)
		render_buffer.m_target_reference.get().upload(a_device);

	for (auto &graph : this->m_frame_graphs)
	{
		for (auto &pass : graph.second)
		{
			// Prepare render targets
			auto pass_render_targets = pass.render_targets();
			for (auto &render_target : pass_render_targets)
			{
				auto &texture = render_target.m_target_reference.get();
				if (!texture.ready())
				{
					texture.width(pass.dimensions().x);
					texture.height(pass.dimensions().y);
					texture.upload(a_device);        // Doesn't necessarily upload a texture to the GPU but creates the texture in the GPU for later use
				}
				else
				{
					if ((texture.width() != pass.dimensions().x) || (texture.height() != pass.dimensions().y))
						ror::log_critical("Reusing render target with different dimensions");
				}
			}

			auto pass_render_buffers = pass.render_buffers();
			for (auto &render_buffer : pass_render_buffers)
			{
				auto &buffer = render_buffer.m_target_reference.get();
				if (!buffer.ready())
				{
					buffer.upload(a_device);        // Creates a gpu buffer with 1 byte size, which can be released and uploaded later with proper size
				}
				else
				{
					ror::log_critical("Reusing buffer target with different size");
				}
			}

			pass.upload(a_device);
		}

		// Make sure all the rendered_inputs and buffer_inputs are ready
		for (auto &pass : graph.second)
		{
			auto pass_render_targets = pass.render_targets();
			auto pass_render_buffers = pass.render_buffers();

			for (auto &render_target : pass_render_targets)
			{
				auto &texture = render_target.m_target_reference.get();
				(void) texture;
				assert(texture.ready() && "Required textures are not ready");
			}
			for (auto &render_buffer : pass_render_buffers)
			{
				auto &buffer = render_buffer.m_target_reference.get();
				(void) buffer;
				assert(buffer.ready() && "Required buffers are not ready");
			}
		}
	}

	this->m_render_state.upload(a_device);
}

std::vector<rhi::RenderpassType> Renderer::render_pass_types(const std::vector<rhi::Renderpass> &a_pass) const
{
	std::vector<rhi::RenderpassType> passes;
	for (auto &rp : a_pass)
	{
		const auto &subpasses = rp.subpasses();
		for (auto &srp : subpasses)
		{
			passes.emplace_back(srp.type());
		}
	}

	return passes;
}

std::vector<rhi::RenderpassType> Renderer::render_pass_types() const
{
	assert(this->m_current_frame_graph);
	return this->render_pass_types(*this->m_current_frame_graph);
}

std::vector<rhi::RenderpassType> Renderer::all_render_pass_types() const
{
	std::vector<rhi::RenderpassType> passes;
	for (const auto &rps : this->m_frame_graphs)
	{
		const auto rpees = this->render_pass_types(rps.second);
		passes.insert(passes.end(), std::make_move_iterator(rpees.begin()), std::make_move_iterator(rpees.end()));
	}

	return passes;
}

}        // namespace ror
