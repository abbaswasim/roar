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

#pragma once

#include "foundation/rorcrtp.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rorsystem.hpp"
#include "rhi/rorhandles.hpp"
#include "rhi/rortexture.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace rhi
{
create_handle(ShaderHandle, int32_t);
create_handle(ShaderProgramHandle, int32_t);
create_handle(BufferHandle, int32_t);

/**
 * This defines a packet for all the render data
 * will contain a list of programs, textures, buffers IDs
 */
class ROAR_ENGINE_ITEM RenderData final
{
  public:
	FORCE_INLINE             RenderData()                              = default;        //! Default constructor
	FORCE_INLINE             RenderData(const RenderData &a_other)     = default;        //! Copy constructor
	FORCE_INLINE             RenderData(RenderData &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderData &operator=(const RenderData &a_other)      = default;        //! Copy assignment operator
	FORCE_INLINE RenderData &operator=(RenderData &&a_other) noexcept  = default;        //! Move assignment operator
	FORCE_INLINE ~RenderData() noexcept                                = default;        //! Destructor

  protected:
  private:
	std::vector<BufferHandle>        m_buffers;
	std::vector<ShaderHandle>        m_shaders;
	std::vector<ShaderProgramHandle> m_programs;
};

}        // namespace rhi
