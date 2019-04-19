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

#include "math/rormatrix4.h"
#include "math/rorvector3.h"
#include "memory_manager/rormemory_check.h"
#include "renderer/rorrendering_state.h"
#include "scene_graph/rorcamera_node.h"

namespace rendering
{
class ROAR_ENGINE_ITEM IRenderingDevice
{
  public:
	FORCE_INLINE IRenderingDevice(void);
	virtual ~IRenderingDevice(void);

	virtual bool prepair()                    = 0;
	virtual void pre_render()                 = 0;
	virtual void render()                     = 0;
	virtual void post_render()                = 0;
	virtual bool shutdown()                   = 0;
	virtual bool update(rUsLong a_delta_time) = 0;
	virtual bool reset()                      = 0;
	virtual void setup_wireframe_rendering()  = 0;
	virtual void setup_shaded_rendering()     = 0;

	FORCE_INLINE ror::Matrix4f *update_projection_matrix();
	FORCE_INLINE ror::Matrix4f *update_model_view_matrix();
	FORCE_INLINE ror::Matrix4f *update_combined_matrix();

	FORCE_INLINE ror::Matrix4f *get_projection_matrix();
	FORCE_INLINE ror::Matrix4f *get_model_view_matrix();
	FORCE_INLINE ror::Matrix4f *get_combined_matrix();

	//
	//		virtual DeviceStatus GetDeviceStatus() = 0;
	//
	//void				SetTextureMatrix(uint32_t stage, const CMatrix4f *matrix) = 0;
	//void				GetTextureMatrix(uint32_t a_Stage, CMatrix4f *a_Matrix) = 0;
	//		virtual void SetLighting(bool enable) = 0;
	//		virtual void SetLight(uint stage, Light *pLight) = 0;
	//		virtual void EnableLight(uint stage, bool enable) = 0;
	//
	//		virtual void SetMaterial(const Material *pMaterial) = 0;
	//
	//		virtual void SetGlobalAmbient(Color color) = 0;
	//
	//		virtual void SetCullMode(CullMode mode) = 0;
	//		virtual void SetFillMode(FillMode mode) = 0;
	//
	//		virtual void SetDepthTest(bool enable) = 0;
	//		virtual void SetDepthWrite(bool enable) = 0;
	//		virtual void SetDepthFunc(CompareFunc func) = 0;
	//
	//		virtual void SetBlending(bool enable) = 0;
	//		virtual void SetBlendMode(Blend src, Blend dest, BlendOperand op = BlendOp_Add) = 0;
	//		virtual void SetBlendAlphaMode(Blend src, Blend dest, BlendOperand op = BlendOp_Add) = 0;
	//		virtual void SetBlendFactor(uint factor = 0xffffffff) = 0;
	//
	//		virtual void SetProjectionMatrix(const Matrix4 *matrix) = 0;
	//		virtual void SetViewMatrix(const Matrix4 *matrix) = 0;
	//		virtual void SetCombinedMatrix(const Matrix4 *matrix) = 0;
	//		virtual void SetTextureMatrix(uint stage, const Matrix4 *matrix) = 0;
	//
	//		virtual void GetProjectionMatrix(Matrix4 *matrix) = 0;
	//		virtual void GetViewMatrix(Matrix4 *matrix) = 0;
	//		virtual void GetCombinedMatrix(Matrix4 *matrix) = 0;
	//		virtual void GetTextureMatrix(uint stage, Matrix4 *matrix) = 0;
	//
	//		virtual void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height) = 0;
  protected:
	rendering::RenderingState *m_rendering_state;          // Contains Current Rendering State information		// TODO: must be Used later
	ror::Camera *              m_active_camera;            // Active Camera in the World (doesn't Take Ownership;
	ror::Matrix4f *            m_model_view_matrix;        // Model View Matrix place holder so don't need to calculate if its not changed
	ror::Matrix4f *            m_projection_matrix;        // Projection Matrix place holder so don't need to calculate if its not changed
	ror::Matrix4f *            m_combined_matrix;          // ModelView and Projection Matrix place holder so don't need to calculate if its not changed
	ror::Matrix4f *            m_buffer_matrix;            // Temporary Buffer for calculations
};
}        // namespace rendering

#include "rorirendering_device.inl"
