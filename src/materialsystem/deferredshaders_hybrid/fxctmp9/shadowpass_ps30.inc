// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW
// !$MODEL && $MORPHING_VTEX
// !$MODEL && $COMPRESSED_VERTS
// !$MODEL && $SKINNING
// !$MODEL && $MORPHING_VTEX
#include "shaderlib/cshader.h"
class shadowpass_ps30_Static_Index
{
private:
	int m_nALPHATEST;
#ifdef _DEBUG
	bool m_bALPHATEST;
#endif
public:
	void SetALPHATEST( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nALPHATEST = i;
#ifdef _DEBUG
		m_bALPHATEST = true;
#endif
	}
	void SetALPHATEST( bool i )
	{
		m_nALPHATEST = i ? 1 : 0;
#ifdef _DEBUG
		m_bALPHATEST = true;
#endif
	}
private:
	int m_nMULTITEXTURE;
#ifdef _DEBUG
	bool m_bMULTITEXTURE;
#endif
public:
	void SetMULTITEXTURE( int i )
	{
		Assert( i >= 0 && i <= 2 );
		m_nMULTITEXTURE = i;
#ifdef _DEBUG
		m_bMULTITEXTURE = true;
#endif
	}
	void SetMULTITEXTURE( bool i )
	{
		m_nMULTITEXTURE = i ? 1 : 0;
#ifdef _DEBUG
		m_bMULTITEXTURE = true;
#endif
	}
public:
	// CONSTRUCTOR
	shadowpass_ps30_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bALPHATEST = false;
#endif // _DEBUG
		m_nALPHATEST = 0;
#ifdef _DEBUG
		m_bMULTITEXTURE = false;
#endif // _DEBUG
		m_nMULTITEXTURE = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bALPHATEST && m_bMULTITEXTURE;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 6 * m_nALPHATEST ) + ( 12 * m_nMULTITEXTURE ) + 0;
	}
};
#define shaderStaticTest_shadowpass_ps30 psh_forgot_to_set_static_ALPHATEST + psh_forgot_to_set_static_MULTITEXTURE + 0
class shadowpass_ps30_Dynamic_Index
{
private:
	int m_nSHADOW_MODE;
#ifdef _DEBUG
	bool m_bSHADOW_MODE;
#endif
public:
	void SetSHADOW_MODE( int i )
	{
		Assert( i >= 0 && i <= 2 );
		m_nSHADOW_MODE = i;
#ifdef _DEBUG
		m_bSHADOW_MODE = true;
#endif
	}
	void SetSHADOW_MODE( bool i )
	{
		m_nSHADOW_MODE = i ? 1 : 0;
#ifdef _DEBUG
		m_bSHADOW_MODE = true;
#endif
	}
private:
	int m_nRADIOSITY;
#ifdef _DEBUG
	bool m_bRADIOSITY;
#endif
public:
	void SetRADIOSITY( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nRADIOSITY = i;
#ifdef _DEBUG
		m_bRADIOSITY = true;
#endif
	}
	void SetRADIOSITY( bool i )
	{
		m_nRADIOSITY = i ? 1 : 0;
#ifdef _DEBUG
		m_bRADIOSITY = true;
#endif
	}
public:
	// CONSTRUCTOR
	shadowpass_ps30_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
	{
#ifdef _DEBUG
		m_bSHADOW_MODE = false;
#endif // _DEBUG
		m_nSHADOW_MODE = 0;
#ifdef _DEBUG
		m_bRADIOSITY = false;
#endif // _DEBUG
		m_nRADIOSITY = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bSHADOW_MODE && m_bRADIOSITY;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nSHADOW_MODE ) + ( 3 * m_nRADIOSITY ) + 0;
	}
};
#define shaderDynamicTest_shadowpass_ps30 psh_forgot_to_set_dynamic_SHADOW_MODE + psh_forgot_to_set_dynamic_RADIOSITY + 0
