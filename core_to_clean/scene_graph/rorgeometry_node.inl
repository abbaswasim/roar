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

namespace ror
{
FORCE_INLINE GeometryNode::GeometryNode(void)
{
	this->m_geometry_data = nullptr;        //rNew CoreEngine::CCBA();
}

FORCE_INLINE void GeometryNode::global_draw()
{
#ifdef DRAWBOUNDS
	this->m_bound->draw_bound();
	this->draw_local_axis();
#endif
#ifdef DRAWSKELETON
	if (this->m_geometry_data->has_skeleton())
	{
		this->m_geometry_data->draw_skeleton();
	}
#else

	glActiveTexture(GL_TEXTURE0);        // Hack ::

	if (nullptr != this->m_texture_object)
	{
		this->m_texture_object->bind_texture();
	}

	this->m_geometry_data->draw();

	if (nullptr != this->m_texture_object)
	{
		this->m_texture_object->bind_texture(false);
	}

	glActiveTexture(GL_TEXTURE1);        // Hack ::

#endif
	//this->m_GeometryData->Draw();
}

FORCE_INLINE void GeometryNode::global_update(rUsLong a_milli_seconds)
{
	this->m_geometry_data->update(a_milli_seconds);
}

FORCE_INLINE void GeometryNode::global_update_newtonian(rUsLong a_milli_seconds)
{
	if (this->get_in_motion())
	{
		ror::Moveable::update_moveable((float32_t) a_milli_seconds * 0.001f);
		this->update_transformation_matrices();
	}
}

FORCE_INLINE void GeometryNode::load_geometry(std::string a_file_name)
{
	if (nullptr == this->m_geometry_data)
		this->m_geometry_data = rNew ror::CCBA();
	this->m_geometry_data->read_cba(a_file_name);
}

FORCE_INLINE void GeometryNode::set_geometry(ror::CCBA *a_cba)
{
	if (nullptr != this->m_geometry_data)
	{
		// We have to delete the Old CBA first
		ror::ccbamanager::get_instance()->release_model(this->m_geometry_data);
	}
	this->m_geometry_data = a_cba;
	*this->m_bound        = *a_cba->get_bound();
}

FORCE_INLINE const CCBA *GeometryNode::get_geometry() const
{
	return this->m_geometry_data;
}

FORCE_INLINE void GeometryNode::setup_texture(std::string *a_texture_file_name)
{
	// Load Its texture too.
	assert(nullptr != this->m_geometry_data);
	std::string file_name;        // = this->m_GeometryData->GetTextureFileName();
	if (nullptr == a_texture_file_name)
	{
		file_name = this->m_geometry_data->get_texture_file_name();
	}
	else
	{
		file_name = *a_texture_file_name;
		rDELETE(a_texture_file_name);
	}
	if ("" != file_name)
	{
		if (file_name.substr(file_name.length() - 3) == "TGA" ||
		    file_name.substr(file_name.length() - 3) == "tga" ||
		    file_name.substr(file_name.length() - 3) == "PCX" ||
		    file_name.substr(file_name.length() - 3) == "pcx" ||
		    file_name.substr(file_name.length() - 3) == "BMP" ||
		    file_name.substr(file_name.length() - 3) == "bmp")
		{
			file_name = ((std::string) WORKINGDIRECTORY) + "Assets/Textures/" + file_name;
			std::fstream file_test;

			file_test.open(file_name.c_str(), std::ios::in);

			if (file_test.is_open())
			{
				file_test.close();
			}
			else
			{
				file_name = "\n" + file_name + " Does not exist";
				LOG->message(file_name.c_str());
				return;
			}
			//this->SetBoundTextureID(this->m_TextureManager->CreateTexture(FileName) );
			this->m_texture_object = this->m_texture_manager->create_texture(file_name);
		}
	}
}

}        // namespace ror
