
#include "cbase.h"
#include "deferred/deferred_shared_common.h"

static const cascade_t g_CascadeInfoLowRes[] = {
	// res	orthosize	light offset	zFar		slopemin	slopemax	normalmax	renderdelay		rad		radcascade
	{ 2048, 1024.0f,	10000.0f,		12000.0f,	1.0f,		2.0f,		2.0f,		0.0f,			true,	0
#if CSM_USE_COMPOSITED_TARGET
	// viewport offset x/y
	, 0, 0
#endif
	},

	{ 1024, 4096.0f,	10000.0f,		15000.0f,	4.0f,		6.0f,		20.0f,		/*0.25f*/ 0.0f,			true,	1
#if CSM_USE_COMPOSITED_TARGET
	, 2048, 0
#endif
	},
};

static const cascade_t g_CascadeInfo[] = {
	// res	orthosize	light offset	zFar		slopemin	slopemax	normalmax	renderdelay		rad		radcascade
	{ 4096, 2048.0f,	10000.0f,		12000.0f,	1.0f,		2.0f,		2.0f,		0.0f,			true,	0
#if CSM_USE_COMPOSITED_TARGET
	// viewport offset x/y
	, 0, 0
#endif
	},

	{ 2048, 4096.0f,	10000.0f,		15000.0f,	4.0f,		6.0f,		20.0f,		0.25f,			true,	1
#if CSM_USE_COMPOSITED_TARGET
	, 4096, 0
#endif
	},
};
static const int iNumCascades = ARRAYSIZE( g_CascadeInfo );

const cascade_t &GetCascadeInfo( int index )
{
	Assert( index >= 0 && index < iNumCascades );
	COMPILE_TIME_ASSERT( iNumCascades == SHADOW_NUM_CASCADES );

	if( IsCMSCompResLow() )
		return g_CascadeInfoLowRes[ index ];
	return g_CascadeInfo[ index ];
}
