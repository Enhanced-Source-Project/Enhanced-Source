// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// DEBUG_SHADOWS == 1 && SHADOW_MODE != 0
// DEBUG_SHADOWS == 2 && SHADOW_MODE != 0
// DEBUG_SHADOWS == 3 && (SHADOW_MODE != 4 || VARIABLE_SOFTNESS != 1 || VARIABLE_SAMPLES != 1)
// BLOBBY_SHADOWS == 1 && SHADOW_MODE != 0
// DEFERRED_SHADOWS == 1 && SHADOW_MODE != 0
// VARIABLE_SOFTNESS != 0 && SHADOW_MODE != 4
// VARIABLE_SAMPLES != 0 && SHADOW_MODE != 4
// ( VARIABLE_SOFTNESS == 0 ) && ( VARIABLE_SAMPLES == 1 )
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW
#include "shaderlib/cshader.h"
class shadow_ps20b_Static_Index
{
private:
	int m_nDEFERRED_SHADOWS;
#ifdef _DEBUG
	bool m_bDEFERRED_SHADOWS;
#endif
public:
	void SetDEFERRED_SHADOWS( int i )
	{
		Assert( i >= 0 && i <= 0 );
		m_nDEFERRED_SHADOWS = i;
#ifdef _DEBUG
		m_bDEFERRED_SHADOWS = true;
#endif
	}
	void SetDEFERRED_SHADOWS( bool i )
	{
		m_nDEFERRED_SHADOWS = i ? 1 : 0;
#ifdef _DEBUG
		m_bDEFERRED_SHADOWS = true;
#endif
	}
private:
	int m_nBLOBBY_SHADOWS;
#ifdef _DEBUG
	bool m_bBLOBBY_SHADOWS;
#endif
public:
	void SetBLOBBY_SHADOWS( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nBLOBBY_SHADOWS = i;
#ifdef _DEBUG
		m_bBLOBBY_SHADOWS = true;
#endif
	}
	void SetBLOBBY_SHADOWS( bool i )
	{
		m_nBLOBBY_SHADOWS = i ? 1 : 0;
#ifdef _DEBUG
		m_bBLOBBY_SHADOWS = true;
#endif
	}
private:
	int m_nDEBUG_SHADOWS;
#ifdef _DEBUG
	bool m_bDEBUG_SHADOWS;
#endif
public:
	void SetDEBUG_SHADOWS( int i )
	{
		Assert( i >= 0 && i <= 3 );
		m_nDEBUG_SHADOWS = i;
#ifdef _DEBUG
		m_bDEBUG_SHADOWS = true;
#endif
	}
	void SetDEBUG_SHADOWS( bool i )
	{
		m_nDEBUG_SHADOWS = i ? 1 : 0;
#ifdef _DEBUG
		m_bDEBUG_SHADOWS = true;
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
		Assert( i >= 0 && i <= 4 );
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
	int m_nVARIABLE_SOFTNESS;
#ifdef _DEBUG
	bool m_bVARIABLE_SOFTNESS;
#endif
public:
	void SetVARIABLE_SOFTNESS( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nVARIABLE_SOFTNESS = i;
#ifdef _DEBUG
		m_bVARIABLE_SOFTNESS = true;
#endif
	}
	void SetVARIABLE_SOFTNESS( bool i )
	{
		m_nVARIABLE_SOFTNESS = i ? 1 : 0;
#ifdef _DEBUG
		m_bVARIABLE_SOFTNESS = true;
#endif
	}
private:
	int m_nVARIABLE_SAMPLES;
#ifdef _DEBUG
	bool m_bVARIABLE_SAMPLES;
#endif
public:
	void SetVARIABLE_SAMPLES( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nVARIABLE_SAMPLES = i;
#ifdef _DEBUG
		m_bVARIABLE_SAMPLES = true;
#endif
	}
	void SetVARIABLE_SAMPLES( bool i )
	{
		m_nVARIABLE_SAMPLES = i ? 1 : 0;
#ifdef _DEBUG
		m_bVARIABLE_SAMPLES = true;
#endif
	}
public:
	// CONSTRUCTOR
	shadow_ps20b_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bDEFERRED_SHADOWS = false;
#endif // _DEBUG
		m_nDEFERRED_SHADOWS = 0;
#ifdef _DEBUG
		m_bBLOBBY_SHADOWS = false;
#endif // _DEBUG
		m_nBLOBBY_SHADOWS = 0;
#ifdef _DEBUG
		m_bDEBUG_SHADOWS = false;
#endif // _DEBUG
		m_nDEBUG_SHADOWS = 0;
#ifdef _DEBUG
		m_bSHADOW_MODE = false;
#endif // _DEBUG
		m_nSHADOW_MODE = 0;
#ifdef _DEBUG
		m_bVARIABLE_SOFTNESS = false;
#endif // _DEBUG
		m_nVARIABLE_SOFTNESS = 0;
#ifdef _DEBUG
		m_bVARIABLE_SAMPLES = false;
#endif // _DEBUG
		m_nVARIABLE_SAMPLES = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bDEFERRED_SHADOWS && m_bBLOBBY_SHADOWS && m_bDEBUG_SHADOWS && m_bSHADOW_MODE && m_bVARIABLE_SOFTNESS && m_bVARIABLE_SAMPLES;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 2 * m_nDEFERRED_SHADOWS ) + ( 2 * m_nBLOBBY_SHADOWS ) + ( 4 * m_nDEBUG_SHADOWS ) + ( 16 * m_nSHADOW_MODE ) + ( 80 * m_nVARIABLE_SOFTNESS ) + ( 160 * m_nVARIABLE_SAMPLES ) + 0;
	}
};
#define shaderStaticTest_shadow_ps20b psh_forgot_to_set_static_DEFERRED_SHADOWS + psh_forgot_to_set_static_BLOBBY_SHADOWS + psh_forgot_to_set_static_DEBUG_SHADOWS + psh_forgot_to_set_static_SHADOW_MODE + psh_forgot_to_set_static_VARIABLE_SOFTNESS + psh_forgot_to_set_static_VARIABLE_SAMPLES + 0
class shadow_ps20b_Dynamic_Index
{
private:
	int m_nPIXELFOGTYPE;
#ifdef _DEBUG
	bool m_bPIXELFOGTYPE;
#endif
public:
	void SetPIXELFOGTYPE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nPIXELFOGTYPE = i;
#ifdef _DEBUG
		m_bPIXELFOGTYPE = true;
#endif
	}
	void SetPIXELFOGTYPE( bool i )
	{
		m_nPIXELFOGTYPE = i ? 1 : 0;
#ifdef _DEBUG
		m_bPIXELFOGTYPE = true;
#endif
	}
public:
	// CONSTRUCTOR
	shadow_ps20b_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
	{
#ifdef _DEBUG
		m_bPIXELFOGTYPE = true;
#endif // _DEBUG
		m_nPIXELFOGTYPE = ( pShaderAPI->GetSceneFogMode() == MATERIAL_FOG_LINEAR_BELOW_FOG_Z ) ;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bPIXELFOGTYPE;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nPIXELFOGTYPE ) + 0;
	}
};
#define shaderDynamicTest_shadow_ps20b 0
