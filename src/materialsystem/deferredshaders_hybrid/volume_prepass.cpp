
#include "deferred_includes.h"

#include "volume_prepass_vs30.inc"
#include "volume_prepass_ps30.inc"

BEGIN_VS_SHADER( VOLUME_PREPASS, "" )
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

			pShaderShadow->EnableCulling( true );

			pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, NULL, 0 );

			DECLARE_STATIC_VERTEX_SHADER( volume_prepass_vs30 );
			SET_STATIC_VERTEX_SHADER( volume_prepass_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( volume_prepass_ps30 );
			SET_STATIC_PIXEL_SHADER( volume_prepass_ps30 );
		}
		DYNAMIC_STATE
		{
			pShaderAPI->SetDefaultState();

			DECLARE_DYNAMIC_VERTEX_SHADER( volume_prepass_vs30 );
			SET_DYNAMIC_VERTEX_SHADER( volume_prepass_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( volume_prepass_ps30 );
			SET_DYNAMIC_PIXEL_SHADER( volume_prepass_ps30 );

			float vPos[4] = {0,0,0,0};
			pShaderAPI->GetWorldSpaceCameraPosition( vPos );
			float zScale[4] = {GetDeferredExt()->GetZScale(),0,0,0};
			pShaderAPI->SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_0, vPos );
			pShaderAPI->SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_1, GetDeferredExt()->GetForwardBase() );
			pShaderAPI->SetVertexShaderConstant( VERTEX_SHADER_SHADER_SPECIFIC_CONST_2, zScale );
		}

		Draw();
	}
END_SHADER