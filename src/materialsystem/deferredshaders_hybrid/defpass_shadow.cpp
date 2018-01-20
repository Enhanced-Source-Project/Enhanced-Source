
#include "deferred_includes.h"

#include "shadowpass_vs30.inc"
#include "shadowpass_ps30.inc"

#include "tier0/memdbgon.h"

static CCommandBufferBuilder< CFixedCommandStorageBuffer< 512 > > tmpBuf;

void InitParmsShadowPass( const defParms_shadow &info, CBaseVSShader *pShader, IMaterialVar **params )
{
	if ( PARM_NO_DEFAULT( info.iAlphatestRef ) ||
		(PARM_VALID( info.iAlphatestRef ) && PARM_FLOAT( info.iAlphatestRef ) == 0.0f) )
		params[ info.iAlphatestRef ]->SetFloatValue( DEFAULT_ALPHATESTREF );

	InitIntParam( info.m_nTreeSway, params, 0 );
	InitFloatParam( info.m_nTreeSwayHeight, params, 1000.0f );
	InitFloatParam( info.m_nTreeSwayStartHeight, params, 0.1f );
	InitFloatParam( info.m_nTreeSwayRadius, params, 300.0f );
	InitFloatParam( info.m_nTreeSwayStartRadius, params, 0.2f );
	InitFloatParam( info.m_nTreeSwaySpeed, params, 1.0f );
	InitFloatParam( info.m_nTreeSwaySpeedHighWindMultiplier, params, 2.0f );
	InitFloatParam( info.m_nTreeSwayStrength, params, 10.0f );
	InitFloatParam( info.m_nTreeSwayScrumbleSpeed, params, 5.0f );
	InitFloatParam( info.m_nTreeSwayScrumbleStrength, params, 10.0f );
	InitFloatParam( info.m_nTreeSwayScrumbleFrequency, params, 12.0f );
	InitFloatParam( info.m_nTreeSwayFalloffExp, params, 1.5f );
	InitFloatParam( info.m_nTreeSwayScrumbleFalloffExp, params, 1.0f );
	InitFloatParam( info.m_nTreeSwaySpeedLerpStart, params, 3.0f );
	InitFloatParam( info.m_nTreeSwaySpeedLerpEnd, params, 6.0f );
}

void InitPassShadowPass( const defParms_shadow &info, CBaseVSShader *pShader, IMaterialVar **params )
{
	if ( PARM_DEFINED( info.iAlbedo ) )
		pShader->LoadTexture( info.iAlbedo );

	if ( PARM_DEFINED( info.iAlbedo2 ) )
		pShader->LoadTexture( info.iAlbedo2 );

	if ( PARM_DEFINED( info.iAlbedo3 ) )
		pShader->LoadTexture( info.iAlbedo3 );

	if ( PARM_DEFINED( info.iAlbedo4 ) )
		pShader->LoadTexture( info.iAlbedo4 );

	if ( PARM_DEFINED( info.nSpecTexture ) )
		pShader->LoadTexture( info.nSpecTexture );

	if ( PARM_DEFINED( info.nSpecTexture2 ) )
		pShader->LoadTexture( info.nSpecTexture2 );

	if ( PARM_DEFINED( info.nSpecTexture3 ) )
		pShader->LoadTexture( info.nSpecTexture3 );

	if ( PARM_DEFINED( info.nSpecTexture4 ) )
		pShader->LoadTexture( info.nSpecTexture4 );
}

void DrawPassShadowPass( const defParms_shadow &info, CBaseVSShader *pShader, IMaterialVar **params,
	IShaderShadow* pShaderShadow, IShaderDynamicAPI* pShaderAPI,
	VertexCompressionType_t vertexCompression, CDeferredPerMaterialContextData *pDeferredContext )
{
	const bool bModel = info.bModel;
	const bool bIsDecal = IS_FLAG_SET( MATERIAL_VAR_DECAL );
	const bool bFastVTex = g_pHardwareConfig->HasFastVertexTextures();
	const bool bNoCull = IS_FLAG_SET( MATERIAL_VAR_NOCULL );

	const bool bAlbedo = PARM_TEX( info.iAlbedo );
	const bool bAlbedo2 = PARM_TEX( info.iAlbedo2 );
	const bool bAlbedo3 = PARM_TEX( info.iAlbedo3 );
#if DEFCFG_ENABLE_RADIOSITY
	const bool bAlbedo4 = PARM_TEX( info.iAlbedo4 );
#endif

#if DEFCFG_ENABLE_RADIOSITY == 1
	const bool bHasSpec1 = ( info.nSpecTexture != -1 && params[ info.nSpecTexture ]->IsDefined() );
	const bool bHasSpec2 = ( info.nSpecTexture2 != -1 && params[ info.nSpecTexture2 ]->IsDefined() );
	const bool bHasSpec3 = ( info.nSpecTexture3 != -1 && params[ info.nSpecTexture3 ]->IsDefined() );
	const bool bHasSpec4 = ( info.nSpecTexture4 != -1 && params[ info.nSpecTexture4 ]->IsDefined() );
#endif

	const bool bAlphatest = IS_FLAG_SET( MATERIAL_VAR_ALPHATEST ) && bAlbedo;
	
#if DEFCFG_ENABLE_RADIOSITY == 1
	const bool bMultiBlend = PARM_SET( info.iMultiblend ) || bAlbedo3 || bAlbedo4;
#else
	const bool bMultiBlend = PARM_SET( info.iMultiblend ) || bAlbedo3;
#endif
	const bool bBaseTexture2 = !bMultiBlend && bAlbedo2;

	const bool bTreeSway = ( GetIntParam( info.m_nTreeSway, params, 0 ) != 0 );
	const int nTreeSwayMode = clamp( GetIntParam( info.m_nTreeSway, params, 0 ), 0, 2 );

	SHADOW_STATE
	{
		pShaderShadow->SetDefaultState();

		pShaderShadow->EnableSRGBWrite( false );

		if ( bNoCull )
		{
			pShaderShadow->EnableCulling( false );
		}

		int iVFmtFlags = VERTEX_POSITION | VERTEX_NORMAL;
		int iUserDataSize = 0;

		int *pTexCoordDim;
		int iTexCoordNum;
		GetTexcoordSettings( ( bModel && bIsDecal && bFastVTex ), bMultiBlend,
			iTexCoordNum, &pTexCoordDim );

		if ( bModel )
		{
			iVFmtFlags |= VERTEX_FORMAT_COMPRESSED;
		}

#if DEFCFG_ENABLE_RADIOSITY == 0
		if ( bAlphatest )
#endif
		{
			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );

			if ( bBaseTexture2 || bMultiBlend )
				pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );

			if ( bMultiBlend )
			{
				pShaderShadow->EnableTexture( SHADER_SAMPLER2, true );
				pShaderShadow->EnableTexture( SHADER_SAMPLER3, true );

				// Spec
				pShaderShadow->EnableTexture( SHADER_SAMPLER10, true );
				pShaderShadow->EnableTexture( SHADER_SAMPLER11, true );
				pShaderShadow->EnableTexture( SHADER_SAMPLER12, true );
				pShaderShadow->EnableTexture( SHADER_SAMPLER13, true );
			}
		}

#if DEFCFG_BILATERAL_DEPTH_TEST
		pShaderShadow->EnableTexture( SHADER_SAMPLER4, true );
#endif

		pShaderShadow->VertexShaderVertexFormat( iVFmtFlags, iTexCoordNum, pTexCoordDim, iUserDataSize );

		DECLARE_STATIC_VERTEX_SHADER( shadowpass_vs30 );
		SET_STATIC_VERTEX_SHADER_COMBO( MODEL, bModel );
		SET_STATIC_VERTEX_SHADER_COMBO( MORPHING_VTEX, bModel && bFastVTex );
		SET_STATIC_VERTEX_SHADER_COMBO( MULTITEXTURE, bMultiBlend ? 2 : bBaseTexture2 ? 1 : 0 );
		SET_STATIC_VERTEX_SHADER_COMBO( TREESWAY, bTreeSway ? nTreeSwayMode : 0 );
		SET_STATIC_VERTEX_SHADER( shadowpass_vs30 );

		DECLARE_STATIC_PIXEL_SHADER( shadowpass_ps30 );
		SET_STATIC_PIXEL_SHADER_COMBO( ALPHATEST, bAlphatest );
		SET_STATIC_PIXEL_SHADER_COMBO( MULTITEXTURE, bMultiBlend ? 2 : bBaseTexture2 ? 1 : 0 );
		SET_STATIC_PIXEL_SHADER( shadowpass_ps30 );
	}
	DYNAMIC_STATE
	{
		Assert( pDeferredContext != NULL );
		const int shadowMode = pShaderAPI->GetIntRenderingParameter( INT_RENDERPARM_DEFERRED_SHADOW_MODE );
		const int radiosityOutput = DEFCFG_ENABLE_RADIOSITY != 0
			&& pShaderAPI->GetIntRenderingParameter( INT_RENDERPARM_DEFERRED_SHADOW_RADIOSITY );

		if ( pDeferredContext->m_bMaterialVarsChanged || !pDeferredContext->HasCommands( CDeferredPerMaterialContextData::DEFSTAGE_SHADOW ) )
		{
			tmpBuf.Reset();

			if ( bAlphatest )
			{
				PARM_VALIDATE( info.iAlphatestRef );

				tmpBuf.SetPixelShaderConstant4( 0, PARM_FLOAT( info.iAlphatestRef ), 0, 0, 0 );
#if DEFCFG_ENABLE_RADIOSITY == 0
				tmpBuf.BindTexture( pShader, SHADER_SAMPLER0, info.iAlbedo );
#endif
			}

#if DEFCFG_ENABLE_RADIOSITY
			if ( bAlbedo )
				tmpBuf.BindTexture( pShader, SHADER_SAMPLER0, info.iAlbedo );
			else
				tmpBuf.BindStandardTexture( SHADER_SAMPLER0, TEXTURE_WHITE );

			if ( bBaseTexture2 || bMultiBlend )
				tmpBuf.BindTexture( pShader, SHADER_SAMPLER1, info.iAlbedo2 );

			if ( bMultiBlend )
			{
				tmpBuf.BindTexture( pShader, SHADER_SAMPLER2, info.iAlbedo3 );

				if ( bAlbedo4 )
					tmpBuf.BindTexture( pShader, SHADER_SAMPLER3, info.iAlbedo4 );
				else
					tmpBuf.BindStandardTexture( SHADER_SAMPLER3, TEXTURE_WHITE );

				// Spec
				if ( bHasSpec1 == true )
					tmpBuf.BindTexture( pShader, SHADER_SAMPLER10, info.nSpecTexture );						// Spec Map 1
				else
					tmpBuf.BindStandardTexture( SHADER_SAMPLER10, TEXTURE_BLACK );
				
				if ( bHasSpec2 == true )
					tmpBuf.BindTexture( pShader, SHADER_SAMPLER11, info.nSpecTexture2 );						// Spec Map 2
				else
					tmpBuf.BindStandardTexture( SHADER_SAMPLER11, TEXTURE_BLACK );
				
				if ( bHasSpec3 == true )
					tmpBuf.BindTexture( pShader, SHADER_SAMPLER12, info.nSpecTexture3 );						// Spec Map 3
				else
					tmpBuf.BindStandardTexture( SHADER_SAMPLER12, TEXTURE_BLACK );
				
				if ( bHasSpec4 == true )
					tmpBuf.BindTexture( pShader, SHADER_SAMPLER13, info.nSpecTexture4 );						// Spec Map 4
				else
					tmpBuf.BindStandardTexture( SHADER_SAMPLER13, TEXTURE_BLACK );

				// Scale and rotations constants of multiblend
				Vector4D	vRotations( DEG2RAD( params[ info.nRotation ]->GetFloatValue() ), DEG2RAD( params[ info.nRotation2 ]->GetFloatValue() ), 
										DEG2RAD( params[ info.nRotation3 ]->GetFloatValue() ), DEG2RAD( params[ info.nRotation4 ]->GetFloatValue() ) );
				tmpBuf.SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_12, vRotations.Base() );

				Vector4D	vScales( params[ info.nScale ]->GetFloatValue() > 0.0f ? params[ info.nScale ]->GetFloatValue() : 1.0f, 
										params[ info.nScale2 ]->GetFloatValue() > 0.0f ? params[ info.nScale2 ]->GetFloatValue() : 1.0f, 
										params[ info.nScale3 ]->GetFloatValue() > 0.0f ? params[ info.nScale3 ]->GetFloatValue() : 1.0f, 
										params[ info.nScale4 ]->GetFloatValue() > 0.0f ? params[ info.nScale4 ]->GetFloatValue() : 1.0f );
				tmpBuf.SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_13, vScales.Base() );
			}
#endif

			if ( bTreeSway )
			{
				float flParams[4];
				flParams[0] = GetFloatParam( info.m_nTreeSwaySpeedHighWindMultiplier, params, 2.0f );
				flParams[1] = GetFloatParam( info.m_nTreeSwayScrumbleFalloffExp, params, 1.0f );
				flParams[2] = GetFloatParam( info.m_nTreeSwayFalloffExp, params, 1.0f );
				flParams[3] = GetFloatParam( info.m_nTreeSwayScrumbleSpeed, params, 3.0f );
				tmpBuf.SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_3, flParams );

				flParams[0] = GetFloatParam( info.m_nTreeSwayHeight, params, 1000.0f );
				flParams[1] = GetFloatParam( info.m_nTreeSwayStartHeight, params, 0.1f );
				flParams[2] = GetFloatParam( info.m_nTreeSwayRadius, params, 300.0f );
				flParams[3] = GetFloatParam( info.m_nTreeSwayStartRadius, params, 0.2f );
				tmpBuf.SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_5, flParams );

				flParams[0] = GetFloatParam( info.m_nTreeSwaySpeed, params, 1.0f );
				flParams[1] = GetFloatParam( info.m_nTreeSwayStrength, params, 10.0f );
				flParams[2] = GetFloatParam( info.m_nTreeSwayScrumbleFrequency, params, 12.0f );
				flParams[3] = GetFloatParam( info.m_nTreeSwayScrumbleStrength, params, 10.0f );
				tmpBuf.SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_6, flParams );

				flParams[0] = GetFloatParam( info.m_nTreeSwaySpeedLerpStart, params, 3.0f );
				flParams[1] = GetFloatParam( info.m_nTreeSwaySpeedLerpEnd, params, 6.0f );
				tmpBuf.SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_14, flParams );
			}

			tmpBuf.End();

			pDeferredContext->SetCommands( CDeferredPerMaterialContextData::DEFSTAGE_SHADOW, tmpBuf.Copy() );
		}

		pShaderAPI->SetDefaultState();

#if DEFCFG_BILATERAL_DEPTH_TEST
		pShaderAPI->SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_3, GetDeferredExt()->GetWorldToCameraDepthTexBase(), 4, true );
#endif

		if ( bModel && bFastVTex )
			pShader->SetHWMorphVertexShaderState( VERTEX_SHADER_SHADER_SPECIFIC_CONST_10, VERTEX_SHADER_SHADER_SPECIFIC_CONST_11, SHADER_VERTEXTEXTURE_SAMPLER0 );

		DECLARE_DYNAMIC_VERTEX_SHADER( shadowpass_vs30 );
		SET_DYNAMIC_VERTEX_SHADER_COMBO( COMPRESSED_VERTS, (bModel && (int)vertexCompression) ? 1 : 0 );
		SET_DYNAMIC_VERTEX_SHADER_COMBO( SKINNING, (bModel && pShaderAPI->GetCurrentNumBones() > 0) ? 1 : 0 );
		SET_DYNAMIC_VERTEX_SHADER_COMBO( MORPHING, (bModel && pShaderAPI->IsHWMorphingEnabled()) ? 1 : 0 );
		SET_DYNAMIC_VERTEX_SHADER_COMBO( SHADOW_MODE, shadowMode );
		SET_DYNAMIC_VERTEX_SHADER_COMBO( RADIOSITY, radiosityOutput );
		SET_DYNAMIC_VERTEX_SHADER( shadowpass_vs30 );

		DECLARE_DYNAMIC_PIXEL_SHADER( shadowpass_ps30 );
		SET_DYNAMIC_PIXEL_SHADER_COMBO( SHADOW_MODE, shadowMode );
		SET_DYNAMIC_PIXEL_SHADER_COMBO( RADIOSITY, radiosityOutput );
		SET_DYNAMIC_PIXEL_SHADER( shadowpass_ps30 );

		if ( bModel && bFastVTex )
		{
			bool bUnusedTexCoords[3] = { false, true, !pShaderAPI->IsHWMorphingEnabled() || !bIsDecal };
			pShaderAPI->MarkUnusedVertexFields( 0, 3, bUnusedTexCoords );
		}

		if ( bTreeSway )
		{
			float fTempConst[4];
			fTempConst[0] = 0; // unused
			fTempConst[1] = pShaderAPI->CurrentTime();
			Vector windDir = pShaderAPI->GetVectorRenderingParameter( VECTOR_RENDERPARM_WIND_DIRECTION );
			fTempConst[2] = windDir.x;
			fTempConst[3] = windDir.y;
			pShaderAPI->SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_4, fTempConst );
		}

		pShaderAPI->ExecuteCommandBuffer( pDeferredContext->GetCommands( CDeferredPerMaterialContextData::DEFSTAGE_SHADOW ) );

		switch ( shadowMode )
		{
		case DEFERRED_SHADOW_MODE_ORTHO:
			{
				CommitShadowcastingConstants_Ortho( pShaderAPI,
					pShaderAPI->GetIntRenderingParameter( INT_RENDERPARM_DEFERRED_SHADOW_INDEX ),
					VERTEX_SHADER_SHADER_SPECIFIC_CONST_0, VERTEX_SHADER_SHADER_SPECIFIC_CONST_1,
					VERTEX_SHADER_SHADER_SPECIFIC_CONST_2
					);
			}
			break;
		case DEFERRED_SHADOW_MODE_PROJECTED:
			{
				CommitShadowcastingConstants_Proj( pShaderAPI,
					pShaderAPI->GetIntRenderingParameter( INT_RENDERPARM_DEFERRED_SHADOW_INDEX ),
					VERTEX_SHADER_SHADER_SPECIFIC_CONST_0, VERTEX_SHADER_SHADER_SPECIFIC_CONST_1,
					VERTEX_SHADER_SHADER_SPECIFIC_CONST_2
					);
			}
			break;
		}

#ifdef SHADOWMAPPING_USE_COLOR
		CommitViewVertexShader( pShaderAPI, VERTEX_SHADER_SHADER_SPECIFIC_CONST_7 );
#endif
	}

	pShader->Draw();
}