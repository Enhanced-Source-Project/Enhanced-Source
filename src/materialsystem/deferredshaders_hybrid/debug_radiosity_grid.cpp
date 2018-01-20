
#include "deferred_includes.h"

#include "debug_radiosity_grid_vs30.inc"
#include "debug_radiosity_grid_ps30.inc"

BEGIN_VS_SHADER( DEBUG_RADIOSITY_GRID, "" )
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
			pShaderShadow->EnableDepthTest( true );
			pShaderShadow->EnableDepthWrites( false );

			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
			//EnableAlphaBlending( SHADER_BLEND_ONE, SHADER_BLEND_ONE );

			pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, NULL, 0 );

			DECLARE_STATIC_VERTEX_SHADER( debug_radiosity_grid_vs30 );
			SET_STATIC_VERTEX_SHADER( debug_radiosity_grid_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( debug_radiosity_grid_ps30 );
			SET_STATIC_PIXEL_SHADER( debug_radiosity_grid_ps30 );
		}
		DYNAMIC_STATE
		{
			pShaderAPI->SetDefaultState();

			DECLARE_DYNAMIC_VERTEX_SHADER( debug_radiosity_grid_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( debug_radiosity_grid_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( debug_radiosity_grid_ps30 );
			SET_DYNAMIC_PIXEL_SHADER( debug_radiosity_grid_ps30 );

			BindTexture( SHADER_SAMPLER0, GetDeferredExt()->GetTexture_RadBuffer( 0 ) );
		}

		Draw();
	}

END_SHADER