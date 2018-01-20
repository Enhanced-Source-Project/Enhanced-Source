
#include "cbase.h"
#include "deferred/deferred_shared_common.h"

#include "iinput.h"

extern ConVar r_flashlightvolumetrics;

CFlashlightEffectDeferred::CFlashlightEffectDeferred( int nEntIndex, const char *pszTextureName, float flFov, float flFarZ, float flLinearAtten )
	: CFlashlightEffect( nEntIndex, pszTextureName, flFov, flFarZ, flLinearAtten )
{
	m_pDefLight = NULL;
}

CFlashlightEffectDeferred::~CFlashlightEffectDeferred()
{
	LightOff();
}

void CFlashlightEffectDeferred::UpdateLightProjection( FlashlightState_t &state )
{
	if ( m_pDefLight == NULL )
	{
		m_pDefLight = new def_light_t();
		GetLightingManager()->AddLight( m_pDefLight );

		if ( !IsErrorTexture( state.m_pSpotlightTexture ) )
		{
			m_pDefLight->SetCookie( new CDefCookieTexture( state.m_pSpotlightTexture ) );
			m_pDefLight->iFlags |= DEFLIGHT_COOKIE_ENABLED;
		}
	}

	m_pDefLight->pos = state.m_vecLightOrigin;
	QuaternionAngles( state.m_quatOrientation, m_pDefLight->ang );

	m_pDefLight->col_diffuse.Init( state.m_Color[0],
		state.m_Color[1],
		state.m_Color[2] );

	m_pDefLight->flRadius = state.m_FarZ;
	m_pDefLight->iLighttype = DEFLIGHTTYPE_SPOT;

	float flFov = MAX( state.m_fHorizontalFOVDegrees, state.m_fVerticalFOVDegrees );
	m_pDefLight->flSpotCone_Outer = SPOT_DEGREE_TO_RAD( flFov );
	m_pDefLight->flSpotCone_Inner = 1;

	if ( m_pDefLight->HasShadow() != state.m_bEnableShadows )
	{
		if ( state.m_bEnableShadows )
			m_pDefLight->iFlags |= DEFLIGHT_SHADOW_ENABLED;
		else
			m_pDefLight->iFlags &= ~DEFLIGHT_SHADOW_ENABLED;
	}

	const bool bDoVolumetrics = ( r_flashlightvolumetrics.GetBool()
								  || state.m_bVolumetric );
		//&& input->CAM_IsThirdPerson();

	if ( m_pDefLight->HasVolumetrics() != bDoVolumetrics )
	{
		if ( bDoVolumetrics )
			m_pDefLight->iFlags |= DEFLIGHT_VOLUMETRICS_ENABLED;
		else
			m_pDefLight->iFlags &= ~DEFLIGHT_VOLUMETRICS_ENABLED;

		m_pDefLight->MakeDirtyRenderMesh();
	}

	m_pDefLight->MakeDirtyXForms();
}

void CFlashlightEffectDeferred::LightOff()
{
	if ( m_pDefLight != NULL )
	{
		GetLightingManager()->RemoveLight( m_pDefLight );
		delete m_pDefLight;
		m_pDefLight = NULL;
	}
}