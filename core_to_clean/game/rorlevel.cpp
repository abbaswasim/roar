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

#include "rorlevel.h"

namespace ror
{
Level::~Level(void)
{
	this->m_scene_root->delete_scene_graph();
	this->m_texture_manager->release_all_textures();
	this->m_font_manager->release_all_fonts();
	this->m_cbamanager->release_all_models();
	this->m_shader_manager->release_all_shaders();
	this->m_guimanager->release_all_elements();
	this->m_particles_system_manager->release_all_particle_systems();

	this->m_cbamanager               = nullptr;
	this->m_font_manager             = nullptr;
	this->m_guimanager               = nullptr;
	this->m_shader_manager           = nullptr;
	this->m_texture_manager          = nullptr;
	this->m_particles_system_manager = nullptr;

	rDELETE(m_cameras);
	rDELETE(m_lights);

	uint32_t dynamic_texture_size = (uint32_t) this->m_dynamic_textures->size();
	for (uint32_t i = 0; i < dynamic_texture_size; i++)
	{
		rDELETE((*this->m_dynamic_textures)[i]);
	}

	this->m_dynamic_textures->clear();

	rDELETE(m_dynamic_textures);
}

void Level::load_level(std::string a_file_name, ror::Camera **a_active_camera, ror::Light **a_shadow_light)
{
	TiXmlDocument doc(a_file_name.c_str());
	if (!doc.LoadFile())
		MESSAGEBOXERROR("Level Load Failed... \n Filename = Level.cpp");
	TiXmlHandle   h_doc(&doc);
	TiXmlElement *p_elem;
	TiXmlHandle   h_root(0);

	p_elem = h_doc.FirstChildElement().Element();

	// Get the GUI Filename
	std::string     guifile_name;
	TiXmlAttribute *attrib = p_elem->FirstAttribute();
	while (attrib)
	{
		std::string at = attrib->Name();
		if ("gui" == at)
		{
			guifile_name = attrib->Value();
		}
		attrib = attrib->Next();
	}

	std::string name = p_elem->Value();

	TiXmlNode *                                  p_parent = h_doc.ToNode();
	TiXmlNode *                                  p_child;
	std::unordered_map<std::string, std::string> entities;
	p_parent = p_parent->FirstChildElement();
	for (p_child = p_parent->FirstChild(); p_child != 0; p_child = p_child->NextSibling())
	{
		std::string t = p_child->ToElement()->Value();
		if (t == "entities")
		{
			entities = parse_entities(p_child);
		}
		else if (t == "scene")
		{
			parse_scene(p_child, entities, a_active_camera, a_shadow_light);
		}
		else if ("window" == t)
		{
			//TODO : ParseWindow(pChild, Entities, a_ActiveCamera);
		}
	}
	// Load the GUI Scene
	this->load_guiscene(guifile_name);
	//this->m_GUIManager->Initialize();
}

void Level::load_guiscene(std::string a_file_name)
{
	a_file_name = WORKINGDIRECTORY + a_file_name;
	TiXmlDocument doc(a_file_name.c_str());
	if (!doc.LoadFile())
		MESSAGEBOXERROR("Level GUI Load Failed... \n Filename = Level.cpp");
	TiXmlHandle   h_doc(&doc);
	TiXmlElement *p_elem;
	TiXmlHandle   h_root(0);

	p_elem = h_doc.FirstChildElement().Element();

	std::string name = p_elem->Value();

	TiXmlNode *p_parent = h_doc.ToNode();
	TiXmlNode *p_child;
	p_parent = p_parent->FirstChildElement();
	for (p_child = p_parent->FirstChild(); p_child != 0; p_child = p_child->NextSibling())
	{
		std::string t = p_child->ToElement()->Value();
		if (t == "element")
		{
			this->parse_guielement(p_child);
		}
	}
	// Now Initialize the GUI Manager And Assign the CallBacks
	this->m_guimanager->initialize();
}

cguielement *Level::parse_guielement(TiXmlNode *a_guielement)
{
	// First we need to read the Attributes for the Element
	// These are all the possible attributes
	//<element name="Button" text="Debug Info..." position="190 25 400" size="22 22" visible="TRUE" ground="TRUE" foreground="128 255 0" background="255 128 0 128" border="255 255 255" nonclientarea="1 1 1 1">
	std::string     name, text, position, size, ground, visible, foreground, background, border, align, nonclientarea;
	TiXmlAttribute *attrib = a_guielement->ToElement()->FirstAttribute();
	while (attrib)
	{
		std::string at = attrib->Name();
		if ("name" == at)
		{
			name = attrib->Value();
		}
		else if ("text" == at)
		{
			text = attrib->Value();
		}
		else if ("position" == at)
		{
			position = attrib->Value();
		}
		else if ("size" == at)
		{
			size = attrib->Value();
		}
		else if ("ground" == at)
		{
			ground = attrib->Value();
		}
		else if ("visible" == at)
		{
			visible = attrib->Value();
		}
		else if ("foreground" == at)
		{
			foreground = attrib->Value();
		}
		else if ("background" == at)
		{
			background = attrib->Value();
		}
		else if ("border" == at)
		{
			border = attrib->Value();
		}
		else if ("align" == at)
		{
			align = attrib->Value();
		}
		else if ("nonclientarea" == at)
		{
			nonclientarea = attrib->Value();
		}
		attrib = attrib->Next();
	}

	// Second we have to read the Properties for the Element like texture, font, callback etc
	cguigraphics_element *element = rNew ror::cguigraphics_element();
	TiXmlNode *                          child;
	Vector4f                             texture_offset;
	TextureObject *                      texture_object = nullptr;
	IFunctor *                           call_back      = nullptr;
	bool                                 scalebale      = true;
	bool                                 groundb        = true;
	int32_t                              font_index     = -1;
	for (child = a_guielement->FirstChild(); child != 0; child = child->NextSibling())
	{
		std::string name = child->ToElement()->Value();
		if ("font" == name)
		{
			font_index = this->read_font(child);
		}
		else if ("texture" == name)
		{
			texture_object = this->read_texture(child, texture_offset, scalebale);
		}
		else if ("callback" == name)
		{
			this->read_call_back(child, element);
		}
	}
	// Now we combine everything in one and create the GUIElement here
	Vector3i  positionv;
	Vector4f *backgroundv = rNew Vector4f();
	Vector3f *foregroundv = rNew Vector3f();
	Vector2i                     bounds;
	Vector4i                     non_client_area;
	cguielement::EElementType    element_type = cguielement::rPANEL;
	cguielement::EAlignment      alignment;
	bool                         visibleb = true;

	if ("FALSE" == visible)
		visibleb = false;
	if ("FALSE" == ground)
		groundb = false;

	// TODO : Use the Name as well as the border, Name is Done
	//position,size,foreground,background,border;
	this->string_to_integers(position, 3, positionv.m_values);
	this->string_to_integers(size, 2, bounds.m_values);
	this->string_to_floats(foreground, 3, foregroundv->m_values);
	this->string_to_floats(background, 4, backgroundv->m_values);
	this->string_to_integers(nonclientarea, 4, non_client_area.m_values);
	alignment = this->enumerate_alignment(align);

	// Normalize foreground and background
	*backgroundv /= 255;
	*foregroundv /= 255;

	element->set(nullptr, text, positionv, non_client_area, visibleb, groundb, backgroundv, foregroundv, bounds, call_back, element_type, font_index, alignment, texture_object, texture_offset, scalebale);
	this->m_guimanager->add_element(element, name);
	element->initialize(0);

	// Now we read all its possible Children
	ror::cguielement *children_element;
	for (child = a_guielement->FirstChild(); child != 0; child = child->NextSibling())
	{
		std::string name = child->ToElement()->Value();
		if ("label" == name)
		{
			children_element = nullptr;
			children_element = this->read_label(child, element);
			if (nullptr != children_element)
			{
				element->increment_sequence_in_parent(children_element);
			}
		}
		if ("button" == name)
		{
			children_element = nullptr;
			children_element = this->read_button(child, element);
			if (nullptr != children_element)
			{
				element->increment_sequence_in_parent(children_element);
			}
		}
	}
	//		if (true == this->m_GUIManager->AddElement(Element))
	//			return Element;
	//		else
	//			return nullptr;
	return element;
}

void Level::string_to_integers(std::string a_string, int32_t a_size, int32_t *a_ints /*int32_t **a_Ints */)
{
	//int32_t *Integers = new int32_t[a_Size];
	// Tokenize the String And extract the Values as Integers
	try
	{
		std::stringstream sstring(a_string);
		uint32_t          index = 0;
		while (sstring >> /*Integers*/ (a_ints)[index++])
			;
	}
	catch (int32_t)
	{
		MESSAGEBOXERROR("Exception Occured in RORLevel.CPP");
	}
	//*a_Ints = Integers;
}

void Level::string_to_floats(std::string a_string, int32_t a_size, float32_t *a_floats)
{
	try
	{
		// Tokenize the String And extract the Values as Floats
		std::stringstream sstring(a_string);
		uint32_t          index = 0;
		while (sstring >> (a_floats)[index++])
			;
	}
	catch (int32_t)
	{
		MESSAGEBOXERROR("Exception Occured in RORLevel.CPP");
	}
}

int32_t Level::read_font(TiXmlNode *a_font_node)
{
	// Read the Attributes
	//<font name="Trajan Pro" height="20" bold="TRUE" italic="FALSE" />
	std::string     name = "", height = "", bold = "", italic = "";
	TiXmlAttribute *attrib = a_font_node->ToElement()->FirstAttribute();
	while (attrib)
	{
		std::string at = attrib->Name();
		if ("name" == at)
			name = attrib->Value();
		else if ("height" == at)
			height = attrib->Value();
		else if ("bold" == at)
			bold = attrib->Value();
		else if ("italic" == at)
			italic = attrib->Value();

		attrib = attrib->Next();
	}
	int32_t heighti = 12;
	bool    boldb = true, italicb = false;

	// Tokenize the String And extract the Values
	std::stringstream ssheight(height);
	ssheight >> heighti;

	if ("FALSE" == bold)
		boldb = false;
	if ("TRUE" == italic)
		italicb = true;

	return this->m_font_manager->get_font(this->m_font_manager->create_font(name, heighti, boldb, italicb));
}

TextureObject *Level::read_texture(TiXmlNode *a_texture_node, Vector4f &a_texture_offset, bool &a_scalable)
{
	// Read the Attributes
	//<texture source="Assets/GUI/FrameTest.tga" scalable="TRUE" textureoffset="0 0 100 100" wraps="REPEAT" wrapt="REPEAT" />
	std::string     source = "", scalable = "", textureoffset = "", wraps = "", wrapt = "";
	TiXmlAttribute *attrib = a_texture_node->ToElement()->FirstAttribute();
	while (attrib)
	{
		std::string at = attrib->Name();
		if ("source" == at)
			source = attrib->Value();
		else if ("scalable" == at)
			scalable = attrib->Value();
		else if ("textureoffset" == at)
			textureoffset = attrib->Value();
		else if ("wraps" == at)
			wraps = attrib->Value();
		else if ("wrapt" == at)
			wrapt = attrib->Value();

		attrib = attrib->Next();
	}
	bool scalableb = true, wrapsb = true, wraptb = false;

	if ("FALSE" == scalable)
		scalableb = false;

	try
	{
		Vector4f texture_offset;
		this->string_to_floats(textureoffset, 4, texture_offset.m_values);
		a_texture_offset = texture_offset;
	}
	catch (int32_t)
	{
		MESSAGEBOXERROR("Error Occurred here : RORLevel.cpp");
	}

	a_scalable = scalableb;
	source     = WORKINGDIRECTORY + source;
	return this->m_texture_manager->create_texture(source.c_str());
}

void Level::read_call_back(TiXmlNode *a_functor_node, cguielement *a_element)
{
	// Read the Attributes
	//<callback name="TestCallBack" classid="SomeClass" function="SomeFunction" />
	std::string     name = "", class_id = "", function = "";
	TiXmlAttribute *attrib = a_functor_node->ToElement()->FirstAttribute();
	while (attrib)
	{
		std::string at = attrib->Name();
		if ("name" == at)
			name = attrib->Value();
		else if ("classid" == at)
			class_id = attrib->Value();
		else if ("function" == at)
			function = attrib->Value();

		attrib = attrib->Next();
	}

	this->m_functors->push_back(FunctorData(a_element, name, class_id, function));
}

cguielement *Level::read_label(TiXmlNode *a_label_node, cguielement *a_parent)
{
	// Read the Attributes
	//<label name="GUIPanelTextVisible" text="Visible : " visible="FALSE" ground="TRUE" align="LEFT" nonclientarea="4 4 30 5"/>
	std::string     name, text, visible, ground, align, nonclientarea;
	TiXmlAttribute *attrib = a_label_node->ToElement()->FirstAttribute();
	while (attrib)
	{
		std::string at = attrib->Name();
		if ("name" == at)
			name = attrib->Value();
		else if ("text" == at)
			text = attrib->Value();
		else if ("align" == at)
			align = attrib->Value();
		else if ("nonclientarea" == at)
			nonclientarea = attrib->Value();
		else if ("visible" == at)
			visible = attrib->Value();
		else if ("ground" == at)
			ground = attrib->Value();

		attrib = attrib->Next();
	}

	IFunctor *                   call_back  = nullptr;
	int32_t                      font_index = -1;
	ror::cguielement::EAlignment alignment  = this->enumerate_alignment(align);
	cguielement *element                    = rNew ror::cguilabel;
	ror::cguilabel *            temp_label  = dynamic_cast<ror::cguilabel *>(element);
	ror::Vector4i               non_client_area;
	bool                        visibleb = true;
	bool                        groundb  = false;

	if ("FALSE" == visible)
		visibleb = false;
	if ("TRUE" == ground)
		ground = true;

	this->string_to_integers(nonclientarea, 4, non_client_area.m_values);
	// Lets Check for its Children, we might have properties in it
	TiXmlNode *child;
	for (child = a_label_node->FirstChild(); child != 0; child = child->NextSibling())
	{
		std::string name = child->ToElement()->Value();
		if ("font" == name)
		{
			font_index = this->read_font(child);
		}
	}

	temp_label->set(a_parent, text, visibleb, groundb, call_back, font_index, alignment, non_client_area);
	this->m_guimanager->add_element(element, name);
	element->initialize(0);
	return element;
}

cguielement *Level::read_button(TiXmlNode *a_button_node, cguielement *a_parent)
{
	// Read the Attributes
	//<button name="GUIPanelClose" text="123456789" visible="TRUE" ground="TRUE" align="CENTER" nonclientarea="3 3 2 2" >
	std::string     name, text, visible, ground, align, nonclientarea;
	TiXmlAttribute *attrib = a_button_node->ToElement()->FirstAttribute();
	while (attrib)
	{
		std::string at = attrib->Name();
		if ("name" == at)
			name = attrib->Value();
		else if ("text" == at)
			text = attrib->Value();
		else if ("align" == at)
			align = attrib->Value();
		else if ("nonclientarea" == at)
			nonclientarea = attrib->Value();
		else if ("visible" == at)
			visible = attrib->Value();
		else if ("ground" == at)
			ground = attrib->Value();

		attrib = attrib->Next();
	}

	IFunctor *                   call_back  = nullptr;
	int32_t                      font_index = -1;
	ror::cguielement::EAlignment alignment  = this->enumerate_alignment(align);
	cguielement *element                    = rNew ror::cguibutton;
	ror::cguibutton *           temp_button = dynamic_cast<ror::cguibutton *>(element);
	ror::Vector4i               non_client_area;
	bool                        visibleb = true;
	bool                        groundb  = false;

	if ("FALSE" == visible)
		visibleb = false;
	if ("TRUE" == ground)
		groundb = true;

	// Lets Check for its Children, we might have properties in it
	TiXmlNode *child;
	for (child = a_button_node->FirstChild(); child != 0; child = child->NextSibling())
	{
		std::string name = child->ToElement()->Value();
		if ("font" == name)
		{
			font_index = this->read_font(child);
		}
		else if ("callback" == name)
		{
			this->read_call_back(child, element);
		}
	}

	this->string_to_integers(nonclientarea, 4, non_client_area.m_values);
	temp_button->set(a_parent, text, visibleb, groundb, call_back, font_index, alignment, non_client_area);
	this->m_guimanager->add_element(element, name);
	//Element->Initialize(); // This will be done incrementally in the case of a button

	// Lets Check for its Children, for States
	for (child = a_button_node->FirstChild(); child != 0; child = child->NextSibling())
	{
		std::string name = child->ToElement()->Value();
		if ("up" == name)
		{
			this->read_button_state(child, temp_button, cguibutton::rBUTTON_UP);
		}
		else if ("over" == name)
		{
			this->read_button_state(child, temp_button, cguibutton::rBUTTON_OVER);
		}
		else if ("down" == name)
		{
			this->read_button_state(child, temp_button, cguibutton::rBUTTON_DOWN);
		}
	}
	temp_button->set_current_state(cguibutton::rBUTTON_UP);
	return element;
}

void Level::read_button_state(TiXmlNode *a_button_node, ror::cguibutton *a_button, cguibutton::EButtonState a_state)
{
	// Read the Attributes
	//<up text="Click Me">
	std::string     text;
	TiXmlAttribute *attrib = a_button_node->ToElement()->FirstAttribute();
	while (attrib)
	{
		std::string at = attrib->Name();
		if ("text" == at)
			text = attrib->Value();
		attrib = attrib->Next();
	}
	// Second we have to read the Properties for the Element like texture
	TiXmlNode *    child;
	Vector4f       texture_offset;
	TextureObject *texture_object = nullptr;
	bool           scalebale      = true;
	bool           ground         = true;

	for (child = a_button_node->FirstChild(); child != 0; child = child->NextSibling())
	{
		std::string name = child->ToElement()->Value();
		if ("font" == name)
		{
			// No Font Support for Individual State
		}
		else if ("texture" == name)
		{
			texture_object = this->read_texture(child, texture_offset, scalebale);
		}
	}

	a_button->set_states(a_state, text);
	a_button->set_texture_offset(texture_offset);
	a_button->set_scalable_texture(scalebale);
	if (nullptr != texture_object)
	{
		a_button->set_texture_object(texture_object);
	}
	a_button->initialize((uint32_t) a_state);
}

Level::entities_map Level::parse_entities(TiXmlNode *a_entities)
{
	entities_map entities;
	TiXmlNode *  child;

	for (child = a_entities->FirstChild(); child != 0; child = child->NextSibling())
	{
		std::string name = child->ToElement()->Value();
		if ("entity" == name)
		{
			std::string     ID, file_name;
			TiXmlAttribute *attrib = child->ToElement()->FirstAttribute();
			while (attrib)
			{
				std::string at = attrib->Name();
				if ("id" == at)
				{
					ID = attrib->Value();
				}
				else if ("filename" == at)
				{
					file_name = attrib->Value();
				}
				attrib = attrib->Next();
			}

			entities.insert(std::make_pair(ID, file_name));
		}
	}
	return entities;
}

void Level::parse_scene(TiXmlNode *a_entities, entities_map &a_entities_map, ror::Camera **a_active_camera, ror::Light **a_shadow_light)
{
	TiXmlNode *child;
	for (child = a_entities->FirstChild(); child != 0; child = child->NextSibling())
	{
		std::string name = child->ToElement()->Value();
		if ("node" == name)
		{
			parse_node(child->ToElement(), a_entities_map, nullptr, a_active_camera, a_shadow_light);
		}
	}
}

void Level::parse_node(TiXmlNode *a_entities, entities_map &a_entities_map, ror::Entities *a_parent, ror::Camera **a_active_camera, ror::Light **a_shadow_light)
{
	assert(a_entities && "Entities Node is null");
	TiXmlNode *child;
	// <node geometryreference="1" vertexshader="na" fragmentshader="na" type="Geometry" texturefile="boy_11.tga" dynamictexture="FALSE">
	std::string     geometry_reference, vertex_shader_file, fragment_shader_file, type, dynamic_texture, *texture_file = nullptr, ambient, diffuse, specular;
	TiXmlAttribute *attrib = a_entities->ToElement()->FirstAttribute();
	// Get the attributes
	while (attrib)
	{
		std::string at = attrib->Name();
		if ("geometryreference" == at)
		{
			geometry_reference = attrib->Value();
		}
		else if ("vertexshader" == at)
		{
			vertex_shader_file = attrib->Value();
		}
		else if ("dynamictexture" == at)
		{
			dynamic_texture = attrib->Value();
		}
		else if ("fragmentshader" == at)
		{
			fragment_shader_file = attrib->Value();
		}
		else if ("type" == at)
		{
			type = attrib->Value();
		}
		else if ("texturefile" == at)
		{
			texture_file  = new std::string;
			*texture_file = attrib->Value();
		}
		else if ("ambient" == at)
		{
			ambient = attrib->Value();
		}
		else if ("diffuse" == at)
		{
			diffuse = attrib->Value();
		}
		else if ("specular" == at)
		{
			specular = attrib->Value();
		}

		attrib = attrib->Next();
	}

	// Get the Matrix
	child           = a_entities->FirstChild();        // Child should be the Matrix Node
	std::string mat = child->Value();
	assert("matrix" == mat && "Matrix has garbage");        // Child should be the Matrix Node
	TiXmlHandle text_handle   = child->FirstChild();
	TiXmlText * text          = text_handle.Text();
	std::string matrix_string = text->Value();

	// Tokenize the String And extract as float32_ts
	std::stringstream matrix_stream(matrix_string);            // Insert the string into a stream
	Matrix4f *matrix_values          = rNew Matrix4f();        // Create vector to hold our words
	uint32_t                       i = 0;

	while (matrix_stream >> matrix_values->m_values[i++])
		;

	matrix_values->transpose();

	assert(17 == i && "Matrix size can't be more then 16 floats");

	ror::Entities *current_entity = nullptr;

	if ("Geometry" == type)
	{
		current_entity = rNew ror::GeometryNode();
		current_entity->set_node_type(ror::Entities::rGEOMETRY);
		((GeometryNode *) current_entity)->set_geometry(this->m_cbamanager->load_model(WORKINGDIRECTORY + (a_entities_map.find(geometry_reference))->second));
		((GeometryNode *) current_entity)->setup_texture(texture_file);
		//CurrentEntity->SetUseGravity(true);
		current_entity->set_in_motion(true);

		// Hack :: Lets try to remove the Base from Shadow Caster
		//			if ("8" == GeometryReference)
		//			{
		//				CurrentEntity->SetObjectType(CEntities::rRENDERABLE | CEntities::rSHADOW_RECIEVER);
		//			}
		if ("Skydom" == geometry_reference)
		{
			current_entity->set_object_type(Entities::rNON_SHADOW_CASTER_RECIEVER);
		}
		if ("TRUE" == dynamic_texture)        // The Object Texture must be rendered separate
		{
			this->setup_dynamic_texture(current_entity);
		}
	}
	else if ("Clouds" == type)
	{
		current_entity = rNew ror::CloudsNode();
		current_entity->set_node_type(ror::Entities::rCLOUDS);
		current_entity->set_object_type(Entities::rNON_SHADOW_CASTER_RECIEVER);
		((CloudsNode *) current_entity)->set_geometry(this->m_cbamanager->load_model(WORKINGDIRECTORY + (a_entities_map.find(geometry_reference))->second));
		((CloudsNode *) current_entity)->set_scrolling_speed(matrix_values->m_values[15]);
		((CloudsNode *) current_entity)->setup_texture();
		matrix_values->m_values[15] = 1.0f;        // Reset the Homogeneous Coordinates
	}
	else if ("Particles" == type)
	{
		current_entity = dynamic_cast<Entities *>(this->m_particles_system_manager->load_particle_system("SomeText.ptl", ParticleSystemBase::rFOUNTAIN));
		current_entity->set_node_type(ror::Entities::rPARTICLES);
		current_entity->set_object_type(Entities::rNON_SHADOW_CASTER_RECIEVER);
		//			((CParticlesNode*)CurrentEntity)->SetGeometry(m_CBAManager->LoadModel(WORKINGDIRECTORY + (a_EntitiesMap.find(GeometryReference))->second));
		//			((CParticlesNode*)CurrentEntity)->SetScrollingSpeed(MatrixValues->m_Values[15]);
		//			((CParticlesNode*)CurrentEntity)->SetupTexture();
	}
	else if ("Billboard" == type)
	{
		current_entity = rNew ror::BillboardNode();
		current_entity->set_node_type(ror::Entities::rBILLBOARD);
		((BillboardNode *) current_entity)->load_geometry((a_entities_map.find(geometry_reference))->second);
	}
	else if ("Terrain" == type)
	{
		current_entity = rNew ror::TerrainNode();
		current_entity->set_node_type(ror::Entities::rTERRAIN);
		//CurrentEntity->SetObjectType(CEntities::rRENDERABLE | CEntities::rSHADOW_RECIEVER);
		((TerrainNode *) current_entity)->set_geometry(this->m_cbamanager->load_model(WORKINGDIRECTORY + (a_entities_map.find(geometry_reference))->second));
		((TerrainNode *) current_entity)->setup_texture();
	}
	else if ("MayaCamera" == type)
	{
		//CoreEngine::CEntities::rCAMERA;
		current_entity = rNew ror::MayaCamera();
		current_entity->set_node_type(ror::Entities::rCAMERA);
		if ("ActiveCamera" == geometry_reference)
		{
			*a_active_camera = dynamic_cast<MayaCamera *>(current_entity);
		}
		this->m_cameras->push_back(dynamic_cast<Camera *>(current_entity));
	}
	else if ("FirstPersonCamera" == type)
	{
		//CoreEngine::CEntities::rCAMERA;
		current_entity = rNew ror::FirstPersonCamera();
		current_entity->set_node_type(ror::Entities::rCAMERA);
		Vector3f cam_position = Vector3f(matrix_values->m_values[12], matrix_values->m_values[13], matrix_values->m_values[14]);
		(dynamic_cast<FirstPersonCamera *>(current_entity))->set_camera_position(cam_position);
		if ("ActiveCamera" == geometry_reference)
		{
			*a_active_camera = dynamic_cast<FirstPersonCamera *>(current_entity);
		}
		this->m_cameras->push_back(dynamic_cast<Camera *>(current_entity));
	}
	else if ("Light" == type)
	{
		//CoreEngine::CEntities::rLIGHT;
		current_entity = rNew ror::LightNode();
		(dynamic_cast<LightNode *>(current_entity))->set_light_position(Vector4f(matrix_values->m_values[12], matrix_values->m_values[13], matrix_values->m_values[14], matrix_values->m_values[15]));
		matrix_values->m_values[15] = 1.0f;        // Since may be we have used the Light matrix for Omni Lights
		current_entity->set_node_type(ror::Entities::rLIGHT);
		// Here i must extract the values from the strings of Specular etc
		// Tokenize the String And extract as float32_ts
		std::stringstream diffuse_stream(diffuse), ambient_stream(ambient), specular_stream(specular);        // Insert the string into a stream
		Vector4f          diffuse_vector, ambient_vector, specular_vector;

		uint32_t i = 0;
		while (diffuse_stream >> diffuse_vector.m_values[i++])
			;
		i = 0;
		while (ambient_stream >> ambient_vector.m_values[i++])
			;
		i = 0;
		while (specular_stream >> specular_vector.m_values[i++])
			;

		(dynamic_cast<LightNode *>(current_entity))->set_specular(specular_vector);
		(dynamic_cast<LightNode *>(current_entity))->set_diffuse(diffuse_vector);
		(dynamic_cast<LightNode *>(current_entity))->set_ambient(ambient_vector);
		if ("ShadowLight" == geometry_reference)
		{
			*a_shadow_light = dynamic_cast<LightNode *>(current_entity);
		}
	}
	else if ("Dummy" == type)
	{
		//CoreEngine::CEntities::rDUMMY;
		current_entity = rNew ror::DummyNode();
		current_entity->set_node_type(ror::Entities::rDUMMY);
	}
	else
	{
		MESSAGEBOXERROR("Unknown type of Object Present in Scene ..");
	}

	assert(current_entity && "Entities Node is null");

	if (nullptr == a_parent)        // Root Node
		this->m_scene_root = current_entity;
	else        // Other Nodes
		a_parent->attach_child(current_entity);

	a_parent = current_entity;

	{
		/*
glMatrixMode(GL_MODELVIEW);
CMatrix4f temp;
float32_t Size = 20.0f;
float32_t SizeIncrement = Size / Size;
SizeIncrement *= 0.5;
if ("7" == GeometryReference)
{
for (float32_t i = -Size; i < Size; i += SizeIncrement)
{
for (float32_t j = -Size; j < Size; j += SizeIncrement)
{
CVector3f tempVector(i,0.0f,j);
glPushMatrix();
glLoadMatrixf(MatrixValues->m_Values);
tempVector.CallGlTranslate();
glGetFloatv(GL_MODELVIEW_MATRIX,temp.m_Values);
glPopMatrix();
CoreEngine::CGeometryNode *ChildEntity = rNew CoreEngine::CGeometryNode();
// *ChildEntity = *(dynamic_cast<CoreEngine::CGeometryNode*> (CurrentEntity));
ChildEntity->SetNodeType(CoreEngine::CEntities::rGEOMETRY);
((CGeometryNode*)ChildEntity)->SetGeometryNode(m_CBAManager->LoadModel(WORKINGDIRECTORY + (a_EntitiesMap.find(GeometryReference))->second));
CurrentEntity->SetInMotion(true);
ChildEntity->SetTransformationMatrices(&temp);
CurrentEntity->AttachChild(ChildEntity);
temp.Identity();
}
}
}
*/
	}

	current_entity->set_transformation_matrices(matrix_values);

	ror::OGLShader *vertex_shader   = nullptr;
	ror::OGLShader *fragment_shader = nullptr;

	ror::OGLShaderProgram *shader_program = nullptr;

	if ("na" != vertex_shader_file && "na" != fragment_shader_file && "" != vertex_shader_file && "" != fragment_shader_file)
	{
		shader_program = rNew ror::OGLShaderProgram(WORKINGDIRECTORY + (a_entities_map.find(vertex_shader_file))->second, WORKINGDIRECTORY + (a_entities_map.find(fragment_shader_file))->second);
	}
	else if ("na" != vertex_shader_file && "" != vertex_shader_file)
	{
		shader_program = rNew ror::OGLShaderProgram(WORKINGDIRECTORY + (a_entities_map.find(vertex_shader_file))->second, rGL_VERTEX_SHADER);
	}
	else if ("na" != fragment_shader_file && "" != fragment_shader_file)
	{
		shader_program = rNew ror::OGLShaderProgram(WORKINGDIRECTORY + (a_entities_map.find(fragment_shader_file))->second, rGL_VERTEX_SHADER);
	}

	current_entity->set_shader(shader_program);

	for (child = a_entities->FirstChild(); child != 0; child = child->NextSibling())
	{
		std::string node = child->Value();
		if ("node" == node)
		{
			parse_node(child, a_entities_map, a_parent, a_active_camera, a_shadow_light);
		}
	}

	rDELETE(matrix_values);
}

void Level::un_load_level()
{}

void Level::setup_dynamic_texture(ror::Entities *a_entity)
{
	ror::GeometryNode *current_model = ((GeometryNode *) a_entity);
	assert(nullptr != current_model && "Entity is not a model");

	if (nullptr == current_model->get_texture_object())
	{
		// So we probably don't have texture file present. so lets make a texture for that.
		// TODO : Now this will not work for more then One such case, so make the "SOmeTING" variable
		TextureObject *temp = OGLTextureManager::get_instance()->create_texture("SOmeTING", TextureObject::rGL_TEXTURE_2D, 4, 512, 512);
		current_model->set_texture_object(temp);
	}

	ror::OGLFrameBuffer *new_frame_buffer   = rNew ror::OGLFrameBuffer();
	ror::OGLRenderBuffer *new_render_buffer = rNew ror::OGLRenderBuffer();

	new_frame_buffer->set_geometry_node(((GeometryNode *) a_entity));

	new_render_buffer->load_render_buffer(ror::OGLRenderBuffer::rGL_DEPTH_COMPONENT,
	                                      current_model->get_texture_object()->get_texture_width(),
	                                      current_model->get_texture_object()->get_texture_height());

	new_frame_buffer->load_frame_buffer();
	new_frame_buffer->attach_texture_object(current_model->get_texture_object());
	//		NewFrameBuffer->AttachTextureObject(temp);
	new_frame_buffer->attach_render_buffer(new_render_buffer, ror::OGLFrameBuffer::rGL_ATTACH_DEPTH);
	rDELETE(new_render_buffer);
	this->m_dynamic_textures->push_back(new_frame_buffer);
	new_frame_buffer->bind_buffer(false);
}

cguielement::EAlignment Level::enumerate_alignment(std::string a_alignment)
{
	const char8_t *alignment = a_alignment.c_str();
	if (!strcmp("LEFT_CENTER", alignment))
		return cguielement::rLEFT_CENTER;
	else if (!strcmp("TOP_LEFT", alignment))
		return cguielement::rTOP_LEFT;
	else if (!strcmp("BOTTOM_LEFT", alignment))
		return cguielement::rBOTTOM_LEFT;
	else if (!strcmp("LEFT", alignment))
		return cguielement::rLEFT;
	else if (!strcmp("RIGHT_CENTER", alignment))
		return cguielement::rRIGHT_CENTER;
	else if (!strcmp("TOP_RIGHT", alignment))
		return cguielement::rTOP_RIGHT;
	else if (!strcmp("BOTTOM_RIGHT", alignment))
		return cguielement::rBOTTOM_RIGHT;
	else if (!strcmp("RIGHT", alignment))
		return cguielement::rRIGHT;
	else if (!strcmp("CENTER", alignment))
		return cguielement::rCENTER;
	else if (!strcmp("TOP_CENTER", alignment))
		return cguielement::rTOP_CENTER;
	else if (!strcmp("BOTTOM_CENTER", alignment))
		return cguielement::rBOTTOM_CENTER;
	else
		return cguielement::rLEFT;
}

}        // namespace ror
