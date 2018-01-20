#ifndef DEFERRED_COMMON_H
#define DEFERRED_COMMON_H

#ifndef DEFERRED_SHARED_COMMON
#error must include deferred_shared_common.h instead
#endif

#define DEFLIGHT_SPOT_ZNEAR 5.0f

#define DEFLIGHT_MAX_LEAVES 64

extern ConVar deferred_rt_shadowspot_res;
extern ConVar deferred_rt_shadowpoint_res;

extern ConVar deferred_lightmanager_debug;

extern ConVar deferred_override_globalLight_enable;
extern ConVar deferred_override_globalLight_shadow_enable;
extern ConVar deferred_override_globalLight_diffuse;
extern ConVar deferred_override_globalLight_ambient_high;
extern ConVar deferred_override_globalLight_ambient_low;

extern ConVar deferred_radiosity_enable;
extern ConVar deferred_radiosity_propagate_count;
extern ConVar deferred_radiosity_propagate_count_far;
extern ConVar deferred_radiosity_blur_count;
extern ConVar deferred_radiosity_blur_count_far;
extern ConVar deferred_radiosity_debug;


#define PROFILER_DECLARE CFastTimer __pft; __pft.Start()
#define PROFILER_RESTART __pft.Start();
#define PROFILER_OUTPUT( line, format ) __pft.End();\
	engine->Con_NPrintf( line, format, __pft.GetDuration().GetCycles() )

#define PATHLOCATION_PROJECTABLE_SCRIPTS "scripts/vguiprojected"

#ifdef DEFERRED_HYBRID
#include "../../materialsystem/deferredshaders_hybrid/IDeferredExt.h"
#else
#include "../../materialsystem/deferredshaders/IDeferredExt.h"
#endif

#include "deferred/deferred_rt.h"
#include "deferred/IDefCookie.h"
#include "deferred/DefCookieTexture.h"
#include "deferred/DefCookieProjectable.h"
#include "deferred/def_light_t.h"
#include "deferred/cascade_t.h"

#include "deferred/vgui/vgui_deferred.h"

#include "deferred/cdeferred_manager_client.h"
#include "deferred/clight_manager.h"
#include "deferred/clight_editor.h"

#include "deferred/viewrender_deferred.h"

#include "deferred/flashlighteffect_deferred.h"

void OnCookieTableChanged( void *object, INetworkStringTable *stringTable, int stringNumber, const char *newString, void const *newData );


#define QUEUE_FIRE( helperName, functionName, varName ){\
		CMatRenderContextPtr pRenderContext( materials );\
		ICallQueue *pCallQueue = pRenderContext->GetCallQueue();\
		if ( pCallQueue )\
			pCallQueue->QueueCall( helperName::functionName, varName );\
		else\
			helperName::functionName( varName );\
	}

#define SATURATE( x ) ( clamp( (x), 0, 1 ) )

inline void DrawLightPassFullscreen( IMaterial *pMaterial, int w, int t )
{
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->DrawScreenSpaceRectangle( pMaterial,
		0, 0, w, t,
		0, 0, w - 1.0f, t - 1.0f,
		w, t
		);
}

void CalcBoundaries( Vector *list, const int &num, Vector &min, Vector &max );

FORCEINLINE void DebugDrawCross( const Vector &pos, float size, float lifetime )
{
	DebugDrawLine(pos-Vector(0,0,size),pos+Vector(0,0,size),0,255,0,true,lifetime);
	DebugDrawLine(pos-Vector(0,size,0),pos+Vector(0,size,0),255,0,0,true,lifetime);
	DebugDrawLine(pos-Vector(size,0,0),pos+Vector(size,0,0),0,0,255,true,lifetime);
}

FORCEINLINE void DrawFrustum( VMatrix &invScreenToWorld )
{
	const Vector normalized_points[8] = {
		Vector( -1, 1, 0 ),
		Vector( 1, 1, 0 ),
		Vector( 1, -1, 0 ),
		Vector( -1, -1, 0 ),

		Vector( -1, 1, 1 ),
		Vector( 1, 1, 1 ),
		Vector( 1, -1, 1 ),
		Vector( -1, -1, 1 )
	};
	Vector _points[8];

	for ( int i = 0; i < 8; i++ )
		Vector3DMultiplyPositionProjective( invScreenToWorld, normalized_points[i], _points[i] );

	for ( int i = 0; i < 8; i++ )
		DebugDrawCross( _points[i], 10.0f, -1.0f );

	DebugDrawLine( _points[0], _points[1], 255,255,255,true,-1.0f );
	DebugDrawLine( _points[1], _points[2], 255,255,255,true,-1.0f );
	DebugDrawLine( _points[2], _points[3], 255,255,255,true,-1.0f );
	DebugDrawLine( _points[0], _points[3], 255,255,255,true,-1.0f );

	DebugDrawLine( _points[4], _points[5], 255,255,255,true,-1.0f );
	DebugDrawLine( _points[6], _points[5], 255,255,255,true,-1.0f );
	DebugDrawLine( _points[6], _points[7], 255,255,255,true,-1.0f );
	DebugDrawLine( _points[4], _points[7], 255,255,255,true,-1.0f );

	DebugDrawLine( _points[0], _points[4], 255,255,255,true,-1.0f );
	DebugDrawLine( _points[1], _points[5], 255,255,255,true,-1.0f );
	DebugDrawLine( _points[2], _points[6], 255,255,255,true,-1.0f );
	DebugDrawLine( _points[3], _points[7], 255,255,255,true,-1.0f );
}

// returns false when no intersection
#if DEFCFG_USE_SSE
FORCEINLINE bool IntersectFrustumWithFrustum( VMatrix &invScreenToWorld_a, VMatrix &invScreenToWorld_b )
{
	static bool bSIMDDataInitialised = false;
	static fltx4 _normPos[8];

	if( !bSIMDDataInitialised )
	{
		const float pNormPos0[4] = { -1, 1, 0, 1 },
			pNormPos1[4] = { 1, 1, 0, 1 },
			pNormPos2[4] = { 1, -1, 0, 1 },
			pNormPos3[4] = { -1, -1, 0, 1 },
			pNormPos4[4] = { -1, 1, 1, 1 },
			pNormPos5[4] = { 1, 1, 1, 1 },
			pNormPos6[4] = { 1, -1, 1, 1 },
			pNormPos7[4] = { -1, -1, 1, 1 };

		_normPos[0] = LoadUnalignedSIMD( pNormPos0 );
		_normPos[1] = LoadUnalignedSIMD( pNormPos1 );
		_normPos[2] = LoadUnalignedSIMD( pNormPos2 );
		_normPos[3] = LoadUnalignedSIMD( pNormPos3 );
		_normPos[4] = LoadUnalignedSIMD( pNormPos4 );
		_normPos[5] = LoadUnalignedSIMD( pNormPos5 );
		_normPos[6] = LoadUnalignedSIMD( pNormPos6 );
		_normPos[7] = LoadUnalignedSIMD( pNormPos7 );
	}

	fltx4 _pointx4[2][8];
	fltx4 axes[22];
	fltx4 on_frustum_directions[2][6];

	fltx4 _invScreenToWorldAInSSE[4], _invScreenToWorldBInSSE[4];

	_invScreenToWorldAInSSE[0] =	LoadUnalignedSIMD( invScreenToWorld_a[0] );
	_invScreenToWorldAInSSE[1] =	LoadUnalignedSIMD( invScreenToWorld_a[1] );
	_invScreenToWorldAInSSE[2] =	LoadUnalignedSIMD( invScreenToWorld_a[2] );
	_invScreenToWorldAInSSE[3] =	LoadUnalignedSIMD( invScreenToWorld_a[3] );

	TransposeSIMD
		(
			_invScreenToWorldAInSSE[0],
			_invScreenToWorldAInSSE[1],
			_invScreenToWorldAInSSE[2],
			_invScreenToWorldAInSSE[3]
		);

	_invScreenToWorldBInSSE[0] =	LoadUnalignedSIMD( invScreenToWorld_b[0] );
	_invScreenToWorldBInSSE[1] =	LoadUnalignedSIMD( invScreenToWorld_b[1] );
	_invScreenToWorldBInSSE[2] =	LoadUnalignedSIMD( invScreenToWorld_b[2] );
	_invScreenToWorldBInSSE[3] =	LoadUnalignedSIMD( invScreenToWorld_b[3] );

	TransposeSIMD
		(
			_invScreenToWorldBInSSE[0],
			_invScreenToWorldBInSSE[1],
			_invScreenToWorldBInSSE[2],
			_invScreenToWorldBInSSE[3]
		);

	for ( int i = 0; i < 8; i++ )
	{
		_pointx4[0][i] = FourDotProducts( _invScreenToWorldAInSSE, _normPos[i] );

		float _w = SubFloat( _pointx4[0][i], 3 );
		if( _w != 0 )
		{
			_w = 1.0f / _w;
		}

		fltx4 _fourWs = ReplicateX4( _w );
		_pointx4[0][i] = _pointx4[0][i] * _fourWs;

		_pointx4[1][i] = FourDotProducts( _invScreenToWorldAInSSE, _normPos[i] );

		_w = SubFloat( _pointx4[1][i], 3 );
		if( _w != 0 )
		{
			_w = 1.0f / _w;
		}

		_fourWs = ReplicateX4( _w );
		_pointx4[1][i] = _pointx4[1][i] * _fourWs;
	}

	for ( int i = 0; i < 2; i++ ) // build minimal frustum frame
	{
		on_frustum_directions[i][0] = _pointx4[i][1] - _pointx4[i][0]; // viewplane x
		on_frustum_directions[i][1] = _pointx4[i][3] - _pointx4[i][0]; // viewplane y

		on_frustum_directions[i][2] = _pointx4[i][0] - _pointx4[i][4]; // frustum 0 0
		on_frustum_directions[i][3] = _pointx4[i][1] - _pointx4[i][5]; // frustum 1 0
		on_frustum_directions[i][4] = _pointx4[i][2] - _pointx4[i][6]; // frustum 1 1
		on_frustum_directions[i][5] = _pointx4[i][3] - _pointx4[i][7]; // frustum 0 1
	}

	for ( int x = 0; x < 2; x++ )
	{
		NormalizeInPlaceSIMD( on_frustum_directions[x][0] );
		NormalizeInPlaceSIMD( on_frustum_directions[x][1] );

		NormaliseFourInPlace
			( 
				on_frustum_directions[x][2],
				on_frustum_directions[x][3],
				on_frustum_directions[x][4],
				on_frustum_directions[x][5]
			);
	}

	for ( int i = 0; i < 2; i++ ) // build axes
	{
		fltx4* pAxes = &axes[ i * 11 + 0 ];

		FourCrossProducts //TODO: this function uses transpose heavily, should do transposing manually
			(
				on_frustum_directions[i][0], // viewplane
				on_frustum_directions[i][2], // left
				on_frustum_directions[i][2], // up
				on_frustum_directions[i][4], // right

				on_frustum_directions[i][1], 
				on_frustum_directions[i][5],
				on_frustum_directions[i][3],
				on_frustum_directions[i][3],

				pAxes[0],
				pAxes[1],
				pAxes[2],
				pAxes[3]
			);

		FourCrossProducts
			(
				on_frustum_directions[i][4], // down
				pAxes[1], // cross prod along local Z
				pAxes[3],
				pAxes[1],

				on_frustum_directions[i][5], 
				pAxes[3],
				pAxes[2],
				pAxes[2],

				pAxes[4],
				pAxes[5],
				pAxes[6],
				pAxes[7]
			);

		fltx4 _dummy;

		FourCrossProducts
			(
				pAxes[1],
				pAxes[2], // cross prod along local Y
				pAxes[3],
				LoadZeroSIMD(),

				pAxes[4],
				pAxes[4],
				pAxes[4],
				LoadZeroSIMD(),

				pAxes[8],
				pAxes[9],
				pAxes[10],
				_dummy
			);
	}

	for ( int i = 0; i < 22; i++ ) // project points onto axes, compare sets
	{
		float bounds_frusta[2][2]; // frustum, min/max
		// project starting points
		bounds_frusta[0][0] = bounds_frusta[0][1] = Dot4SIMD2( _pointx4[0][0], axes[i] );
		bounds_frusta[1][0] = bounds_frusta[1][1] = Dot4SIMD2( _pointx4[1][0], axes[i] );

		for ( int x = 1; x < 8; x++ )
		{
			//TODO: Implement optimised solution using FourDotProducts function
			float point_frustum_a = Dot4SIMD2( _pointx4[0][x], axes[i] );
			float point_frustum_b = Dot4SIMD2( _pointx4[1][x], axes[i] );

			bounds_frusta[0][0] = MIN( bounds_frusta[0][0], point_frustum_a );
			bounds_frusta[0][1] = MAX( bounds_frusta[0][1], point_frustum_a );

			bounds_frusta[1][0] = MIN( bounds_frusta[1][0], point_frustum_b );
			bounds_frusta[1][1] = MAX( bounds_frusta[1][1], point_frustum_b );
		}

		if ( bounds_frusta[0][1] < bounds_frusta[1][0] ||
			bounds_frusta[0][0] > bounds_frusta[1][1] )
			return true;
	}

	return false;
}
#else
FORCEINLINE bool IntersectFrustumWithFrustum( VMatrix &invScreenToWorld_a, VMatrix &invScreenToWorld_b )
{
	const Vector normalized_points[8] = {
		Vector( -1, 1, 0 ),
		Vector( 1, 1, 0 ),
		Vector( 1, -1, 0 ),
		Vector( -1, -1, 0 ),

		Vector( -1, 1, 1 ),
		Vector( 1, 1, 1 ),
		Vector( 1, -1, 1 ),
		Vector( -1, -1, 1 )
	};

	Vector _points[2][8];
	Vector axes[22];
	Vector on_frustum_directions[2][6];

	for ( int i = 0; i < 8; i++ )
	{
		Vector3DMultiplyPositionProjective( invScreenToWorld_a, normalized_points[i], _points[0][i] );
		Vector3DMultiplyPositionProjective( invScreenToWorld_b, normalized_points[i], _points[1][i] );
	}

	for ( int i = 0; i < 2; i++ ) // build minimal frustum frame
	{
		on_frustum_directions[i][0] = _points[i][1] - _points[i][0]; // viewplane x
		on_frustum_directions[i][1] = _points[i][3] - _points[i][0]; // viewplane y

		on_frustum_directions[i][2] = _points[i][0] - _points[i][4]; // frustum 0 0
		on_frustum_directions[i][3] = _points[i][1] - _points[i][5]; // frustum 1 0
		on_frustum_directions[i][4] = _points[i][2] - _points[i][6]; // frustum 1 1
		on_frustum_directions[i][5] = _points[i][3] - _points[i][7]; // frustum 0 1
	}
	for ( int x = 0; x < 2; x++ )
	{
		for ( int y = 0; y < 6; y++ )
		{
			on_frustum_directions[x][y].NormalizeInPlace();
		}
	}

	for ( int i = 0; i < 2; i++ ) // build axes
	{
		// normals
		CrossProduct( on_frustum_directions[i][0], on_frustum_directions[i][1], axes[ i * 11 + 0 ] ); // viewplane

		CrossProduct( on_frustum_directions[i][2], on_frustum_directions[i][5], axes[ i * 11 + 1 ] ); // left
		CrossProduct( on_frustum_directions[i][2], on_frustum_directions[i][3], axes[ i * 11 + 2 ] ); // up
		CrossProduct( on_frustum_directions[i][4], on_frustum_directions[i][3], axes[ i * 11 + 3 ] ); // right
		CrossProduct( on_frustum_directions[i][4], on_frustum_directions[i][5], axes[ i * 11 + 4 ] ); // down

		// and their crossproducts
		CrossProduct( axes[ i * 11 + 1 ], axes[ i * 11 + 3 ], axes[ i * 11 + 5 ] ); // along local Z
		CrossProduct( axes[ i * 11 + 2 ], axes[ i * 11 + 4 ], axes[ i * 11 + 6 ] ); // along local Y

		CrossProduct( axes[ i * 11 + 1 ], axes[ i * 11 + 2 ], axes[ i * 11 + 7 ] );
		CrossProduct( axes[ i * 11 + 1 ], axes[ i * 11 + 4 ], axes[ i * 11 + 8 ] );
		CrossProduct( axes[ i * 11 + 3 ], axes[ i * 11 + 2 ], axes[ i * 11 + 9 ] );
		CrossProduct( axes[ i * 11 + 3 ], axes[ i * 11 + 4 ], axes[ i * 11 + 10 ] );
	}

	for ( int i = 0; i < 22; i++ ) // project points onto axes, compare sets
	{
		float bounds_frusta[2][2]; // frustum, min/max
		// project starting points
		bounds_frusta[0][0] = bounds_frusta[0][1] = DotProduct( _points[0][0], axes[i] );
		bounds_frusta[1][0] = bounds_frusta[1][1] = DotProduct( _points[1][0], axes[i] );

		for ( int x = 1; x < 8; x++ )
		{
			float point_frustum_a = DotProduct( _points[0][x], axes[i] );
			float point_frustum_b = DotProduct( _points[1][x], axes[i] );

			bounds_frusta[0][0] = MIN( bounds_frusta[0][0], point_frustum_a );
			bounds_frusta[0][1] = MAX( bounds_frusta[0][1], point_frustum_a );

			bounds_frusta[1][0] = MIN( bounds_frusta[1][0], point_frustum_b );
			bounds_frusta[1][1] = MAX( bounds_frusta[1][1], point_frustum_b );
		}

		if ( bounds_frusta[0][1] < bounds_frusta[1][0] ||
			bounds_frusta[0][0] > bounds_frusta[1][1] )
			return true;
	}

	return false;
}
#endif

FORCEINLINE void VectorDeviceToSourceSpace( Vector &v )
{
	v.Init( -v.z, -v.x, v.y );
}

FORCEINLINE void VectorSourceToDeviceSpace( Vector &v )
{
	v.Init( -v.y, v.z, -v.x );
}

FORCEINLINE void MatrixDeviceToSourceSpace( VMatrix &m )
{
	Vector x, y, z;
	m.GetBasisVectors( x, y, z );
	m.SetBasisVectors( -z, -x, y );
}

FORCEINLINE void MatrixSourceToDeviceSpace( VMatrix &m )
{
	Vector x, y, z;
	m.GetBasisVectors( x, y, z );
	m.SetBasisVectors( -y, z, -x );
}

FORCEINLINE void MatrixSourceToDeviceSpaceInv( VMatrix &m )
{
	Vector x, y, z;
	m.GetBasisVectors( x, y, z );
	m.SetBasisVectors( y, -z, x );
}

#endif