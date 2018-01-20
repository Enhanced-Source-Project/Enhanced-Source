// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW
#include "shaderlib/cshader.h"
class downsample_nohdr_ps20b_Static_Index
{
private:
	int m_nBLOOMTYPE;
#ifdef _DEBUG
	bool m_bBLOOMTYPE;
#endif
public:
	void SetBLOOMTYPE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nBLOOMTYPE = i;
#ifdef _DEBUG
		m_bBLOOMTYPE = true;
#endif
	}
	void SetBLOOMTYPE( bool i )
	{
		m_nBLOOMTYPE = i ? 1 : 0;
#ifdef _DEBUG
		m_bBLOOMTYPE = true;
#endif
	}
public:
	// CONSTRUCTOR
	downsample_nohdr_ps20b_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bBLOOMTYPE = false;
#endif // _DEBUG
		m_nBLOOMTYPE = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bBLOOMTYPE;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 2 * m_nBLOOMTYPE ) + 0;
	}
};
#define shaderStaticTest_downsample_nohdr_ps20b psh_forgot_to_set_static_BLOOMTYPE + 0
class downsample_nohdr_ps20b_Dynamic_Index
{
private:
	int m_nFLOAT_BACK_BUFFER;
#ifdef _DEBUG
	bool m_bFLOAT_BACK_BUFFER;
#endif
public:
	void SetFLOAT_BACK_BUFFER( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nFLOAT_BACK_BUFFER = i;
#ifdef _DEBUG
		m_bFLOAT_BACK_BUFFER = true;
#endif
	}
	void SetFLOAT_BACK_BUFFER( bool i )
	{
		m_nFLOAT_BACK_BUFFER = i ? 1 : 0;
#ifdef _DEBUG
		m_bFLOAT_BACK_BUFFER = true;
#endif
	}
public:
	// CONSTRUCTOR
	downsample_nohdr_ps20b_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
	{
#ifdef _DEBUG
		m_bFLOAT_BACK_BUFFER = false;
#endif // _DEBUG
		m_nFLOAT_BACK_BUFFER = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bFLOAT_BACK_BUFFER;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nFLOAT_BACK_BUFFER ) + 0;
	}
};
#define shaderDynamicTest_downsample_nohdr_ps20b psh_forgot_to_set_dynamic_FLOAT_BACK_BUFFER + 0
