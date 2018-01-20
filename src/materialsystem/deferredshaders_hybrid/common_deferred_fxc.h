
#ifndef COMMON_DEFERRED_FXC_H
#define COMMON_DEFERRED_FXC_H

// SKIP:		!$MODEL && $MORPHING_VTEX

// SKIP:		!$MODEL && $COMPRESSED_VERTS
// SKIP:		!$MODEL && $SKINNING
// SKIP:		!$MODEL && $MORPHING_VTEX

#include "common_fxc.h"
#include "deferred_global_common.h"
#include "common_lighting_fxc.h"
#include "common_shadowmapping_fxc.h"


float WriteDepth( float3 eyeworldvec, float3 eyeforward, float scale )
{
	return dot( eyeforward, eyeworldvec ) * scale;
}
float WriteDepth( float3 worldpos, float3 eyepos, float3 eyeforward, float scale )
{
	float3 d = worldpos - eyepos;
	return WriteDepth( d, eyeforward, scale );
}

float4 WriteLighting( float4 normalizedLight )
{
#if DEFCFG_LIGHTACCUM_COMPRESSED
	return normalizedLight / DEFCFG_LIGHTSCALE_COMPRESS_RATIO;
#else
	return normalizedLight;
#endif
}

float4 ReadLighting( float4 compressedLight )
{
#if DEFCFG_LIGHTACCUM_COMPRESSED
	return compressedLight * DEFCFG_LIGHTSCALE_COMPRESS_RATIO;
#else
	return compressedLight;
#endif
}

half2 EncodeDirectionVectorXYZToSphericalST( float3 vDirectionVector )
{
	half p = sqrt(vDirectionVector.z*8+8);
	return half2( vDirectionVector.xy/p + 0.5 );

    /*float2 vEncoded = normalize( vDirectionVector.xy ) * sqrt( -vDirectionVector.z * 0.5 + 0.5 );
    vEncoded = vEncoded * 0.5 + 0.5;
    return vEncoded;*/
}

float3 DecodeDirectionVectorXYZFromSphericalST( half2 vSphericalST )
{
	half2 fenc = vSphericalST*4-2;
    half f = dot(fenc,fenc);
    half g = sqrt(1-f/4);
    float3 n;
    n.xy = fenc*g;
    n.z = 1-f/2;
    return n;
    /*float3 vDirectionVector = float3(vSphericalST.x, vSphericalST.y, 0) * float3(2,2,0) + float3(-1,-1,1);
	float flZLength = dot( vDirectionVector.xyz, float3( -vDirectionVector.xy, -1 ) );
	vDirectionVector.z = flZLength;
	vDirectionVector.xy *= sqrt(flZLength);
    return vDirectionVector.xyz * 2 + float3(0,0,-1);*/
}

// phong exp 6						-half lambert 1	- litface 1
// 128 + 64 + 32 + 16 + 8 + 4		- 2				- 1
// 6 bits, 63						- 1				- 1

float PackLightingControls( int phong_exp, int half_lambert, int litface )
{
	return ( litface +
		half_lambert * 2 +
		phong_exp * 4 ) * 0.0039215686f;
}

void UnpackLightingControls( float mixed,
	out float phong_exp, out bool half_lambert, out bool litface )
{
	mixed *= 255.0f;

	litface = fmod( mixed, 2.0f );
	float tmp = fmod( mixed -= litface, 4.0f );
	phong_exp = fmod( mixed -= tmp, 256.0f );

#if 0
	// normalized values
	half_lambert /= 2.0f;
	phong_exp /= 252.0f;
#else
	// pre-scaled values for lighting
	//half_lambert *= 0.5f;
	half_lambert = tmp * 0.5f;
	phong_exp = pow( SPECULAREXP_BASE, 1 + phong_exp * 0.02f );
#endif
}

#define ONEOVERTHREE 0.33333333333333333333333333333333
#define TWOOVERTHREE 0.66666666666666666666666666666666

float PackLightingControls( bool half_lambert, bool litface )
{
	if( half_lambert && litface )
		return 1;
	else if( half_lambert )
		return 1/3;
	else if( litface )
		return 2/3;
	else return 0;
}

void UnpackLightingControls( float mixed, 
	out bool half_lambert, out bool litface )
{
	if( mixed == 1 )
	{
		half_lambert = true;
		litface = true;
	}
	else if( mixed == 1/3 )
	{
		half_lambert = true;
		litface = false;
	}
	else if( mixed == 2/3 )
	{
		half_lambert = false;
		litface = true;
	}
	else
	{
		half_lambert = false;
		litface = false;
	}
}

// compensates for point sampling
float2 GetLightAccumUVs( float3 projXYW, float2 halfScreenTexelSize )
{
	return projXYW.xy / projXYW.z *
		float2( 0.5f, -0.5f ) +
		0.5f +
		halfScreenTexelSize;
}

float2 GetTransformedUVs( in float2 uv, in float4 transform[2] )
{
	float2 uvOut;

	uvOut.x = dot( uv, transform[0] ) + transform[0].w;
	uvOut.y = dot( uv, transform[1] ) + transform[1].w;

	return uvOut;
}

float GetModulatedBlend( in float flBlendAmount, in float2 modt )
{
	float minb = max( 0, modt.g - modt.r );
	float maxb = min( 1, modt.g + modt.r );

	return smoothstep( minb, maxb, flBlendAmount );
}

float GetMultiBlendModulated( in float2 modt, const float flBlendAmount, const float AlphaBlend, inout float Remaining )
{
	//modt.r = lerp( modt.r, 1, AlphaBlend );

	float Result = any( flBlendAmount )
		* GetModulatedBlend( flBlendAmount, modt );

	float alpha = 2.0 - AlphaBlend - modt.g;
	Result *= lerp( saturate( alpha ), 1, step( AlphaBlend, modt.g ) );

	Result = min( Result, Remaining );
	Remaining -= Result;

	return Result;
}

float2 ComputeTexCoord( const float2 vBaseCoord, const float flRotation, const float flScale )
{
	float2 	vAdjust = vBaseCoord - float2( 0.5, 0.5 );
	float2 	vResult;
	float 	c = cos( flRotation );
	float 	s = sin( flRotation );
	
   	vResult.x = ( vAdjust.x * c ) + ( vAdjust.y * -s );
   	vResult.y = ( vAdjust.x * s ) + ( vAdjust.y * c );
   	
   	return ( vResult / flScale ) + float2( 0.5, 0.5 );
}

float ComputeMultiBlendFactor( const float BlendStart, const float BlendEnd, const float BlendAmount, const float AlphaBlend, inout float Remaining )
{
	float Result = 0.0;
   
	if ( Remaining > 0.0 && BlendAmount > 0.0 )
	{
		float minb = max( 0.0, BlendEnd - BlendStart );
		float maxb = min( 1.0, BlendEnd + BlendStart );

		if ( minb != maxb )
		{
			Result = smoothstep( minb, maxb, BlendAmount );
		}
		else if ( BlendAmount >= minb )
		{
			Result = 1.0;
		}

	  	if ( BlendEnd < AlphaBlend )
	  	{
	  		float alpha = 2.0 - AlphaBlend - BlendEnd;
	  		Result *= clamp( alpha, 0.0, 1.0 );
   		}
	}
   
	Result = clamp( Result, 0.0, Remaining );
	Remaining -= Result;
   
	return Result;
}

#endif