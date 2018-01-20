
#include "deferred_includes.h"

#include "defconstruct_vs30.inc"
#include "screenspace_shading_ps30.inc"


BEGIN_VS_SHADER( SCREENSPACE_SHADING, "" )
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
		const bool bUseSRGB = DEFCFG_USE_SRGB_CONVERSION != 0;

		SHADOW_STATE
		{
			pShaderShadow->SetDefaultState();

			pShaderShadow->EnableDepthTest( false );
			pShaderShadow->EnableDepthWrites( false );
			pShaderShadow->EnableAlphaWrites( false );

			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER2, true );
			pShaderShadow->EnableTexture( SHADER_SAMPLER3, true );

			pShaderShadow->EnableSRGBRead( SHADER_SAMPLER1, bUseSRGB );
			pShaderShadow->EnableSRGBWrite( bUseSRGB );

			pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, NULL, 0 );

			DECLARE_STATIC_VERTEX_SHADER( defconstruct_vs30 );
			SET_STATIC_VERTEX_SHADER_COMBO( USEWORLDTRANSFORM, 0 );
			SET_STATIC_VERTEX_SHADER_COMBO( SENDWORLDPOS, 0 );
			SET_STATIC_VERTEX_SHADER( defconstruct_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( screenspace_shading_ps30 );
			SET_STATIC_PIXEL_SHADER( screenspace_shading_ps30 );
		}
		DYNAMIC_STATE
		{
			pShaderAPI->SetDefaultState();

			DECLARE_DYNAMIC_VERTEX_SHADER( defconstruct_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( defconstruct_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( screenspace_shading_ps30 );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( RADIOSITY, GetDeferredExt()->IsRadiosityEnabled() ? 1 : 0 );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( PIXELFOGTYPE, pShaderAPI->GetPixelFogCombo() );
			SET_DYNAMIC_PIXEL_SHADER( screenspace_shading_ps30 );

			BindTexture( SHADER_SAMPLER0, GetDeferredExt()->GetTexture_Depth() );
#if DEFCFG_DEFERRED_SHADING == 1
			BindTexture( SHADER_SAMPLER1, GetDeferredExt()->GetTexture_Albedo() );
#else
			Assert( 0 );
#endif
			BindTexture( SHADER_SAMPLER2, GetDeferredExt()->GetTexture_LightAccum() );
			BindTexture( SHADER_SAMPLER3, GetDeferredExt()->GetTexture_RadBuffer( 0 ) );

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

			pShaderAPI->SetPixelShaderFogParams( 4 );
		}

		Draw();
	}
END_SHADER