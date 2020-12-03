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

#include "appearance/rorlight.h"
#include "camera/rorcamera.h"
#include "data_structure/rorstack.h"
#include "profiling/rorlog.h"

#include "math/rormatrix4.h"
#include "math/rorvector3.h"
#include "opengl/rorogl_shader_program.h"
#include "rormoveable.h"
#include "rorsg_node.h"
//#include "OpenGL/ROROGLRenderingDevice.h"

namespace ror
{
class ROAR_ENGINE_ITEM Entities : public ror::csgnode, public ror::Moveable
{
  public:
	FORCE_INLINE Entities(void);
	virtual ~Entities(void);

	enum ENodeType
	{
		rEMPTY,
		rGEOMETRY,
		rBILLBOARD,
		rTERRAIN,
		rCAMERA,
		rLIGHT,
		rDUMMY,
		rCLOUDS,
		rPARTICLES,
		rNODE_TYPE_MAX
	};

	// EObjectTypes can be "OR"ed together
	enum EObjectType
	{
		rNONE                       = 0,
		rRENDERABLE                 = 1,
		rSHADOW_CASTER_RECIEVER     = 2,
		rNON_SHADOW_CASTER_RECIEVER = 4,
		rSHADOW_CASTER              = 8,
		rSHADOW_RECIEVER            = 16,
		rCOLLIDABLE                 = 32,
		rTRANSPARENT                = 64
	};

	enum ERenderingPass
	{
		rRENDERING_PASS,
		rSHADOWING_PASS,
		rNON_SHADOWING_PASS,
		rSHADOWING_PASS_ONE,
		rSHADOWING_PASS_TWO,
		rSHADOWING_PASS_THREE,
		rSPECULARHIGHTING
	};

	// Public Utility wrappers for the Virtual Counterparts
	FORCE_INLINE void draw();
	FORCE_INLINE void draw_recurse();
	FORCE_INLINE void draw_in_world_space();
	FORCE_INLINE void update(rUsLong a_milli_seconds);
	FORCE_INLINE void update_newtonian(rUsLong a_milli_seconds);
	FORCE_INLINE void prepair();
	FORCE_INLINE void check_collision(Entities *a_other_entity);

	FORCE_INLINE void draw_axis();
	FORCE_INLINE void draw_line_to_parent();
	FORCE_INLINE void draw_parent_lines();
	FORCE_INLINE void delete_scene_graph();
	FORCE_INLINE Vector2i generate_visibility_set(const ror::Camera *a_active_camera);
	FORCE_INLINE const BoundingSphere *get_bound() const;
	FORCE_INLINE void                  set_bound(ror::BoundingSphere *a_bound);
	FORCE_INLINE ENodeType get_node_type() const;
	FORCE_INLINE void      set_node_type(ENodeType a_node_type);
	FORCE_INLINE const BoundingSphere *get_tree_bound() const;
	FORCE_INLINE void                  set_tree_bound(ror::BoundingSphere *a_bound);
	FORCE_INLINE OGLShaderProgram *get_shader() const;
	FORCE_INLINE void              set_shader(ror::OGLShaderProgram *a_shader);
	FORCE_INLINE void              get_transformation_matrix(ror::Matrix4f &a_matrix);
	FORCE_INLINE void              update_transformation_matrices();
	FORCE_INLINE void              set_transformation_matrices(ror::Matrix4f *a_local_matrix);
	/*FORCE_INLINE void					SetTransformationMatrices(float32_t *a_LocalMatrix);*/
	FORCE_INLINE Matrix4f *get_local_transformation() const;
	/*FORCE_INLINE void					SetLocalTransformation(float32_t *a_Matrix);*/
	FORCE_INLINE void  set_local_transformation(ror::Matrix4f *a_matrix);
	FORCE_INLINE const Matrix4f *get_world_transformation() const;
	/*FORCE_INLINE void					SetWorldTransformation(float32_t *a_Matrix);*/
	FORCE_INLINE void set_world_transformation(ror::Matrix4f *a_matrix);
	FORCE_INLINE void setup_orientation_and_placement_vectors();
	FORCE_INLINE void set_orientation_vectors();
	FORCE_INLINE void set_position_and_bound_vectors();
	FORCE_INLINE int32_t get_object_type() const;
	FORCE_INLINE void    set_object_type(int32_t a_object_type);

	static ERenderingPass get_current_render_pass();
	static void           set_current_render_pass(ror::Entities::ERenderingPass a_render_pass);

  protected:
	virtual void global_draw()                                    = 0;
	virtual void global_update(rUsLong a_milli_seconds)           = 0;
	virtual void global_prepair()                                 = 0;
	virtual void global_check_collision(Entities *a_other_entity) = 0;
	virtual void global_update_newtonian(rUsLong a_milli_seconds) = 0;

	ror::Matrix4f *        m_local_transformation;        // Local-Coordinate Transformation
	ror::Matrix4f *        m_world_transformation;        // World-Coordinate Transformation
	ror::BoundingSphere *  m_bound;                       // Pointer to Bounding Sphere of the Node
	ror::BoundingSphere *  m_tree_bound;                  // Bounding Sphere of the Subtree
	ror::OGLShaderProgram *m_shaders;                     // Pointer to the Shader Program
	EVisibiltyInFrustrum   m_visibility;                  // Visibility in Frustrum
	ENodeType              m_node_type;                   // Can be Dummy, Camera, Light, Geometry etc type of Node
	int32_t                m_object_type;                 // Can be Renderable, Collidable, Shadow Caster etc type of Node

	static bool                      m_registered;                 // For First Time Use of The Scene Graph
	static Vector3f                  m_origin_offset;              // Its Not used (though i don't remember why i put it in the first place)
	static Stack<OGLShaderProgram *> m_shaders_stack;              // Stack of Shaders which are used through out the Draw
	static ERenderingPass            m_current_render_pass;        // Defines which pass is the current rendering going on for
};
}        // namespace ror

#include "rorentities.hh"
