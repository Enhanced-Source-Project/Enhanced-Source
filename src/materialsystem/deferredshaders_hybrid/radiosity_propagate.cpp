
#include "deferred_includes.h"

#include "radiosity_propagate_ps30.inc"
#include "radiosity_propagate_vs30.inc"

BEGIN_VS_SHADER( RADIOSITY_PROPAGATE, "" )
	BEGIN_SHADER_PARAMS

		SHADER_PARAM( NORMALMAP, SHADER_PARAM_TYPE_TEXTURE, "", "" )
		SHADER_PARAM( BLUR, SHADER_PARAM_TYPE_BOOL, "", "" )

	END_SHADER_PARAMS

	SHADER_INIT_PARAMS()
	{
	}

	SHADER_INIT
	{
		Assert( params[ BASETEXTURE ]->IsDefined() );
		Assert( params[ NORMALMAP ]->IsDefined() );

		if ( params[ BASETEXTURE ]->IsDefined() )
			LoadTexture( BASETEXTURE );

		if ( params[ NORMALMAP ]->IsDefined() )
			LoadTexture( NORMALMAP );
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
			pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );

			int iTexDim[] = { 4, 4 };
			pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 2, iTexDim, 0 );

			DECLARE_STATIC_VERTEX_SHADER( radiosity_propagate_vs30 );
			SET_STATIC_VERTEX_SHADER( radiosity_propagate_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( radiosity_propagate_ps30 );
			SET_STATIC_PIXEL_SHADER_COMBO( BLUR, PARM_INT( BLUR ) );
			SET_STATIC_PIXEL_SHADER( radiosity_propagate_ps30 );
		}
		DYNAMIC_STATE
		{
			pShaderAPI->SetDefaultState();

			DECLARE_DYNAMIC_VERTEX_SHADER( radiosity_propagate_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( radiosity_propagate_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( radiosity_propagate_ps30 );
			SET_DYNAMIC_PIXEL_SHADER( radiosity_propagate_ps30 );

			BindTexture( SHADER_SAMPLER0, BASETEXTURE );
			BindTexture( SHADER_SAMPLER1, NORMALMAP );
		}

		Draw();
	}
END_SHADER