
#include "deferred_includes.h"

#include "tier0/memdbgon.h"

ConVar deferred_radiosity_multiplier( "deferred_radiosity_multiplier", "0.4" );

void GetTexcoordSettings( const bool bDecal, const bool bMultiBlend,
	int &iNumTexcoords, int **iTexcoordDim )
{
	static int iDimDefault[] = {
		2, 0, 3,
	};

	static int iDimMultiBlend[] = {
		2, 2, 0, 
		4,			// alpha blend
		4,			// vertex / blend color 0
		4,			// vertex / blend color 1
		4,			// vertex / blend color 2
		4			// vertex / blend color 3
	};

	if ( bMultiBlend )
	{
		*iTexcoordDim = iDimMultiBlend;
		iNumTexcoords = 8;
	}
	else if ( bDecal )
	{
		*iTexcoordDim = iDimDefault;
		iNumTexcoords = 3;
	}
	else
	{
		*iTexcoordDim = iDimDefault;
		iNumTexcoords = 1;
	}
}