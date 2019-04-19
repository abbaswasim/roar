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
FORCE_INLINE Export::Export(void)
{
	//m_File.open ("c:/Export.sdf", std::ios::out | std::ios::binary);
}

FORCE_INLINE Export::Export(uint32_t a_out, std::string a_file_name)
{
	if (a_out)
		this->m_file.open(a_file_name.c_str(), std::ios::out | std::ios::binary);
	else
		this->m_file.open(a_file_name.c_str(), std::ios::in | std::ios::binary);

	if (!this->m_file.is_open())
	{
		a_file_name += "\nThe File does Not exist.";
		MESSAGEBOX(a_file_name.c_str());
	}

	this->m_input = (a_out ? true : false);

	if (!a_out)
	{
		this->load_file();
	}
}

bool Export::load_file()
{
	// Find the size of the file
	this->m_file.seekg(0, std::ios_base::end);
	uint32_t file_size = m_file.tellg();
	this->m_file.seekg(0, std::ios_base::beg);

	if (!file_size)
	{
		MESSAGEBOXERROR("Seek Failed on the file...");
	}

	this->m_buffer = new char8_t[file_size];

	if (this->m_buffer == nullptr)
	{
		MESSAGEBOXERROR("Not Enough Memory for the Read...");
	}

	this->m_file.read(this->m_buffer, file_size);
	this->m_file.close();

	this->m_buffer1 = this->m_buffer;

	return true;
};

FORCE_INLINE void Export::write(char8_t *a_bytes, uint32_t a_size)
{
	this->m_file.write(a_bytes, a_size);
}
//
//	FORCE_INLINE void CExport::Write( uint32_t a_Size )
//	{
//		if (sizeof(a_Size) != 4)
//		{
//			// TODO: Handle the error
//			MessageBox(nullptr,L"Only 4 Bytes Integers are supported",L"Error...",MB_OK|MB_ICONERROR);
//		}
//		m_File.write((const char8_t*)&a_Size,sizeof(uint32_t));
//	}
//
//	FORCE_INLINE void CExport::Write( float32_t a_Size )
//	{
//		if (sizeof(a_Size) != 4)
//		{
//			// TODO: Handle the error
//			MessageBox(nullptr,L"Only 4 Bytes Integers are supported",L"Error...",MB_OK|MB_ICONERROR);
//		}
//		m_File.write((const char8_t*)&a_Size,sizeof(float32_t));
//	}

FORCE_INLINE void Export::read(char8_t *a_bytes, uint32_t a_size)
{
	memcpy(a_bytes, this->m_buffer, a_size);
	this->m_buffer += a_size;
	//m_File.read(a_Bytes,a_Size);
}

FORCE_INLINE void Export::read(float32_t *a_bytes, uint32_t a_size)
{
	memcpy((char8_t *) a_bytes, this->m_buffer, a_size);
	this->m_buffer += a_size;

	//m_File.read((char8_t*)a_Bytes,a_Size);
}

FORCE_INLINE void Export::read(uint32_t *a_bytes, uint32_t a_size)
{
	memcpy((char8_t *) a_bytes, this->m_buffer, a_size);
	this->m_buffer += a_size;

	//m_File.read((char8_t*)a_Bytes,a_Size);
}

FORCE_INLINE void Export::read(int32_t *a_bytes, uint32_t a_size)
{
	memcpy((char8_t *) a_bytes, this->m_buffer, a_size);
	this->m_buffer += a_size;

	//m_File.read((char8_t*)a_Bytes,a_Size);
}

FORCE_INLINE void Export::read(uint32_t *a_size1)
{
	if (sizeof(uint32_t) != 4)
	{
		MESSAGEBOXERROR("Only 4 Bytes Integers are supported");
	}

	memcpy((char8_t *) a_size1, this->m_buffer, sizeof(uint32_t));
	this->m_buffer += sizeof(uint32_t);

	//m_File.read( (char8_t*) a_Size1, sizeof(uint32_t) );
}

FORCE_INLINE void Export::read(int32_t *a_size1)
{
	if (sizeof(int32_t) != 4)
	{
		MESSAGEBOXERROR("Only 4 Bytes Integers are supported");
	}

	memcpy((char8_t *) a_size1, this->m_buffer, sizeof(int32_t));
	this->m_buffer += sizeof(int32_t);

	//m_File.read( (char8_t*) a_Size1, sizeof(int32_t) );
}

FORCE_INLINE void Export::read(rUsShort *a_size1)
{
	if (sizeof(rUsShort) != 2)
	{
		MESSAGEBOXERROR("Only 2 Bytes Shorts are supported");
	}

	memcpy((char8_t *) a_size1, this->m_buffer, sizeof(rUsShort));
	this->m_buffer += sizeof(rUsShort);

	//m_File.read( (char8_t*) a_Size1, sizeof(rUsShort) );
}

FORCE_INLINE void Export::read(rShort *a_size1)
{
	if (sizeof(rShort) != 2)
	{
		// TODO: Handle the error
		MESSAGEBOXERROR("Only 2 Bytes Shorts are supported");
	}

	memcpy((char8_t *) a_size1, this->m_buffer, sizeof(rShort));
	this->m_buffer += sizeof(rShort);

	//m_File.read( (char8_t*) a_Size1, sizeof(rShort) );
}

FORCE_INLINE void Export::read(float32_t *a_size1)
{
	if (sizeof(float32_t) != 4)
	{
		// TODO: Handle the error
		MESSAGEBOXERROR("Only 4 Bytes Floats are supported");
	}

	memcpy((char8_t *) a_size1, this->m_buffer, sizeof(float32_t));
	this->m_buffer += sizeof(float32_t);

	//m_File.read( (char8_t*) a_Size1, sizeof(float32_t) );
}

FORCE_INLINE void Export::finish_write()
{
	if (!m_input)
	{
		this->m_file.close();
	}
}

}        // namespace ror
