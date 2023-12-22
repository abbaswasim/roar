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

#include <any>

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

namespace rhi
{
// From https://github.com/spnda/metal-cpp-bug/blob/main/glfw_bridge.mm
// Probably also need to add   target_compile_options(roar PRIVATE "-fno-objc-arc") somewhere some day
void *get_metal_layer(std::any a_window,               // NSWindow 
					  double a_width,                  // Width of the layer
					  double a_height,                 // Height of the layer
					  void *a_device,                  // MTLDevice pointer
					  unsigned int a_pixel_format)     // MTLPixelFormat, usually RGBA8Unorm
{
	CGSize size = {};
	size.width  = a_width;
	size.height = a_height;

	CAMetalLayer *layer = [CAMetalLayer layer];
	layer.device        = (__bridge static_cast<id<MTLDevice>>(a_device));
	layer.pixelFormat   = static_cast<MTLPixelFormat>(a_pixel_format); // TODO: Find out why Metal only supports BGRA and not RGBA
	layer.drawableSize  = size;
	layer.opaque        = YES;

	assert(layer.device && "CAMetalLayer device is null");

	NSWindow *nswindow              = (__bridge std::any_cast<id>(a_window));
	nswindow.contentView.layer      = layer;
	nswindow.contentView.wantsLayer = YES;

	return layer;
}

// TODO: Probably better to use the metalCPP way of getting this;
/*
#import <QuartzCore/QuartzCore.hpp>

CA::MetalLayer*    pMetalLayer    = layer associated with the view;
CA::MetalDrawable* pMetalDrawable = pMetalLayer->nextDrawable();
*/
void *next_drawable(void *a_layer)
{
	CAMetalLayer *metalLayer = (__bridge static_cast<CAMetalLayer *>(a_layer));
	return [metalLayer nextDrawable];
}

void release_layer(void *a_layer)
{
	CAMetalLayer *layer = (__bridge static_cast<CAMetalLayer *>(a_layer));
	if (layer)
		[layer release];
}

}
