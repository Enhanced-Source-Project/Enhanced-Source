// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// !$MODEL && $DECAL
// !$BASETEXTURE2 && $BLENDMODULATE
// $MULTIBLEND && $BLENDMODULATE
// $MODEL && $BASETEXTURE2
// $MODEL && $MULTIBLEND
// $BASETEXTURE2 && $MULTIBLEND
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// !$MODEL && $MORPHING_VTEX
// !$MODEL && $COMPRESSED_VERTS
// !$MODEL && $SKINNING
// !$MODEL && $MORPHING_VTEX
#include "shaderlib/cshader.h"
class composite_vs30_Static_Index
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
	int m_nDECAL;
#ifdef _DEBUG
	bool m_bDECAL;
#endif
public:
	void SetDECAL( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDECAL = i;
#ifdef _DEBUG
		m_bDECAL = true;
#endif
	}
	void SetDECAL( bool i )
	{
		m_nDECAL = i ? 1 : 0;
#ifdef _DEBUG
		m_bDECAL = true;
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
	int m_nEYEVEC;
#ifdef _DEBUG
	bool m_bEYEVEC;
#endif
public:
	void SetEYEVEC( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nEYEVEC = i;
#ifdef _DEBUG
		m_bEYEVEC = true;
#endif
	}
	void SetEYEVEC( bool i )
	{
		m_nEYEVEC = i ? 1 : 0;
#ifdef _DEBUG
		m_bEYEVEC = true;
#endif
	}
private:
	int m_nBASETEXTURE2;
#ifdef _DEBUG
	bool m_bBASETEXTURE2;
#endif
public:
	void SetBASETEXTURE2( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nBASETEXTURE2 = i;
#ifdef _DEBUG
		m_bBASETEXTURE2 = true;
#endif
	}
	void SetBASETEXTURE2( bool i )
	{
		m_nBASETEXTURE2 = i ? 1 : 0;
#ifdef _DEBUG
		m_bBASETEXTURE2 = true;
#endif
	}
private:
	int m_nBLENDMODULATE;
#ifdef _DEBUG
	bool m_bBLENDMODULATE;
#endif
public:
	void SetBLENDMODULATE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nBLENDMODULATE = i;
#ifdef _DEBUG
		m_bBLENDMODULATE = true;
#endif
	}
	void SetBLENDMODULATE( bool i )
	{
		m_nBLENDMODULATE = i ? 1 : 0;
#ifdef _DEBUG
		m_bBLENDMODULATE = true;
#endif
	}
private:
	int m_nMULTIBLEND;
#ifdef _DEBUG
	bool m_bMULTIBLEND;
#endif
public:
	void SetMULTIBLEND( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nMULTIBLEND = i;
#ifdef _DEBUG
		m_bMULTIBLEND = true;
#endif
	}
	void SetMULTIBLEND( bool i )
	{
		m_nMULTIBLEND = i ? 1 : 0;
#ifdef _DEBUG
		m_bMULTIBLEND = true;
#endif
	}
private:
	int m_nFOW;
#ifdef _DEBUG
	bool m_bFOW;
#endif
public:
	void SetFOW( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nFOW = i;
#ifdef _DEBUG
		m_bFOW = true;
#endif
	}
	void SetFOW( bool i )
	{
		m_nFOW = i ? 1 : 0;
#ifdef _DEBUG
		m_bFOW = true;
#endif
	}
public:
	// CONSTRUCTOR
	composite_vs30_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bMODEL = false;
#endif // _DEBUG
		m_nMODEL = 0;
#ifdef _DEBUG
		m_bDECAL = false;
#endif // _DEBUG
		m_nDECAL = 0;
#ifdef _DEBUG
		m_bMORPHING_VTEX = false;
#endif // _DEBUG
		m_nMORPHING_VTEX = 0;
#ifdef _DEBUG
		m_bEYEVEC = false;
#endif // _DEBUG
		m_nEYEVEC = 0;
#ifdef _DEBUG
		m_bBASETEXTURE2 = false;
#endif // _DEBUG
		m_nBASETEXTURE2 = 0;
#ifdef _DEBUG
		m_bBLENDMODULATE = false;
#endif // _DEBUG
		m_nBLENDMODULATE = 0;
#ifdef _DEBUG
		m_bMULTIBLEND = false;
#endif // _DEBUG
		m_nMULTIBLEND = 0;
#ifdef _DEBUG
		m_bFOW = false;
#endif // _DEBUG
		m_nFOW = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bMODEL && m_bDECAL && m_bMORPHING_VTEX && m_bEYEVEC && m_bBASETEXTURE2 && m_bBLENDMODULATE && m_bMULTIBLEND && m_bFOW;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 8 * m_nMODEL ) + ( 16 * m_nDECAL ) + ( 32 * m_nMORPHING_VTEX ) + ( 64 * m_nEYEVEC ) + ( 128 * m_nBASETEXTURE2 ) + ( 256 * m_nBLENDMODULATE ) + ( 512 * m_nMULTIBLEND ) + ( 1024 * m_nFOW ) + 0;
	}
};
#define shaderStaticTest_composite_vs30 vsh_forgot_to_set_static_MODEL + vsh_forgot_to_set_static_DECAL + vsh_forgot_to_set_static_MORPHING_VTEX + vsh_forgot_to_set_static_EYEVEC + vsh_forgot_to_set_static_BASETEXTURE2 + vsh_forgot_to_set_static_BLENDMODULATE + vsh_forgot_to_set_static_MULTIBLEND + vsh_forgot_to_set_static_FOW + 0
class composite_vs30_Dynamic_Index
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
public:
	// CONSTRUCTOR
	composite_vs30_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
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
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bCOMPRESSED_VERTS && m_bSKINNING && m_bMORPHING;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nCOMPRESSED_VERTS ) + ( 2 * m_nSKINNING ) + ( 4 * m_nMORPHING ) + 0;
	}
};
#define shaderDynamicTest_composite_vs30 vsh_forgot_to_set_dynamic_COMPRESSED_VERTS + vsh_forgot_to_set_dynamic_SKINNING + vsh_forgot_to_set_dynamic_MORPHING + 0
