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

#include "rorcba_manager.h"

namespace ror
{
ccbamanager::~ccbamanager(void)
{
	rDELETE(this->m_cbamodels);
}

CCBA *ccbamanager::load_model(std::string a_file_name)
{
	// Before trying to create a new CBA check if its already in the CBAs loaded
	cbaobjects_map::iterator iter = this->m_cbamodels->find(a_file_name);
	if (iter != this->m_cbamodels->end())
	{
		iter->second->increment_reference_count();
		return iter->second->get_cbapointer();
	}
	// Otherwise we load it from disk
	CCBA *temp_cba = rNew CCBA();
	temp_cba->read_cba(a_file_name);

	ccbaobject *new_cbaobject = rNew ccbaobject();
	new_cbaobject->set_cbapointer(temp_cba);

	this->m_cbamodels->insert(std::make_pair(std::string(a_file_name), new_cbaobject));

	return new_cbaobject->get_cbapointer();
}

void ccbamanager::release_model(CCBA *a_cbapointer)
{
	if (nullptr != a_cbapointer)
	{
		cbaobjects_map::iterator iter;
		for (iter = this->m_cbamodels->begin(); iter != this->m_cbamodels->end(); ++iter)
		{
			if (a_cbapointer == iter->second->get_cbapointer())
			{
				if (iter->second->decrement_reference_count() == 0)
				{
					rDELETE(a_cbapointer);
					rDELETE(iter->second);
					this->m_cbamodels->erase(iter);
				}
				return;
			}
		}
	}
}

void ccbamanager::release_all_models()
{
	cbaobjects_map::iterator iter;        // = m_CBAModels->begin();
	for (iter = this->m_cbamodels->begin(); iter != this->m_cbamodels->end(); ++iter)
	{
		CCBA *temp_pointer = iter->second->get_cbapointer();
		rDELETE(iter->second);
		rDELETE(temp_pointer);
	}
	this->m_cbamodels->clear();
}

}        // namespace ror
