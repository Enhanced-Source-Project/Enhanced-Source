
#ifndef COMMON_LIGHTING_H
#define COMMON_LIGHTING_H

static const float3 flWorldGridSize = float3(
	RADIOSITY_BUFFER_GRID_STEP_SIZE_CLOSE * RADIOSITY_BUFFER_SAMPLES_XY,
	RADIOSITY_BUFFER_GRID_STEP_SIZE_CLOSE * RADIOSITY_BUFFER_SAMPLES_XY,
	RADIOSITY_BUFFER_GRID_STEP_SIZE_CLOSE * RADIOSITY_BUFFER_SAMPLES_Z
	);
static const float3 flWorldGridSize_Far = float3(
	RADIOSITY_BUFFER_GRID_STEP_SIZE_FAR * RADIOSITY_BUFFER_SAMPLES_XY,
	RADIOSITY_BUFFER_GRID_STEP_SIZE_FAR * RADIOSITY_BUFFER_SAMPLES_XY,
	RADIOSITY_BUFFER_GRID_STEP_SIZE_FAR * RADIOSITY_BUFFER_SAMPLES_Z
	);

static const float2 flRadiosityTexelSizeHalf = float2( 0.5f / RADIOSITY_BUFFER_RES_X,
	0.5f / RADIOSITY_BUFFER_RES_Y );
static const float2 flRadiosityUVRatio = float2( RADIOSITY_UVRATIO_X, RADIOSITY_UVRATIO_Y );

float3 DoStandardCookie( sampler sCookie, float2 uvs )
{
	return tex2D( sCookie, uvs ).rgb;
}

float3 DoCubemapCookie( sampler sCubemap, float3 delta )
{
	return texCUBE( sCubemap, delta ).rgb;
}

float4 DoLightFinal( float3 diffuse, float3 ambient, float4 litdot_lamount_spec_fade )
{
#if DEFCFG_CHEAP_LIGHTS
	return float4( diffuse * litdot_lamount_spec_fade.y, 0 );
#else
	return float4( lerp( ambient,
		diffuse * ( litdot_lamount_spec_fade.x ),
		litdot_lamount_spec_fade.y ),
		litdot_lamount_spec_fade.z * litdot_lamount_spec_fade.y ) * litdot_lamount_spec_fade.w;
#endif
}

float4 DoLightFinalCookied( float3 diffuse, float3 ambient, float4 litdot_lamount_spec_fade, float3 vecCookieRGB )
{
#if DEFCFG_CHEAP_LIGHTS
	return float4( diffuse * vecCookieRGB * litdot_lamount_spec_fade.y, 0 );
#else
	return float4( lerp( ambient,
		diffuse * vecCookieRGB * ( litdot_lamount_spec_fade.x ),
		litdot_lamount_spec_fade.y ),
		litdot_lamount_spec_fade.z * litdot_lamount_spec_fade.y * dot( vecCookieRGB, float3( 0.3f, 0.59f, 0.11f ) ) )
		* litdot_lamount_spec_fade.w;
#endif
}

float3 GetBilinearRadiositySample( sampler RadiositySampler, float3 vecPositionDelta, float2 vecUVOffset )
{
	float2 flGridUVLocal = vecPositionDelta.xy / RADIOSITY_BUFFER_GRIDS_PER_AXIS;

	float2 flGridIndexSplit;
	flGridIndexSplit.x = modf( vecPositionDelta.z * RADIOSITY_BUFFER_GRIDS_PER_AXIS, flGridIndexSplit.y );

	flGridIndexSplit.x *= RADIOSITY_BUFFER_GRIDS_PER_AXIS;
	float flSampleFrac = modf( flGridIndexSplit.x, flGridIndexSplit.x );

	flGridIndexSplit /= RADIOSITY_BUFFER_GRIDS_PER_AXIS;

	float2 flGridUVLow = flRadiosityUVRatio * (flGridUVLocal + flGridIndexSplit) + flRadiosityTexelSizeHalf;

	flGridIndexSplit.x = modf(
		( floor( vecPositionDelta.z * RADIOSITY_BUFFER_SAMPLES_Z ) + 1 ) / RADIOSITY_BUFFER_GRIDS_PER_AXIS,
		flGridIndexSplit.y );
	flGridIndexSplit.y /= RADIOSITY_BUFFER_GRIDS_PER_AXIS;

	float2 flGridUVHigh = flRadiosityUVRatio * (flGridUVLocal + flGridIndexSplit) + flRadiosityTexelSizeHalf;

	return lerp( tex2D( RadiositySampler, flGridUVLow + vecUVOffset ).rgb,
		tex2D( RadiositySampler, flGridUVHigh + vecUVOffset ).rgb, flSampleFrac );
}


float3 DoRadiosity( float3 worldPos,
	sampler RadiositySampler, float3 vecRadiosityOrigin, float3 vecRadiosityOrigin_Far,
	float flRadiositySettings )
{
#if RADIOSITY_SMOOTH_TRANSITION == 1
	float3 vecDeltaFar = ( worldPos - vecRadiosityOrigin_Far ) / flWorldGridSize_Far;

#if VENDOR == VENDOR_FXC_AMD
	AMD_PRE_5K_NON_COMPLIANT
#elif ( DEFCFG_DEFERRED_SHADING == 0 )
	clip( 0.5f - any( floor( vecDeltaFar ) ) );
#else
	flRadiositySettings *= 1 - any( floor( vecDeltaFar ) );
#endif

	float3 vecDeltaClose = ( worldPos - vecRadiosityOrigin ) / flWorldGridSize;

	float3 flTransition = abs( saturate( vecDeltaClose ) * 2 - 1 );
	float flBlendAmt = max( flTransition.x, max( flTransition.y, flTransition.z ) );
	flBlendAmt = smoothstep( 0.7f, 1.0f, flBlendAmt );

	return lerp( GetBilinearRadiositySample( RadiositySampler, vecDeltaClose, float2(0,0) ),
		GetBilinearRadiositySample( RadiositySampler, vecDeltaFar, float2(0,0.5f) ),
		flBlendAmt ) * flRadiositySettings.x;
#else
	float3 vecDelta = ( worldPos - vecRadiosityOrigin ) / flWorldGridSize;

#if VENDOR == VENDOR_FXC_AMD
	AMD_PRE_5K_NON_COMPLIANT
#else
	float flLerpTo1 = any( floor( (vecDelta.xyz - 0.025f) * 1.05f) );
#endif

	vecDelta = lerp( vecDelta,
		( worldPos - vecRadiosityOrigin_Far ) / flWorldGridSize_Far,
		flLerpTo1 );

	float2 flUV_Y_Offset = float2( 0, flLerpTo1 ) * 0.5f;

#if VENDOR == VENDOR_FXC_AMD
	AMD_PRE_5K_NON_COMPLIANT
#elif ( DEFCFG_DEFERRED_SHADING == 0 )
	clip( 0.5f - any( floor( vecDelta ) ) );
#else
	flRadiositySettings *= 1 - any( floor( vecDeltaFar ) );
#endif

	return GetBilinearRadiositySample( RadiositySampler, vecDelta, flUV_Y_Offset ) * flRadiositySettings.x;
#endif
}

#endif