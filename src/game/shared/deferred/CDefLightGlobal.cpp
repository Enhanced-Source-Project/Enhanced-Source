
#include "cbase.h"
#include "deferred/deferred_shared_common.h"

static CDeferredLightGlobal *__g_pGlobalLight = NULL;
CDeferredLightGlobal *GetGlobalLight()
{
	return __g_pGlobalLight;
}

#ifdef GAME_DLL
BEGIN_DATADESC( CDeferredLightGlobal )

	DEFINE_KEYFIELD( m_str_Diff, FIELD_STRING, "diffuse" ),
	DEFINE_KEYFIELD( m_str_Ambient_High, FIELD_STRING, "ambient_high" ),
	DEFINE_KEYFIELD( m_str_Ambient_Low, FIELD_STRING, "ambient_low" ),

	DEFINE_KEYFIELD( m_flFadeTime, FIELD_FLOAT, "fadetime" ),

	DEFINE_FIELD( m_vecColor_Diff, FIELD_VECTOR ),
	DEFINE_FIELD( m_vecColor_Ambient_High, FIELD_VECTOR ),
	DEFINE_FIELD( m_vecColor_Ambient_Low, FIELD_VECTOR ),
	DEFINE_FIELD( m_iDefFlags, FIELD_INTEGER ),

END_DATADESC()
#endif

IMPLEMENT_NETWORKCLASS_DT( CDeferredLightGlobal, CDeferredLightGlobal_DT )
#ifdef GAME_DLL
	SendPropVector( SENDINFO( m_vecColor_Diff ), 32 ),
	SendPropVector( SENDINFO( m_vecColor_Ambient_High ), 32 ),
	SendPropVector( SENDINFO( m_vecColor_Ambient_Low ), 32 ),

	SendPropInt( SENDINFO( m_iDefFlags ), DEFLIGHTGLOBAL_FLAGS_MAX_SHARED_BITS, SPROP_UNSIGNED ),
#else
	RecvPropVector( RECVINFO( m_vecColor_Diff ) ),
	RecvPropVector( RECVINFO( m_vecColor_Ambient_High ) ),
	RecvPropVector( RECVINFO( m_vecColor_Ambient_Low ) ),

	RecvPropInt( RECVINFO( m_iDefFlags ) ),
#endif
END_NETWORK_TABLE();

LINK_ENTITY_TO_CLASS( light_deferred_global, CDeferredLightGlobal );

CDeferredLightGlobal::CDeferredLightGlobal()
{
	Assert( __g_pGlobalLight == NULL );
	__g_pGlobalLight = this;

	m_iDefFlags = DEFLIGHTGLOBAL_ENABLED | DEFLIGHTGLOBAL_SHADOW_ENABLED;
}

CDeferredLightGlobal::~CDeferredLightGlobal()
{
	Assert( __g_pGlobalLight == this );
	__g_pGlobalLight = NULL;
}

#ifdef GAME_DLL

void CDeferredLightGlobal::Activate()
{
	BaseClass::Activate();

	SetSolid( SOLID_NONE );
	SetMoveType( MOVETYPE_NONE );
	AddEffects( EF_NODRAW );

	m_iDefFlags = GetSpawnFlags();

	m_vecColor_Diff.GetForModify() = stringColToVec( STRING( m_str_Diff ) );
	m_vecColor_Ambient_High.GetForModify() = stringColToVec( STRING( m_str_Ambient_High ) );
	m_vecColor_Ambient_Low.GetForModify() = stringColToVec( STRING( m_str_Ambient_Low ) );
}

int CDeferredLightGlobal::UpdateTransmitState()
{
	return SetTransmitState( FL_EDICT_ALWAYS );
}

CON_COMMAND( deferred_debug_globalLight_SetAngles, "" )
{
	if ( !GetGlobalLight() )
	{
		Warning( "No global light existing!\n" );
		return;
	}

	if ( args.ArgC() < 4 )
	{
		Warning( "Not enough parameters!\n" );
		return;
	}

	QAngle ang( atof( args[1] ), atof( args[2] ), atof( args[3] ) );
	GetGlobalLight()->SetAbsAngles( ang );
}

#else

lightData_Global_t CDeferredLightGlobal::GetState()
{
	lightData_Global_t data;

	Vector diff;
	Vector normal(0, 0, 1);
	engine->ComputeLighting( GetAbsOrigin(), &normal, false, diff );

	data.diff.Init( diff );
	//data.diff.Init( GetColor_Diffuse() ); // Original
	data.ambh.Init( GetColor_Ambient_High() );
	data.ambl.Init( GetColor_Ambient_Low() );

	Vector dir;
	AngleVectors( GetAbsAngles(), &dir );
	data.vecLight.Init( -dir );

	if ( IsEnabled() &&
		( data.diff.LengthSqr() > 0.01f ||
		data.ambh.LengthSqr() > 0.01f ||
		data.ambl.LengthSqr() > 0.01f ) )
	{
		data.bEnabled = true;
		data.bShadow = HasShadow();
	}

	return data;
}


#endif