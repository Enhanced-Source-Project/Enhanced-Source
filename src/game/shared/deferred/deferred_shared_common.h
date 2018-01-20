#ifndef DEFERRED_SHARED_COMMON
#define DEFERRED_SHARED_COMMON

#define FOR_EACH_VEC_FAST( vecType, vecName, keyName ) { vecType *keyName##_p = vecName.Base();\
	for ( int keyName##_size = vecName.Count(); keyName##_size > 0; keyName##_size--, keyName##_p++ )\
	{ vecType keyName = *keyName##_p;
#define FOR_EACH_VEC_FAST_END }}

#define COOKIE_STRINGTBL_NAME "CookieTextures"
#define MAX_COOKIE_TEXTURES_BITS 7
#define MAX_COOKIE_TEXTURES (( 1 << MAX_COOKIE_TEXTURES_BITS ))

enum DEFLIGHT_FLAGS
{
	DEFLIGHT_ENABLED =					( 1 << 0 ),
	DEFLIGHT_SHADOW_ENABLED =			( 1 << 1 ),
	DEFLIGHT_COOKIE_ENABLED =			( 1 << 2 ),
	DEFLIGHT_VOLUMETRICS_ENABLED =		( 1 << 3 ),
	DEFLIGHT_LIGHTSTYLE_ENABLED =		( 1 << 4 ),

	DEFLIGHT_DIRTY_XFORMS =				( 1 << 6 ),
	DEFLIGHT_DIRTY_RENDERMESH =			( 1 << 7 ),
	DEFLIGHT_DIRTY_CONFIGURATION =		( 1 << 8 ),
};
#define DEFLIGHT_FLAGS_MAX_SHARED_BITS 5

enum DEFLIGHTGLOBAL_FLAGS
{
	DEFLIGHTGLOBAL_ENABLED =			( 1 << 0 ),
	DEFLIGHTGLOBAL_SHADOW_ENABLED =		( 1 << 1 ),
	DEFLIGHTGLOBAL_TRANSITION_FADE =	( 1 << 2 ),
};
#define DEFLIGHTGLOBAL_FLAGS_MAX_SHARED_BITS 3

#define DEFLIGHT_SEED_MAX_BITS 15
#define DEFLIGHT_SEED_MAX (( 1 << DEFLIGHT_SEED_MAX_BITS ) - 1)

#define SPOT_DEGREE_TO_RAD( x ) ( cos( DEG2RAD( x ) / 2.0f ) )
#define SPOT_RAD_TO_DEGREE( x ) ( RAD2DEG( acos( x ) * 2.0f ) )

inline Vector c32ToVec( const color32 &c )
{
	Vector v( vec3_origin );
	v.x = c.r / 255.0f;
	v.y = c.g / 255.0f;
	v.z = c.b / 255.0f;
	v *= c.a / 255.0f;
	return v;
}

inline Vector quatColorToVec( const Quaternion &q )
{
	Vector v( vec3_origin );
	for ( int i = 0; i < 3; i++ )
		v[ i ] = q[ i ] / 255.0f;
	v *= q.w / 255.0f;
	return v;
}

inline Vector stringColToVec( const char *str )
{
	float f[4] = { 0 };
	UTIL_StringToFloatArray( f, 4, str );
	Vector v( f[0], f[1], f[2] );
	v *= 1.0f / 255.0f * f[3] / 255.0f;
	return v;
}

inline void vecToStringCol( Vector col, char *out, int maxLen )
{
	float flMax = MAX( col.x, MAX( col.y, col.z ) );
	if ( flMax > 0 )
		col /= flMax;

	col *= 255;
	int it4[] = { XYZ( col ), flMax * 255 };

	Q_snprintf( out, maxLen, "%i %i %i %i", it4[0], it4[1], it4[2], it4[3] );
}

inline void normalizeAngles( QAngle &ang )
{
	ang[ 0 ] = AngleNormalize( ang[ 0 ] );
	ang[ 1 ] = AngleNormalize( ang[ 1 ] );
	ang[ 2 ] = AngleNormalize( ang[ 2 ] );
}

void UTIL_StringToIntArray( int *pVector, int count, const char *pString );

#define DEFLIGHTCONTAINER_MAXLIGHT_BITS 7
#define DEFLIGHTCONTAINER_MAXLIGHTS 113

#define NETWORK_MASK_LIGHTTYPE	0x0001
#define NETWORK_MASK_FLAGS		0x001F
#define NETWORK_MASK_COOKIE		0x007F
#define NETWORK_MASK_SEED		0xFFFF

#ifdef DEFERRED_HYBRID
#include "../../materialsystem/deferredshaders_hybrid/deferred_global_common.h"
#else
#include "../../materialsystem/deferredshaders/deferred_global_common.h"
#endif

enum LIGHT_PARAM_ID
{
	LPARAM_DIFFUSE = 0,
	LPARAM_AMBIENT,
	LPARAM_RADIUS,
	LPARAM_POWER,
	LPARAM_SPOTCONE_INNER,
	LPARAM_SPOTCONE_OUTER,
	LPARAM_VIS_DIST,
	LPARAM_VIS_RANGE,
	LPARAM_SHADOW_DIST,
	LPARAM_SHADOW_RANGE,
	LPARAM_LIGHTTYPE,
	LPARAM_COOKIETEX,
	LPARAM_STYLE_AMT,
	LPARAM_STYLE_SPEED,
	LPARAM_STYLE_SMOOTH,
	LPARAM_STYLE_RANDOM,
	LPARAM_STYLE_SEED,

	LPARAM_SPAWNFLAGS,
	LPARAM_ANGLES,
	LPARAM_ORIGIN,

	LPARAM_AMBIENT_LOW,
	LPARAM_AMBIENT_HIGH,

#if DEFCFG_ADAPTIVE_VOLUMETRIC_LOD
	LPARAM_VOLUME_LOD0_DIST,
	LPARAM_VOLUME_LOD1_DIST,
	LPARAM_VOLUME_LOD2_DIST,
	LPARAM_VOLUME_LOD3_DIST,
#endif

#if DEFCFG_CONFIGURABLE_VOLUMETRIC_LOD
	LPARAM_VOLUME_SAMPLES,
#endif

	LPARAM_COUNT,
};
const char *GetLightParamName( LIGHT_PARAM_ID id );


#include "networkstringtabledefs.h"

#include "deferred/CDefLight.h"
#include "deferred/CDefLightContainer.h"
#include "deferred/CDefLightGlobal.h"
#include "deferred/ssemath_ext.h"

#ifdef GAME_DLL
#include "deferred/deferred_server_common.h"
#else
#include "deferred/deferred_client_common.h"
#endif


extern INetworkStringTable *g_pStringTable_LightCookies;

#endif