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
FORCE_INLINE Entities::Entities(void)
{
	this->m_bound = rNew ror::BoundingSphere();
	//this->m_Shaders				= rNew CoreEngine::COGLShaderProgram();
	this->m_shaders              = nullptr;
	this->m_tree_bound           = rNew           ror::BoundingSphere();
	this->m_world_transformation = rNew ror::Matrix4f();
	this->m_local_transformation = rNew ror::Matrix4f();

	this->m_node_type   = rEMPTY;
	this->m_object_type = rSHADOW_CASTER_RECIEVER;
	this->m_visibility  = rINSIDE;
}

FORCE_INLINE void Entities::draw_in_world_space()        // Testing :
{
#ifdef OPENGL_RENDERER

	// Drawing the Tree
	glPushMatrix();
	//glMultMatrixf(this->m_WorldTransformation->m_Values);
	Stack<Entities *> elements_stack;
	elements_stack.push(this);
	//static GLUquadric *qd = gluNewQuadric();
	while (!elements_stack.empty())
	{
		Entities *current_object = elements_stack.pop();
		assert(current_object != nullptr && "Current Object is Null");

		// Draw CurrentObject
		glPushMatrix();
		current_object->m_position->call_gl_translate();
		//glMultMatrixf(CurrentObject->m_WorldTransformation->m_Values);		// Testing ::
		//gluSphere(qd,0.1,10,10);
		ror::Utility::draw_sphere(0.0f, 0.0f, 0.0f, 0.1f, 10, 10);
		glPopMatrix();

		// Handle its Children
		if (current_object->has_child())
		{
			std::vector<csgnode *>::iterator index = current_object->m_child_node->begin();
			for (; index < current_object->m_child_node->end(); index++)
			{
				elements_stack.push(dynamic_cast<Entities *>(*(index)));
			}
		}
	}        // While
	glPopMatrix();
#endif
}

/*FORCE_INLINE void CEntities::Draw()
	{
		// Drawing the Tree
		static CStack<CEntities*> ElementsStack;
		ElementsStack.Push(this);

		glPushMatrix();
		glMultMatrixf(this->m_WorldTransformation->m_Values);

		while (!ElementsStack.Empty())
		{
			CEntities *CurrentObject = ElementsStack.Pop();
			assert(CurrentObject != nullptr  && "Current Object is Null");

			// Draw CurrentObject
			glPushMatrix();
			glMultMatrixf(CurrentObject->m_LocalTransformation->m_Values);

			if (rOUTSIDE != CurrentObject->m_Visibility)
			{
				CurrentObject->Update();
				CurrentObject->GlobalDraw();
				//CurrentObject->DrawLocalAxis();
				//CurrentObject->m_Bound->DrawBound();
			}

			// Handle its Children
			if (CurrentObject->HasChild())
			{
				std::vector<CSGNode*>::iterator Index = CurrentObject->m_ChildNode->begin();
				for (; Index < CurrentObject->m_ChildNode->end(); Index++)
				{
					ElementsStack.Push(dynamic_cast<CEntities*>(*(Index)));
				}
			}
			else
			{
				glPopMatrix();
				// So if they are not siblings discard Parent Node's Transformation
				if (ElementsStack.Empty())
				{
					glPopMatrix();
					glPopMatrix();
				}
				else if (CurrentObject->m_ParentNode != nullptr)
				{
					if (CurrentObject->m_ParentNode != ElementsStack.Top()->m_ParentNode)
					{
						glPopMatrix();
					}
				}
			}
		} // While
		//////////////////////////////////////////////////////////////////////////
		//CoreEngine::COGLShaderProgram::Disable();
		glPopMatrix();
		rGLEnum t = glGetError();
		if (GL_NO_ERROR != t)
		{
		}
	}*/

FORCE_INLINE void Entities::draw()
{
	glPushMatrix();
	//m_OriginOffset.CallGlTranslate();
	//glMultMatrixf(this->m_WorldTransformation->m_Values);					// Testing ::
	this->draw_recurse();
	//////////////////////////////////////////////////////////////////////////
	//		uint32_t E = CoreEngine::COGLShaderProgram::m_Enables;
	//		uint32_t D = CoreEngine::COGLShaderProgram::m_Disables;

	GLenum t3 = glGetError();
	if (GL_NO_ERROR != t3)
	{
	}
	glPopMatrix();
}

FORCE_INLINE void Entities::draw_recurse()
{
	this->m_shaders_stack.push(this->m_shaders);

	if (nullptr != this->m_shaders_stack.top())
		(this->m_shaders_stack.top())->enable();

	// Draw CurrentObject
	glPushMatrix();
	glMultMatrixf(this->m_local_transformation->m_values);        // Testing ::

	if (rOUTSIDE != this->m_visibility)
	{
		if (rRENDERING_PASS == this->m_current_render_pass)
		{
			//this->Update(0);
			this->global_draw();
		}
		else if (rNON_SHADOWING_PASS == this->m_current_render_pass && (rNON_SHADOW_CASTER_RECIEVER & this->m_object_type))
		{
			//this->Update(0);
			this->global_draw();
		}
		else if ((rSHADOWING_PASS == this->m_current_render_pass) && (rSHADOW_CASTER_RECIEVER & this->m_object_type))
		{
			this->global_draw();
		}
	}

	// Handle its Children
	if (this->has_child())
	{
		std::vector<csgnode *>::iterator index = this->m_child_node->begin();
		for (; index < this->m_child_node->end(); index++)
		{
			((Entities *) *index)->draw_recurse();
		}
	}

	glPopMatrix();

	//		if (rLIGHT == this->m_NodeType)
	//		{
	//			(dynamic_cast<CLight*>(this))->Disable();
	//		}

	if (nullptr != this->m_shaders_stack.top())
		(this->m_shaders_stack.top())->disable();

	this->m_shaders_stack.pop();

	if (!this->m_shaders_stack.empty())
		if (nullptr != this->m_shaders_stack.top())
			(this->m_shaders_stack.top())->enable();
}

FORCE_INLINE void Entities::draw_axis()
{
	// Drawing the Tree
	static Stack<Entities *> elements_stack;
	elements_stack.push(this);

	glPushMatrix();
	while (!elements_stack.empty())
	{
		Entities *current_object = elements_stack.pop();
		assert(current_object != nullptr && "Current Object is Null");

		current_object->draw_world_axis();

		// Handle its Children
		if (current_object->has_child())
		{
			std::vector<csgnode *>::iterator index = current_object->m_child_node->begin();
			for (; index < current_object->m_child_node->end(); index++)
			{
				elements_stack.push(dynamic_cast<Entities *>(*(index)));
			}
		}
	}        // While
	glPopMatrix();
}

FORCE_INLINE void Entities::draw_parent_lines()
{
	// Drawing the Tree
	glPushMatrix();
	//glMultMatrixf(this->m_WorldTransformation->m_Values);
	static Stack<Entities *> elements_stack;
	elements_stack.push(this);

	while (!elements_stack.empty())
	{
		Entities *current_object = elements_stack.pop();
		assert(current_object != nullptr && "Current Object is Null");

		//CoreEngine::CUtility::DrawSphere(0,0,0,1,10,10);
		current_object->draw_line_to_parent();

		// Handle its Children
		if (current_object->has_child())
		{
			std::vector<csgnode *>::iterator index = current_object->m_child_node->begin();
			for (; index < current_object->m_child_node->end(); index++)
			{
				elements_stack.push(dynamic_cast<Entities *>(*(index)));
			}
		}
	}        // While
	glPopMatrix();
}

FORCE_INLINE void Entities::draw_line_to_parent()
{
#ifdef OPENGL_RENDERER

	if (nullptr != this->m_parent_node)
	{
		glBegin(GL_LINES);
		((Entities *) this->m_parent_node)->m_position->call_gl_vertex();
		this->m_position->call_gl_vertex();
		glEnd();
	}
#endif
}

FORCE_INLINE void Entities::update(rUsLong a_milli_seconds)
{
	static Stack<Entities *> elements_stack;
	elements_stack.push(this);

	while (!elements_stack.empty())
	{
		Entities *current_object = elements_stack.pop();
		assert(current_object != nullptr && "Current Object is Null");

		if (rOUTSIDE != current_object->m_visibility)
		{
			current_object->global_update(a_milli_seconds);
		}

		current_object->update_newtonian(a_milli_seconds);
		// Handle its Children
		if (current_object->has_child())
		{
			std::vector<csgnode *>::iterator index = current_object->m_child_node->begin();
			for (; index < current_object->m_child_node->end(); index++)
			{
				elements_stack.push(dynamic_cast<Entities *>(*(index)));
			}
		}
	}        // While
}

FORCE_INLINE void Entities::update_newtonian(rUsLong a_milli_seconds)
{
	this->global_update_newtonian(a_milli_seconds);
}

FORCE_INLINE void Entities::prepair()
{
	this->global_prepair();
}

FORCE_INLINE void Entities::check_collision(Entities *a_other_entity)
{}

FORCE_INLINE Matrix4f *Entities::get_local_transformation() const
{
	return this->m_local_transformation;
}

FORCE_INLINE void Entities::set_local_transformation(Matrix4f *a_matrix)
{
	memcpy(this->m_local_transformation->m_values, a_matrix->m_values, sizeof(float32_t) * 16);
}

//	FORCE_INLINE void CEntities::SetLocalTransformation( float32_t *a_Matrix )
//	{
//		memcpy(this->m_LocalTransformation->m_Values, a_Matrix, sizeof(float32_t) * 16);
//	}

FORCE_INLINE const Matrix4f *Entities::get_world_transformation() const
{
	return this->m_world_transformation;
}

FORCE_INLINE void Entities::set_world_transformation(Matrix4f *a_matrix)
{
	memcpy(this->m_world_transformation->m_values, a_matrix->m_values, sizeof(float32_t) * 16);
}

//	FORCE_INLINE void CEntities::SetWorldTransformation( float32_t *a_Matrix )
//	{
//		memcpy(this->m_WorldTransformation->m_Values, a_Matrix, sizeof(float32_t) * 16);
//	}

FORCE_INLINE const BoundingSphere *Entities::get_bound() const
{
	return this->m_bound;
}

FORCE_INLINE void Entities::set_bound(BoundingSphere *a_bound)
{
	rDELETE(this->m_bound);
	this->m_bound = a_bound;
}

FORCE_INLINE const BoundingSphere *Entities::get_tree_bound() const
{
	return this->m_tree_bound;
}

FORCE_INLINE void Entities::set_tree_bound(BoundingSphere *a_bound)
{
	rDELETE(this->m_tree_bound);

	this->m_tree_bound = a_bound;
}

FORCE_INLINE OGLShaderProgram *Entities::get_shader() const
{
	return m_shaders;
}

FORCE_INLINE void Entities::set_shader(ror::OGLShaderProgram *a_shaders)
{
	rDELETE(m_shaders);
	m_shaders = a_shaders;
}

FORCE_INLINE void Entities::set_transformation_matrices(Matrix4f *a_local_matrix)
{
	this->set_local_transformation(a_local_matrix);
	if (nullptr != this->m_parent_node)
	{
		*a_local_matrix = *a_local_matrix * *((Entities *) this->m_parent_node)->m_world_transformation;
		this->set_world_transformation(a_local_matrix);
	}
	else
	{
		this->set_world_transformation(a_local_matrix);
		//m_OriginOffset = a_LocalMatrix->TranslationVector();
	}
	this->setup_orientation_and_placement_vectors();
}

FORCE_INLINE void Entities::update_transformation_matrices()
{
	Vector3f position_delta = *this->m_position - this->m_world_transformation->translation_vector();
	this->m_local_transformation->set_translation_vector(this->m_local_transformation->translation_vector() + position_delta);

	if (nullptr != this->m_parent_node)
	{
		*this->m_world_transformation = *this->m_local_transformation * *((Entities *) this->m_parent_node)->m_world_transformation;
	}
	else
	{
		*this->m_world_transformation = *this->m_local_transformation;
	}
	this->set_position_and_bound_vectors();
}

FORCE_INLINE void Entities::setup_orientation_and_placement_vectors()
{
	this->set_orientation_vectors();
	this->set_position_and_bound_vectors();
}

FORCE_INLINE void Entities::set_orientation_vectors()
{
	*this->m_forward_vector = this->m_local_transformation->forward_vector();
	*this->m_up_vector      = this->m_local_transformation->up_vector();
	*this->m_right_vector   = this->m_local_transformation->right_vector();

	this->m_forward_vector->normalize();
	this->m_up_vector->normalize();
	this->m_right_vector->normalize();
}

FORCE_INLINE void Entities::set_position_and_bound_vectors()
{
#ifdef OPENGL_RENDERER
	*this->m_position = this->m_world_transformation->translation_vector();        // + this->m_OriginOffset;
	// Update the Bound Centers
	Vector3f temp_center = this->m_bound->get_center() + *this->m_position;
	this->m_bound->set_center_transformed(temp_center);
#endif
}

FORCE_INLINE void Entities::get_transformation_matrix(Matrix4f &a_matrix)
{
	a_matrix = *this->m_local_transformation * *this->m_world_transformation;
}

FORCE_INLINE ror::Entities::ENodeType Entities::get_node_type() const
{
	return this->m_node_type;
}

FORCE_INLINE void Entities::set_node_type(ror::Entities::ENodeType a_node_type)
{
	this->m_node_type = a_node_type;
}

FORCE_INLINE void Entities::delete_scene_graph()
{
	// Here We delete the whole Scene Graph
	static Stack<Entities *> elements_stack;
	elements_stack.push(this);

	while (!elements_stack.empty())
	{
		Entities *current_object = elements_stack.pop();
		// Handle its Children
		if (current_object->has_child())
		{
			std::vector<csgnode *>::iterator index = current_object->m_child_node->begin();
			for (; index < current_object->m_child_node->end(); index++)
			{
				elements_stack.push(dynamic_cast<Entities *>(*(index)));
			}
		}
		rDELETE(current_object);
	}
}

FORCE_INLINE Vector2i Entities::generate_visibility_set(const Camera *a_active_camera)
{
	Vector2i visible(0, 1);
	this->prepair();
	static Stack<Entities *> elements_stack;
	elements_stack.push(this);

	while (!elements_stack.empty())
	{
		Entities *current_object = elements_stack.pop();
		assert(current_object != nullptr && "Current Object is Null");

		current_object->m_visibility = a_active_camera->contains_sphere(current_object->m_bound, false);

		if (rOUTSIDE == current_object->m_visibility)
		{
			++visible.x;
		}

		// Handle its Children
		if (current_object->has_child())
		{
			std::vector<csgnode *>::iterator index = current_object->m_child_node->begin();
			for (; index < current_object->m_child_node->end(); index++)
			{
				elements_stack.push(dynamic_cast<Entities *>(*(index)));
				++visible.y;
			}
		}
	}        // While
	return visible;
}

FORCE_INLINE void Entities::set_object_type(int32_t a_object_type)
{
	this->m_object_type = a_object_type;
}

FORCE_INLINE int32_t Entities::get_object_type() const
{
	return this->m_object_type;
}

}        // namespace ror

// #define PM-
// #define PM+
// #define D
// #define _1
// #define _2
// #define _3
// #define _4
// #define _5
// #define _6
// #define _7
// #define _8
// #define _9
// #define _10
//
//
//
//
//
//
//	{
//		_1 PM+
//		_1 D
//		{
//			_2 PM+
//			_2 D
//			{
//				_3 PM+
//				_3 D
//				_3 PM-
//
//				_4 PM+
//				_4 D
//				_4 PM-
//			}
//			_2 PM-
//
//			_5 PM+
//			_5 D
//			{
//				_6 PM+
//				_6 D
//				_6 PM-
//			}
//			_5 PM-
//
//			_7 PM+
//			_7 D
//			{
//				_8 PM+
//				_8 D
//				_8 PM-
//
//				_9 PM+
//				_9 D
//				_9 PM-
//
//				_10 PM+
//				_10 D
//				_10 PM-
//			}
//			_7 PM-
//		}
//		_1 PM-
//	}
//
