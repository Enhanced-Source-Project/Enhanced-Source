// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW
#include "shaderlib/cshader.h"
class shadowbuildtexture_ps20b_Static_Index
{
private:
	int m_nWRITE_DEPTH;
#ifdef _DEBUG
	bool m_bWRITE_DEPTH;
#endif
public:
	void SetWRITE_DEPTH( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nWRITE_DEPTH = i;
#ifdef _DEBUG
		m_bWRITE_DEPTH = true;
#endif
	}
	void SetWRITE_DEPTH( bool i )
	{
		m_nWRITE_DEPTH = i ? 1 : 0;
#ifdef _DEBUG
		m_bWRITE_DEPTH = true;
#endif
	}
public:
	// CONSTRUCTOR
	shadowbuildtexture_ps20b_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bWRITE_DEPTH = false;
#endif // _DEBUG
		m_nWRITE_DEPTH = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bWRITE_DEPTH;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nWRITE_DEPTH ) + 0;
	}
};
#define shaderStaticTest_shadowbuildtexture_ps20b psh_forgot_to_set_static_WRITE_DEPTH + 0
class shadowbuildtexture_ps20b_Dynamic_Index
{
public:
	// CONSTRUCTOR
	shadowbuildtexture_ps20b_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
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
#define shaderDynamicTest_shadowbuildtexture_ps20b 0
