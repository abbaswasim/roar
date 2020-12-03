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

namespace ror
{
FORCE_INLINE CCBA::CCBA(void) :
    m_header(nullptr)        //, m_BoundTextureID(-1)
{
	this->m_geometry_data       = rNew GeometryData();
	this->m_skeleton_data       = rNew SkeletonData();
	this->m_triangles_groups    = new std::vector<ror::TriangleGroup *>();
	this->m_clips               = new std::unordered_map<EAnimationClips, AnimationClip *>();
	this->m_bound               = rNew BoundingSphere();
	this->m_vbo                 = rNew   ror::OGLVertexBuffer();
	this->m_ibo                 = rNew   ror::OGLIndexBuffer();
	this->m_animation_time      = 0.0f;
	this->m_animation_speed     = 1.0f;
	this->m_need_update         = true;
	this->m_has_dynamic_texture = false;
	this->m_current_clip        = nullptr;
	//this->m_TextureObject		= nullptr;
	//m_FirstUpdate		= true;
}

FORCE_INLINE void CCBA::draw()
{
	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

	//		if (nullptr != this->m_TextureObject)
	//		{
	//			this->m_TextureObject->BindTexture();
	//		}

	this->m_vbo->bind_buffer(true);
	this->m_ibo->bind_buffer(true);

	for (uint32_t i = 0; i < this->m_triangles_groups->size(); i++)
	{
		(*this->m_triangles_groups)[i]->m_material->bind_material();

		this->m_ibo->draw_elements(i);
	}

	//this->DrawSkeleton();
	this->m_vbo->bind_buffer(false);
	this->m_ibo->bind_buffer(false);

	//		if (nullptr != this->m_TextureObject)
	//		{
	//			this->m_TextureObject->BindTexture(false);
	//		}

	glPopMatrix();
}

FORCE_INLINE void CCBA::draw_skeleton()
{
#ifdef OPENGL_RENDERER
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_COLOR_MATERIAL);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	//glRotatef(-90,1,0,0);
	for (uint32_t i = 0; i < this->m_skeleton_data->m_no_of_bones; i++)
	{
		glColor3f(0.0f, 1.0f, 1.0f);
		ror::Utility::draw_sphere(
		    this->m_skeleton_data->m_bones[i]->m_world_matrix->translation_vector().x,
		    this->m_skeleton_data->m_bones[i]->m_world_matrix->translation_vector().y,
		    this->m_skeleton_data->m_bones[i]->m_world_matrix->translation_vector().z,
		    0.03f, 10, 10);
		if (this->m_skeleton_data->m_bones[i]->has_parent())
		{
			glBegin(GL_LINES);
			this->m_skeleton_data->m_bones[this->m_skeleton_data->m_bones[i]->m_parent_id]->m_world_matrix->translation_vector().call_gl_vertex();
			this->m_skeleton_data->m_bones[i]->m_world_matrix->translation_vector().call_gl_vertex();
			glEnd();
		}

		//			glColor3f(1.0f,0.0f,0.0f);
		//			CoreEngine::CUtility::DrawSphere(
		//				this->m_SkeletonData->m_Bones[i]->m_SkinningMatrix->TranslationVector().x,
		//				this->m_SkeletonData->m_Bones[i]->m_SkinningMatrix->TranslationVector().y,
		//				this->m_SkeletonData->m_Bones[i]->m_SkinningMatrix->TranslationVector().z,
		//				0.02f,10,10
		//				);
		//			if (this->m_SkeletonData->m_Bones[i]->HasParent())
		//			{
		//				glBegin(GL_LINES);
		//				this->m_SkeletonData->m_Bones[this->m_SkeletonData->m_Bones[i]->m_ParentID]->m_SkinningMatrix->TranslationVector().CallGlVertex();
		//				this->m_SkeletonData->m_Bones[i]->m_SkinningMatrix->TranslationVector().CallGlVertex();
		//				glEnd();
		//			}
	}
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glPopAttrib();

	this->reset_skeleton();
#endif
}

FORCE_INLINE bool CCBA::update(rUsLong a_mili_seconds)
{
	//return true;
	if (65536 & this->m_header->m_main_chunk.m_has_animation_has_skeleton)
	{
		uint32_t frame = calculate_current_frame(a_mili_seconds);

		if (this->m_need_update)
		{
			//this->m_AnimationSpeed += 0.001f;
			//this->EnableClip(rJUMP);
			this->setup_skeleton(frame);
			this->setup_bind_pose();
			this->m_vbo->update_vertex_buffer();
			this->m_need_update = false;
		}

		return true;
	}
	return false;
}

FORCE_INLINE uint32_t CCBA::calculate_current_frame(rUsLong a_mili_seconds)
{
	uint32_t frame;
	uint32_t previous_frame = (uint32_t)(this->m_animation_time * (float32_t) this->m_skeleton_data->m_no_of_keyframes / this->m_header->m_animation_chunk.m_total_time);
	//		uint32_t PreviousFrame = (uint32_t)(this->m_AnimationTime * (float32_t)(this->m_CurrentClip->m_EndKeyframe - this->m_CurrentClip->m_StartKeyframe)
	//								/ (this->m_CurrentClip->m_EndKeyTime - this->m_CurrentClip->m_StartKeyTime) );
	//
	this->m_animation_time += (float32_t) a_mili_seconds * 0.001f * this->m_animation_speed;

	if (this->m_animation_time > this->m_current_clip->m_end_key_time)
		this->m_animation_time = this->m_current_clip->m_start_key_time;

	//		if (this->m_AnimationTime > this->m_Header->m_AnimationChunk.m_TotalTime)
	//			this->m_AnimationTime = this->m_Header->m_AnimationChunk.m_StartTime;

	// For one Clip animation this is the Frame Calculator
	frame = (uint32_t)(this->m_animation_time * (float32_t) this->m_skeleton_data->m_no_of_keyframes / this->m_header->m_animation_chunk.m_total_time);

	// For Multi Clip animation this is the Frame Calculator
	//		Frame = (uint32_t)(this->m_AnimationTime * (float32_t)(this->m_CurrentClip->m_EndKeyframe - this->m_CurrentClip->m_StartKeyframe)
	//							/ (this->m_CurrentClip->m_EndKeyTime - this->m_CurrentClip->m_StartKeyTime) );

	if (frame == previous_frame)
		this->m_need_update = false;
	else
		this->m_need_update = true;

	return frame;
}

FORCE_INLINE void CCBA::setup_skeleton(int32_t a_frame)
{
	if (a_frame == -1)
	{
		*this->m_skeleton_data->m_root_bone->m_world_matrix = *this->m_skeleton_data->m_root_bone->m_joint_matrix;
		// Update the tree of bones
		static Stack<int32_t> bones_stack;
		bones_stack.push(this->m_skeleton_data->m_root_bone->m_id);

		while (!bones_stack.empty())
		{
			uint32_t current_bone = bones_stack.pop();
			assert(current_bone != -1 && "Current Bone is Null");

			Matrix4f joint_matrix = *this->m_skeleton_data->m_bones[current_bone]->m_joint_matrix;

			if (this->m_skeleton_data->m_bones[current_bone]->has_parent())
			{
				joint_matrix                                                  = joint_matrix * *this->m_skeleton_data->m_bones[this->m_skeleton_data->m_bones[current_bone]->m_parent_id]->m_world_matrix;
				*this->m_skeleton_data->m_bones[current_bone]->m_world_matrix = joint_matrix;
			}

			// Handle its Children
			if (this->m_skeleton_data->m_bones[current_bone]->has_children())
			{
				uint32_t no_of_children = this->m_skeleton_data->m_bones[current_bone]->m_child_count;
				for (uint32_t index = 0; index < no_of_children; index++)
				{
					bones_stack.push(this->m_skeleton_data->m_bones[current_bone]->get_child_at(index));
				}
			}
		}        // While
	}
	else
	{
		// Update the tree of bones
		static Stack<int32_t> bones_stack;
		bones_stack.push(this->m_skeleton_data->m_root_bone->m_id);

		while (!bones_stack.empty())
		{
			uint32_t current_bone = bones_stack.pop();
			assert(current_bone != -1 && "Current Bone is Null");

			Matrix4f world_matrix = *this->m_skeleton_data->m_bones[current_bone]->m_joint_matrix;

			if (this->m_skeleton_data->m_bones[current_bone]->m_no_of_keyframes > 0 /* && frame > 0*/)
			{
				assert(a_frame < (int32_t) this->m_skeleton_data->m_bones[current_bone]->m_no_of_keyframes && "Invalid Key frame");

				float32_t in_between = (float32_t)(this->m_animation_time * (float32_t) this->m_skeleton_data->m_no_of_keyframes / this->m_header->m_animation_chunk.m_total_time);
				in_between -= a_frame;

				if (a_frame < (int32_t) this->m_skeleton_data->m_bones[current_bone]->m_no_of_keyframes - 1)
				{
					world_matrix = interpolate_matrix(*this->m_skeleton_data->m_bones[current_bone]->m_keyframes[a_frame]->m_transform,
					                                  *this->m_skeleton_data->m_bones[current_bone]->m_keyframes[a_frame + 1]->m_transform, in_between);
				}
				else
					world_matrix = *this->m_skeleton_data->m_bones[current_bone]->m_keyframes[a_frame]->m_transform;
			}

			if (this->m_skeleton_data->m_bones[current_bone]->has_parent())
				world_matrix = world_matrix * *this->m_skeleton_data->m_bones[this->m_skeleton_data->m_bones[current_bone]->m_parent_id]->m_world_matrix;

			*this->m_skeleton_data->m_bones[current_bone]->m_world_matrix    = world_matrix;
			*this->m_skeleton_data->m_bones[current_bone]->m_skinning_matrix = *this->m_skeleton_data->m_bones[current_bone]->m_inverse_bind_matrix * world_matrix;

			// Handle its Children
			if (this->m_skeleton_data->m_bones[current_bone]->has_children())
			{
				uint32_t no_of_children = this->m_skeleton_data->m_bones[current_bone]->m_child_count;
				for (uint32_t index = 0; index < no_of_children; index++)
				{
					bones_stack.push(this->m_skeleton_data->m_bones[current_bone]->get_child_at(index));
				}
			}
		}        // While
	}
	//		for (uint32_t i = 0;i < this->m_SkeletonData->m_NoOfBones; i++)
	//			*this->m_SkeletonData->m_Bones[i]->m_SkinningMatrix =	*this->m_SkeletonData->m_Bones[i]->m_InverseBindMatrix *
	//																	*this->m_SkeletonData->m_Bones[i]->m_SkinningMatrix;
}

FORCE_INLINE void CCBA::setup_bind_pose()
{
	/*
		The skinning calculation for each vertex v in a bind shape is
		for i to n
			v += {[(v * BSM) * IBMi * JMi] * JW}

		• n: The number of joints that influence vertex v
		• BSM: Bind-shape matrix
		• IBMi: Inverse bind-pose matrix of joint i
		• JMi: Transformation matrix of joint i
		• JW: Weight of the influence of joint i on vertex v

		I have Got (v * BSM) and (IBMi * JMi) already multiplied since they are constants
		*/
	uint32_t         number_of_vertices = this->m_geometry_data->m_vertices_array_size / 3;
	VertexInfluence *temp_vertex_influence;
	temp_vertex_influence = nullptr;

	for (uint32_t current_vertex = 0; current_vertex < number_of_vertices; current_vertex++)
	{
		Vector3f temp_vertex;
		Vector3f temp_normal;
		Vector3f temp_normal_transform;

		uint32_t current_vertex_times3 = current_vertex * 3;

		Vector3f vertex(this->m_geometry_data->m_vertices_array[current_vertex_times3],
		                this->m_geometry_data->m_vertices_array[current_vertex_times3 + 1],
		                this->m_geometry_data->m_vertices_array[current_vertex_times3 + 2]);

		Vector3f normal(this->m_geometry_data->m_normals_array[current_vertex_times3],
		                this->m_geometry_data->m_normals_array[current_vertex_times3 + 1],
		                this->m_geometry_data->m_normals_array[current_vertex_times3 + 2]);

		float32_t total_joints_weight = 0;
		float32_t normalized_weight   = 0;

		temp_vertex_influence = (*this->m_geometry_data->m_vertex_influences)[current_vertex];

		uint32_t influence_count = temp_vertex_influence->m_no_of_influences;
		for (uint32_t current_influence = 0; current_influence < influence_count; current_influence++)
		{
			temp_vertex += ((vertex *
			                 *this->m_skeleton_data->m_bones[temp_vertex_influence->m_joints[current_influence]]->m_skinning_matrix) *
			                this->m_geometry_data->m_vertex_weights_array[temp_vertex_influence->m_weights[current_influence]]);

			this->m_skeleton_data->m_bones[temp_vertex_influence->m_joints[current_influence]]->m_skinning_matrix->rotate_vector(normal, temp_normal_transform);

			temp_normal += temp_normal_transform * this->m_geometry_data->m_vertex_weights_array[temp_vertex_influence->m_weights[current_influence]];

			//				TempNormal	+= ((Normal *
			//					*this->m_SkeletonData->m_Bones[(*this->m_GeometryData->m_VertexInfluences)[CurrentVertex]->m_Joints[CurrentInfluence]]->m_SkinningMatrix) *
			//					(*this->m_GeometryData->m_VertexInfluences)[CurrentVertex]->m_Weights[CurrentInfluence]);

			total_joints_weight += this->m_geometry_data->m_vertex_weights_array[temp_vertex_influence->m_weights[current_influence]];
		}

		if (total_joints_weight != 1.0f)
		{
			normalized_weight = 1.0f / total_joints_weight;
			temp_vertex *= normalized_weight;
			temp_normal *= normalized_weight;
		}

		this->m_geometry_data->m_skinned_vertices_array[current_vertex_times3]     = temp_vertex.x;
		this->m_geometry_data->m_skinned_vertices_array[current_vertex_times3 + 1] = temp_vertex.y;
		this->m_geometry_data->m_skinned_vertices_array[current_vertex_times3 + 2] = temp_vertex.z;

		this->m_geometry_data->m_skinned_normals_array[current_vertex_times3]     = temp_normal.x;
		this->m_geometry_data->m_skinned_normals_array[current_vertex_times3 + 1] = temp_normal.y;
		this->m_geometry_data->m_skinned_normals_array[current_vertex_times3 + 2] = temp_normal.z;
	}
}

FORCE_INLINE void CCBA::reset_skeleton()
{
	for (uint32_t current_bone = 0; current_bone < this->m_skeleton_data->m_no_of_bones; current_bone++)
		this->m_skeleton_data->m_bones[current_bone]->reset();
}

FORCE_INLINE bool CCBA::has_skeleton() const
{
	return (65536 & this->m_header->m_main_chunk.m_has_animation_has_skeleton ? true : false);
}

FORCE_INLINE bool CCBA::has_animation() const
{
	return (1 & this->m_header->m_main_chunk.m_has_animation_has_skeleton ? true : false);
}

FORCE_INLINE bool CCBA::get_has_dynamic_texture() const
{
	return m_has_dynamic_texture;
}

FORCE_INLINE void CCBA::set_has_dynamic_texture(bool a_has_dynamic_texture)
{
	m_has_dynamic_texture = a_has_dynamic_texture;
}

FORCE_INLINE bool CCBA::get_use_vbo() const
{
	/*return m_UseVBO;*/
}

FORCE_INLINE void CCBA::set_use_vbo(bool a_use_vbo)
{
	/*m_UseVBO = a_UseVBO;*/
}

FORCE_INLINE ccbaheader *CCBA::get_header() const
{
	return this->m_header;
}

FORCE_INLINE void CCBA::set_header(ccbaheader *a_header)
{
	rDELETE(this->m_header);
	this->m_header = a_header;
}

FORCE_INLINE char8_t *CCBA::get_texture_file_name()
{
	return this->m_header->m_geometry_chunk.m_texture_file_name;
}

FORCE_INLINE BoundingSphere *CCBA::get_bound() const
{
	return this->m_bound;
}

FORCE_INLINE void CCBA::set_bound(BoundingSphere *a_bound)
{
	rDELETE(this->m_bound);
	this->m_bound = a_bound;
}

FORCE_INLINE std::vector<TriangleGroup *> *CCBA::get_triangles_groups() const
{
	return this->m_triangles_groups;
}

FORCE_INLINE void CCBA::set_triangles_groups(std::vector<TriangleGroup *> *a_triangle_group)
{
	this->m_triangles_groups = a_triangle_group;
}

FORCE_INLINE GeometryData *CCBA::get_geometry_data() const
{
	return this->m_geometry_data;
}

FORCE_INLINE void CCBA::set_geometry_data(GeometryData *a_geometry_data)
{
	rDELETE(this->m_geometry_data);
	this->m_geometry_data = a_geometry_data;
}

FORCE_INLINE void CCBA::enable_clip(EAnimationClips a_clip_name)
{
	std::unordered_map<EAnimationClips, AnimationClip *>::iterator iter = this->m_clips->find(a_clip_name);
	if (iter != this->m_clips->end())
	{
		this->m_current_clip = iter->second;
	}
}

}        // namespace ror
