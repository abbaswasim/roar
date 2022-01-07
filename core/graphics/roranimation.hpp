// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021-2022
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

#include "foundation/rortypes.hpp"
#include "math/rorvector4.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorvertex_description.hpp"

namespace ror
{

enum class AnimationTarget : uint32_t
{
	translation,
	rotation,
	scale,
	weight
};

enum class AnimationInterpolation : uint32_t
{
	linear,
	step,
	cubicspline
};

class ROAR_ENGINE_ITEM Animation
{
  public:
	FORCE_INLINE            Animation()                             = default;        //! Default constructor
	FORCE_INLINE            Animation(const Animation &a_other)     = default;        //! Copy constructor
	FORCE_INLINE            Animation(Animation &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE Animation &operator=(const Animation &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Animation &operator=(Animation &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Animation() noexcept                              = default;        //! Destructor

	struct AnimationInput
	{
		float32_t m_value;
	};

	struct AnimationOutput
	{
		// Vector4f m_values;
		float32_t m_value;
	};

	struct AnimationSampler
	{
		AnimationInterpolation m_interpolation{AnimationInterpolation::linear};        //! Interpolation of this sampler values

		std::vector<AnimationInput, rhi::BufferAllocator<AnimationInput>>   m_input{};         //! Input of type float, these are keyframe times
		std::vector<AnimationOutput, rhi::BufferAllocator<AnimationOutput>> m_output{};        //! Output of type float[3-4], these are keyframe TRS values
	};

	struct AnimationChannel
	{
		uint32_t        m_sampler_index{};                                       //! Which samplers this channel uses
		uint32_t        m_target_node_index{};                                   //! Which Node this channel targets
		AnimationTarget m_target_node_path{AnimationTarget::translation};        //! Which aspect of the node will be targeted
	};

	std::vector<AnimationSampler, rhi::BufferAllocator<AnimationSampler>> m_samplers;        //! All samplers in this animation
	std::vector<AnimationChannel, rhi::BufferAllocator<AnimationChannel>> m_channels;        //! All channels in this animation
};

static_assert(sizeof(Animation::AnimationInput) == 4, "AnimationInput is not 4 bytes float");
static_assert(sizeof(Animation::AnimationOutput) == 4, "AnimationOutput is not 4 bytes float");
}        // namespace ror

namespace rhi
{
// Define type to semantic for buffers allocator
define_type_to_shader_semantics(ror::Animation::AnimationInput)
{
	return rhi::BufferSemantic::animation_input_data;
}

define_type_to_shader_semantics(ror::Animation::AnimationOutput)
{
	return rhi::BufferSemantic::animation_output_data;
}

define_type_to_shader_semantics(ror::Animation::AnimationSampler)
{
	return rhi::BufferSemantic::animation_sampler_data;
}

define_type_to_shader_semantics(ror::Animation::AnimationChannel)
{
	return rhi::BufferSemantic::animation_channel_data;
}

define_type_to_shader_semantics(ror::Animation)
{
	return rhi::BufferSemantic::animation_data;
}

}        // namespace rhi
