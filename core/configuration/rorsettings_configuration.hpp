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
#include "profiling/rorlog.hpp"
#include <string>
#include <unordered_map>
#include <variant>

namespace ror
{
using generic_config = std::variant<bool, uint32_t, std::string, float32_t>;
class ROAR_ENGINE_ITEM SettingsConfig : public Configuration<SettingsConfig>
{
  public:
	FORCE_INLINE                 SettingsConfig()                                  = default;        //! Default constructor
	FORCE_INLINE                 SettingsConfig(const SettingsConfig &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                 SettingsConfig(SettingsConfig &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE SettingsConfig &operator=(const SettingsConfig &a_other) = default;                 //! Copy assignment operator
	FORCE_INLINE SettingsConfig &operator=(SettingsConfig &&a_other) noexcept = default;             //! Move assignment operator
	FORCE_INLINE virtual ~SettingsConfig() noexcept override                  = default;             //! Destructor

	declare_translation_unit_vtable();

	FORCE_INLINE explicit SettingsConfig(std::filesystem::path a_config_path);        //! Single argument constructor

	void load_specific()
	{
		for (json::iterator it = this->m_json_file.begin(); it != this->m_json_file.end(); ++it)
		{
			generic_config c;
			if (it->is_number())
				c = static_cast<uint32_t>(it.value());
			if (it->is_string())
				c = static_cast<std::string>(it.value());
			if (it->is_boolean())
				c = static_cast<bool>(it.value());
			if (it->is_number_float())
				c = static_cast<float32_t>(it.value());

			this->m_generic_configs[it.key()] = c;
		}
	}

	template <typename _type>
	auto get(std::string a_key) const
	{
		auto conf = this->m_generic_configs.find(a_key);

		if (conf != this->m_generic_configs.end())
			return std::get<_type>(conf->second);

		return _type{};
	}

  protected:
  private:
	std::unordered_map<std::string, generic_config> m_generic_configs{};
};

define_translation_unit_vtable(SettingsConfig)
{}

FORCE_INLINE SettingsConfig::SettingsConfig(std::filesystem::path a_config_path)
{
	this->load(a_config_path);
}

FORCE_INLINE const SettingsConfig &get_settings()
{
	static SettingsConfig sc{"settings.json"};        // Loads a predefined config file required to setup buffers formats, since this is a resource it will just work without full path
	return sc;                                        // The path could be extracted out to clients further later
}

}        // namespace ror
