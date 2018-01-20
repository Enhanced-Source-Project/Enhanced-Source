
#include "deferred_includes.h"

#include "screenspace_vs30.inc"
#include "screenspace_combine_ps30.inc"


BEGIN_VS_SHADER( SCREENSPACE_COMBINE, "" )
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

			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );

			pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, NULL, 0 );

			DECLARE_STATIC_VERTEX_SHADER( screenspace_vs30 );
			SET_STATIC_VERTEX_SHADER( screenspace_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( screenspace_combine_ps30 );
			SET_STATIC_PIXEL_SHADER( screenspace_combine_ps30 );
		}
		DYNAMIC_STATE
		{
			pShaderAPI->SetDefaultState();

			DECLARE_DYNAMIC_VERTEX_SHADER( screenspace_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( screenspace_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( screenspace_combine_ps30 );
			SET_DYNAMIC_PIXEL_SHADER( screenspace_combine_ps30 );

#if DEFCFG_DEFERRED_SHADING == 1
			BindTexture( SHADER_SAMPLER0, GetDeferredExt()->GetTexture_Albedo() );
#else
			Assert( 0 );
#endif
		}

		Draw();
	}
END_SHADER