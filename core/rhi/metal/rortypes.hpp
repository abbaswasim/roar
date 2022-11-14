// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2021
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

#include <Metal/MTLPixelFormat.hpp>
#include <Metal/MTLRenderCommandEncoder.hpp>
#include <Metal/MTLResource.hpp>
#include <Metal/MTLSampler.hpp>
#include <Metal/MTLVertexDescriptor.hpp>

namespace rhi
{
// Pixel formats
constexpr auto PlatformPixelFormatInvalid               = MTL::PixelFormat::PixelFormatInvalid;
constexpr auto PlatformPixelFormatA8Unorm               = MTL::PixelFormat::PixelFormatA8Unorm;
constexpr auto PlatformPixelFormatR8Unorm               = MTL::PixelFormat::PixelFormatR8Unorm;
constexpr auto PlatformPixelFormatR8Unorm_sRGB          = MTL::PixelFormat::PixelFormatR8Unorm_sRGB;
constexpr auto PlatformPixelFormatR8Snorm               = MTL::PixelFormat::PixelFormatR8Snorm;
constexpr auto PlatformPixelFormatR8Uint                = MTL::PixelFormat::PixelFormatR8Uint;
constexpr auto PlatformPixelFormatR8Sint                = MTL::PixelFormat::PixelFormatR8Sint;
constexpr auto PlatformPixelFormatR16Unorm              = MTL::PixelFormat::PixelFormatR16Unorm;
constexpr auto PlatformPixelFormatR16Snorm              = MTL::PixelFormat::PixelFormatR16Snorm;
constexpr auto PlatformPixelFormatR16Uint               = MTL::PixelFormat::PixelFormatR16Uint;
constexpr auto PlatformPixelFormatR16Sint               = MTL::PixelFormat::PixelFormatR16Sint;
constexpr auto PlatformPixelFormatR16Float              = MTL::PixelFormat::PixelFormatR16Float;
constexpr auto PlatformPixelFormatRG8Unorm              = MTL::PixelFormat::PixelFormatRG8Unorm;
constexpr auto PlatformPixelFormatRG8Unorm_sRGB         = MTL::PixelFormat::PixelFormatRG8Unorm_sRGB;
constexpr auto PlatformPixelFormatRG8Snorm              = MTL::PixelFormat::PixelFormatRG8Snorm;
constexpr auto PlatformPixelFormatRG8Uint               = MTL::PixelFormat::PixelFormatRG8Uint;
constexpr auto PlatformPixelFormatRG8Sint               = MTL::PixelFormat::PixelFormatRG8Sint;
constexpr auto PlatformPixelFormatB5G6R5Unorm           = MTL::PixelFormat::PixelFormatB5G6R5Unorm;
constexpr auto PlatformPixelFormatA1BGR5Unorm           = MTL::PixelFormat::PixelFormatA1BGR5Unorm;
constexpr auto PlatformPixelFormatABGR4Unorm            = MTL::PixelFormat::PixelFormatABGR4Unorm;
constexpr auto PlatformPixelFormatBGR5A1Unorm           = MTL::PixelFormat::PixelFormatBGR5A1Unorm;
constexpr auto PlatformPixelFormatR32Uint               = MTL::PixelFormat::PixelFormatR32Uint;
constexpr auto PlatformPixelFormatR32Sint               = MTL::PixelFormat::PixelFormatR32Sint;
constexpr auto PlatformPixelFormatR32Float              = MTL::PixelFormat::PixelFormatR32Float;
constexpr auto PlatformPixelFormatRG16Unorm             = MTL::PixelFormat::PixelFormatRG16Unorm;
constexpr auto PlatformPixelFormatRG16Snorm             = MTL::PixelFormat::PixelFormatRG16Snorm;
constexpr auto PlatformPixelFormatRG16Uint              = MTL::PixelFormat::PixelFormatRG16Uint;
constexpr auto PlatformPixelFormatRG16Sint              = MTL::PixelFormat::PixelFormatRG16Sint;
constexpr auto PlatformPixelFormatRG16Float             = MTL::PixelFormat::PixelFormatRG16Float;
constexpr auto PlatformPixelFormatRGBA8Unorm            = MTL::PixelFormat::PixelFormatRGBA8Unorm;
constexpr auto PlatformPixelFormatRGBA8Unorm_sRGB       = MTL::PixelFormat::PixelFormatRGBA8Unorm_sRGB;
constexpr auto PlatformPixelFormatRGBA8Snorm            = MTL::PixelFormat::PixelFormatRGBA8Snorm;
constexpr auto PlatformPixelFormatRGBA8Uint             = MTL::PixelFormat::PixelFormatRGBA8Uint;
constexpr auto PlatformPixelFormatRGBA8Sint             = MTL::PixelFormat::PixelFormatRGBA8Sint;
constexpr auto PlatformPixelFormatBGRA8Unorm            = MTL::PixelFormat::PixelFormatBGRA8Unorm;
constexpr auto PlatformPixelFormatBGRA8Unorm_sRGB       = MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB;
constexpr auto PlatformPixelFormatRGB10A2Unorm          = MTL::PixelFormat::PixelFormatRGB10A2Unorm;
constexpr auto PlatformPixelFormatRGB10A2Uint           = MTL::PixelFormat::PixelFormatRGB10A2Uint;
constexpr auto PlatformPixelFormatRG11B10Float          = MTL::PixelFormat::PixelFormatRG11B10Float;
constexpr auto PlatformPixelFormatRGB9E5Float           = MTL::PixelFormat::PixelFormatRGB9E5Float;
constexpr auto PlatformPixelFormatBGR10A2Unorm          = MTL::PixelFormat::PixelFormatBGR10A2Unorm;
constexpr auto PlatformPixelFormatRG32Uint              = MTL::PixelFormat::PixelFormatRG32Uint;
constexpr auto PlatformPixelFormatRG32Sint              = MTL::PixelFormat::PixelFormatRG32Sint;
constexpr auto PlatformPixelFormatRG32Float             = MTL::PixelFormat::PixelFormatRG32Float;
constexpr auto PlatformPixelFormatRGBA16Unorm           = MTL::PixelFormat::PixelFormatRGBA16Unorm;
constexpr auto PlatformPixelFormatRGBA16Snorm           = MTL::PixelFormat::PixelFormatRGBA16Snorm;
constexpr auto PlatformPixelFormatRGBA16Uint            = MTL::PixelFormat::PixelFormatRGBA16Uint;
constexpr auto PlatformPixelFormatRGBA16Sint            = MTL::PixelFormat::PixelFormatRGBA16Sint;
constexpr auto PlatformPixelFormatRGBA16Float           = MTL::PixelFormat::PixelFormatRGBA16Float;
constexpr auto PlatformPixelFormatRGBA32Uint            = MTL::PixelFormat::PixelFormatRGBA32Uint;
constexpr auto PlatformPixelFormatRGBA32Sint            = MTL::PixelFormat::PixelFormatRGBA32Sint;
constexpr auto PlatformPixelFormatRGBA32Float           = MTL::PixelFormat::PixelFormatRGBA32Float;
constexpr auto PlatformPixelFormatBC1_RGBA              = MTL::PixelFormat::PixelFormatBC1_RGBA;
constexpr auto PlatformPixelFormatBC1_RGBA_sRGB         = MTL::PixelFormat::PixelFormatBC1_RGBA_sRGB;
constexpr auto PlatformPixelFormatBC2_RGBA              = MTL::PixelFormat::PixelFormatBC2_RGBA;
constexpr auto PlatformPixelFormatBC2_RGBA_sRGB         = MTL::PixelFormat::PixelFormatBC2_RGBA_sRGB;
constexpr auto PlatformPixelFormatBC3_RGBA              = MTL::PixelFormat::PixelFormatBC3_RGBA;
constexpr auto PlatformPixelFormatBC3_RGBA_sRGB         = MTL::PixelFormat::PixelFormatBC3_RGBA_sRGB;
constexpr auto PlatformPixelFormatBC4_RUnorm            = MTL::PixelFormat::PixelFormatBC4_RUnorm;
constexpr auto PlatformPixelFormatBC4_RSnorm            = MTL::PixelFormat::PixelFormatBC4_RSnorm;
constexpr auto PlatformPixelFormatBC5_RGUnorm           = MTL::PixelFormat::PixelFormatBC5_RGUnorm;
constexpr auto PlatformPixelFormatBC5_RGSnorm           = MTL::PixelFormat::PixelFormatBC5_RGSnorm;
constexpr auto PlatformPixelFormatBC6H_RGBFloat         = MTL::PixelFormat::PixelFormatBC6H_RGBFloat;
constexpr auto PlatformPixelFormatBC6H_RGBUfloat        = MTL::PixelFormat::PixelFormatBC6H_RGBUfloat;
constexpr auto PlatformPixelFormatBC7_RGBAUnorm         = MTL::PixelFormat::PixelFormatBC7_RGBAUnorm;
constexpr auto PlatformPixelFormatBC7_RGBAUnorm_sRGB    = MTL::PixelFormat::PixelFormatBC7_RGBAUnorm_sRGB;
constexpr auto PlatformPixelFormatPVRTC_RGB_2BPP        = MTL::PixelFormat::PixelFormatPVRTC_RGB_2BPP;
constexpr auto PlatformPixelFormatPVRTC_RGB_2BPP_sRGB   = MTL::PixelFormat::PixelFormatPVRTC_RGB_2BPP_sRGB;
constexpr auto PlatformPixelFormatPVRTC_RGB_4BPP        = MTL::PixelFormat::PixelFormatPVRTC_RGB_4BPP;
constexpr auto PlatformPixelFormatPVRTC_RGB_4BPP_sRGB   = MTL::PixelFormat::PixelFormatPVRTC_RGB_4BPP_sRGB;
constexpr auto PlatformPixelFormatPVRTC_RGBA_2BPP       = MTL::PixelFormat::PixelFormatPVRTC_RGBA_2BPP;
constexpr auto PlatformPixelFormatPVRTC_RGBA_2BPP_sRGB  = MTL::PixelFormat::PixelFormatPVRTC_RGBA_2BPP_sRGB;
constexpr auto PlatformPixelFormatPVRTC_RGBA_4BPP       = MTL::PixelFormat::PixelFormatPVRTC_RGBA_4BPP;
constexpr auto PlatformPixelFormatPVRTC_RGBA_4BPP_sRGB  = MTL::PixelFormat::PixelFormatPVRTC_RGBA_4BPP_sRGB;
constexpr auto PlatformPixelFormatEAC_R11Unorm          = MTL::PixelFormat::PixelFormatEAC_R11Unorm;
constexpr auto PlatformPixelFormatEAC_R11Snorm          = MTL::PixelFormat::PixelFormatEAC_R11Snorm;
constexpr auto PlatformPixelFormatEAC_RG11Unorm         = MTL::PixelFormat::PixelFormatEAC_RG11Unorm;
constexpr auto PlatformPixelFormatEAC_RG11Snorm         = MTL::PixelFormat::PixelFormatEAC_RG11Snorm;
constexpr auto PlatformPixelFormatEAC_RGBA8             = MTL::PixelFormat::PixelFormatEAC_RGBA8;
constexpr auto PlatformPixelFormatEAC_RGBA8_sRGB        = MTL::PixelFormat::PixelFormatEAC_RGBA8_sRGB;
constexpr auto PlatformPixelFormatETC2_RGB8             = MTL::PixelFormat::PixelFormatETC2_RGB8;
constexpr auto PlatformPixelFormatETC2_RGB8_sRGB        = MTL::PixelFormat::PixelFormatETC2_RGB8_sRGB;
constexpr auto PlatformPixelFormatETC2_RGB8A1           = MTL::PixelFormat::PixelFormatETC2_RGB8A1;
constexpr auto PlatformPixelFormatETC2_RGB8A1_sRGB      = MTL::PixelFormat::PixelFormatETC2_RGB8A1_sRGB;
constexpr auto PlatformPixelFormatASTC_4x4_sRGB         = MTL::PixelFormat::PixelFormatASTC_4x4_sRGB;
constexpr auto PlatformPixelFormatASTC_5x4_sRGB         = MTL::PixelFormat::PixelFormatASTC_5x4_sRGB;
constexpr auto PlatformPixelFormatASTC_5x5_sRGB         = MTL::PixelFormat::PixelFormatASTC_5x5_sRGB;
constexpr auto PlatformPixelFormatASTC_6x5_sRGB         = MTL::PixelFormat::PixelFormatASTC_6x5_sRGB;
constexpr auto PlatformPixelFormatASTC_6x6_sRGB         = MTL::PixelFormat::PixelFormatASTC_6x6_sRGB;
constexpr auto PlatformPixelFormatASTC_8x5_sRGB         = MTL::PixelFormat::PixelFormatASTC_8x5_sRGB;
constexpr auto PlatformPixelFormatASTC_8x6_sRGB         = MTL::PixelFormat::PixelFormatASTC_8x6_sRGB;
constexpr auto PlatformPixelFormatASTC_8x8_sRGB         = MTL::PixelFormat::PixelFormatASTC_8x8_sRGB;
constexpr auto PlatformPixelFormatASTC_10x5_sRGB        = MTL::PixelFormat::PixelFormatASTC_10x5_sRGB;
constexpr auto PlatformPixelFormatASTC_10x6_sRGB        = MTL::PixelFormat::PixelFormatASTC_10x6_sRGB;
constexpr auto PlatformPixelFormatASTC_10x8_sRGB        = MTL::PixelFormat::PixelFormatASTC_10x8_sRGB;
constexpr auto PlatformPixelFormatASTC_10x10_sRGB       = MTL::PixelFormat::PixelFormatASTC_10x10_sRGB;
constexpr auto PlatformPixelFormatASTC_12x10_sRGB       = MTL::PixelFormat::PixelFormatASTC_12x10_sRGB;
constexpr auto PlatformPixelFormatASTC_12x12_sRGB       = MTL::PixelFormat::PixelFormatASTC_12x12_sRGB;
constexpr auto PlatformPixelFormatASTC_4x4_LDR          = MTL::PixelFormat::PixelFormatASTC_4x4_LDR;
constexpr auto PlatformPixelFormatASTC_5x4_LDR          = MTL::PixelFormat::PixelFormatASTC_5x4_LDR;
constexpr auto PlatformPixelFormatASTC_5x5_LDR          = MTL::PixelFormat::PixelFormatASTC_5x5_LDR;
constexpr auto PlatformPixelFormatASTC_6x5_LDR          = MTL::PixelFormat::PixelFormatASTC_6x5_LDR;
constexpr auto PlatformPixelFormatASTC_6x6_LDR          = MTL::PixelFormat::PixelFormatASTC_6x6_LDR;
constexpr auto PlatformPixelFormatASTC_8x5_LDR          = MTL::PixelFormat::PixelFormatASTC_8x5_LDR;
constexpr auto PlatformPixelFormatASTC_8x6_LDR          = MTL::PixelFormat::PixelFormatASTC_8x6_LDR;
constexpr auto PlatformPixelFormatASTC_8x8_LDR          = MTL::PixelFormat::PixelFormatASTC_8x8_LDR;
constexpr auto PlatformPixelFormatASTC_10x5_LDR         = MTL::PixelFormat::PixelFormatASTC_10x5_LDR;
constexpr auto PlatformPixelFormatASTC_10x6_LDR         = MTL::PixelFormat::PixelFormatASTC_10x6_LDR;
constexpr auto PlatformPixelFormatASTC_10x8_LDR         = MTL::PixelFormat::PixelFormatASTC_10x8_LDR;
constexpr auto PlatformPixelFormatASTC_10x10_LDR        = MTL::PixelFormat::PixelFormatASTC_10x10_LDR;
constexpr auto PlatformPixelFormatASTC_12x10_LDR        = MTL::PixelFormat::PixelFormatASTC_12x10_LDR;
constexpr auto PlatformPixelFormatASTC_12x12_LDR        = MTL::PixelFormat::PixelFormatASTC_12x12_LDR;
constexpr auto PlatformPixelFormatASTC_4x4_HDR          = MTL::PixelFormat::PixelFormatASTC_4x4_HDR;
constexpr auto PlatformPixelFormatASTC_5x4_HDR          = MTL::PixelFormat::PixelFormatASTC_5x4_HDR;
constexpr auto PlatformPixelFormatASTC_5x5_HDR          = MTL::PixelFormat::PixelFormatASTC_5x5_HDR;
constexpr auto PlatformPixelFormatASTC_6x5_HDR          = MTL::PixelFormat::PixelFormatASTC_6x5_HDR;
constexpr auto PlatformPixelFormatASTC_6x6_HDR          = MTL::PixelFormat::PixelFormatASTC_6x6_HDR;
constexpr auto PlatformPixelFormatASTC_8x5_HDR          = MTL::PixelFormat::PixelFormatASTC_8x5_HDR;
constexpr auto PlatformPixelFormatASTC_8x6_HDR          = MTL::PixelFormat::PixelFormatASTC_8x6_HDR;
constexpr auto PlatformPixelFormatASTC_8x8_HDR          = MTL::PixelFormat::PixelFormatASTC_8x8_HDR;
constexpr auto PlatformPixelFormatASTC_10x5_HDR         = MTL::PixelFormat::PixelFormatASTC_10x5_HDR;
constexpr auto PlatformPixelFormatASTC_10x6_HDR         = MTL::PixelFormat::PixelFormatASTC_10x6_HDR;
constexpr auto PlatformPixelFormatASTC_10x8_HDR         = MTL::PixelFormat::PixelFormatASTC_10x8_HDR;
constexpr auto PlatformPixelFormatASTC_10x10_HDR        = MTL::PixelFormat::PixelFormatASTC_10x10_HDR;
constexpr auto PlatformPixelFormatASTC_12x10_HDR        = MTL::PixelFormat::PixelFormatASTC_12x10_HDR;
constexpr auto PlatformPixelFormatASTC_12x12_HDR        = MTL::PixelFormat::PixelFormatASTC_12x12_HDR;
constexpr auto PlatformPixelFormatGBGR422               = MTL::PixelFormat::PixelFormatGBGR422;
constexpr auto PlatformPixelFormatBGRG422               = MTL::PixelFormat::PixelFormatBGRG422;
constexpr auto PlatformPixelFormatDepth16Unorm          = MTL::PixelFormat::PixelFormatDepth16Unorm;
constexpr auto PlatformPixelFormatDepth32Float          = MTL::PixelFormat::PixelFormatDepth32Float;
constexpr auto PlatformPixelFormatStencil8              = MTL::PixelFormat::PixelFormatStencil8;
constexpr auto PlatformPixelFormatDepth24Unorm_Stencil8 = MTL::PixelFormat::PixelFormatDepth24Unorm_Stencil8;
constexpr auto PlatformPixelFormatDepth32Float_Stencil8 = MTL::PixelFormat::PixelFormatDepth32Float_Stencil8;
constexpr auto PlatformPixelFormatX32_Stencil8          = MTL::PixelFormat::PixelFormatX32_Stencil8;
constexpr auto PlatformPixelFormatX24_Stencil8          = MTL::PixelFormat::PixelFormatX24_Stencil8;
constexpr auto PlatformPixelFormatBGRA10_XR             = MTL::PixelFormat::PixelFormatBGRA10_XR;
constexpr auto PlatformPixelFormatBGRA10_XR_sRGB        = MTL::PixelFormat::PixelFormatBGRA10_XR_sRGB;
constexpr auto PlatformPixelFormatBGR10_XR              = MTL::PixelFormat::PixelFormatBGR10_XR;
constexpr auto PlatformPixelFormatBGR10_XR_sRGB         = MTL::PixelFormat::PixelFormatBGR10_XR_sRGB;

// Vertex formats
constexpr auto PlatformVertexFormatInvalid               = MTL::VertexFormat::VertexFormatInvalid;
constexpr auto PlatformVertexFormatUChar2                = MTL::VertexFormat::VertexFormatUChar2;
constexpr auto PlatformVertexFormatUChar3                = MTL::VertexFormat::VertexFormatUChar3;
constexpr auto PlatformVertexFormatUChar4                = MTL::VertexFormat::VertexFormatUChar4;
constexpr auto PlatformVertexFormatChar2                 = MTL::VertexFormat::VertexFormatChar2;
constexpr auto PlatformVertexFormatChar3                 = MTL::VertexFormat::VertexFormatChar3;
constexpr auto PlatformVertexFormatChar4                 = MTL::VertexFormat::VertexFormatChar4;
constexpr auto PlatformVertexFormatUChar2Normalized      = MTL::VertexFormat::VertexFormatUChar2Normalized;
constexpr auto PlatformVertexFormatUChar3Normalized      = MTL::VertexFormat::VertexFormatUChar3Normalized;
constexpr auto PlatformVertexFormatUChar4Normalized      = MTL::VertexFormat::VertexFormatUChar4Normalized;
constexpr auto PlatformVertexFormatChar2Normalized       = MTL::VertexFormat::VertexFormatChar2Normalized;
constexpr auto PlatformVertexFormatChar3Normalized       = MTL::VertexFormat::VertexFormatChar3Normalized;
constexpr auto PlatformVertexFormatChar4Normalized       = MTL::VertexFormat::VertexFormatChar4Normalized;
constexpr auto PlatformVertexFormatUShort2               = MTL::VertexFormat::VertexFormatUShort2;
constexpr auto PlatformVertexFormatUShort3               = MTL::VertexFormat::VertexFormatUShort3;
constexpr auto PlatformVertexFormatUShort4               = MTL::VertexFormat::VertexFormatUShort4;
constexpr auto PlatformVertexFormatShort2                = MTL::VertexFormat::VertexFormatShort2;
constexpr auto PlatformVertexFormatShort3                = MTL::VertexFormat::VertexFormatShort3;
constexpr auto PlatformVertexFormatShort4                = MTL::VertexFormat::VertexFormatShort4;
constexpr auto PlatformVertexFormatUShort2Normalized     = MTL::VertexFormat::VertexFormatUShort2Normalized;
constexpr auto PlatformVertexFormatUShort3Normalized     = MTL::VertexFormat::VertexFormatUShort3Normalized;
constexpr auto PlatformVertexFormatUShort4Normalized     = MTL::VertexFormat::VertexFormatUShort4Normalized;
constexpr auto PlatformVertexFormatShort2Normalized      = MTL::VertexFormat::VertexFormatShort2Normalized;
constexpr auto PlatformVertexFormatShort3Normalized      = MTL::VertexFormat::VertexFormatShort3Normalized;
constexpr auto PlatformVertexFormatShort4Normalized      = MTL::VertexFormat::VertexFormatShort4Normalized;
constexpr auto PlatformVertexFormatHalf2                 = MTL::VertexFormat::VertexFormatHalf2;
constexpr auto PlatformVertexFormatHalf3                 = MTL::VertexFormat::VertexFormatHalf3;
constexpr auto PlatformVertexFormatHalf4                 = MTL::VertexFormat::VertexFormatHalf4;
constexpr auto PlatformVertexFormatFloat                 = MTL::VertexFormat::VertexFormatFloat;
constexpr auto PlatformVertexFormatFloat2                = MTL::VertexFormat::VertexFormatFloat2;
constexpr auto PlatformVertexFormatFloat3                = MTL::VertexFormat::VertexFormatFloat3;
constexpr auto PlatformVertexFormatFloat4                = MTL::VertexFormat::VertexFormatFloat4;
constexpr auto PlatformVertexFormatInt                   = MTL::VertexFormat::VertexFormatInt;
constexpr auto PlatformVertexFormatInt2                  = MTL::VertexFormat::VertexFormatInt2;
constexpr auto PlatformVertexFormatInt3                  = MTL::VertexFormat::VertexFormatInt3;
constexpr auto PlatformVertexFormatInt4                  = MTL::VertexFormat::VertexFormatInt4;
constexpr auto PlatformVertexFormatUInt                  = MTL::VertexFormat::VertexFormatUInt;
constexpr auto PlatformVertexFormatUInt2                 = MTL::VertexFormat::VertexFormatUInt2;
constexpr auto PlatformVertexFormatUInt3                 = MTL::VertexFormat::VertexFormatUInt3;
constexpr auto PlatformVertexFormatUInt4                 = MTL::VertexFormat::VertexFormatUInt4;
constexpr auto PlatformVertexFormatInt1010102Normalized  = MTL::VertexFormat::VertexFormatInt1010102Normalized;
constexpr auto PlatformVertexFormatUInt1010102Normalized = MTL::VertexFormat::VertexFormatUInt1010102Normalized;
constexpr auto PlatformVertexFormatUChar4Normalized_BGRA = MTL::VertexFormat::VertexFormatUChar4Normalized_BGRA;
constexpr auto PlatformVertexFormatUChar                 = MTL::VertexFormat::VertexFormatUChar;
constexpr auto PlatformVertexFormatChar                  = MTL::VertexFormat::VertexFormatChar;
constexpr auto PlatformVertexFormatUCharNormalized       = MTL::VertexFormat::VertexFormatUCharNormalized;
constexpr auto PlatformVertexFormatCharNormalized        = MTL::VertexFormat::VertexFormatCharNormalized;
constexpr auto PlatformVertexFormatUShort                = MTL::VertexFormat::VertexFormatUShort;
constexpr auto PlatformVertexFormatShort                 = MTL::VertexFormat::VertexFormatShort;
constexpr auto PlatformVertexFormatUShortNormalized      = MTL::VertexFormat::VertexFormatUShortNormalized;
constexpr auto PlatformVertexFormatShortNormalized       = MTL::VertexFormat::VertexFormatShortNormalized;
constexpr auto PlatformVertexFormatHalf                  = MTL::VertexFormat::VertexFormatHalf;

constexpr auto PlatformTextureAddressModeClampToEdge        = MTL::SamplerAddressMode::SamplerAddressModeClampToEdge;
constexpr auto PlatformTextureAddressModeMirrorClampToEdge  = MTL::SamplerAddressMode::SamplerAddressModeMirrorClampToEdge;
constexpr auto PlatformTextureAddressModeRepeat             = MTL::SamplerAddressMode::SamplerAddressModeRepeat;
constexpr auto PlatformTextureAddressModeMirrorRepeat       = MTL::SamplerAddressMode::SamplerAddressModeMirrorRepeat;
constexpr auto PlatformTextureAddressModeClampToZero        = MTL::SamplerAddressMode::SamplerAddressModeClampToZero;
constexpr auto PlatformTextureAddressModeClampToBorderColor = MTL::SamplerAddressMode::SamplerAddressModeClampToBorderColor;

constexpr auto PlatformTextureBorderTransparent = MTL::SamplerBorderColorTransparentBlack;
constexpr auto PlatformTextureBorderOpaque      = MTL::SamplerBorderColorOpaqueBlack;
constexpr auto PlatformTextureBorderWhite       = MTL::SamplerBorderColorOpaqueWhite;

constexpr auto PlatformTextureFilterNearest = MTL::SamplerMinMagFilterNearest;
constexpr auto PlatformTextureFilterLinear  = MTL::SamplerMinMagFilterLinear;

constexpr auto PlatformTextureMipFilterNotMipmapped = MTL::SamplerMipFilterNotMipmapped;
constexpr auto PlatformTextureMipFilterNearest      = MTL::SamplerMipFilterNearest;
constexpr auto PlatformTextureMipFilterLinear       = MTL::SamplerMipFilterLinear;

constexpr auto PlatformStorageModeManaged    = MTL::ResourceStorageModeManaged;        // MTL::StorageModeManaged;
constexpr auto PlatformStorageModeShared     = MTL::ResourceStorageModeShared;
constexpr auto PlatformStorageModePrivate    = MTL::ResourceStorageModePrivate;
constexpr auto PlatformStorageModeMemoryLess = MTL::ResourceStorageModeMemoryless;

constexpr auto PlatformPrimitiveCullModeNone  = MTL::CullModeNone;
constexpr auto PlatformPrimitiveCullModeFront = MTL::CullModeFront;
constexpr auto PlatformPrimitiveCullModeBack  = MTL::CullModeBack;

constexpr auto PlatformPrimitiveWindingClockWise        = MTL::WindingClockwise;
constexpr auto PlatformPrimitiveWindingCounterClockWise = MTL::WindingCounterClockwise;

constexpr auto PlatformPrimitiveTopologyPoint         = MTL::PrimitiveTypePoint;
constexpr auto PlatformPrimitiveTopologyLine          = MTL::PrimitiveTypeLine;
constexpr auto PlatformPrimitiveTopologyLineStrip     = MTL::PrimitiveTypeLineStrip;
constexpr auto PlatformPrimitiveTopologyTriangle      = MTL::PrimitiveTypeTriangle;
constexpr auto PlatformPrimitiveTopologyTriangleStrip = MTL::PrimitiveTypeTriangleStrip;

}        // namespace rhi
