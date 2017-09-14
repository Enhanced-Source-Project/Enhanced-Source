//========= Copyright � 1996-2007, Valve LLC, All rights reserved. ============
//
// Purpose: Lightmap only shader
//
// $Header: $
// $NoKeywords: $
//=============================================================================

#include "lightmappedgeneric_dx9_helper.h"
#include "BaseVSShader.h"
#include "shaderlib/commandbuilder.h"
#include "convar.h"

#include "lightmappedgeneric_vs30.inc"
#include "lightmappedgeneric_ps30.inc"

#include "lightmappedgeneric_deferred_vs30.inc"
#include "lightmappedgeneric_deferred_ps30.inc"

#include "tier0/vprof.h"

#include "../materialsystem_global.h"
#include "shaderapi/ishaderutil.h"

#include "tier0/memdbgon.h"

ConVar mat_disable_lightwarp( "mat_disable_lightwarp", "0" );
ConVar mat_disable_fancy_blending( "mat_disable_fancy_blending", "0" );
ConVar mat_fullbright( "mat_fullbright","0", FCVAR_CHEAT );

ConVar mat_ambient_light_r( "mat_ambient_light_r", "0.0", FCVAR_CHEAT );
ConVar mat_ambient_light_g( "mat_ambient_light_g", "0.0", FCVAR_CHEAT );
ConVar mat_ambient_light_b( "mat_ambient_light_b", "0.0", FCVAR_CHEAT );

extern ConVar r_twopasspaint;



static ConVar mat_force_vertexfog( "mat_force_vertexfog", "0", FCVAR_DEVELOPMENTONLY );

void InitParamsLightmappedGeneric_DX9( CBaseVSShader *pShader, IMaterialVar** params, const char *pMaterialName, LightmappedGeneric_DX9_Vars_t &info )
{
	// Override vertex fog via the global setting if it isn't enabled/disabled in the material file.
	if ( !IS_FLAG_DEFINED( MATERIAL_VAR_VERTEXFOG ) && mat_force_vertexfog.GetBool() )
	{
		SET_FLAGS( MATERIAL_VAR_VERTEXFOG );
	}

	params[FLASHLIGHTTEXTURE]->SetStringValue( GetFlashlightTextureFilename() );

	// Write over $basetexture with $albedo if we are going to be using diffuse normal mapping.
	if( g_pConfig->UseBumpmapping() && params[info.m_nBumpmap]->IsDefined() && params[info.m_nAlbedo]->IsDefined() &&
		params[info.m_nBaseTexture]->IsDefined() && 
		!( params[info.m_nNoDiffuseBumpLighting]->IsDefined() && params[info.m_nNoDiffuseBumpLighting]->GetIntValue() ) )
	{
		params[info.m_nBaseTexture]->SetStringValue( params[info.m_nAlbedo]->GetStringValue() );
	}

	if( pShader->IsUsingGraphics() && params[info.m_nEnvmap]->IsDefined() && !pShader->CanUseEditorMaterials() )
	{
		if( stricmp( params[info.m_nEnvmap]->GetStringValue(), "env_cubemap" ) == 0 )
		{
			Warning( "env_cubemap used on world geometry without rebuilding map. . ignoring: %s\n", pMaterialName );
			params[info.m_nEnvmap]->SetUndefined();
		}
	}
	
	if ( (mat_disable_lightwarp.GetBool() ) &&
		 (info.m_nLightWarpTexture != -1) )
	{
		params[info.m_nLightWarpTexture]->SetUndefined();
	}
	if ( (mat_disable_fancy_blending.GetBool() ) &&
		 (info.m_nBlendModulateTexture != -1) )
	{
		params[info.m_nBlendModulateTexture]->SetUndefined();
	}

	if( !params[info.m_nEnvmapTint]->IsDefined() )
		params[info.m_nEnvmapTint]->SetVecValue( 1.0f, 1.0f, 1.0f );

	if( !params[info.m_nNoDiffuseBumpLighting]->IsDefined() )
		params[info.m_nNoDiffuseBumpLighting]->SetIntValue( 0 );

	if( !params[info.m_nSelfIllumTint]->IsDefined() )
		params[info.m_nSelfIllumTint]->SetVecValue( 1.0f, 1.0f, 1.0f );

	if( !params[info.m_nDetailScale]->IsDefined() )
		params[info.m_nDetailScale]->SetFloatValue( 4.0f );

	if ( !params[info.m_nDetailTint]->IsDefined() )
		params[info.m_nDetailTint]->SetVecValue( 1.0f, 1.0f, 1.0f, 1.0f );

	InitFloatParam( info.m_nDetailTextureBlendFactor, params, 1.0 );
	InitIntParam( info.m_nDetailTextureCombineMode, params, 0 );

	if( !params[info.m_nFresnelReflection]->IsDefined() )
		params[info.m_nFresnelReflection]->SetFloatValue( 1.0f );

	if( !params[info.m_nEnvmapMaskFrame]->IsDefined() )
		params[info.m_nEnvmapMaskFrame]->SetIntValue( 0 );
	
	if( !params[info.m_nEnvmapFrame]->IsDefined() )
		params[info.m_nEnvmapFrame]->SetIntValue( 0 );

	if( !params[info.m_nBumpFrame]->IsDefined() )
		params[info.m_nBumpFrame]->SetIntValue( 0 );

	if( !params[info.m_nDetailFrame]->IsDefined() )
		params[info.m_nDetailFrame]->SetIntValue( 0 );

	if( !params[info.m_nEnvmapContrast]->IsDefined() )
		params[info.m_nEnvmapContrast]->SetFloatValue( 0.0f );
	
	if( !params[info.m_nEnvmapSaturation]->IsDefined() )
		params[info.m_nEnvmapSaturation]->SetFloatValue( 1.0f );
	
	InitFloatParam( info.m_nAlphaTestReference, params, 0.0f );

	// No texture means no self-illum or env mask in base alpha
	if ( !params[info.m_nBaseTexture]->IsDefined() )
	{
		CLEAR_FLAGS( MATERIAL_VAR_SELFILLUM );
		CLEAR_FLAGS( MATERIAL_VAR_BASEALPHAENVMAPMASK );
	}

	if( params[info.m_nBumpmap]->IsDefined() )
	{
		params[info.m_nEnvmapMask]->SetUndefined();
	}
	
	// If in decal mode, no debug override...
	if (IS_FLAG_SET(MATERIAL_VAR_DECAL))
	{
		SET_FLAGS( MATERIAL_VAR_NO_DEBUG_OVERRIDE );
	}

	SET_FLAGS2( MATERIAL_VAR2_LIGHTING_LIGHTMAP );
	if( g_pConfig->UseBumpmapping() && params[info.m_nBumpmap]->IsDefined() && (params[info.m_nNoDiffuseBumpLighting]->GetIntValue() == 0) )
	{
		SET_FLAGS2( MATERIAL_VAR2_LIGHTING_BUMPED_LIGHTMAP );
	}

	// If mat_specular 0, then get rid of envmap
	if( !g_pConfig->UseSpecular() && params[info.m_nEnvmap]->IsDefined() && params[info.m_nBaseTexture]->IsDefined() )
	{
		params[info.m_nEnvmap]->SetUndefined();
	}

	if( !params[info.m_nBaseTextureNoEnvmap]->IsDefined() )
	{
		params[info.m_nBaseTextureNoEnvmap]->SetIntValue( 0 );
	}
	if( !params[info.m_nBaseTexture2NoEnvmap]->IsDefined() )
	{
		params[info.m_nBaseTexture2NoEnvmap]->SetIntValue( 0 );
	}

	if( ( info.m_nSelfShadowedBumpFlag != -1 ) &&
		( !params[info.m_nSelfShadowedBumpFlag]->IsDefined() )
		)
	{
		params[info.m_nSelfShadowedBumpFlag]->SetIntValue( 0 );
	}
	// handle line art parms
	InitFloatParam( info.m_nEdgeSoftnessStart, params, 0.5 );
	InitFloatParam( info.m_nEdgeSoftnessEnd, params, 0.5 );
	InitFloatParam( info.m_nOutlineAlpha, params, 1.0 );

	// parallax mapping
	InitFloatParam( info.m_nHeightScale, params, 0.1 );

	// srgb read 360
	InitIntParam( info.m_nShaderSrgbRead360, params, 0 );

	InitFloatParam( info.m_nEnvMapLightScale, params, 0.0f );

	if ( g_pConfig->m_bPaintInGame )
	{
		if( info.m_nPaintSplatNormal != -1 )
		{
			if( !params[info.m_nPaintSplatNormal]->IsDefined() )
			{
				params[info.m_nPaintSplatNormal]->SetStringValue( "paint/splatnormal_default" );
			}
		}
		
		if( info.m_nPaintSplatEnvMap != -1 )
		{
			if( !params[info.m_nPaintSplatEnvMap]->IsDefined() )
			{
				if( params[info.m_nEnvmap]->IsDefined() )
				{
					params[info.m_nPaintSplatEnvMap]->SetStringValue( params[info.m_nEnvmap]->GetStringValue() );
				}
				else
				{
					params[info.m_nPaintSplatEnvMap]->SetStringValue( "env_cubemap" );
				}
			}
		}
	}
	else
	{
		if( info.m_nPaintSplatNormal != -1 )
		{
			params[info.m_nPaintSplatNormal]->SetUndefined();
		}

		if( info.m_nPaintSplatEnvMap != -1 )
		{
			params[info.m_nPaintSplatEnvMap]->SetUndefined();
		}
	}
}

void InitLightmappedGeneric_DX9( CBaseVSShader *pShader, IMaterialVar** params, LightmappedGeneric_DX9_Vars_t &info )
{
	bool hasNormalMapAlphaEnvmapMask = g_pConfig->UseSpecular() && IS_FLAG_SET( MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK );

	if ( ( g_pConfig->UseBumpmapping() || hasNormalMapAlphaEnvmapMask ) && params[info.m_nBumpmap]->IsDefined() )
	{
		pShader->LoadBumpMap( info.m_nBumpmap );
	}

	if ( g_pConfig->UseBumpmapping() && params[info.m_nBumpmap2]->IsDefined() )
	{
		pShader->LoadBumpMap( info.m_nBumpmap2 );
	}

	if ( g_pConfig->UseBumpmapping() && params[info.m_nBumpMask]->IsDefined() )
	{
		pShader->LoadBumpMap( info.m_nBumpMask );
	}

	if (params[info.m_nBaseTexture]->IsDefined())
	{
		pShader->LoadTexture( info.m_nBaseTexture );

		if (!params[info.m_nBaseTexture]->GetTextureValue()->IsTranslucent())
		{
			CLEAR_FLAGS( MATERIAL_VAR_SELFILLUM );
			CLEAR_FLAGS( MATERIAL_VAR_BASEALPHAENVMAPMASK );
		}
	}

	if (params[info.m_nBaseTexture2]->IsDefined() )
	{
		pShader->LoadTexture( info.m_nBaseTexture2 );
	}

	if (params[info.m_nBaseTexture2]->IsDefined() )
	{
		pShader->LoadTexture( info.m_nBaseTexture2 );
	}

	if ((info.m_nBlendModulateTexture != -1) &&
		(params[info.m_nBlendModulateTexture]->IsDefined()) )
	{
		pShader->LoadTexture( info.m_nBlendModulateTexture );
	}

	if (params[info.m_nDetail]->IsDefined())
	{
		int nDetailBlendMode = ( info.m_nDetailTextureCombineMode == -1 ) ? 0 : params[info.m_nDetailTextureCombineMode]->GetIntValue();
		nDetailBlendMode = nDetailBlendMode > 1 ? 1 : nDetailBlendMode;

		if ( nDetailBlendMode == 0 ) //Mod2X
			pShader->LoadTexture( info.m_nDetail );
		else
			pShader->LoadTexture( info.m_nDetail );
	}

	pShader->LoadTexture( info.m_nFlashlightTexture );
	
	// Don't alpha test if the alpha channel is used for other purposes
	if (IS_FLAG_SET(MATERIAL_VAR_SELFILLUM) || IS_FLAG_SET(MATERIAL_VAR_BASEALPHAENVMAPMASK) )
	{
		CLEAR_FLAGS( MATERIAL_VAR_ALPHATEST );
	}
		
	if ( g_pConfig->UseSpecular() )
	{
		if ( params[info.m_nEnvmap]->IsDefined() )
		{
			pShader->LoadCubeMap( info.m_nEnvmap );

			if (params[info.m_nEnvmapMask]->IsDefined())
			{
				pShader->LoadTexture( info.m_nEnvmapMask );
			}
		}
		else
		{
			params[info.m_nEnvmapMask]->SetUndefined();
		}
	}
	else
	{
		params[info.m_nEnvmap]->SetUndefined();
		params[info.m_nEnvmapMask]->SetUndefined();
	}

	// We always need this because of the flashlight.
	SET_FLAGS2( MATERIAL_VAR2_NEEDS_TANGENT_SPACES );

	if( IS_PARAM_DEFINED( info.m_nPaintSplatNormal ) )
	{
		pShader->LoadBumpMap( info.m_nPaintSplatNormal );
	}

	if( IS_PARAM_DEFINED( info.m_nPaintSplatEnvMap ) )
	{
		pShader->LoadCubeMap( info.m_nPaintSplatEnvMap );
	}

	if ( IS_PARAM_DEFINED( info.m_nOffsetTexture ) )
	{
		pShader->LoadTexture( info.m_nOffsetTexture );
	}
}

void DrawLightmappedGeneric_DX9( CBaseVSShader *pShader, IMaterialVar** params, IShaderDynamicAPI *pShaderAPI, IShaderShadow* pShaderShadow, 
								 LightmappedGeneric_DX9_Vars_t &info, CBasePerMaterialContextData **pContextDataPtr, bool bDeferredActive )
{
	bool bSinglePassFlashlight = true;
	bool hasFlashlight = !bDeferredActive && pShader->UsingFlashlight( params );

	CLightmappedGeneric_DX9_Context *pContextData = reinterpret_cast< CLightmappedGeneric_DX9_Context *> ( *pContextDataPtr );
	bool bShaderSrgbRead = ( IsX360() && IS_PARAM_DEFINED( info.m_nShaderSrgbRead360 ) && params[info.m_nShaderSrgbRead360]->GetIntValue() );

	if ( pShaderShadow || ( ! pContextData )|| pContextData->m_bMaterialVarsChanged || pContextData->m_bNeedsCmdRegen || ( hasFlashlight && !IsX360() ) )
	{
		bool hasBaseTexture = params[info.m_nBaseTexture]->IsTexture();
		int nAlphaChannelTextureVar = hasBaseTexture ? (int)info.m_nBaseTexture : (int)info.m_nEnvmapMask;
		BlendType_t nBlendType = pShader->EvaluateBlendRequirements( nAlphaChannelTextureVar, hasBaseTexture );
		bool bIsAlphaTested = IS_FLAG_SET( MATERIAL_VAR_ALPHATEST ) != 0;
		bool bFullyOpaqueWithoutAlphaTest = (nBlendType != BT_BLENDADD) && (nBlendType != BT_BLEND) && (!hasFlashlight || IsX360()); //dest alpha is free for special use
		bool bFullyOpaque = bFullyOpaqueWithoutAlphaTest && !bIsAlphaTested;
		bool bNeedRegenStaticCmds = (! pContextData ) || pShaderShadow || pContextData->m_bNeedsCmdRegen;

		if ( ! pContextData )								// make sure allocated
		{
			pContextData = new CLightmappedGeneric_DX9_Context;
			*pContextDataPtr = pContextData;
		}

		bool hasBump = ( params[info.m_nBumpmap]->IsTexture() ) && g_pConfig->UseBumpmapping();
		bool hasSSBump = hasBump && (info.m_nSelfShadowedBumpFlag != -1) &&	( params[info.m_nSelfShadowedBumpFlag]->GetIntValue() );
		bool hasBaseTexture2 = hasBaseTexture && params[info.m_nBaseTexture2]->IsTexture();
		bool hasLightWarpTexture = params[info.m_nLightWarpTexture]->IsTexture();
		bool hasBump2 = hasBump && params[info.m_nBumpmap2]->IsTexture();
		bool hasDetailTexture = params[info.m_nDetail]->IsTexture();
		bool hasSelfIllum = IS_FLAG_SET( MATERIAL_VAR_SELFILLUM );
		bool hasBumpMask = hasBump && hasBump2 && params[info.m_nBumpMask]->IsTexture() && !hasSelfIllum &&
			!hasDetailTexture && !hasBaseTexture2 && (params[info.m_nBaseTextureNoEnvmap]->GetIntValue() == 0);
		bool bHasBlendModulateTexture = 
			(info.m_nBlendModulateTexture != -1) &&
			(params[info.m_nBlendModulateTexture]->IsTexture() );
		bool hasNormalMapAlphaEnvmapMask = g_pConfig->UseSpecular() && IS_FLAG_SET( MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK );

		if( g_pConfig->bEditMode )
		{
			hasBump = false;
			hasBump2 = false;
		}

		bool bParallaxMapping = false;
		// L4D: no parallax mapping
		/*
		if ( g_pHardwareConfig->SupportsPixelShaders_2_b() )
			bParallaxMapping = ( info.m_nParallaxMap != -1 ) && ( params[info.m_nParallaxMap]->GetIntValue() != 0 );
		*/

		if ( hasFlashlight && !IsX360() )				
		{
			// !!speed!! do this in the caller so we don't build struct every time
			CBaseVSShader::DrawFlashlight_dx90_Vars_t vars;
			vars.m_bBump = hasBump;
			vars.m_nBumpmapVar = info.m_nBumpmap;
			vars.m_nBumpmapFrame = info.m_nBumpFrame;
			vars.m_nBumpTransform = info.m_nBumpTransform;
			vars.m_nFlashlightTextureVar = info.m_nFlashlightTexture;
			vars.m_nFlashlightTextureFrameVar = info.m_nFlashlightTextureFrame;
			vars.m_bLightmappedGeneric = true;
			vars.m_bWorldVertexTransition = hasBaseTexture2;
			vars.m_nBaseTexture2Var = info.m_nBaseTexture2;
			vars.m_nBaseTexture2FrameVar = info.m_nBaseTexture2Frame;
			vars.m_nBumpmap2Var = info.m_nBumpmap2;
			vars.m_nBumpmap2Frame = info.m_nBumpFrame2;
			vars.m_nBump2Transform = info.m_nBumpTransform2;
			vars.m_nAlphaTestReference = info.m_nAlphaTestReference;
			vars.m_bSSBump = hasSSBump;
			vars.m_nDetailVar = info.m_nDetail;
			vars.m_nDetailScale = info.m_nDetailScale;
			vars.m_nDetailTextureCombineMode = info.m_nDetailTextureCombineMode;
			vars.m_nDetailTextureBlendFactor = info.m_nDetailTextureBlendFactor;
			vars.m_nDetailTint = info.m_nDetailTint;

			if ( ( info.m_nSeamlessMappingScale != -1 ) )
				vars.m_fSeamlessScale = params[info.m_nSeamlessMappingScale]->GetFloatValue();
			else
				vars.m_fSeamlessScale = 0.0;

			pShader->DrawFlashlight_dx90( params, pShaderAPI, pShaderShadow, vars );
			return;
		}

		pContextData->m_bFullyOpaque = bFullyOpaque;
		pContextData->m_bFullyOpaqueWithoutAlphaTest = bFullyOpaqueWithoutAlphaTest;

		bool bHasOutline = IsBoolSet( info.m_nOutline, params );
		pContextData->m_bPixelShaderForceFastPathBecauseOutline = bHasOutline;
		bool bHasSoftEdges = IsBoolSet( info.m_nSoftEdges, params );
		bool hasEnvmapMask = params[info.m_nEnvmapMask]->IsTexture();
		
		
		float fDetailBlendFactor = GetFloatParam( info.m_nDetailTextureBlendFactor, params, 1.0 );

		if ( pShaderShadow || bNeedRegenStaticCmds )
		{
			bool hasVertexColor = IS_FLAG_SET( MATERIAL_VAR_VERTEXCOLOR );
			bool hasDiffuseBumpmap = hasBump && (params[info.m_nNoDiffuseBumpLighting]->GetIntValue() == 0);

			bool hasEnvmap = params[info.m_nEnvmap]->IsTexture();
			int envmap_variant; //0 = no envmap, 1 = regular, 2 = darken in shadow mode
			if( hasEnvmap )
			{
				//only enabled darkened cubemap mode when the scale calls for it. And not supported in ps20 when also using a 2nd bumpmap
				envmap_variant = ((GetFloatParam( info.m_nEnvMapLightScale, params ) > 0.0f) && (g_pHardwareConfig->SupportsPixelShaders_2_b() || !hasBump2)) ? 2 : 1;
			}
			else
			{
				envmap_variant = 0; 
			}

			bool bSeamlessMapping = ( ( info.m_nSeamlessMappingScale != -1 ) && 
									  ( params[info.m_nSeamlessMappingScale]->GetFloatValue() != 0.0 ) );
			
			if ( bNeedRegenStaticCmds )
			{
				pContextData->m_bNeedsCmdRegen = false;

				pContextData->ResetStaticCmds();
				CCommandBufferBuilder< CFixedCommandStorageBuffer< 5000 > > staticCmdsBuf;

				int nLightingPreviewMode = IS_FLAG2_SET( MATERIAL_VAR2_USE_GBUFFER0 ) + 2 * IS_FLAG2_SET( MATERIAL_VAR2_USE_GBUFFER1 );
				if ( ( nLightingPreviewMode == ENABLE_FIXED_LIGHTING_OUTPUTNORMAL_AND_DEPTH ) && IsPC() )
				{
					staticCmdsBuf.SetVertexShaderNearAndFarZ( VERTEX_SHADER_SHADER_SPECIFIC_CONST_6 );	// Needed for SSAO
				}

				if( !hasBaseTexture )
				{
					if( hasEnvmap )
					{
						// if we only have an envmap (no basetexture), then we want the albedo to be black.
						staticCmdsBuf.BindStandardTexture( SHADER_SAMPLER0, TEXTURE_BLACK );
					}
					else
					{
						staticCmdsBuf.BindStandardTexture( SHADER_SAMPLER0, TEXTURE_WHITE );
					}
				}
				staticCmdsBuf.BindStandardTexture( SHADER_SAMPLER1, TEXTURE_LIGHTMAP );

				if ( g_pConfig->m_bPaintInGame && !r_twopasspaint.GetBool() )
				{
					staticCmdsBuf.BindStandardTexture( SHADER_SAMPLER9, TEXTURE_PAINT );
				}

				if ( bSeamlessMapping )
				{
					staticCmdsBuf.SetVertexShaderConstant4(
						VERTEX_SHADER_SHADER_SPECIFIC_CONST_0,
						params[info.m_nSeamlessMappingScale]->GetFloatValue(),0,0,0 );
				}
				staticCmdsBuf.StoreEyePosInPixelShaderConstant( 10 );
				staticCmdsBuf.SetPixelShaderFogParams( 11 );
				
				staticCmdsBuf.End();
				// now, copy buf
				pContextData->m_pStaticCmds = new uint8[staticCmdsBuf.Size()];
				memcpy( pContextData->m_pStaticCmds, staticCmdsBuf.Base(), staticCmdsBuf.Size() );
			}
			if ( pShaderShadow )
			{

				// Alpha test: FIXME: shouldn't this be handled in Shader_t::SetInitialShadowState
				pShaderShadow->EnableAlphaTest( bIsAlphaTested );
				if ( info.m_nAlphaTestReference != -1 && params[info.m_nAlphaTestReference]->GetFloatValue() > 0.0f )
				{
					pShaderShadow->AlphaFunc( SHADER_ALPHAFUNC_GEQUAL, params[info.m_nAlphaTestReference]->GetFloatValue() );
				}

				pShader->SetDefaultBlendingShadowState( nAlphaChannelTextureVar, hasBaseTexture );

				unsigned int flags = VERTEX_POSITION;

				// base texture
				pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
				pShaderShadow->EnableSRGBRead( SHADER_SAMPLER0, !bShaderSrgbRead );

				if ( g_pConfig->m_bPaintInGame && !r_twopasspaint.GetBool() )
				{
					pShaderShadow->EnableTexture( SHADER_SAMPLER9, true );
					pShaderShadow->EnableSRGBRead( SHADER_SAMPLER9, !bShaderSrgbRead );
				}

				if ( hasLightWarpTexture )
				{
					pShaderShadow->EnableTexture( SHADER_SAMPLER6, true );
					pShaderShadow->EnableSRGBRead( SHADER_SAMPLER6, false );
				}
				if ( bHasBlendModulateTexture )
				{
					pShaderShadow->EnableTexture( SHADER_SAMPLER3, true );
					pShaderShadow->EnableSRGBRead( SHADER_SAMPLER3, false );
				}

				if ( hasBaseTexture2 )
				{
					pShaderShadow->EnableTexture( SHADER_SAMPLER7, true );
					pShaderShadow->EnableSRGBRead( SHADER_SAMPLER7, !bShaderSrgbRead );
				}
//		if( hasLightmap )
				pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );
				if( g_pHardwareConfig->GetHDRType() == HDR_TYPE_NONE )
				{
					pShaderShadow->EnableSRGBRead( SHADER_SAMPLER1, true );
				}
				else
				{
					pShaderShadow->EnableSRGBRead( SHADER_SAMPLER1, false );
				}

				if( hasEnvmap || ( IsX360() && hasFlashlight ) )
				{
					if( hasEnvmap )
					{
						pShaderShadow->EnableTexture( SHADER_SAMPLER2, true );
						if( g_pHardwareConfig->GetHDRType() == HDR_TYPE_NONE )
						{
							pShaderShadow->EnableSRGBRead( SHADER_SAMPLER2, true );
						}
					}
					flags |= VERTEX_TANGENT_S | VERTEX_TANGENT_T | VERTEX_NORMAL;
				}

				pShaderShadow->EnableVertexTexture( SHADER_VERTEXTEXTURE_SAMPLER0, true );

#define TCOMBINE_NONE 12									// there is no detail texture

				int nDetailBlendMode = TCOMBINE_NONE;

				if ( hasDetailTexture )
				{
					nDetailBlendMode = GetIntParam( info.m_nDetailTextureCombineMode, params );
					ITexture *pDetailTexture = params[info.m_nDetail]->GetTextureValue();
					if ( pDetailTexture->GetFlags() & TEXTUREFLAGS_SSBUMP )
					{
						if ( hasBump )
							nDetailBlendMode = 10;					// ssbump
						else
							nDetailBlendMode = 11;					// ssbump_nobump
					}
					pShaderShadow->EnableTexture( SHADER_SAMPLER12, true );
					bool bSRGBState = ( nDetailBlendMode == 1 );
					pShaderShadow->EnableSRGBRead( SHADER_SAMPLER12, bSRGBState );
				}

				// Hijack detail blend mode 9 for paint (this blend mode was previously skipped/unused in lightmappedgeneric)
				if ( g_pConfig->m_bPaintInGame && !r_twopasspaint.GetBool() )
				{
					nDetailBlendMode = 9;
				}
				
				if( hasBump || hasNormalMapAlphaEnvmapMask )
				{
					pShaderShadow->EnableTexture( SHADER_SAMPLER4, true );
				}
				if( hasBump2 )
				{
					pShaderShadow->EnableTexture( SHADER_SAMPLER5, true );
				}
				if( hasBumpMask )
				{
					pShaderShadow->EnableTexture( SHADER_SAMPLER8, true );
				}
				if( hasEnvmapMask )
				{
					pShaderShadow->EnableTexture( SHADER_SAMPLER5, true );
				}

				if( bDeferredActive )
				{
					pShaderShadow->EnableTexture( SHADER_SAMPLER14, true );
					pShaderShadow->EnableTexture( SHADER_SAMPLER15, true );
				}

				if( hasFlashlight && IsX360() )
				{
					pShaderShadow->EnableTexture( SHADER_SAMPLER13, true );
					pShaderShadow->EnableTexture( SHADER_SAMPLER14, true );
					pShaderShadow->SetShadowDepthFiltering( SHADER_SAMPLER14 );
					pShaderShadow->EnableTexture( SHADER_SAMPLER15, true );
				}

				if( hasVertexColor || hasBaseTexture2 || hasBump2 )
				{
					flags |= VERTEX_COLOR;
				}

				// texcoord0 : base texcoord
				// texcoord1 : lightmap texcoord
				// texcoord2 : lightmap texcoord offset
				int numTexCoords;
				
				// if ( pShaderAPI->InEditorMode() )
// 				if ( pShader->CanUseEditorMaterials() )
// 				{
// 					numTexCoords = 1;
// 				}
// 				else
				{
					numTexCoords = 2;
					if( hasBump )
					{
						numTexCoords = 3;
					}
				}
		
				int nLightingPreviewMode = IS_FLAG2_SET( MATERIAL_VAR2_USE_GBUFFER0 ) + 2 * IS_FLAG2_SET( MATERIAL_VAR2_USE_GBUFFER1 );

				pShaderShadow->VertexShaderVertexFormat( flags, numTexCoords, 0, 0 );

				// Pre-cache pixel shaders
				bool hasBaseAlphaEnvmapMask = IS_FLAG_SET( MATERIAL_VAR_BASEALPHAENVMAPMASK );

				int bumpmap_variant=(hasSSBump) ? 2 : hasBump;
				bool bMaskedBlending=( (info.m_nMaskedBlending != -1) &&
									   (params[info.m_nMaskedBlending]->GetIntValue() != 0) );

				if( bDeferredActive )
				{
					DECLARE_STATIC_VERTEX_SHADER( lightmappedgeneric_deferred_vs30 );
					SET_STATIC_VERTEX_SHADER_COMBO( ENVMAP_MASK,  hasEnvmapMask );
					SET_STATIC_VERTEX_SHADER_COMBO( TANGENTSPACE,  params[info.m_nEnvmap]->IsTexture() );
					SET_STATIC_VERTEX_SHADER_COMBO( BUMPMAP,  hasBump );
					SET_STATIC_VERTEX_SHADER_COMBO( DIFFUSEBUMPMAP, hasDiffuseBumpmap );
					SET_STATIC_VERTEX_SHADER_COMBO( VERTEXCOLOR, IS_FLAG_SET( MATERIAL_VAR_VERTEXCOLOR ) );
					SET_STATIC_VERTEX_SHADER_COMBO( VERTEXALPHATEXBLENDFACTOR, hasBaseTexture2 || hasBump2 );
					SET_STATIC_VERTEX_SHADER_COMBO( BUMPMASK, hasBumpMask );
					SET_STATIC_VERTEX_SHADER_COMBO( LIGHTING_PREVIEW, nLightingPreviewMode );
					SET_STATIC_VERTEX_SHADER_COMBO( PARALLAX_MAPPING, bParallaxMapping );
					SET_STATIC_VERTEX_SHADER_COMBO( SEAMLESS, bSeamlessMapping );
					SET_STATIC_VERTEX_SHADER_COMBO( DETAILTEXTURE, hasDetailTexture );
					SET_STATIC_VERTEX_SHADER_COMBO( FANCY_BLENDING, bHasBlendModulateTexture );
					SET_STATIC_VERTEX_SHADER_COMBO( SELFILLUM,  hasSelfIllum );
					SET_STATIC_VERTEX_SHADER( lightmappedgeneric_deferred_vs30 );

					DECLARE_STATIC_PIXEL_SHADER( lightmappedgeneric_deferred_ps30 );
					SET_STATIC_PIXEL_SHADER_COMBO( BASETEXTURE2, hasBaseTexture2 );
					SET_STATIC_PIXEL_SHADER_COMBO( BUMPMAP,  bumpmap_variant );
					SET_STATIC_PIXEL_SHADER_COMBO( BUMPMAP2, hasBump2 );
					SET_STATIC_PIXEL_SHADER_COMBO( BUMPMASK, hasBumpMask );
					SET_STATIC_PIXEL_SHADER_COMBO( DIFFUSEBUMPMAP,  hasDiffuseBumpmap );
					SET_STATIC_PIXEL_SHADER_COMBO( CUBEMAP,  envmap_variant );
					SET_STATIC_PIXEL_SHADER_COMBO( ENVMAPMASK,  hasEnvmapMask );
					SET_STATIC_PIXEL_SHADER_COMBO( BASEALPHAENVMAPMASK,  hasBaseAlphaEnvmapMask );
					SET_STATIC_PIXEL_SHADER_COMBO( SELFILLUM,  hasSelfIllum );
					SET_STATIC_PIXEL_SHADER_COMBO( NORMALMAPALPHAENVMAPMASK,  hasNormalMapAlphaEnvmapMask );
					SET_STATIC_PIXEL_SHADER_COMBO( BASETEXTURENOENVMAP,  params[info.m_nBaseTextureNoEnvmap]->GetIntValue() );
					SET_STATIC_PIXEL_SHADER_COMBO( BASETEXTURE2NOENVMAP, params[info.m_nBaseTexture2NoEnvmap]->GetIntValue() );
					SET_STATIC_PIXEL_SHADER_COMBO( WARPLIGHTING, hasLightWarpTexture );
					SET_STATIC_PIXEL_SHADER_COMBO( FANCY_BLENDING, bHasBlendModulateTexture );
					SET_STATIC_PIXEL_SHADER_COMBO( MASKEDBLENDING, bMaskedBlending);
					SET_STATIC_PIXEL_SHADER_COMBO( SEAMLESS, bSeamlessMapping );
					SET_STATIC_PIXEL_SHADER_COMBO( OUTLINE, bHasOutline );
					SET_STATIC_PIXEL_SHADER_COMBO( SOFTEDGES, bHasSoftEdges );
					SET_STATIC_PIXEL_SHADER_COMBO( DETAILTEXTURE, hasDetailTexture );
					SET_STATIC_PIXEL_SHADER_COMBO( DETAIL_BLEND_MODE, nDetailBlendMode );
					SET_STATIC_PIXEL_SHADER_COMBO( PARALLAX_MAPPING, bParallaxMapping );
					SET_STATIC_PIXEL_SHADER_COMBO( SHADER_SRGB_READ, bShaderSrgbRead );
					SET_STATIC_PIXEL_SHADER_COMBO( LIGHTING_PREVIEW, nLightingPreviewMode );
					SET_STATIC_PIXEL_SHADER( lightmappedgeneric_deferred_ps30 );
				}
				else
				{
					DECLARE_STATIC_VERTEX_SHADER( lightmappedgeneric_vs30 );
					SET_STATIC_VERTEX_SHADER_COMBO( ENVMAP_MASK,  hasEnvmapMask );
					SET_STATIC_VERTEX_SHADER_COMBO( TANGENTSPACE,  params[info.m_nEnvmap]->IsTexture() );
					SET_STATIC_VERTEX_SHADER_COMBO( BUMPMAP,  hasBump );
					SET_STATIC_VERTEX_SHADER_COMBO( DIFFUSEBUMPMAP, hasDiffuseBumpmap );
					SET_STATIC_VERTEX_SHADER_COMBO( VERTEXCOLOR, IS_FLAG_SET( MATERIAL_VAR_VERTEXCOLOR ) );
					SET_STATIC_VERTEX_SHADER_COMBO( VERTEXALPHATEXBLENDFACTOR, hasBaseTexture2 || hasBump2 );
					SET_STATIC_VERTEX_SHADER_COMBO( BUMPMASK, hasBumpMask );
					SET_STATIC_VERTEX_SHADER_COMBO( LIGHTING_PREVIEW, nLightingPreviewMode );
					SET_STATIC_VERTEX_SHADER_COMBO( PARALLAX_MAPPING, bParallaxMapping );
					SET_STATIC_VERTEX_SHADER_COMBO( SEAMLESS, bSeamlessMapping );
					SET_STATIC_VERTEX_SHADER_COMBO( DETAILTEXTURE, hasDetailTexture );
					SET_STATIC_VERTEX_SHADER_COMBO( FANCY_BLENDING, bHasBlendModulateTexture );
					SET_STATIC_VERTEX_SHADER_COMBO( SELFILLUM,  hasSelfIllum );
					SET_STATIC_VERTEX_SHADER( lightmappedgeneric_vs30 );

					DECLARE_STATIC_PIXEL_SHADER( lightmappedgeneric_ps30 );
					SET_STATIC_PIXEL_SHADER_COMBO( BASETEXTURE2, hasBaseTexture2 );
					SET_STATIC_PIXEL_SHADER_COMBO( BUMPMAP,  bumpmap_variant );
					SET_STATIC_PIXEL_SHADER_COMBO( BUMPMAP2, hasBump2 );
					SET_STATIC_PIXEL_SHADER_COMBO( BUMPMASK, hasBumpMask );
					SET_STATIC_PIXEL_SHADER_COMBO( DIFFUSEBUMPMAP,  hasDiffuseBumpmap );
					SET_STATIC_PIXEL_SHADER_COMBO( CUBEMAP,  envmap_variant );
					SET_STATIC_PIXEL_SHADER_COMBO( ENVMAPMASK,  hasEnvmapMask );
					SET_STATIC_PIXEL_SHADER_COMBO( BASEALPHAENVMAPMASK,  hasBaseAlphaEnvmapMask );
					SET_STATIC_PIXEL_SHADER_COMBO( SELFILLUM,  hasSelfIllum );
					SET_STATIC_PIXEL_SHADER_COMBO( NORMALMAPALPHAENVMAPMASK,  hasNormalMapAlphaEnvmapMask );
					SET_STATIC_PIXEL_SHADER_COMBO( BASETEXTURENOENVMAP,  params[info.m_nBaseTextureNoEnvmap]->GetIntValue() );
					SET_STATIC_PIXEL_SHADER_COMBO( BASETEXTURE2NOENVMAP, params[info.m_nBaseTexture2NoEnvmap]->GetIntValue() );
					SET_STATIC_PIXEL_SHADER_COMBO( WARPLIGHTING, hasLightWarpTexture );
					SET_STATIC_PIXEL_SHADER_COMBO( FANCY_BLENDING, bHasBlendModulateTexture );
					SET_STATIC_PIXEL_SHADER_COMBO( MASKEDBLENDING, bMaskedBlending);
					SET_STATIC_PIXEL_SHADER_COMBO( SEAMLESS, bSeamlessMapping );
					SET_STATIC_PIXEL_SHADER_COMBO( OUTLINE, bHasOutline );
					SET_STATIC_PIXEL_SHADER_COMBO( SOFTEDGES, bHasSoftEdges );
					SET_STATIC_PIXEL_SHADER_COMBO( DETAILTEXTURE, hasDetailTexture );
					SET_STATIC_PIXEL_SHADER_COMBO( DETAIL_BLEND_MODE, nDetailBlendMode );
					//SET_STATIC_PIXEL_SHADER_COMBO( PARALLAX_MAPPING, bParallaxMapping );
					_pshIndex.SetSHADER_SRGB_READ( bShaderSrgbRead );
					SET_STATIC_PIXEL_SHADER_COMBO( LIGHTING_PREVIEW, nLightingPreviewMode );
					SET_STATIC_PIXEL_SHADER( lightmappedgeneric_ps30 );
				}

				// HACK HACK HACK - enable alpha writes all the time so that we have them for
				// underwater stuff and writing depth to dest alpha
				// But only do it if we're not using the alpha already for translucency
				pShaderShadow->EnableAlphaWrites( bFullyOpaque );

				pShaderShadow->EnableSRGBWrite( true );

				pShader->DefaultFog();

				// NOTE: This isn't optimal. If $color2 is ever changed by a material
				// proxy, this code won't get re-run, but too bad. No time to make this work
				// Also note that if the lightmap scale factor changes
				// all shadow state blocks will be re-run, so that's ok
				float flLScale = pShaderShadow->GetLightMapScaleFactor();
				pShader->PI_BeginCommandBuffer();
				pShader->PI_SetModulationPixelShaderDynamicState( 21 );

				// MAINTOL4DMERGEFIXME
				// Need to reflect this change which is from this rel changelist since this constant set was moved from the dynamic block to here:
				// Change 578692 by Alex@alexv_rel on 2008/06/04 18:07:31
				//
				// Fix for portalareawindows in ep2 being rendered black. The color variable was being multipurposed for both the vs and ps differently where the ps doesn't care about alpha, but the vs does. Only applying the alpha2 DoD hack to the pixel shader constant where the alpha was never used in the first place and leaving alpha as is for the vs.

  				// color[3] *= ( IS_PARAM_DEFINED( info.m_nAlpha2 ) && params[ info.m_nAlpha2 ]->GetFloatValue() > 0.0f ) ? params[ info.m_nAlpha2 ]->GetFloatValue() : 1.0f;
  	  	  		// pContextData->m_SemiStaticCmdsOut.SetPixelShaderConstant( 12, color );

				pShader->PI_SetModulationPixelShaderDynamicState_LinearScale_ScaleInW( 12, flLScale );
				pShader->PI_SetModulationVertexShaderDynamicState_LinearScale( flLScale );
				pShader->PI_EndCommandBuffer();
			} // end shadow state
		} // end shadow || regen display list

		if ( pShaderAPI && ( pContextData->m_bMaterialVarsChanged ) )
		{
			// need to regenerate the semistatic cmds
			pContextData->m_SemiStaticCmdsOut.Reset();
			pContextData->m_bMaterialVarsChanged = false;

			bool bHasBlendMaskTransform= (
				(info.m_nBlendMaskTransform != -1) &&
				(info.m_nMaskedBlending != -1) &&
				(params[info.m_nMaskedBlending]->GetIntValue() ) &&
				( ! (params[info.m_nBumpTransform]->MatrixIsIdentity() ) ) );
			
			// If we don't have a texture transform, we don't have
			// to set vertex shader constants or run vertex shader instructions
			// for the texture transform.
			bool bHasTextureTransform = 
				!( params[info.m_nBaseTextureTransform]->MatrixIsIdentity() &&
				   params[info.m_nBumpTransform]->MatrixIsIdentity() &&
				   params[info.m_nBumpTransform2]->MatrixIsIdentity() &&
				   params[info.m_nEnvmapMaskTransform]->MatrixIsIdentity() );
			
			bHasTextureTransform |= bHasBlendMaskTransform;
			
			pContextData->m_bVertexShaderFastPath = !bHasTextureTransform;

			if( params[info.m_nDetail]->IsTexture() )
			{
				pContextData->m_bVertexShaderFastPath = false;
			}
			int nTransformToLoad = info.m_nBlendMaskTransform;
			if( ( hasBump || hasSSBump ) && hasDetailTexture && !hasSelfIllum && !bHasBlendModulateTexture )
			{
				nTransformToLoad = info.m_nBumpTransform;
			}
			pContextData->m_SemiStaticCmdsOut.SetVertexShaderTextureTransform( 
				VERTEX_SHADER_SHADER_SPECIFIC_CONST_10, nTransformToLoad );

			if ( ! pContextData->m_bVertexShaderFastPath )
			{
				bool bSeamlessMapping = ( ( info.m_nSeamlessMappingScale != -1 ) && 
										  ( params[info.m_nSeamlessMappingScale]->GetFloatValue() != 0.0 ) );
				bool hasEnvmapMask = params[info.m_nEnvmapMask]->IsTexture();
				if (!bSeamlessMapping )
					pContextData->m_SemiStaticCmdsOut.SetVertexShaderTextureTransform( VERTEX_SHADER_SHADER_SPECIFIC_CONST_0, info.m_nBaseTextureTransform );
				// If we have a detail texture, then the bump texcoords are the same as the base texcoords.
				if( hasBump && !hasDetailTexture )
				{
					pContextData->m_SemiStaticCmdsOut.SetVertexShaderTextureTransform( VERTEX_SHADER_SHADER_SPECIFIC_CONST_2, info.m_nBumpTransform );
				}
				if( hasEnvmapMask )
				{
					pContextData->m_SemiStaticCmdsOut.SetVertexShaderTextureTransform( VERTEX_SHADER_SHADER_SPECIFIC_CONST_4, info.m_nEnvmapMaskTransform );
				}
				else if ( hasBump2 )
				{
					pContextData->m_SemiStaticCmdsOut.SetVertexShaderTextureTransform( VERTEX_SHADER_SHADER_SPECIFIC_CONST_4, info.m_nBumpTransform2 );
				}
			}
			pContextData->m_SemiStaticCmdsOut.SetEnvMapTintPixelShaderDynamicState( 0, info.m_nEnvmapTint );
			
			if ( hasDetailTexture )
			{
				float detailTintAndBlend[4] = {1, 1, 1, 1};
				
				if ( info.m_nDetailTint != -1 )
				{
					params[info.m_nDetailTint]->GetVecValue( detailTintAndBlend, 3 );
				}
				
				detailTintAndBlend[3] = fDetailBlendFactor;
				pContextData->m_SemiStaticCmdsOut.SetPixelShaderConstant( 8, detailTintAndBlend );
			}
			
			float envmapTintVal[4];
			float selfIllumTintVal[4];
			params[info.m_nEnvmapTint]->GetVecValue( envmapTintVal, 3 );
			params[info.m_nSelfIllumTint]->GetVecValue( selfIllumTintVal, 3 );
			float envmapContrast = params[info.m_nEnvmapContrast]->GetFloatValue();
			float envmapSaturation = params[info.m_nEnvmapSaturation]->GetFloatValue();
			float fresnelReflection = params[info.m_nFresnelReflection]->GetFloatValue();
			bool hasEnvmap = params[info.m_nEnvmap]->IsTexture();
			int envmap_variant; //0 = no envmap, 1 = regular, 2 = darken in shadow mode
			if( hasEnvmap )
			{
				//only enabled darkened cubemap mode when the scale calls for it. And not supported in ps20 when also using a 2nd bumpmap
				envmap_variant = ((GetFloatParam( info.m_nEnvMapLightScale, params ) > 0.0f) && (g_pHardwareConfig->SupportsPixelShaders_2_b() || !hasBump2)) ? 2 : 1;
			}
			else
			{
				envmap_variant = 0; 
			}

			pContextData->m_bPixelShaderFastPath = true;
			bool bUsingContrastOrSaturation = hasEnvmap && ( ( (envmapContrast != 0.0f) && (envmapContrast != 1.0f) ) || (envmapSaturation != 1.0f) );
			bool bUsingFresnel = hasEnvmap && (fresnelReflection != 1.0f);
			bool bUsingSelfIllumTint = IS_FLAG_SET(MATERIAL_VAR_SELFILLUM) && (selfIllumTintVal[0] != 1.0f || selfIllumTintVal[1] != 1.0f || selfIllumTintVal[2] != 1.0f); 
			if ( bUsingContrastOrSaturation || bUsingFresnel || bUsingSelfIllumTint || !g_pConfig->bShowSpecular )
			{
				pContextData->m_bPixelShaderFastPath = false;
			}
			if( !pContextData->m_bPixelShaderFastPath )
			{
				pContextData->m_SemiStaticCmdsOut.SetPixelShaderConstants( 2, 3 );
				pContextData->m_SemiStaticCmdsOut.OutputConstantData( params[info.m_nEnvmapContrast]->GetVecValue() );
				pContextData->m_SemiStaticCmdsOut.OutputConstantData( params[info.m_nEnvmapSaturation]->GetVecValue() );
				float flFresnel = params[info.m_nFresnelReflection]->GetFloatValue();
				// [ 0, 0, 1-R(0), R(0) ]
				pContextData->m_SemiStaticCmdsOut.OutputConstantData4( 0., 0., 1.0 - flFresnel, flFresnel );
				
				pContextData->m_SemiStaticCmdsOut.SetPixelShaderConstant( 7, params[info.m_nSelfIllumTint]->GetVecValue() );
			}
			else
			{
				if ( bHasOutline )
				{
					float flOutlineParms[8] = { GetFloatParam( info.m_nOutlineStart0, params ),
												GetFloatParam( info.m_nOutlineStart1, params ),
												GetFloatParam( info.m_nOutlineEnd0, params ),
												GetFloatParam( info.m_nOutlineEnd1, params ),
												0,0,0,
												GetFloatParam( info.m_nOutlineAlpha, params ) };
					if ( info.m_nOutlineColor != -1 )
					{
						params[info.m_nOutlineColor]->GetVecValue( flOutlineParms + 4, 3 );
					}
					pContextData->m_SemiStaticCmdsOut.SetPixelShaderConstant( 2, flOutlineParms, 2 );
				}

				if ( bHasSoftEdges )
				{
					pContextData->m_SemiStaticCmdsOut.SetPixelShaderConstant4( 
						4, GetFloatParam( info.m_nEdgeSoftnessStart, params ),
						GetFloatParam( info.m_nEdgeSoftnessEnd, params ),
						0,0 );
				}
			}

			// parallax and cubemap light scale mapping parms (c20)
			if ( bParallaxMapping || (envmap_variant == 2) )
			{
				pContextData->m_SemiStaticCmdsOut.SetPixelShaderConstant4( 20, GetFloatParam( info.m_nHeightScale, params), GetFloatParam( info.m_nEnvMapLightScale, params), 0, 0 );
			}

			// texture binds
			if( hasBaseTexture )
			{
				pContextData->m_SemiStaticCmdsOut.BindTexture( pShader, SHADER_SAMPLER0, info.m_nBaseTexture, info.m_nBaseTextureFrame );
			}
			// handle mat_fullbright 2
			bool bLightingOnly = mat_fullbright.GetInt() == 2 && !IS_FLAG_SET( MATERIAL_VAR_NO_DEBUG_OVERRIDE );
			if( bLightingOnly )
			{
				// BASE TEXTURE
				if( hasSelfIllum )
				{
					pContextData->m_SemiStaticCmdsOut.BindStandardTexture( SHADER_SAMPLER0, TEXTURE_GREY_ALPHA_ZERO );
				}
				else
				{
					pContextData->m_SemiStaticCmdsOut.BindStandardTexture( SHADER_SAMPLER0, TEXTURE_GREY );
				}

				// BASE TEXTURE 2	
				if( hasBaseTexture2 )
				{
					pContextData->m_SemiStaticCmdsOut.BindStandardTexture( SHADER_SAMPLER7, TEXTURE_GREY );
				}

				// DETAIL TEXTURE
				if( hasDetailTexture )
				{
					pContextData->m_SemiStaticCmdsOut.BindStandardTexture( SHADER_SAMPLER12, TEXTURE_GREY );
				}

				// disable color modulation
				float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
				pContextData->m_SemiStaticCmdsOut.SetVertexShaderConstant( VERTEX_SHADER_MODULATION_COLOR, color );

				// turn off environment mapping
				envmapTintVal[0] = 0.0f;
				envmapTintVal[1] = 0.0f;
				envmapTintVal[2] = 0.0f;
			}

			// always set the transform for detail textures since I'm assuming that you'll
			// always have a detailscale.
			if( hasDetailTexture )
			{
				pContextData->m_SemiStaticCmdsOut.SetVertexShaderTextureScaledTransform( VERTEX_SHADER_SHADER_SPECIFIC_CONST_2, info.m_nBaseTextureTransform, info.m_nDetailScale );
			}

			if( hasBaseTexture2 )
			{
				pContextData->m_SemiStaticCmdsOut.BindTexture( pShader, SHADER_SAMPLER7, info.m_nBaseTexture2, info.m_nBaseTexture2Frame );
			}
			if( hasDetailTexture )
			{
				pContextData->m_SemiStaticCmdsOut.BindTexture( pShader, SHADER_SAMPLER12, info.m_nDetail, info.m_nDetailFrame );
			}

			if( hasBump || hasNormalMapAlphaEnvmapMask )
			{
				if( !g_pConfig->m_bFastNoBump )
				{
					pContextData->m_SemiStaticCmdsOut.BindTexture( pShader, SHADER_SAMPLER4, info.m_nBumpmap, info.m_nBumpFrame );
				}
				else
				{
					if( hasSSBump )
					{
						pContextData->m_SemiStaticCmdsOut.BindStandardTexture( SHADER_SAMPLER4, TEXTURE_SSBUMP_FLAT );
					}
					else
					{
						pContextData->m_SemiStaticCmdsOut.BindStandardTexture( SHADER_SAMPLER4, TEXTURE_NORMALMAP_FLAT );
					}
				}
			}
			if( hasBump2 )
			{
				if( !g_pConfig->m_bFastNoBump )
				{
					pContextData->m_SemiStaticCmdsOut.BindTexture( pShader, SHADER_SAMPLER5, info.m_nBumpmap2, info.m_nBumpFrame2 );
				}
				else
				{
					if( hasSSBump )
					{
						pContextData->m_SemiStaticCmdsOut.BindStandardTexture( SHADER_SAMPLER5, TEXTURE_NORMALMAP_FLAT );
					}
					else
					{
						pContextData->m_SemiStaticCmdsOut.BindStandardTexture( SHADER_SAMPLER5, TEXTURE_SSBUMP_FLAT );
					}
				}
			}
			if( hasBumpMask )
			{
				if( !g_pConfig->m_bFastNoBump )
				{
					pContextData->m_SemiStaticCmdsOut.BindTexture( pShader, SHADER_SAMPLER8, info.m_nBumpMask, -1 );
				}
				else
				{
					// this doesn't make sense
					pContextData->m_SemiStaticCmdsOut.BindStandardTexture( SHADER_SAMPLER8, TEXTURE_NORMALMAP_FLAT );
				}
			}

			if( hasEnvmapMask )
			{
				pContextData->m_SemiStaticCmdsOut.BindTexture( pShader, SHADER_SAMPLER5, info.m_nEnvmapMask, info.m_nEnvmapMaskFrame );
			}

			if ( hasLightWarpTexture )
			{
				pContextData->m_SemiStaticCmdsOut.BindTexture( pShader, SHADER_SAMPLER6, info.m_nLightWarpTexture, -1 );
			}

			if ( bHasBlendModulateTexture )
			{
				pContextData->m_SemiStaticCmdsOut.BindTexture( pShader, SHADER_SAMPLER3, info.m_nBlendModulateTexture, -1 );
			}

			if ( hasFlashlight && IsX360() )
			{
				pContextData->m_SemiStaticCmdsOut.SetVertexShaderFlashlightState( VERTEX_SHADER_SHADER_SPECIFIC_CONST_6 );

				CBCmdSetPixelShaderFlashlightState_t state;
				state.m_LightSampler = SHADER_SAMPLER13;
				state.m_DepthSampler = SHADER_SAMPLER14;
				state.m_ShadowNoiseSampler = SHADER_SAMPLER15;
				state.m_nColorConstant = 28;
				state.m_nAttenConstant = 13;
				state.m_nOriginConstant = 14;
				state.m_nDepthTweakConstant = 19;
				state.m_nScreenScaleConstant = 31;
				state.m_nWorldToTextureConstant = -1;
				state.m_bFlashlightNoLambert = false;
				state.m_bSinglePassFlashlight = bSinglePassFlashlight;
				pContextData->m_SemiStaticCmdsOut.SetPixelShaderFlashlightState( state );
			}

			pContextData->m_SemiStaticCmdsOut.End();
		}
	}
	DYNAMIC_STATE
	{
		CCommandBufferBuilder< CFixedCommandStorageBuffer< 1000 > > DynamicCmdsOut;
		DynamicCmdsOut.Call( pContextData->m_pStaticCmds );
		DynamicCmdsOut.Call( pContextData->m_SemiStaticCmdsOut.Base() );

		bool hasEnvmap = params[info.m_nEnvmap]->IsTexture();

		if( hasEnvmap )
		{
			DynamicCmdsOut.BindTexture( pShader, SHADER_SAMPLER2, info.m_nEnvmap, info.m_nEnvmapFrame );
		}

		bool bVertexShaderFastPath = pContextData->m_bVertexShaderFastPath;

		int nFixedLightingMode = pShaderAPI->GetIntRenderingParameter( INT_RENDERPARM_ENABLE_FIXED_LIGHTING );
		if( nFixedLightingMode != ENABLE_FIXED_LIGHTING_NONE )
		{
			if ( pContextData->m_bPixelShaderForceFastPathBecauseOutline )
			{
				nFixedLightingMode = ENABLE_FIXED_LIGHTING_NONE;
			}
			else
			{
				bVertexShaderFastPath = false;
			}
		}

		bool bWorldNormal = ( nFixedLightingMode == ENABLE_FIXED_LIGHTING_OUTPUTNORMAL_AND_DEPTH );
		if ( bWorldNormal && IsPC() )
		{
			float vEyeDir[4];
			pShaderAPI->GetWorldSpaceCameraDirection( vEyeDir );

			float flFarZ = pShaderAPI->GetFarZ();
			vEyeDir[0] /= flFarZ;	// Divide by farZ for SSAO algorithm
			vEyeDir[1] /= flFarZ;
			vEyeDir[2] /= flFarZ;
			DynamicCmdsOut.SetVertexShaderConstant4( 12, vEyeDir[0], vEyeDir[1], vEyeDir[2], 1.0f );
		}

		MaterialFogMode_t fogType = pShaderAPI->GetSceneFogMode();

		if( bDeferredActive )
		{
			DECLARE_DYNAMIC_VERTEX_SHADER( lightmappedgeneric_deferred_vs30 );
			SET_DYNAMIC_VERTEX_SHADER_COMBO( FASTPATH,  bVertexShaderFastPath );
			SET_DYNAMIC_VERTEX_SHADER_CMD( DynamicCmdsOut, lightmappedgeneric_deferred_vs30 );
		}
		else
		{
			DECLARE_DYNAMIC_VERTEX_SHADER( lightmappedgeneric_vs30 );
			SET_DYNAMIC_VERTEX_SHADER_COMBO( FASTPATH,  bVertexShaderFastPath );
			SET_DYNAMIC_VERTEX_SHADER_CMD( DynamicCmdsOut, lightmappedgeneric_vs30 );
		}

		bool bPixelShaderFastPath = pContextData->m_bPixelShaderFastPath;

		if ( nFixedLightingMode != ENABLE_FIXED_LIGHTING_NONE )
		{
			bPixelShaderFastPath = false;
		}
		bool bWriteDepthToAlpha;
		bool bWriteWaterFogToAlpha;
		if(  pContextData->m_bFullyOpaque ) 
		{
			bWriteDepthToAlpha = pShaderAPI->ShouldWriteDepthToDestAlpha();
			bWriteWaterFogToAlpha = (fogType == MATERIAL_FOG_LINEAR_BELOW_FOG_Z);
			AssertMsg( !(bWriteDepthToAlpha && bWriteWaterFogToAlpha), "Can't write two values to alpha at the same time." );
		}
		else
		{
			//can't write a special value to dest alpha if we're actually using as-intended alpha
			bWriteDepthToAlpha = false;
			bWriteWaterFogToAlpha = false;
		}

		if( bDeferredActive )
		{
			DynamicCmdsOut.BindTexture( pShader, SHADER_SAMPLER14, GetDeferredExt()->GetTexture_LightAccum(), 0 );
			DynamicCmdsOut.BindTexture( pShader, SHADER_SAMPLER15, GetDeferredExt()->GetTexture_LightAccum2(), 0 );
			//DynamicCmdsOut.BindStandardTexture( SHADER_SAMPLER14, TEXTURE_WHITE );
			int x, y, w, t;
			pShaderAPI->GetCurrentViewport( x, y, w, t );
			float fl1[4] = { 1.0f / w, 1.0f / t, 0, 0 };

			DynamicCmdsOut.SetPixelShaderConstant( PSREG_UBERLIGHT_SMOOTH_EDGE_0, fl1 );
		}

		bool bFlashlightShadows = false;
		bool bUberlight = false;
		if( hasFlashlight && IsX360() )
		{
			pShaderAPI->GetFlashlightShaderInfo( &bFlashlightShadows, &bUberlight );
		}
		else
		{
			// only do ambient light when not using flashlight
			static ConVarRef mat_ambient_light_r_forced( "mat_ambient_light_r_forced" );
			static ConVarRef mat_ambient_light_g_forced( "mat_ambient_light_g_forced" );
			static ConVarRef mat_ambient_light_b_forced( "mat_ambient_light_b_forced" );

			float vAmbientColor[4] = { mat_ambient_light_r_forced.GetFloat() != -1.0f ? mat_ambient_light_r_forced.GetFloat() : mat_ambient_light_r.GetFloat(), 
									   mat_ambient_light_g_forced.GetFloat() != -1.0f ? mat_ambient_light_g_forced.GetFloat() : mat_ambient_light_g.GetFloat(), 
									   mat_ambient_light_b_forced.GetFloat() != -1.0f ? mat_ambient_light_b_forced.GetFloat() : mat_ambient_light_b.GetFloat(), 
									   0.0f };
			if ( mat_fullbright.GetInt() == 1 )
			{
				vAmbientColor[0] = vAmbientColor[1] = vAmbientColor[2] = 0.0f;
			}
			DynamicCmdsOut.SetPixelShaderConstant( 31, vAmbientColor, 1 );
		}

		float envmapContrast = params[info.m_nEnvmapContrast]->GetFloatValue();

		if( bDeferredActive )
		{
			DECLARE_DYNAMIC_PIXEL_SHADER( lightmappedgeneric_deferred_ps30);
			SET_DYNAMIC_PIXEL_SHADER_COMBO( FASTPATH,  bPixelShaderFastPath || pContextData->m_bPixelShaderForceFastPathBecauseOutline );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( FASTPATHENVMAPCONTRAST,  bPixelShaderFastPath && envmapContrast == 1.0f );

			// Don't write fog to alpha if we're using translucency
			SET_DYNAMIC_PIXEL_SHADER_COMBO( WRITE_DEPTH_TO_DESTALPHA, bWriteDepthToAlpha );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( WRITEWATERFOGTODESTALPHA, bWriteWaterFogToAlpha );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( FLASHLIGHTSHADOWS, /*bFlashlightShadows*/ 0 );
			SET_DYNAMIC_PIXEL_SHADER_CMD( DynamicCmdsOut, lightmappedgeneric_deferred_ps30 );
		}
		else
		{
			DECLARE_DYNAMIC_PIXEL_SHADER( lightmappedgeneric_ps30 );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( FASTPATH,  bPixelShaderFastPath || pContextData->m_bPixelShaderForceFastPathBecauseOutline );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( FASTPATHENVMAPCONTRAST,  bPixelShaderFastPath && envmapContrast == 1.0f );

			// Don't write fog to alpha if we're using translucency
			SET_DYNAMIC_PIXEL_SHADER_COMBO( WRITE_DEPTH_TO_DESTALPHA, bWriteDepthToAlpha );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( WRITEWATERFOGTODESTALPHA, bWriteWaterFogToAlpha );
			_pshIndex.SetFLASHLIGHTSHADOWS( bFlashlightShadows );
			SET_DYNAMIC_PIXEL_SHADER_CMD( DynamicCmdsOut, lightmappedgeneric_ps30 );
		}

		if ( IS_PARAM_DEFINED( info.m_nOffsetTexture ) && IS_PARAM_DEFINED( info.m_nOffsetScale ) && params[info.m_nOffsetTexture]->IsTexture() )
		{
			DynamicCmdsOut.SetVertexShaderConstant4( VERTEX_SHADER_SHADER_SPECIFIC_CONST_6, params[info.m_nOffsetScale]->GetFloatValue(), 0, 0, 0 );
			pShader->BindVertexTexture( SHADER_VERTEXTEXTURE_SAMPLER0, info.m_nOffsetTexture );
		}
		else
		{
			DynamicCmdsOut.SetVertexShaderConstant4( VERTEX_SHADER_SHADER_SPECIFIC_CONST_6, 0, 0, 0, 0 );
			pShaderAPI->BindStandardVertexTexture( SHADER_VERTEXTEXTURE_SAMPLER0, TEXTURE_BLACK );
		}

		DynamicCmdsOut.End();
		pShaderAPI->ExecuteCommandBuffer( DynamicCmdsOut.Base() );
	}
	pShader->Draw();

	if( !bDeferredActive && IsPC() && (IS_FLAG_SET( MATERIAL_VAR_ALPHATEST ) != 0) && pContextData->m_bFullyOpaqueWithoutAlphaTest )
	{
		//Alpha testing makes it so we can't write to dest alpha
		//Writing to depth makes it so later polygons can't write to dest alpha either
		//This leads to situations with garbage in dest alpha.

		//Fix it now by converting depth to dest alpha for any pixels that just wrote.
		pShader->DrawEqualDepthToDestAlpha();
	}
}