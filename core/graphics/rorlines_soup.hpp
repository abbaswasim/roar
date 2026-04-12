// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2023
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

#include "event_system/rorevent_system.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "renderer/rorrenderer.hpp"
#include <cstdint>
#include <type_traits>

namespace ror
{

class ROAR_ENGINE_ITEM LineSoup final
{
  public:
	FORCE_INLINE           LineSoup(const LineSoup &a_other)      = delete;         //! Copy constructor
	FORCE_INLINE           LineSoup(LineSoup &&a_other) noexcept  = delete;         //! Move constructor
	FORCE_INLINE LineSoup &operator=(const LineSoup &a_other)     = delete;         //! Copy assignment operator
	FORCE_INLINE LineSoup &operator=(LineSoup &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~LineSoup() noexcept                             = default;        //! Move assignment operator

	LineSoup();        //! Default constructor

	void init(const rhi::Device &a_device, ror::EventSystem &a_event_system);
	void upload(const rhi::Device &a_device, const ror::Renderer &a_renderer);
	void render(const rhi::Device &a_device, const ror::Renderer &a_renderer, rhi::RenderCommandEncoder &a_encoder, ror::OrbitCamera &a_camera, ror::EventSystem &a_event_system);
	void push_lines(const uint8_t *a_data, uint32_t a_lines_count);

  protected:
  private:
	void install_input_handlers();
	void uninstall_input_handlers();
	void setup_render_state(const rhi::Device &a_device, rhi::RenderCommandEncoder &a_encoder, const ror::Renderer &a_renderer);

	const rhi::Device    *m_device{nullptr};               //! Non-Owning pointer to a device that is used to initiliazed this line soup
	ror::EventSystem     *m_event_system{nullptr};         //! Non-owning pointer to the event system
	EventCallback         m_push_line_callback{};          //! Push line function that will be used to subscribe and unsubscribe this with event system
	EventCallback         m_show_toggle_callback{};        //! Show/Unshow function that will be used to subscribe and unsubscribe this with event system
	bool                  m_visible{false};                //! Is the thing showing, can we go and render
	uint32_t              m_lines_count{0u};               //! All the lines pushed in to the soup so far
	rhi::Program          m_shader_program{-1, -1};        //! Program in undefined/uinitialized state
	rhi::VertexDescriptor m_vertex_descriptor{};           //! The vertex descriptor of the line soup, that is defined by xy,  rgb in the vertex buffer
	rhi::Buffer           m_vertex_buffer{};               //! Vertex buffer with interleaved data of xyz, rgb
};

FORCE_INLINE constexpr void push_position(std::vector<float32_t> &a_data, float32_t x, float32_t y, float32_t z)
{
	a_data.push_back(x);
	a_data.push_back(y);
	a_data.push_back(z);
}

// Overload for float32_t color components (normalized [0.0f, 1.0f])
template <typename R, typename G, typename B, typename A,
          std::enable_if_t<
              std::is_floating_point<R>::value &&
                  std::is_floating_point<G>::value &&
                  std::is_floating_point<B>::value &&
                  std::is_floating_point<A>::value,
              bool> = true>
FORCE_INLINE constexpr void push_point(std::vector<float32_t> &a_data, float32_t x, float32_t y, float32_t z, R r, G g, B b, A a)
{
	push_position(a_data, x, y, z);

	uint32_t c = static_cast<uint8_t>(r * 255.0f) |
	             static_cast<uint8_t>(g * 255.0f) << 8 |
	             static_cast<uint8_t>(b * 255.0f) << 16 |
	             static_cast<uint8_t>(a * 255.0f) << 24;

	a_data.push_back(static_cast<float32_t>(c));
}

// Overload for uint8_t color components
template <
    typename R, typename G, typename B, typename A,
    std::enable_if_t<
        (std::is_same_v<R, uint8_t> || std::is_same_v<R, uint32_t> || std::is_same_v<R, int32_t>) &&
            (std::is_same_v<G, uint8_t> || std::is_same_v<G, uint32_t> || std::is_same_v<R, int32_t>) &&
            (std::is_same_v<B, uint8_t> || std::is_same_v<B, uint32_t> || std::is_same_v<R, int32_t>) &&
            (std::is_same_v<A, uint8_t> || std::is_same_v<A, uint32_t> || std::is_same_v<R, int32_t>),
        bool> = true>
FORCE_INLINE constexpr void push_point(std::vector<float32_t> &a_data, float32_t x, float32_t y, float32_t z, R r, G g, B b, A a)
{
	push_position(a_data, x, y, z);

	uint8_t _r{0}, _g{0}, _b{0}, _a{0};

	_r = static_cast<uint8_t>(r);
	_g = static_cast<uint8_t>(g);
	_b = static_cast<uint8_t>(b);
	_a = static_cast<uint8_t>(a);

	uint32_t c = static_cast<uint32_t>((_r) | (_g) << 8 | (_b) << 16 | (_a) << 24);

	a_data.push_back(static_cast<float32_t>(c));
}

FORCE_INLINE LineSoup &line_soup() noexcept
{
	static LineSoup lines{};        // Global lines container
	return lines;
}

}        // namespace ror
