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

#include "profiling/rorlog.hpp"
#include "rhi/crtp_interfaces/rortexture.hpp"
#include "rhi/metal/rortexture.hpp"

namespace rhi
{

void TextureImageMetal::upload()
{
	(void) this->m_texture;
	ror::log_critical("Uploading texture to Metal");

	// // Create texture
	// // read_texture_from_file("./assets/astroboy/astro_boy.jpg", &data, tex_width, tex_height, tex_bpp);

	// this->format();

	// MTL::TextureDescriptor *texture_descriptor = MTL::TextureDescriptor::alloc()->init();
	// texture_descriptor->setWidth(this->width());
	// texture_descriptor->setHeight(this->height());
	// texture_descriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
	// texture_descriptor->setTextureType(MTL::TextureType2D);
	// texture_descriptor->setStorageMode(MTL::StorageModeManaged);
	// texture_descriptor->setUsage(MTL::ResourceUsageSample | MTL::ResourceUsageRead);

// texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
//																								 width:textureSize.width
//																								height:textureSize.height
//	[                                                                                         mipmapped:NO]

//	textureDescriptor.storageMode = MTLStorageModeShared;
//	textureDescriptor.storageMode = MTLStorageModeShared;


//	// Read https://developer.apple.com/documentation/metal/resource_fundamentals/copying_data_to_a_private_resource?language=objc
//	// for details of how to copy texture to private space in the GPU for fast access

//	// Unsigned normalized value (i.e. 0 maps to 0.0 and 255 maps to 1.0)
//	// Set the pixel dimensions of the texture
//	texture_descriptor->setWidth(this->width());
//	texture_descriptor->setHeight(this->height());
//	texture_descriptor->setPixelFormat();        // MTL::PixelFormatRGBA8Unorm);
//	texture_descriptor->setTextureType(MTL::TextureType2D);
//	texture_descriptor->setStorageMode(MTL::StorageModeManaged);
//	texture_descriptor->setUsage(MTL::ResourceUsageSample | MTL::ResourceUsageRead);

//	// Create the texture from the device by using the descriptor
//	this->m_texture = device->newTexture(texture_descriptor);

//	// // Copy data
//	// MTLRegion region = {
//	//     {0, 0, 0},                        // MTLOrigin
//	//     {tex_width, tex_height, 1}        // MTLSize
//	// };

//	uint32_t bytesPerRow = tex_bpp * tex_width;
//	texture->replaceRegion(MTL::Region(0, 0, 0, tex_width, tex_height, 1), 0, data, bytesPerRow);

//	texture_descriptor->release();

	// If you have uploaded it to a new private space then release the texture like
	// this->m_texture->release();

	// delete[] data;
}

define_translation_unit_vtable(TextureImageMetal)
{}

define_translation_unit_vtable(TextureSamplerMetal)
{}

define_translation_unit_vtable(TextureMetal)
{}

}        // namespace rhi
