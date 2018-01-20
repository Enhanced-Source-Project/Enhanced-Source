
#include "deferred_includes.h"

#include "globallitgeneric_vs30.inc"
#include "globallitgeneric_ps30.inc"

#include "tier0/memdbgon.h"


BEGIN_VS_SHADER( GlobalLitSimple, "" )
	BEGIN_SHADER_PARAMS
		SHADER_PARAM( FULLBRIGHT, SHADER_PARAM_TYPE_BOOL, "0", "Render full bright" )
		SHADER_PARAM( ALPHATESTREFERENCE, SHADER_PARAM_TYPE_FLOAT, "0.5", "" )

		SHADER_PARAM( FOW, SHADER_PARAM_TYPE_TEXTURE, "_rt_fog_of_war", "FoW Render Target" )

		// For deferred
		SHADER_PARAM( PHONG_EXP, SHADER_PARAM_TYPE_FLOAT, "", "" )
	END_SHADER_PARAMS

	void SetupParmsGBuffer( defParms_gBuffer &p )
	{
		p.bModel = false;

		p.iAlbedo = BASETEXTURE;
		p.iAlphatestRef = ALPHATESTREFERENCE;
		p.iPhongExp = PHONG_EXP;
	}

	void SetupParmsShadow( defParms_shadow &p )
	{
		p.bModel = false;
		p.iAlbedo = BASETEXTURE;
		p.iAlphatestRef = ALPHATESTREFERENCE;
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
		SET_FLAGS2( MATERIAL_VAR2_SUPPORTS_HW_SKINNING );

		if ( g_pHardwareConfig->HasFastVertexTextures() )
			SET_FLAGS2( MATERIAL_VAR2_USES_VERTEXID );

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

	SHADER_FALLBACK
	{
		const bool bIsDecal = IS_FLAG_SET( MATERIAL_VAR_DECAL );
		const bool bHasVertexColor = IS_FLAG_SET( MATERIAL_VAR_VERTEXCOLOR );
		if( bIsDecal )
			return "LightmappedGeneric";
		if( params[FULLBRIGHT]->GetIntValue() == 0 && !bIsDecal && !bHasVertexColor )
		{
			return "VertexLitGeneric";
		}

		return 0;
	}

	inline void DrawGlobalLitGeneric( IMaterialVar **params, IShaderShadow* pShaderShadow,
		IShaderDynamicAPI* pShaderAPI, VertexCompressionType_t vertexCompression, CBasePerMaterialContextData *pContextDataPtr ) 
	{
		bool bHasFoW = ( ( params[FOW]->IsTexture() != 0 ) );
		if ( bHasFoW == true )
		{
			ITexture *pTexture = params[FOW]->GetTextureValue();
			if ( ( pTexture->GetFlags() & TEXTUREFLAGS_RENDERTARGET ) == 0 )
			{
				bHasFoW = false;
			}
		}

		const bool bIsFullbright = params[FULLBRIGHT]->GetIntValue() != 0;
		const bool bIsAlphaTested = IS_FLAG_SET( MATERIAL_VAR_ALPHATEST ) != 0;
		const bool bHasVertexColor = IS_FLAG_SET( MATERIAL_VAR_VERTEXCOLOR );

		SHADOW_STATE
		{
			SetInitialShadowState( );

			// Alpha test: FIXME: shouldn't this be handled in Shader_t::SetInitialShadowState
			pShaderShadow->EnableAlphaTest( bIsAlphaTested );
			if ( params[ALPHATESTREFERENCE]->GetFloatValue() > 0.0f )
			{
				pShaderShadow->AlphaFunc( SHADER_ALPHAFUNC_GEQUAL, params[ALPHATESTREFERENCE]->GetFloatValue() );
			}

			DefaultFog();

			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );

			if ( bHasFoW )
			{
				pShaderShadow->EnableTexture( SHADER_SAMPLER10, true );
			}

			if( !bIsFullbright )
			{
				pShaderShadow->EnableTexture( SHADER_SAMPLER13, true );
			}

			int iVFmtFlags = VERTEX_POSITION;
			int iUserDataSize = 0;

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

			DECLARE_STATIC_VERTEX_SHADER( globallitgeneric_vs30 );
			SET_STATIC_VERTEX_SHADER_COMBO( FOW, bHasFoW );
			SET_STATIC_VERTEX_SHADER_COMBO( VERTEXCOLOR, bHasVertexColor );
			SET_STATIC_VERTEX_SHADER( globallitgeneric_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( globallitgeneric_ps30 );
			SET_STATIC_PIXEL_SHADER_COMBO( FULLBRIGHT, bIsFullbright );
			SET_STATIC_PIXEL_SHADER_COMBO( FOW, bHasFoW );
			SET_STATIC_PIXEL_SHADER_COMBO( VERTEXCOLOR, bHasVertexColor );
			SET_STATIC_PIXEL_SHADER( globallitgeneric_ps30 );

		}
		DYNAMIC_STATE
		{
			pShaderAPI->SetDefaultState();

			BindTexture( SHADER_SAMPLER0, BASETEXTURE );							// Base Map 1

			if ( bHasFoW )
			{
				BindTexture( SHADER_SAMPLER10, FOW, -1 );

				float	vFoWSize[ 4 ];
				Vector	vMins = pShaderAPI->GetVectorRenderingParameter( VECTOR_RENDERPARM_GLOBAL_FOW_MINS );
				Vector	vMaxs = pShaderAPI->GetVectorRenderingParameter( VECTOR_RENDERPARM_GLOBAL_FOW_MAXS );
				vFoWSize[ 0 ] = vMins.x;
				vFoWSize[ 1 ] = vMins.y;
				vFoWSize[ 2 ] = vMaxs.x - vMins.x;
				vFoWSize[ 3 ] = vMaxs.y - vMins.y;
				pShaderAPI->SetVertexShaderConstant( 26, vFoWSize );
			}

			if( !bIsFullbright )
			{
				BindTexture( SHADER_SAMPLER13, GetDeferredExt()->GetTexture_LightAccum()  );
				int x, y, w, t;
				pShaderAPI->GetCurrentViewport( x, y, w, t );
				float fl1[4] = { 1.0f / w, 1.0f / t, 0, 0 };

				pShaderAPI->SetPixelShaderConstant( 3, fl1 );
			}

			DECLARE_DYNAMIC_VERTEX_SHADER( globallitgeneric_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( globallitgeneric_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( globallitgeneric_ps30 );
			//SET_DYNAMIC_PIXEL_SHADER_COMBO( PIXELFOGTYPE, pShaderAPI->GetPixelFogCombo() );
			SET_DYNAMIC_PIXEL_SHADER( globallitgeneric_ps30 );
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
			DrawGlobalLitGeneric( params, pShaderShadow, pShaderAPI, vertexCompression, pDefContext );
		}
		else
			Draw( false );

		if ( pShaderAPI != NULL && pDefContext->m_bMaterialVarsChanged )
			pDefContext->m_bMaterialVarsChanged = false;
	}

END_SHADER
