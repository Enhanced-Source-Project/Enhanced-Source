// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// !$MODEL && $MORPHING_VTEX
// !$MODEL && $COMPRESSED_VERTS
// !$MODEL && $SKINNING
// !$MODEL && $MORPHING_VTEX
#include "shaderlib/cshader.h"
class shadowpass_vs30_Static_Index
{
private:
	int m_nMODEL;
#ifdef _DEBUG
	bool m_bMODEL;
#endif
public:
	void SetMODEL( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nMODEL = i;
#ifdef _DEBUG
		m_bMODEL = true;
#endif
	}
	void SetMODEL( bool i )
	{
		m_nMODEL = i ? 1 : 0;
#ifdef _DEBUG
		m_bMODEL = true;
#endif
	}
private:
	int m_nMORPHING_VTEX;
#ifdef _DEBUG
	bool m_bMORPHING_VTEX;
#endif
public:
	void SetMORPHING_VTEX( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nMORPHING_VTEX = i;
#ifdef _DEBUG
		m_bMORPHING_VTEX = true;
#endif
	}
	void SetMORPHING_VTEX( bool i )
	{
		m_nMORPHING_VTEX = i ? 1 : 0;
#ifdef _DEBUG
		m_bMORPHING_VTEX = true;
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
private:
	int m_nTREESWAY;
#ifdef _DEBUG
	bool m_bTREESWAY;
#endif
public:
	void SetTREESWAY( int i )
	{
		Assert( i >= 0 && i <= 2 );
		m_nTREESWAY = i;
#ifdef _DEBUG
		m_bTREESWAY = true;
#endif
	}
	void SetTREESWAY( bool i )
	{
		m_nTREESWAY = i ? 1 : 0;
#ifdef _DEBUG
		m_bTREESWAY = true;
#endif
	}
public:
	// CONSTRUCTOR
	shadowpass_vs30_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bMODEL = false;
#endif // _DEBUG
		m_nMODEL = 0;
#ifdef _DEBUG
		m_bMORPHING_VTEX = false;
#endif // _DEBUG
		m_nMORPHING_VTEX = 0;
#ifdef _DEBUG
		m_bMULTITEXTURE = false;
#endif // _DEBUG
		m_nMULTITEXTURE = 0;
#ifdef _DEBUG
		m_bTREESWAY = false;
#endif // _DEBUG
		m_nTREESWAY = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bMODEL && m_bMORPHING_VTEX && m_bMULTITEXTURE && m_bTREESWAY;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 48 * m_nMODEL ) + ( 96 * m_nMORPHING_VTEX ) + ( 192 * m_nMULTITEXTURE ) + ( 576 * m_nTREESWAY ) + 0;
	}
};
#define shaderStaticTest_shadowpass_vs30 vsh_forgot_to_set_static_MODEL + vsh_forgot_to_set_static_MORPHING_VTEX + vsh_forgot_to_set_static_MULTITEXTURE + vsh_forgot_to_set_static_TREESWAY + 0
class shadowpass_vs30_Dynamic_Index
{
private:
	int m_nCOMPRESSED_VERTS;
#ifdef _DEBUG
	bool m_bCOMPRESSED_VERTS;
#endif
public:
	void SetCOMPRESSED_VERTS( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nCOMPRESSED_VERTS = i;
#ifdef _DEBUG
		m_bCOMPRESSED_VERTS = true;
#endif
	}
	void SetCOMPRESSED_VERTS( bool i )
	{
		m_nCOMPRESSED_VERTS = i ? 1 : 0;
#ifdef _DEBUG
		m_bCOMPRESSED_VERTS = true;
#endif
	}
private:
	int m_nSKINNING;
#ifdef _DEBUG
	bool m_bSKINNING;
#endif
public:
	void SetSKINNING( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSKINNING = i;
#ifdef _DEBUG
		m_bSKINNING = true;
#endif
	}
	void SetSKINNING( bool i )
	{
		m_nSKINNING = i ? 1 : 0;
#ifdef _DEBUG
		m_bSKINNING = true;
#endif
	}
private:
	int m_nMORPHING;
#ifdef _DEBUG
	bool m_bMORPHING;
#endif
public:
	void SetMORPHING( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nMORPHING = i;
#ifdef _DEBUG
		m_bMORPHING = true;
#endif
	}
	void SetMORPHING( bool i )
	{
		m_nMORPHING = i ? 1 : 0;
#ifdef _DEBUG
		m_bMORPHING = true;
#endif
	}
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
	shadowpass_vs30_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
	{
#ifdef _DEBUG
		m_bCOMPRESSED_VERTS = false;
#endif // _DEBUG
		m_nCOMPRESSED_VERTS = 0;
#ifdef _DEBUG
		m_bSKINNING = false;
#endif // _DEBUG
		m_nSKINNING = 0;
#ifdef _DEBUG
		m_bMORPHING = false;
#endif // _DEBUG
		m_nMORPHING = 0;
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
		bool bAllDynamicVarsDefined = m_bCOMPRESSED_VERTS && m_bSKINNING && m_bMORPHING && m_bSHADOW_MODE && m_bRADIOSITY;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nCOMPRESSED_VERTS ) + ( 2 * m_nSKINNING ) + ( 4 * m_nMORPHING ) + ( 8 * m_nSHADOW_MODE ) + ( 24 * m_nRADIOSITY ) + 0;
	}
};
#define shaderDynamicTest_shadowpass_vs30 vsh_forgot_to_set_dynamic_COMPRESSED_VERTS + vsh_forgot_to_set_dynamic_SKINNING + vsh_forgot_to_set_dynamic_MORPHING + vsh_forgot_to_set_dynamic_SHADOW_MODE + vsh_forgot_to_set_dynamic_RADIOSITY + 0
