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

#pragma once

//#include "NvTriStrip.h" // For doing the Triangle Strips

#include "rorcba_header.h"
#include "rorcba_instance.h"

#include "animation/roranimation_clip.h"
#include "animation/rorskeleton_data.h"
#include "memory_manager/rormemory_check.h"
#include "profiling/rorlog.h"

#include "geometry/rorgeometry_data.h"
#include "geometry/rorindex_count_pair.h"
#include "geometry/rortriangle_group.h"
#include "geometry/rorvertex_influence.h"
#include "geometry/rorweighted_vertex.h"

#include "bounds/rorbounding_sphere.h"
#include "data_structure/rorstack.h"
#include "io/rorexport.h"
#include "math/rormatrix4.h"
#include "opengl/rorogl_index_buffer.h"
#include "opengl/rorogl_texture_manager.h"
#include "opengl/rorogl_vertex_buffer.h"
#include "thirdparty/tinyxml/tinyxml.h"
//#include INCLUDERENDERER

namespace ror
{
//	class ROAR_ENGINE_ITEM COpenGL;
class ROAR_ENGINE_ITEM CCBA
{
  public:
	FORCE_INLINE CCBA(void);
	~CCBA(void);

	FORCE_INLINE void draw();
	FORCE_INLINE bool update(rUsLong a_mili_seconds);
	FORCE_INLINE uint32_t calculate_current_frame(rUsLong a_mili_seconds);
	FORCE_INLINE void     draw_skeleton();
	FORCE_INLINE void     reset_skeleton();
	FORCE_INLINE bool     has_skeleton() const;
	FORCE_INLINE bool     has_animation() const;
	FORCE_INLINE bool     get_use_vbo() const;
	FORCE_INLINE void     set_use_vbo(bool a_use_vbo);
	FORCE_INLINE ccbaheader *get_header() const;
	FORCE_INLINE void        set_header(ccbaheader *a_header);
	FORCE_INLINE char8_t *get_texture_file_name();
	FORCE_INLINE BoundingSphere *get_bound() const;
	FORCE_INLINE void            set_bound(BoundingSphere *a_bound);
	FORCE_INLINE std::vector<TriangleGroup *> *get_triangles_groups() const;
	FORCE_INLINE void                          set_triangles_groups(std::vector<TriangleGroup *> *a_triangle_group);
	FORCE_INLINE GeometryData *get_geometry_data() const;
	FORCE_INLINE void          set_geometry_data(GeometryData *a_geometry_data);
	FORCE_INLINE void          setup_skeleton(int32_t a_frame);
	FORCE_INLINE void          setup_bind_pose();
	FORCE_INLINE void          enable_clip(EAnimationClips a_clip_name);
	FORCE_INLINE bool          get_has_dynamic_texture() const;
	FORCE_INLINE void          set_has_dynamic_texture(bool a_has_dynamic_texture);

	void read_cba(std::string a_file_name);

  protected:
	void read_header(Export *a_file, std::string a_file_name);
	void read_array(Export *a_file, float32_t **a_destination, uint32_t &a_size);
	void read_triangles(Export *a_file);
	void read_skeleton(Export *a_file);
	void read_animations(Export *a_file);
	void read_clips_information(std::string a_file_name);

	void write_cba(std::string a_file_name);
	void write_header(Export *a_file);
	void write_array(Export *a_file, float32_t *a_destination, uint32_t &a_size);
	void write_triangles(Export *a_file);
	void write_skeleton(Export *a_file);
	void write_animations(Export *a_file);

	void calculate_bound();
	void convert_to_arrays();
	void duplicate_skinned_data();
	void duplicate_backup_data();
	void rotate_bound90degrees();
	void fill_vertex_buffer_object();        //uint32_t a_MaxSize,uint32_t a_PositionSizeVBO,uint32_t a_NormalSizeVBO,uint32_t a_TexCoordSizeVBO,
	                                         //float32_t *a_PositionDataVBO,float32_t *a_NormalDataVBO,float32_t *a_TexCoordDataVBO);
	void      fill_index_buffer_object();
	rUsShort *stripify(rUsShort *p_indices, uint32_t &numindices);
	//void							SetupBindPose();
	//void							SetupTexture();
	void setup_inverse_bind_attributes();

  private:
	//CCBAInstance							*m_Instance;			// CBA models are instanced in the Scene Graph this is what is used
	ccbaheader *                                          m_header;                  // Header (Contains information about the file)
	GeometryData *                                        m_geometry_data;           // Common data in the Mesh (List of Vertices/Normals/TexCoords)
	SkeletonData *                                        m_skeleton_data;           // Skeleton Data used for animation
	std::vector<TriangleGroup *> *                        m_triangles_groups;        // Individual Triangle Groups Separated By Material Data (Just Indices and Material)
	std::unordered_map<EAnimationClips, AnimationClip *> *m_clips;                   // Animation Clips information
	AnimationClip *                                       m_current_clip;            // Animation Clip

	OGLVertexBuffer *m_vbo;          // VBO used for rendering the Model
	OGLIndexBuffer * m_ibo;          // IBO used for rendering the Model
	BoundingSphere * m_bound;        // Local Bound of the Model
	//CTextureObject							*m_TextureObject;		// The texture attached with Model
	bool      m_need_update;                // Should be Updated or Not
	bool      m_has_dynamic_texture;        // If Its texture is rendered Dynamically
	float32_t m_animation_time;             // Animation Counter
	float32_t m_animation_speed;            // Animation Speed Scale
	                                        //bool									 m_HasTexture;			// Does a texture exists for this Model
	                                        //uint32_t								 m_HasAnimationSkeleton;	// If is animated with skeleton
};
}        // namespace ror

#include "rorcba.inl"
