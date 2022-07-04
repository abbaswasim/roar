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

#include "camera/rorcamera.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "graphics/rornode.hpp"
#include "graphics/rorscene.hpp"
#include "math/rorquaternion.hpp"
#include "math/rorvector3.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorhandles.hpp"
#include <array>
#include <ostream>
#include <sys/_types/_int32_t.h>
#include <type_traits>
#include <vector>

namespace ror
{

define_translation_unit_vtable(Scene)
{}

Scene::Scene(std::filesystem::path a_level)
{
	this->load(a_level, ResourceSemantic::scenes);
}

void Scene::render(const RenderDevice *a_rendering_device)
{
	(void) a_rendering_device;
}

void Scene::update(double64_t a_milli_seconds)
{
	(void) a_milli_seconds;
}

void Scene::load_models()
{
	for (auto &node : this->m_nodes_data)
	{
		Model model;
		model.load_from_gltf_file(node.m_model_path);
		this->m_models.emplace_back(std::move(model));
	}
}

void Scene::read_nodes()
{
	assert(this->m_json_file.contains("nodes") && "Provided scene file is not a roar scene.");

	// Read all the nodes
	auto nodes = this->m_json_file["nodes"];
	for (auto &node : nodes)
	{
		SceneNode     nod;
		SceneNodeData nod_data;

		nod_data.m_name = node["name"];

		if (node.contains("children"))
			nod_data.m_children = node["children"].get<std::vector<uint32_t>>();

		if (node.contains("translation"))
		{
			std::array<float32_t, 3> t = node["translation"];
			nod.m_trs_transform.translation({t[0], t[1], t[2]});
		}

		if (node.contains("rotation"))
		{
			std::array<float32_t, 4> r = node["rotation"];
			nod.m_trs_transform.rotation({r[0], r[1], r[2], r[3]});
		}

		if (node.contains("scale"))
		{
			std::array<float32_t, 3> s = node["scale"];
			nod.m_trs_transform.scale({s[0], s[1], s[2]});
		}

		if (node.contains("path"))
			nod_data.m_model_path = node["path"];

		if (node.contains("shader"))
			nod_data.m_program_id = node["shader"];

		if (node.contains("bvh"))
			nod_data.m_has_bvh = node["bvh"];

		if (node.contains("light"))
			nod_data.m_light_id = node["light"];

		if (node.contains("camera"))
			nod_data.m_light_id = node["camera"];

		if (node.contains("particle_emitter"))
			nod_data.m_particle_id = node["particle_emitter"];

		this->m_nodes.emplace_back(std::move(nod));
		this->m_nodes_data.emplace_back(std::move(nod_data));
	}

	// Now lets since all nodes are loaded lets set the parent ids
	for (size_t i = 0; i < this->m_nodes_data.size(); ++i)
	{
		auto &node_data = this->m_nodes_data[i];
		for (auto &child : node_data.m_children)
		{
			auto &node    = this->m_nodes[child];
			node.m_parent = static_cast<int32_t>(i);
		}
	}
}

void Scene::read_cameras()
{
	if (this->m_json_file.contains("cameras"))
	{
		// Read all the cameras
		auto cameras = this->m_json_file["cameras"];
		for (auto &camera : cameras)
		{
			OrbitCamera cam;
			auto        cam_type = camera["type"];

			if (cam_type == "perspective")
			{
				cam.type(CameraType::perspective);
				if (camera.contains("perspective"))
				{
					auto cam_args = camera["perspective"];

					for (auto &[key, value] : cam_args.items())
					{
						if (key == "aspectRatio")
						{
							cam.aspect_ratio(value);
						}
						if (key == "yfov")
						{
							cam.y_fov(value);
						}
						if (key == "znear")
						{
							cam.z_near(value);
						}
						if (key == "zfar")
						{
							cam.z_far(value);
						}
					}
				}
				if (camera.contains("orthographic"))
				{
					auto cam_args = camera["orthographic"];

					for (auto &[key, value] : cam_args.items())
					{
						if (key == "xmag")
						{
							cam.width(value);
						}
						if (key == "ymag")
						{
							cam.height(value);
						}
						if (key == "znear")
						{
							cam.z_near(value);
						}
						if (key == "zfar")
						{
							cam.z_far(value);
						}
					}
				}
			}
			else
			{
				cam.type(CameraType::orthographic);
			}

			this->m_cameras.emplace_back(std::move(cam));
		}
	}
	else
	{
		log_info("No cameras founds in the scene, will assign default one");
		OrbitCamera cam;
		this->m_cameras.emplace_back(std::move(cam));
	}
}

void Scene::read_lights()
{
	if (this->m_json_file.contains("lights"))
	{
		// Read all the lights
		auto lights = this->m_json_file["lights"];
		for (auto &light : lights)
		{
			Light lit;
			auto  type = light["type"];

			if (type == "directional")
				lit.m_type = Light::LightType::directional;
			else if (type == "point")
				lit.m_type = Light::LightType::point;
			else if (type == "spot")
				lit.m_type = Light::LightType::spot;
			else if (type == "area")
				lit.m_type = Light::LightType::area;

			if (light.contains("color"))
			{
				std::array<float32_t, 3> c = light["color"];

				lit.m_color.x = c[0];
				lit.m_color.y = c[1];
				lit.m_color.z = c[2];
			}

			if (light.contains("intensity"))
				lit.m_intensity = light["intensity"];

			if (light.contains("innerAngle"))
				lit.m_innerAngle = light["innerAngle"];

			if (light.contains("outerAngle"))
				lit.m_outerAngle = light["outerAngle"];

			if (light.contains("range"))
				lit.m_range = light["range"];

			this->m_lights.emplace_back(std::move(lit));
		}
	}
}

void Scene::read_programs()
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
			(void) program;

			int32_t vid{-1};
			int32_t fid{-1};

			if (program.contains("vertex"))
			{
				assert(program.contains("fragment") && "Program must contain both vertex and fragment ids");

				vid = program["vertex"];
				fid = program["fragment"];
			}

			this->m_programs.emplace_back(vid, fid);
		}
	}
}

void Scene::load_specific()
{
	this->read_nodes();
	this->read_cameras();
	this->read_lights();
	this->read_programs();
}

void Scene::unload()
{}

}        // namespace ror
