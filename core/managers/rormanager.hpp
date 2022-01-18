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

#include "foundation/rorcache.hpp"
#include "foundation/rorcrtp.hpp"
#include "foundation/rormacros.hpp"
#include "rhi/rortexture.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace ror
{

/**
 * An Manager base of Crtp type
 * This one is an interface, specific implementations can be defined based on this
 */
template <typename _type, typename _map_key, typename _map_value>
class ROAR_ENGINE_ITEM Manager
{
  public:
	FORCE_INLINE          Manager(const Manager &a_other)     = default;          //! Copy constructor
	FORCE_INLINE          Manager(Manager &&a_other) noexcept = default;          //! Move constructor
	FORCE_INLINE Manager &operator=(const Manager &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Manager &operator=(Manager &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~Manager() noexcept                    = default;        //! Destructor

	_map_value load(_map_key a_key)
	{
		auto iter = this->m_content.find(a_key);

		if (iter.second)
			return iter->first;

		return this->underlying()._load_specific(a_key);
	}

	void _load()
	{
		this->underlying()._load_specific();
	}

  private:
	// clang-format off
	_type       &underlying()       { return static_cast<_type &>(*this);       }
	_type const &underlying() const { return static_cast<_type const &>(*this); }
	// clang-format on

	FORCE_INLINE Manager() = default;        //! Default constructor
	friend _type;

	FORCE_INLINE void _insert(const _map_key &a_key, _map_value _value_ptr)
	{
		auto result = this->m_content.insert(a_key, _value_ptr);

		assert(result && "Insert failed in texture manager load");
		(void) result;
	}

	// std::unordered_map<_map_key, _map_value> m_content{};
	using ContentCache = Cache<_map_key, _map_value, true>;        // Thread Safe content cache
	ContentCache m_content{};                                      //! Stuff that is managed by the manager
};

std::shared_ptr<rhi::TextureImage> load_texture_resource(std::string)
{
	return nullptr;
}

/**
 * An implementation of Manager for texture images
 */
class ROAR_ENGINE_ITEM TextureImageManager final : public Manager<TextureImageManager, std::string /*path*/, std::shared_ptr<rhi::TextureImage>>
{
  public:
	FORCE_INLINE                      TextureImageManager()                                       = default;        //! Default constructor
	FORCE_INLINE                      TextureImageManager(const TextureImageManager &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                      TextureImageManager(TextureImageManager &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE TextureImageManager &operator=(const TextureImageManager &a_other) = delete;                       //! Copy assignment operator
	FORCE_INLINE TextureImageManager &operator=(TextureImageManager &&a_other) noexcept = delete;                   //! Move assignment operator
	FORCE_INLINE ~TextureImageManager() noexcept override                               = default;                  //! Destructor

	declare_translation_unit_vtable();

  protected:
  private:
	std::shared_ptr<rhi::TextureImage> _load_specific(const std::string &a_key)
	{
		// TODO: create texture properly from a_key
		auto ptr = load_texture_resource(a_key);        // std::make_shared<rhi::TextureImage>();
		this->_insert(a_key, ptr);
		return ptr;
	}
};

define_translation_unit_vtable(TextureImageManager)
{}

FORCE_INLINE TextureImageManager &get_texture_image_manager()
{
	static TextureImageManager tm;
	return tm;
}

/**
 * An implementation of Manager for texture images
 */
class ROAR_ENGINE_ITEM TextureSamplerManager final : public Manager<TextureSamplerManager, std::string /*path*/, std::shared_ptr<rhi::TextureSampler>>
{
  public:
	FORCE_INLINE                        TextureSamplerManager()                                         = default;        //! Default constructor
	FORCE_INLINE                        TextureSamplerManager(const TextureSamplerManager &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                        TextureSamplerManager(TextureSamplerManager &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE TextureSamplerManager &operator=(const TextureSamplerManager &a_other) = delete;                         //! Copy assignment operator
	FORCE_INLINE TextureSamplerManager &operator=(TextureSamplerManager &&a_other) noexcept = delete;                     //! Move assignment operator
	FORCE_INLINE ~TextureSamplerManager() noexcept override                                 = default;                    //! Destructor

	declare_translation_unit_vtable();

  protected:
  private:
	std::shared_ptr<rhi::TextureSampler> _load_specific(const std::string &a_key)
	{
		// TODO: create texture properly from a_key
		auto ptr = std::make_shared<rhi::TextureSampler>();
		this->_insert(a_key, ptr);
		return ptr;
	}
};

define_translation_unit_vtable(TextureSamplerManager)
{}

FORCE_INLINE TextureSamplerManager &get_texture_sampler_manager()
{
	static TextureSamplerManager tm;
	return tm;
}

/**
 * An implementation of Manager for texture images
 */
class ROAR_ENGINE_ITEM TextureManager final : public Manager<TextureManager, std::string /*path*/, std::shared_ptr<rhi::Texture>>
{
  public:
	FORCE_INLINE                 TextureManager()                                  = default;        //! Default constructor
	FORCE_INLINE                 TextureManager(const TextureManager &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                 TextureManager(TextureManager &&a_other) noexcept = delete;         //! Move constructor
	FORCE_INLINE TextureManager &operator=(const TextureManager &a_other) = delete;                  //! Copy assignment operator
	FORCE_INLINE TextureManager &operator=(TextureManager &&a_other) noexcept = delete;              //! Move assignment operator
	FORCE_INLINE ~TextureManager() noexcept override                          = default;             //! Destructor

	declare_translation_unit_vtable();

  protected:
  private:
	std::shared_ptr<rhi::Texture> _load_specific(const std::string &a_key)
	{
		// TODO: create texture properly from a_key
		auto ptr = std::make_shared<rhi::Texture>();
		this->_insert(a_key, ptr);
		return ptr;
	}
};

define_translation_unit_vtable(TextureManager)
{}

FORCE_INLINE TextureManager &get_texture_manager()
{
	static TextureManager tm;
	return tm;
}

}        // namespace ror
