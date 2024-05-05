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

#if defined(USE_VOLK)
#	define VK_ENABLE_BETA_EXTENSIONS
#	define VK_NO_PROTOTYPES
#	include "volk/volk.h"
#else
#	include <vusym/vusym.hpp>
#endif

namespace rhi
{
// Pixel formats
constexpr auto get_max_vk_format(uint32_t a_plus)
{
	return VK_FORMAT_MAX_ENUM + a_plus;
}

constexpr auto PlatformPixelFormatInvalid               = VK_FORMAT_UNDEFINED;
constexpr auto PlatformPixelFormatA8Unorm               = get_max_vk_format(1);
constexpr auto PlatformPixelFormatR8Unorm               = VK_FORMAT_R8_UNORM;
constexpr auto PlatformPixelFormatR8Unorm_sRGB          = VK_FORMAT_R8_SRGB;
constexpr auto PlatformPixelFormatR8Snorm               = VK_FORMAT_R8_SNORM;
constexpr auto PlatformPixelFormatR8Uint                = VK_FORMAT_R8_UINT;
constexpr auto PlatformPixelFormatR8Sint                = VK_FORMAT_R8_SINT;
constexpr auto PlatformPixelFormatR16Unorm              = VK_FORMAT_R16_UNORM;
constexpr auto PlatformPixelFormatR16Snorm              = VK_FORMAT_R16_SNORM;
constexpr auto PlatformPixelFormatR16Uint               = VK_FORMAT_R16_UINT;
constexpr auto PlatformPixelFormatR16Sint               = VK_FORMAT_R16_SINT;
constexpr auto PlatformPixelFormatR16Float              = VK_FORMAT_R16_SFLOAT;
constexpr auto PlatformPixelFormatRG8Unorm              = VK_FORMAT_R8G8_UNORM;
constexpr auto PlatformPixelFormatRG8Unorm_sRGB         = VK_FORMAT_R8G8_SRGB;
constexpr auto PlatformPixelFormatRG8Snorm              = VK_FORMAT_R8G8_SNORM;
constexpr auto PlatformPixelFormatRG8Uint               = VK_FORMAT_R8G8_UINT;
constexpr auto PlatformPixelFormatRG8Sint               = VK_FORMAT_R8G8_SINT;
constexpr auto PlatformPixelFormatB5G6R5Unorm           = VK_FORMAT_B5G6R5_UNORM_PACK16;
constexpr auto PlatformPixelFormatA1BGR5Unorm           = VK_FORMAT_R5G5B5A1_UNORM_PACK16;        // Is this correct
constexpr auto PlatformPixelFormatABGR4Unorm            = VK_FORMAT_A4B4G4R4_UNORM_PACK16;
constexpr auto PlatformPixelFormatBGR5A1Unorm           = VK_FORMAT_B5G5R5A1_UNORM_PACK16;
constexpr auto PlatformPixelFormatR32Uint               = VK_FORMAT_R32_UINT;
constexpr auto PlatformPixelFormatR32Sint               = VK_FORMAT_R32_SINT;
constexpr auto PlatformPixelFormatR32Float              = VK_FORMAT_R32_SFLOAT;
constexpr auto PlatformPixelFormatRG16Unorm             = VK_FORMAT_R16G16_UNORM;
constexpr auto PlatformPixelFormatRG16Snorm             = VK_FORMAT_R16G16_SNORM;
constexpr auto PlatformPixelFormatRG16Uint              = VK_FORMAT_R16G16_UINT;
constexpr auto PlatformPixelFormatRG16Sint              = VK_FORMAT_R16G16_SINT;
constexpr auto PlatformPixelFormatRG16Float             = VK_FORMAT_R16G16_SFLOAT;
constexpr auto PlatformPixelFormatRGBA8Unorm            = VK_FORMAT_R8G8B8A8_UNORM;
constexpr auto PlatformPixelFormatRGBA8Unorm_sRGB       = VK_FORMAT_R8G8B8A8_SRGB;
constexpr auto PlatformPixelFormatRGBA8Snorm            = VK_FORMAT_R8G8B8A8_SNORM;
constexpr auto PlatformPixelFormatRGBA8Uint             = VK_FORMAT_R8G8B8A8_UINT;
constexpr auto PlatformPixelFormatRGBA8Sint             = VK_FORMAT_R8G8B8A8_SINT;
constexpr auto PlatformPixelFormatBGRA8Unorm            = VK_FORMAT_B8G8R8A8_UNORM;
constexpr auto PlatformPixelFormatBGRA8Unorm_sRGB       = VK_FORMAT_B8G8R8A8_SRGB;
constexpr auto PlatformPixelFormatRGB10A2Unorm          = VK_FORMAT_A2R10G10B10_UNORM_PACK32;        // Is this correct
constexpr auto PlatformPixelFormatRGB10A2Uint           = VK_FORMAT_A2B10G10R10_UINT_PACK32;         // Is this correct
constexpr auto PlatformPixelFormatRG11B10Float          = VK_FORMAT_B10G11R11_UFLOAT_PACK32;         // Flipped
constexpr auto PlatformPixelFormatRGB9E5Float           = VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;          // Flipped
constexpr auto PlatformPixelFormatBGR10A2Unorm          = VK_FORMAT_A2B10G10R10_UNORM_PACK32;
constexpr auto PlatformPixelFormatRG32Uint              = VK_FORMAT_R32G32_UINT;
constexpr auto PlatformPixelFormatRG32Sint              = VK_FORMAT_R32G32_SINT;
constexpr auto PlatformPixelFormatRG32Float             = VK_FORMAT_R32G32_SFLOAT;
constexpr auto PlatformPixelFormatRGBA16Unorm           = VK_FORMAT_R16G16B16A16_UNORM;
constexpr auto PlatformPixelFormatRGBA16Snorm           = VK_FORMAT_R16G16B16A16_SNORM;
constexpr auto PlatformPixelFormatRGBA16Uint            = VK_FORMAT_R16G16B16A16_UINT;
constexpr auto PlatformPixelFormatRGBA16Sint            = VK_FORMAT_R16G16B16A16_SINT;
constexpr auto PlatformPixelFormatRGBA16Float           = VK_FORMAT_R16G16B16A16_SFLOAT;
constexpr auto PlatformPixelFormatRGBA32Uint            = VK_FORMAT_R32G32B32A32_UINT;
constexpr auto PlatformPixelFormatRGBA32Sint            = VK_FORMAT_R32G32B32A32_SINT;
constexpr auto PlatformPixelFormatRGBA32Float           = VK_FORMAT_R32G32B32A32_SFLOAT;
constexpr auto PlatformPixelFormatBC1_RGBA              = VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
constexpr auto PlatformPixelFormatBC1_RGBA_sRGB         = VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
constexpr auto PlatformPixelFormatBC2_RGBA              = VK_FORMAT_BC2_UNORM_BLOCK;
constexpr auto PlatformPixelFormatBC2_RGBA_sRGB         = VK_FORMAT_BC2_SRGB_BLOCK;
constexpr auto PlatformPixelFormatBC3_RGBA              = VK_FORMAT_BC3_UNORM_BLOCK;
constexpr auto PlatformPixelFormatBC3_RGBA_sRGB         = VK_FORMAT_BC3_SRGB_BLOCK;
constexpr auto PlatformPixelFormatBC4_RUnorm            = VK_FORMAT_BC4_UNORM_BLOCK;
constexpr auto PlatformPixelFormatBC4_RSnorm            = VK_FORMAT_BC4_SNORM_BLOCK;
constexpr auto PlatformPixelFormatBC5_RGUnorm           = VK_FORMAT_BC5_UNORM_BLOCK;
constexpr auto PlatformPixelFormatBC5_RGSnorm           = VK_FORMAT_BC5_SNORM_BLOCK;        // Not sure about this one
constexpr auto PlatformPixelFormatBC6H_RGBFloat         = VK_FORMAT_BC6H_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatBC6H_RGBUfloat        = VK_FORMAT_BC6H_UFLOAT_BLOCK;
constexpr auto PlatformPixelFormatBC7_RGBAUnorm         = VK_FORMAT_BC7_UNORM_BLOCK;
constexpr auto PlatformPixelFormatBC7_RGBAUnorm_sRGB    = VK_FORMAT_BC7_SRGB_BLOCK;
constexpr auto PlatformPixelFormatPVRTC_RGB_2BPP        = VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG;
constexpr auto PlatformPixelFormatPVRTC_RGB_2BPP_sRGB   = VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG;
constexpr auto PlatformPixelFormatPVRTC_RGB_4BPP        = VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG;
constexpr auto PlatformPixelFormatPVRTC_RGB_4BPP_sRGB   = VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG;
constexpr auto PlatformPixelFormatPVRTC_RGBA_2BPP       = VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG;        // Are these PVRTC2/4 values correct
constexpr auto PlatformPixelFormatPVRTC_RGBA_2BPP_sRGB  = VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG;
constexpr auto PlatformPixelFormatPVRTC_RGBA_4BPP       = VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG;
constexpr auto PlatformPixelFormatPVRTC_RGBA_4BPP_sRGB  = VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG;
constexpr auto PlatformPixelFormatEAC_R11Unorm          = VK_FORMAT_EAC_R11_UNORM_BLOCK;
constexpr auto PlatformPixelFormatEAC_R11Snorm          = VK_FORMAT_EAC_R11_SNORM_BLOCK;
constexpr auto PlatformPixelFormatEAC_RG11Unorm         = VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
constexpr auto PlatformPixelFormatEAC_RG11Snorm         = VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
constexpr auto PlatformPixelFormatEAC_RGBA8             = get_max_vk_format(2);
constexpr auto PlatformPixelFormatEAC_RGBA8_sRGB        = get_max_vk_format(3);
constexpr auto PlatformPixelFormatETC2_RGB8             = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
constexpr auto PlatformPixelFormatETC2_RGB8_sRGB        = VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
constexpr auto PlatformPixelFormatETC2_RGB8A1           = VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
constexpr auto PlatformPixelFormatETC2_RGB8A1_sRGB      = VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_4x4_sRGB         = VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_5x4_sRGB         = VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_5x5_sRGB         = VK_FORMAT_ASTC_5x5_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_6x5_sRGB         = VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_6x6_sRGB         = VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_8x5_sRGB         = VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_8x6_sRGB         = VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_8x8_sRGB         = VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_10x5_sRGB        = VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_10x6_sRGB        = VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_10x8_sRGB        = VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_10x10_sRGB       = VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_12x10_sRGB       = VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_12x12_sRGB       = VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
constexpr auto PlatformPixelFormatASTC_4x4_LDR          = VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_5x4_LDR          = VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_5x5_LDR          = VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_6x5_LDR          = VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_6x6_LDR          = VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_8x5_LDR          = VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_8x6_LDR          = VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_8x8_LDR          = VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_10x5_LDR         = VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_10x6_LDR         = VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_10x8_LDR         = VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_10x10_LDR        = VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_12x10_LDR        = VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_12x12_LDR        = VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
constexpr auto PlatformPixelFormatASTC_4x4_HDR          = VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatASTC_5x4_HDR          = VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatASTC_5x5_HDR          = VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatASTC_6x5_HDR          = VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatASTC_6x6_HDR          = VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatASTC_8x5_HDR          = VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatASTC_8x6_HDR          = VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatASTC_8x8_HDR          = VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatASTC_10x5_HDR         = VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatASTC_10x6_HDR         = VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatASTC_10x8_HDR         = VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatASTC_10x10_HDR        = VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatASTC_12x10_HDR        = VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatASTC_12x12_HDR        = VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK;
constexpr auto PlatformPixelFormatGBGR422               = VK_FORMAT_G8B8G8R8_422_UNORM;
constexpr auto PlatformPixelFormatBGRG422               = VK_FORMAT_B8G8R8G8_422_UNORM;
constexpr auto PlatformPixelFormatDepth16Unorm          = VK_FORMAT_D16_UNORM;
constexpr auto PlatformPixelFormatDepth32Float          = VK_FORMAT_D32_SFLOAT;
constexpr auto PlatformPixelFormatStencil8              = VK_FORMAT_S8_UINT;
constexpr auto PlatformPixelFormatDepth24Unorm_Stencil8 = VK_FORMAT_D24_UNORM_S8_UINT;
constexpr auto PlatformPixelFormatDepth32Float_Stencil8 = VK_FORMAT_D32_SFLOAT_S8_UINT;
constexpr auto PlatformPixelFormatX32_Stencil8          = get_max_vk_format(4);
constexpr auto PlatformPixelFormatX24_Stencil8          = get_max_vk_format(5);
constexpr auto PlatformPixelFormatBGRA10_XR             = VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16;
constexpr auto PlatformPixelFormatBGRA10_XR_sRGB        = get_max_vk_format(6);
constexpr auto PlatformPixelFormatBGR10_XR              = get_max_vk_format(7);
constexpr auto PlatformPixelFormatBGR10_XR_sRGB         = get_max_vk_format(8);

// Vertex formats
constexpr auto PlatformVertexFormatInvalid               = VK_FORMAT_UNDEFINED;
constexpr auto PlatformVertexFormatUChar2                = VK_FORMAT_R8G8_UINT;
constexpr auto PlatformVertexFormatUChar3                = VK_FORMAT_R8G8B8_UINT;
constexpr auto PlatformVertexFormatUChar4                = VK_FORMAT_R8G8B8A8_UINT;
constexpr auto PlatformVertexFormatChar2                 = VK_FORMAT_R8G8_SINT;
constexpr auto PlatformVertexFormatChar3                 = VK_FORMAT_R8G8B8_SINT;
constexpr auto PlatformVertexFormatChar4                 = VK_FORMAT_R8G8B8A8_SINT;
constexpr auto PlatformVertexFormatUChar2Normalized      = VK_FORMAT_R8G8_UNORM;
constexpr auto PlatformVertexFormatUChar3Normalized      = VK_FORMAT_R8G8B8_UNORM;
constexpr auto PlatformVertexFormatUChar4Normalized      = VK_FORMAT_R8G8B8A8_UNORM;
constexpr auto PlatformVertexFormatChar2Normalized       = VK_FORMAT_R8G8_SNORM;
constexpr auto PlatformVertexFormatChar3Normalized       = VK_FORMAT_R8G8B8_SNORM;
constexpr auto PlatformVertexFormatChar4Normalized       = VK_FORMAT_R8G8B8A8_SNORM;
constexpr auto PlatformVertexFormatUShort2               = VK_FORMAT_R16G16_UINT;
constexpr auto PlatformVertexFormatUShort3               = VK_FORMAT_R16G16B16_UINT;
constexpr auto PlatformVertexFormatUShort4               = VK_FORMAT_R16G16B16A16_UINT;
constexpr auto PlatformVertexFormatShort2                = VK_FORMAT_R16G16_SINT;
constexpr auto PlatformVertexFormatShort3                = VK_FORMAT_R16G16B16_SINT;
constexpr auto PlatformVertexFormatShort4                = VK_FORMAT_R16G16B16A16_SINT;
constexpr auto PlatformVertexFormatUShort2Normalized     = VK_FORMAT_R16G16_UNORM;
constexpr auto PlatformVertexFormatUShort3Normalized     = VK_FORMAT_R16G16B16_UNORM;
constexpr auto PlatformVertexFormatUShort4Normalized     = VK_FORMAT_R16G16B16A16_UNORM;
constexpr auto PlatformVertexFormatShort2Normalized      = VK_FORMAT_R16G16_SNORM;
constexpr auto PlatformVertexFormatShort3Normalized      = VK_FORMAT_R16G16B16_SNORM;
constexpr auto PlatformVertexFormatShort4Normalized      = VK_FORMAT_R16G16B16A16_SNORM;
constexpr auto PlatformVertexFormatHalf2                 = VK_FORMAT_R16G16_SFLOAT;
constexpr auto PlatformVertexFormatHalf3                 = VK_FORMAT_R16G16B16_SFLOAT;
constexpr auto PlatformVertexFormatHalf4                 = VK_FORMAT_R16G16B16A16_SFLOAT;
constexpr auto PlatformVertexFormatFloat                 = VK_FORMAT_R32_SFLOAT;
constexpr auto PlatformVertexFormatFloat2                = VK_FORMAT_R32G32_SFLOAT;
constexpr auto PlatformVertexFormatFloat3                = VK_FORMAT_R32G32B32_SFLOAT;
constexpr auto PlatformVertexFormatFloat4                = VK_FORMAT_R32G32B32A32_SFLOAT;
constexpr auto PlatformVertexFormatInt                   = VK_FORMAT_R32_SINT;
constexpr auto PlatformVertexFormatInt2                  = VK_FORMAT_R32G32_SINT;
constexpr auto PlatformVertexFormatInt3                  = VK_FORMAT_R32G32B32_SINT;
constexpr auto PlatformVertexFormatInt4                  = VK_FORMAT_R32G32B32A32_SINT;
constexpr auto PlatformVertexFormatUInt                  = VK_FORMAT_R32_UINT;
constexpr auto PlatformVertexFormatUInt2                 = VK_FORMAT_R32G32_UINT;
constexpr auto PlatformVertexFormatUInt3                 = VK_FORMAT_R32G32B32_UINT;
constexpr auto PlatformVertexFormatUInt4                 = VK_FORMAT_R32G32B32A32_UINT;
constexpr auto PlatformVertexFormatInt1010102Normalized  = VK_FORMAT_A2R10G10B10_SNORM_PACK32;
constexpr auto PlatformVertexFormatUInt1010102Normalized = VK_FORMAT_A2R10G10B10_UNORM_PACK32;
constexpr auto PlatformVertexFormatUChar4Normalized_BGRA = VK_FORMAT_B4G4R4A4_UNORM_PACK16;
constexpr auto PlatformVertexFormatUChar                 = VK_FORMAT_R8_UINT;
constexpr auto PlatformVertexFormatChar                  = VK_FORMAT_R8_SINT;
constexpr auto PlatformVertexFormatUCharNormalized       = VK_FORMAT_R8_UNORM;
constexpr auto PlatformVertexFormatCharNormalized        = VK_FORMAT_R8_SNORM;
constexpr auto PlatformVertexFormatUShort                = VK_FORMAT_R16_UINT;
constexpr auto PlatformVertexFormatShort                 = VK_FORMAT_R16_SINT;
constexpr auto PlatformVertexFormatUShortNormalized      = VK_FORMAT_R16_UNORM;
constexpr auto PlatformVertexFormatShortNormalized       = VK_FORMAT_R16_SNORM;
constexpr auto PlatformVertexFormatHalf                  = VK_FORMAT_R16_SFLOAT;
constexpr auto PlatformVertexFormatUnsupported           = VK_FORMAT_MAX_ENUM;

constexpr auto PlatformTextureTargetTexture_1D            = VK_IMAGE_VIEW_TYPE_1D;
constexpr auto PlatformTextureTargetTexture_2D            = VK_IMAGE_VIEW_TYPE_2D;
constexpr auto PlatformTextureTargetTexture_3D            = VK_IMAGE_VIEW_TYPE_3D;
constexpr auto PlatformTextureTargetTexture_cube          = VK_IMAGE_VIEW_TYPE_CUBE;
constexpr auto PlatformTextureTargetTexture_2D_MS         = VK_IMAGE_VIEW_TYPE_MAX_ENUM;
constexpr auto PlatformTextureTargetTexture_2D_MS_array   = VK_IMAGE_VIEW_TYPE_MAX_ENUM;
constexpr auto PlatformTextureTargetTexture_1D_array      = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
constexpr auto PlatformTextureTargetTexture_2D_array      = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
constexpr auto PlatformTextureTargetTexture_1D_cube_array = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;

constexpr auto PlatformTextureAddressModeClampToEdge        = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
constexpr auto PlatformTextureAddressModeMirrorClampToEdge  = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
constexpr auto PlatformTextureAddressModeRepeat             = VK_SAMPLER_ADDRESS_MODE_REPEAT;
constexpr auto PlatformTextureAddressModeMirrorRepeat       = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
constexpr auto PlatformTextureAddressModeClampToZero        = VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;        // No VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_ZERO in Vulkan
constexpr auto PlatformTextureAddressModeClampToBorderColor = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;

// NOTE: Border modes are very different from what Vulkan provides
// typedef enum VkBorderColor
// {
// 	VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK = 0,
// 	VK_BORDER_COLOR_INT_TRANSPARENT_BLACK   = 1,
// 	VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK      = 2,
// 	VK_BORDER_COLOR_INT_OPAQUE_BLACK        = 3,
// 	VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE      = 4,
// 	VK_BORDER_COLOR_INT_OPAQUE_WHITE        = 5,
// 	VK_BORDER_COLOR_FLOAT_CUSTOM_EXT        = 1000287003,
// 	VK_BORDER_COLOR_INT_CUSTOM_EXT          = 1000287004,
// 	VK_BORDER_COLOR_MAX_ENUM                = 0x7FFFFFFF
// } VkBorderColor;

constexpr auto PlatformTextureBorderTransparent = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
constexpr auto PlatformTextureBorderOpaque      = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
constexpr auto PlatformTextureBorderWhite       = VK_BORDER_COLOR_INT_OPAQUE_WHITE;

// The following are not supported
// VK_FILTER_CUBIC_EXT = 1000015000,
constexpr auto PlatformTextureFilterNearest = VK_FILTER_NEAREST;
constexpr auto PlatformTextureFilterLinear  = VK_FILTER_LINEAR;

constexpr auto PlatformTextureMipFilterNearest      = VK_SAMPLER_MIPMAP_MODE_NEAREST;
constexpr auto PlatformTextureMipFilterLinear       = VK_SAMPLER_MIPMAP_MODE_LINEAR;
constexpr auto PlatformTextureMipFilterNotMipmapped = VK_SAMPLER_MIPMAP_MODE_MAX_ENUM;

constexpr auto PlatformStorageModeManaged    = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
constexpr auto PlatformStorageModeShared     = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
constexpr auto PlatformStorageModePrivate    = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
constexpr auto PlatformStorageModeMemoryLess = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;

constexpr auto PlatformPrimitiveCullModeNone      = VK_CULL_MODE_NONE;
constexpr auto PlatformPrimitiveCullModeFront     = VK_CULL_MODE_FRONT_BIT;
constexpr auto PlatformPrimitiveCullModeBack      = VK_CULL_MODE_BACK_BIT;
constexpr auto PlatformPrimitiveCullModeFrontBack = VK_CULL_MODE_FRONT_AND_BACK;

constexpr auto PlatformCompareFunctionNever        = VK_COMPARE_OP_NEVER;
constexpr auto PlatformCompareFunctionLess         = VK_COMPARE_OP_LESS;
constexpr auto PlatformCompareFunctionEqual        = VK_COMPARE_OP_EQUAL;
constexpr auto PlatformCompareFunctionLessEqual    = VK_COMPARE_OP_LESS_OR_EQUAL;
constexpr auto PlatformCompareFunctionGreater      = VK_COMPARE_OP_GREATER;
constexpr auto PlatformCompareFunctionNotEqual     = VK_COMPARE_OP_NOT_EQUAL;
constexpr auto PlatformCompareFunctionGreaterEqual = VK_COMPARE_OP_GREATER_OR_EQUAL;
constexpr auto PlatformCompareFunctionAlways       = VK_COMPARE_OP_ALWAYS;

constexpr auto PlatformPrimitiveWindingClockWise        = VK_FRONT_FACE_CLOCKWISE;
constexpr auto PlatformPrimitiveWindingCounterClockWise = VK_FRONT_FACE_COUNTER_CLOCKWISE;

// The following are not supported
// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN                  = 5,
// VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY      = 6,
// VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY     = 7,
// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY  = 8,
// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY = 9,
// VK_PRIMITIVE_TOPOLOGY_PATCH_LIST                    = 10,

constexpr auto PlatformPrimitiveTopologyPoint         = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
constexpr auto PlatformPrimitiveTopologyLine          = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
constexpr auto PlatformPrimitiveTopologyLineStrip     = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
constexpr auto PlatformPrimitiveTopologyTriangle      = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
constexpr auto PlatformPrimitiveTopologyTriangleStrip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

// There is no concept of PrimitiveTopologyClass in Vulkan so these are default
constexpr auto PlatformPrimitiveTopologyClassPoint    = 0;
constexpr auto PlatformPrimitiveTopologyClassLine     = 1;
constexpr auto PlatformPrimitiveTopologyClassTriangle = 2;

// The following are not supported
// VK_POLYGON_MODE_POINT = 2,
// VK_POLYGON_MODE_FILL_RECTANGLE_NV = 1000153000,
constexpr auto PlatformTriangleFillModeFill = VK_POLYGON_MODE_FILL;
constexpr auto PlatformTriangleFillModeLine = VK_POLYGON_MODE_LINE;

}        // namespace rhi
