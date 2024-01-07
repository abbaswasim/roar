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

#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "roar.hpp"
#include <cassert>

#if defined(ROR_RENDER_TYPE_VULKAN)
#	include "rhi/vulkan/rortypes.hpp"
#elif defined(ROR_RENDER_TYPE_METAL)
#	include "rhi/metal/rortypes.hpp"
#else
#	error "Unsupported buffer platform"
#endif

namespace rhi
{
enum class PixelFormat : uint32_t
{
	invalid                       = PlatformPixelFormatInvalid,
	a8_uint8_norm                 = PlatformPixelFormatA8Unorm,
	r8_uint8_norm                 = PlatformPixelFormatR8Unorm,
	r8_uint8_norm_srgb            = PlatformPixelFormatR8Unorm_sRGB,
	r8_int8_norm                  = PlatformPixelFormatR8Snorm,
	r8_uint8                      = PlatformPixelFormatR8Uint,
	r8_int8                       = PlatformPixelFormatR8Sint,
	r16_uint16_norm               = PlatformPixelFormatR16Unorm,
	r16_int16_norm                = PlatformPixelFormatR16Snorm,
	r16_uint16                    = PlatformPixelFormatR16Uint,
	r16_int16                     = PlatformPixelFormatR16Sint,
	r16_float16                   = PlatformPixelFormatR16Float,
	r8g8_uint16_norm              = PlatformPixelFormatRG8Unorm,
	r8g8_uint16_norm_srgb         = PlatformPixelFormatRG8Unorm_sRGB,
	r8g8_int16_norm               = PlatformPixelFormatRG8Snorm,
	r8g8_uint16                   = PlatformPixelFormatRG8Uint,
	r8g8_int16                    = PlatformPixelFormatRG8Sint,
	b5g6r5_uint16_norm            = PlatformPixelFormatB5G6R5Unorm,
	a1b5g5r5_uint16_norm          = PlatformPixelFormatA1BGR5Unorm,
	a4b4g4r4_uint16_norm          = PlatformPixelFormatABGR4Unorm,
	b5g5r5a1_uint16_norm          = PlatformPixelFormatBGR5A1Unorm,
	r32_uint32                    = PlatformPixelFormatR32Uint,
	r32_int32                     = PlatformPixelFormatR32Sint,
	r32_float32                   = PlatformPixelFormatR32Float,
	r16g16_uint32_norm            = PlatformPixelFormatRG16Unorm,
	r16g16_int32_norm             = PlatformPixelFormatRG16Snorm,
	r16g16_uint32                 = PlatformPixelFormatRG16Uint,
	r16g16_int32                  = PlatformPixelFormatRG16Sint,
	r16g16_float32                = PlatformPixelFormatRG16Float,
	r8g8b8a8_uint32_norm          = PlatformPixelFormatRGBA8Unorm,
	r8g8b8a8_uint32_norm_srgb     = PlatformPixelFormatRGBA8Unorm_sRGB,
	r8g8b8a8_int32_norm           = PlatformPixelFormatRGBA8Snorm,
	r8g8b8a8_uint32               = PlatformPixelFormatRGBA8Uint,
	r8g8b8a8_int32                = PlatformPixelFormatRGBA8Sint,
	b8g8r8a8_uint32_norm          = PlatformPixelFormatBGRA8Unorm,
	b8g8r8a8_uint32_norm_srgb     = PlatformPixelFormatBGRA8Unorm_sRGB,
	r10g10b10a2_uint32_norm       = PlatformPixelFormatRGB10A2Unorm,
	r10g10b10a2_uint32            = PlatformPixelFormatRGB10A2Uint,
	r11g11b10_float32             = PlatformPixelFormatRG11B10Float,
	r9g9b9e5_float32              = PlatformPixelFormatRGB9E5Float,
	b10g10r10a2_uint32_norm       = PlatformPixelFormatBGR10A2Unorm,
	r32g32_uint64                 = PlatformPixelFormatRG32Uint,
	r32g32_int64                  = PlatformPixelFormatRG32Sint,
	r32g32_float64                = PlatformPixelFormatRG32Float,
	r16g16b16a16_uint64_norm      = PlatformPixelFormatRGBA16Unorm,
	r16g16b16a16_int64_norm       = PlatformPixelFormatRGBA16Snorm,
	r16g16b16a16_uint64           = PlatformPixelFormatRGBA16Uint,
	r16g16b16a16_int64            = PlatformPixelFormatRGBA16Sint,
	r16g16b16a16_float64          = PlatformPixelFormatRGBA16Float,
	r32g32b32a32_uint128          = PlatformPixelFormatRGBA32Uint,
	r32g32b32a32_int128           = PlatformPixelFormatRGBA32Sint,
	r32g32b32a32_float128         = PlatformPixelFormatRGBA32Float,
	bc1_rgba                      = PlatformPixelFormatBC1_RGBA,
	bc1_rgba_srgb                 = PlatformPixelFormatBC1_RGBA_sRGB,
	bc2_rgba                      = PlatformPixelFormatBC2_RGBA,
	bc2_rgba_srgb                 = PlatformPixelFormatBC2_RGBA_sRGB,
	bc3_rgba                      = PlatformPixelFormatBC3_RGBA,
	bc3_rgba_srgb                 = PlatformPixelFormatBC3_RGBA_sRGB,
	bc4_r_unsigned_norm           = PlatformPixelFormatBC4_RUnorm,
	bc4_r_norm                    = PlatformPixelFormatBC4_RSnorm,
	bc5_rg_unsigned_norm          = PlatformPixelFormatBC5_RGUnorm,
	bc5_rg_norm                   = PlatformPixelFormatBC5_RGSnorm,
	bc6h_rgb_float                = PlatformPixelFormatBC6H_RGBFloat,
	bc6h_rgbu_float               = PlatformPixelFormatBC6H_RGBUfloat,
	bc7_rgba_unsigned_norm        = PlatformPixelFormatBC7_RGBAUnorm,
	bc7_rgba_unsinged_norm_srgb   = PlatformPixelFormatBC7_RGBAUnorm_sRGB,
	pvrtc_rgb_2bpp                = PlatformPixelFormatPVRTC_RGB_2BPP,
	pvrtc_rgb_2bpp_srgb           = PlatformPixelFormatPVRTC_RGB_2BPP_sRGB,
	pvrtc_rgb_4bpp                = PlatformPixelFormatPVRTC_RGB_4BPP,
	pvrtc_rgb_4bpp_srgb           = PlatformPixelFormatPVRTC_RGB_4BPP_sRGB,
	pvrtc_rgba_2bpp               = PlatformPixelFormatPVRTC_RGBA_2BPP,
	pvrtc_rgba_2bpp_srgb          = PlatformPixelFormatPVRTC_RGBA_2BPP_sRGB,
	pvrtc_rgba_4bpp               = PlatformPixelFormatPVRTC_RGBA_4BPP,
	pvrtc_rgba_4bpp_srgb          = PlatformPixelFormatPVRTC_RGBA_4BPP_sRGB,
	eac_r11_unsigned_norm         = PlatformPixelFormatEAC_R11Unorm,
	eac_r11_norm                  = PlatformPixelFormatEAC_R11Snorm,
	eac_r11g11_unsigned_norm      = PlatformPixelFormatEAC_RG11Unorm,
	eac_r11g11_norm               = PlatformPixelFormatEAC_RG11Snorm,
	eac_r8g8b8a8                  = PlatformPixelFormatEAC_RGBA8,
	eac_r8g8b8a8_srgb             = PlatformPixelFormatEAC_RGBA8_sRGB,
	etc2_r8g8b8                   = PlatformPixelFormatETC2_RGB8,
	etc2_r8g8b8_srgb              = PlatformPixelFormatETC2_RGB8_sRGB,
	etc2_r8g8b8a1                 = PlatformPixelFormatETC2_RGB8A1,
	etc2_r8g8b8a1_srgb            = PlatformPixelFormatETC2_RGB8A1_sRGB,
	astc_4x4_srgb                 = PlatformPixelFormatASTC_4x4_sRGB,
	astc_5x4_srgb                 = PlatformPixelFormatASTC_5x4_sRGB,
	astc_5x5_srgb                 = PlatformPixelFormatASTC_5x5_sRGB,
	astc_6x5_srgb                 = PlatformPixelFormatASTC_6x5_sRGB,
	astc_6x6_srgb                 = PlatformPixelFormatASTC_6x6_sRGB,
	astc_8x5_srgb                 = PlatformPixelFormatASTC_8x5_sRGB,
	astc_8x6_srgb                 = PlatformPixelFormatASTC_8x6_sRGB,
	astc_8x8_srgb                 = PlatformPixelFormatASTC_8x8_sRGB,
	astc_10x5_srgb                = PlatformPixelFormatASTC_10x5_sRGB,
	astc_10x6_srgb                = PlatformPixelFormatASTC_10x6_sRGB,
	astc_10x8_srgb                = PlatformPixelFormatASTC_10x8_sRGB,
	astc_10x10_srgb               = PlatformPixelFormatASTC_10x10_sRGB,
	astc_12x10_srgb               = PlatformPixelFormatASTC_12x10_sRGB,
	astc_12x12_srgb               = PlatformPixelFormatASTC_12x12_sRGB,
	astc_4x4_ldr                  = PlatformPixelFormatASTC_4x4_LDR,
	astc_5x4_ldr                  = PlatformPixelFormatASTC_5x4_LDR,
	astc_5x5_ldr                  = PlatformPixelFormatASTC_5x5_LDR,
	astc_6x5_ldr                  = PlatformPixelFormatASTC_6x5_LDR,
	astc_6x6_ldr                  = PlatformPixelFormatASTC_6x6_LDR,
	astc_8x5_ldr                  = PlatformPixelFormatASTC_8x5_LDR,
	astc_8x6_ldr                  = PlatformPixelFormatASTC_8x6_LDR,
	astc_8x8_ldr                  = PlatformPixelFormatASTC_8x8_LDR,
	astc_10x5_ldr                 = PlatformPixelFormatASTC_10x5_LDR,
	astc_10x6_ldr                 = PlatformPixelFormatASTC_10x6_LDR,
	astc_10x8_ldr                 = PlatformPixelFormatASTC_10x8_LDR,
	astc_10x10_ldr                = PlatformPixelFormatASTC_10x10_LDR,
	astc_12x10_ldr                = PlatformPixelFormatASTC_12x10_LDR,
	astc_12x12_ldr                = PlatformPixelFormatASTC_12x12_LDR,
	astc_4x4_hdr                  = PlatformPixelFormatASTC_4x4_HDR,
	astc_5x4_hdr                  = PlatformPixelFormatASTC_5x4_HDR,
	astc_5x5_hdr                  = PlatformPixelFormatASTC_5x5_HDR,
	astc_6x5_hdr                  = PlatformPixelFormatASTC_6x5_HDR,
	astc_6x6_hdr                  = PlatformPixelFormatASTC_6x6_HDR,
	astc_8x5_hdr                  = PlatformPixelFormatASTC_8x5_HDR,
	astc_8x6_hdr                  = PlatformPixelFormatASTC_8x6_HDR,
	astc_8x8_hdr                  = PlatformPixelFormatASTC_8x8_HDR,
	astc_10x5_hdr                 = PlatformPixelFormatASTC_10x5_HDR,
	astc_10x6_hdr                 = PlatformPixelFormatASTC_10x6_HDR,
	astc_10x8_hdr                 = PlatformPixelFormatASTC_10x8_HDR,
	astc_10x10_hdr                = PlatformPixelFormatASTC_10x10_HDR,
	astc_12x10_hdr                = PlatformPixelFormatASTC_12x10_HDR,
	astc_12x12_hdr                = PlatformPixelFormatASTC_12x12_HDR,
	gbgr422                       = PlatformPixelFormatGBGR422,
	bgrg422                       = PlatformPixelFormatBGRG422,
	depth16_uint16_norm           = PlatformPixelFormatDepth16Unorm,
	depth32_float32               = PlatformPixelFormatDepth32Float,
	stencil8_int8                 = PlatformPixelFormatStencil8,
	depth24_norm_stencil8_uint32  = PlatformPixelFormatDepth24Unorm_Stencil8,
	depth32_float32_stencil8_int8 = PlatformPixelFormatDepth32Float_Stencil8,
	x32_stencil8                  = PlatformPixelFormatX32_Stencil8,
	x24_stencil8                  = PlatformPixelFormatX24_Stencil8,
	b10g10r10a10_xr               = PlatformPixelFormatBGRA10_XR,
	b10g10r10a10_xr_srgb          = PlatformPixelFormatBGRA10_XR_sRGB,
	b10g10r10_xr                  = PlatformPixelFormatBGR10_XR,
	b10g10r10_xr_srgb             = PlatformPixelFormatBGR10_XR_sRGB
};

enum class VertexFormat : uint32_t
{
	invalid = PlatformVertexFormatInvalid,

	int8_1 = PlatformVertexFormatChar,
	int8_2 = PlatformVertexFormatChar2,
	int8_3 = PlatformVertexFormatChar3,
	int8_4 = PlatformVertexFormatChar4,

	int16_1 = PlatformVertexFormatShort,
	int16_2 = PlatformVertexFormatShort2,
	int16_3 = PlatformVertexFormatShort3,
	int16_4 = PlatformVertexFormatShort4,

	int32_1 = PlatformVertexFormatInt,
	int32_2 = PlatformVertexFormatInt2,
	int32_3 = PlatformVertexFormatInt3,
	int32_4 = PlatformVertexFormatInt4,

	half16_1 = PlatformVertexFormatHalf,
	half16_2 = PlatformVertexFormatHalf2,
	half16_3 = PlatformVertexFormatHalf3,
	half16_4 = PlatformVertexFormatHalf4,

	float32_1 = PlatformVertexFormatFloat,
	float32_2 = PlatformVertexFormatFloat2,
	float32_3 = PlatformVertexFormatFloat3,
	float32_4 = PlatformVertexFormatFloat4,

	uint8_1 = PlatformVertexFormatUChar,
	uint8_2 = PlatformVertexFormatUChar2,
	uint8_3 = PlatformVertexFormatUChar3,
	uint8_4 = PlatformVertexFormatUChar4,

	uint16_1 = PlatformVertexFormatUShort,
	uint16_2 = PlatformVertexFormatUShort2,
	uint16_3 = PlatformVertexFormatUShort3,
	uint16_4 = PlatformVertexFormatUShort4,

	uint32_1 = PlatformVertexFormatUInt,
	uint32_2 = PlatformVertexFormatUInt2,
	uint32_3 = PlatformVertexFormatUInt3,
	uint32_4 = PlatformVertexFormatUInt4,

	int8_1_norm = PlatformVertexFormatCharNormalized,
	int8_2_norm = PlatformVertexFormatChar2Normalized,
	int8_3_norm = PlatformVertexFormatChar3Normalized,
	int8_4_norm = PlatformVertexFormatChar4Normalized,

	int16_1_norm = PlatformVertexFormatShortNormalized,
	int16_2_norm = PlatformVertexFormatShort2Normalized,
	int16_3_norm = PlatformVertexFormatShort3Normalized,
	int16_4_norm = PlatformVertexFormatShort4Normalized,

	uint8_1_norm = PlatformVertexFormatUCharNormalized,
	uint8_2_norm = PlatformVertexFormatUChar2Normalized,
	uint8_3_norm = PlatformVertexFormatUChar3Normalized,
	uint8_4_norm = PlatformVertexFormatUChar4Normalized,

	uint16_1_norm = PlatformVertexFormatUShortNormalized,
	uint16_2_norm = PlatformVertexFormatUShort2Normalized,
	uint16_3_norm = PlatformVertexFormatUShort3Normalized,
	uint16_4_norm = PlatformVertexFormatUShort4Normalized,

	int1010102_norm   = PlatformVertexFormatInt1010102Normalized,
	uint1010102_norm  = PlatformVertexFormatUInt1010102Normalized,
	uint8_4_norm_bgra = PlatformVertexFormatUChar4Normalized_BGRA,

	bool32_1 = PlatformVertexFormatUnsupported + 1u,
	bool32_2,
	bool32_3,
	bool32_4,

	uint64_1,
	uint64_2,
	uint64_3,
	uint64_4,

	float64_1,
	float64_2,
	float64_3,
	float64_4,

	float32_2x2,
	float32_2x3,
	float32_2x4,

	float32_3x2,
	float32_3x3,
	float32_3x4,

	float32_4x2,
	float32_4x3,
	float32_4x4,

	float64_2x2,
	float64_2x3,
	float64_2x4,

	float64_3x2,
	float64_3x3,
	float64_3x4,

	float64_4x2,
	float64_4x3,
	float64_4x4,

	uint8_custom,
	uint16_custom,
	uint32_custom,
	float32_custom,

	int101111_norm,
	uint101111_norm,

	struct_1,
	struct_0
};

// Generic name when semantics are not clear
using Format = VertexFormat;

/*
  The following types are not used widely so not supported
  details https://github.com/KhronosGroup/glTF/issues/832
  int32_1_norm,
  int32_2_norm,
  int32_3_norm,
  int32_4_norm,
  uint32_1_norm,
  uint32_2_norm,
  uint32_3_norm,
  uint32_4_norm,
*/

// Defines what's in the buffer
enum class BufferPackSemantic
{
	positions,
	non_positions,
	indices,
	instances
};

enum class StepFunction
{
	constant,
	vertex,
	instance,
	patch,
	patch_control_point
};

enum class ResourceStorageOption
{
	shared      = PlatformStorageModeShared,           //  The resource is stored in system memory and is accessible to both the CPU and the GPU
	managed     = PlatformStorageModeManaged,          // The CPU and GPU may maintain separate copies of the resource, which you need to explicitly synchronize.
	exclusive   = PlatformStorageModePrivate,          // The resource can be accessed only by the GPU.
	memory_less = PlatformStorageModeMemoryLess        // The resource’s contents can be accessed only by the GPU and only exist temporarily during a render pass.
};

// If new types are required make sure to change in rormodel.hpp
enum class PrimitiveTopology
{
	points          = PlatformPrimitiveTopologyPoint,
	lines           = PlatformPrimitiveTopologyLine,
	lines_strip     = PlatformPrimitiveTopologyLineStrip,
	triangles       = PlatformPrimitiveTopologyTriangle,
	triangles_strip = PlatformPrimitiveTopologyTriangleStrip
};

enum class PrimitiveTopologyClass
{
	point    = PlatformPrimitiveTopologyClassPoint,
	line     = PlatformPrimitiveTopologyClassLine,
	triangle = PlatformPrimitiveTopologyClassTriangle
};

enum class TriangleFillMode
{
	fill  = PlatformTriangleFillModeFill,
	lines = PlatformTriangleFillModeLine
};

enum class PrimitiveWinding
{
	clockwise         = PlatformPrimitiveWindingClockWise,
	counter_clockwise = PlatformPrimitiveWindingCounterClockWise
};

enum class PrimitiveCullMode
{
	none       = PlatformPrimitiveCullModeNone,
	front      = PlatformPrimitiveCullModeFront,
	back       = PlatformPrimitiveCullModeBack,
	front_back = PlatformPrimitiveCullModeFrontBack,
};

enum class FormatType
{
	format_unorm,
	format_norm,
	format_float
};

enum class DepthCompareFunction
{
	never         = PlatformCompareFunctionNever,
	less          = PlatformCompareFunctionLess,
	equal         = PlatformCompareFunctionEqual,
	less_equal    = PlatformCompareFunctionLessEqual,
	greater       = PlatformCompareFunctionGreater,
	not_equal     = PlatformCompareFunctionNotEqual,
	greater_equal = PlatformCompareFunctionGreaterEqual,
	always        = PlatformCompareFunctionAlways
};

enum class TextureFilter
{
	nearest = PlatformTextureFilterNearest,
	linear  = PlatformTextureFilterLinear
};

enum class TextureMipFilter
{
	not_mipmapped = PlatformTextureMipFilterNotMipmapped,
	nearest       = PlatformTextureMipFilterNearest,
	linear        = PlatformTextureMipFilterLinear
};

enum class TextureMipGenMode
{
	manual,
	automatic
};

enum class TextureTransfer
{
	linear,
	srgb
};

enum class TextureAddressMode
{
	clamp_to_edge         = PlatformTextureAddressModeClampToEdge,
	mirror_clamp_to_edge  = PlatformTextureAddressModeMirrorClampToEdge,
	repeat                = PlatformTextureAddressModeRepeat,
	mirror_repeat         = PlatformTextureAddressModeMirrorRepeat,
	clamp_to_zero         = PlatformTextureAddressModeClampToZero,
	clamp_to_border_color = PlatformTextureAddressModeClampToBorderColor
};

enum class TextureBorder
{
	transparent = PlatformTextureBorderTransparent,
	opaque      = PlatformTextureBorderOpaque,
	white       = PlatformTextureBorderWhite
};

enum class TextureTarget
{
	texture_1D          = PlatformTextureTargetTexture_1D,
	texture_2D          = PlatformTextureTargetTexture_2D,
	texture_3D          = PlatformTextureTargetTexture_3D,
	texture_cube        = PlatformTextureTargetTexture_cube,
	texture_2D_MS       = PlatformTextureTargetTexture_2D_MS,
	texture_2D_MS_array = PlatformTextureTargetTexture_2D_MS_array,
	texture_1D_array    = PlatformTextureTargetTexture_1D_array,
	texture_2D_array    = PlatformTextureTargetTexture_2D_array,
	texture_cube_array  = PlatformTextureTargetTexture_1D_cube_array
};

enum class TextureUsage
{
	unknown,
	shader_read,              // Read only textures that are sampled in shaders
	shader_write,             // Implies a read somewhere later, otherwise what is the point of writing it
	render_target,            // Not same as write, and you don't need to combine write and render target for render targets, metal likes it this way
	render_target_read        // A readable (sample) render target, some drivers might be able to optimise write only render targets
};

enum class BlendMode
{
	opaque,
	mask,
	blend
};

enum class BlendFactor
{
	zero,
	one,
	source_color,
	one_minus_source_color,
	source_alpha,
	one_minus_source_alpha,
	destination_color,
	one_minus_destination_color,
	destination_alpha,
	one_minus_destination_alpha,
	source_alpha_saturated,
	blend_color,
	one_minus_blend_color,
	blend_alpha,
	one_minus_blend_alpha,
	source1_color,
	one_minus_source1_color,
	source1_alpha,
	one_minus_source1_alpha,
};

enum class BlendOperation
{
	add,
	subtract,
	reverse_subtract,
	min,
	max,
};

enum class MaterialModel
{
	lit,
	eye,
	hair,
	cloth,
	water,
	unlit,
	// twosided,           // This is handled in the material itself
	clearcoat,
	subsurface
};

enum class ShadingMode
{
	forward,                   // Classic forward (GeomLight)
	deferred,                  // Classic deferred (Geom + Light)
	deferred_tiled,            // Tiles contains depth bounded light lists (Geom + LightCull + Light) uses Tile shaders for persistent tile light lists
	forward_tiled,             // Tiles contains depth bounded light lists (Depth + LightCull + GeomLight) uses Tile shaders for persistent tile light lists
	forward_clustered,         // Tiles contains clustered bounded light lists (ClusterLightCull + Light)
	deferred_clustered,        // Tiles contains clustered bounded light lists (Geom + ClusterLightCull + Light)
	visibility_buffer,         // Save visibility data, prim_id + Barycentric (Geom + GeomReconstruct,MeterialShade,Light)
	gpu_driven                 // Uses argument buffers and indirect command buffers, for sparse ICB use indirect range buffers
};

enum class RenderpassType
{
	lut,
	main,
	depth,
	shadow,
	light_bin,
	reflection,
	refraction,
	pre_process,
	post_process,
	tone_mapping,
	forward_light,
	node_transform,
	deferred_gbuffer,
	reflection_probes,
	image_based_light,
	ambient_occlusion,
	skeletal_transform,
	deferred_clustered,
	image_based_light_lut,
	max
};

enum class RenderpassTechnique
{
	fragment,
	compute
};

enum class RenderpassState
{
	transient,
	persistent
};

enum class ShaderType
{
	none,
	mesh,
	task,
	tile,
	vertex,
	compute,
	fragment,
	ray_miss,
	ray_any_hit,
	ray_closest_hit,
	ray_intersection,
	ray_generation
};

enum class ShaderStage
{

	none                    = 1 << 0,
	mesh                    = 1 << 1,
	task                    = 1 << 2,
	tile                    = 1 << 3,
	vertex                  = 1 << 4,
	compute                 = 1 << 5,
	fragment                = 1 << 6,
	ray_miss                = 1 << 7,
	ray_any_hit             = 1 << 8,
	ray_closest_hit         = 1 << 9,
	ray_intersection        = 1 << 10,
	ray_generation          = 1 << 11,
	vertex_fragment         = ShaderStage::vertex | ShaderStage::fragment,
	compute_vertex          = ShaderStage::compute | ShaderStage::vertex,
	compute_fragment        = ShaderStage::compute | ShaderStage::fragment,
	compute_vertex_fragment = ShaderStage::compute | ShaderStage::vertex | ShaderStage::fragment
};

enum class LoadAction
{
	load,
	clear,
	dont_care
};

enum class StoreAction
{
	store,
	discard,
	dont_care
};

// enum class ShaderSemantic : uint32_t
// {
//	vertex_position        = 1 << 0,
//	vertex_texture_coord_0 = 1 << 1,
//	vertex_texture_coord_1 = 1 << 2,
//	vertex_texture_coord_2 = 1 << 3,
//	vertex_normal          = 1 << 4,
//	vertex_tangent         = 1 << 5,
//	vertex_color           = 1 << 6,
//	vertex_bone_id         = 1 << 7,
//	vertex_weight          = 1 << 8,
//	instance_translation   = 1 << 9,         // Usually x,y,z translation
//	instance_rotation      = 1 << 10,        // Usually a quaternion
//	instance_scale         = 1 << 11,        // Usually a non-uniform scale in x, y and z axis
//	instance_transform     = 1 << 12,        // Usually a matrix4
//	vertex_index           = 1 << 13,
//	mesh_index             = 1 << 14,
//	meshlet_data           = 1 << 15,
//	drawcall_data          = 1 << 16,
//	...
//  ...
// };

// NOTE: If any more vertex attribute types are added, change the max attribute binding/location in settings.json
// All this gross dance is required to not have to define the enum values and strings twice, basically defines the one above
// Position is usually x,y,z translation
// Rotation is usually a quaternion
// Scale is usually a non-uniform scale in x, y and z axis
// Transformation is usually a matrix4
// clang-format off
#define describe_buffer_semantics(item)                                 \
	item(vertex_position)                 item_value(= 0),              \
	item(vertex_texture_coord_0)          item_value(= 1ul << 0),       \
	item(vertex_texture_coord_1)          item_value(= 1ul << 1),       \
	item(vertex_texture_coord_2)          item_value(= 1ul << 2),       \
	item(vertex_normal)                   item_value(= 1ul << 3),       \
	item(vertex_bent_normal)              item_value(= 1ul << 4),       \
	item(vertex_tangent)                  item_value(= 1ul << 5),       \
	item(vertex_color_0)                  item_value(= 1ul << 6),       \
	item(vertex_color_1)                  item_value(= 1ul << 7),       \
	item(vertex_bone_id_0)                item_value(= 1ul << 8),       \
	item(vertex_bone_id_1)                item_value(= 1ul << 9),       \
	item(vertex_weight_0)                 item_value(= 1ul << 10),      \
	item(vertex_weight_1)                 item_value(= 1ul << 11),      \
	item(vertex_morph_target)             item_value(= 1ul << 12),      \
	item(vertex_morph_weight)             item_value(= 1ul << 13),      \
	item(vertex_index)                    item_value(= 1ul << 14),      \
	item(instance_translation)            item_value(= 1ul << 15),      \
	item(instance_rotation)               item_value(= 1ul << 16),      \
	item(instance_scale)                  item_value(= 1ul << 17),      \
	item(instance_transform)              item_value(= 1ul << 18),      \
	item(mesh_index)                      item_value(= 1ul << 19),      \
	item(meshlet_data)                    item_value(= 1ul << 20),      \
	item(drawcall_data)                   item_value(= 1ul << 21),      \
	item(texture_image_data)              item_value(= 1ul << 22),      \
	item(texture_sampler_data)            item_value(= 1ul << 23),      \
	item(texture_data)                    item_value(= 1ul << 24),      \
	item(material_data)                   item_value(= 1ul << 25),      \
	item(mesh_data)                       item_value(= 1ul << 26),      \
	item(skin_data)                       item_value(= 1ul << 27),      \
	item(node_data)                       item_value(= 1ul << 28),      \
	item(node_suplementory_data)          item_value(= 1ul << 29),      \
	item(animation_sampler_data)          item_value(= 1ul << 30),      \
	item(animation_channel_data)          item_value(= 1ul << 31),      \
	item(animation_input_data)            item_value(= 1ul << 32),      \
	item(animation_output_data)           item_value(= 1ul << 33),      \
	item(animation_data)                  item_value(= 1ul << 34),      \
	item(bounding_box_data)               item_value(= 1ul << 35),      \
	item(index_data)                      item_value(= 1ul << 36),      \
	item(joint_index_data)                item_value(= 1ul << 37),      \
	item(instance_trs)                    item_value(= 1ul << 38),      \
	item(gltf_bin_buffer)                 item_value(= 1ul << 39),      \
	item(custom)                          item_value(= 1ul << 40)
// clang-format on
#define item(_enum) _enum
#define item_value(_enum) _enum

enum class BufferSemantic : uint64_t
{
	describe_buffer_semantics(item)
};

#undef item
#undef item_value

#define item(_enum) _enum
#define item_value(_enum)

enum class AttributeIndex : uint32_t
{
	describe_buffer_semantics(item)
};

#undef item
#undef item_value

constexpr uint32_t pixel_format_to_bits(PixelFormat a_pixel_format)
{
	switch (a_pixel_format)
	{
		case PixelFormat::invalid:
		case PixelFormat::a8_uint8_norm:
		case PixelFormat::r8_uint8_norm:
		case PixelFormat::r8_uint8_norm_srgb:
		case PixelFormat::r8_int8_norm:
		case PixelFormat::r8_uint8:
		case PixelFormat::r8_int8:
		case PixelFormat::stencil8_int8:
			return 8;
		case PixelFormat::depth16_uint16_norm:
		case PixelFormat::r16_uint16_norm:
		case PixelFormat::r16_int16_norm:
		case PixelFormat::r16_uint16:
		case PixelFormat::r16_int16:
		case PixelFormat::r16_float16:
		case PixelFormat::r8g8_uint16_norm:
		case PixelFormat::r8g8_uint16_norm_srgb:
		case PixelFormat::r8g8_int16_norm:
		case PixelFormat::r8g8_uint16:
		case PixelFormat::r8g8_int16:
		case PixelFormat::b5g6r5_uint16_norm:
		case PixelFormat::a1b5g5r5_uint16_norm:
		case PixelFormat::a4b4g4r4_uint16_norm:
		case PixelFormat::b5g5r5a1_uint16_norm:
			return 16;
		case PixelFormat::x24_stencil8:
		case PixelFormat::depth32_float32:
		case PixelFormat::depth24_norm_stencil8_uint32:
		case PixelFormat::r32_uint32:
		case PixelFormat::r32_int32:
		case PixelFormat::r32_float32:
		case PixelFormat::r16g16_uint32_norm:
		case PixelFormat::r16g16_int32_norm:
		case PixelFormat::r16g16_uint32:
		case PixelFormat::r16g16_int32:
		case PixelFormat::r16g16_float32:
		case PixelFormat::r8g8b8a8_uint32_norm:
		case PixelFormat::r8g8b8a8_uint32_norm_srgb:
		case PixelFormat::r8g8b8a8_int32_norm:
		case PixelFormat::r8g8b8a8_uint32:
		case PixelFormat::r8g8b8a8_int32:
		case PixelFormat::b8g8r8a8_uint32_norm:
		case PixelFormat::b8g8r8a8_uint32_norm_srgb:
		case PixelFormat::r10g10b10a2_uint32_norm:
		case PixelFormat::r10g10b10a2_uint32:
		case PixelFormat::r11g11b10_float32:
		case PixelFormat::r9g9b9e5_float32:
		case PixelFormat::b10g10r10a2_uint32_norm:
			return 32;
		case PixelFormat::r32g32_uint64:
		case PixelFormat::r32g32_int64:
		case PixelFormat::r32g32_float64:
		case PixelFormat::r16g16b16a16_uint64_norm:
		case PixelFormat::r16g16b16a16_int64_norm:
		case PixelFormat::r16g16b16a16_uint64:
		case PixelFormat::r16g16b16a16_int64:
		case PixelFormat::r16g16b16a16_float64:
			return 64;
		case PixelFormat::r32g32b32a32_uint128:
		case PixelFormat::r32g32b32a32_int128:
		case PixelFormat::r32g32b32a32_float128:
			return 128;
		case PixelFormat::bc1_rgba:
		case PixelFormat::bc1_rgba_srgb:
		case PixelFormat::bc2_rgba:
		case PixelFormat::bc2_rgba_srgb:
		case PixelFormat::bc3_rgba:
		case PixelFormat::bc3_rgba_srgb:
		case PixelFormat::bc4_r_unsigned_norm:
		case PixelFormat::bc4_r_norm:
		case PixelFormat::bc5_rg_unsigned_norm:
		case PixelFormat::bc5_rg_norm:
		case PixelFormat::bc6h_rgb_float:
		case PixelFormat::bc6h_rgbu_float:
		case PixelFormat::bc7_rgba_unsigned_norm:
		case PixelFormat::bc7_rgba_unsinged_norm_srgb:

		case PixelFormat::pvrtc_rgb_2bpp:
		case PixelFormat::pvrtc_rgb_2bpp_srgb:
		case PixelFormat::pvrtc_rgb_4bpp:
		case PixelFormat::pvrtc_rgb_4bpp_srgb:
		case PixelFormat::pvrtc_rgba_2bpp:
		case PixelFormat::pvrtc_rgba_2bpp_srgb:
		case PixelFormat::pvrtc_rgba_4bpp:
		case PixelFormat::pvrtc_rgba_4bpp_srgb:

		case PixelFormat::eac_r11_unsigned_norm:
		case PixelFormat::eac_r11_norm:
		case PixelFormat::eac_r11g11_unsigned_norm:
		case PixelFormat::eac_r11g11_norm:
		case PixelFormat::eac_r8g8b8a8:
		case PixelFormat::eac_r8g8b8a8_srgb:

		case PixelFormat::etc2_r8g8b8:
		case PixelFormat::etc2_r8g8b8_srgb:
		case PixelFormat::etc2_r8g8b8a1:
		case PixelFormat::etc2_r8g8b8a1_srgb:
			assert(0);
			return 128;
		case PixelFormat::astc_4x4_srgb:
		case PixelFormat::astc_5x4_srgb:
		case PixelFormat::astc_5x5_srgb:
		case PixelFormat::astc_6x5_srgb:
		case PixelFormat::astc_6x6_srgb:
		case PixelFormat::astc_8x5_srgb:
		case PixelFormat::astc_8x6_srgb:
		case PixelFormat::astc_8x8_srgb:
		case PixelFormat::astc_10x5_srgb:
		case PixelFormat::astc_10x6_srgb:
		case PixelFormat::astc_10x8_srgb:
		case PixelFormat::astc_10x10_srgb:
		case PixelFormat::astc_12x10_srgb:
		case PixelFormat::astc_12x12_srgb:
		case PixelFormat::astc_4x4_ldr:
		case PixelFormat::astc_5x4_ldr:
		case PixelFormat::astc_5x5_ldr:
		case PixelFormat::astc_6x5_ldr:
		case PixelFormat::astc_6x6_ldr:
		case PixelFormat::astc_8x5_ldr:
		case PixelFormat::astc_8x6_ldr:
		case PixelFormat::astc_8x8_ldr:
		case PixelFormat::astc_10x5_ldr:
		case PixelFormat::astc_10x6_ldr:
		case PixelFormat::astc_10x8_ldr:
		case PixelFormat::astc_10x10_ldr:
		case PixelFormat::astc_12x10_ldr:
		case PixelFormat::astc_12x12_ldr:
		case PixelFormat::astc_4x4_hdr:
		case PixelFormat::astc_5x4_hdr:
		case PixelFormat::astc_5x5_hdr:
		case PixelFormat::astc_6x5_hdr:
		case PixelFormat::astc_6x6_hdr:
		case PixelFormat::astc_8x5_hdr:
		case PixelFormat::astc_8x6_hdr:
		case PixelFormat::astc_8x8_hdr:
		case PixelFormat::astc_10x5_hdr:
		case PixelFormat::astc_10x6_hdr:
		case PixelFormat::astc_10x8_hdr:
		case PixelFormat::astc_10x10_hdr:
		case PixelFormat::astc_12x10_hdr:
		case PixelFormat::astc_12x12_hdr:
			assert(0);        // This needs to be divided by the number of pixels in the block
			return 128;
		case PixelFormat::gbgr422:
		case PixelFormat::bgrg422:
		case PixelFormat::b10g10r10a10_xr:
		case PixelFormat::b10g10r10a10_xr_srgb:
		case PixelFormat::b10g10r10_xr:
		case PixelFormat::b10g10r10_xr_srgb:
			assert(0);
			return 128;
		case PixelFormat::x32_stencil8:
		case PixelFormat::depth32_float32_stencil8_int8:
			return 40;
	}

	return 0;
}

constexpr uint32_t vertex_format_to_bytes(VertexFormat a_vertex_format)
{
	switch (a_vertex_format)
	{
		case VertexFormat::invalid:
			return 0;
		case VertexFormat::struct_0:
		case VertexFormat::struct_1:
			return 0;
		case VertexFormat::int8_1:
		case VertexFormat::int8_1_norm:
		case VertexFormat::uint8_1:
		case VertexFormat::uint8_1_norm:
		case VertexFormat::uint8_custom:
			return 1;
		case VertexFormat::int8_2:
		case VertexFormat::int8_2_norm:
		case VertexFormat::uint8_2:
		case VertexFormat::uint8_2_norm:
		case VertexFormat::int16_1:
		case VertexFormat::int16_1_norm:
		case VertexFormat::uint16_1:
		case VertexFormat::uint16_1_norm:
		case VertexFormat::half16_1:
		case VertexFormat::uint16_custom:
			return 2;
		case VertexFormat::int8_3:
		case VertexFormat::int8_3_norm:
		case VertexFormat::uint8_3:
		case VertexFormat::uint8_3_norm:
			return 3;
		case VertexFormat::bool32_1:
		case VertexFormat::int8_4:
		case VertexFormat::int8_4_norm:
		case VertexFormat::uint8_4:
		case VertexFormat::uint8_4_norm:
		case VertexFormat::int16_2:
		case VertexFormat::int16_2_norm:
		case VertexFormat::uint16_2:
		case VertexFormat::uint16_2_norm:
		case VertexFormat::int32_1:
		case VertexFormat::uint32_1:
		case VertexFormat::half16_2:
		case VertexFormat::float32_1:
		case VertexFormat::int1010102_norm:
		case VertexFormat::uint1010102_norm:
		case VertexFormat::int101111_norm:
		case VertexFormat::uint101111_norm:
		case VertexFormat::uint8_4_norm_bgra:
		case VertexFormat::uint32_custom:
		case VertexFormat::float32_custom:
			return 4;
		case VertexFormat::int16_3:
		case VertexFormat::int16_3_norm:
		case VertexFormat::uint16_3:
		case VertexFormat::uint16_3_norm:
		case VertexFormat::half16_3:
			return 6;
		case VertexFormat::int16_4:
		case VertexFormat::int16_4_norm:
		case VertexFormat::uint16_4:
		case VertexFormat::uint16_4_norm:
		case VertexFormat::bool32_2:
		case VertexFormat::int32_2:
		case VertexFormat::uint32_2:
		case VertexFormat::uint64_1:
		case VertexFormat::half16_4:
		case VertexFormat::float32_2:
		case VertexFormat::float64_1:
			return 8;
		case VertexFormat::bool32_3:
		case VertexFormat::int32_3:
		case VertexFormat::uint32_3:
		case VertexFormat::float32_3:
			return 12;
		case VertexFormat::bool32_4:
		case VertexFormat::int32_4:
		case VertexFormat::uint32_4:
		case VertexFormat::uint64_2:
		case VertexFormat::float32_4:
		case VertexFormat::float64_2:
		case VertexFormat::float32_2x2:
			return 16;
		case VertexFormat::float64_3:
		case VertexFormat::uint64_3:
		case VertexFormat::float32_2x3:
		case VertexFormat::float32_3x2:
			return 24;
		case VertexFormat::float64_4:
		case VertexFormat::uint64_4:
		case VertexFormat::float32_2x4:
		case VertexFormat::float32_4x2:
		case VertexFormat::float64_2x2:
			return 32;
		case VertexFormat::float32_3x3:
			return 36;
		case VertexFormat::float32_3x4:
		case VertexFormat::float32_4x3:
		case VertexFormat::float64_2x3:
		case VertexFormat::float64_3x2:
			return 48;
		case VertexFormat::float32_4x4:
		case VertexFormat::float64_2x4:
		case VertexFormat::float64_4x2:
			return 64;
		case VertexFormat::float64_3x3:
			return 72;
		case VertexFormat::float64_3x4:
		case VertexFormat::float64_4x3:
			return 96;
		case VertexFormat::float64_4x4:
			return 128;
	}

	return 0;
}

constexpr uint32_t vertex_format_to_location(VertexFormat a_vertex_format)
{
	switch (a_vertex_format)
	{
		case VertexFormat::invalid:
			return 0;
		case VertexFormat::struct_0:
		case VertexFormat::struct_1:
		case VertexFormat::bool32_1:
		case VertexFormat::bool32_2:
		case VertexFormat::bool32_3:
		case VertexFormat::bool32_4:
		case VertexFormat::int8_1:
		case VertexFormat::int8_2:
		case VertexFormat::int8_3:
		case VertexFormat::int8_4:
		case VertexFormat::int16_1:
		case VertexFormat::int16_2:
		case VertexFormat::int16_3:
		case VertexFormat::int16_4:
		case VertexFormat::int32_1:
		case VertexFormat::int32_2:
		case VertexFormat::int32_3:
		case VertexFormat::int32_4:
		case VertexFormat::half16_1:
		case VertexFormat::half16_2:
		case VertexFormat::half16_3:
		case VertexFormat::half16_4:
		case VertexFormat::float32_1:
		case VertexFormat::float32_2:
		case VertexFormat::float32_3:
		case VertexFormat::float32_4:
		case VertexFormat::uint8_1:
		case VertexFormat::uint8_2:
		case VertexFormat::uint8_3:
		case VertexFormat::uint8_4:
		case VertexFormat::uint16_1:
		case VertexFormat::uint16_2:
		case VertexFormat::uint16_3:
		case VertexFormat::uint16_4:
		case VertexFormat::uint32_1:
		case VertexFormat::uint32_2:
		case VertexFormat::uint32_3:
		case VertexFormat::uint32_4:
		case VertexFormat::int8_1_norm:
		case VertexFormat::int8_2_norm:
		case VertexFormat::int8_3_norm:
		case VertexFormat::int8_4_norm:
		case VertexFormat::int16_1_norm:
		case VertexFormat::int16_2_norm:
		case VertexFormat::int16_3_norm:
		case VertexFormat::int16_4_norm:
		case VertexFormat::uint8_1_norm:
		case VertexFormat::uint8_2_norm:
		case VertexFormat::uint8_3_norm:
		case VertexFormat::uint8_4_norm:
		case VertexFormat::uint16_1_norm:
		case VertexFormat::uint16_2_norm:
		case VertexFormat::uint16_3_norm:
		case VertexFormat::uint16_4_norm:
		case VertexFormat::int1010102_norm:
		case VertexFormat::uint1010102_norm:
		case VertexFormat::int101111_norm:
		case VertexFormat::uint101111_norm:
		case VertexFormat::uint8_4_norm_bgra:
		case VertexFormat::uint8_custom:          // To be handled differently
		case VertexFormat::uint16_custom:         // To be handled differently
		case VertexFormat::uint32_custom:         // To be handled differently
		case VertexFormat::float32_custom:        // To be handled differently
		case VertexFormat::uint64_1:
		case VertexFormat::uint64_2:
		case VertexFormat::float64_1:
		case VertexFormat::float64_2:
			return 1;        // TODO: This is only true if vertex attribute is used in Vulkan Vertex shader
		case VertexFormat::uint64_3:
		case VertexFormat::uint64_4:
		case VertexFormat::float64_3:
		case VertexFormat::float64_4:
		case VertexFormat::float32_2x2:
		case VertexFormat::float32_2x3:
		case VertexFormat::float32_2x4:
			return 2;
		case VertexFormat::float32_3x3:
		case VertexFormat::float32_3x2:
		case VertexFormat::float32_3x4:
			return 3;
		case VertexFormat::float32_4x2:
		case VertexFormat::float32_4x3:
		case VertexFormat::float32_4x4:
			return 4;
		case VertexFormat::float64_2x2:
		case VertexFormat::float64_2x3:
		case VertexFormat::float64_3x2:
		case VertexFormat::float64_2x4:
		case VertexFormat::float64_4x2:
		case VertexFormat::float64_3x3:
		case VertexFormat::float64_3x4:
		case VertexFormat::float64_4x3:
		case VertexFormat::float64_4x4:
			return 8;        // TODO: This is a guess, confirm
	}

	return 0;
}

constexpr auto vertex_format_to_glsl_type(rhi::VertexFormat a_format)
{
	// bool
	// int
	// uint
	// float
	// double

	// bvecn:
	// ivecn:
	// uvecn:
	// vecn:
	// dvecn:

	// mat3x3
	// mat3x4
	// mat4x4

	// clang-format off
	switch (a_format)
	{
		case rhi::VertexFormat::invalid:			return " ";
		case rhi::VertexFormat::struct_0:			return " ";
		case rhi::VertexFormat::struct_1:			return " ";
		case rhi::VertexFormat::bool32_1:			return "bool   ";
		case rhi::VertexFormat::bool32_2:			return "bvec2  ";
		case rhi::VertexFormat::bool32_3:			return "bvec3  ";
		case rhi::VertexFormat::bool32_4:			return "bvec4  ";
		case rhi::VertexFormat::int8_1:				return "int    ";
		case rhi::VertexFormat::int8_2:				return "ivec2  ";
		case rhi::VertexFormat::int8_3:				return "ivec3  ";
		case rhi::VertexFormat::int8_4:				return "ivec4  ";
		case rhi::VertexFormat::int16_1:			return "int    ";
		case rhi::VertexFormat::int16_2:			return "ivec2  ";
		case rhi::VertexFormat::int16_3:			return "ivec3  ";
		case rhi::VertexFormat::int16_4:			return "ivec4  ";
		case rhi::VertexFormat::int32_1:			return "int    ";
		case rhi::VertexFormat::int32_2:			return "ivec2  ";
		case rhi::VertexFormat::int32_3:			return "ivec3  ";
		case rhi::VertexFormat::int32_4:			return "ivec4  ";
		case rhi::VertexFormat::half16_1:			return "int    ";
		case rhi::VertexFormat::half16_2:			return "ivec2  ";
		case rhi::VertexFormat::half16_3:			return "ivec3  ";
		case rhi::VertexFormat::half16_4:			return "ivec4  ";

		case rhi::VertexFormat::float32_1:			return "float  ";
		case rhi::VertexFormat::float32_2:			return "vec2   ";
		case rhi::VertexFormat::float32_3:			return "vec3   ";
		case rhi::VertexFormat::float32_4:			return "vec4   ";
		case rhi::VertexFormat::float32_2x2:		return "mat2   ";
		case rhi::VertexFormat::float32_2x3:		return "mat2x3 ";
		case rhi::VertexFormat::float32_2x4:		return "mat2x4 ";
		case rhi::VertexFormat::float32_3x2:		return "mat3x2 ";
		case rhi::VertexFormat::float32_3x3:		return "mat3   ";
		case rhi::VertexFormat::float32_3x4:		return "mat3x4 ";
		case rhi::VertexFormat::float32_4x2:		return "mat4x2 ";
		case rhi::VertexFormat::float32_4x3:		return "mat4x3 ";
		case rhi::VertexFormat::float32_4x4:		return "mat4   ";
		case rhi::VertexFormat::float64_2x2:		return "dmat2  ";
		case rhi::VertexFormat::float64_2x3:		return "dmat2x3";
		case rhi::VertexFormat::float64_2x4:		return "dmat2x4";
		case rhi::VertexFormat::float64_3x2:		return "dmat3x2";
		case rhi::VertexFormat::float64_3x3:		return "dmat3  ";
		case rhi::VertexFormat::float64_3x4:		return "dmat3x4";
		case rhi::VertexFormat::float64_4x2:		return "dmat4x2";
		case rhi::VertexFormat::float64_4x3:		return "dmat4x3";
		case rhi::VertexFormat::float64_4x4:		return "dmat4  ";
		case rhi::VertexFormat::float64_1:			return "double ";
		case rhi::VertexFormat::float64_2:			return "dvec2  ";
		case rhi::VertexFormat::float64_3:			return "dvec3  ";
		case rhi::VertexFormat::float64_4:			return "dvec4  ";

		case rhi::VertexFormat::uint8_1:			return "uint   ";
		case rhi::VertexFormat::uint8_2:			return "uvec2  ";
		case rhi::VertexFormat::uint8_3:			return "uvec3  ";
		case rhi::VertexFormat::uint8_4:			return "uvec4  ";
		case rhi::VertexFormat::uint16_1:			return "uint   ";
		case rhi::VertexFormat::uint16_2:			return "uvec2  ";
		case rhi::VertexFormat::uint16_3:			return "uvec3  ";
		case rhi::VertexFormat::uint16_4:			return "uvec4  ";
		case rhi::VertexFormat::uint32_1:			return "uint   ";
		case rhi::VertexFormat::uint32_2:			return "uvec2  ";
		case rhi::VertexFormat::uint32_3:			return "uvec3  ";
		case rhi::VertexFormat::uint32_4:			return "uvec4  ";
		case rhi::VertexFormat::uint64_1:			return "double ";
		case rhi::VertexFormat::uint64_2:			return "dvec2  ";
		case rhi::VertexFormat::uint64_3:			return "dvec3  ";
		case rhi::VertexFormat::uint64_4:			return "dvec4  ";

		case rhi::VertexFormat::int8_1_norm:		return "int    ";
		case rhi::VertexFormat::int8_2_norm:		return "ivec2  ";
		case rhi::VertexFormat::int8_3_norm:		return "ivec3  ";
		case rhi::VertexFormat::int8_4_norm:		return "ivec4  ";
		case rhi::VertexFormat::int16_1_norm:		return "int    ";
		case rhi::VertexFormat::int16_2_norm:		return "ivec2  ";
		case rhi::VertexFormat::int16_3_norm:		return "ivec3  ";
		case rhi::VertexFormat::int16_4_norm:		return "ivec4  ";
		case rhi::VertexFormat::uint8_1_norm:		return "uint   ";
		case rhi::VertexFormat::uint8_2_norm:		return "uvec2  ";
		case rhi::VertexFormat::uint8_3_norm:		return "uvec3  ";
		case rhi::VertexFormat::uint8_4_norm:		return "uvec4  ";
		case rhi::VertexFormat::uint16_1_norm:		return "uint   ";
		case rhi::VertexFormat::uint16_2_norm:		return "uvec2  ";
		case rhi::VertexFormat::uint16_3_norm:		return "uvec3  ";
		case rhi::VertexFormat::uint16_4_norm:		return "uvec4  ";
		case rhi::VertexFormat::int1010102_norm:	return "int    ";
		case rhi::VertexFormat::uint1010102_norm:	return "uint   ";
		case rhi::VertexFormat::int101111_norm:		return "int    ";
		case rhi::VertexFormat::uint101111_norm:	return "uint   ";
		case rhi::VertexFormat::uint8_4_norm_bgra:	return "uint   ";
		case rhi::VertexFormat::uint8_custom:		return "uint   ";
		case rhi::VertexFormat::uint16_custom:		return "uint   ";
		case rhi::VertexFormat::uint32_custom:		return "uint   ";
		case rhi::VertexFormat::float32_custom:		return "float  ";
	}
	// clang-format on

	return "";
}

constexpr auto primitve_toplogy_to_class(PrimitiveTopology a_toplogy)
{
	switch (a_toplogy)
	{
			// clang-format off
		case PrimitiveTopology::points:              return PrimitiveTopologyClass::point;
		case PrimitiveTopology::lines:
		case PrimitiveTopology::lines_strip:         return PrimitiveTopologyClass::line;
		case PrimitiveTopology::triangles:
		case PrimitiveTopology::triangles_strip:     return PrimitiveTopologyClass::triangle;
			// clang-format on
	}

	assert(0);
}

constexpr bool is_attribute_required_in_pass(rhi::BufferSemantic a_semantic, bool a_depth_shadow)
{
	if (a_semantic < rhi::BufferSemantic::vertex_index)        // Only need vertex attributes that are bellow vertex_index, FIXME: what happens to custom ones?
	{
		if (!a_depth_shadow)
			return true;
		else if (a_semantic == rhi::BufferSemantic::vertex_position ||
		         a_semantic == rhi::BufferSemantic::vertex_bone_id_0 ||
		         a_semantic == rhi::BufferSemantic::vertex_bone_id_1 ||
		         a_semantic == rhi::BufferSemantic::vertex_weight_0 ||
		         a_semantic == rhi::BufferSemantic::vertex_weight_1 ||
		         a_semantic == rhi::BufferSemantic::vertex_morph_target ||
		         a_semantic == rhi::BufferSemantic::vertex_morph_weight)
			return true;
		else
			return false;
	}

	return false;
}

constexpr bool is_texture_cubemap(rhi::TextureTarget a_target)
{
	if (a_target == rhi::TextureTarget::texture_cube ||
		a_target == rhi::TextureTarget::texture_cube_array)
		return true;

	return false;
}

constexpr bool is_texture_array(rhi::TextureTarget a_target)
{
	if (a_target == rhi::TextureTarget::texture_1D_array ||
		a_target == rhi::TextureTarget::texture_2D_array ||
		a_target == rhi::TextureTarget::texture_2D_MS_array ||
		a_target == rhi::TextureTarget::texture_cube_array)
		return true;

	return false;
}

constexpr bool is_texture_2d(rhi::TextureTarget a_target)
{
	if (a_target == rhi::TextureTarget::texture_2D ||
		a_target == rhi::TextureTarget::texture_2D_array ||
		a_target == rhi::TextureTarget::texture_2D_MS ||
		a_target == rhi::TextureTarget::texture_2D_MS_array)
		return true;

	return false;
}

constexpr uint32_t semantic_to_index(BufferSemantic a_semantic)
{
#define item(_enum) BufferSemantic::_enum
#define item_value(_enum)

	const BufferSemantic indices[]{describe_buffer_semantics(item)};

#undef item
#undef item_value

	for (size_t i = 0; i < sizeof(indices); ++i)
		if (a_semantic == indices[i])
			return static_cast<uint32_t>(i);

	return 0;
}

constexpr bool has_semantic(uint64_t a_type, BufferSemantic a_semantic)
{
	return ((a_type & ror::enum_to_type_cast(a_semantic)) == ror::enum_to_type_cast(a_semantic));
}

BufferSemantic         get_format_semantic(const std::string &a_format);
std::string            get_format_semantic(const BufferSemantic &a_semantic);
rhi::PixelFormat       string_to_pixel_format(const std::string &a_format);
rhi::VertexFormat      string_to_vertex_format(const std::string &a_format);
rhi::ShaderType        string_to_shader_type(const std::string &a_extension);
const std::string      shader_type_to_string(rhi::ShaderType a_type);
rhi::ShaderStage       string_to_shader_stage(const std::string &a_stage);
rhi::PrimitiveWinding  string_to_primitive_winding(const std::string &a_winding);
rhi::PrimitiveCullMode string_to_cull_mode(const std::string &a_cull_mode);
bool                   is_pixel_format_depth_format(const rhi::PixelFormat a_format);
std::string            renderpass_type_to_string(const rhi::RenderpassType &a_type);

const auto format_to_bytes = vertex_format_to_bytes;

constexpr uint32_t pixel_format_to_bytes(rhi::PixelFormat a_pixel_format)
{
	return pixel_format_to_bits(a_pixel_format) / 8;
}

// Specialisation of these are defined in different places
template <typename _semantic_type>
constexpr rhi::BufferSemantic get_format_shader_semantic()
{
	assert(0 && "Can't use generic version of this function, define specialisation first");
	return rhi::BufferSemantic::custom;
}

#define define_type_to_shader_semantics(type) \
	template <>                               \
	constexpr rhi::BufferSemantic get_format_shader_semantic<type>()
/*
// To use the above for an Animation type do
define_type_to_shader_semantics(Animation)
{
    return BufferSemantic::animation_data;
}
*/
}        // namespace rhi
