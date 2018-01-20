
#include "cbase.h"
#include "deferred/deferred_shared_common.h"

ConVar deferred_rt_shadowspot_res( "deferred_rt_shadowspot_res", "1024", FCVAR_HIDDEN | FCVAR_RELOAD_TEXTURES | FCVAR_RELOAD_MATERIALS );
#if DEFCFG_ADAPTIVE_SHADOWMAP_LOD
ConVar deferred_rt_shadowspot_lod1_res( "deferred_rt_shadowspot_lod1_res", "512", FCVAR_HIDDEN | FCVAR_RELOAD_TEXTURES | FCVAR_RELOAD_MATERIALS );
ConVar deferred_rt_shadowspot_lod2_res( "deferred_rt_shadowspot_lod2_res", "256", FCVAR_HIDDEN | FCVAR_RELOAD_TEXTURES | FCVAR_RELOAD_MATERIALS );
#endif
ConVar deferred_rt_shadowpoint_res( "deferred_rt_shadowpoint_res", "1024", FCVAR_HIDDEN | FCVAR_RELOAD_TEXTURES | FCVAR_RELOAD_MATERIALS );
#if DEFCFG_ADAPTIVE_SHADOWMAP_LOD
ConVar deferred_rt_shadowpoint_lod1_res( "deferred_rt_shadowpoint_lod1_res", "512", FCVAR_HIDDEN | FCVAR_RELOAD_TEXTURES | FCVAR_RELOAD_MATERIALS );
ConVar deferred_rt_shadowpoint_lod2_res( "deferred_rt_shadowpoint_lod2_res", "256", FCVAR_HIDDEN | FCVAR_RELOAD_TEXTURES | FCVAR_RELOAD_MATERIALS );
#endif

ConVar deferred_lightmanager_debug( "deferred_lightmanager_debug", "0" );

ConVar deferred_override_globalLight_enable( "deferred_override_globalLight_enable", "0" );
ConVar deferred_override_globalLight_shadow_enable( "deferred_override_globalLight_shadow_enable", "1" );
ConVar deferred_override_globalLight_diffuse( "deferred_override_globalLight_diffuse", "1 1 1" );
ConVar deferred_override_globalLight_ambient_high( "deferred_override_globalLight_ambient_high", "0 0.04 0.08" );
ConVar deferred_override_globalLight_ambient_low( "deferred_override_globalLight_ambient_low", "0 0.07 0.09" );

ConVar deferred_radiosity_enable( "deferred_radiosity_enable", "1" );
ConVar deferred_radiosity_propagate_count( "deferred_radiosity_propagate_count", "1" ); // 1
ConVar deferred_radiosity_propagate_count_far( "deferred_radiosity_propagate_count_far", "0" );
ConVar deferred_radiosity_blur_count( "deferred_radiosity_blur_count", "2" ); // 2
ConVar deferred_radiosity_blur_count_far( "deferred_radiosity_blur_count_far", "1" ); // 1
ConVar deferred_radiosity_debug( "deferred_radiosity_debug", "0" );

void OnCookieTableChanged( void *object, INetworkStringTable *stringTable, int stringNumber, const char *newString, void const *newData )
{
	if ( !newString || Q_strlen( newString ) < 1 )
		return;

	GetLightingManager()->OnCookieStringReceived( newString, stringNumber );
}

void CalcBoundaries( Vector *list, const int &num, Vector &min, Vector &max )
{
	Assert( num > 0 );

#if DEFCFG_USE_SSE && 0
	fltx4 vTestPoint = _mm_set_ps( list[0].x, list[0].y, list[0].z, 0 );
	fltx4 vMin = vTestPoint;
	fltx4 vMax = vTestPoint;

	for( int i = 1; i < num; i++ )
	{
		vTestPoint = _mm_set_ps( list[i].x, list[i].y, list[i].z, 0 );
		vMin = _mm_min_ps( vMin, vTestPoint );
		vMax = _mm_max_ps( vMax, vTestPoint );
	}

	min = Vector( SubFloat( vMin, 0 ), SubFloat( vMin, 1 ), SubFloat( vMin, 2 ) );
	max = Vector( SubFloat( vMax, 0 ), SubFloat( vMax, 1 ), SubFloat( vMax, 2 ) );
#else
	min = list[0];
	max = list[0];

	for ( int i = 1; i < num; i++ )
	{
		min.Min( list[i] );
		max.Max( list[i] );
	}
#endif
}