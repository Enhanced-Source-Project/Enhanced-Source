
#include "deferred_includes.h"

#include "defconstruct_vs30.inc"
#include "lightingpass_global_ps30.inc"

ConVar mat_def_shadow_threshold( "mat_def_shadow_threshold", "0.85" );

BEGIN_VS_SHADER( LIGHTING_GLOBAL, "" )
	BEGIN_SHADER_PARAMS

	END_SHADER_PARAMS

	SHADER_INIT_PARAMS()
	{
	}

	SHADER_INIT
	{
	}

	SHADER_FALLBACK
	{
		return 0;
	}

	SHADER_DRAW
	{
		SHADOW_STATE
		{
			pShaderShadow->SetDefaultState();
			pShaderShadow->EnableDepthTest( false );
			pShaderShadow->EnableDepthWrites( false );
			pShaderShadow->EnableAlphaWrites( true );

			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER2, true );

			pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, NULL, 0 );

			DECLARE_STATIC_VERTEX_SHADER( defconstruct_vs30 );
			SET_STATIC_VERTEX_SHADER_COMBO( USEWORLDTRANSFORM, 0 );
			SET_STATIC_VERTEX_SHADER_COMBO( SENDWORLDPOS, 0 );
			SET_STATIC_VERTEX_SHADER( defconstruct_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( lightingpass_global_ps30 );
			SET_STATIC_PIXEL_SHADER( lightingpass_global_ps30 );

			// Per-instance state
			/*PI_BeginCommandBuffer();
			PI_SetPixelShaderAmbientLightCube( 20 );
			PI_EndCommandBuffer();*/
		}
		DYNAMIC_STATE
		{
			pShaderAPI->SetDefaultState();

			lightData_Global_t data = GetDeferredExt()->GetLightData_Global();

			AssertMsg( data.bEnabled, "I shouldn't be drawn at all." );

			DECLARE_DYNAMIC_VERTEX_SHADER( defconstruct_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( defconstruct_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( lightingpass_global_ps30 );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( HAS_SHADOW, data.bShadow );
			SET_DYNAMIC_PIXEL_SHADER( lightingpass_global_ps30 );

			BindTexture( SHADER_SAMPLER0, GetDeferredExt()->GetTexture_Normals() );
			BindTexture( SHADER_SAMPLER1, GetDeferredExt()->GetTexture_Depth() );

			if ( data.bShadow )
			{
				BindTexture( SHADER_SAMPLER2, GetDeferredExt()->GetTexture_ShadowDepth_Ortho( 0 ) );

				COMPILE_TIME_ASSERT( CSM_USE_COMPOSITED_TARGET == 1 ); // This shader relies on composited cascades!
				COMPILE_TIME_ASSERT( SHADOW_NUM_CASCADES == 2 ); // This shader has been made for 2 cascades!

				CommitShadowProjectionConstants_Ortho_Composite( pShaderAPI, 2, 2 );
			}

			CommitGlobalLightForward( pShaderAPI, 1 );

			CommitBaseDeferredConstants_Frustum( pShaderAPI, VERTEX_SHADER_SHADER_SPECIFIC_CONST_0 );
			CommitBaseDeferredConstants_Origin( pShaderAPI, 0 );

			pShaderAPI->SetPixelShaderConstant( 16, data.diff.Base() );
			pShaderAPI->SetPixelShaderConstant( 17, data.ambh.Base() );
			pShaderAPI->SetPixelShaderConstant( 18, MakeHalfAmbient( data.ambl, data.ambh ).Base() );

			Vector shadowConstants( mat_def_shadow_threshold.GetFloat(), 0.0, 0.0 );
			pShaderAPI->SetPixelShaderConstant( 19, shadowConstants.Base() );
		}

		Draw();
	}

END_SHADER