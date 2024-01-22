// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021-2023
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

#include "foundation/rormacros.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include "math/rortransform.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rortexture.hpp"
#include "roar_export_import.hpp"
#include <string>

namespace ror
{

class ROAR_ENGINE_ITEM IBLEnvironment final
{
  public:
	FORCE_INLINE                 IBLEnvironment()                                  = default;        //! Default constructor
	FORCE_INLINE                 IBLEnvironment(const IBLEnvironment &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                 IBLEnvironment(IBLEnvironment &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE IBLEnvironment &operator=(const IBLEnvironment &a_other)          = default;        //! Copy assignment operator
	FORCE_INLINE IBLEnvironment &operator=(IBLEnvironment &&a_other) noexcept      = default;        //! Move assignment operator
	FORCE_INLINE ~IBLEnvironment() noexcept                                        = default;        //! Destructor

	enum class InputType : uint32_t
	{
		unknown,
		equirectangular,
		plus
	};

	// clang-format off
	FORCE_INLINE void path(std::filesystem::path a_path)            { this->m_path = a_path;                     }
	FORCE_INLINE void name(std::string a_name)                      { this->m_name = a_name;                     }
	FORCE_INLINE void ready(bool a_ready)                           { this->m_ready = a_ready;                   }
	FORCE_INLINE void type(InputType a_type)                        { this->m_type = a_type;                     }
	FORCE_INLINE void input(uint32_t a_index)                       { this->m_input = a_index;                   }
	FORCE_INLINE void skybox(uint32_t a_index)                      { this->m_skybox = a_index;                  }
	FORCE_INLINE void radiance(uint32_t a_index)                    { this->m_radiance = a_index;                }
	FORCE_INLINE void irradiance(uint32_t a_index)                  { this->m_irradiance = a_index;              }
	FORCE_INLINE void skybox_pso(uint32_t a_index)                  { this->m_skybox_pso = a_index;              }
	FORCE_INLINE void radiance_pso(uint32_t a_index)                { this->m_radiance_pso = a_index;            }
	FORCE_INLINE void irradiance_pso(uint32_t a_index)              { this->m_irradiance_pso = a_index;          }
	FORCE_INLINE void brdf_integration_pso(int32_t a_index)         { this->m_brdf_integration_pso = a_index;    }
	FORCE_INLINE void skybox_render_pso(uint32_t a_index)           { this->m_skybox_render_pso = a_index;       }
	FORCE_INLINE void brdf_integration(uint32_t a_index)            { this->m_brdf_integration = a_index;        }
	FORCE_INLINE void input_sampler(uint32_t a_index)               { this->m_input_sampler_id = a_index;        }
	FORCE_INLINE void skybox_sampler(uint32_t a_index)              { this->m_skybox_sampler_id = a_index;       }
	FORCE_INLINE void radiance_sampler(uint32_t a_index)            { this->m_radiance_sampler_id = a_index;     }
	FORCE_INLINE void irradiance_sampler(uint32_t a_index)          { this->m_irradiance_sampler_id = a_index;   }

	FORCE_INLINE auto name()                          const noexcept { return this->m_name;                    }
	FORCE_INLINE auto path()                          const noexcept { return this->m_path;                    }
	FORCE_INLINE constexpr auto ready()               const noexcept { return this->m_ready;                   }
	FORCE_INLINE constexpr auto type()                const noexcept { return this->m_type;                    }
	FORCE_INLINE constexpr auto input()               const noexcept { return this->m_input;                   }
	FORCE_INLINE constexpr auto skybox()              const noexcept { return this->m_skybox;                  }
	FORCE_INLINE constexpr auto radiance()            const noexcept { return this->m_radiance;                }
	FORCE_INLINE constexpr auto irradiance()          const noexcept { return this->m_irradiance;              }
	FORCE_INLINE constexpr auto skybox_pso()          const noexcept { return this->m_skybox_pso;              }
	FORCE_INLINE constexpr auto radiance_pso()        const noexcept { return this->m_radiance_pso;            }
	FORCE_INLINE constexpr auto irradiance_pso()      const noexcept { return this->m_irradiance_pso;          }
	FORCE_INLINE constexpr auto brdf_integration_pso()const noexcept { return this->m_brdf_integration_pso;    }
	FORCE_INLINE constexpr auto skybox_render_pso()   const noexcept { return this->m_skybox_render_pso;       }
	FORCE_INLINE constexpr auto brdf_integration()    const noexcept { return this->m_brdf_integration;        }
	FORCE_INLINE constexpr auto input_sampler()       const noexcept { return this->m_input_sampler_id;        }
	FORCE_INLINE constexpr auto skybox_sampler()      const noexcept { return this->m_skybox_sampler_id;       }
	FORCE_INLINE constexpr auto radiance_sampler()    const noexcept { return this->m_radiance_sampler_id;     }
	FORCE_INLINE constexpr auto irradiance_sampler()  const noexcept { return this->m_irradiance_sampler_id;   }
	// clang-format on

  protected:
  private:
	std::string           m_name{};                          //! Name of the environment
	std::filesystem::path m_path{};                          //! Path to input equirectangular 2D .hdr/.exr image
	bool                  m_ready{false};                    //! Whether the environment is fully loaded and upload or not
	InputType             m_type{InputType::unknown};        //! Input type of the input cube map like equirectangular and plus etc
	uint32_t              m_input{0};                        //! Index of input texture in renderer textures
	uint32_t              m_skybox{0};                       //! Index of skybox environment cube map texture in renderer textures that is used as it is
	uint32_t              m_radiance{0};                     //! Index of radiance cube map texture in renderer textures
	uint32_t              m_irradiance{0};                   //! Index of irradiance cube map texture in renderer textures
	uint32_t              m_brdf_integration{0};             //! Index of brdf_integration lut texture in renderer textures
	uint32_t              m_skybox_pso{0};                   //! Index of skybox cube map pso in renderer programs
	uint32_t              m_radiance_pso{0};                 //! Index of radiance cube map pso in renderer programs
	uint32_t              m_irradiance_pso{0};               //! Index of irradiance cube map pso in renderer programs
	int32_t               m_brdf_integration_pso{0};         //! Index of brdf_integration_pso renderer programs, only used for debugging visualisation
	uint32_t              m_skybox_render_pso{0};            //! Index of skybox cube map pso in renderer programs to use for rendering the skybox
	uint32_t              m_input_sampler_id{0};             //! Index of input cube map sampler in renderer samplers
	uint32_t              m_skybox_sampler_id{0};            //! Index of skybox cube map sampler in renderer samplers
	uint32_t              m_radiance_sampler_id{0};          //! Index of radiance cube map sampler in renderer samplers
	uint32_t              m_irradiance_sampler_id{0};        //! Index of irradiance cube map sampler in renderer samplers
};

}        // namespace ror
