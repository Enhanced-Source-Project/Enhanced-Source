// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// !$TWOTEXTURE && !$MULTIBLEND && $BLENDMODULATE
// $MODEL && $TWOTEXTURE
// $MODEL && $MULTIBLEND
// $TWOTEXTURE && $MULTIBLEND
// $DECAL && $MULTIBLEND
// $DECAL && $BLENDMODULATE
// $DECAL && $BUMPMAP
// $DECAL && $NOCULL
// $DECAL && $PHONGMAP
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
class gbuffer_defshading_ps30_Static_Index
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
	int m_nBUMPMAP;
#ifdef _DEBUG
	bool m_bBUMPMAP;
#endif
public:
	void SetBUMPMAP( int i )
	{
		Assert( i >= 0 && i <= 2 );
		m_nBUMPMAP = i;
#ifdef _DEBUG
		m_bBUMPMAP = true;
#endif
	}
	void SetBUMPMAP( bool i )
	{
		m_nBUMPMAP = i ? 1 : 0;
#ifdef _DEBUG
		m_bBUMPMAP = true;
#endif
	}
private:
	int m_nNOCULL;
#ifdef _DEBUG
	bool m_bNOCULL;
#endif
public:
	void SetNOCULL( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nNOCULL = i;
#ifdef _DEBUG
		m_bNOCULL = true;
#endif
	}
	void SetNOCULL( bool i )
	{
		m_nNOCULL = i ? 1 : 0;
#ifdef _DEBUG
		m_bNOCULL = true;
#endif
	}
private:
	int m_nPHONGMAP;
#ifdef _DEBUG
	bool m_bPHONGMAP;
#endif
public:
	void SetPHONGMAP( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nPHONGMAP = i;
#ifdef _DEBUG
		m_bPHONGMAP = true;
#endif
	}
	void SetPHONGMAP( bool i )
	{
		m_nPHONGMAP = i ? 1 : 0;
#ifdef _DEBUG
		m_bPHONGMAP = true;
#endif
	}
private:
	int m_nTWOTEXTURE;
#ifdef _DEBUG
	bool m_bTWOTEXTURE;
#endif
public:
	void SetTWOTEXTURE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nTWOTEXTURE = i;
#ifdef _DEBUG
		m_bTWOTEXTURE = true;
#endif
	}
	void SetTWOTEXTURE( bool i )
	{
		m_nTWOTEXTURE = i ? 1 : 0;
#ifdef _DEBUG
		m_bTWOTEXTURE = true;
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
public:
	// CONSTRUCTOR
	gbuffer_defshading_ps30_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bALPHATEST = false;
#endif // _DEBUG
		m_nALPHATEST = 0;
#ifdef _DEBUG
		m_bBUMPMAP = false;
#endif // _DEBUG
		m_nBUMPMAP = 0;
#ifdef _DEBUG
		m_bNOCULL = false;
#endif // _DEBUG
		m_nNOCULL = 0;
#ifdef _DEBUG
		m_bPHONGMAP = false;
#endif // _DEBUG
		m_nPHONGMAP = 0;
#ifdef _DEBUG
		m_bTWOTEXTURE = false;
#endif // _DEBUG
		m_nTWOTEXTURE = 0;
#ifdef _DEBUG
		m_bBLENDMODULATE = false;
#endif // _DEBUG
		m_nBLENDMODULATE = 0;
#ifdef _DEBUG
		m_bMULTIBLEND = false;
#endif // _DEBUG
		m_nMULTIBLEND = 0;
#ifdef _DEBUG
		m_bDECAL = false;
#endif // _DEBUG
		m_nDECAL = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bALPHATEST && m_bBUMPMAP && m_bNOCULL && m_bPHONGMAP && m_bTWOTEXTURE && m_bBLENDMODULATE && m_bMULTIBLEND && m_bDECAL;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nALPHATEST ) + ( 2 * m_nBUMPMAP ) + ( 6 * m_nNOCULL ) + ( 12 * m_nPHONGMAP ) + ( 24 * m_nTWOTEXTURE ) + ( 48 * m_nBLENDMODULATE ) + ( 96 * m_nMULTIBLEND ) + ( 192 * m_nDECAL ) + 0;
	}
};
#define shaderStaticTest_gbuffer_defshading_ps30 psh_forgot_to_set_static_ALPHATEST + psh_forgot_to_set_static_BUMPMAP + psh_forgot_to_set_static_NOCULL + psh_forgot_to_set_static_PHONGMAP + psh_forgot_to_set_static_TWOTEXTURE + psh_forgot_to_set_static_BLENDMODULATE + psh_forgot_to_set_static_MULTIBLEND + psh_forgot_to_set_static_DECAL + 0
class gbuffer_defshading_ps30_Dynamic_Index
{
public:
	// CONSTRUCTOR
	gbuffer_defshading_ps30_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
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
#define shaderDynamicTest_gbuffer_defshading_ps30 0
