
#include "BaseVSShader.h"
#include "deferred_includes.h"

#include "composite_vs30.inc"
#include "composite_ps30.inc"

#include "tier0/memdbgon.h"

static CCommandBufferBuilder< CFixedCommandStorageBuffer< 512 > > tmpBuf;

ConVar building_cubemaps( "building_cubemaps", "0" );

void InitParmsComposite( const defParms_composite &info, CBaseVSShader *pShader, IMaterialVar **params )
{
	if ( PARM_NO_DEFAULT( info.iAlphatestRef ) ||
		(PARM_VALID( info.iAlphatestRef ) && PARM_FLOAT( info.iAlphatestRef ) == 0.0f) )
		params[ info.iAlphatestRef ]->SetFloatValue( DEFAULT_ALPHATESTREF );

	PARM_INIT_FLOAT( info.iPhongScale, DEFAULT_PHONG_SCALE );
	PARM_INIT_INT( info.iPhongFresnel, 0 );

	PARM_INIT_FLOAT( info.iEnvmapContrast, 0.0f );
	PARM_INIT_FLOAT( info.iEnvmapSaturation, 1.0f );
	PARM_INIT_VEC3( info.iEnvmapTint, 1.0f, 1.0f, 1.0f );
	PARM_INIT_INT( info.iEnvmapFresnel, 0 );

	PARM_INIT_INT( info.iRimlightEnable, 0 );
	PARM_INIT_FLOAT( info.iRimlightExponent, 4.0f );
	PARM_INIT_FLOAT( info.iRimlightAlbedoScale, 0.0f );
	PARM_INIT_VEC3( info.iRimlightTint, 1.0f, 1.0f, 1.0f );
	PARM_INIT_INT( info.iRimlightModLight, 0 );

	PARM_INIT_VEC3( info.iSelfIllumTint, 1.0f, 1.0f, 1.0f );
	PARM_INIT_INT( info.iSelfIllumMaskInEnvmapAlpha, 0 );
	PARM_INIT_INT( info.iSelfIllumFresnelModulate, 0 );
}

void InitPassComposite( const defParms_composite &info, CBaseVSShader *pShader, IMaterialVar **params )
{
	if ( PARM_DEFINED( info.iAlbedo ) )
		pShader->LoadTexture( info.iAlbedo );

	if ( PARM_DEFINED( info.iEnvmap ) )
		pShader->LoadCubeMap( info.iEnvmap );

	if ( PARM_DEFINED( info.iEnvmapMask ) )
		pShader->LoadTexture( info.iEnvmapMask );

	if ( PARM_DEFINED( info.iAlbedo2 ) )
		pShader->LoadTexture( info.iAlbedo2 );

	if ( PARM_DEFINED( info.iAlbedo3 ) )
		pShader->LoadTexture( info.iAlbedo3 );

	if ( PARM_DEFINED( info.iAlbedo4 ) )
		pShader->LoadTexture( info.iAlbedo4 );

	if ( PARM_DEFINED( info.iBlendmodulate ) )
		pShader->LoadTexture( info.iBlendmodulate );

#if 0
	if ( PARM_DEFINED( info.iBlendmodulate2 ) )
		pShader->LoadTexture( info.iBlendmodulate2 );

	if ( PARM_DEFINED( info.iBlendmodulate3 ) )
		pShader->LoadTexture( info.iBlendmodulate3 );
#endif // 0

	if ( PARM_DEFINED( info.nSpecTexture ) )
		pShader->LoadTexture( info.nSpecTexture );

	if ( PARM_DEFINED( info.nSpecTexture2 ) )
		pShader->LoadTexture( info.nSpecTexture2 );

	if ( PARM_DEFINED( info.nSpecTexture3 ) )
		pShader->LoadTexture( info.nSpecTexture3 );

	if ( PARM_DEFINED( info.nSpecTexture4 ) )
		pShader->LoadTexture( info.nSpecTexture4 );

	if ( PARM_DEFINED( info.iSelfIllumMask ) )
		pShader->LoadTexture( info.iSelfIllumMask );
}

void DrawPassComposite( const defParms_composite &info, CBaseVSShader *pShader, IMaterialVar **params,
	IShaderShadow* pShaderShadow, IShaderDynamicAPI* pShaderAPI,
	VertexCompressionType_t vertexCompression, CDeferredPerMaterialContextData *pDeferredContext )
{
	const bool bModel = info.bModel;
	const bool bIsDecal = IS_FLAG_SET( MATERIAL_VAR_DECAL );
	const bool bFastVTex = g_pHardwareConfig->HasFastVertexTextures();

	const bool bAlbedo = PARM_TEX( info.iAlbedo );
	const bool bAlbedo2 = !bModel && bAlbedo && PARM_TEX( info.iAlbedo2 );
	const bool bAlbedo3 = !bModel && bAlbedo && PARM_TEX( info.iAlbedo3 );
	const bool bAlbedo4 = !bModel && bAlbedo && PARM_TEX( info.iAlbedo4 );

	const bool bAlphatest = IS_FLAG_SET( MATERIAL_VAR_ALPHATEST ) && bAlbedo;
	const bool bTranslucent = IS_FLAG_SET( MATERIAL_VAR_TRANSLUCENT ) && bAlbedo && !bAlphatest;

	const bool bNoCull = IS_FLAG_SET( MATERIAL_VAR_NOCULL );

	const bool bUseSRGB = DEFCFG_USE_SRGB_CONVERSION != 0;
	const bool bPhongFresnel = PARM_SET( info.iPhongFresnel );

	const bool bEnvmap = PARM_TEX( info.iEnvmap );
	const bool bEnvmapMask = bEnvmap && PARM_TEX( info.iEnvmapMask );
	const bool bEnvmapMask2 = bEnvmapMask && PARM_TEX( info.iEnvmapMask2 );
	const bool bEnvmapFresnel = bEnvmap && PARM_SET( info.iEnvmapFresnel );

	const bool bRimLight = PARM_SET( info.iRimlightEnable );
	const bool bRimLightModLight = bRimLight && PARM_SET( info.iRimlightModLight );

	const bool bBlendmodulate = bAlbedo2 && PARM_TEX( info.iBlendmodulate );
#if 0
	const bool bBlendmodulate2 = bBlendmodulate && PARM_TEX( info.iBlendmodulate2 );
	const bool bBlendmodulate3 = bBlendmodulate && PARM_TEX( info.iBlendmodulate3 );
#endif // 0

	const bool bHasSpec1 = ( info.nSpecTexture != -1 && params[ info.nSpecTexture ]->IsDefined() );
	const bool bHasSpec2 = ( info.nSpecTexture2 != -1 && params[ info.nSpecTexture2 ]->IsDefined() );
	const bool bHasSpec3 = ( info.nSpecTexture3 != -1 && params[ info.nSpecTexture3 ]->IsDefined() );
	const bool bHasSpec4 = ( info.nSpecTexture4 != -1 && params[ info.nSpecTexture4 ]->IsDefined() );

	const bool bSelfIllum = !bAlbedo2 && IS_FLAG_SET( MATERIAL_VAR_SELFILLUM );
	const bool bSelfIllumMaskInEnvmapMask = bSelfIllum && bEnvmapMask && PARM_SET( info.iSelfIllumMaskInEnvmapAlpha );
	const bool bSelfIllumMask = bSelfIllum && !bSelfIllumMaskInEnvmapMask && !bEnvmapMask && PARM_TEX( info.iSelfIllumMask );

	//const bool bMultiBlend = PARM_SET( info.iMultiblend )
	//	&& bAlbedo && bAlbedo2 && bAlbedo3 && !bEnvmapMask && !bSelfIllumMask;
	const bool bMultiBlend = PARM_SET( info.iMultiblend )|| bAlbedo3 || bAlbedo4 || bHasSpec3 || bHasSpec4;
	//const bool bWorldVertexTransition = !bMultiBlend && bAlbedo2;

	const bool bNeedsFresnel = bPhongFresnel || bEnvmapFresnel;
	const bool bGBufferNormal = bEnvmap || bRimLight || bNeedsFresnel;
	const bool bWorldEyeVec = bGBufferNormal;

	AssertMsgOnce( !(bTranslucent || bAlphatest) || !bAlbedo2,
		"blended albedo not supported by gbuffer pass!" );

	AssertMsgOnce( IS_FLAG_SET( MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK ) == false,
		"Normal map sampling should stay out of composition pass." );

	AssertMsgOnce( !PARM_TEX( info.iSelfIllumMask ) || !bEnvmapMask,
		"Can't use separate selfillum mask with envmap mask - use SELFILLUM_ENVMAPMASK_ALPHA instead." );

	AssertMsgOnce( PARM_SET( info.iMultiblend ) == bMultiBlend,
		"Multiblend forced off due to invalid usage! May cause vertexformat mis-matches between passes." );


	SHADOW_STATE
	{
		pShaderShadow->SetDefaultState();
		pShaderShadow->EnableSRGBWrite( bUseSRGB );

		if ( bNoCull )
		{
			pShaderShadow->EnableCulling( false );
		}

		int iVFmtFlags = VERTEX_POSITION;
		int iUserDataSize = 0;

		int *pTexCoordDim;
		int iTexCoordNum;
		GetTexcoordSettings( ( bModel && bIsDecal && bFastVTex ), bMultiBlend,
			iTexCoordNum, &pTexCoordDim );

		if ( bModel )
		{
			iVFmtFlags |= VERTEX_NORMAL;
			iVFmtFlags |= VERTEX_FORMAT_COMPRESSED;
		}
		else
		{
			if ( bAlbedo2 )
				iVFmtFlags |= VERTEX_COLOR;
		}

		pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
		pShaderShadow->EnableSRGBRead( SHADER_SAMPLER0, bUseSRGB );

		if ( bGBufferNormal )
		{
			pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );
			pShaderShadow->EnableSRGBRead( SHADER_SAMPLER1, false );
		}

		if ( bTranslucent )
		{
			pShader->EnableAlphaBlending( SHADER_BLEND_SRC_ALPHA, SHADER_BLEND_ONE_MINUS_SRC_ALPHA );
		}

		pShaderShadow->EnableTexture( SHADER_SAMPLER2, true );
		pShaderShadow->EnableSRGBRead( SHADER_SAMPLER2, false );

		if ( bEnvmap )
		{
			pShaderShadow->EnableTexture( SHADER_SAMPLER3, true );

			if( g_pHardwareConfig->GetHDRType() == HDR_TYPE_NONE )
				pShaderShadow->EnableSRGBRead( SHADER_SAMPLER3, true );

			if ( bEnvmapMask )
			{
				pShaderShadow->EnableTexture( SHADER_SAMPLER4, true );

				if ( bAlbedo2 )
					pShaderShadow->EnableTexture( SHADER_SAMPLER7, true );
			}
		}
		else if ( bSelfIllumMask )
		{
			pShaderShadow->EnableTexture( SHADER_SAMPLER4, true );
		}

		if ( bAlbedo2 )
		{
			pShaderShadow->EnableTexture( SHADER_SAMPLER5, true );
			pShaderShadow->EnableSRGBRead( SHADER_SAMPLER5, bUseSRGB );

			if ( bBlendmodulate )
				pShaderShadow->EnableTexture( SHADER_SAMPLER9, true );
		}

		if ( bMultiBlend )
		{
			pShaderShadow->EnableTexture( SHADER_SAMPLER7, true );
			pShaderShadow->EnableSRGBRead( SHADER_SAMPLER7, bUseSRGB );

			pShaderShadow->EnableTexture( SHADER_SAMPLER8, true );
			pShaderShadow->EnableSRGBRead( SHADER_SAMPLER8, bUseSRGB );

			pShaderShadow->EnableTexture( SHADER_SAMPLER9, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER10, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER13, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER14, true );
		}

		pShaderShadow->EnableAlphaWrites( false );
		pShaderShadow->EnableDepthWrites( !bTranslucent );

		pShader->DefaultFog();

		pShaderShadow->VertexShaderVertexFormat( iVFmtFlags, iTexCoordNum, pTexCoordDim, iUserDataSize );

		DECLARE_STATIC_VERTEX_SHADER( composite_vs30 );
		SET_STATIC_VERTEX_SHADER_COMBO( MODEL, bModel );
		SET_STATIC_VERTEX_SHADER_COMBO( MORPHING_VTEX, bModel && bFastVTex );
		SET_STATIC_VERTEX_SHADER_COMBO( DECAL, bModel && bIsDecal );
		SET_STATIC_VERTEX_SHADER_COMBO( EYEVEC, bWorldEyeVec );
		SET_STATIC_VERTEX_SHADER_COMBO( BASETEXTURE2, bAlbedo2 && !bMultiBlend );
		SET_STATIC_VERTEX_SHADER_COMBO( BLENDMODULATE, bBlendmodulate );
		SET_STATIC_VERTEX_SHADER_COMBO( MULTIBLEND, bMultiBlend );
		SET_STATIC_VERTEX_SHADER_COMBO( FOW, 0 );
		SET_STATIC_VERTEX_SHADER( composite_vs30 );

		DECLARE_STATIC_PIXEL_SHADER( composite_ps30 );
		SET_STATIC_PIXEL_SHADER_COMBO( ALPHATEST, bAlphatest );
		SET_STATIC_PIXEL_SHADER_COMBO( TRANSLUCENT, bTranslucent );
		SET_STATIC_PIXEL_SHADER_COMBO( READNORMAL, bGBufferNormal );
		SET_STATIC_PIXEL_SHADER_COMBO( NOCULL, bNoCull );
		SET_STATIC_PIXEL_SHADER_COMBO( ENVMAP, bEnvmap );
		SET_STATIC_PIXEL_SHADER_COMBO( ENVMAPMASK, bEnvmapMask );
		SET_STATIC_PIXEL_SHADER_COMBO( ENVMAPFRESNEL, bEnvmapFresnel );
		SET_STATIC_PIXEL_SHADER_COMBO( PHONGFRESNEL, bPhongFresnel );
		SET_STATIC_PIXEL_SHADER_COMBO( RIMLIGHT, bRimLight );
		SET_STATIC_PIXEL_SHADER_COMBO( RIMLIGHTMODULATELIGHT, bRimLightModLight );
		SET_STATIC_PIXEL_SHADER_COMBO( BASETEXTURE2, bAlbedo2 && !bMultiBlend );
		SET_STATIC_PIXEL_SHADER_COMBO( BLENDMODULATE, bBlendmodulate );
		SET_STATIC_PIXEL_SHADER_COMBO( MULTIBLEND, bMultiBlend );
		SET_STATIC_PIXEL_SHADER_COMBO( SELFILLUM, bSelfIllum );
		SET_STATIC_PIXEL_SHADER_COMBO( SELFILLUM_MASK, bSelfIllumMask );
		SET_STATIC_PIXEL_SHADER_COMBO( SELFILLUM_ENVMAP_ALPHA, bSelfIllumMaskInEnvmapMask );
		SET_STATIC_PIXEL_SHADER( composite_ps30 );
	}
	DYNAMIC_STATE
	{
		Assert( pDeferredContext != NULL );

		if ( pDeferredContext->m_bMaterialVarsChanged || !pDeferredContext->HasCommands( CDeferredPerMaterialContextData::DEFSTAGE_COMPOSITE )
			|| building_cubemaps.GetBool() )
		{
			tmpBuf.Reset();

			if ( bAlphatest )
			{
				PARM_VALIDATE( info.iAlphatestRef );
				tmpBuf.SetPixelShaderConstant4( 0, PARM_FLOAT( info.iAlphatestRef ), 0, 0, 0 );
			}

			if ( bAlbedo )
				tmpBuf.BindTexture( pShader, SHADER_SAMPLER0, info.iAlbedo );
			else
				tmpBuf.BindStandardTexture( SHADER_SAMPLER0, TEXTURE_GREY );

			if ( bEnvmap )
			{
				if ( building_cubemaps.GetBool() )
					tmpBuf.BindStandardTexture( SHADER_SAMPLER3, TEXTURE_BLACK );
				else
				{
					if ( PARM_TEX( info.iEnvmap ) && !bModel )
						tmpBuf.BindTexture( pShader, SHADER_SAMPLER3, info.iEnvmap );
					else
						tmpBuf.BindStandardTexture( SHADER_SAMPLER3, TEXTURE_LOCAL_ENV_CUBEMAP );
				}

				if ( bEnvmapMask )
					tmpBuf.BindTexture( pShader, SHADER_SAMPLER4, info.iEnvmapMask );

				if ( bAlbedo2 )
				{
					if ( bEnvmapMask2 )
						tmpBuf.BindTexture( pShader, SHADER_SAMPLER7, info.iEnvmapMask2 );
					else
						tmpBuf.BindStandardTexture( SHADER_SAMPLER7, TEXTURE_WHITE );
				}

				tmpBuf.SetPixelShaderConstant( 5, info.iEnvmapTint );

				float fl6[4] = { 0 };
				fl6[0] = PARM_FLOAT( info.iEnvmapSaturation );
				fl6[1] = PARM_FLOAT( info.iEnvmapContrast );
				tmpBuf.SetPixelShaderConstant( 6, fl6 );
			}

			if ( bNeedsFresnel )
			{
				tmpBuf.SetPixelShaderConstant( 7, info.iFresnelRanges );
			}

			if ( bRimLight )
			{
				float fl9[4] = { 0 };
				fl9[0] = PARM_FLOAT( info.iRimlightExponent );
				fl9[1] = PARM_FLOAT( info.iRimlightAlbedoScale );
				tmpBuf.SetPixelShaderConstant( 9, fl9 );
			}

			if ( bAlbedo2 )
			{
				tmpBuf.BindTexture( pShader, SHADER_SAMPLER5, info.iAlbedo2 );

				if ( bBlendmodulate )
				{
					tmpBuf.SetVertexShaderTextureTransform( VERTEX_SHADER_SHADER_SPECIFIC_CONST_1, info.iBlendmodulateTransform );
					tmpBuf.BindTexture( pShader, SHADER_SAMPLER9, info.iBlendmodulate );
				}
			}

			if ( bMultiBlend )
			{
				if( bAlbedo3 )
					tmpBuf.BindTexture( pShader, SHADER_SAMPLER7, info.iAlbedo3 );
				else
					tmpBuf.BindStandardTexture( SHADER_SAMPLER7, TEXTURE_WHITE );

				if ( bAlbedo4 )
					tmpBuf.BindTexture( pShader, SHADER_SAMPLER8, info.iAlbedo4 );
				else
					tmpBuf.BindStandardTexture( SHADER_SAMPLER8, TEXTURE_WHITE );

				if ( bHasSpec1 == true )
					tmpBuf.BindTexture( pShader, SHADER_SAMPLER9, info.nSpecTexture );						// Spec Map 1
				else
					tmpBuf.BindStandardTexture( SHADER_SAMPLER9, TEXTURE_BLACK );
				
				if ( bHasSpec2 == true )
					tmpBuf.BindTexture( pShader, SHADER_SAMPLER10, info.nSpecTexture2 );						// Spec Map 2
				else
					tmpBuf.BindStandardTexture( SHADER_SAMPLER10, TEXTURE_BLACK );
				
				if ( bHasSpec3 == true )
					tmpBuf.BindTexture( pShader, SHADER_SAMPLER13, info.nSpecTexture3 );						// Spec Map 3
				else
					tmpBuf.BindStandardTexture( SHADER_SAMPLER13, TEXTURE_BLACK );
				
				if ( bHasSpec4 == true )
					tmpBuf.BindTexture( pShader, SHADER_SAMPLER14, info.nSpecTexture4 );						// Spec Map 4
				else
					tmpBuf.BindStandardTexture( SHADER_SAMPLER14, TEXTURE_BLACK );

				// Scale and rotations constants of multiblend
				Vector4D	vRotations( DEG2RAD( params[ info.nRotation ]->GetFloatValue() ), DEG2RAD( params[ info.nRotation2 ]->GetFloatValue() ), 
										DEG2RAD( params[ info.nRotation3 ]->GetFloatValue() ), DEG2RAD( params[ info.nRotation4 ]->GetFloatValue() ) );
				tmpBuf.SetVertexShaderConstant( 27, vRotations.Base() );

				Vector4D	vScales( params[ info.nScale ]->GetFloatValue() > 0.0f ? params[ info.nScale ]->GetFloatValue() : 1.0f, 
										params[ info.nScale2 ]->GetFloatValue() > 0.0f ? params[ info.nScale2 ]->GetFloatValue() : 1.0f, 
										params[ info.nScale3 ]->GetFloatValue() > 0.0f ? params[ info.nScale3 ]->GetFloatValue() : 1.0f, 
										params[ info.nScale4 ]->GetFloatValue() > 0.0f ? params[ info.nScale4 ]->GetFloatValue() : 1.0f );
				tmpBuf.SetVertexShaderConstant( 28, vScales.Base() );
			}

			if ( bSelfIllum && bSelfIllumMask )
			{
				tmpBuf.BindTexture( pShader, SHADER_SAMPLER4, info.iSelfIllumMask );
			}

			int x, y, w, t;
			pShaderAPI->GetCurrentViewport( x, y, w, t );
			float fl1[4] = { 1.0f / w, 1.0f / t, 0, 0 };

			tmpBuf.SetPixelShaderConstant( 1, fl1 );

			tmpBuf.SetPixelShaderFogParams( 2 );

			float fl4 = { PARM_FLOAT( info.iPhongScale ) };
			tmpBuf.SetPixelShaderConstant4( 4, fl4, 0, 0, 0 );

			tmpBuf.End();

			pDeferredContext->SetCommands( CDeferredPerMaterialContextData::DEFSTAGE_COMPOSITE, tmpBuf.Copy() );
		}

		pShaderAPI->SetDefaultState();

		if ( bModel && bFastVTex )
			pShader->SetHWMorphVertexShaderState( VERTEX_SHADER_SHADER_SPECIFIC_CONST_10, VERTEX_SHADER_SHADER_SPECIFIC_CONST_11, SHADER_VERTEXTEXTURE_SAMPLER0 );
		
		DECLARE_DYNAMIC_VERTEX_SHADER( composite_vs30 );
		SET_DYNAMIC_VERTEX_SHADER_COMBO( COMPRESSED_VERTS, (bModel && (int)vertexCompression) ? 1 : 0 );
		SET_DYNAMIC_VERTEX_SHADER_COMBO( SKINNING, (bModel && pShaderAPI->GetCurrentNumBones() > 0) ? 1 : 0 );
		SET_DYNAMIC_VERTEX_SHADER_COMBO( MORPHING, (bModel && pShaderAPI->IsHWMorphingEnabled()) ? 1 : 0 );
		SET_DYNAMIC_VERTEX_SHADER( composite_vs30 );

		DECLARE_DYNAMIC_PIXEL_SHADER( composite_ps30 );
		SET_DYNAMIC_PIXEL_SHADER_COMBO( PIXELFOGTYPE, pShaderAPI->GetPixelFogCombo() );
		SET_DYNAMIC_PIXEL_SHADER( composite_ps30 );

		if ( bModel && bFastVTex )
		{
			bool bUnusedTexCoords[3] = { false, true, !pShaderAPI->IsHWMorphingEnabled() || !bIsDecal };
			pShaderAPI->MarkUnusedVertexFields( 0, 3, bUnusedTexCoords );
		}

		pShaderAPI->ExecuteCommandBuffer( pDeferredContext->GetCommands( CDeferredPerMaterialContextData::DEFSTAGE_COMPOSITE ) );

		if ( bGBufferNormal )
			pShader->BindTexture( SHADER_SAMPLER1, GetDeferredExt()->GetTexture_Normals() );

		pShader->BindTexture( SHADER_SAMPLER2, GetDeferredExt()->GetTexture_LightAccum() );

		CommitBaseDeferredConstants_Origin( pShaderAPI, 3 );

		if ( bWorldEyeVec )
		{
			float vEyepos[4] = {0,0,0,0};
			pShaderAPI->GetWorldSpaceCameraPosition( vEyepos );
			pShaderAPI->SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_0, vEyepos );
		}

		if ( bRimLight )
		{
			pShaderAPI->SetPixelShaderConstant( 8, params[ info.iRimlightTint ]->GetVecValue() );
		}

		if ( bSelfIllum )
		{
			pShaderAPI->SetPixelShaderConstant( 10, params[ info.iSelfIllumTint ]->GetVecValue() );
		}
	}

	pShader->Draw();
}