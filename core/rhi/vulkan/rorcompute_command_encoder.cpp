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

#include "rhi/vulkan/rorcompute_command_encoder.hpp"

namespace rhi
{
void ComputeCommandEncoder::compute_pipeline_state(const rhi::Device &a_device, const rhi::Program &a_compute_pipeline_state) noexcept
{
	vkCmdBindPipeline(this->m_command_buffer, VK_PIPELINE_BIND_POINT_COMPUTE, a_compute_pipeline_state.compute_pipeline_state());
	this->bind_descriptors(a_device, a_compute_pipeline_state);
}

void ComputeCommandEncoder::bind_descriptors(const rhi::Device &a_device, const rhi::ProgramVulkan &a_pso) const noexcept
{
	auto &descriptor_cache = a_device.descriptor_set_cache();
	for (auto &pd : a_pso.platform_descriptors())
	{
		DescriptorSet &set = descriptor_cache.at(pd);
		vkCmdBindDescriptorSets(this->m_command_buffer, VK_PIPELINE_BIND_POINT_COMPUTE, a_pso.pipeline_layout(), set.set_id(), 1, &set.platform_descriptor(), 0, nullptr);
	}
}

}        // namespace rhi
