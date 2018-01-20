//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $Header: $
// $NoKeywords: $
//===========================================================================//

#include "BaseVSShader.h"
#include "deferred_includes.h"

// Auto generated inc files
#include "flora_vs30.inc"
#include "flora_ps30.inc"

static CCommandBufferBuilder< CFixedCommandStorageBuffer< 512 > > tmpBuf;

extern ConVar building_cubemaps;

BEGIN_VS_SHADER( Flora, "Help for Flora" )

	BEGIN_SHADER_PARAMS
		SHADER_PARAM( ALPHATESTREFERENCE, SHADER_PARAM_TYPE_FLOAT, "0.5", "" )	

		// Tree sway animation control
		SHADER_PARAM( TREESWAY, SHADER_PARAM_TYPE_INTEGER, "0", "" );
		SHADER_PARAM( TREESWAYHEIGHT, SHADER_PARAM_TYPE_FLOAT, "1000", "" );
		SHADER_PARAM( TREESWAYSTARTHEIGHT, SHADER_PARAM_TYPE_FLOAT, "0.2", "" );
		SHADER_PARAM( TREESWAYRADIUS, SHADER_PARAM_TYPE_FLOAT, "300", "" );
		SHADER_PARAM( TREESWAYSTARTRADIUS, SHADER_PARAM_TYPE_FLOAT, "0.1", "" );
		SHADER_PARAM( TREESWAYSPEED, SHADER_PARAM_TYPE_FLOAT, "1", "" );
		SHADER_PARAM( TREESWAYSPEEDHIGHWINDMULTIPLIER, SHADER_PARAM_TYPE_FLOAT, "2", "" );
		SHADER_PARAM( TREESWAYSTRENGTH, SHADER_PARAM_TYPE_FLOAT, "10", "" );
		SHADER_PARAM( TREESWAYSCRUMBLESPEED, SHADER_PARAM_TYPE_FLOAT, "0.1", "" );
		SHADER_PARAM( TREESWAYSCRUMBLESTRENGTH, SHADER_PARAM_TYPE_FLOAT, "0.1", "" );
		SHADER_PARAM( TREESWAYSCRUMBLEFREQUENCY, SHADER_PARAM_TYPE_FLOAT, "0.1", "" );
		SHADER_PARAM( TREESWAYFALLOFFEXP, SHADER_PARAM_TYPE_FLOAT, "1.5", "" );
		SHADER_PARAM( TREESWAYSCRUMBLEFALLOFFEXP, SHADER_PARAM_TYPE_FLOAT, "1.0", "" );
		SHADER_PARAM( TREESWAYSPEEDLERPSTART, SHADER_PARAM_TYPE_FLOAT, "3", "" );
		SHADER_PARAM( TREESWAYSPEEDLERPEND, SHADER_PARAM_TYPE_FLOAT, "6", "" );

		// Deferred control options
		SHADER_PARAM( NOSHADOWPASS, SHADER_PARAM_TYPE_BOOL, "0", "Allows turning off the shadow pass of this material" )
		SHADER_PARAM( NODEFERREDLIGHT, SHADER_PARAM_TYPE_BOOL, "0", "No deferred light input" )
		SHADER_PARAM( MODELGLOBALNORMAL, SHADER_PARAM_TYPE_BOOL, "0", "Use global light direction as normal for all model vertices" )
		SHADER_PARAM( PHONG_EXP, SHADER_PARAM_TYPE_FLOAT, "", "" )
	END_SHADER_PARAMS

	void SetupParmsGBuffer( defParms_gBuffer &p )
	{
		p.bModel = true;

		p.iAlbedo = BASETEXTURE;
		p.iAlphatestRef = ALPHATESTREFERENCE;
		p.iPhongExp = PHONG_EXP;

		p.m_nTreeSway = TREESWAY;
		p.m_nTreeSwayHeight = TREESWAYHEIGHT;
		p.m_nTreeSwayStartHeight = TREESWAYSTARTHEIGHT;
		p.m_nTreeSwayRadius = TREESWAYRADIUS;
		p.m_nTreeSwayStartRadius = TREESWAYSTARTRADIUS;
		p.m_nTreeSwaySpeed = TREESWAYSPEED;
		p.m_nTreeSwaySpeedHighWindMultiplier = TREESWAYSPEEDHIGHWINDMULTIPLIER;
		p.m_nTreeSwayStrength = TREESWAYSTRENGTH;
		p.m_nTreeSwayScrumbleSpeed = TREESWAYSCRUMBLESPEED;
		p.m_nTreeSwayScrumbleStrength = TREESWAYSCRUMBLESTRENGTH;
		p.m_nTreeSwayScrumbleFrequency = TREESWAYSCRUMBLEFREQUENCY;
		p.m_nTreeSwayFalloffExp = TREESWAYFALLOFFEXP;
		p.m_nTreeSwayScrumbleFalloffExp = TREESWAYSCRUMBLEFALLOFFEXP;		
		p.m_nTreeSwaySpeedLerpStart = TREESWAYSPEEDLERPSTART;
		p.m_nTreeSwaySpeedLerpEnd = TREESWAYSPEEDLERPEND;
	}

	void SetupParmsShadow( defParms_shadow &p )
	{
		p.bModel = true;
		p.iAlbedo = BASETEXTURE;
		p.iAlphatestRef = ALPHATESTREFERENCE;

		p.m_nTreeSway = TREESWAY;
		p.m_nTreeSwayHeight = TREESWAYHEIGHT;
		p.m_nTreeSwayStartHeight = TREESWAYSTARTHEIGHT;
		p.m_nTreeSwayRadius = TREESWAYRADIUS;
		p.m_nTreeSwayStartRadius = TREESWAYSTARTRADIUS;
		p.m_nTreeSwaySpeed = TREESWAYSPEED;
		p.m_nTreeSwaySpeedHighWindMultiplier = TREESWAYSPEEDHIGHWINDMULTIPLIER;
		p.m_nTreeSwayStrength = TREESWAYSTRENGTH;
		p.m_nTreeSwayScrumbleSpeed = TREESWAYSCRUMBLESPEED;
		p.m_nTreeSwayScrumbleStrength = TREESWAYSCRUMBLESTRENGTH;
		p.m_nTreeSwayScrumbleFrequency = TREESWAYSCRUMBLEFREQUENCY;
		p.m_nTreeSwayFalloffExp = TREESWAYFALLOFFEXP;
		p.m_nTreeSwayScrumbleFalloffExp = TREESWAYSCRUMBLEFALLOFFEXP;		
		p.m_nTreeSwaySpeedLerpStart = TREESWAYSPEEDLERPSTART;
		p.m_nTreeSwaySpeedLerpEnd = TREESWAYSPEEDLERPEND;
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
		//SET_FLAGS2( MATERIAL_VAR2_LIGHTING_VERTEX_LIT );
		SET_FLAGS2( MATERIAL_VAR2_SUPPORTS_HW_SKINNING );
		CLEAR_FLAGS( MATERIAL_VAR_SELFILLUM );
		CLEAR_FLAGS( MATERIAL_VAR_BASEALPHAENVMAPMASK );

		if ( g_pHardwareConfig->HasFastVertexTextures() )
			SET_FLAGS2( MATERIAL_VAR2_USES_VERTEXID );

		InitIntParam( TREESWAY, params, 0 );
		InitFloatParam( TREESWAYHEIGHT, params, 1000.0f );
		InitFloatParam( TREESWAYSTARTHEIGHT, params, 0.1f );
		InitFloatParam( TREESWAYRADIUS, params, 300.0f );
		InitFloatParam( TREESWAYSTARTRADIUS, params, 0.2f );
		InitFloatParam( TREESWAYSPEED, params, 1.0f );
		InitFloatParam( TREESWAYSPEEDHIGHWINDMULTIPLIER, params, 2.0f );
		InitFloatParam( TREESWAYSTRENGTH, params, 10.0f );
		InitFloatParam( TREESWAYSCRUMBLESPEED, params, 5.0f );
		InitFloatParam( TREESWAYSCRUMBLESTRENGTH, params, 10.0f );
		InitFloatParam( TREESWAYSCRUMBLEFREQUENCY, params, 12.0f );
		InitFloatParam( TREESWAYFALLOFFEXP, params, 1.5f );
		InitFloatParam( TREESWAYSCRUMBLEFALLOFFEXP, params, 1.0f );
		InitFloatParam( TREESWAYSPEEDLERPSTART, params, 3.0f );
		InitFloatParam( TREESWAYSPEEDLERPEND, params, 6.0f );

		//const bool bIsDecal = IS_FLAG_SET( MATERIAL_VAR_DECAL );
		const bool bDeferredActive = GetDeferredExt()->IsDeferredLightingEnabled();
		if( bDeferredActive )// && !bIsDecal )
		{
			const bool bTranslucent = IS_FLAG_SET( MATERIAL_VAR_TRANSLUCENT );
			const bool bAlphaTest = IS_FLAG_SET( MATERIAL_VAR_ALPHATEST );

			if( bTranslucent ) 
			{
				CLEAR_FLAGS( MATERIAL_VAR_TRANSLUCENT );
				SET_FLAGS( MATERIAL_VAR_ALPHATEST );

				params[ALPHATESTREFERENCE]->SetFloatValue( 0.5f );
			}
			else if( bAlphaTest )
			{
				if( params[ALPHATESTREFERENCE]->GetFloatValue() == 0.0f )
					params[ALPHATESTREFERENCE]->SetFloatValue( 0.5f );
			}
		}

		const bool bDrawToGBuffer = DrawToGBuffer( params );

		if ( bDrawToGBuffer )
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
		const bool bDrawToGBuffer = DrawToGBuffer( params );

		if ( bDrawToGBuffer )
		{
			defParms_gBuffer parms_gbuffer;
			SetupParmsGBuffer( parms_gbuffer );
			InitPassGBuffer( parms_gbuffer, this, params );

			defParms_shadow parms_shadow;
			SetupParmsShadow( parms_shadow );
			InitPassShadowPass( parms_shadow, this, params );
		}

		if( params[BASETEXTURE]->IsDefined() )
		{
			LoadTexture( BASETEXTURE );
		}
	}

	inline void DrawFlora( IMaterialVar **params, IShaderShadow* pShaderShadow,
		IShaderDynamicAPI* pShaderAPI, VertexCompressionType_t vertexCompression, CDeferredPerMaterialContextData *pDeferredContext  ) 
	{
		const bool bIsAlphaTested = IS_FLAG_SET( MATERIAL_VAR_ALPHATEST ) != 0;
		const bool bHasVertexColor = IS_FLAG_SET( MATERIAL_VAR_VERTEXCOLOR );
		const bool bModel = true;
		const bool bSRGBWrite = true;

		bool bTreeSway = ( GetIntParam( TREESWAY, params, 0 ) != 0 );
		int nTreeSwayMode = GetIntParam( TREESWAY, params, 0 );
		nTreeSwayMode = clamp( nTreeSwayMode, 0, 2 );
		
		SHADOW_STATE
		{
			// Diffuse modulation
			PI_BeginCommandBuffer();
			PI_SetModulationPixelShaderDynamicState_LinearColorSpace( 1 );
			PI_EndCommandBuffer();

			SetInitialShadowState();

			// Alpha test: FIXME: shouldn't this be handled in Shader_t::SetInitialShadowState
			pShaderShadow->EnableAlphaTest( bIsAlphaTested );
			if ( params[ALPHATESTREFERENCE]->GetFloatValue() > 0.0f )
			{
				pShaderShadow->AlphaFunc( SHADER_ALPHAFUNC_GEQUAL, params[ALPHATESTREFERENCE]->GetFloatValue() );
			}

			DefaultFog();

			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
			pShaderShadow->EnableSRGBRead( SHADER_SAMPLER0, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER13, true ); // Deferred light 1
			pShaderShadow->EnableTexture( SHADER_SAMPLER14, true ); // Deferred light 2

			int iVFmtFlags = VERTEX_POSITION|VERTEX_NORMAL;
			int iUserDataSize = 0;

			pShaderShadow->EnableSRGBWrite( bSRGBWrite );

			// texcoord0 : base texcoord
			int pTexCoordDim[3] = { 2, 2, 3 };
			int nTexCoordCount = 1;

			// This shader supports compressed vertices, so OR in that flag:
			iVFmtFlags |= VERTEX_FORMAT_COMPRESSED;

			if ( bHasVertexColor )
			{
				iVFmtFlags |= VERTEX_COLOR;
			}

			pShaderShadow->VertexShaderVertexFormat( iVFmtFlags, nTexCoordCount, pTexCoordDim, iUserDataSize );

			// The vertex shader uses the vertex id stream
			if( g_pHardwareConfig->HasFastVertexTextures() )
			{
				SET_FLAGS2( MATERIAL_VAR2_USES_VERTEXID );
				SET_FLAGS2( MATERIAL_VAR2_SUPPORTS_TESSELLATION );
			}

			// Vertex Shader
			DECLARE_STATIC_VERTEX_SHADER( flora_vs30 );
			SET_STATIC_VERTEX_SHADER_COMBO( VERTEXCOLOR, bHasVertexColor );
			SET_STATIC_VERTEX_SHADER_COMBO( TREESWAY, bTreeSway ? nTreeSwayMode : 0 );		
			SET_STATIC_VERTEX_SHADER( flora_vs30 );

			// Pixel Shader
			DECLARE_STATIC_PIXEL_SHADER( flora_ps30 );
			SET_STATIC_PIXEL_SHADER_COMBO( VERTEXCOLOR, bHasVertexColor );
			SET_STATIC_PIXEL_SHADER( flora_ps30 );

			pShaderShadow->EnableAlphaWrites( true );
		}
		DYNAMIC_STATE
		{
			Assert( pDeferredContext != NULL );

			if ( pDeferredContext->m_bMaterialVarsChanged || !pDeferredContext->HasCommands( CDeferredPerMaterialContextData::DEFSTAGE_COMPOSITE )
				|| building_cubemaps.GetBool() )
			{
				tmpBuf.Reset();

				float fWriteDepthToAlpha = /*bWriteDepthToAlpha && IsPC() ? 1.0f :*/ 0.0f;
				float fWriteWaterFogToDestAlpha = /*bWriteWaterFogToAlpha ? 1.0f :*/ 0.0f;
				float fVertexAlpha = /*bHasVertexAlpha ? 1.0f :*/ 0.0f;
				float fBlendTintByBaseAlpha = /*IsBoolSet( info.m_nBlendTintByBaseAlpha, params ) ? 1.0f :*/ 0.0f;

				// Controls for lerp-style paths through shader code (used by bump and non-bump)
				float vShaderControls[4] = { 1.0f - fBlendTintByBaseAlpha, fWriteDepthToAlpha, fWriteWaterFogToDestAlpha, fVertexAlpha };
				tmpBuf.SetPixelShaderConstant( 12, vShaderControls, 1 );
				float flEyeW = TextureIsTranslucent( BASETEXTURE, true ) ? 1.0f : 0.0f;
				tmpBuf.StoreEyePosInPixelShaderConstant( 20, flEyeW );

				// Mostly fixed treesway parameters
				if ( bTreeSway )
				{
					float flParams[4];
					flParams[0] = GetFloatParam( TREESWAYSPEEDHIGHWINDMULTIPLIER, params, 2.0f );
					flParams[1] = GetFloatParam( TREESWAYSCRUMBLEFALLOFFEXP, params, 1.0f );
					flParams[2] = GetFloatParam( TREESWAYFALLOFFEXP, params, 1.0f );
					flParams[3] = GetFloatParam( TREESWAYSCRUMBLESPEED, params, 3.0f );
					tmpBuf.SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_4, flParams );

					flParams[0] = GetFloatParam( TREESWAYSPEEDLERPSTART, params, 3.0f );
					flParams[1] = GetFloatParam( TREESWAYSPEEDLERPEND, params, 6.0f );
					tmpBuf.SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_5, flParams );

					flParams[0] = GetFloatParam( TREESWAYHEIGHT, params, 1000.0f );
					flParams[1] = GetFloatParam( TREESWAYSTARTHEIGHT, params, 0.1f );
					flParams[2] = GetFloatParam( TREESWAYRADIUS, params, 300.0f );
					flParams[3] = GetFloatParam( TREESWAYSTARTRADIUS, params, 0.2f );
					tmpBuf.SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_6, flParams );

					flParams[0] = GetFloatParam( TREESWAYSPEED, params, 1.0f );
					flParams[1] = GetFloatParam( TREESWAYSTRENGTH, params, 10.0f );
					flParams[2] = GetFloatParam( TREESWAYSCRUMBLEFREQUENCY, params, 12.0f );
					flParams[3] = GetFloatParam( TREESWAYSCRUMBLESTRENGTH, params, 10.0f );
					tmpBuf.SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_7, flParams );
				}

				tmpBuf.End();

				pDeferredContext->SetCommands( CDeferredPerMaterialContextData::DEFSTAGE_COMPOSITE, tmpBuf.Copy() );
			}

			// Reset render state
			pShaderAPI->SetDefaultState();

			BindTexture( SHADER_SAMPLER0, BASETEXTURE );							// Base Map 1

			BindTexture( SHADER_SAMPLER13, GetDeferredExt()->GetTexture_LightAccum()  );
			BindTexture( SHADER_SAMPLER14, GetDeferredExt()->GetTexture_LightAccum2()  );
			int x, y, w, t;
			pShaderAPI->GetCurrentViewport( x, y, w, t );
			float fl1[4] = { 1.0f / w, 1.0f / t, 0, 0 };

			pShaderAPI->SetPixelShaderConstant( 3, fl1 );

			const lightData_Global_t &data = GetDeferredExt()->GetLightData_Global();
			//Vector4D vecLight( 0, 0, 1, 0 );
			//pShaderAPI->SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_0, (data.vecLight).Base() );
			pShaderAPI->SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_2, data.diff.Base() );
			//pShaderAPI->SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_3, MakeHalfAmbient( data.ambl, data.ambh ).Base() );

			pShaderAPI->SetPixelShaderFogParams( 21 );

			// Set Vertex Shader Combos
			DECLARE_DYNAMIC_VERTEX_SHADER( flora_vs30 );
			SET_DYNAMIC_VERTEX_SHADER_COMBO( SKINNING, (bModel && pShaderAPI->GetCurrentNumBones() > 0) ? 1 : 0 );
			SET_DYNAMIC_VERTEX_SHADER_COMBO( COMPRESSED_VERTS, (int)vertexCompression );
			SET_DYNAMIC_VERTEX_SHADER( flora_vs30 );

			// Set Pixel Shader Combos
			DECLARE_DYNAMIC_PIXEL_SHADER( flora_ps30 );
			SET_DYNAMIC_PIXEL_SHADER( flora_ps30 );

			// Treesway: Time and wind direction parameters (changes all the time)
			if ( bTreeSway )
			{
				float flParams[4];
				flParams[1] = pShaderAPI->CurrentTime();
				Vector windDir = pShaderAPI->GetVectorRenderingParameter( VECTOR_RENDERPARM_WIND_DIRECTION );
				flParams[2] = windDir.x;
				flParams[3] = windDir.y;
				pShaderAPI->SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_8, flParams );
			}

			pShaderAPI->ExecuteCommandBuffer( pDeferredContext->GetCommands( CDeferredPerMaterialContextData::DEFSTAGE_COMPOSITE ) );
		}
		Draw();
	}

	SHADER_DRAW
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

		if ( pShaderShadow != NULL ||
			iDeferredRenderStage == DEFERRED_RENDER_STAGE_COMPOSITION )
		{
			DrawFlora( params, pShaderShadow, pShaderAPI, vertexCompression, pDefContext );
		}
		else
			Draw( false );

		if ( pShaderAPI != NULL && pDefContext->m_bMaterialVarsChanged )
			pDefContext->m_bMaterialVarsChanged = false;
	}
END_SHADER
