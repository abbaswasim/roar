struct Line
{
	vec2 p1;
	vec2 p2;
};

struct Segment
{
	Line left;
	Line right;
};

// Positive return to the left, negative to the right, 0 bang on
float side_of_line(Line l, vec2 c)
{
	return (l.p2.x - l.p1.x) * (c.y - l.p1.y) - (l.p2.y - l.p1.y) * (c.x - l.p1.x);
}

bool in_segment(Segment s, vec2 c)
{
	return side_of_line(s.left, c) < 0.0 && side_of_line(s.right, c) > 0.0;
}

// Skew of the line segments in percentage of the horizontal resolution
// also defined same in rorgui.cpp in show_multi_render_view_titles, need to be kept in sync
const float skew           = 0.5;
const float line_width     = 2000.0;
const uint  segments_count = 6; 

Segment segments[segments_count];

void make_segments()
{
	float w = in_per_view_uniform.viewport.z;
	float h = in_per_view_uniform.viewport.w;

	float wc = w / (segments_count + 2);
	float hc = h / (segments_count + 2);

	float w_skew = wc * skew;

	for (uint i = 0; i < segments_count; i++)
	{
		segments[i].left.p1.x = wc * i + w_skew + wc;
		segments[i].left.p2.x = wc * i - w_skew + wc;

		segments[i].left.p1.y = 0.0f;
		segments[i].left.p2.y = h;

		segments[i].right.p1.x = wc * (i + 1) + w_skew + wc;
		segments[i].right.p2.x = wc * (i + 1) - w_skew + wc;

		segments[i].right.p1.y = 0.0f;
		segments[i].right.p2.y = h;
	}
}

void set_render_mode(Material material, Fragment fragment)
{
	vec4 out_color_copy = out_color;

	if (!("Shaded"))
	{
		out_color = vec4(1.0, 0.0, 1.0, 1.0);
	}

	if ("Multi-Render View")
	{
		make_segments();

		vec2 c = gl_FragCoord.xy;

		if (in_segment(segments[0], c))
			out_color = vec4(material.occlusion, material.occlusion, material.occlusion, 1.0);
		else if (in_segment(segments[1], c))
			out_color = vec4(material.normal, 1.0);
		else if (in_segment(segments[2], c))
			out_color = vec4(material.roughness, material.roughness, material.roughness, 1.0);
		else if (in_segment(segments[3], c))
			out_color = vec4(material.metallic, material.metallic, material.metallic, 1.0);
		else if (in_segment(segments[4], c))
			out_color = vec4(material.base_color);
		else if (in_segment(segments[5], c))
			out_color = vec4(material.emissive);
		else
			out_color = out_color_copy;

		if (abs(side_of_line(segments[0].left, c)) < line_width ||
		    abs(side_of_line(segments[0].right, c)) < line_width ||
		    abs(side_of_line(segments[1].right, c)) < line_width ||
		    abs(side_of_line(segments[2].right, c)) < line_width ||
		    abs(side_of_line(segments[3].right, c)) < line_width ||
		    abs(side_of_line(segments[4].right, c)) < line_width ||
		    abs(side_of_line(segments[5].right, c)) < line_width)
			out_color = vec4(0.7, 0.7, 0.7, 1.0);
	}
	// Texture coordinates
	else if ("Set 0")
	{
		@out_color = vec4(in_vertex_texture_coord_0.x, in_vertex_texture_coord_0.y, 0.0, 1.0);
	}
	else if ("Set 1")
	{
		@out_color = vec4(in_vertex_texture_coord_1.x, in_vertex_texture_coord_1.y, 0.0, 1.0);
	}
	else if ("Set 2")
	{
		@out_color = vec4(in_vertex_texture_coord_2.x, in_vertex_texture_coord_2.y, 0.0, 1.0);
	}
	// Geometric Data
	else if ("Geometric Normal")
	{
		@out_color     = vec4(normalize(in_vertex_normal), 1.0);
		@out_color.xyz = pow(out_color.xyz, vec3(2.2));        // Gamma corrected
	}
	else if ("Geometric Normal Scaled")        // Scaled to * .5 and + .5 to make it look nice on display
	{
		@out_color     = vec4(normalize(in_vertex_normal) * .5 + .5, 1.0);
		@out_color.xyz = pow(out_color.xyz, vec3(2.2));        // Gamma corrected
	}
	else if ("Geometric Tangent")
	{
		@out_color = get_tangent();
	}
	else if ("Geometric Bitangent")
	{
		@out_color = vec4(get_bitangent(in_vertex_normal, get_tangent()), 1.0);
	}
	else if ("Geometric Color 0")
	{
		@out_color = in_vertex_color_0;
	}
	else if ("Geometric Color 1")
	{
		@out_color = in_vertex_color_1;
	}
	// Texture Maps
	else if ("Base Color Map")
	{
		@out_color = texture(base_color_sampler, get_base_color_uvs());
	}
	else if ("Ambient Occlusion Map")
	{
		@float occlusion = texture(occlusion_sampler, get_occlusion_uvs()).r;
		@out_color       = vec4(occlusion, occlusion, occlusion, 1.0);
	}
	else if ("Roughness Map")
	{
		@float roughness = texture(roughness_sampler, get_roughness_uvs()).g;
		@out_color       = vec4(roughness, roughness, roughness, 1.0);
	}
	else if ("Metallic Map")
	{
		@float metallic = texture(metallic_sampler, get_metallic_uvs()).b;
		@out_color      = vec4(metallic, metallic, metallic, 1.0);
	}
	else if ("Normal Map")
	{
		@vec3 N    = texture(normal_sampler, get_normal_uvs()).xyz;
		@out_color = vec4(N, 1.0);
	}
	else if ("Normal Map Scaled")
	{
		@vec3 N    = texture(normal_sampler, get_normal_uvs()).xyz * 2.0 - vec3(1.0);
		@out_color = vec4(N, 1.0);
	}
	else if ("Height Map")
	{
		@float height = texture(height_sampler, get_height_uvs()).a;
		@out_color    = vec4(height, height, height, 1.0);
	}
	else if ("Emissive Map")
	{
		@out_color = texture(emissive_sampler, get_emissive_uvs());
	}
	// Shading Values
	else if ("Shading Normal")
	{
		out_color = vec4(material.normal, 1.0);
	}
	else if ("Material Normal Scaled")
	{
		@out_color = vec4(get_normal(), 1.0);        // Scaled from -1 to 1 and multiplied with factor
	}
	else if ("Base Color Factor")
	{
		@out_color = in_material_factors.base_color_factor;
	}
	else if ("Ambient Occlusion Factor")
	{
		@float of  = in_material_factors.occlusion_factor;
		@out_color = vec4(of, of, of, 1.0);
	}
	else if ("Roughness Factor")
	{
		@float rf  = in_material_factors.roughness_factor;
		@out_color = vec4(rf, rf, rf, 1.0);
	}
	else if ("Metallic Factor")
	{
		@float mf  = in_material_factors.metallic_factor;
		@out_color = vec4(mf, mf, mf, 1.0);
	}
	else if ("Normal Factor")
	{
		@float nf  = in_material_factors.normal_factor;
		@out_color = vec4(nf, nf, 1.0, 1.0);
	}
	else if ("Emissive Factor")
	{
		@out_color = in_material_factors.emissive_factor;
	}
	else if ("Material Roughness")        // its clamped roughness * factor
	{
		float pr  = material.roughness;
		out_color = vec4(pr, pr, pr, 1.0);
	}
	else if ("Fragment Roughness")        // its material roughness squared
	{
		float fr  = fragment.roughness;
		out_color = vec4(fr, fr, fr, 1.0);
	}
	else if ("Fragment Metallic")        // Clamped metallic same as fragment metallic
	{
		float mm  = fragment.metallic;
		out_color = vec4(mm, mm, mm, 1.0);
	}
	else if ("Fragment Ambient Occlusion")        // Same as material occlusion
	{
		float mo  = fragment.occlusion;
		out_color = vec4(mo, mo, mo, 1.0);
	}
	else if ("Base Color Factored")
	{
		@out_color = material.base_color;
	}
	// Other Values
	else if ("World Position")
	{
		vec3 pc   = clamp(fragment.position, 0.0, 1.0);
		vec3 v    = normalize(clamp(pc, 0.0, 1.0));
		out_color = vec4(v, 1.0);
	}
	else if ("View Vector")
	{
		out_color = vec4(material.view, 1.0);
	}
	else if ("Material f0")
	{
		out_color.xyz = vec3(material.f0);
	}
	else if ("Fragment F0")
	{
		out_color.xyz = fragment.F0;
	}
	else if ("NoV")
	{
		vec3  N   = material.normal;
		vec3  V   = material.view;
		float NdV = clamp(dot(N, V), 0.0, 1.0);
		out_color = vec4(NdV, NdV, NdV, 1.0);
	}
	else if ("NoL")
	{
		Light light = get_directional_light(material, fragment, 0);
		float NoL   = clamp(dot(fragment.normal, light.direction), 0.0, 1.0);
		out_color   = vec4(NoL, NoL, NoL, 1.0);
	}
	else if ("H")
	{
		Light light = get_directional_light(material, fragment, 0);
		vec3  H     = normalize(fragment.view + light.direction);
		out_color   = vec4(H, 1.0);
	}
	else if ("NoH")
	{
		Light light = get_directional_light(material, fragment, 0);
		vec3  H     = normalize(fragment.view + light.direction);
		float NoH   = clamp(dot(fragment.normal, H), 0.0, 1.0);
		out_color   = vec4(NoH, NoH, NoH, 1.0);
	}
	else if ("VoH")
	{
		Light light = get_directional_light(material, fragment, 0);
		vec3  H     = normalize(fragment.view + light.direction);
		float VoH   = clamp(dot(fragment.view, H), 0.0, 1.0);
		out_color   = vec4(VoH, VoH, VoH, 1.0);
	}
}
