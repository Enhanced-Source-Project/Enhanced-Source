
#include "deferred_includes.h"

#include "tier0/memdbgon.h"

#include "screenspace_vs30.inc"
#include "gaussianblur_6_ps30.inc"

BEGIN_VS_SHADER( GAUSSIAN_BLUR_6, "" )
	BEGIN_SHADER_PARAMS

		SHADER_PARAM( ISVERTICAL, SHADER_PARAM_TYPE_BOOL, "0", "" )

	END_SHADER_PARAMS

	SHADER_INIT_PARAMS()
	{
	}

	SHADER_INIT
	{
		Assert( params[ BASETEXTURE ]->IsDefined() );

		if ( params[ BASETEXTURE ]->IsDefined() )
			LoadTexture( BASETEXTURE );
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

			pShaderShadow->EnableCulling( false );

			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );

			pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, NULL, 0 );

			DECLARE_STATIC_VERTEX_SHADER( screenspace_vs30 );
			SET_STATIC_VERTEX_SHADER( screenspace_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( gaussianblur_6_ps30 );
			SET_STATIC_PIXEL_SHADER( gaussianblur_6_ps30 );
		}
		DYNAMIC_STATE
		{
			pShaderAPI->SetDefaultState();

			DECLARE_DYNAMIC_VERTEX_SHADER( screenspace_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( screenspace_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( gaussianblur_6_ps30 );
			SET_DYNAMIC_PIXEL_SHADER( gaussianblur_6_ps30 );

			BindTexture( SHADER_SAMPLER0, BASETEXTURE );

			int w, t;
			pShaderAPI->GetBackBufferDimensions( w, t );
			float fl4[4] = { 4.0f / w, 4.0f / t, 0, 0 };

			if ( params[ ISVERTICAL ]->GetIntValueFast() != 0 )
				fl4[ 0 ] = 0;
			else
				fl4[ 1 ] = 0;

			pShaderAPI->SetPixelShaderConstant( 0, fl4 );
		}

		Draw();
	}

END_SHADER