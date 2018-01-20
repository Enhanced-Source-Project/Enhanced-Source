// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// $ALPHATEST && $TRANSLUCENT
// !$ENVMAP && $ENVMAPFRESNEL
// !$ENVMAP && $ENVMAPMASK
// $ENVMAP && !$READNORMAL
// $ENVMAPFRESNEL && !$READNORMAL
// $PHONGFRESNEL && !$READNORMAL
// $RIMLIGHT && !$READNORMAL
// !$RIMLIGHT && $RIMLIGHTMODULATELIGHT
// !$BASETEXTURE2 && $BLENDMODULATE
// $MULTIBLEND && $BLENDMODULATE
// $MODEL && $BASETEXTURE2
// $MODEL && $MULTIBLEND
// $BASETEXTURE2 && $MULTIBLEND
// $ENVMAPMASK && $MULTIBLEND
// $SELFILLUM_MASK && $MULTIBLEND
// !$SELFILLUM && $SELFILLUM_MASK
// !$SELFILLUM && $SELFILLUM_ENVMAP_ALPHA
// $SELFILLUM_MASK && $SELFILLUM_ENVMAP_ALPHA
// $SELFILLUM_MASK && $ENVMAPMASK
// $SELFILLUM_ENVMAP_ALPHA && !$ENVMAPMASK
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW
// !$MODEL && $MORPHING_VTEX
// !$MODEL && $COMPRESSED_VERTS
// !$MODEL && $SKINNING
// !$MODEL && $MORPHING_VTEX
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW
#include "shaderlib/cshader.h"
class composite_ps30_Static_Index
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
	int m_nTRANSLUCENT;
#ifdef _DEBUG
	bool m_bTRANSLUCENT;
#endif
public:
	void SetTRANSLUCENT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nTRANSLUCENT = i;
#ifdef _DEBUG
		m_bTRANSLUCENT = true;
#endif
	}
	void SetTRANSLUCENT( bool i )
	{
		m_nTRANSLUCENT = i ? 1 : 0;
#ifdef _DEBUG
		m_bTRANSLUCENT = true;
#endif
	}
private:
	int m_nREADNORMAL;
#ifdef _DEBUG
	bool m_bREADNORMAL;
#endif
public:
	void SetREADNORMAL( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nREADNORMAL = i;
#ifdef _DEBUG
		m_bREADNORMAL = true;
#endif
	}
	void SetREADNORMAL( bool i )
	{
		m_nREADNORMAL = i ? 1 : 0;
#ifdef _DEBUG
		m_bREADNORMAL = true;
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
	int m_nPHONGFRESNEL;
#ifdef _DEBUG
	bool m_bPHONGFRESNEL;
#endif
public:
	void SetPHONGFRESNEL( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nPHONGFRESNEL = i;
#ifdef _DEBUG
		m_bPHONGFRESNEL = true;
#endif
	}
	void SetPHONGFRESNEL( bool i )
	{
		m_nPHONGFRESNEL = i ? 1 : 0;
#ifdef _DEBUG
		m_bPHONGFRESNEL = true;
#endif
	}
private:
	int m_nENVMAP;
#ifdef _DEBUG
	bool m_bENVMAP;
#endif
public:
	void SetENVMAP( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nENVMAP = i;
#ifdef _DEBUG
		m_bENVMAP = true;
#endif
	}
	void SetENVMAP( bool i )
	{
		m_nENVMAP = i ? 1 : 0;
#ifdef _DEBUG
		m_bENVMAP = true;
#endif
	}
private:
	int m_nENVMAPMASK;
#ifdef _DEBUG
	bool m_bENVMAPMASK;
#endif
public:
	void SetENVMAPMASK( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nENVMAPMASK = i;
#ifdef _DEBUG
		m_bENVMAPMASK = true;
#endif
	}
	void SetENVMAPMASK( bool i )
	{
		m_nENVMAPMASK = i ? 1 : 0;
#ifdef _DEBUG
		m_bENVMAPMASK = true;
#endif
	}
private:
	int m_nENVMAPFRESNEL;
#ifdef _DEBUG
	bool m_bENVMAPFRESNEL;
#endif
public:
	void SetENVMAPFRESNEL( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nENVMAPFRESNEL = i;
#ifdef _DEBUG
		m_bENVMAPFRESNEL = true;
#endif
	}
	void SetENVMAPFRESNEL( bool i )
	{
		m_nENVMAPFRESNEL = i ? 1 : 0;
#ifdef _DEBUG
		m_bENVMAPFRESNEL = true;
#endif
	}
private:
	int m_nRIMLIGHT;
#ifdef _DEBUG
	bool m_bRIMLIGHT;
#endif
public:
	void SetRIMLIGHT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nRIMLIGHT = i;
#ifdef _DEBUG
		m_bRIMLIGHT = true;
#endif
	}
	void SetRIMLIGHT( bool i )
	{
		m_nRIMLIGHT = i ? 1 : 0;
#ifdef _DEBUG
		m_bRIMLIGHT = true;
#endif
	}
private:
	int m_nRIMLIGHTMODULATELIGHT;
#ifdef _DEBUG
	bool m_bRIMLIGHTMODULATELIGHT;
#endif
public:
	void SetRIMLIGHTMODULATELIGHT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nRIMLIGHTMODULATELIGHT = i;
#ifdef _DEBUG
		m_bRIMLIGHTMODULATELIGHT = true;
#endif
	}
	void SetRIMLIGHTMODULATELIGHT( bool i )
	{
		m_nRIMLIGHTMODULATELIGHT = i ? 1 : 0;
#ifdef _DEBUG
		m_bRIMLIGHTMODULATELIGHT = true;
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
	int m_nSELFILLUM;
#ifdef _DEBUG
	bool m_bSELFILLUM;
#endif
public:
	void SetSELFILLUM( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSELFILLUM = i;
#ifdef _DEBUG
		m_bSELFILLUM = true;
#endif
	}
	void SetSELFILLUM( bool i )
	{
		m_nSELFILLUM = i ? 1 : 0;
#ifdef _DEBUG
		m_bSELFILLUM = true;
#endif
	}
private:
	int m_nSELFILLUM_MASK;
#ifdef _DEBUG
	bool m_bSELFILLUM_MASK;
#endif
public:
	void SetSELFILLUM_MASK( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSELFILLUM_MASK = i;
#ifdef _DEBUG
		m_bSELFILLUM_MASK = true;
#endif
	}
	void SetSELFILLUM_MASK( bool i )
	{
		m_nSELFILLUM_MASK = i ? 1 : 0;
#ifdef _DEBUG
		m_bSELFILLUM_MASK = true;
#endif
	}
private:
	int m_nSELFILLUM_ENVMAP_ALPHA;
#ifdef _DEBUG
	bool m_bSELFILLUM_ENVMAP_ALPHA;
#endif
public:
	void SetSELFILLUM_ENVMAP_ALPHA( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSELFILLUM_ENVMAP_ALPHA = i;
#ifdef _DEBUG
		m_bSELFILLUM_ENVMAP_ALPHA = true;
#endif
	}
	void SetSELFILLUM_ENVMAP_ALPHA( bool i )
	{
		m_nSELFILLUM_ENVMAP_ALPHA = i ? 1 : 0;
#ifdef _DEBUG
		m_bSELFILLUM_ENVMAP_ALPHA = true;
#endif
	}
public:
	// CONSTRUCTOR
	composite_ps30_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bALPHATEST = false;
#endif // _DEBUG
		m_nALPHATEST = 0;
#ifdef _DEBUG
		m_bTRANSLUCENT = false;
#endif // _DEBUG
		m_nTRANSLUCENT = 0;
#ifdef _DEBUG
		m_bREADNORMAL = false;
#endif // _DEBUG
		m_nREADNORMAL = 0;
#ifdef _DEBUG
		m_bNOCULL = false;
#endif // _DEBUG
		m_nNOCULL = 0;
#ifdef _DEBUG
		m_bPHONGFRESNEL = false;
#endif // _DEBUG
		m_nPHONGFRESNEL = 0;
#ifdef _DEBUG
		m_bENVMAP = false;
#endif // _DEBUG
		m_nENVMAP = 0;
#ifdef _DEBUG
		m_bENVMAPMASK = false;
#endif // _DEBUG
		m_nENVMAPMASK = 0;
#ifdef _DEBUG
		m_bENVMAPFRESNEL = false;
#endif // _DEBUG
		m_nENVMAPFRESNEL = 0;
#ifdef _DEBUG
		m_bRIMLIGHT = false;
#endif // _DEBUG
		m_nRIMLIGHT = 0;
#ifdef _DEBUG
		m_bRIMLIGHTMODULATELIGHT = false;
#endif // _DEBUG
		m_nRIMLIGHTMODULATELIGHT = 0;
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
		m_bSELFILLUM = false;
#endif // _DEBUG
		m_nSELFILLUM = 0;
#ifdef _DEBUG
		m_bSELFILLUM_MASK = false;
#endif // _DEBUG
		m_nSELFILLUM_MASK = 0;
#ifdef _DEBUG
		m_bSELFILLUM_ENVMAP_ALPHA = false;
#endif // _DEBUG
		m_nSELFILLUM_ENVMAP_ALPHA = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bALPHATEST && m_bTRANSLUCENT && m_bREADNORMAL && m_bNOCULL && m_bPHONGFRESNEL && m_bENVMAP && m_bENVMAPMASK && m_bENVMAPFRESNEL && m_bRIMLIGHT && m_bRIMLIGHTMODULATELIGHT && m_bBASETEXTURE2 && m_bBLENDMODULATE && m_bMULTIBLEND && m_bSELFILLUM && m_bSELFILLUM_MASK && m_bSELFILLUM_ENVMAP_ALPHA;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 2 * m_nALPHATEST ) + ( 4 * m_nTRANSLUCENT ) + ( 8 * m_nREADNORMAL ) + ( 16 * m_nNOCULL ) + ( 32 * m_nPHONGFRESNEL ) + ( 64 * m_nENVMAP ) + ( 128 * m_nENVMAPMASK ) + ( 256 * m_nENVMAPFRESNEL ) + ( 512 * m_nRIMLIGHT ) + ( 1024 * m_nRIMLIGHTMODULATELIGHT ) + ( 2048 * m_nBASETEXTURE2 ) + ( 4096 * m_nBLENDMODULATE ) + ( 8192 * m_nMULTIBLEND ) + ( 16384 * m_nSELFILLUM ) + ( 32768 * m_nSELFILLUM_MASK ) + ( 65536 * m_nSELFILLUM_ENVMAP_ALPHA ) + 0;
	}
};
#define shaderStaticTest_composite_ps30 psh_forgot_to_set_static_ALPHATEST + psh_forgot_to_set_static_TRANSLUCENT + psh_forgot_to_set_static_READNORMAL + psh_forgot_to_set_static_NOCULL + psh_forgot_to_set_static_PHONGFRESNEL + psh_forgot_to_set_static_ENVMAP + psh_forgot_to_set_static_ENVMAPMASK + psh_forgot_to_set_static_ENVMAPFRESNEL + psh_forgot_to_set_static_RIMLIGHT + psh_forgot_to_set_static_RIMLIGHTMODULATELIGHT + psh_forgot_to_set_static_BASETEXTURE2 + psh_forgot_to_set_static_BLENDMODULATE + psh_forgot_to_set_static_MULTIBLEND + psh_forgot_to_set_static_SELFILLUM + psh_forgot_to_set_static_SELFILLUM_MASK + psh_forgot_to_set_static_SELFILLUM_ENVMAP_ALPHA + 0
class composite_ps30_Dynamic_Index
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
	composite_ps30_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
	{
#ifdef _DEBUG
		m_bPIXELFOGTYPE = false;
#endif // _DEBUG
		m_nPIXELFOGTYPE = 0;
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
#define shaderDynamicTest_composite_ps30 psh_forgot_to_set_dynamic_PIXELFOGTYPE + 0
