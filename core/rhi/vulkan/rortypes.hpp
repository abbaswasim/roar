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

namespace rhi
{
// TODO: Change the following to Vulkan format enum values
// Pixel formats
constexpr auto PlatformPixelFormatInvalid               = 0;
constexpr auto PlatformPixelFormatA8Unorm               = 1;
constexpr auto PlatformPixelFormatR8Unorm               = 2;
constexpr auto PlatformPixelFormatR8Unorm_sRGB          = 3;
constexpr auto PlatformPixelFormatR8Snorm               = 4;
constexpr auto PlatformPixelFormatR8Uint                = 5;
constexpr auto PlatformPixelFormatR8Sint                = 6;
constexpr auto PlatformPixelFormatR16Unorm              = 7;
constexpr auto PlatformPixelFormatR16Snorm              = 8;
constexpr auto PlatformPixelFormatR16Uint               = 9;
constexpr auto PlatformPixelFormatR16Sint               = 10;
constexpr auto PlatformPixelFormatR16Float              = 11;
constexpr auto PlatformPixelFormatRG8Unorm              = 12;
constexpr auto PlatformPixelFormatRG8Unorm_sRGB         = 13;
constexpr auto PlatformPixelFormatRG8Snorm              = 14;
constexpr auto PlatformPixelFormatRG8Uint               = 15;
constexpr auto PlatformPixelFormatRG8Sint               = 16;
constexpr auto PlatformPixelFormatB5G6R5Unorm           = 17;
constexpr auto PlatformPixelFormatA1BGR5Unorm           = 18;
constexpr auto PlatformPixelFormatABGR4Unorm            = 19;
constexpr auto PlatformPixelFormatBGR5A1Unorm           = 20;
constexpr auto PlatformPixelFormatR32Uint               = 21;
constexpr auto PlatformPixelFormatR32Sint               = 22;
constexpr auto PlatformPixelFormatR32Float              = 23;
constexpr auto PlatformPixelFormatRG16Unorm             = 24;
constexpr auto PlatformPixelFormatRG16Snorm             = 25;
constexpr auto PlatformPixelFormatRG16Uint              = 26;
constexpr auto PlatformPixelFormatRG16Sint              = 27;
constexpr auto PlatformPixelFormatRG16Float             = 28;
constexpr auto PlatformPixelFormatRGBA8Unorm            = 29;
constexpr auto PlatformPixelFormatRGBA8Unorm_sRGB       = 30;
constexpr auto PlatformPixelFormatRGBA8Snorm            = 31;
constexpr auto PlatformPixelFormatRGBA8Uint             = 32;
constexpr auto PlatformPixelFormatRGBA8Sint             = 33;
constexpr auto PlatformPixelFormatBGRA8Unorm            = 34;
constexpr auto PlatformPixelFormatBGRA8Unorm_sRGB       = 35;
constexpr auto PlatformPixelFormatRGB10A2Unorm          = 36;
constexpr auto PlatformPixelFormatRGB10A2Uint           = 37;
constexpr auto PlatformPixelFormatRG11B10Float          = 38;
constexpr auto PlatformPixelFormatRGB9E5Float           = 39;
constexpr auto PlatformPixelFormatBGR10A2Unorm          = 40;
constexpr auto PlatformPixelFormatRG32Uint              = 41;
constexpr auto PlatformPixelFormatRG32Sint              = 42;
constexpr auto PlatformPixelFormatRG32Float             = 43;
constexpr auto PlatformPixelFormatRGBA16Unorm           = 44;
constexpr auto PlatformPixelFormatRGBA16Snorm           = 45;
constexpr auto PlatformPixelFormatRGBA16Uint            = 46;
constexpr auto PlatformPixelFormatRGBA16Sint            = 47;
constexpr auto PlatformPixelFormatRGBA16Float           = 48;
constexpr auto PlatformPixelFormatRGBA32Uint            = 49;
constexpr auto PlatformPixelFormatRGBA32Sint            = 50;
constexpr auto PlatformPixelFormatRGBA32Float           = 51;
constexpr auto PlatformPixelFormatBC1_RGBA              = 52;
constexpr auto PlatformPixelFormatBC1_RGBA_sRGB         = 53;
constexpr auto PlatformPixelFormatBC2_RGBA              = 54;
constexpr auto PlatformPixelFormatBC2_RGBA_sRGB         = 55;
constexpr auto PlatformPixelFormatBC3_RGBA              = 56;
constexpr auto PlatformPixelFormatBC3_RGBA_sRGB         = 57;
constexpr auto PlatformPixelFormatBC4_RUnorm            = 58;
constexpr auto PlatformPixelFormatBC4_RSnorm            = 59;
constexpr auto PlatformPixelFormatBC5_RGUnorm           = 60;
constexpr auto PlatformPixelFormatBC5_RGSnorm           = 61;
constexpr auto PlatformPixelFormatBC6H_RGBFloat         = 62;
constexpr auto PlatformPixelFormatBC6H_RGBUfloat        = 63;
constexpr auto PlatformPixelFormatBC7_RGBAUnorm         = 64;
constexpr auto PlatformPixelFormatBC7_RGBAUnorm_sRGB    = 65;
constexpr auto PlatformPixelFormatPVRTC_RGB_2BPP        = 66;
constexpr auto PlatformPixelFormatPVRTC_RGB_2BPP_sRGB   = 67;
constexpr auto PlatformPixelFormatPVRTC_RGB_4BPP        = 68;
constexpr auto PlatformPixelFormatPVRTC_RGB_4BPP_sRGB   = 69;
constexpr auto PlatformPixelFormatPVRTC_RGBA_2BPP       = 70;
constexpr auto PlatformPixelFormatPVRTC_RGBA_2BPP_sRGB  = 71;
constexpr auto PlatformPixelFormatPVRTC_RGBA_4BPP       = 72;
constexpr auto PlatformPixelFormatPVRTC_RGBA_4BPP_sRGB  = 73;
constexpr auto PlatformPixelFormatEAC_R11Unorm          = 74;
constexpr auto PlatformPixelFormatEAC_R11Snorm          = 75;
constexpr auto PlatformPixelFormatEAC_RG11Unorm         = 76;
constexpr auto PlatformPixelFormatEAC_RG11Snorm         = 77;
constexpr auto PlatformPixelFormatEAC_RGBA8             = 78;
constexpr auto PlatformPixelFormatEAC_RGBA8_sRGB        = 79;
constexpr auto PlatformPixelFormatETC2_RGB8             = 80;
constexpr auto PlatformPixelFormatETC2_RGB8_sRGB        = 81;
constexpr auto PlatformPixelFormatETC2_RGB8A1           = 82;
constexpr auto PlatformPixelFormatETC2_RGB8A1_sRGB      = 83;
constexpr auto PlatformPixelFormatASTC_4x4_sRGB         = 84;
constexpr auto PlatformPixelFormatASTC_5x4_sRGB         = 85;
constexpr auto PlatformPixelFormatASTC_5x5_sRGB         = 86;
constexpr auto PlatformPixelFormatASTC_6x5_sRGB         = 87;
constexpr auto PlatformPixelFormatASTC_6x6_sRGB         = 88;
constexpr auto PlatformPixelFormatASTC_8x5_sRGB         = 89;
constexpr auto PlatformPixelFormatASTC_8x6_sRGB         = 90;
constexpr auto PlatformPixelFormatASTC_8x8_sRGB         = 91;
constexpr auto PlatformPixelFormatASTC_10x5_sRGB        = 92;
constexpr auto PlatformPixelFormatASTC_10x6_sRGB        = 93;
constexpr auto PlatformPixelFormatASTC_10x8_sRGB        = 94;
constexpr auto PlatformPixelFormatASTC_10x10_sRGB       = 95;
constexpr auto PlatformPixelFormatASTC_12x10_sRGB       = 96;
constexpr auto PlatformPixelFormatASTC_12x12_sRGB       = 97;
constexpr auto PlatformPixelFormatASTC_4x4_LDR          = 98;
constexpr auto PlatformPixelFormatASTC_5x4_LDR          = 99;
constexpr auto PlatformPixelFormatASTC_5x5_LDR          = 100;
constexpr auto PlatformPixelFormatASTC_6x5_LDR          = 101;
constexpr auto PlatformPixelFormatASTC_6x6_LDR          = 102;
constexpr auto PlatformPixelFormatASTC_8x5_LDR          = 103;
constexpr auto PlatformPixelFormatASTC_8x6_LDR          = 104;
constexpr auto PlatformPixelFormatASTC_8x8_LDR          = 105;
constexpr auto PlatformPixelFormatASTC_10x5_LDR         = 106;
constexpr auto PlatformPixelFormatASTC_10x6_LDR         = 107;
constexpr auto PlatformPixelFormatASTC_10x8_LDR         = 108;
constexpr auto PlatformPixelFormatASTC_10x10_LDR        = 109;
constexpr auto PlatformPixelFormatASTC_12x10_LDR        = 110;
constexpr auto PlatformPixelFormatASTC_12x12_LDR        = 111;
constexpr auto PlatformPixelFormatASTC_4x4_HDR          = 112;
constexpr auto PlatformPixelFormatASTC_5x4_HDR          = 113;
constexpr auto PlatformPixelFormatASTC_5x5_HDR          = 114;
constexpr auto PlatformPixelFormatASTC_6x5_HDR          = 115;
constexpr auto PlatformPixelFormatASTC_6x6_HDR          = 116;
constexpr auto PlatformPixelFormatASTC_8x5_HDR          = 117;
constexpr auto PlatformPixelFormatASTC_8x6_HDR          = 118;
constexpr auto PlatformPixelFormatASTC_8x8_HDR          = 119;
constexpr auto PlatformPixelFormatASTC_10x5_HDR         = 120;
constexpr auto PlatformPixelFormatASTC_10x6_HDR         = 121;
constexpr auto PlatformPixelFormatASTC_10x8_HDR         = 122;
constexpr auto PlatformPixelFormatASTC_10x10_HDR        = 123;
constexpr auto PlatformPixelFormatASTC_12x10_HDR        = 124;
constexpr auto PlatformPixelFormatASTC_12x12_HDR        = 125;
constexpr auto PlatformPixelFormatGBGR422               = 126;
constexpr auto PlatformPixelFormatBGRG422               = 127;
constexpr auto PlatformPixelFormatDepth16Unorm          = 128;
constexpr auto PlatformPixelFormatDepth32Float          = 129;
constexpr auto PlatformPixelFormatStencil8              = 130;
constexpr auto PlatformPixelFormatDepth24Unorm_Stencil8 = 131;
constexpr auto PlatformPixelFormatDepth32Float_Stencil8 = 132;
constexpr auto PlatformPixelFormatX32_Stencil8          = 133;
constexpr auto PlatformPixelFormatX24_Stencil8          = 134;
constexpr auto PlatformPixelFormatBGRA10_XR             = 135;
constexpr auto PlatformPixelFormatBGRA10_XR_sRGB        = 136;
constexpr auto PlatformPixelFormatBGR10_XR              = 137;
constexpr auto PlatformPixelFormatBGR10_XR_sRGB         = 138;

// Vertex formats
constexpr auto PlatformVertexFormatInvalid               = 0;
constexpr auto PlatformVertexFormatUChar2                = 1;
constexpr auto PlatformVertexFormatUChar3                = 2;
constexpr auto PlatformVertexFormatUChar4                = 3;
constexpr auto PlatformVertexFormatChar2                 = 4;
constexpr auto PlatformVertexFormatChar3                 = 5;
constexpr auto PlatformVertexFormatChar4                 = 6;
constexpr auto PlatformVertexFormatUChar2Normalized      = 7;
constexpr auto PlatformVertexFormatUChar3Normalized      = 8;
constexpr auto PlatformVertexFormatUChar4Normalized      = 9;
constexpr auto PlatformVertexFormatChar2Normalized       = 10;
constexpr auto PlatformVertexFormatChar3Normalized       = 11;
constexpr auto PlatformVertexFormatChar4Normalized       = 12;
constexpr auto PlatformVertexFormatUShort2               = 13;
constexpr auto PlatformVertexFormatUShort3               = 14;
constexpr auto PlatformVertexFormatUShort4               = 15;
constexpr auto PlatformVertexFormatShort2                = 16;
constexpr auto PlatformVertexFormatShort3                = 17;
constexpr auto PlatformVertexFormatShort4                = 18;
constexpr auto PlatformVertexFormatUShort2Normalized     = 19;
constexpr auto PlatformVertexFormatUShort3Normalized     = 20;
constexpr auto PlatformVertexFormatUShort4Normalized     = 21;
constexpr auto PlatformVertexFormatShort2Normalized      = 22;
constexpr auto PlatformVertexFormatShort3Normalized      = 23;
constexpr auto PlatformVertexFormatShort4Normalized      = 24;
constexpr auto PlatformVertexFormatHalf2                 = 25;
constexpr auto PlatformVertexFormatHalf3                 = 26;
constexpr auto PlatformVertexFormatHalf4                 = 27;
constexpr auto PlatformVertexFormatFloat                 = 28;
constexpr auto PlatformVertexFormatFloat2                = 29;
constexpr auto PlatformVertexFormatFloat3                = 30;
constexpr auto PlatformVertexFormatFloat4                = 31;
constexpr auto PlatformVertexFormatInt                   = 32;
constexpr auto PlatformVertexFormatInt2                  = 33;
constexpr auto PlatformVertexFormatInt3                  = 34;
constexpr auto PlatformVertexFormatInt4                  = 35;
constexpr auto PlatformVertexFormatUInt                  = 36;
constexpr auto PlatformVertexFormatUInt2                 = 37;
constexpr auto PlatformVertexFormatUInt3                 = 38;
constexpr auto PlatformVertexFormatUInt4                 = 39;
constexpr auto PlatformVertexFormatInt1010102Normalized  = 40;
constexpr auto PlatformVertexFormatUInt1010102Normalized = 41;
constexpr auto PlatformVertexFormatUChar4Normalized_BGRA = 42;
constexpr auto PlatformVertexFormatUChar                 = 43;
constexpr auto PlatformVertexFormatChar                  = 44;
constexpr auto PlatformVertexFormatUCharNormalized       = 45;
constexpr auto PlatformVertexFormatCharNormalized        = 46;
constexpr auto PlatformVertexFormatUShort                = 47;
constexpr auto PlatformVertexFormatShort                 = 48;
constexpr auto PlatformVertexFormatUShortNormalized      = 49;
constexpr auto PlatformVertexFormatShortNormalized       = 50;
constexpr auto PlatformVertexFormatHalf                  = 51;

constexpr auto PlatformTextureAddressModeClampToEdge        = 0;
constexpr auto PlatformTextureAddressModeMirrorClampToEdge  = 1;
constexpr auto PlatformTextureAddressModeRepeat             = 2;
constexpr auto PlatformTextureAddressModeMirrorRepeat       = 3;
constexpr auto PlatformTextureAddressModeClampToZero        = 4;
constexpr auto PlatformTextureAddressModeClampToBorderColor = 5;

constexpr auto PlatformTextureBorderTransparent = 0;
constexpr auto PlatformTextureBorderOpaque      = 1;
constexpr auto PlatformTextureBorderWhite       = 2;

constexpr auto PlatformTextureFilterNearest = 0;
constexpr auto PlatformTextureFilterLinear  = 1;

constexpr auto PlatformTextureMipFilterNotMipmapped = 0;
constexpr auto PlatformTextureMipFilterNearest      = 1;
constexpr auto PlatformTextureMipFilterLinear       = 2;
}        // namespace rhi
