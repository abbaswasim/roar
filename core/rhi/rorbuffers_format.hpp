// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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

#include "configuration/rorconfiguration.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "rhi/rorbuffer.hpp"
#include <memory>
#include <stdint.h>
#include <vector>

namespace ror
{
struct BufferPack
{
	std::string              m_name{};
	std::vector<rhi::Buffer> m_buffers{};
};

struct BuffersFormat
{
	uint32_t                m_unit{1024};               //! 1024 means KiB, could be turned into MiB if required
	uint32_t                m_current_format{0};        //! Reference to current format that should be used, possibility to move around others
	std::vector<BufferPack> m_buffer_packs{};
};

class ROAR_ENGINE_ITEM BuffersFormatConfig : public Configuration<BuffersFormatConfig>
{
  public:
	FORCE_INLINE BuffersFormatConfig()                                       = default;                   //! Default constructor
	FORCE_INLINE BuffersFormatConfig(const BuffersFormatConfig &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE BuffersFormatConfig(BuffersFormatConfig &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE BuffersFormatConfig &operator=(const BuffersFormatConfig &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE BuffersFormatConfig &operator=(BuffersFormatConfig &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~BuffersFormatConfig() noexcept override                       = default;        //! Destructor

	declare_translation_unit_vtable();
	BuffersFormatConfig(std::filesystem::path a_config_path);        //! Default constructor

	void                 load_specific();
	const BuffersFormat &buffers_format() const;
	void                 load_remaining_buffers();        //! Allocate buffer locations to semantics not specified in the config

  protected:
  private:
	BuffersFormat m_buffers_format{};
};

const BuffersFormat &get_buffers_format();

}        // namespace ror
