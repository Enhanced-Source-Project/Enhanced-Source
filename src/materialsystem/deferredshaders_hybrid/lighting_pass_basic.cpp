
#include "deferred_includes.h"

#include "defconstruct_vs30.inc"
#include "lightingpass_point_ps30.inc"
#include "lightingpass_spot_ps30.inc"
#include "shaderapi\ishaderapi.h"

#include "tier0/memdbgon.h"


void InitParmsLightPass( const lightPassParms &info, CBaseVSShader *pShader, IMaterialVar **params )
{
	if ( !PARM_DEFINED( info.iLightTypeVar ) )
		params[ info.iLightTypeVar ]->SetIntValue( DEFLIGHTTYPE_POINT );
}


void InitPassLightPass( const lightPassParms &info, CBaseVSShader *pShader, IMaterialVar **params )
{
}


void DrawPassLightPass( const lightPassParms &info, CBaseVSShader *pShader, IMaterialVar **params,
	IShaderShadow* pShaderShadow, IShaderDynamicAPI* pShaderAPI,
	VertexCompressionType_t vertexCompression )
{
	const int bWorldProjection = PARM_SET( info.iWorldProjection );

	const int iLightType = PARM_INT( info.iLightTypeVar );
	const bool bPoint = iLightType == DEFLIGHTTYPE_POINT;

	SHADOW_STATE
	{
		//NEED TO CLEAR STENCIL
		//First set stencil to reflect where lit geometry lies in front of the light geometry. Set to 1 where depth test fails drawing only back faces.
		/*ShaderStencilState_t lightGeometryStencil1;
		lightGeometryStencil.m_bEnable = true;
		lightGeometryStencil.m_nReferenceValue = 1;
		lightGeometryStencil.m_nWriteMask = 1;
		lightGeometryStencil.m_CompareFunc = ShaderStencilFunc_t::SHADER_STENCILFUNC_NEVER;
		lightGeometryStencil.m_FailOp = ShaderStencilOp_t::SHADER_STENCILOP_KEEP;
		lightGeometryStencil.m_ZFailOp = ShaderStencilOp_t::SHADER_STENCILOP_SET_TO_REFERENCE;*/

		//Then draw only where depth test succeeds and stencil buffer has been set to 1.
		/*ShaderStencilState_t lightGeometryStencil2;
		lightGeometryStencil2.m_bEnable = true;
		lightGeometryStencil2.m_nTestMask = 1;
		lightGeometryStencil2.m_nWriteMask = 0;
		lightGeometryStencil2.m_CompareFunc = ShaderStencilFunc_t::SHADER_STENCILFUNC_EQUAL;
		lightGeometryStencil2.m_PassOp = ShaderStencilOp_t::SHADER_STENCILOP_ZERO;
		lightGeometryStencil2.m_FailOp = ShaderStencilOp_t::SHADER_STENCILOP_KEEP;
		lightGeometryStencil2.m_ZFailOp = ShaderStencilOp_t::SHADER_STENCILOP_SET_TO_REFERENCE;*/

		/*
		pShaderShadow->SetDefaultState();
		pShaderShadow->EnableDepthTest( true );
		pShaderShadow->DepthFunc( SHADER_DEPTHFUNC_FARTHEROREQUAL );
		pShaderShadow->PolyMode( SHADER_POLYMODEFACE_FRONT, SHADER_POLYMODE_FILL );
		pShaderShadow->EnableDepthWrites( false );
		pShaderShadow->EnableAlphaWrites( true );
		*/

		pShaderShadow->SetDefaultState();
		pShaderShadow->EnableDepthTest( false );
		pShaderShadow->EnableDepthWrites( false );
		pShaderShadow->EnableAlphaWrites( true );

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
		SET_STATIC_VERTEX_SHADER_COMBO( SENDWORLDPOS, 0 );
		SET_STATIC_VERTEX_SHADER( defconstruct_vs30 );

		switch ( iLightType )
		{
		case DEFLIGHTTYPE_POINT:
			{
				DECLARE_STATIC_PIXEL_SHADER( lightingpass_point_ps30 );
				SET_STATIC_PIXEL_SHADER_COMBO( USEWORLDTRANSFORM, bWorldProjection ? 1 : 0 );
				SET_STATIC_PIXEL_SHADER( lightingpass_point_ps30 );
			}
			break;
		case DEFLIGHTTYPE_SPOT:
			{
				DECLARE_STATIC_PIXEL_SHADER( lightingpass_spot_ps30 );
				SET_STATIC_PIXEL_SHADER_COMBO( USEWORLDTRANSFORM, bWorldProjection ? 1 : 0 );
				SET_STATIC_PIXEL_SHADER( lightingpass_spot_ps30 );
			}
			break;
		}
	}
	DYNAMIC_STATE
	{
		pShaderAPI->SetDefaultState();

		CDeferredExtension *pExt = GetDeferredExt();

		Assert( pExt->GetActiveLightData() != NULL );
		Assert( pExt->GetActiveLights_NumRows() != NULL );

		const int iNumShadowedCookied = pExt->GetNumActiveLights_ShadowedCookied();
		const int iNumShadowed = pExt->GetNumActiveLights_Shadowed();
		const int iNumCookied = pExt->GetNumActiveLights_Cookied();

		DECLARE_DYNAMIC_VERTEX_SHADER( defconstruct_vs30 );
		SET_DYNAMIC_VERTEX_SHADER( defconstruct_vs30 );

		switch ( iLightType )
		{
		case DEFLIGHTTYPE_POINT:
			{
				DECLARE_DYNAMIC_PIXEL_SHADER( lightingpass_point_ps30 );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_SHADOWED_COOKIE, iNumShadowedCookied );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_SHADOWED, iNumShadowed );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_COOKIE, iNumCookied );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_SIMPLE, pExt->GetNumActiveLights_Simple() );
				SET_DYNAMIC_PIXEL_SHADER( lightingpass_point_ps30 );
			}
			break;
		case DEFLIGHTTYPE_SPOT:
			{
				DECLARE_DYNAMIC_PIXEL_SHADER( lightingpass_spot_ps30 );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_SHADOWED_COOKIE, iNumShadowedCookied );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_SHADOWED, iNumShadowed );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_COOKIE, iNumCookied );
				SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_SIMPLE, pExt->GetNumActiveLights_Simple() );
				SET_DYNAMIC_PIXEL_SHADER( lightingpass_spot_ps30 );
			}
			break;
		}

		pShader->BindTexture( SHADER_SAMPLER0, GetDeferredExt()->GetTexture_Normals() );
		pShader->BindTexture( SHADER_SAMPLER1, GetDeferredExt()->GetTexture_Depth() );

		int iSampler = 0;
		int iShadow = 0;
		int iCookie = 0;

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

		for ( ; iSampler < (iNumShadowedCookied*2+iNumShadowed+iNumCookied); )
		{
			pShader->BindTexture( (Sampler_t)( FIRST_LIGHT_SAMPLER + iSampler ), GetDeferredExt()->GetTexture_Cookie(iCookie) );

			iSampler++;
			iCookie++;
		}

		const int frustumReg = bWorldProjection ? 3 : VERTEX_SHADER_SHADER_SPECIFIC_CONST_0;
		CommitBaseDeferredConstants_Frustum( pShaderAPI, frustumReg, !bWorldProjection );
		CommitBaseDeferredConstants_Origin( pShaderAPI, 0 );

		switch ( iLightType )
		{
		case DEFLIGHTTYPE_POINT:
				CommitShadowProjectionConstants_DPSM( pShaderAPI, 1 );
			break;
		case DEFLIGHTTYPE_SPOT:
				CommitShadowProjectionConstants_Proj( pShaderAPI, 1 );
			break;
		}

		pShaderAPI->SetPixelShaderConstant( FIRST_SHARED_LIGHTDATA_CONSTANT,
			pExt->GetActiveLightData(),
			pExt->GetActiveLights_NumRows() );

		if ( bWorldProjection )
		{
			CommitHalfScreenTexel( pShaderAPI, 6 );
		}
	}

	pShader->Draw();
}