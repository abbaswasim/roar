// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2026
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

#include "graphics/rorline_soup.hpp"
#include "rorshadow_data.hpp"

namespace ror
{

//--------------------------------------------------------------------------------------
// Copied from https://github.com/walbourn/directx-sdk-samples-reworked/blob/main/CascadedShadowMaps11/CascadedShadowMaps11.cpp to find a scene + view frustum fit
// Used to compute an intersection of the orthographic projection and the Scene AABB
//--------------------------------------------------------------------------------------
struct TriangleT
{
	ror::Vector3f pt[3];
	bool          culled;
};
//--------------------------------------------------------------------------------------
// Computing an accurate near and flar plane will decrease surface acne and Peter-panning.
// Surface acne is the term for erroneous self shadowing.  Peter-panning is the effect where
// shadows disappear near the base of an object.
// As offsets are generally used with PCF filtering due self shadowing issues, computing the
// correct near and far planes becomes even more important.
// This concept is not complicated, but the intersection code is.
// camera_bounds_min_ls Is frustum AABB min in light space
// camera_bounds_max_ls Is frustum AABB max in light space
// a_scene_points_ls    Is all point of Scene AABB in light space
//--------------------------------------------------------------------------------------
void compute_near_and_far(float32_t                   &a_near_plane,
                                 float32_t                   &a_far_plane,
                                 ror::Vector3f                camera_bounds_min_ls,
                                 ror::Vector3f                camera_bounds_max_ls,
                                 std::array<ror::Vector3f, 8> a_scene_bbox_points_ls)
{
	// Initialize the near and far planes
	a_near_plane = FLT_MAX;
	a_far_plane  = -FLT_MAX;

	TriangleT triangleList[16];
	int32_t  iTriangleCnt = 1;

	triangleList[0].pt[0]  = a_scene_bbox_points_ls[0];
	triangleList[0].pt[1]  = a_scene_bbox_points_ls[1];
	triangleList[0].pt[2]  = a_scene_bbox_points_ls[2];
	triangleList[0].culled = false;

	// These are the indices used to tesselate an AABB into a list of triangles.
	static const int32_t iAABBTriIndexes[] =
	    {
	        0, 1, 2, 1, 2, 3,
	        4, 5, 6, 5, 6, 7,
	        0, 2, 4, 2, 4, 6,
	        1, 3, 5, 3, 5, 7,
	        0, 1, 4, 1, 4, 5,
	        2, 3, 6, 3, 6, 7};

	int32_t iPointPassesCollision[3];

	// At a high level:
	// 1. Iterate over all 12 triangles of the AABB.
	// 2. Clip the triangles against each plane. Create new triangles as needed.
	// 3. Find the min and max z values as the near and far plane.

	// This is easier because the triangles are in camera spacing making the collisions tests simple comparisions.

	float fLightCameraOrthographicMinX = camera_bounds_min_ls.x;
	float fLightCameraOrthographicMaxX = camera_bounds_max_ls.x;
	float fLightCameraOrthographicMinY = camera_bounds_min_ls.y;
	float fLightCameraOrthographicMaxY = camera_bounds_max_ls.y;

	for (int32_t AABBTriIter = 0; AABBTriIter < 12; ++AABBTriIter)
	{
		triangleList[0].pt[0]  = a_scene_bbox_points_ls[static_cast<size_t>(iAABBTriIndexes[AABBTriIter * 3 + 0])];
		triangleList[0].pt[1]  = a_scene_bbox_points_ls[static_cast<size_t>(iAABBTriIndexes[AABBTriIter * 3 + 1])];
		triangleList[0].pt[2]  = a_scene_bbox_points_ls[static_cast<size_t>(iAABBTriIndexes[AABBTriIter * 3 + 2])];
		iTriangleCnt           = 1;
		triangleList[0].culled = FALSE;

		// Clip each invidual triangle against the 4 frustums.  When ever a triangle is clipped into new triangles,
		// add them to the list.
		for (int32_t frustumPlaneIter = 0; frustumPlaneIter < 4; ++frustumPlaneIter)
		{
			float32_t fEdge;
			int32_t   iComponent;

			if (frustumPlaneIter == 0)
			{
				fEdge      = fLightCameraOrthographicMinX;        // todo make float temp
				iComponent = 0;
			}
			else if (frustumPlaneIter == 1)
			{
				fEdge      = fLightCameraOrthographicMaxX;
				iComponent = 0;
			}
			else if (frustumPlaneIter == 2)
			{
				fEdge      = fLightCameraOrthographicMinY;
				iComponent = 1;
			}
			else
			{
				fEdge      = fLightCameraOrthographicMaxY;
				iComponent = 1;
			}

			for (int32_t triIter = 0; triIter < iTriangleCnt; ++triIter)
			{
				// We don't delete triangles, so we skip those that have been culled.
				if (!triangleList[triIter].culled)
				{
					int32_t       iInsideVertCount = 0;
					ror::Vector3f tempOrder;
					// Test against the correct frustum plane.
					// This could be written more compactly, but it would be harder to understand.

					if (frustumPlaneIter == 0)
					{
						for (int32_t triPtIter = 0; triPtIter < 3; ++triPtIter)
						{
							if ((triangleList[triIter].pt[triPtIter].x) > (camera_bounds_min_ls.x))
							{
								iPointPassesCollision[triPtIter] = 1;
							}
							else
							{
								iPointPassesCollision[triPtIter] = 0;
							}
							iInsideVertCount += iPointPassesCollision[triPtIter];
						}
					}
					else if (frustumPlaneIter == 1)
					{
						for (int32_t triPtIter = 0; triPtIter < 3; ++triPtIter)
						{
							if ((triangleList[triIter].pt[triPtIter].x) < (camera_bounds_max_ls.x))
							{
								iPointPassesCollision[triPtIter] = 1;
							}
							else
							{
								iPointPassesCollision[triPtIter] = 0;
							}
							iInsideVertCount += iPointPassesCollision[triPtIter];
						}
					}
					else if (frustumPlaneIter == 2)
					{
						for (int32_t triPtIter = 0; triPtIter < 3; ++triPtIter)
						{
							if ((triangleList[triIter].pt[triPtIter].y) > (camera_bounds_min_ls.y))
							{
								iPointPassesCollision[triPtIter] = 1;
							}
							else
							{
								iPointPassesCollision[triPtIter] = 0;
							}
							iInsideVertCount += iPointPassesCollision[triPtIter];
						}
					}
					else
					{
						for (int32_t triPtIter = 0; triPtIter < 3; ++triPtIter)
						{
							if ((triangleList[triIter].pt[triPtIter].y) < (camera_bounds_max_ls.y))
							{
								iPointPassesCollision[triPtIter] = 1;
							}
							else
							{
								iPointPassesCollision[triPtIter] = 0;
							}
							iInsideVertCount += iPointPassesCollision[triPtIter];
						}
					}

					// Move the points that pass the frustum test to the begining of the array.
					if (iPointPassesCollision[1] && !iPointPassesCollision[0])
					{
						tempOrder                   = triangleList[triIter].pt[0];
						triangleList[triIter].pt[0] = triangleList[triIter].pt[1];
						triangleList[triIter].pt[1] = tempOrder;
						iPointPassesCollision[0]    = TRUE;
						iPointPassesCollision[1]    = FALSE;
					}
					if (iPointPassesCollision[2] && !iPointPassesCollision[1])
					{
						tempOrder                   = triangleList[triIter].pt[1];
						triangleList[triIter].pt[1] = triangleList[triIter].pt[2];
						triangleList[triIter].pt[2] = tempOrder;
						iPointPassesCollision[1]    = TRUE;
						iPointPassesCollision[2]    = FALSE;
					}
					if (iPointPassesCollision[1] && !iPointPassesCollision[0])
					{
						tempOrder                   = triangleList[triIter].pt[0];
						triangleList[triIter].pt[0] = triangleList[triIter].pt[1];
						triangleList[triIter].pt[1] = tempOrder;
						iPointPassesCollision[0]    = TRUE;
						iPointPassesCollision[1]    = FALSE;
					}

					if (iInsideVertCount == 0)
					{        // All points failed. We're done,
						triangleList[triIter].culled = true;
					}
					else if (iInsideVertCount == 1)
					{        // One point passed. Clip the triangle against the Frustum plane
						triangleList[triIter].culled = false;

						//
						ror::Vector3f vVert0ToVert1 = triangleList[triIter].pt[1] - triangleList[triIter].pt[0];
						ror::Vector3f vVert0ToVert2 = triangleList[triIter].pt[2] - triangleList[triIter].pt[0];

						// Find the collision ratio.
						float32_t fHitPointTimeRatio = fEdge - (&triangleList[triIter].pt[0].x)[iComponent];
						// Calculate the distance along the vector as ratio of the hit ratio to the component.
						float32_t fDistanceAlongVector01 = fHitPointTimeRatio / (&vVert0ToVert1.x)[iComponent];
						float32_t fDistanceAlongVector02 = fHitPointTimeRatio / (&vVert0ToVert2.x)[iComponent];
						// Add the point plus a percentage of the vector.
						vVert0ToVert1 *= fDistanceAlongVector01;
						vVert0ToVert1 += triangleList[triIter].pt[0];
						vVert0ToVert2 *= fDistanceAlongVector02;
						vVert0ToVert2 += triangleList[triIter].pt[0];

						triangleList[triIter].pt[1] = vVert0ToVert2;
						triangleList[triIter].pt[2] = vVert0ToVert1;
					}
					else if (iInsideVertCount == 2)
					{        // 2 in  // tesselate into 2 triangles

						// Copy the triangle\(if it exists) after the current triangle out of
						// the way so we can override it with the new triangle we're inserting.
						triangleList[iTriangleCnt] = triangleList[triIter + 1];

						triangleList[triIter].culled     = false;
						triangleList[triIter + 1].culled = false;

						// Get the vector from the outside point into the 2 inside points.
						ror::Vector3f vVert2ToVert0 = triangleList[triIter].pt[0] - triangleList[triIter].pt[2];
						ror::Vector3f vVert2ToVert1 = triangleList[triIter].pt[1] - triangleList[triIter].pt[2];

						// Get the hit point ratio.
						float32_t fHitPointTime_2_0        = fEdge - (&triangleList[triIter].pt[2].x)[iComponent];
						float32_t fDistanceAlongVector_2_0 = fHitPointTime_2_0 / (&vVert2ToVert0.x)[iComponent];
						// Calcaulte the new vert by adding the percentage of the vector plus point 2.
						vVert2ToVert0 *= fDistanceAlongVector_2_0;
						vVert2ToVert0 += triangleList[triIter].pt[2];

						// Add a new triangle.
						triangleList[triIter + 1].pt[0] = triangleList[triIter].pt[0];
						triangleList[triIter + 1].pt[1] = triangleList[triIter].pt[1];
						triangleList[triIter + 1].pt[2] = vVert2ToVert0;

						// Get the hit point ratio.
						float32_t fHitPointTime_2_1        = fEdge - (&triangleList[triIter].pt[2].x)[iComponent];
						float32_t fDistanceAlongVector_2_1 = fHitPointTime_2_1 / (&vVert2ToVert1.x)[iComponent];
						vVert2ToVert1 *= fDistanceAlongVector_2_1;
						vVert2ToVert1 += triangleList[triIter].pt[2];
						triangleList[triIter].pt[0] = triangleList[triIter + 1].pt[1];
						triangleList[triIter].pt[1] = triangleList[triIter + 1].pt[2];
						triangleList[triIter].pt[2] = vVert2ToVert1;
						// Cncrement triangle count and skip the triangle we just inserted.
						++iTriangleCnt;
						++triIter;
					}
					else
					{        // all in
						triangleList[triIter].culled = false;
					}
				}        // end if !culled loop
			}
		}

		for (int32_t index = 0; index < iTriangleCnt; ++index)
		{
			if (!triangleList[index].culled)
			{
				// Set the near and far plan and the min and max z values respectivly.
				for (int vertind = 0; vertind < 3; ++vertind)
				{
					float fTriangleCoordZ = triangleList[index].pt[vertind].z;
					if (a_near_plane > fTriangleCoordZ)
					{
						a_near_plane = fTriangleCoordZ;
					}
					if (a_far_plane < fTriangleCoordZ)
					{
						a_far_plane = fTriangleCoordZ;
					}
				}
			}
		}
	}
}

/*
void shadow_volume_debug_geometry(ror::Scene &a_scene, ror::EventSystem &a_event_system, ror::Matrix4f &a_light_view_fit, ror::Matrix4f &a_projection_fit,
                                  ror::Vector3f &a_eye, ror::Vector3f &a_target, ror::Vector3f frustum_min_ls, ror::Vector3f frustum_max_ls)
{
	(void) a_event_system;

	auto       &scene_camera       = a_scene.current_camera();
	const auto &scene_bbox_ws      = a_scene.bounding_box();
	const auto &frustum_corners_ws = scene_camera.frustum_corners();        // frustum_corners returns corners in world space

	static bool light_to_camera_toggle = true;
	static bool use_light_camera       = false;

	if (use_light_camera)
	{
		scene_camera.view(a_light_view_fit);
		scene_camera.projection(a_projection_fit);
	}

	ror::EventCallback toggle_callback = [](ror::Event) {
		use_light_camera = !use_light_camera;

		if (use_light_camera)
			ror::log_critical("Will use light camera now");
		else
			ror::log_critical("Will use scene camera now");
	};

	if (light_to_camera_toggle)
	{
		a_event_system.subscribe(ror::keyboard_p_click, toggle_callback);
		light_to_camera_toggle = false;
	}

	// Now lets display the light orthographic projection
	ror::Vector3f l_b_f{frustum_min_ls.x, frustum_min_ls.y, frustum_min_ls.z};
	ror::Vector3f l_t_f{frustum_min_ls.x, frustum_max_ls.y, frustum_min_ls.z};
	ror::Vector3f r_t_f{frustum_max_ls.x, frustum_max_ls.y, frustum_min_ls.z};
	ror::Vector3f r_b_f{frustum_max_ls.x, frustum_min_ls.y, frustum_min_ls.z};

	ror::Vector3f l_b_b{frustum_min_ls.x, frustum_min_ls.y, frustum_max_ls.z};
	ror::Vector3f l_t_b{frustum_min_ls.x, frustum_max_ls.y, frustum_max_ls.z};
	ror::Vector3f r_t_b{frustum_max_ls.x, frustum_max_ls.y, frustum_max_ls.z};
	ror::Vector3f r_b_b{frustum_max_ls.x, frustum_min_ls.y, frustum_max_ls.z};

	ror::Vector3f light_projection_corners_ws[8];

	ror::Matrix4f light_view_inv{};
	bool          result = a_light_view_fit.inverse(light_view_inv);
	assert(result && "Light view can't be inverted while fitting it to the shadow volume");
	(void) result;

	light_projection_corners_ws[0] = light_view_inv * l_b_f;
	light_projection_corners_ws[1] = light_view_inv * l_t_f;
	light_projection_corners_ws[2] = light_view_inv * r_t_f;
	light_projection_corners_ws[3] = light_view_inv * r_b_f;

	light_projection_corners_ws[4] = light_view_inv * l_b_b;
	light_projection_corners_ws[5] = light_view_inv * l_t_b;
	light_projection_corners_ws[6] = light_view_inv * r_t_b;
	light_projection_corners_ws[7] = light_view_inv * r_b_b;

	static uint32_t light_projection_ws_index{0xFFFFFFFF};
	static uint32_t light_lines_ws_index{0xFFFFFFFF};
	static uint32_t frustum_corners_ws_index{0xFFFFFFFF};

	auto &ls = ror::line_soup();

	light_lines_ws_index      = ls.push_box(scene_bbox_ws, light_lines_ws_index, ror::orange4f);
	light_projection_ws_index = ls.push_box(light_projection_corners_ws, light_projection_ws_index, ror::red4f);
	frustum_corners_ws_index  = ls.push_box(frustum_corners_ws, frustum_corners_ws_index, ror::green4f);

	static uint32_t ce{0xFFFFFFFF};
	static uint32_t cen{0xFFFFFFFF};
	static uint32_t lp{0xFFFFFFFF};

	const uint32_t         lines_count = 2;
	std::vector<float32_t> data{};
	data.reserve(lines_count * 2 * 4);

	ror::push_point(data, a_eye, ror::white4f);
	ror::push_point(data, a_target, ror::red4f);

	ce  = ls.push_lines(reinterpret_cast<uint8_t *>(data.data()), lines_count, ce);
	lp  = ls.push_cross(a_eye, 2.0f, lp, ror::blue4f);
	cen = ls.push_cross(a_target, 2.0f, cen, ror::green4f);
}
*/

void shadow_volume_debug_geometry(ror::Scene &a_scene, const std::array<ror::Frustum, ror::cascade_count> &a_frustums)
{
	auto &ls = ror::line_soup();

	static uint32_t scene_bbox_index{0xFFFFFFFF};
	scene_bbox_index = ls.push_box(a_scene.bounding_box(), scene_bbox_index, ror::orange4f);

	static std::array<uint32_t, ror::cascade_count> frustum_indices{
	    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

	const ror::Vector4f colors[ror::cascade_count] = {ror::green4f, ror::red4f, ror::blue4f, ror::yellow4f};

	for (size_t cascade = 0; cascade < ror::cascade_count; ++cascade)
	{
		frustum_indices[cascade] = ls.push_box(a_frustums[cascade].corners(), frustum_indices[cascade], colors[cascade]);
	}
}

// void print_matrix(ror::Matrix4f &a_matrix)
// {
// 	std::cout << "[";
// 	for (size_t i = 0; i < 15; ++i)
// 		std::cout << std::fixed << a_matrix.m_values[i] << ", ";
// 	std::cout << std::fixed << a_matrix.m_values[15] << "]\n";
// }

// NOTE: Projection deterimines the size of the box you see, while viewing determines where the box is located.
// In orthographics projection the viewing eye position really doesn't matter as long as the box covers the sceen
// The center of the viewing also doesn't matter as long as its not behind the eye, in which case you will look backwards
void fit_light_frustrum(ror::Scene &a_scene, const ror::Light &a_light, ror::Matrix4f &a_light_view_fit, ror::Matrix4f &a_projection_fit, ror::Vector3f &a_eye)
{
	const auto  light_direction    = a_light.direction();
	auto       &scene_camera       = a_scene.current_camera();
	const auto &frustum_corners_ws = scene_camera.frustum_corners();             // frustum_corners returns corners in world space
	const auto &frustum_box_ws     = scene_camera.frustum_bounding_box();        // frustum_bounding_box returns box in world space

	// light_view is usually looking at the center of the scene in the diretion of the light, but here looking to the center of the camera frustum
	auto target      = frustum_box_ws.center();
	a_eye            = target - (light_direction * 20.0f);
	a_light_view_fit = ror::make_look_at(a_eye, target, ror::yaxis3f, ror::yaxis3f, ror::xaxis3f);

	// First lets get the scene bounding box in light space
	const auto                  &scene_bbox_ws        = a_scene.bounding_box();
	const auto                   scene_bbox_points_ws = scene_bbox_ws.corners();
	std::array<ror::Vector3f, 8> scene_bbox_points_ls;

	for (size_t i = 0; i < 8; ++i)
		scene_bbox_points_ls[i] = a_light_view_fit * scene_bbox_points_ws[i];
	// Now we have all the scene bounding box points in an array in light space needed by compute_near_and_far

	// Next lets get the current scene's current camera and its frustum bounding box in light space
	ror::BoundingBoxf frustum_bbox_ls{};

	for (size_t i = 0; i < ror::frustum_corners_count; ++i)
		frustum_bbox_ls.add_point(a_light_view_fit * frustum_corners_ws[i]);

	// Next find out the tight near and far using DX samples method
	auto      frustum_min_ls = frustum_bbox_ls.minimum();
	auto      frustum_max_ls = frustum_bbox_ls.maximum();
	float32_t frustum_near_ls;
	float32_t frustum_far_ls;

	compute_near_and_far(frustum_near_ls, frustum_far_ls, frustum_min_ls, frustum_max_ls, scene_bbox_points_ls);
	// Now we have bounds of both scene and frustum in light space, we will apply some logic to consolidate which ones to use

	if (frustum_bbox_ls.minimum().z > frustum_near_ls)        // FIXME: reverse-z consideration
		frustum_near_ls = frustum_bbox_ls.minimum().z;

	frustum_min_ls.z = frustum_near_ls;
	frustum_max_ls.z = frustum_far_ls;

	// DX sample returns z in light space for a left-handed view (+Z forward). Our light_view is right-handed (-Z forward)
	// so in front of the light the z values are negative. Convert those to positive near/far distances before building the orthographic projection (most negative -> farthest, least negative -> closest)
	a_projection_fit = ror::make_ortho(frustum_min_ls.x, frustum_max_ls.x, frustum_min_ls.y, frustum_max_ls.y, -frustum_far_ls, -frustum_near_ls);
	// we are done with creating an orthographic tight projection

	// If want to see the shadow volume geometry, uncomment below and provide event_system
	// shadow_volume_debug_geometry(a_scene, a_event_system, a_light_view_fit, a_projection_fit, a_eye, target, frustum_min_ls, frustum_max_ls);
}

// Test fit_light_frustum just to fix viewing and projection to hardcoded values, to be removed
void fit_light_frustrum(const ror::Light &a_light, ror::Matrix4f &a_light_view_fit, ror::Matrix4f &a_projection_fit, ror::Vector3f &a_eye)
{
	float         s = 5.0f;
	ror::Vector3f min{-s, -s, 0.01f};
	ror::Vector3f max{s, s, 1000.0f};
	ror::Vector3f target{0.0f, 0.0f, 0.0f};

	a_eye = a_light.position();

	a_projection_fit = ror::make_ortho(min.x, max.x, min.y, max.y, min.z, max.z);
	a_light_view_fit = ror::make_look_at(a_eye, target, ror::yaxis3f, ror::yaxis3f, ror::xaxis3f);

	// ror::log_warn("About to print the shadow matrices used for light from ({}, {}, {})", a_eye.x, a_eye.y, a_eye.z);

	// auto mvp = a_projection_fit * a_light_view_fit;

	// print_matrix(mvp);
	// print_matrix(a_projection_fit);
	// print_matrix(a_light_view_fit);
}

void render_scene_into_shadowmap(ror::Scene &a_scene, ror::EventSystem &a_event_system, ror::Renderer &a_renderer, const ror::Vector4ui &a_viewport, const ror::Vector3f &a_eye, const ror::Matrix4f &a_view, const ror::Matrix4f &a_projection)
{
	if constexpr (ror::get_build() == ror::BuildType::build_debug)
	{
		// To test with main camera
		static bool scene_to_shadowmap_toggle = true;
		static bool use_scene_camera          = false;

		if (scene_to_shadowmap_toggle)
		{
			ror::EventCallback toggle_callback = [](ror::Event) {
				use_scene_camera = !use_scene_camera;

				if (use_scene_camera)
					ror::log_critical("Using Scene camera to render shadowmap");
				else
					ror::log_critical("Using Light camera to render shadowmap");
			};

			a_event_system.subscribe(ror::keyboard_p_click, toggle_callback);
			scene_to_shadowmap_toggle = false;
		}

		if (use_scene_camera)
		{
			auto &camera = a_scene.current_camera();

			camera.view(a_view);
			camera.projection(a_projection);

			auto view       = camera.view();
			auto projection = camera.projection();

			a_renderer.update_per_view_uniform(view, projection, a_viewport, a_eye);
		}
	}
}

}        // namespace ror
