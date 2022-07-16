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


#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

namespace rhi
{
// From https://github.com/spnda/metal-cpp-bug/blob/main/glfw_bridge.mm
void *get_metal_layer(void *a_window,                  // NSWindow 
					  double a_width,                  // Width of the layer
					  double a_height,                 // Height of the layer
					  void *a_device,                  // MTLDevice pointer
					  unsigned int a_pixel_format)     // MTLPixelFormat, usually RGBA8Unorm
{
	(void) a_pixel_format;
	CGSize size = {};
	size.height = a_height;
	size.width  = a_width;

	CAMetalLayer *layer = [CAMetalLayer layer];
	layer.device        = (__bridge static_cast<id<MTLDevice>>(a_device));
	layer.pixelFormat   = static_cast<MTLPixelFormat>(a_pixel_format); // TODO: Find out why Metal only supports BGRA and not RGBA
	layer.drawableSize  = size;

	NSWindow *nswindow              = (__bridge static_cast<NSWindow*>(a_window));
	nswindow.contentView.layer      = layer;
	nswindow.contentView.wantsLayer = YES;

	return layer;
}

void *next_drawable(void *a_layer)
{
	CAMetalLayer *metalLayer = (__bridge static_cast<CAMetalLayer *>(a_layer));
	return [metalLayer nextDrawable];
}

}