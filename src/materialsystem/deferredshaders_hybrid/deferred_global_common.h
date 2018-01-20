/****************************************************************
 * Shared knobs and defs for shaders, shader, client and server dll
 *
 * IF YOU CHANGE SHIT IN HERE YOU WILL MOST LIKELY HAVE TO
 * RECOMPILE *EVERYTHING* SO MAKE SURE THAT YOU CAN
 * SUCCESSFULLY COMPILE ALL SHADERS FIRST TOO
 ****************************************************************/

#ifndef DEFERRED_GLOBAL_COMMON_H
#define DEFERRED_GLOBAL_COMMON_H


/* Run with deferred shading instead of deferred lighting,
 * Light accumulation is used for both.
 */
#define DEFCFG_DEFERRED_SHADING 0

/* Toggles packing mode of lighting controls (phong, halflambert, litface)
 * 0 - DISABLE packing, gbuffer 0 at 3 bytes, gbuffer 2 at 4 bytes, less expensive math
 * 1 - ENABLE packing, gbuffer 0 at 4 bytes, gbuffer 2 disabled, more expensive math
 */
#define DEFCFG_LIGHTCTRL_PACKING 1


/* Toggles compression for light accumulation buffer
 * 0 - DISABLE compression, 8 bytes light buffer, floating point
 * 1 - ENABLE compression, 3 bytes light buffer, integer
 */
#define DEFCFG_LIGHTACCUM_COMPRESSED 0


/* Toggles SRGB color space conversion for composition pass
 */
#define DEFCFG_USE_SRGB_CONVERSION 1


/* Disables specular, ambient, lit face and
 * per light falloff exponent math
 */
#define DEFCFG_CHEAP_LIGHTS 0


/* Use blinn phong instead of reflection/viewdot
 */
#define DEFCFG_BLINN_PHONG 1

/* Use SSE vector processor instrinsics to speed up
 * deferred light performance
 */
#define DEFCFG_USE_SSE 1

/* Use extra sorting to diminish render context state
 * changes.
  */
#define DEFCFG_EXTRA_SORT 0

/* Enable adaptive shadowmap LOD controls.
 */
#define DEFCFG_ADAPTIVE_SHADOWMAP_LOD 0

/* Enable adaptive volumetrics LOD controls.
 */
#define DEFCFG_ADAPTIVE_VOLUMETRIC_LOD	0

/* Enable configurable volumetrics LOD controls.
 */
#define DEFCFG_CONFIGURABLE_VOLUMETRIC_LOD	1

#if DEFCFG_ADAPTIVE_VOLUMETRIC_LOD && DEFCFG_CONFIGURABLE_VOLUMETRIC_LOD
#	undef	DEFCFG_ADAPTIVE_VOLUMETRIC_LOD
#	define	DEFCFG_ADAPTIVE_VOLUMETRIC_LOD 0
#endif

/* RT Names
 */
#define DEFRTNAME_GBUFFER0 "_rt_defNormals"
#define DEFRTNAME_GBUFFER1 "_rt_defProjDepth"
#if DEFCFG_LIGHTCTRL_PACKING == 0
#	define DEFRTNAME_GBUFFER2 "_rt_def_UnpackedLightControl"
#endif
#if DEFCFG_DEFERRED_SHADING
#	define DEFRTNAME_GBUFFER2 "_rt_defAlbedo"
#	define DEFRTNAME_GBUFFER3 "_rt_defSpecular"
#endif
#define DEFRTNAME_LIGHTACCUM "_rt_LightAccum"
#define DEFRTNAME_LIGHTACCUM2 "_rt_LightAccum2"

#define DEFRTNAME_VOLUMPREPASS "_rt_VolumPrepass"
#define DEFRTNAME_VOLUMACCUM "_rt_VolumAccum_"					// + %02i

#define DEFRTNAME_SHADOWDEPTH_ORTHO "_rt_ShadowDepth_ortho_"	// + %02i
#define DEFRTNAME_SHADOWDEPTH_PROJ "_rt_ShadowDepth_proj_"		// + %02i
#define DEFRTNAME_SHADOWDEPTH_DP "_rt_ShadowDepth_dp_"			// + %02i
#define DEFRTNAME_SHADOWCOLOR_ORTHO "_rt_ShadowColor_ortho_"	// + %02i
#define DEFRTNAME_SHADOWCOLOR_PROJ "_rt_ShadowColor_proj_"		// + %02i
#if DEFCFG_ADAPTIVE_SHADOWMAP_LOD
#	define DEFRTNAME_SHADOWDEPTH_PROJ_LOD1 "_rt_ShadowDepth_proj_lod1_"	// + %02i	
#	define DEFRTNAME_SHADOWCOLOR_PROJ_LOD1 "_rt_ShadowColor_proj_lod1_"	// + %02i	
#	define DEFRTNAME_SHADOWDEPTH_PROJ_LOD2 "_rt_ShadowDepth_proj_lod2_"	// + %02i	
#	define DEFRTNAME_SHADOWCOLOR_PROJ_LOD2 "_rt_ShadowColor_proj_lod2_"	// + %02i	
#endif
#define DEFRTNAME_SHADOWCOLOR_DP "_rt_ShadowColor_dp_"			// + %02i
#define DEFRTNAME_SHADOWRAD_ALBEDO_ORTHO "_rt_ShadowRad_Albedo_ortho_"	// + %02i
#define DEFRTNAME_SHADOWRAD_NORMAL_ORTHO "_rt_ShadowRad_Normal_ortho_"	// + %02i

#define DEFRTNAME_PROJECTABLE_VGUI "_rt_projvgui_"				// + %02i

#define DEFRTNAME_RADIOSITY_BUFFER "_rt_radBuffer_"				// + %02i
#define DEFRTNAME_RADIOSITY_NORMAL "_rt_radNormal_"				// + %02i


/* One physical shadowmap for multiple cascades
 * The default projection shaders REQUIRE this
 */
#define CSM_USE_COMPOSITED_TARGET 1

#if CSM_USE_COMPOSITED_TARGET
/* Composited resolution
 */
//#	define CSM_COMP_RES_X 4096
//#	define CSM_COMP_RES_Y 2048

#	define CSM_COMP_RES_X_LOW 4096
#	define CSM_COMP_RES_Y_LOW 2048

#	define CSM_COMP_RES_X_HIGH 8192
#	define CSM_COMP_RES_Y_HIGH 4096

#	define CSM_COMP_RES_X GetCMSCompResX()
#	define CSM_COMP_RES_Y GetCMSCompResY()
//#	define CSM_COMP_RES_X 8192
//#	define CSM_COMP_RES_Y 4096
#endif


/* Radiosity stuff
 */
#define DEFCFG_ENABLE_RADIOSITY 0

#	define RADIOSITY_SMOOTH_TRANSITION 1

#	define RADIOSITY_BUFFER_SAMPLES_XY 42
#	define RADIOSITY_BUFFER_SAMPLES_Z 36

#	define RADIOSITY_BUFFER_RES_X 256
#	define RADIOSITY_BUFFER_RES_Y 512
#	define RADIOSITY_BUFFER_VIEWPORT_SX 252
#	define RADIOSITY_BUFFER_VIEWPORT_SY 252

#	define RADIOSITY_BUFFER_GRID_STEP_SIZE_CLOSE 20.0f
#	define RADIOSITY_BUFFER_GRID_STEP_DISTANCEMULT_CLOSE 0.3f
#	define RADIOSITY_BUFFER_GRID_STEP_SIZE_FAR 72.0f
#	define RADIOSITY_BUFFER_GRID_STEP_DISTANCEMULT_FAR 0.5f
#	define RADIOSITY_BUFFER_GRIDS_PER_AXIS 6

#	define RADIOSITY_UVRATIO_X (RADIOSITY_BUFFER_VIEWPORT_SX/(float)RADIOSITY_BUFFER_RES_X)
#	define RADIOSITY_UVRATIO_Y (RADIOSITY_BUFFER_VIEWPORT_SY/(float)RADIOSITY_BUFFER_RES_Y)


/* Amount of RTs (or views for composited cascades) allocated per shadow type
 * Not the max amount of shadows in total!
 */
#define MAX_SHADOW_ORTHO 2
#define MAX_SHADOW_PROJ 5
#define MAX_SHADOW_DP 5


/* Supported light types
 */
#define DEFLIGHTTYPE_POINT 0
#define DEFLIGHTTYPE_SPOT 1

#define MAX_DEFLIGHTTYPE_BITS 1


/* Max Amount of lights per pass
 */
#define MAX_LIGHTS_SHADOWEDCOOKIE	2
#define MAX_LIGHTS_SHADOWED			3
#define MAX_LIGHTS_COOKIE			3
#define MAX_LIGHTS_SIMPLE			10

/* Num consts per light type
*/
#define NUM_CONSTS_POINT_SIMPLE		3
#define NUM_CONSTS_POINT_ADVANCED	6
#define NUM_CONSTS_SPOT_SIMPLE		4
#define NUM_CONSTS_SPOT_ADVANCED	9


/* Shadowing modes
 */
#define	DEFERRED_SHADOW_MODE_ORTHO 0
#define	DEFERRED_SHADOW_MODE_PROJECTED 1
#define	DEFERRED_SHADOW_MODE_DPSM 2


/* Shadowmapping type and filter
 */
#define SHADOWMAPPING_DEPTH_COLOR__RAW 0
#define SHADOWMAPPING_DEPTH_COLOR__4X4_SOFTWARE_BILINEAR_BOX 1
#define SHADOWMAPPING_DEPTH_COLOR__4X4_SOFTWARE_BILINEAR_GAUSSIAN 2
#define SHADOWMAPPING_DEPTH_COLOR__5X5_SOFTWARE_BILINEAR_GAUSSIAN 3
#define SHADOWMAPPING_DEPTH_COLOR__PCSS_4X4_PCF_4X4 4
#define SHADOWMAPPING_DEPTH_STENCIL__RAW 5
#define SHADOWMAPPING_DEPTH_STENCIL__3X3_GAUSSIAN 6
#define SHADOWMAPPING_DEPTH_STENCIL__5X5_GAUSSIAN 7

#define SHADOWMAPPING_METHOD	SHADOWMAPPING_DEPTH_COLOR__5X5_SOFTWARE_BILINEAR_GAUSSIAN

#if ( SHADOWMAPPING_METHOD <= SHADOWMAPPING_DEPTH_COLOR__PCSS_4X4_PCF_4X4 )
#	define SHADOWMAPPING_USE_COLOR 1
#endif


/* Vendor defs for specific hardware filters
 * or goddamn stupid code because some amd cards are damn stupid and broken!!!
 */
#define VENDOR_FXC_NVIDIA 0
#define VENDOR_FXC_AMD 1


/* Volumetrics quality/tweaks
 */
#if DEFCFG_ADAPTIVE_VOLUMETRIC_LOD
#	define VOLUMQUALITY_POINT_SAMPLES_LOD0	49
#	define VOLUMQUALITY_POINT_SAMPLES_LOD1	25
#	define VOLUMQUALITY_POINT_SAMPLES_LOD2	13
#	define VOLUMQUALITY_POINT_SAMPLES_LOD3	7
#	define VOLUMQUALITY_POINT_SAMPLES_LOD4	4
#	define VOLUMQUALITY_SPOT_SAMPLES_LOD0	49
#	define VOLUMQUALITY_SPOT_SAMPLES_LOD1	25
#	define VOLUMQUALITY_SPOT_SAMPLES_LOD2	13
#	define VOLUMQUALITY_SPOT_SAMPLES_LOD3	7
#	define VOLUMQUALITY_SPOT_SAMPLES_LOD4	4
#else
#	define VOLUMQUALITY_POINT_SAMPLES 50
#	define VOLUMQUALITY_SPOT_SAMPLES 50
#endif
#define VOLUMTWEAK_INTENSITY_POINT 1.0f
#define VOLUMTWEAK_INTENSITY_SPOT 0.5f

/* Global hard coded filter tweaks
 */
#define SHADOWMAPPINGTWEAK_DPSM_EPSILON 0.01f


/* Lighting tweaks
 */
#define SPECULAREXP_BASE 6.0f
#define SPECULARSCALE_DIV 0.005f //196.0f


/* Maximal depth that can be reconstructed
 * Fullscreen version for fullscreen world lights
 */
#define DEPTH_RECONSTRUCTION_LIMIT 7000.0f
#define DEPTH_RECONSTRUCTION_LIMIT_FULLSCREEN 4096.0f


/* Compression scale for integer light buff
 * 2.0 means that albedos with any channel < 0.5 can never become fully white by diffuse lighting
 */
#if DEFCFG_LIGHTACCUM_COMPRESSED
#	define DEFCFG_LIGHTSCALE_COMPRESS_RATIO 2.0f
#endif


/* How many samplers are used for shadows/cookies per pass?
 */
#if DEFCFG_LIGHTCTRL_PACKING
#	define FREE_LIGHT_SAMPLERS			13 //14
#	define FIRST_LIGHT_SAMPLER			2
#	define FIRST_LIGHT_SAMPLER_FXC		s2
#else
#	define FREE_LIGHT_SAMPLERS			13
#	define FIRST_LIGHT_SAMPLER			3
#	define FIRST_LIGHT_SAMPLER_FXC		s3
#endif

#define FIRST_SHARED_LIGHTDATA_CONSTANT 31
#define FIRST_SHARED_LIGHTDATA_CONSTANT_FXC c31
#define MAX_LIGHTDATA_CONSTANT_ROWS 193

/* Projectable VGUI settings
 */
#define PROJECTABLE_VGUI_RES 512


/* DON'T TOUCH THE STUFF BELOW
 */
static const int SHADOW_NUM_CASCADES = MAX_SHADOW_ORTHO;

#if DEFCFG_DEFERRED_SHADING && DEFCFG_LIGHTCTRL_PACKING == 0
#error "can't use deferred shading and unpacked lighting controls at the same time"
#endif

#ifdef __cplusplus
static const int NUM_COOKIE_SLOTS = MAX_LIGHTS_SHADOWEDCOOKIE + MAX_LIGHTS_COOKIE;
static const int NUM_PROJECTABLE_VGUI = NUM_COOKIE_SLOTS;
#endif

#if CSM_USE_COMPOSITED_TARGET
#	undef MAX_SHADOW_ORTHO
#	define MAX_SHADOW_ORTHO 1
#endif

#endif