//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: Multiblending shader for Spy in TF2 (and probably many other things to come)
//
// $NoKeywords: $
//=====================================================================================//

#include "BaseVSShader.h"
#include "convar.h"
#include "multiblend_dx9_helper.h"

#ifdef DEFERRED
#include "deferred_includes.h"
#endif // DEFERRED

// NOTE: This has to be the last file included!
#include "tier0/memdbgon.h"


DEFINE_FALLBACK_SHADER( Multiblend, Multiblend_DX90 )

BEGIN_VS_SHADER( Multiblend_DX90, "Help for Multiblend" )

	BEGIN_SHADER_PARAMS
		SHADER_PARAM( SPECTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "", "" )
		SHADER_PARAM( BASETEXTURE2, SHADER_PARAM_TYPE_TEXTURE, "", "" )
		SHADER_PARAM( SPECTEXTURE2, SHADER_PARAM_TYPE_TEXTURE, "", "" )
		SHADER_PARAM( BASETEXTURE3, SHADER_PARAM_TYPE_TEXTURE, "", "" )
		SHADER_PARAM( SPECTEXTURE3, SHADER_PARAM_TYPE_TEXTURE, "", "" )
		SHADER_PARAM( BASETEXTURE4, SHADER_PARAM_TYPE_TEXTURE, "", "" )
		SHADER_PARAM( SPECTEXTURE4, SHADER_PARAM_TYPE_TEXTURE, "", "" )
		SHADER_PARAM( FOW, SHADER_PARAM_TYPE_TEXTURE, "", "FoW Render Target" )
		SHADER_PARAM( ROTATION, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
		SHADER_PARAM( SCALE, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
		SHADER_PARAM( ROTATION2, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
		SHADER_PARAM( SCALE2, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
		SHADER_PARAM( ROTATION3, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
		SHADER_PARAM( SCALE3, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )
		SHADER_PARAM( ROTATION4, SHADER_PARAM_TYPE_FLOAT, "0.0", "" )
		SHADER_PARAM( SCALE4, SHADER_PARAM_TYPE_FLOAT, "1.0", "" )

		// Deferred:
		SHADER_PARAM( PHONG_EXP, SHADER_PARAM_TYPE_FLOAT, "", "" )
		SHADER_PARAM( PHONG_EXP2, SHADER_PARAM_TYPE_FLOAT, "", "" )
	END_SHADER_PARAMS

	void SetupVars( Multiblend_DX9_Vars_t& info )
	{
		info.m_nBaseTextureTransform = BASETEXTURETRANSFORM;
		info.m_nBaseTexture = BASETEXTURE;
		info.m_nSpecTexture = SPECTEXTURE;
		info.m_nBaseTexture2 = BASETEXTURE2;
		info.m_nSpecTexture2 = SPECTEXTURE2;
		info.m_nBaseTexture3 = BASETEXTURE3;
		info.m_nSpecTexture3 = SPECTEXTURE3;
		info.m_nBaseTexture4 = BASETEXTURE4;
		info.m_nSpecTexture4 = SPECTEXTURE4;
		info.m_nFoW = FOW;
		info.m_nRotation = ROTATION;
		info.m_nRotation2 = ROTATION2;
		info.m_nRotation3 = ROTATION3;
		info.m_nRotation4 = ROTATION4;
		info.m_nScale = SCALE;
		info.m_nScale2 = SCALE2;
		info.m_nScale3 = SCALE3;
		info.m_nScale4 = SCALE4;
		info.m_nFlashlightTexture = FLASHLIGHTTEXTURE;
		info.m_nFlashlightTextureFrame = FLASHLIGHTTEXTUREFRAME;
	}

	void SetupParmsGBuffer( defParms_gBuffer &p )
	{
		p.bModel = false;

		p.iAlbedo = BASETEXTURE;
		p.iAlbedo2 = BASETEXTURE2;
		p.iAlbedo3 = BASETEXTURE3;
		p.iAlbedo4 = BASETEXTURE4;

		p.nSpecTexture = SPECTEXTURE;
		p.nSpecTexture2 = SPECTEXTURE2;
		p.nSpecTexture3 = SPECTEXTURE3;
		p.nSpecTexture4 = SPECTEXTURE4;
		p.nRotation = ROTATION;
		p.nRotation2 = ROTATION2;
		p.nRotation3 = ROTATION3;
		p.nRotation4 = ROTATION4;
		p.nScale = SCALE;
		p.nScale2 = SCALE2;
		p.nScale3 = SCALE3;
		p.nScale4 = SCALE4;

		p.iPhongExp = PHONG_EXP;
		p.iPhongExp2 = PHONG_EXP2;
	}

	void SetupParmsShadow( defParms_shadow &p )
	{
		p.bModel = false;
		p.iAlbedo = BASETEXTURE;
		p.iAlbedo2 = BASETEXTURE2;
		p.iAlbedo3 = BASETEXTURE3;
		p.iAlbedo4 = BASETEXTURE4;

		p.nSpecTexture = SPECTEXTURE;
		p.nSpecTexture2 = SPECTEXTURE2;
		p.nSpecTexture3 = SPECTEXTURE3;
		p.nSpecTexture4 = SPECTEXTURE4;
		p.nRotation = ROTATION;
		p.nRotation2 = ROTATION2;
		p.nRotation3 = ROTATION3;
		p.nRotation4 = ROTATION4;
		p.nScale = SCALE;
		p.nScale2 = SCALE2;
		p.nScale3 = SCALE3;
		p.nScale4 = SCALE4;

		//p.iAlphatestRef = ALPHATESTREFERENCE;
	}

	bool DrawToGBuffer( IMaterialVar **params )
	{
#if DEFCFG_DEFERRED_SHADING == 1
		return true;
#else
		const bool bIsDecal = IS_FLAG_SET( MATERIAL_VAR_DECAL );
		const bool bTranslucent = IS_FLAG_SET( MATERIAL_VAR_TRANSLUCENT );

		return !bTranslucent && !bIsDecal;
#endif
	}

	SHADER_INIT_PARAMS()
	{
		Multiblend_DX9_Vars_t info;
		SetupVars( info );
		InitParamsMultiblend_DX9( this, params, pMaterialName, info );

		bool bDeferredActive = GetDeferredExt()->IsDeferredLightingEnabled();
		if( bDeferredActive )
		{
			defParms_gBuffer parms_gbuffer;
			SetupParmsGBuffer( parms_gbuffer );
			InitParmsGBuffer( parms_gbuffer, this, params );
		
			defParms_shadow parms_shadow;
			SetupParmsShadow( parms_shadow );
			InitParmsShadowPass( parms_shadow, this, params );
		}
	}

	SHADER_FALLBACK
	{
		return 0;
	}

	SHADER_INIT
	{
		Multiblend_DX9_Vars_t info;
		SetupVars( info );
		InitMultiblend_DX9( this, params, info );

		bool bDeferredActive = GetDeferredExt()->IsDeferredLightingEnabled();
		if( bDeferredActive )
		{
			defParms_gBuffer parms_gbuffer;
			SetupParmsGBuffer( parms_gbuffer );
			InitPassGBuffer( parms_gbuffer, this, params );

			defParms_shadow parms_shadow;
			SetupParmsShadow( parms_shadow );
			InitPassShadowPass( parms_shadow, this, params );
		}
	}

	SHADER_DRAW
	{
		bool bDeferredActive = GetDeferredExt()->IsDeferredLightingEnabled();

		if( !bDeferredActive )
		{
			Multiblend_DX9_Vars_t info;
			SetupVars( info );
			DrawMultiblend_DX9( this, params, pShaderAPI, pShaderShadow, info, vertexCompression, pContextDataPtr );
		}
		else
		{
			if ( pShaderAPI != NULL && *pContextDataPtr == NULL )
				*pContextDataPtr = new CDeferredPerMaterialContextData();

			CDeferredPerMaterialContextData *pDefContext = reinterpret_cast< CDeferredPerMaterialContextData* >(*pContextDataPtr);

			const int iDeferredRenderStage = pShaderAPI ?
				pShaderAPI->GetIntRenderingParameter( INT_RENDERPARM_DEFERRED_RENDER_STAGE )
				: DEFERRED_RENDER_STAGE_INVALID;

			const bool bDrawToGBuffer = DrawToGBuffer( params );

			Assert( pShaderAPI == NULL ||
				iDeferredRenderStage != DEFERRED_RENDER_STAGE_INVALID );

			if ( bDrawToGBuffer )
			{
				if ( pShaderShadow != NULL ||
					iDeferredRenderStage == DEFERRED_RENDER_STAGE_GBUFFER )
				{
					defParms_gBuffer parms_gbuffer;
					SetupParmsGBuffer( parms_gbuffer );
					DrawPassGBuffer( parms_gbuffer, this, params, pShaderShadow, pShaderAPI,
						vertexCompression, pDefContext );
				}
				else
					Draw( false );

				if ( pShaderShadow != NULL ||
					iDeferredRenderStage == DEFERRED_RENDER_STAGE_SHADOWPASS )
				{
					defParms_shadow parms_shadow;
					SetupParmsShadow( parms_shadow );
					DrawPassShadowPass( parms_shadow, this, params, pShaderShadow, pShaderAPI,
						vertexCompression, pDefContext );
				}
				else
					Draw( false );
			}

	#if ( DEFCFG_DEFERRED_SHADING == 0 )
			if ( pShaderShadow != NULL ||
				iDeferredRenderStage == DEFERRED_RENDER_STAGE_COMPOSITION )
			{
				Multiblend_DX9_Vars_t info;
				SetupVars( info );
				DrawMultiblend_DX9( this, params, pShaderAPI, pShaderShadow, info, vertexCompression, pContextDataPtr );
			}
			else
				Draw( false );
	#endif

			if ( pShaderAPI != NULL && pDefContext->m_bMaterialVarsChanged )
				pDefContext->m_bMaterialVarsChanged = false;
		}
	}
END_SHADER

