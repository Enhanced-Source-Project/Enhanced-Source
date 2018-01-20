// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// ($SEPARATE_DETAIL_UVS) && ($SEAMLESS_DETAIL)
// ( $FOW ) && ( $SEAMLESS_DETAIL )
// ( $FOW ) && ( $SEAMLESS_BASE )
// ( $FOW ) && ( $TESSELLATION )
// ($TREESWAY) && ($MORPHING)
// ($TREESWAY) && ($TESSELLATION)
// ( $TREESWAY ) && ( $SEAMLESS_DETAIL || $SEAMLESS_BASE )
// ( $DOWATERFOG == 0 ) && ( $DOPIXELFOG && $HARDWAREFOGBLEND )
// ( $STATICLIGHT3 || $VERTEXCOLOR || $SEAMLESS_BASE || $SEAMLESS_DETAIL || $SEPARATE_DETAIL_UVS || $MORPHING || $SKINNING || $COMPRESSED_VERTS ) && $TESSELLATION
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
#include "shaderlib/cshader.h"
class vertexlit_and_unlit_generic_vs30_Static_Index
{
private:
	int m_nVERTEXCOLOR;
#ifdef _DEBUG
	bool m_bVERTEXCOLOR;
#endif
public:
	void SetVERTEXCOLOR( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nVERTEXCOLOR = i;
#ifdef _DEBUG
		m_bVERTEXCOLOR = true;
#endif
	}
	void SetVERTEXCOLOR( bool i )
	{
		m_nVERTEXCOLOR = i ? 1 : 0;
#ifdef _DEBUG
		m_bVERTEXCOLOR = true;
#endif
	}
private:
	int m_nCUBEMAP;
#ifdef _DEBUG
	bool m_bCUBEMAP;
#endif
public:
	void SetCUBEMAP( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nCUBEMAP = i;
#ifdef _DEBUG
		m_bCUBEMAP = true;
#endif
	}
	void SetCUBEMAP( bool i )
	{
		m_nCUBEMAP = i ? 1 : 0;
#ifdef _DEBUG
		m_bCUBEMAP = true;
#endif
	}
private:
	int m_nHALFLAMBERT;
#ifdef _DEBUG
	bool m_bHALFLAMBERT;
#endif
public:
	void SetHALFLAMBERT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nHALFLAMBERT = i;
#ifdef _DEBUG
		m_bHALFLAMBERT = true;
#endif
	}
	void SetHALFLAMBERT( bool i )
	{
		m_nHALFLAMBERT = i ? 1 : 0;
#ifdef _DEBUG
		m_bHALFLAMBERT = true;
#endif
	}
private:
	int m_nFLASHLIGHT;
#ifdef _DEBUG
	bool m_bFLASHLIGHT;
#endif
public:
	void SetFLASHLIGHT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nFLASHLIGHT = i;
#ifdef _DEBUG
		m_bFLASHLIGHT = true;
#endif
	}
	void SetFLASHLIGHT( bool i )
	{
		m_nFLASHLIGHT = i ? 1 : 0;
#ifdef _DEBUG
		m_bFLASHLIGHT = true;
#endif
	}
private:
	int m_nSEAMLESS_BASE;
#ifdef _DEBUG
	bool m_bSEAMLESS_BASE;
#endif
public:
	void SetSEAMLESS_BASE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSEAMLESS_BASE = i;
#ifdef _DEBUG
		m_bSEAMLESS_BASE = true;
#endif
	}
	void SetSEAMLESS_BASE( bool i )
	{
		m_nSEAMLESS_BASE = i ? 1 : 0;
#ifdef _DEBUG
		m_bSEAMLESS_BASE = true;
#endif
	}
private:
	int m_nSEAMLESS_DETAIL;
#ifdef _DEBUG
	bool m_bSEAMLESS_DETAIL;
#endif
public:
	void SetSEAMLESS_DETAIL( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSEAMLESS_DETAIL = i;
#ifdef _DEBUG
		m_bSEAMLESS_DETAIL = true;
#endif
	}
	void SetSEAMLESS_DETAIL( bool i )
	{
		m_nSEAMLESS_DETAIL = i ? 1 : 0;
#ifdef _DEBUG
		m_bSEAMLESS_DETAIL = true;
#endif
	}
private:
	int m_nSEPARATE_DETAIL_UVS;
#ifdef _DEBUG
	bool m_bSEPARATE_DETAIL_UVS;
#endif
public:
	void SetSEPARATE_DETAIL_UVS( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSEPARATE_DETAIL_UVS = i;
#ifdef _DEBUG
		m_bSEPARATE_DETAIL_UVS = true;
#endif
	}
	void SetSEPARATE_DETAIL_UVS( bool i )
	{
		m_nSEPARATE_DETAIL_UVS = i ? 1 : 0;
#ifdef _DEBUG
		m_bSEPARATE_DETAIL_UVS = true;
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
	int m_nSTATICLIGHT3;
#ifdef _DEBUG
	bool m_bSTATICLIGHT3;
#endif
public:
	void SetSTATICLIGHT3( int i )
	{
		Assert( i >= 0 && i <= 0 );
		m_nSTATICLIGHT3 = i;
#ifdef _DEBUG
		m_bSTATICLIGHT3 = true;
#endif
	}
	void SetSTATICLIGHT3( bool i )
	{
		m_nSTATICLIGHT3 = i ? 1 : 0;
#ifdef _DEBUG
		m_bSTATICLIGHT3 = true;
#endif
	}
private:
	int m_nLIGHTING_PREVIEW;
#ifdef _DEBUG
	bool m_bLIGHTING_PREVIEW;
#endif
public:
	void SetLIGHTING_PREVIEW( int i )
	{
		Assert( i >= 0 && i <= 3 );
		m_nLIGHTING_PREVIEW = i;
#ifdef _DEBUG
		m_bLIGHTING_PREVIEW = true;
#endif
	}
	void SetLIGHTING_PREVIEW( bool i )
	{
		m_nLIGHTING_PREVIEW = i ? 1 : 0;
#ifdef _DEBUG
		m_bLIGHTING_PREVIEW = true;
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
private:
	int m_nMODELGLOBALNORMAL;
#ifdef _DEBUG
	bool m_bMODELGLOBALNORMAL;
#endif
public:
	void SetMODELGLOBALNORMAL( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nMODELGLOBALNORMAL = i;
#ifdef _DEBUG
		m_bMODELGLOBALNORMAL = true;
#endif
	}
	void SetMODELGLOBALNORMAL( bool i )
	{
		m_nMODELGLOBALNORMAL = i ? 1 : 0;
#ifdef _DEBUG
		m_bMODELGLOBALNORMAL = true;
#endif
	}
private:
	int m_nDOPIXELFOG;
#ifdef _DEBUG
	bool m_bDOPIXELFOG;
#endif
public:
	void SetDOPIXELFOG( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDOPIXELFOG = i;
#ifdef _DEBUG
		m_bDOPIXELFOG = true;
#endif
	}
	void SetDOPIXELFOG( bool i )
	{
		m_nDOPIXELFOG = i ? 1 : 0;
#ifdef _DEBUG
		m_bDOPIXELFOG = true;
#endif
	}
private:
	int m_nHARDWAREFOGBLEND;
#ifdef _DEBUG
	bool m_bHARDWAREFOGBLEND;
#endif
public:
	void SetHARDWAREFOGBLEND( int i )
	{
		Assert( i >= 0 && i <= 0 );
		m_nHARDWAREFOGBLEND = i;
#ifdef _DEBUG
		m_bHARDWAREFOGBLEND = true;
#endif
	}
	void SetHARDWAREFOGBLEND( bool i )
	{
		m_nHARDWAREFOGBLEND = i ? 1 : 0;
#ifdef _DEBUG
		m_bHARDWAREFOGBLEND = true;
#endif
	}
public:
	// CONSTRUCTOR
	vertexlit_and_unlit_generic_vs30_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bVERTEXCOLOR = false;
#endif // _DEBUG
		m_nVERTEXCOLOR = 0;
#ifdef _DEBUG
		m_bCUBEMAP = false;
#endif // _DEBUG
		m_nCUBEMAP = 0;
#ifdef _DEBUG
		m_bHALFLAMBERT = false;
#endif // _DEBUG
		m_nHALFLAMBERT = 0;
#ifdef _DEBUG
		m_bFLASHLIGHT = false;
#endif // _DEBUG
		m_nFLASHLIGHT = 0;
#ifdef _DEBUG
		m_bSEAMLESS_BASE = false;
#endif // _DEBUG
		m_nSEAMLESS_BASE = 0;
#ifdef _DEBUG
		m_bSEAMLESS_DETAIL = false;
#endif // _DEBUG
		m_nSEAMLESS_DETAIL = 0;
#ifdef _DEBUG
		m_bSEPARATE_DETAIL_UVS = false;
#endif // _DEBUG
		m_nSEPARATE_DETAIL_UVS = 0;
#ifdef _DEBUG
		m_bDECAL = false;
#endif // _DEBUG
		m_nDECAL = 0;
#ifdef _DEBUG
		m_bSTATICLIGHT3 = false;
#endif // _DEBUG
		m_nSTATICLIGHT3 = 0;
#ifdef _DEBUG
		m_bLIGHTING_PREVIEW = false;
#endif // _DEBUG
		m_nLIGHTING_PREVIEW = 0;
#ifdef _DEBUG
		m_bFOW = false;
#endif // _DEBUG
		m_nFOW = 0;
#ifdef _DEBUG
		m_bTREESWAY = false;
#endif // _DEBUG
		m_nTREESWAY = 0;
#ifdef _DEBUG
		m_bMODELGLOBALNORMAL = false;
#endif // _DEBUG
		m_nMODELGLOBALNORMAL = 0;
#ifdef _DEBUG
		m_bDOPIXELFOG = true;
#endif // _DEBUG
		m_nDOPIXELFOG = ( g_pHardwareConfig->SupportsPixelShaders_2_b() && !IS_FLAG_SET( MATERIAL_VAR_VERTEXFOG ) ) ? 1 : 0 ;
#ifdef _DEBUG
		m_bHARDWAREFOGBLEND = true;
#endif // _DEBUG
		m_nHARDWAREFOGBLEND = 0 ;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bVERTEXCOLOR && m_bCUBEMAP && m_bHALFLAMBERT && m_bFLASHLIGHT && m_bSEAMLESS_BASE && m_bSEAMLESS_DETAIL && m_bSEPARATE_DETAIL_UVS && m_bDECAL && m_bSTATICLIGHT3 && m_bLIGHTING_PREVIEW && m_bFOW && m_bTREESWAY && m_bMODELGLOBALNORMAL && m_bDOPIXELFOG && m_bHARDWAREFOGBLEND;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 96 * m_nVERTEXCOLOR ) + ( 192 * m_nCUBEMAP ) + ( 384 * m_nHALFLAMBERT ) + ( 768 * m_nFLASHLIGHT ) + ( 1536 * m_nSEAMLESS_BASE ) + ( 3072 * m_nSEAMLESS_DETAIL ) + ( 6144 * m_nSEPARATE_DETAIL_UVS ) + ( 12288 * m_nDECAL ) + ( 24576 * m_nSTATICLIGHT3 ) + ( 24576 * m_nLIGHTING_PREVIEW ) + ( 98304 * m_nFOW ) + ( 196608 * m_nTREESWAY ) + ( 589824 * m_nMODELGLOBALNORMAL ) + ( 1179648 * m_nDOPIXELFOG ) + ( 2359296 * m_nHARDWAREFOGBLEND ) + 0;
	}
};
#define shaderStaticTest_vertexlit_and_unlit_generic_vs30 vsh_forgot_to_set_static_VERTEXCOLOR + vsh_forgot_to_set_static_CUBEMAP + vsh_forgot_to_set_static_HALFLAMBERT + vsh_forgot_to_set_static_FLASHLIGHT + vsh_forgot_to_set_static_SEAMLESS_BASE + vsh_forgot_to_set_static_SEAMLESS_DETAIL + vsh_forgot_to_set_static_SEPARATE_DETAIL_UVS + vsh_forgot_to_set_static_DECAL + vsh_forgot_to_set_static_STATICLIGHT3 + vsh_forgot_to_set_static_LIGHTING_PREVIEW + vsh_forgot_to_set_static_FOW + vsh_forgot_to_set_static_TREESWAY + vsh_forgot_to_set_static_MODELGLOBALNORMAL + 0
class vertexlit_and_unlit_generic_vs30_Dynamic_Index
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
	int m_nDYNAMIC_LIGHT;
#ifdef _DEBUG
	bool m_bDYNAMIC_LIGHT;
#endif
public:
	void SetDYNAMIC_LIGHT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDYNAMIC_LIGHT = i;
#ifdef _DEBUG
		m_bDYNAMIC_LIGHT = true;
#endif
	}
	void SetDYNAMIC_LIGHT( bool i )
	{
		m_nDYNAMIC_LIGHT = i ? 1 : 0;
#ifdef _DEBUG
		m_bDYNAMIC_LIGHT = true;
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
	int m_nTESSELLATION;
#ifdef _DEBUG
	bool m_bTESSELLATION;
#endif
public:
	void SetTESSELLATION( int i )
	{
		Assert( i >= 0 && i <= 2 );
		m_nTESSELLATION = i;
#ifdef _DEBUG
		m_bTESSELLATION = true;
#endif
	}
	void SetTESSELLATION( bool i )
	{
		m_nTESSELLATION = i ? 1 : 0;
#ifdef _DEBUG
		m_bTESSELLATION = true;
#endif
	}
private:
	int m_nDOWATERFOG;
#ifdef _DEBUG
	bool m_bDOWATERFOG;
#endif
public:
	void SetDOWATERFOG( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDOWATERFOG = i;
#ifdef _DEBUG
		m_bDOWATERFOG = true;
#endif
	}
	void SetDOWATERFOG( bool i )
	{
		m_nDOWATERFOG = i ? 1 : 0;
#ifdef _DEBUG
		m_bDOWATERFOG = true;
#endif
	}
public:
	// CONSTRUCTOR
	vertexlit_and_unlit_generic_vs30_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
	{
#ifdef _DEBUG
		m_bCOMPRESSED_VERTS = false;
#endif // _DEBUG
		m_nCOMPRESSED_VERTS = 0;
#ifdef _DEBUG
		m_bDYNAMIC_LIGHT = false;
#endif // _DEBUG
		m_nDYNAMIC_LIGHT = 0;
#ifdef _DEBUG
		m_bSKINNING = false;
#endif // _DEBUG
		m_nSKINNING = 0;
#ifdef _DEBUG
		m_bMORPHING = true;
#endif // _DEBUG
		m_nMORPHING = pShaderAPI->IsHWMorphingEnabled() ;
#ifdef _DEBUG
		m_bTESSELLATION = false;
#endif // _DEBUG
		m_nTESSELLATION = 0;
#ifdef _DEBUG
		m_bDOWATERFOG = true;
#endif // _DEBUG
		m_nDOWATERFOG = ( pShaderAPI->GetSceneFogMode() == MATERIAL_FOG_LINEAR_BELOW_FOG_Z ) ;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bCOMPRESSED_VERTS && m_bDYNAMIC_LIGHT && m_bSKINNING && m_bMORPHING && m_bTESSELLATION && m_bDOWATERFOG;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nCOMPRESSED_VERTS ) + ( 2 * m_nDYNAMIC_LIGHT ) + ( 4 * m_nSKINNING ) + ( 8 * m_nMORPHING ) + ( 16 * m_nTESSELLATION ) + ( 48 * m_nDOWATERFOG ) + 0;
	}
};
#define shaderDynamicTest_vertexlit_and_unlit_generic_vs30 vsh_forgot_to_set_dynamic_COMPRESSED_VERTS + vsh_forgot_to_set_dynamic_DYNAMIC_LIGHT + vsh_forgot_to_set_dynamic_SKINNING + vsh_forgot_to_set_dynamic_TESSELLATION + 0
