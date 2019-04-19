// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2019
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

#include "foundation/rorsingleton.h"
#include "models/rorcba.h"
#include "models/rorcba_object.h"

namespace ror
{
class ROAR_ENGINE_ITEM ccbamanager : public Singleton<ccbamanager>
{
  public:
	FORCE_INLINE bool update_model(CCBA *a_cbapointer);
	FORCE_INLINE uint32_t get_model_size(CCBA *a_cbapointer);
	FORCE_INLINE uint32_t get_all_models_size();

	CCBA *load_model(std::string a_file_name);
	void  release_model(CCBA *a_cbapointer);
	void  release_all_models();

	CCBA *get_model(std::string a_file_name) const;

  private:
	FORCE_INLINE ccbamanager(void);
	~ccbamanager(void);

	typedef std::unordered_map<std::string, ccbaobject *> cbaobjects_map;

	cbaobjects_map *m_cbamodels;        // CBA Store

	// Make CSingleton<CCBAManager> as friend
	friend class Singleton<ccbamanager>;
};
}        // namespace ror

#include "rorcba_manager.inl"
