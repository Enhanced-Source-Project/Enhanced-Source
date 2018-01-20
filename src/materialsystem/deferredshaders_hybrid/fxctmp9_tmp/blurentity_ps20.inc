// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
#include "shaderlib/cshader.h"
class blurentity_ps20_Static_Index
{
public:
	// CONSTRUCTOR
	blurentity_ps20_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
#endif // _DEBUG
		return 0;
	}
};
#define shaderStaticTest_blurentity_ps20 0
class blurentity_ps20_Dynamic_Index
{
public:
	// CONSTRUCTOR
	blurentity_ps20_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
	{
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
#endif // _DEBUG
		return 0;
	}
};
#define shaderDynamicTest_blurentity_ps20 0
