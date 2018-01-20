
#include "deferred_includes.h"

#include "defconstruct_vs30.inc"
#include "volumpass_point_ps30.inc"
#include "volumpass_spot_ps30.inc"

#include "tier0/memdbgon.h"


void InitParmsLightPassVolum( const lightPassParms &info, CBaseVSShader *pShader, IMaterialVar **params )
{
	if ( !PARM_DEFINED( info.iLightTypeVar ) )
		params[ info.iLightTypeVar ]->SetIntValue( DEFLIGHTTYPE_POINT );
}


void InitPassLightPassVolum( const lightPassParms &info, CBaseVSShader *pShader, IMaterialVar **params )
{
}

void DrawPassLightPassVolum( const lightPassParms &info, CBaseVSShader *pShader, IMaterialVar **params,
	IShaderShadow* pShaderShadow, IShaderDynamicAPI* pShaderAPI,
	VertexCompressionType_t vertexCompression )
{
	const bool bWorldProjection = PARM_SET( info.iWorldProjection );
	const int iLightType = PARM_INT( info.iLightTypeVar );

	const bool bPoint = iLightType == DEFLIGHTTYPE_POINT;

	SHADOW_STATE
	{
		pShaderShadow->SetDefaultState();
		pShaderShadow->EnableDepthTest( false );
		pShaderShadow->EnableDepthWrites( false );
		pShaderShadow->EnableAlphaWrites( false );
		pShaderShadow->EnableCulling( true );

		pShader->EnableAlphaBlending( SHADER_BLEND_ONE, SHADER_BLEND_ONE );

		pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
		pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );

		for ( int i = 0; i < FREE_LIGHT_SAMPLERS; i++ )
		{
			pShaderShadow->EnableTexture( (Sampler_t)( FIRST_LIGHT_SAMPLER + i ), true );
		}

		pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, NULL, 0 );

		DECLARE_STATIC_VERTEX_SHADER( defconstruct_vs30 );
		SET_STATIC_VERTEX_SHADER_COMBO( USEWORLDTRANSFORM, bWorldProjection ? 1 : 0 );
		SET_STATIC_VERTEX_SHADER_COMBO( SENDWORLDPOS, bWorldProjection ? 1 : 0 );
		SET_STATIC_VERTEX_SHADER( defconstruct_vs30 );

		switch ( iLightType )
		{
		case DEFLIGHTTYPE_POINT:
			{
				DECLARE_STATIC_PIXEL_SHADER( volumpass_point_ps30 );
				SET_STATIC_PIXEL_SHADER_COMBO( USEWORLDTRANSFORM, bWorldProjection ? 1 : 0 );
				SET_STATIC_PIXEL_SHADER( volumpass_point_ps30 );
			}
			break;
		case DEFLIGHTTYPE_SPOT:
			{
				DECLARE_STATIC_PIXEL_SHADER( volumpass_spot_ps30 );
				SET_STATIC_PIXEL_SHADER_COMBO( USEWORLDTRANSFORM, bWorldProjection ? 1 : 0 );
				SET_STATIC_PIXEL_SHADER( volumpass_spot_ps30 );
			}
			break;
		}
	}
	DYNAMIC_STATE
	{
		pShaderAPI->SetDefaultState();

		CDeferredExtension *pExt = GetDeferredExt();

		const volumeData_t &vData = GetDeferredExt()->GetVolumeData();

		Assert( pExt->GetActiveLightData() != NULL );
		Assert( pExt->GetActiveLights_NumRows() != NULL );

		const int iNumShadowedCookied = vData.bHasCookie ? 1:0;
		const int iNumShadowed = vData.bHasCookie ? 0:1;

#if DEFCFG_ADAPTIVE_VOLUMETRIC_LOD
		const int iVolumeLOD = vData.iLOD;
#endif
#if DEFCFG_CONFIGURABLE_VOLUMETRIC_LOD
		const int iVolumeSamples = vData.iSamples;
#endif

		Assert( (iNumShadowedCookied + iNumShadowed) == 1 );
		Assert( iNumShadowedCookied <= pExt->GetNumActiveLights_ShadowedCookied() );
		Assert( iNumShadowed <= pExt->GetNumActiveLights_Shadowed() );

		DECLARE_DYNAMIC_VERTEX_SHADER( defconstruct_vs30 );
		SET_DYNAMIC_VERTEX_SHADER( defconstruct_vs30 );

		switch ( iLightType )
		{
		case DEFLIGHTTYPE_POINT:
			{
				DECLARE_DYNAMIC_PIXEL_SHADER( volumpass_point_ps30 );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_SHADOWED_COOKIE, iNumShadowedCookied );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_SHADOWED, iNumShadowed );
#if DEFCFG_ADAPTIVE_VOLUMETRIC_LOD
				SET_DYNAMIC_PIXEL_SHADER_COMBO( VOLUME_LOD, iVolumeLOD );
#else
				SET_DYNAMIC_PIXEL_SHADER_COMBO( VOLUME_LOD, 0 );
#endif
#if DEFCFG_CONFIGURABLE_VOLUMETRIC_LOD
				SET_DYNAMIC_PIXEL_SHADER_COMBO( VOLUME_SAMPLES, iVolumeSamples );
#else
				SET_DYNAMIC_PIXEL_SHADER_COMBO( VOLUME_SAMPLES, 0 );
#endif
				SET_DYNAMIC_PIXEL_SHADER( volumpass_point_ps30 );
			}
			break;
		case DEFLIGHTTYPE_SPOT:
			{
				DECLARE_DYNAMIC_PIXEL_SHADER( volumpass_spot_ps30 );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_SHADOWED_COOKIE, iNumShadowedCookied );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_SHADOWED, iNumShadowed );
#if DEFCFG_ADAPTIVE_VOLUMETRIC_LOD
				SET_DYNAMIC_PIXEL_SHADER_COMBO( VOLUME_LOD, iVolumeLOD );
#else
				SET_DYNAMIC_PIXEL_SHADER_COMBO( VOLUME_LOD, 0 );
#endif
#if DEFCFG_CONFIGURABLE_VOLUMETRIC_LOD
				SET_DYNAMIC_PIXEL_SHADER_COMBO( VOLUME_SAMPLES, iVolumeSamples );
#else
				SET_DYNAMIC_PIXEL_SHADER_COMBO( VOLUME_SAMPLES, 0 );
#endif
				SET_DYNAMIC_PIXEL_SHADER( volumpass_spot_ps30 );
			}
			break;
		}

		pShader->BindTexture( SHADER_SAMPLER0, GetDeferredExt()->GetTexture_VolumePrePass() );
		pShader->BindTexture( SHADER_SAMPLER1, GetDeferredExt()->GetTexture_Depth() );

		int iSampler = 0;
		int iShadow = vData.iSamplerOffset;
		int iCookie = vData.iSamplerOffset;

		for ( ; iSampler < (iNumShadowedCookied*2);)
		{
			ITexture *pDepth = bPoint ? GetDeferredExt()->GetTexture_ShadowDepth_DP(iShadow) :
				GetDeferredExt()->GetTexture_ShadowDepth_Proj(iShadow);

			pShader->BindTexture( (Sampler_t)( FIRST_LIGHT_SAMPLER + iSampler ), pDepth );
			pShader->BindTexture( (Sampler_t)( FIRST_LIGHT_SAMPLER + iSampler + 1 ), GetDeferredExt()->GetTexture_Cookie(iCookie) );

			iSampler += 2;
			iShadow++;
			iCookie++;
		}

		for ( ; iSampler < (iNumShadowedCookied*2+iNumShadowed); )
		{
			ITexture *pDepth = bPoint ? GetDeferredExt()->GetTexture_ShadowDepth_DP(iShadow) :
				GetDeferredExt()->GetTexture_ShadowDepth_Proj(iShadow);

			pShader->BindTexture( (Sampler_t)( FIRST_LIGHT_SAMPLER + iSampler ), pDepth );

			iSampler++;
			iShadow++;
		}

		const int frustumReg = bWorldProjection ? 3 : VERTEX_SHADER_SHADER_SPECIFIC_CONST_0;
		CommitBaseDeferredConstants_Frustum( pShaderAPI, frustumReg, !bWorldProjection );
		CommitBaseDeferredConstants_Origin( pShaderAPI, 0 );

		pShaderAPI->SetPixelShaderConstant( FIRST_SHARED_LIGHTDATA_CONSTANT,
			pExt->GetActiveLightData() + vData.iDataOffset,
			vData.iNumRows );

		if ( bWorldProjection )
		{
			CommitHalfScreenTexel( pShaderAPI, 6 );
		}
	}

	pShader->Draw();
}