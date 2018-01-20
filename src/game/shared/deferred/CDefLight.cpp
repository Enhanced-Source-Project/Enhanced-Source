
#include "cbase.h"
#include "deferred/deferred_shared_common.h"


#ifdef GAME_DLL
BEGIN_DATADESC( CDeferredLight )

	DEFINE_KEYFIELD( m_str_Diff, FIELD_STRING, GetLightParamName( LPARAM_DIFFUSE ) ),
	DEFINE_KEYFIELD( m_str_Ambient, FIELD_STRING, GetLightParamName( LPARAM_AMBIENT ) ),

	DEFINE_KEYFIELD( m_flRadius, FIELD_FLOAT, GetLightParamName( LPARAM_RADIUS ) ),
	DEFINE_KEYFIELD( m_flFalloffPower, FIELD_FLOAT, GetLightParamName( LPARAM_POWER ) ),
	DEFINE_KEYFIELD( m_flSpotConeInner, FIELD_FLOAT, GetLightParamName( LPARAM_SPOTCONE_INNER ) ),
	DEFINE_KEYFIELD( m_flSpotConeOuter, FIELD_FLOAT, GetLightParamName( LPARAM_SPOTCONE_OUTER ) ),

	DEFINE_KEYFIELD( m_iVisible_Dist, FIELD_INTEGER, GetLightParamName( LPARAM_VIS_DIST ) ),
	DEFINE_KEYFIELD( m_iVisible_FadeRange, FIELD_INTEGER, GetLightParamName( LPARAM_VIS_RANGE ) ),
	DEFINE_KEYFIELD( m_iShadow_Dist, FIELD_INTEGER, GetLightParamName( LPARAM_SHADOW_DIST ) ),
	DEFINE_KEYFIELD( m_iShadow_FadeRange, FIELD_INTEGER, GetLightParamName( LPARAM_SHADOW_RANGE ) ),

	DEFINE_KEYFIELD( m_iLightType, FIELD_INTEGER, GetLightParamName( LPARAM_LIGHTTYPE ) ),
	DEFINE_KEYFIELD( m_str_CookieString, FIELD_STRING, GetLightParamName( LPARAM_COOKIETEX ) ),

	DEFINE_KEYFIELD( m_flStyle_Amount, FIELD_FLOAT, GetLightParamName( LPARAM_STYLE_AMT ) ),
	DEFINE_KEYFIELD( m_flStyle_Speed, FIELD_FLOAT, GetLightParamName( LPARAM_STYLE_SPEED ) ),
	DEFINE_KEYFIELD( m_flStyle_Smooth, FIELD_FLOAT, GetLightParamName( LPARAM_STYLE_SMOOTH ) ),
	DEFINE_KEYFIELD( m_flStyle_Random, FIELD_FLOAT, GetLightParamName( LPARAM_STYLE_RANDOM ) ),
	DEFINE_KEYFIELD( m_iStyle_Seed, FIELD_INTEGER, GetLightParamName( LPARAM_STYLE_SEED ) ),

#if DEFCFG_ADAPTIVE_VOLUMETRIC_LOD
	DEFINE_KEYFIELD( m_flVolumeLOD0Dist, FIELD_FLOAT, GetLightParamName( LPARAM_VOLUME_LOD0_DIST ) ),
	DEFINE_KEYFIELD( m_flVolumeLOD1Dist, FIELD_FLOAT, GetLightParamName( LPARAM_VOLUME_LOD1_DIST ) ),
	DEFINE_KEYFIELD( m_flVolumeLOD2Dist, FIELD_FLOAT, GetLightParamName( LPARAM_VOLUME_LOD2_DIST ) ),
	DEFINE_KEYFIELD( m_flVolumeLOD3Dist, FIELD_FLOAT, GetLightParamName( LPARAM_VOLUME_LOD3_DIST ) ),
#endif

#if DEFCFG_CONFIGURABLE_VOLUMETRIC_LOD
	DEFINE_KEYFIELD( m_iVolumeSamples, FIELD_INTEGER, GetLightParamName( LPARAM_VOLUME_SAMPLES ) ),
#endif

END_DATADESC()
#endif

IMPLEMENT_NETWORKCLASS_DT( CDeferredLight, CDeferredLight_DT )

#ifdef GAME_DLL
	SendPropVector( SENDINFO( m_vecColor_Diff ), 32 ),
	SendPropVector( SENDINFO( m_vecColor_Ambient ), 32 ),

	SendPropFloat( SENDINFO( m_flRadius ) ),
	SendPropFloat( SENDINFO( m_flFalloffPower ) ),
	SendPropFloat( SENDINFO( m_flSpotConeInner ) ),
	SendPropFloat( SENDINFO( m_flSpotConeOuter ) ),

	SendPropInt( SENDINFO( m_iVisible_Dist ), 15, SPROP_UNSIGNED ),
	SendPropInt( SENDINFO( m_iVisible_FadeRange ), 15, SPROP_UNSIGNED ),
	SendPropInt( SENDINFO( m_iShadow_Dist ), 15, SPROP_UNSIGNED ),
	SendPropInt( SENDINFO( m_iShadow_FadeRange ), 15, SPROP_UNSIGNED ),

	SendPropInt( SENDINFO( m_iLightType ), MAX_DEFLIGHTTYPE_BITS, SPROP_UNSIGNED ),
	SendPropInt( SENDINFO( m_iDefFlags ), DEFLIGHT_FLAGS_MAX_SHARED_BITS, SPROP_UNSIGNED ),
	SendPropInt( SENDINFO( m_iCookieIndex ), MAX_COOKIE_TEXTURES_BITS, SPROP_UNSIGNED ),

	SendPropFloat( SENDINFO( m_flStyle_Amount ) ),
	SendPropFloat( SENDINFO( m_flStyle_Speed ) ),
	SendPropFloat( SENDINFO( m_flStyle_Smooth ) ),
	SendPropFloat( SENDINFO( m_flStyle_Random ) ),
	SendPropInt( SENDINFO( m_iStyle_Seed ), DEFLIGHT_SEED_MAX_BITS, SPROP_UNSIGNED ),

#	if DEFCFG_ADAPTIVE_VOLUMETRIC_LOD
	SendPropFloat( SENDINFO( m_flVolumeLOD0Dist ) ),
	SendPropFloat( SENDINFO( m_flVolumeLOD1Dist ) ),
	SendPropFloat( SENDINFO( m_flVolumeLOD2Dist ) ),
	SendPropFloat( SENDINFO( m_flVolumeLOD3Dist ) ),
#	endif

#	if DEFCFG_CONFIGURABLE_VOLUMETRIC_LOD
	SendPropInt( SENDINFO( m_iVolumeSamples ) ),
#	endif
#else
	RecvPropVector( RECVINFO( m_vecColor_Diff ) ),
	RecvPropVector( RECVINFO( m_vecColor_Ambient ) ),

	RecvPropFloat( RECVINFO( m_flRadius ) ),
	RecvPropFloat( RECVINFO( m_flFalloffPower ) ),
	RecvPropFloat( RECVINFO( m_flSpotConeInner ) ),
	RecvPropFloat( RECVINFO( m_flSpotConeOuter ) ),

	RecvPropInt( RECVINFO( m_iVisible_Dist ) ),
	RecvPropInt( RECVINFO( m_iVisible_FadeRange ) ),
	RecvPropInt( RECVINFO( m_iShadow_Dist ) ),
	RecvPropInt( RECVINFO( m_iShadow_FadeRange ) ),

	RecvPropInt( RECVINFO( m_iLightType ) ),
	RecvPropInt( RECVINFO( m_iDefFlags ) ),
	RecvPropInt( RECVINFO( m_iCookieIndex ) ),

	RecvPropFloat( RECVINFO( m_flStyle_Amount ) ),
	RecvPropFloat( RECVINFO( m_flStyle_Speed ) ),
	RecvPropFloat( RECVINFO( m_flStyle_Smooth ) ),
	RecvPropFloat( RECVINFO( m_flStyle_Random ) ),
	RecvPropInt( RECVINFO( m_iStyle_Seed ) ),

#	if DEFCFG_ADAPTIVE_VOLUMETRIC_LOD
	RecvPropFloat( RECVINFO( m_flVolumeLOD0Dist ) ),
	RecvPropFloat( RECVINFO( m_flVolumeLOD1Dist ) ),
	RecvPropFloat( RECVINFO( m_flVolumeLOD2Dist ) ),
	RecvPropFloat( RECVINFO( m_flVolumeLOD3Dist ) ),
#	endif

#	if DEFCFG_CONFIGURABLE_VOLUMETRIC_LOD
	RecvPropInt( RECVINFO( m_iVolumeSamples ) ),
#	endif

#endif

END_NETWORK_TABLE();

LINK_ENTITY_TO_CLASS( light_deferred, CDeferredLight );

CDeferredLight::CDeferredLight()
{
#ifdef GAME_DLL
	m_bShouldTransmit = false;
#else
	m_pLight = NULL;
#endif
}

CDeferredLight::~CDeferredLight()
{
#ifdef CLIENT_DLL
	Assert( m_pLight == NULL );
#endif
}

#ifdef GAME_DLL

void CDeferredLight::Activate()
{
	BaseClass::Activate();

	SetSolid( SOLID_NONE );
	AddEffects( EF_NODRAW );

	m_iDefFlags = GetSpawnFlags();

	m_bShouldTransmit = GetParent() != NULL ||
		Q_strlen( GetEntityNameAsCStr() ) > 0;

	SetMoveType( (GetParent() != NULL) ? MOVETYPE_PUSH : MOVETYPE_NONE );

	DispatchUpdateTransmitState();

	const char *pszCookie = STRING( m_str_CookieString );

	if ( m_iDefFlags & DEFLIGHT_COOKIE_ENABLED &&
		pszCookie != NULL && Q_strlen( pszCookie ) > 0 )
		m_iCookieIndex = GetDeferredManager()->AddCookieTexture( pszCookie );
	else
		m_iCookieIndex = 0;

	Assert( m_iCookieIndex >= 0 && m_iCookieIndex < MAX_COOKIE_TEXTURES );

	m_vecColor_Diff.GetForModify() = stringColToVec( STRING( m_str_Diff ) );
	m_vecColor_Ambient.GetForModify() = stringColToVec( STRING( m_str_Ambient ) );

	if ( !m_bShouldTransmit )
	{
		if ( m_iDefFlags & DEFLIGHT_ENABLED &&
			( m_vecColor_Diff.Get().LengthSqr() > 0 || m_vecColor_Ambient.Get().LengthSqr() > 0 ) &&
			m_flSpotConeOuter > 0.01f && m_flRadius > 0 )
		{
			GetDeferredManager()->AddWorldLight( this );
		}
		else
			Warning( "#%d: light_deferred at (%f %f %f) is turned off and can't be turned on\n", entindex(), GetAbsOrigin().x, GetAbsOrigin().y, GetAbsOrigin().z );

		UTIL_Remove( this );
	}
	else
	{
		UpdateSize();
	}
}

int CDeferredLight::UpdateTransmitState()
{
	return SetTransmitState( m_bShouldTransmit ? FL_EDICT_PVSCHECK : FL_EDICT_DONTSEND );
}

void CDeferredLight::UpdateSize()
{
	SetSize( Vector( -m_flRadius, -m_flRadius, -m_flRadius ),
			Vector( m_flRadius, m_flRadius, m_flRadius ) );
}

void CDeferredLight::SetRadius( float r )
{
	m_flRadius = r;
	UpdateSize();
}

#else

void CDeferredLight::ApplyDataToLight()
{
	Assert( m_pLight != NULL );
	ABS_QUERY_GUARD( true );

	m_pLight->ang = GetRenderAngles();
	m_pLight->pos = GetRenderOrigin();

	m_pLight->col_diffuse = GetColor_Diffuse();
	m_pLight->col_ambient = GetColor_Ambient();

	m_pLight->flRadius = GetRadius();
	m_pLight->flFalloffPower = GetFalloffPower();

	m_pLight->flSpotCone_Inner = SPOT_DEGREE_TO_RAD( GetSpotCone_Inner() );
	m_pLight->flSpotCone_Outer = SPOT_DEGREE_TO_RAD( GetSpotCone_Outer() );

	m_pLight->iVisible_Dist = GetVisible_Distance();
	m_pLight->iVisible_Range = GetVisible_FadeRange();
	m_pLight->iShadow_Dist = GetShadow_Distance();
	m_pLight->iShadow_Range = GetShadow_FadeRange();

	m_pLight->iStyleSeed = GetStyle_Seed();
	m_pLight->flStyle_Amount = GetStyle_Amount();
	m_pLight->flStyle_Random = GetStyle_Random();
	m_pLight->flStyle_Smooth = GetStyle_Smooth();
	m_pLight->flStyle_Speed = GetStyle_Speed();

	m_pLight->iLighttype = GetLight_Type();
	m_pLight->iFlags >>= DEFLIGHTGLOBAL_FLAGS_MAX_SHARED_BITS;
	m_pLight->iFlags <<= DEFLIGHTGLOBAL_FLAGS_MAX_SHARED_BITS;
	m_pLight->iFlags |= GetLight_Flags();
	m_pLight->iCookieIndex = GetCookieIndex();

#if DEFCFG_ADAPTIVE_VOLUMETRIC_LOD
	GetVolumeLODDistances( m_pLight->flVolumeLOD0Dist,
		 m_pLight->flVolumeLOD1Dist, m_pLight->flVolumeLOD2Dist,
		 m_pLight->flVolumeLOD3Dist );
#endif

#if DEFCFG_CONFIGURABLE_VOLUMETRIC_LOD
	m_pLight->iVolumeSamples = Clamp( GetVolumeSamples(), 0, 100 );
#endif
}

void CDeferredLight::PostDataUpdate( DataUpdateType_t t )
{
	BaseClass::PostDataUpdate( t );

	if ( t == DATA_UPDATE_CREATED )
	{
		Assert( m_pLight == NULL );

		m_pLight = new def_light_t();

		ApplyDataToLight();

		GetLightingManager()->AddLight( m_pLight );

		SetNextClientThink( CLIENT_THINK_ALWAYS );
	}
	else
	{
		ApplyDataToLight();

		m_pLight->MakeDirtyAll();
	}
}

void CDeferredLight::UpdateOnRemove()
{
	BaseClass::UpdateOnRemove();

	if ( m_pLight != NULL )
	{
		GetLightingManager()->RemoveLight( m_pLight );
		delete m_pLight;
		m_pLight = NULL;
	}
}

void CDeferredLight::ClientThink()
{
	if ( m_pLight == NULL )
		return;

	Vector curOrig = GetRenderOrigin();
	QAngle curAng = GetRenderAngles();

	if ( VectorCompare( curOrig.Base(), m_pLight->pos.Base() ) == 0 ||
		VectorCompare( curAng.Base(), m_pLight->ang.Base() ) == 0 )
	{
		ApplyDataToLight();

		if ( m_pLight->flSpotCone_Outer != GetSpotCone_Outer() )
			m_pLight->MakeDirtyAll();
		else
			m_pLight->MakeDirtyXForms();
	}
}

#endif