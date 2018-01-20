
#include "deferred_includes.h"

#include "defconstruct_vs30.inc"
#include "radiosity_blend_ps30.inc"


BEGIN_VS_SHADER( RADIOSITY_BLEND, "" )
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
			pShaderShadow->EnableAlphaWrites( false );

			EnableAlphaBlending( SHADER_BLEND_ONE, SHADER_BLEND_ONE );

			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );

			pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, NULL, 0 );

			DECLARE_STATIC_VERTEX_SHADER( defconstruct_vs30 );
			SET_STATIC_VERTEX_SHADER_COMBO( USEWORLDTRANSFORM, 0 );
			SET_STATIC_VERTEX_SHADER_COMBO( SENDWORLDPOS, 0 );
			SET_STATIC_VERTEX_SHADER( defconstruct_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( radiosity_blend_ps30 );
			SET_STATIC_PIXEL_SHADER( radiosity_blend_ps30 );
		}
		DYNAMIC_STATE
		{
			pShaderAPI->SetDefaultState();

			DECLARE_DYNAMIC_VERTEX_SHADER( defconstruct_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( defconstruct_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( radiosity_blend_ps30 );
			SET_DYNAMIC_PIXEL_SHADER( radiosity_blend_ps30 );

			BindTexture( SHADER_SAMPLER0, GetDeferredExt()->GetTexture_Depth() );
			BindTexture( SHADER_SAMPLER1, GetDeferredExt()->GetTexture_RadBuffer( 0 ) );

			CommitBaseDeferredConstants_Frustum( pShaderAPI, VERTEX_SHADER_SHADER_SPECIFIC_CONST_0 );
			CommitBaseDeferredConstants_Origin( pShaderAPI, 0 );

			const radiosityData_t &data = GetDeferredExt()->GetRadiosityData();

			const Vector &vecOrigin = data.vecOrigin[0];
			const Vector &vecOrigin2 = data.vecOrigin[1];
			float fl1[4] = { vecOrigin.x, vecOrigin.y, vecOrigin.z, 0 };
			float fl2[4] = { vecOrigin2.x, vecOrigin2.y, vecOrigin2.z, 0 };
			pShaderAPI->SetPixelShaderConstant( 1, fl1 );
			pShaderAPI->SetPixelShaderConstant( 2, fl2 );

			float fl3[4] = { deferred_radiosity_multiplier.GetFloat() };
			pShaderAPI->SetPixelShaderConstant( 3, fl3 );
		}

		Draw();
	}
END_SHADER