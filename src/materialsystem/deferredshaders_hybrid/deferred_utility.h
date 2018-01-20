#ifndef DEFERRED_UTILITY_H
#define DEFERRED_UTILITY_H


#define DEFAULT_ALPHATESTREF 0.0f
#define DEFAULT_PHONG_SCALE 0.0f
#define DEFAULT_PHONG_EXP 0.0f
#define DEFAULT_PHONG_BOOST 0.0f

#define PARM_VALID( x ) ( x != -1 )

#define PARM_DEFINED( x ) ( x != -1 &&\
	params[ x ]->IsDefined() == true )

#define PARM_NO_DEFAULT( x ) ( PARM_VALID( x ) &&\
	!PARM_DEFINED( x ) )

#define PARM_SET( x ) ( PARM_DEFINED( x ) &&\
	params[ x ]->GetIntValue() != 0 )

#define PARM_TEX( x ) ( PARM_DEFINED( x ) &&\
	params[ x ]->IsTexture() == true )

#define PARM_FLOAT( x ) ( params[x]->GetFloatValue() )

#define PARM_INT( x ) ( params[x]->GetIntValue() )

#define PARM_VALIDATE( x ) Assert( PARM_DEFINED( x ) );

#define PARM_INIT_INT( x, val ) \
	if ( PARM_VALID( x ) && !params[ x ]->IsDefined() )\
	params[ x ]->SetIntValue( val );

#define PARM_INIT_FLOAT( x, val ) \
	if ( PARM_VALID( x ) && !params[ x ]->IsDefined() )\
	params[ x ]->SetFloatValue( val );

#define PARM_INIT_VEC3( x, val0, val1, val2 ) \
	if ( PARM_VALID( x ) && !params[ x ]->IsDefined() )\
	params[ x ]->SetVecValue( val0, val1, val2 );

void GetTexcoordSettings( const bool bDecal, const bool bMultiBlend,
	int &iNumTexcoords, int **iTexcoordDim );

extern ConVar deferred_radiosity_multiplier;

#include "defpass_gbuffer.h"

#endif