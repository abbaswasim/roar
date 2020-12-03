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

#include "RORCBA.h"

namespace ror
{
CCBA::~CCBA(void)
{
	rDELETE(m_header);
	rDELETE(m_geometry_data);
	rDELETE(m_skeleton_data);
	rDELETE(m_vbo);
	rDELETE(m_ibo);
	rDELETE(m_bound);

	this->m_current_clip = nullptr;

	if (!this->m_triangles_groups->empty())
	{
		std::vector<TriangleGroup *>::iterator iter;
		for (iter = this->m_triangles_groups->begin(); iter != this->m_triangles_groups->end(); ++iter)
		{
			rDELETE(*iter);
		}
		m_triangles_groups->clear();
		rDELETE(m_triangles_groups);
	}

	if (!this->m_clips->empty())
	{
		std::unordered_map<EAnimationClips, AnimationClip *>::iterator iter;
		for (iter = this->m_clips->begin(); iter != this->m_clips->end(); ++iter)
		{
			rDELETE(iter->second);
		}
		this->m_clips->clear();
		rDELETE(m_clips);
	}
}

void CCBA::read_cba(std::string a_file_name)
{
	Export *file = rNew Export(0, a_file_name);

	this->read_header(file, a_file_name);                                                                                              // Read Header
	this->read_array(file, &this->m_geometry_data->m_vertices_array, this->m_geometry_data->m_vertices_array_size);                    // Read Points
	this->read_array(file, &this->m_geometry_data->m_normals_array, this->m_geometry_data->m_normals_array_size);                      // Read Normals
	this->read_array(file, &this->m_geometry_data->m_texture_coords_array, this->m_geometry_data->m_texture_coords_array_size);        // Read TextureCoords

	if (65536 & this->m_header->m_main_chunk.m_has_animation_has_skeleton)
		this->read_array(file, &this->m_geometry_data->m_vertex_weights_array, this->m_geometry_data->m_vertex_weights_array_size);        // Read Weights Data

	this->read_triangles(file);

	if (65536 & this->m_header->m_main_chunk.m_has_animation_has_skeleton)
	{
		this->read_skeleton(file);
		this->read_animations(file);
		//this->SetupSkeleton(-1);
		this->setup_inverse_bind_attributes();
	}

	rDELETE(file);

	this->read_clips_information(a_file_name);        // Read Clips

	this->calculate_bound();

	if (this->m_header->m_main_chunk.m_version == 1)
	{
		this->convert_to_arrays();
		this->write_cba(a_file_name);
		this->duplicate_skinned_data();
	}
	else
	{
		this->duplicate_backup_data();
	}

	this->fill_vertex_buffer_object();
	this->fill_index_buffer_object();
	//this->SetupTexture();
}

void CCBA::write_cba(std::string a_file_name)
{
	Export *file = rNew Export(1, a_file_name);

	this->write_header(file);                                                                                                          // Write Header
	this->write_array(file, this->m_geometry_data->m_skinned_vertices_array, this->m_geometry_data->m_vertices_array_size);            // Write Points
	this->write_array(file, this->m_geometry_data->m_skinned_normals_array, this->m_geometry_data->m_normals_array_size);              // Write Normals
	this->write_array(file, this->m_geometry_data->m_texture_coords_array, this->m_geometry_data->m_texture_coords_array_size);        // Write TextureCoords

	if (65536 & this->m_header->m_main_chunk.m_has_animation_has_skeleton)
		this->write_array(file, this->m_geometry_data->m_vertex_weights_array, this->m_geometry_data->m_vertex_weights_array_size);        // Write Weights Data

	this->write_triangles(file);

	if (65536 & this->m_header->m_main_chunk.m_has_animation_has_skeleton)
	{
		this->write_skeleton(file);
		this->write_animations(file);
	}

	file->finish_write();

	rDELETE(file);
}

void CCBA::read_header(Export *a_file, std::string a_file_name)
{
	this->m_header = rNew ccbaheader();
	a_file->read((char8_t *) this->m_header, sizeof(ccbaheader));
	if (3 != this->m_header->m_geometry_chunk.m_triangles_details)
	{
		a_file_name += "\nCan't Read This CBA file without Normals and Texture Coordinates";
		MESSAGEBOX(a_file_name.c_str());
		return;
	}
}

void CCBA::write_header(Export *a_file)
{
	this->m_header->m_main_chunk.m_version = 2;
	a_file->write((char8_t *) this->m_header, sizeof(ccbaheader));
}

void CCBA::read_array(Export *a_file, float32_t **a_destination, uint32_t &a_size)
{
	uint32_t temp_size;
	a_file->read(&temp_size);
	assert(temp_size > 0 && "Size of array is less then zero");
	*a_destination = new float32_t[temp_size];
	a_size         = temp_size;
	a_file->read(*a_destination, temp_size * sizeof(float32_t));
}

void CCBA::write_array(Export *a_file, float32_t *a_destination, uint32_t &a_size)
{
	a_file->write((char8_t *) &a_size, sizeof(uint32_t));
	a_file->write((char8_t *) a_destination, a_size * sizeof(float32_t));
}

void CCBA::read_triangles(Export *a_file)
{
	// Now Read the Triangles Data
	uint32_t no_oftriangles = 0;
	for (uint32_t no_of_triangle_groups = 0; no_of_triangle_groups < this->m_header->m_geometry_chunk.m_no_of_triangle_groups; no_of_triangle_groups++)
	{
		ror::TriangleGroup *temp_triangle_group = rNew ror::TriangleGroup;
		no_oftriangles                          = 0;
		a_file->read(&no_oftriangles);        // Number of Triangles
		temp_triangle_group->m_triangles       = rNew ror::Triangle[no_oftriangles];
		temp_triangle_group->m_no_of_triangles = no_oftriangles;

		// Read Material First
		ror::Material current_material;
		a_file->read((char8_t *) &current_material, sizeof(ror::Material));
		*temp_triangle_group->m_material                       = current_material;
		temp_triangle_group->m_material->m_diffuse.m_values[3] = current_material.m_transparency;

		a_file->read((char8_t *) temp_triangle_group->m_triangles, no_oftriangles * 3 * this->m_header->m_geometry_chunk.m_triangles_details * sizeof(rUsShort));
		this->m_triangles_groups->push_back(temp_triangle_group);
	}
}

void CCBA::write_triangles(Export *a_file)
{
	// Now Write the Triangles Data
	uint32_t no_oftriangles = 0;
	for (uint32_t tgr = 0; tgr < this->m_header->m_geometry_chunk.m_no_of_triangle_groups; tgr++)
	{
		a_file->write((char8_t *) &(*this->m_triangles_groups)[tgr]->m_no_of_triangles, sizeof(uint32_t));        // Number of Triangles
		// Write Material First
		a_file->write((char8_t *) (*this->m_triangles_groups)[tgr]->m_material, sizeof(ror::Material));
		// Now Write the Triangles Data
		a_file->write((char8_t *) (*this->m_triangles_groups)[tgr]->m_triangles, (*this->m_triangles_groups)[tgr]->m_no_of_triangles * 3 * this->m_header->m_geometry_chunk.m_triangles_details * sizeof(rUsShort));
	}
}

void CCBA::read_skeleton(Export *a_file)
{
	uint32_t no_of_joints;
	// Read Skeleton Data
	a_file->read((char8_t *) this->m_skeleton_data->m_bind_shape_matrix->m_values, 16 * sizeof(float32_t));
	this->m_skeleton_data->m_bind_shape_matrix->transpose();
	no_of_joints = 0;
	a_file->read(&no_of_joints);
	assert(no_of_joints > 0 && "Number of Joints is less then zero");
	int32_t bone_id;
	this->m_skeleton_data->m_bones       = new ror::Bone *[no_of_joints];
	this->m_skeleton_data->m_no_of_bones = no_of_joints;

	for (uint32_t i = 0; i < no_of_joints; i++)
		this->m_skeleton_data->m_bones[i] = rNew Bone();

	for (uint32_t i = 0; i < no_of_joints; i++)
	{
		this->m_skeleton_data->m_bones[i]->m_id = i;
		bone_id                                 = 0;
		a_file->read(&bone_id);
		assert(bone_id <= (int32_t) no_of_joints && "Unknown Bone Start Is read");
		this->m_skeleton_data->m_bones[i]->m_parent_id = bone_id;

		if (-1 != bone_id)        // -1 indicates a root in the skeleton
			this->m_skeleton_data->m_bones[bone_id]->m_children->push_back(i);
		else
			this->m_skeleton_data->m_root_bone = this->m_skeleton_data->m_bones[i];        // the root bone

		a_file->read(this->m_skeleton_data->m_bones[i]->m_joint_matrix->m_values, 16 * sizeof(float32_t));
		a_file->read(this->m_skeleton_data->m_bones[i]->m_inverse_bind_matrix->m_values, 16 * sizeof(float32_t));

		this->m_skeleton_data->m_bones[i]->m_joint_matrix->transpose();
		this->m_skeleton_data->m_bones[i]->m_inverse_bind_matrix->transpose();
	}

	// Now we read the Weighted Vertices data
	uint32_t number_of_vertics = this->m_geometry_data->m_vertices_array_size / 3;

	//this->m_GeometryData->m_VertexInfluences = new CoreEngine::CVertexInfluence*[NumberOfVertics];

	for (uint32_t i = 0; i < number_of_vertics; i++)
	{
		no_of_joints = 0;
		a_file->read(&no_of_joints);
		assert(no_of_joints > 0 && "Number of Bones is less then zero");
		VertexInfluence *temp_vertex_influence = rNew VertexInfluence(no_of_joints);

		for (uint32_t j = 0; j < no_of_joints; j++)
		{
			WeightedVertex temp_weighted_vertex;
			temp_weighted_vertex.m_vertex_index = i;
			bone_id                             = 0;
			a_file->read(&bone_id);        // Read Bone Id
			assert(bone_id <= (int32_t) this->m_skeleton_data->m_no_of_bones && "Unknown Bone Id");
			a_file->read(&temp_weighted_vertex.m_weight_index);        // Read the weight with this bone
			//this->m_SkeletonData->m_Bones[BoneId]->m_InfluencedVertices->push_back(tempWeightedVertex);

			temp_vertex_influence->m_joints[j]  = bone_id;
			temp_vertex_influence->m_weights[j] = /*this->m_GeometryData->m_VertexWeightsArray[*/ temp_weighted_vertex.m_weight_index;        //];
		}

		this->m_geometry_data->m_vertex_influences->push_back(temp_vertex_influence);
	}

	// Update the Counters inside too
	for (uint32_t i = 0; i < this->m_skeleton_data->m_no_of_bones; i++)
	{
		this->m_skeleton_data->m_bones[i]->m_child_count = (uint32_t) this->m_skeleton_data->m_bones[i]->m_children->size();
		//this->m_SkeletonData->m_Bones[i]->m_InfluencedVertexCount = (uint32_t)this->m_SkeletonData->m_Bones[i]->m_InfluencedVertices->size();
	}
}

void CCBA::write_skeleton(Export *a_file)
{
	// Write Skeleton Data
	this->m_skeleton_data->m_bind_shape_matrix->transpose();
	a_file->write((char8_t *) this->m_skeleton_data->m_bind_shape_matrix->m_values, 16 * sizeof(float32_t));
	this->m_skeleton_data->m_bind_shape_matrix->transpose();
	a_file->write((char8_t *) &this->m_skeleton_data->m_no_of_bones, sizeof(uint32_t));

	for (uint32_t i = 0; i < this->m_skeleton_data->m_no_of_bones; i++)
	{
		this->m_skeleton_data->m_bones[i]->m_joint_matrix->transpose();
		this->m_skeleton_data->m_bones[i]->m_inverse_bind_matrix->transpose();

		a_file->write((char8_t *) &this->m_skeleton_data->m_bones[i]->m_parent_id, sizeof(int32_t));
		a_file->write((char8_t *) this->m_skeleton_data->m_bones[i]->m_joint_matrix->m_values, 16 * sizeof(float32_t));
		a_file->write((char8_t *) this->m_skeleton_data->m_bones[i]->m_inverse_bind_matrix->m_values, 16 * sizeof(float32_t));

		this->m_skeleton_data->m_bones[i]->m_joint_matrix->transpose();
		this->m_skeleton_data->m_bones[i]->m_inverse_bind_matrix->transpose();
	}

	// Now we Write the Weighted Vertices data
	uint32_t number_of_vertics = this->m_geometry_data->m_vertices_array_size / 3;

	for (uint32_t i = 0; i < number_of_vertics; i++)
	{
		a_file->write((char8_t *) &(*this->m_geometry_data->m_vertex_influences)[i]->m_no_of_influences, sizeof(uint32_t));

		for (uint32_t j = 0; j < (*this->m_geometry_data->m_vertex_influences)[i]->m_no_of_influences; j++)
		{
			a_file->write((char8_t *) &(*this->m_geometry_data->m_vertex_influences)[i]->m_joints[j], sizeof(int32_t));          // Write Bone Id
			a_file->write((char8_t *) &(*this->m_geometry_data->m_vertex_influences)[i]->m_weights[j], sizeof(uint32_t));        // Write the weight with this bone
		}
	}
}

void CCBA::read_clips_information(std::string a_file_name)
{
	// Create the CLP Filename
	//assert(nullptr != a_FileName && "File name Empty");
	a_file_name = a_file_name.substr(0, a_file_name.length() - 3);
	a_file_name += "CLP";

	TiXmlDocument doc(a_file_name.c_str());
	if (!doc.LoadFile())
	{
		ror::AnimationClip *clip = rNew ror::AnimationClip();
		clip->m_start_keyframe   = 0;
		clip->m_end_keyframe     = (int32_t) this->m_header->m_animation_chunk.m_no_of_keyframes - 1;
		clip->m_start_key_time   = this->m_header->m_animation_chunk.m_start_time;
		clip->m_end_key_time     = this->m_header->m_animation_chunk.m_total_time;

		this->m_clips->insert(std::make_pair(rIDLE, clip));
		this->m_current_clip = clip;
		return;
	}

	TiXmlHandle h_doc(&doc);
	TiXmlHandle h_root(0);

	TiXmlNode *p_parent = h_doc.ToNode();
	TiXmlNode *child;
	p_parent = p_parent->FirstChildElement();
	for (child = p_parent->FirstChild(); child != 0; child = child->NextSibling())
	{
		std::string         name = child->ToElement()->Value();
		std::string         start, end;
		ror::AnimationClip *clip               = rNew ror::AnimationClip();
		TiXmlAttribute *                attrib = child->ToElement()->FirstAttribute();
		while (attrib)
		{
			std::string at = attrib->Name();
			if ("start" == at)
			{
				start = attrib->Value();
			}
			if ("end" == at)
			{
				end = attrib->Value();
			}
			attrib = attrib->Next();
		}

		std::string       clip_string = start + " " + end;
		std::stringstream clip_stream(clip_string);        // Insert the string into a stream
		clip_stream >> clip->m_start_keyframe;
		clip_stream >> clip->m_end_keyframe;

		clip->m_start_key_time = (((float32_t)(clip->m_start_keyframe + 1) * this->m_header->m_animation_chunk.m_total_time) / (float32_t) this->m_header->m_animation_chunk.m_no_of_keyframes);
		clip->m_end_key_time   = (((float32_t)(clip->m_end_keyframe + 1) * this->m_header->m_animation_chunk.m_total_time) / (float32_t) this->m_header->m_animation_chunk.m_no_of_keyframes);

		if (clip->m_start_key_time > 0)
			clip->m_start_key_time -= 0.001f;        // Hack for enabling the initial Frame

		EAnimationClips e_name = ror::Utility::get_animation_clip(name);
		this->m_clips->insert(std::make_pair(e_name, clip));
		if (nullptr == this->m_current_clip)
			this->m_current_clip = clip;
	}
}

void CCBA::read_animations(Export *a_file)
{
	int32_t bone_id;
	// Now lets read the Key frames data
	this->m_skeleton_data->m_no_of_keyframes = this->m_header->m_animation_chunk.m_no_of_keyframes;
	for (uint32_t i = 0; i < this->m_header->m_animation_chunk.m_no_of_animations; i++)
	{
		bone_id = 0;
		a_file->read(&bone_id);
		assert(bone_id <= (int32_t) this->m_skeleton_data->m_no_of_bones && "Unknown Bone Start is found in animation");
		this->m_skeleton_data->m_bones[bone_id]->m_no_of_keyframes = this->m_header->m_animation_chunk.m_no_of_keyframes;
		this->m_skeleton_data->m_bones[bone_id]->m_keyframes       = new ror::Keyframe *[this->m_skeleton_data->m_bones[bone_id]->m_no_of_keyframes];

		for (uint32_t j = 0; j < this->m_skeleton_data->m_bones[bone_id]->m_no_of_keyframes; j++)
			this->m_skeleton_data->m_bones[bone_id]->m_keyframes[j] = rNew ror::Keyframe();

		for (uint32_t j = 0; j < this->m_header->m_animation_chunk.m_no_of_keyframes; j++)
		{
			a_file->read(&this->m_skeleton_data->m_bones[bone_id]->m_keyframes[j]->m_time);                                                          // Read time
			a_file->read((char8_t *) this->m_skeleton_data->m_bones[bone_id]->m_keyframes[j]->m_transform->m_values, 16 * sizeof(float32_t));        // Read Transform
			this->m_skeleton_data->m_bones[bone_id]->m_keyframes[j]->m_transform->transpose();
		}
	}
}

void CCBA::write_animations(Export *a_file)
{
	// Now lets Write the Key frames data
	for (uint32_t i = 0; i < this->m_skeleton_data->m_no_of_bones; i++)
	{
		if (0 != this->m_skeleton_data->m_bones[i]->m_no_of_keyframes)
		{
			a_file->write((char8_t *) &this->m_skeleton_data->m_bones[i]->m_id, sizeof(int32_t));

			for (uint32_t j = 0; j < this->m_skeleton_data->m_bones[i]->m_no_of_keyframes; j++)
			{
				this->m_skeleton_data->m_bones[i]->m_keyframes[j]->m_transform->transpose();
				a_file->write((char8_t *) &this->m_skeleton_data->m_bones[i]->m_keyframes[j]->m_time, sizeof(float32_t));                           // Write time
				a_file->write((char8_t *) this->m_skeleton_data->m_bones[i]->m_keyframes[j]->m_transform->m_values, 16 * sizeof(float32_t));        // Write Transform
				this->m_skeleton_data->m_bones[i]->m_keyframes[j]->m_transform->transpose();
			}
		}
	}
}

void CCBA::convert_to_arrays()
{
	float32_t *position_data;
	float32_t *normal_data;
	float32_t *texture_data;

	uint32_t position_size;
	uint32_t normal_size;
	uint32_t texture_size;

	float32_t *position_data_vbo;
	float32_t *normal_data_vbo;
	float32_t *texture_data_vbo;

	uint32_t position_size_vbo;
	uint32_t normal_size_vbo;
	uint32_t texture_size_vbo;

	// These are the Original Buffers From Collada
	position_size = this->m_geometry_data->m_vertices_array_size;
	position_data = this->m_geometry_data->m_vertices_array;
	normal_size   = this->m_geometry_data->m_normals_array_size;
	normal_data   = this->m_geometry_data->m_normals_array;
	texture_size  = this->m_geometry_data->m_texture_coords_array_size;
	texture_data  = this->m_geometry_data->m_texture_coords_array;

	// Now Uniform them in the Big Possible Buffer Array
	typedef std::unordered_map<std::string, IndexCountPair> index_buffer_map_type;
	index_buffer_map_type                                   indix_buffer_map;

	// First Combine all the Indices Buffers for all the groups of Triangles
	for (uint32_t tgr = 0; tgr < this->m_triangles_groups->size(); tgr++)
	{
		// Check for Uniform Sets of (Vertex,Normal,TexCoord)
		uint32_t no_of_triangles = (*this->m_triangles_groups)[tgr]->m_no_of_triangles;
		for (uint32_t no_triangles = 0; no_triangles < no_of_triangles; no_triangles++)
		{
			for (uint32_t nov = 0; nov < 3; nov++)
			{
				std::stringstream index_string;
				// The indices are Saved in a String like "PositionIndex.NormalIndex.TexCoordIndex"
				IndexCountPair                                   temp_pair;
				std::pair<index_buffer_map_type::iterator, bool> ret;
				temp_pair.m_position_index  = (*this->m_triangles_groups)[tgr]->m_triangles[no_triangles].m_vertices_index[nov];
				temp_pair.m_normal_index    = (*this->m_triangles_groups)[tgr]->m_triangles[no_triangles].m_normals_index[nov];
				temp_pair.m_tex_coord_index = (*this->m_triangles_groups)[tgr]->m_triangles[no_triangles].m_textures_index[nov];

				index_string << temp_pair.m_position_index << '.' << temp_pair.m_normal_index << '.' << temp_pair.m_tex_coord_index;

				//++IndixBuffestd::unordered_map[IndexString.str()]; // Increments Number of Occurrences of this Set and also inserts if not present
				ret = indix_buffer_map.insert(index_buffer_map_type::value_type(index_string.str(), temp_pair));
				if (ret.second == false)
				{
					// This element already exists so do increment
					ret.first->second.m_count++;
				}
			}
		}
	}

	uint32_t max_size = (uint32_t) indix_buffer_map.size();

	position_size_vbo = max_size * 3;
	position_data_vbo = new float32_t[position_size_vbo];
	normal_size_vbo   = max_size * 3;
	normal_data_vbo   = new float32_t[normal_size_vbo];
	texture_size_vbo  = max_size * 2;
	texture_data_vbo  = new float32_t[texture_size_vbo];

	std::vector<VertexInfluence *> *influence_vector_vbo = new std::vector<VertexInfluence *>();

	// for all the Unique Sets of (Position,Normal,TexCoord)
	uint32_t index = 0;
	for (index_buffer_map_type::iterator iter = indix_buffer_map.begin(); iter != indix_buffer_map.end(); ++iter)
	{
		// Update all the Data Arrays and keep them in sync. (having the same Index for each Unique Set of (Position,Normal,TexCoord))
		IndexCountPair *temp_index_pair = &iter->second;

		// Copy Position data in the PositionVBO Buffer
		position_data_vbo[(index * 3)]     = position_data[(temp_index_pair->m_position_index * 3)];
		position_data_vbo[(index * 3) + 1] = position_data[(temp_index_pair->m_position_index * 3) + 1];
		position_data_vbo[(index * 3) + 2] = position_data[(temp_index_pair->m_position_index * 3) + 2];

		// If we have Vertex Weights then lets update those IDs too
		if (65536 & this->m_header->m_main_chunk.m_has_animation_has_skeleton)
		{
			assert(index == influence_vector_vbo->size() && "Index is not same as Influence Vectors Size");
			influence_vector_vbo->push_back((*this->m_geometry_data->m_vertex_influences)[temp_index_pair->m_position_index]);
			/*
				for (uint32_t i = 0; i < this->m_SkeletonData->m_NoOfBones; i++)
				{
					for (uint32_t j = 0; j < this->m_SkeletonData->m_Bones[i]->m_InfluencedVertexCount; j++)
					{
						if ((*this->m_SkeletonData->m_Bones[i]->m_InfluencedVertices)[j].m_VertexIndex == tempIndexPair->m_PositionIndex)
						{
							(*this->m_SkeletonData->m_Bones[i]->m_InfluencedVertices)[j].m_VertexIndex = Index;
						}
					}
				}
*/
		}

		// Copy Normal data in the NormalVBO Buffer
		normal_data_vbo[(index * 3)]     = normal_data[(temp_index_pair->m_normal_index * 3)];
		normal_data_vbo[(index * 3) + 1] = normal_data[(temp_index_pair->m_normal_index * 3) + 1];
		normal_data_vbo[(index * 3) + 2] = normal_data[(temp_index_pair->m_normal_index * 3) + 2];

		// Copy TexCoord data in the TexCoordVBO Buffer
		texture_data_vbo[(index * 2)]     = texture_data[(temp_index_pair->m_tex_coord_index * 2)];
		texture_data_vbo[(index * 2) + 1] = texture_data[(temp_index_pair->m_tex_coord_index * 2) + 1];

		// Also keep in record which index was changed
		temp_index_pair->m_index = index++;
	}

	// Now Update the Actual Data Arrays and their sizes
	rDELETEARRAY(this->m_geometry_data->m_vertices_array);
	rDELETEARRAY(this->m_geometry_data->m_normals_array);
	rDELETEARRAY(this->m_geometry_data->m_texture_coords_array);

	//this->m_GeometryData->m_VerticesArray			= PositionDataVBO;
	this->m_geometry_data->m_skinned_vertices_array = position_data_vbo;
	this->m_geometry_data->m_skinned_normals_array  = normal_data_vbo;
	this->m_geometry_data->m_texture_coords_array   = texture_data_vbo;

	this->m_geometry_data->m_vertices_array_size       = position_size_vbo;
	this->m_geometry_data->m_normals_array_size        = normal_size_vbo;
	this->m_geometry_data->m_texture_coords_array_size = texture_size_vbo;

	rDELETE(this->m_geometry_data->m_vertex_influences);
	this->m_geometry_data->m_vertex_influences = influence_vector_vbo;

	// Now Change all the Occurrences of These Unique Sets of (Position,Normal,TexCoord) in all groups of IndexBuffers for all TriangleGroups
	// And Make the IBO ready

	for (uint32_t tgr = 0; tgr < this->m_triangles_groups->size(); tgr++)
	{
		uint32_t index_size_vbo = (*this->m_triangles_groups)[tgr]->m_no_of_triangles * 3;

		for (uint32_t index = 0; index < index_size_vbo; index++)
		{
			IndexCountPair temp_pair;
			temp_pair.m_position_index  = (uint32_t)(*this->m_triangles_groups)[tgr]->m_triangles[index / 3].m_vertices_index[index % 3];
			temp_pair.m_normal_index    = (uint32_t)(*this->m_triangles_groups)[tgr]->m_triangles[index / 3].m_normals_index[index % 3];
			temp_pair.m_tex_coord_index = (uint32_t)(*this->m_triangles_groups)[tgr]->m_triangles[index / 3].m_textures_index[index % 3];

			// Find this combination in the List and change its occurrence in the IndexList to this Index
			std::stringstream index_string;
			index_string << temp_pair.m_position_index << '.' << temp_pair.m_normal_index << '.' << temp_pair.m_tex_coord_index;
			std::string                     key          = index_string.str();
			index_buffer_map_type::iterator change_index = indix_buffer_map.find(key);

			(*this->m_triangles_groups)[tgr]->m_triangles[index / 3].m_vertices_index[index % 3] =
			    (*this->m_triangles_groups)[tgr]->m_triangles[index / 3].m_normals_index[index % 3] =
			        (*this->m_triangles_groups)[tgr]->m_triangles[index / 3].m_textures_index[index % 3] = change_index->second.m_index;
		}
	}
}

void CCBA::duplicate_backup_data()
{
	this->m_geometry_data->m_skinned_vertices_array = new float32_t[this->m_geometry_data->m_vertices_array_size];
	this->m_geometry_data->m_skinned_normals_array  = new float32_t[this->m_geometry_data->m_normals_array_size];

	memcpy((char8_t *) this->m_geometry_data->m_skinned_vertices_array, (char8_t *) this->m_geometry_data->m_vertices_array, this->m_geometry_data->m_vertices_array_size * sizeof(float32_t));
	memcpy((char8_t *) this->m_geometry_data->m_skinned_normals_array, (char8_t *) this->m_geometry_data->m_normals_array, this->m_geometry_data->m_normals_array_size * sizeof(float32_t));
}

void CCBA::duplicate_skinned_data()
{
	this->m_geometry_data->m_vertices_array = new float32_t[this->m_geometry_data->m_vertices_array_size];
	this->m_geometry_data->m_normals_array  = new float32_t[this->m_geometry_data->m_normals_array_size];

	//		float32_t *TempPointer = this->m_GeometryData->m_SkinnedVerticesArray;
	//		this->m_GeometryData->m_SkinnedVerticesArray = this->m_GeometryData->m_VerticesArray;
	//		this->m_GeometryData->m_VerticesArray = TempPointer;
	//
	//		TempPointer = this->m_GeometryData->m_SkinnedNormalsArray;
	//		this->m_GeometryData->m_SkinnedNormalsArray = this->m_GeometryData->m_NormalsArray;
	//		this->m_GeometryData->m_NormalsArray = TempPointer;

	memcpy((char8_t *) this->m_geometry_data->m_vertices_array, (char8_t *) this->m_geometry_data->m_skinned_vertices_array, this->m_geometry_data->m_vertices_array_size * sizeof(float32_t));
	memcpy((char8_t *) this->m_geometry_data->m_normals_array, (char8_t *) this->m_geometry_data->m_skinned_normals_array, this->m_geometry_data->m_normals_array_size * sizeof(float32_t));
}

void CCBA::fill_vertex_buffer_object()        //(uint32_t a_MaxSize,uint32_t a_PositionSizeVBO,uint32_t a_NormalSizeVBO,uint32_t a_TexCoordSizeVBO, float32_t *a_PositionDataVBO,float32_t *a_NormalDataVBO,float32_t *a_TexCoordDataVBO)
{
	// And put them directly in VBOs
	assert(nullptr != this->m_vbo && "VBO Creation Failed");

	// Fill the Data in VBO
	this->m_vbo->set_no_of_attributes(3);
	this->m_vbo->set_total_no_of_attributes(this->m_geometry_data->m_vertices_array_size / 3);
	ror::VertexAttributes *temp_attribs = rNew ror::VertexAttributes[3];        // = m_VBO->GetAttributes();

	temp_attribs[0].set_vertex_attribute(ror::VertexAttributes::rVERTICES);
	temp_attribs[0].set_offset(0);
	temp_attribs[0].set_no_of_components(3);        // For Positions
	temp_attribs[0].set_vbodata(this->m_geometry_data->m_skinned_vertices_array);

	temp_attribs[1].set_vertex_attribute(ror::VertexAttributes::rNORMALS);
	temp_attribs[1].set_offset(this->m_geometry_data->m_vertices_array_size * sizeof(float32_t));
	temp_attribs[1].set_no_of_components(3);        // For Normals
	temp_attribs[1].set_vbodata(this->m_geometry_data->m_skinned_normals_array);

	temp_attribs[2].set_vertex_attribute(ror::VertexAttributes::rTEXCOORDS);
	temp_attribs[2].set_offset((this->m_geometry_data->m_vertices_array_size * sizeof(float32_t)) + (this->m_geometry_data->m_normals_array_size * sizeof(float32_t)));
	temp_attribs[2].set_no_of_components(2);        // For TexCoords
	temp_attribs[2].set_vbodata(this->m_geometry_data->m_texture_coords_array);

	this->m_vbo->set_attributes(temp_attribs);
	this->m_vbo->set_size_of_vbo(
	    (this->m_geometry_data->m_vertices_array_size * sizeof(float32_t)) +
	    (this->m_geometry_data->m_normals_array_size * sizeof(float32_t)) +
	    (this->m_geometry_data->m_texture_coords_array_size * sizeof(float32_t)));
	this->m_vbo->load_vertex_buffer();
}

void CCBA::fill_index_buffer_object()
{
	assert(nullptr != this->m_ibo && "IBO Creation Failed");
	uint32_t              index_size_vbo;
	rUsShort **           index_data_vbo;
	ror::IndexAttributes *temp_index_attribs = rNew ror::IndexAttributes[this->m_triangles_groups->size()];
	index_data_vbo                           = new rUsShort *[this->m_triangles_groups->size()];
	uint32_t current_offset                  = 0;
	uint32_t a_indices_min                   = 0;
	uint32_t a_indices_max                   = 1;

	for (uint32_t tgr = 0; tgr < this->m_triangles_groups->size(); tgr++)
	{
		index_size_vbo      = (*this->m_triangles_groups)[tgr]->m_no_of_triangles * 3;
		index_data_vbo[tgr] = new rUsShort[index_size_vbo];

		for (uint32_t index = 0; index < index_size_vbo; index++)
		{
			index_data_vbo[tgr][index] = (uint32_t)(*this->m_triangles_groups)[tgr]->m_triangles[index / 3].m_vertices_index[index % 3];

			// Also Update the Max and Min Indices
			if (index_data_vbo[tgr][index] > a_indices_max)
				a_indices_max = index_data_vbo[tgr][index];

			if (index_data_vbo[tgr][index] < a_indices_min)
				a_indices_min = index_data_vbo[tgr][index];
		}

		// Fill the Data in Attributes for IBO for the current TriangleGroup
		temp_index_attribs[tgr].set_offset(current_offset);
		temp_index_attribs[tgr].set_ibodata(index_data_vbo[tgr]);
		temp_index_attribs[tgr].set_no_of_attribute(index_size_vbo);
		temp_index_attribs[tgr].set_indices_minimum(a_indices_min);
		temp_index_attribs[tgr].set_indices_maximum(a_indices_max);
		current_offset += index_size_vbo * sizeof(rUsShort);
	}

	// Fill the Data in IBO for the All TriangleGroups
	this->m_ibo->set_no_of_groups_in_ibo((uint32_t) this->m_triangles_groups->size());
	this->m_ibo->set_size_of_ibo(this->m_header->m_geometry_chunk.m_total_no_of_triangles * 3 * sizeof(rUsLong));
	this->m_ibo->set_attributes(temp_index_attribs);
	this->m_ibo->load_index_buffer();

	for (uint32_t tgr = 0; tgr < this->m_triangles_groups->size(); tgr++)
		rDELETEARRAY(index_data_vbo[tgr]);
	rDELETE(index_data_vbo);
}

rUsShort *CCBA::stripify(rUsShort *p_indices, uint32_t &numindices)
{
	// calls nvtristrips stripification function on the indices - resulting in tri-lists in cache friendly order
	/*
		PrimitiveGroup* primGroups;
		uint16_t numGroups;

		//try to use nvtristrip to optimise the order for the vertex cache
		if (!GenerateStrips(pIndices, numindices,&primGroups,&numGroups,false))
		return nullptr;

		if (primGroups->numIndices != numindices)
		numindices = primGroups->numIndices;

		//if we get here, the indices have been reordered and there are the right number
		//so we return the new array of indices
		return primGroups->indices;
		*/
	return 0;
}

void CCBA::calculate_bound()
{
	// Lets Update the Bounds First
	uint32_t  sizei = this->m_geometry_data->m_vertices_array_size;
	float32_t xmin = 0.0f, ymin = 0.0f, zmin = 0.0f;
	float32_t xmax = 0.0f, ymax = 0.0f, zmax = 0.0f;
	float32_t x = 0.0f, y = 0.0f, z = 0.0f;
	for (uint32_t bound_index = 0; bound_index < sizei; bound_index += 3)
	{
		x = this->m_geometry_data->m_vertices_array[bound_index];
		y = this->m_geometry_data->m_vertices_array[bound_index + 1];
		z = this->m_geometry_data->m_vertices_array[bound_index + 2];
		if (x < xmin)
			xmin = x;
		if (x > xmax)
			xmax = x;

		if (y < ymin)
			ymin = y;
		if (y > ymax)
			ymax = y;

		if (z < zmin)
			zmin = z;
		if (z > zmax)
			zmax = z;
	}
	Vector3f  v_min(xmin, ymin, zmin), v_max(xmax, ymax, zmax);
	Vector3f  v_center = (v_min + v_max) / 2;              // Midpoint
	float32_t radius   = abs(v_center.x - v_max.x);        //distance(vFace,vCenter);

	if (radius < abs(v_center.y - v_max.y))
		radius = abs(v_center.y - v_max.y);

	if (radius < abs(v_center.z - v_max.z))
		radius = abs(v_center.z - v_max.z);

	this->m_bound->set_center(v_center);
	this->m_bound->set_radius(radius);        // Approximate Distance First

	for (uint32_t bound_index = 0; bound_index < sizei; bound_index += 3)
	{
		Vector3f tem_point(
		    this->m_geometry_data->m_vertices_array[bound_index],
		    this->m_geometry_data->m_vertices_array[bound_index + 1],
		    this->m_geometry_data->m_vertices_array[bound_index + 2]);
		this->m_bound->add_point_to_bound(tem_point);
	}

	this->rotate_bound90degrees();        // Since 3D Studio Max Up is Z
}

void CCBA::rotate_bound90degrees()
{
	Matrix4f *rotation_around_x = rNew Matrix4f();
	glPushMatrix();
	glLoadIdentity();
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, rotation_around_x->m_values);
	glPopMatrix();

	Vector3f temp_center = this->m_bound->get_center();
	rotation_around_x->rotate_vector(temp_center);
	this->m_bound->set_center(temp_center);

	//		// Rotate Vertices
	//		//RotationAroundX->Transpose();
	//		uint32_t sizei = this->m_GeometryData->m_VerticesArraySize;
	//		for (uint32_t RotationIndex = 0; RotationIndex < sizei; RotationIndex += 3)
	//		{
	//			CVector3f temPoint(
	//				this->m_GeometryData->m_VerticesArray[RotationIndex    ],
	//				this->m_GeometryData->m_VerticesArray[RotationIndex + 1],
	//				this->m_GeometryData->m_VerticesArray[RotationIndex + 2]
	//			);
	//			RotationAroundX->RotateVector(temPoint.m_Values);
	//
	//			this->m_GeometryData->m_VerticesArray[RotationIndex    ] = temPoint.x;
	//			this->m_GeometryData->m_VerticesArray[RotationIndex + 1] = temPoint.y;
	//			this->m_GeometryData->m_VerticesArray[RotationIndex + 2] = temPoint.z;
	//		}
	//		// Rotate Normals
	//		sizei = this->m_GeometryData->m_NormalsArraySize;
	//		for (uint32_t RotationIndex = 0; RotationIndex < sizei; RotationIndex += 3)
	//		{
	//			CVector3f temPoint(
	//				this->m_GeometryData->m_NormalsArray[RotationIndex    ],
	//				this->m_GeometryData->m_NormalsArray[RotationIndex + 1],
	//				this->m_GeometryData->m_NormalsArray[RotationIndex + 2]
	//			);
	//			RotationAroundX->RotateVector(temPoint.m_Values);
	//			temPoint.Normalize();
	//			this->m_GeometryData->m_NormalsArray[RotationIndex    ] = temPoint.x;
	//			this->m_GeometryData->m_NormalsArray[RotationIndex + 1] = temPoint.y;
	//			this->m_GeometryData->m_NormalsArray[RotationIndex + 2] = temPoint.z;
	//		}

	rDELETE(rotation_around_x);
}

void CCBA::setup_inverse_bind_attributes()
{
	uint32_t number_of_vertices = this->m_geometry_data->m_vertices_array_size / 3;
	for (uint32_t current_vertex = 0; current_vertex < number_of_vertices; current_vertex++)
	{
		Vector3f vertex(this->m_geometry_data->m_vertices_array[(current_vertex * 3)],
		                this->m_geometry_data->m_vertices_array[(current_vertex * 3) + 1],
		                this->m_geometry_data->m_vertices_array[(current_vertex * 3) + 2]);

		Vector3f normal(this->m_geometry_data->m_normals_array[(current_vertex * 3)],
		                this->m_geometry_data->m_normals_array[(current_vertex * 3) + 1],
		                this->m_geometry_data->m_normals_array[(current_vertex * 3) + 2]);

		vertex = vertex * *this->m_skeleton_data->m_bind_shape_matrix;
		normal = normal * *this->m_skeleton_data->m_bind_shape_matrix;

		this->m_geometry_data->m_vertices_array[(current_vertex * 3)]     = vertex.x;
		this->m_geometry_data->m_vertices_array[(current_vertex * 3) + 1] = vertex.y;
		this->m_geometry_data->m_vertices_array[(current_vertex * 3) + 2] = vertex.z;

		this->m_geometry_data->m_normals_array[(current_vertex * 3)]     = normal.x;
		this->m_geometry_data->m_normals_array[(current_vertex * 3) + 1] = normal.y;
		this->m_geometry_data->m_normals_array[(current_vertex * 3) + 2] = normal.z;
	}
}

}        // namespace ror
