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

#include "memory_manager/rormemory_check.h"

//#include "RORnv/NvTriStrip.h"
namespace ror
{
class ROAR_ENGINE_ITEM CCBA;
// CBA File header contains all Chunks information (58 Bytes in Total)
class ROAR_ENGINE_ITEM ccbaheader
{
  public:
	// CBA Main Chunk contains information about all other chunks (28 Bytes in Total)
	class ROAR_ENGINE_ITEM ccbamain_chunk
	{
	  public:
		ccbamain_chunk() :
		    m_magic_number(),
		    m_version(1),
		    m_no_of_sub_chunks(1),
		    m_has_animation_has_skeleton(0),
		    m_first_chunk_offset(0),
		    m_file_size(0),
		    m_extra_data(0)
		{}
		~ccbamain_chunk()
		{}

		uint32_t m_magic_number;                      // Always CBA0
		uint32_t m_chunk_id;                          // Must be 0
		uint32_t m_version;                           // Version initial 1, In this version only supports GeometryChunk
		uint32_t m_no_of_sub_chunks;                  // Total Number of Chunks
		uint32_t m_has_animation_has_skeleton;        // Has Animation Data and Skeleton for Animation or not. LowBytes = Animation, HighBytes = Skeleton
		uint32_t m_first_chunk_offset;                // From where does the First Chunk Starts
		uint32_t m_file_size;                         // Total number of Bytes in the File
		uint32_t m_extra_data;                        // Can be any number of Bytes after this bytes
	};

	class ROAR_ENGINE_ITEM ccbageometry_chunk        // Contains the Bind Position Geometry Data in CBA (40 Bytes in Total)
	{
	  public:
		ccbageometry_chunk() :
		    m_total_no_of_triangles(0),
		    m_no_of_triangle_groups(0),
		    m_data_offset(0)
		{
			//memcpy(m_TextureFileName,0,20);
		}
		~ccbageometry_chunk()
		{}

		uint32_t m_total_no_of_triangles;        // Total Triangles
		uint32_t m_chunk_id;                     // Must be 1
		uint32_t m_no_of_triangle_groups;        // Number of Triangle Groups Categorized by Material
		uint32_t m_triangles_details;            // 1 = Only Vertices, 2 = Vertices + Normals, 3 = Vertices + Normals + TextureCoords
		uint32_t m_data_offset;                  // Actual Data Starts from this Offset
		char8_t  m_texture_file_name[20];        // Texture files name (.jpg,.png,.tga,.bmp)
	};

	class ROAR_ENGINE_ITEM ccbaanimation_chunk        // Contains the Animation Data in CBA (0 Bytes in Total)
	{
	  public:
		//CCBAAnimationChunk(){}
		~ccbaanimation_chunk()
		{}

		ccbaanimation_chunk()
		{
			m_no_of_animations  = 0;
			m_chunk_id          = 2;
			m_no_of_keyframes   = 0;
			m_start_time        = 0.0f;
			m_total_time        = 0.0f;
			m_frames_per_second = 0.0f;
		}

		uint32_t  m_no_of_animations;         // Total Animations Keyframes
		uint32_t  m_chunk_id;                 // Always 2
		uint32_t  m_no_of_keyframes;          // Total Animations Keyframes
		float32_t m_start_time;               // Start Time of the animation
		float32_t m_total_time;               // End time or total time of the animation
		float32_t m_frames_per_second;        // Frames Per second to render
	};

	class ROAR_ENGINE_ITEM ccbabones_chunk        // Contains the Bones Data in CBA (0 Bytes in Total)
	{
	  public:
		ccbabones_chunk()
		{}
		~ccbabones_chunk()
		{}

		uint32_t m_total_no_of_bones;            // Total Bones
		uint32_t m_chunk_id;                     // Must be 1
		uint32_t m_no_of_triangle_groups;        // Number of Triangle Groups Categorized by Material
		uint32_t m_triangles_details;            // 1 = Only Vertices, 2 = Vertices + Normals, 3 = Vertices + Normals + TextureCoords
		uint32_t m_data_offset;                  // Actual Data Starts from this Offset
		char8_t  m_texture_file_name[20];        // Texture files name (.jpg,.png,.tga,.bmp)
	};

	FORCE_INLINE ccbageometry_chunk get_geometry_chunk() const;
	FORCE_INLINE void               set_geometry_chunk(ccbageometry_chunk a_geom_chunk);

  private:
	// CBA should be able to access the data
	friend class ROAR_ENGINE_ITEM CCBA;

	ccbamain_chunk      m_main_chunk;             // The Main Chunk of CBA file
	ccbageometry_chunk  m_geometry_chunk;         // The Geometry Chunk of CBA file
	ccbaanimation_chunk m_animation_chunk;        // The Animation Chunk of CBA file
};
}        // namespace ror

#include "rorcba_header.hh"
