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

#include "profiling/rorlog.hpp"
#include "rhi/crtp_interfaces/rorshader.hpp"

namespace rhi
{

template <class _type>
FORCE_INLINE ShaderCrtp<_type>::ShaderCrtp(const std::string &a_shader, hash_64_t a_hash, rhi::ShaderType a_type, ror::ResourceAction a_action) :
    m_hash(a_hash), m_type(a_type)
{
	if (a_action == ror::ResourceAction::create)
		this->m_shader = &ror::resource(a_shader, ror::ResourceSemantic::caches, ror::ResourceAction::create, "shaders");
	else
		this->m_shader = &ror::resource(a_shader, ror::ResourceSemantic::shaders, a_action);
}

template <class _type>
FORCE_INLINE void ShaderCrtp<_type>::source(const std::string &a_source)
{
	assert(this->m_shader);
	this->m_shader->update({a_source.begin(), a_source.end()}, false, false);

	this->compile();
}

template <class _type>
FORCE_INLINE void ShaderCrtp<_type>::compile()
{
	std::string info_log;
	if (!compile_to_spirv(this->source(), this->m_type, "main", this->m_spirv, info_log))
	{
		ror::log_critical("Shader to SPIR-V conversion failed \n{}", info_log.c_str());
	}

	this->platform_source();
}


template <class _type>
FORCE_INLINE constexpr auto ShaderCrtp<_type>::source() const noexcept
{
	assert(this->m_shader);
	return std::string(reinterpret_cast<const char *>(this->m_shader->data().data()), this->m_shader->data().size());
}
}        // namespace rhi
