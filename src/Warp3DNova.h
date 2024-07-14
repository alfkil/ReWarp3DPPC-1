/** Warp3DNova.h
 *
 * Part of Warp3DNova, a 3D graphics API.
 *
 * Written by Hans de Ruiter (at Kea Sigma Delta Limited)
 *
 * Copyright (C) 2015-2016 by A-EON Technology Limited, all rights reserved.
 *
 * See the documentation for more.
 */

#ifndef WARP3DNOVA_WARP3DNOVA_H
#define WARP3DNOVA_WARP3DNOVA_H

#ifndef   EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef GRAPHICS_DISPLAYINFO_H
#include <graphics/displayinfo.h>
#endif

#ifndef   UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

/** Describes an available GPU.
*/
typedef struct W3DN_Gpu_s {
	const char *name; // The driver's name
	const char *libName; // The driver library's name
	uint32 boardNum; // The board number belonging to this GPU
	struct W3DN_Gpu_s *next; // The next driver in the list
} W3DN_Gpu;

/** Describes an available screenmode.
 */
typedef struct W3DN_ScreenMode_s {
	uint32 modeID; // ModeID for OpenScreen 
	uint32 width; // The screen's visible width
	uint32 height; // The screen's visible height
	uint32 depth; // The pixel depth
	uint32 refreshRate; // The screen's refresh rate in Hz (or frames/second)
	const char displayName[DISPLAYNAMELEN]; // readable name of this display mode
	W3DN_Gpu *gpu; // The GPU that this mode belongs to
	struct W3DN_ScreenMode_s *next; // Next screen-mode in the list
} W3DN_ScreenMode;

/** A common set of error/status codes for all functions.
 */
typedef enum W3DN_ErrorCode_e {
	W3DNEC_SUCCESS = 0, // Operation successful
	W3DNEC_ILLEGALINPUT, // Illegal input parameters
	W3DNEC_UNSUPPORTED, // The requested feature(s)/setting(s) is unsupported
	W3DNEC_NOMEMORY, // No memory available
	W3DNEC_NOVRAM, // No VRAM (video/graphics memory)
	W3DNEC_NODRIVER, // No driver available
	W3DNEC_ILLEGALBITMAP, // Illegal bitmap (i.e., not usable by the driver)
	W3DNEC_NOBITMAP, // No bitmap
	W3DNEC_NOTEXTURE, // No texture
	W3DNEC_UNSUPPORTEDFMT, // The texture/bitmap format is unsupported
	W3DNEC_NOZBUFFER, // No z-buffer
	W3DNEC_NOSTENCILBUFFER, // No stencil buffer
	W3DNEC_UNKNOWNERROR, // Something weird went wrong...
	W3DNEC_INCOMPLETEFRAMEBUFFER, // The framebuffer is incomplete (e.g., the bitmap and z buffer are missing)
	W3DNEC_TIMEOUT, // A timeout occurred
	W3DNEC_QUEUEEMPTY, // The command queue is empty
	W3DNEC_MISSINGVERTEXARRAYS, // One or more vertex attribute arrays are missing
	W3DNEC_FILENOTFOUND, // File not found (e.g., the shader)
	W3DNEC_SHADERSINCOMPATIBLE, // Shaders cannot be used together
	W3DNEC_IOERROR, // An I/O error
	W3DNEC_CORRUPTSHADER, // The shader file was corrupt
	W3DNEC_INCOMPLETESHADERPIPELINE, // The shader pipeline creation failed due to missing shaders
	W3DNEC_NOSHADERPIPELINE, // The RSO doesn't have a shader pipeline set
	W3DNEC_SHADERERRORS, // The shader contained errors and couldn't be compiled
	W3DNEC_MISSINGSHADERDATABUFFERS, // Shader data buffers are missing
	W3DNEC_DEPTHSTENCILPRIVATE, // The depth-stencil buffer is private
	W3DNEC_NOTFOUND, // Object not found
	W3DNEC_EXCEEDEDMAXVARYING, // Exceeded W3DN_Q_MAXVARYINGVECTORS
	W3DNEC_EXCEEDEDMAXTEXUNITS, // Exceeded W3DN_Q_MAXTEXUNITS
	W3DNEC_EXCEEDEDMAXDIM, // Exceeded max dimensions
	
	W3DNEC_MAXERROR // A placeholder marking the end of the error codes
} W3DN_ErrorCode;


// Tags for the W3DN_GetDriversList(), W3DN_GetScreenModeList(), W3DN_BestModeID(),
// & W3DN_CreateContext()

/** Specify which graphics board to use (uint32). 
 * Board IDs/numbers are assigned by the graphics library (see
 * graphics.library/GetMonitorDataTagList()).
 */
#define W3DNTag_BoardNum (TAG_USER + 0)

/** Specifies which screen mode ID to use (uint32).
 * These are the mode IDs assigned by the graphics library (e.g., the value 
 * returned by graphics.library/BestModeID()).
 */
#define W3DNTag_ModeID (TAG_USER + 1)

/** Specifies which screen we wish to use (struct Screen*).
 * If set to NULL, then the default public screen will be used.
 * NOTE: This is simply a shortcut that eliminates having to get a screen's mode ID.
 */
#define W3DNTag_Screen (TAG_USER + 2)

/** Specifies a feature that the GPU/driver must support. 
 * To specify multiple features, just use multiple tags. Features are specified by the
 * query values passed to W3DN_Query()  (e.g., W3DN_Q_ANISOTROPICFILTER). See the 
 * W3DN_Query documentation and the W3DN_Query type for a full list.
 *
 * For "features" with a numeric value such as W3DN_Q_MAXTEXUNITS, you can specify
 * a minimum value by following this tag with W3DNTag_Min, with the numeric value.
 */
#define W3DNTag_ReqFeature (TAG_USER + 3)

/** A followup tag used to specify a minimum value (type varies).
 * For example, if the previous tag was "W3DNTag_ReqFeature, W3DN_Q_MaxTexUnits",
 * then following with "W3DNTag_Min, minTexUnits" would specify the minimum 
 * number of texture units that you need.
 */
#define W3DNTag_Min (TAG_USER + 4)

#define W3DNTag_Reserved4 (TAG_USER + 5)

#define W3DNTag_Reserved5 (TAG_USER + 6)

/** Specifies the desired screen width (uint32).
 * When used with W3DN_GetDriversList(), this will search for drivers that supports a
 * screen-mode with similar dimensions.
 *
 * NOTE: This must be used together with W3DNTag_DesiredScreenHeight
 */
#define W3DNTag_DesiredScreenWidth (TAG_USER + 7)

/** Specifies the desired screen height (uint32).
 * When used with W3DN_GetDriversList(), this will search for drivers that support a
 * screen-mode with similar dimensions (i.e., within +/- 10% of the desired dimensions).
 *
 * NOTE: This must be used together with W3DNTag_DesiredScreenWidth 
 */
#define W3DNTag_DesiredScreenHeight  (TAG_USER + 8)

#define W3DNTag_Reserved7 (TAG_USER + 9)

/** Specifies a minimum screen width (uint32).
 */
#define W3DNTag_MinScreenWidth (TAG_USER + 10)

/** Specifies a minimum screen height (uint32).
 */
#define W3DNTag_MinScreenHeight  (TAG_USER + 11)

#define W3DNTag_Reserved11 (TAG_USER + 12)

/** Specifies a maximum screen width (uint32).
 */
#define W3DNTag_MaxScreenWidth (TAG_USER + 13)

/** Specifies a maximum screen height (uint32).
 */
#define W3DNTag_MaxScreenHeight  (TAG_USER + 14)

#define W3DNTag_Reserved14 (TAG_USER + 15)

/** Specifies a specific GPU to open a context on (W3DN_Gpu*).
 * For W3DN_GetScreenModeList() & W3DN_CreateContext() only.
 */
#define W3DNTag_GPU (TAG_USER + 16)

/** Sets what type of context we want (W3DN_DriverType).
 * This is for W3DN_CreateContext() only. 
 * The options are:
 * - W3DN_DRIVER_BEST - use a hardware driver if available, but allow a software
 *     renderer as fallback (default)
 * - W3DN_DRIVER_HARDWARE - only use a hardware driver. W3DN_CreateContext() will 
 *     fail if no hardware driver is available
 * - W3DN_DRIVER_SOFTWARE - only use a software renderer. W3DN_CreateContext() will 
 *     fail if no software renderer is available
 */
#define W3DNTag_DriverType (TAG_USER + 17)

/** Driver types.
 */
typedef enum W3DN_DriverType_e {
	W3DN_DRIVER_BEST,
	W3DN_DRIVER_HARDWARE,
 	W3DN_DRIVER_SOFTWARE,
	W3DN_DRIVER_END // Marks the end of the enum range
} W3DN_DriverType;


/** Queries for W3DN_Query.
 */
typedef enum W3DN_CapQuery_e {
	W3DN_Q_MAXTEXUNITS, // The maximum number of texture units (uint32)
	W3DN_Q_MAXTEXWIDTH, // Max. texture width
	W3DN_Q_MAXTEXHEIGHT, // Max. texture height
	W3DN_Q_ANISOTROPICFILTER, // Is anisotropic filtering supported (W3DN_CapQueryResult)?
	W3DN_Q_MAXANISOTROPY, // The maximum level of anisotropy supported (uint32). 
	                      // NOTE: W3DN_Q_ANISOTROPICFILTER must be W3DN_SUPPORTED
	W3DN_Q_RENDERTOTEXTURE, // Is render-to-texture supported (W3DN_CapQueryResult)?
	W3DN_Q_BITMAPASTEXTURE, // Is bitmap-as-texture supported  (W3DN_CapQueryResult)?
	W3DN_Q_DEPTHTEXTURE, // Are depth-textures supported (W3DN_CapQueryResult)?
	W3DN_Q_TEXTURE_1D, // Are 1D textures supported (W3DN_CapQueryResult)?
	// NOTE: There is no W3DN_Q_TEXTURE_2D because all drivers MUST implement 2D texturing
	W3DN_Q_TEXTURE_3D, // Are 3D textures supported (W3DN_CapQueryResult)?
	W3DN_Q_TEXTURE_CUBEMAP, // Are cube-map textures supported?
	
	W3DN_Q_MAXCOLOURBUFFERS, // The maximum number of colour buffers that can be rendered to at once
	W3DN_Q_MAXRENDERWIDTH, // The maximum bitmap width that can be rendered to
	W3DN_Q_MAXRENDERHEIGHT, // The maximum bitmap height that can be rendered to
	W3DN_Q_MAXVERTEXATTRIBS, // The maximum number of vertex attribute arrays that can be used
	W3DN_Q_MIPMAPPING, // Is mipmapping supported (W3DN_CapQueryResult)?
	W3DN_Q_MIPMAPGENERATION, // Is mipmap generation supported (W3DN_CapQueryResult)?
	W3DN_Q_MAXTEXDEPTH, // Max. texture depth; only relevant if W3DN_Q_TEXTURE_3D returns W3DN_SUPPORTED (uint32)
	W3DN_Q_NPOT_MIPMAPPING, // Can textures with Non-Power-Of-Two (NPOT) dimensions be mipmapped (W3DN_CapQueryResult)?
	W3DN_Q_STENCIL, // Is stencil buffering supported (W3DN_CapQueryResult)?
	// NOTE: Depth buffer support is mandatory, so there is no query for it
	W3DN_Q_VERTEX_TEXTUREFETCH, // Can vertex shaders access textures?
	W3DN_Q_MAXVARYINGVECTORS, // The max number of 4-element vectors available for interpolating varying variables 
	                          // used by vertex and fragment shaders. Varying variables declared as matrices or arrays 
	                          // will consume multiple interpolators.
	W3DN_Q_MAXTEXCUBEMAPSIZE, // The maximum size for a cube-map
	W3DN_Q_MAXLINEWIDTH, // The maximum line width
	W3DN_Q_MAXPOINTSIZE, // The maximum point size
	W3DN_Q_POLYGONOFFSET, // Is polygon offset supported
	W3DN_Q_POLYGONMODE, // Use different polygon modes for front and back (SetPolygonMode()/GetPolygonMode())
	W3DN_Q_FLATSHADE, // Flat shading can be used (incl. flat GLSL specifier and SetProvokingVertex()/GetProvokingVertex())
	W3DN_Q_END // Marks the end of the enum range
} W3DN_CapQuery;

// For backward compatibility...
#define W3DN_Q_MAX_VARYING_VECTORS W3DN_Q_MAXVARYINGVECTORS

/** The return value for feature queries (e.g., W3DN_Query(gpu, W3DN_Q_ANISOTROPICFILTER).
 */
typedef enum W3DN_CapQueryResult_e {
	W3DN_NOT_SUPPORTED = 0, // Feature not supported
	W3DN_SUPPORTED, // Completely supported 
 	W3DN_CAPQUERY_END // Marks the end of the enum range
} W3DN_CapQueryResult;


/** Pixel formats (for both textures and render targets).
 * NOTE: While the channels are labelled, Red, Green, Blue, & Alpha (RGBA), the 
 * meaning of each channel is entirely dependent on what a shader uses it for.
 * So, you can also consider:
 * - Red = channel 0/x
 * - Green = channel 1/y
 * - Blue = channel 2/z
 * - Alpha = channel 3/w
 * If you need the channels to be in a different order, then set the texture's/image's
 * swizzle mask.
 */
typedef enum W3DN_PixelFormat_e {
	W3DNPF_DEPTH, // One channel: depth data
	W3DNPF_DEPTH_STENCIL, // Two channels: depth and stencil
	W3DNPF_RED, // One channel: (the remaining channels are set to 0.0f, 0.0f, and 1.0f)
	W3DNPF_RG, // Two channels: Red and Green (the remaining channels are set to 0.0f and 1.0f)
	W3DNPF_RGB, // Three channels: Red, Green, & Blue (the alpha channel is set to 1.0f)
	W3DNPF_RGBA, // Four channels: Red, Green, Blue, & Alpha
	
	// Special formats
	W3DNPF_SRGB8, // Three channels in sRGB colour-space (element format must be W3DNEF_UINT8)
	W3DNPF_SRGB8_A8, // Four channels: sRGB, and an 8-bit alpha (element format must be W3DNEF_UINT8)
	// ##### TODO! ##### Compressed texture formats
	W3DNPF_END, // Marks the end of the enum range
	W3DNPF_NONE = ~0 // Indicates no pixel format
} W3DN_PixelFormat;

/** The number formats for the elements within a pixel. 
 */
typedef enum W3DN_ElementFormat_e {
	W3DNEF_UINT8, // 8-bit unsigned integers
	W3DNEF_SINT8, // 8-bit signed integers
	W3DNEF_UINT16, // 16-bit unsigned integers
	W3DNEF_SINT16, // 16-bit signed integers
	W3DNEF_UINT32, // 32-bit unsigned integers
	W3DNEF_SINT32, // 32-bit signed integers
	W3DNEF_FLOAT, // 32-bit floating point
	W3DNEF_DOUBLE, // 64-bit double-precision floating point
	
	// Special formats
	W3DNEF_UINT8_3_3_2, // 8-bit packed format: 3 bits for R, 3 bits for G, and 2 bits for B (pixel format must be W3DNFT_RGB)
	W3DNEF_UINT8_2_3_3_REV, // The same as W3DNEF_UINT8_3_3_2, but the channels are stored in BGR order
	W3DNEF_UINT16_5_6_5, // 16-bit packed format: 5 bits for R, 6 bits for G, and 5 bits for B (pixel format must be W3DNFT_RGB)
	W3DNEF_UINT16_4_4_4_4, // 16-bit packed format: 4 bits for each channel (pixel format must be W3DNFT_RGBA)
	W3DNEF_UINT16_5_5_5_1, // 16-bit packed format: 5 bits each for R, G, & B, with 1 bit for A (pixel format must be W3DNFT_RGBA)
	W3DNEF_UINT16_1_5_5_5_REV, // The same as W3DNEF_UINT16_5_5_5_1, but the channels are stored in ABGR order 
	                           // (pixel format must be W3DNFT_RGBA)
	W3DNEF_UINT32_10_10_10_2, // 32-bit format: 10 bits each for R, G, & B, with 2 bits for A (pixel format must be W3DNFT_RGBA)
	W3DNEF_UINT32_2_10_10_10_REV, // The same as W3DNEF_UINT32_10_10_10_2, but the channels are stored in ABGR order 
	                              // (pixel format must be W3DNFT_RGBA)
	W3DNEF_END, // Marks the end of the enum range
	W3DNEF_NONE = ~0 // Indicates no element format
} W3DN_ElementFormat;

// Tags for W3DN_GetTexFmtInfo() and W3DN_GetBMFmtInfo()
#define W3DNFmtTag_Base (TAG_USER + 0x1000) // Means nothing, but is the base for all W3DN_GetTexFmtInfo() tags

/** Is the texture/bitmap pixel format supported for texture mapping?
 * This tag points to a BOOL variable which will be set to TRUE if the texture/bitmap
 * can be used as a texture, and is set to FALSE otherwise.
 */ 
#define W3DNFmtTag_CanBeTexture (W3DNFmtTag_Base + 0)

/** Is this texture/bitmap format supported natively?
 * This tag points to a BOOL variable which will be set to TRUE if the texture/bitmap
 * format is one that is supported natively by the GPU/driver (for texturing), and is 
 * set to FALSE if the format has to be converted internally to another format.
 */
#define W3DNFmtTag_NativeTexture (W3DNFmtTag_Base + 1)

/** Can this texture/bitmap format be used as a (colour) render target?
 * This tag points to a BOOL variable which will be set to TRUE if the texture/bitmap
 * format is one that can be rendered to, and FALSE if it cannot be used as a colour
 * render target.
 */
#define W3DNFmtTag_CanBeRenderTarget (W3DNFmtTag_Base + 2)


/** The maximum width texture/bitmap of this format that can use (as texture).
 * This tag points to a uint32 variable where the maximum texture width will be 
 * written.
 */
#define W3DNFmtTag_MaxTexWidth (W3DNFmtTag_Base + 3)

/** The maximum height texture/bitmap of this format that can use (as texture).
 * This tag points to a uint32 variable where the maximum texture height will be 
 * written.
 */
#define W3DNFmtTag_MaxTexHeight (W3DNFmtTag_Base + 4)

/** The maximum width texture/bitmap of this format that can be rendered to.
 * This tag points to a uint32 variable where the maximum render width will be 
 * written.
 */
#define W3DNFmtTag_MaxRenderWidth (W3DNFmtTag_Base + 5)

/** The maximum height texture/bitmap of this format that can be rendered to.
 * This tag points to a uint32 variable where the maximum render height will be 
 * written.
 */
#define W3DNFmtTag_MaxRenderHeight (W3DNFmtTag_Base + 6)


// Putting this here because the header file relies on definititions in this header
#include <Warp3DNova/Context.h>

#endif

