//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: spawn and think functions for editor-placed lights
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "lights.h"
#include "world.h"

#ifdef DEFERRED
#include "deferred/deferred_shared_common.h"
#endif // DEFERRED

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS( light, CLight );

BEGIN_DATADESC( CLight )

	DEFINE_FIELD( m_iCurrentFade, FIELD_CHARACTER),
	DEFINE_FIELD( m_iTargetFade, FIELD_CHARACTER),

	DEFINE_KEYFIELD( m_iStyle, FIELD_INTEGER, "style" ),
	DEFINE_KEYFIELD( m_iDefaultStyle, FIELD_INTEGER, "defaultstyle" ),
	DEFINE_KEYFIELD( m_iszPattern, FIELD_STRING, "pattern" ),

	// Fuctions
	DEFINE_FUNCTION( FadeThink ),

	// Inputs
	DEFINE_INPUTFUNC( FIELD_STRING, "SetPattern", InputSetPattern ),
	DEFINE_INPUTFUNC( FIELD_STRING, "FadeToPattern", InputFadeToPattern ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"Toggle", InputToggle ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"TurnOn", InputTurnOn ),
	DEFINE_INPUTFUNC( FIELD_VOID,	"TurnOff", InputTurnOff ),

END_DATADESC()



//
// Cache user-entity-field values until spawn is called.
//
bool CLight::KeyValue( const char *szKeyName, const char *szValue )
{
	if (FStrEq(szKeyName, "pitch"))
	{
		QAngle angles = GetAbsAngles();
		angles.x = atof(szValue);
		SetAbsAngles( angles );
	}
	else
	{
		return BaseClass::KeyValue( szKeyName, szValue );
	}

	return true;
}

// Light entity
// If targeted, it will toggle between on or off.
void CLight::Spawn( void )
{
	if (!GetEntityName())
	{       // inert light
		UTIL_Remove( this );
		return;
	}
	
	if (m_iStyle >= 32)
	{
		if ( m_iszPattern == NULL_STRING && m_iDefaultStyle > 0 )
		{
			m_iszPattern = MAKE_STRING(GetDefaultLightstyleString(m_iDefaultStyle));
		}

		if (FBitSet(m_spawnflags, SF_LIGHT_START_OFF))
			engine->LightStyle(m_iStyle, "a");
		else if (m_iszPattern != NULL_STRING)
			engine->LightStyle(m_iStyle, (char *)STRING( m_iszPattern ));
		else
			engine->LightStyle(m_iStyle, "m");
	}
}


void CLight::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if (m_iStyle >= 32)
	{
		if ( !ShouldToggle( useType, !FBitSet(m_spawnflags, SF_LIGHT_START_OFF) ) )
			return;

		Toggle();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Turn the light on
//-----------------------------------------------------------------------------
void CLight::TurnOn( void )
{
	if ( m_iszPattern != NULL_STRING )
	{
		engine->LightStyle( m_iStyle, (char *) STRING( m_iszPattern ) );
	}
	else
	{
		engine->LightStyle( m_iStyle, "m" );
	}

	CLEARBITS( m_spawnflags, SF_LIGHT_START_OFF );
}

//-----------------------------------------------------------------------------
// Purpose: Turn the light off
//-----------------------------------------------------------------------------
void CLight::TurnOff( void )
{
	engine->LightStyle( m_iStyle, "a" );
	SETBITS( m_spawnflags, SF_LIGHT_START_OFF );
}

//-----------------------------------------------------------------------------
// Purpose: Toggle the light on/off
//-----------------------------------------------------------------------------
void CLight::Toggle( void )
{
	//Toggle it
	if ( FBitSet( m_spawnflags, SF_LIGHT_START_OFF ) )
	{
		TurnOn();
	}
	else
	{
		TurnOff();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Handle the "turnon" input handler
// Input  : &inputdata - 
//-----------------------------------------------------------------------------
void CLight::InputTurnOn( inputdata_t &inputdata )
{
	TurnOn();
}

//-----------------------------------------------------------------------------
// Purpose: Handle the "turnoff" input handler
// Input  : &inputdata - 
//-----------------------------------------------------------------------------
void CLight::InputTurnOff( inputdata_t &inputdata )
{
	TurnOff();
}

//-----------------------------------------------------------------------------
// Purpose: Handle the "toggle" input handler
// Input  : &inputdata - 
//-----------------------------------------------------------------------------
void CLight::InputToggle( inputdata_t &inputdata )
{
	Toggle();
}

//-----------------------------------------------------------------------------
// Purpose: Input handler for setting a light pattern
//-----------------------------------------------------------------------------
void CLight::InputSetPattern( inputdata_t &inputdata )
{
	m_iszPattern = inputdata.value.StringID();
	engine->LightStyle(m_iStyle, (char *)STRING( m_iszPattern ));

	// Light is on if pattern is set
	CLEARBITS(m_spawnflags, SF_LIGHT_START_OFF);
}


//-----------------------------------------------------------------------------
// Purpose: Input handler for fading from first value in old pattern to 
//			first value in new pattern
//-----------------------------------------------------------------------------
void CLight::InputFadeToPattern( inputdata_t &inputdata )
{
	m_iCurrentFade	= (STRING(m_iszPattern))[0];
	m_iTargetFade	= inputdata.value.String()[0];
	m_iszPattern	= inputdata.value.StringID();
	SetThink(&CLight::FadeThink);
	SetNextThink( gpGlobals->curtime );

	// Light is on if pattern is set
	CLEARBITS(m_spawnflags, SF_LIGHT_START_OFF);
}


//------------------------------------------------------------------------------
// Purpose : Fade light to new starting pattern value then stop thinking
//------------------------------------------------------------------------------
void CLight::FadeThink(void)
{
	if (m_iCurrentFade < m_iTargetFade)
	{
		m_iCurrentFade++;
	}
	else if (m_iCurrentFade > m_iTargetFade)
	{
		m_iCurrentFade--;
	}

	// If we're done fading instantiate our light pattern and stop thinking
	if (m_iCurrentFade == m_iTargetFade)
	{
		engine->LightStyle(m_iStyle, (char *)STRING( m_iszPattern ));
		SetNextThink( TICK_NEVER_THINK );
	}
	// Otherwise instantiate our current fade value and keep thinking
	else
	{
		char sCurString[2];
		sCurString[0] = m_iCurrentFade;
		sCurString[1] = NULL;
		engine->LightStyle(m_iStyle, sCurString);

		// UNDONE: Consider making this settable war to control fade speed
		SetNextThink( gpGlobals->curtime + 0.1f );
	}
}

//
// shut up spawn functions for new spotlights
//
LINK_ENTITY_TO_CLASS( light_spot, CLight );
LINK_ENTITY_TO_CLASS( light_glspot, CLight );
LINK_ENTITY_TO_CLASS( light_directional, CLight );


class CEnvLight : public CLight
{
public:
	DECLARE_CLASS( CEnvLight, CLight );

	bool	KeyValue( const char *szKeyName, const char *szValue ); 
	void	Spawn( void );

#ifdef DEFERRED
	void	Activate( void );

private:
	float m_vecLight[4];
	float m_vecAmbientLight[4];
	float m_fLightPitch;
#endif // DEFERRED
};

LINK_ENTITY_TO_CLASS( light_environment, CEnvLight );

bool CEnvLight::KeyValue( const char *szKeyName, const char *szValue )
{
	if (FStrEq(szKeyName, "_light"))
	{
		// nothing
#ifdef DEFERRED
		UTIL_StringToFloatArray( m_vecLight, 4,  szValue );
#endif // DEFERRED
	}
	else
	{
#ifdef DEFERRED
		if( FStrEq(szKeyName, "pitch") )
		{
			m_fLightPitch = atof( szValue );
		}
		else if( FStrEq(szKeyName, "_ambient") )
		{
			UTIL_StringToFloatArray( m_vecAmbientLight, 4,  szValue );
		}
#endif // DEFERRED
		return BaseClass::KeyValue( szKeyName, szValue );
	}

	return true;
}


void CEnvLight::Spawn( void )
{
#ifdef DEFERRED
	SetName( MAKE_STRING( "light_environment" ) );
#endif // DEFERRED
	BaseClass::Spawn( );
}

#ifdef DEFERRED
static ConVar deferred_autoenvlight_ambient_intensity_low("deferred_autoenvlight_ambient_intensity_low", "0.15");
static ConVar deferred_autoenvlight_ambient_intensity_high("deferred_autoenvlight_ambient_intensity_high", "0.45");
static ConVar deferred_autoenvlight_diffuse_intensity("deferred_autoenvlight_diffuse_intensity", "1");

void CEnvLight::Activate( void )
{
	BaseClass::Activate( );

	if ( GetGlobalLight() == NULL )
	{
		CBaseEntity *pGlobalLight = CreateEntityByName( "light_deferred_global" );
		if( pGlobalLight )
		{
			float ds = deferred_autoenvlight_diffuse_intensity.GetFloat();
			float asl = deferred_autoenvlight_ambient_intensity_low.GetFloat();
			float ash = deferred_autoenvlight_ambient_intensity_high.GetFloat();

			const QAngle &vecAngles = GetAbsAngles();
			pGlobalLight->KeyValue( "origin", UTIL_VarArgs("%f %f %f", GetAbsOrigin().x, GetAbsOrigin().y, GetAbsOrigin().z ) );
			pGlobalLight->KeyValue( "diffuse", UTIL_VarArgs("%f %f %f %f", m_vecLight[0], m_vecLight[1], m_vecLight[2], m_vecLight[3] * ds ) );
			pGlobalLight->KeyValue( "ambient_high", UTIL_VarArgs("%f %f %f %f", m_vecAmbientLight[0], m_vecAmbientLight[1], m_vecAmbientLight[2], m_vecAmbientLight[3] * ash ) );
			pGlobalLight->KeyValue( "ambient_low", UTIL_VarArgs("%f %f %f %f", m_vecAmbientLight[0], m_vecAmbientLight[1], m_vecAmbientLight[2], m_vecAmbientLight[3] * asl ) );
			pGlobalLight->KeyValue( "spawnflags", DEFLIGHTGLOBAL_ENABLED | DEFLIGHTGLOBAL_SHADOW_ENABLED );
			pGlobalLight->KeyValue( "angles", UTIL_VarArgs("%f %f %f", -m_fLightPitch, vecAngles.y, vecAngles.z ) );
			DispatchSpawn( pGlobalLight );
			//pGlobalLight->Activate(); // Should not be activated here: the global light is created before the level activates all entities.
		}
	}

	UTIL_Remove( this );
}
#endif // DEFERRED