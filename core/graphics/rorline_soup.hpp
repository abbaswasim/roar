// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2025
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
#include "math/rorvector4.hpp"
#include "renderer/rorrenderer.hpp"
#include "rhi/rordevice.hpp"
#include <cstdint>
#include <type_traits>

namespace ror
{

/**
 * A Dynamic Mesh type which can be used at runtime to push lines into and update as required
 * This is a very simple version of DynamicMesh that only supports 3D-lines with RGBA colors.
 * Nothing else needs specificying. A global object is already created, initialised and updated by scene loading.
 * To add a soup of lines just use it like below

 auto &ls = ror::line_soup(); // Get the global object reference

 const uint32_t lines_count = 10; // Make 20 lines

 // Make a data buffer for all the lines and then push it into the line_soup
 std::vector<float32_t> data{};
 data.reserve(lines_count * 2 * 4);        // 2 points per line, 4 floats each point (x, y, z, c)

 float32_t corner = 1000.0f;
 ror::Random<float32_t> rd{0, corner};

 for (size_t i = 0; i < lines_count; ++i)
 {
     // First point
     auto x = rd.next(); auto y = rd.next(); auto z = rd.next();
     ror::push_point(data, x, y, z, 255, 0, 0);

     // Second point
     x = rd.next(); y = rd.next(); z = rd.next();
     ror::push_point(data, x, y, z, 0, 0, 255, 0);
 }

 // Important ID marker, if its not something bigger than the existing lines in the line soup,
 // It can be used to replace the existing ones, this is useful when you just want to keep changing the same lines each frame
 static uint32_t id = 0xFFFFFFFF;

 // Push it into the soup
 id = ls.push_lines(reinterpret_cast<uint8_t *>(data.data()), lines_count, id);
 */
class ROAR_ENGINE_ITEM LineSoup final
{
  public:
	FORCE_INLINE           LineSoup(const LineSoup &a_other)      = delete;         //! Copy constructor
	FORCE_INLINE           LineSoup(LineSoup &&a_other) noexcept  = delete;         //! Move constructor
	FORCE_INLINE LineSoup &operator=(const LineSoup &a_other)     = delete;         //! Copy assignment operator
	FORCE_INLINE LineSoup &operator=(LineSoup &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~LineSoup() noexcept                             = default;        //! Move assignment operator

	LineSoup();        //! Default constructor

	void     init(const rhi::Device &a_device, ror::EventSystem &a_event_system);
	void     upload(const rhi::Device &a_device, const ror::Renderer &a_renderer);
	void     render(const rhi::Device &a_device, rhi::RenderCommandEncoder &a_encoder);
	uint32_t push_lines(const uint8_t *a_data, uint32_t a_lines_count, uint32_t a_lines_index = 0xFFFFFFFF);
	uint32_t push_box(const ror::BoundingBoxf &a_box, uint32_t a_lines_index = 0xFFFFFFFF, ror::Vector4f a_color = ror::orange4f);
	uint32_t push_box(const ror::Vector3f a_corners[8], uint32_t a_lines_index = 0xFFFFFFFF, ror::Vector4f a_color = ror::orange4f);
	uint32_t push_cross(const ror::Vector3f &a_center, const float32_t a_size, uint32_t a_lines_index = 0xFFFFFFFF, ror::Vector4f a_color = ror::orange4f);

  protected:
  private:
	void install_input_handlers();
	void uninstall_input_handlers();
	void setup_render_state(const rhi::Device &a_device, rhi::RenderCommandEncoder &a_encoder);

	const rhi::Device    *m_device{nullptr};               //! Non-Owning pointer to a device that is used to initiliazed this line soup
	ror::EventSystem     *m_event_system{nullptr};         //! Non-owning pointer to the event system
	EventCallback         m_show_toggle_callback{};        //! Show/Unshow function that will be used to subscribe and unsubscribe this with event system
	bool                  m_visible{true};                 //! Is the thing showing, can we go and render
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

FORCE_INLINE constexpr void push_color(std::vector<float32_t> &a_data, float32_t r, float32_t g, float32_t b, float32_t a)
{
	a_data.push_back(r);
	a_data.push_back(g);
	a_data.push_back(b);
	a_data.push_back(a);
}

template <typename R, typename G, typename B, typename A = float32_t,
          std::enable_if_t<
              std::is_floating_point<R>::value &&
                  std::is_floating_point<G>::value &&
                  std::is_floating_point<B>::value &&
                  std::is_floating_point<A>::value,
              bool> = true>
FORCE_INLINE constexpr void push_point(std::vector<float32_t> &a_data, float32_t x, float32_t y, float32_t z, R r, G g, B b, A a = 1.0f)
{
	push_position(a_data, x, y, z);
	push_color(a_data, r, g, b, a);
}

template <typename _type3, typename _type4>
FORCE_INLINE constexpr void push_point(std::vector<float32_t> &a_data, _type3 p, _type4 c)
{
	push_position(a_data, p.x, p.y, p.z);
	push_color(a_data, c.x, c.y, c.z, c.w);
}

template <
    typename R, typename G, typename B, typename A = uint8_t,
    std::enable_if_t<
        (std::is_same_v<R, uint8_t> || std::is_same_v<R, uint32_t> || std::is_same_v<R, int32_t>) &&
            (std::is_same_v<G, uint8_t> || std::is_same_v<G, uint32_t> || std::is_same_v<R, int32_t>) &&
            (std::is_same_v<B, uint8_t> || std::is_same_v<B, uint32_t> || std::is_same_v<R, int32_t>) &&
            (std::is_same_v<A, uint8_t> || std::is_same_v<A, uint32_t> || std::is_same_v<R, int32_t>),
        bool> = true>
FORCE_INLINE constexpr void push_point(std::vector<float32_t> &a_data, float32_t x, float32_t y, float32_t z, R r, G g, B b, A a = 255)
{
	push_position(a_data, x, y, z);
	push_color(a_data, r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

FORCE_INLINE LineSoup &line_soup() noexcept
{
	static LineSoup lines{};        // Global lines container
	return lines;
}

}        // namespace ror
