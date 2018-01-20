// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// ( $FLASHLIGHT == 1 ) && ( $CUBEMAP == 1 ) 
// ($DETAILTEXTURE == 0 ) && ( $DETAIL_BLEND_MODE != 0 )
// ($DETAILTEXTURE == 0 ) && ( $SEAMLESS_DETAIL )
// ($ENVMAPMASK || $SELFILLUM_ENVMAPMASK_ALPHA) && ($SEAMLESS_BASE || $SEAMLESS_DETAIL)
// $BASEALPHAENVMAPMASK && $ENVMAPMASK
// $BASEALPHAENVMAPMASK && $SELFILLUM
// $SELFILLUM && $SELFILLUM_ENVMAPMASK_ALPHA
// $SELFILLUM_ENVMAPMASK_ALPHA && (! $ENVMAPMASK)
// $BASEALPHAENVMAPMASK && ($SEAMLESS_BASE || $SEAMLESS_DETAIL)
// ($DISTANCEALPHA == 0) && ($DISTANCEALPHAFROMDETAIL || $SOFT_MASK || $OUTLINE || $OUTER_GLOW)
// ($DETAILTEXTURE == 0) && ($DISTANCEALPHAFROMDETAIL)
// ( $CUBEMAP == 0 ) && ( ( $ENVMAPFRESNEL == 1 ) || ( $BASEALPHAENVMAPMASK == 1 ) )
// ( $CUBEMAP == 0 ) && ( $ENVMAPMASK == 1 ) && ( $SELFILLUM_ENVMAPMASK_ALPHA == 0 )
// ( $FLASHLIGHT == 0 ) && ( $FLASHLIGHTSHADOWS == 1 )	
// ( $FLASHLIGHT == 0 ) && ( $UBERLIGHT == 1 )	
// ($DISTANCEALPHA) && ($ENVMAPMASK || $BASEALPHAENVMAPMASK || $SELFILLUM || $SELFILLUM_ENVMAPMASK_ALPHA || $ENVMAPFRESNEL)
// ($DISTANCEALPHA) && ($SEAMLESS_BASE || $SEAMLESS_DETAIL || $CUBEMAP )
// ($DISTANCEALPHA) && ($WRITEWATERFOGTODESTALPHA || $PIXELFOGTYPE || $FLASHLIGHT || $FLASHLIGHTSHADOWS )
// $DESATURATEWITHBASEALPHA && ( $SEAMLESS_BASE || $DETAILTEXTURE )
// $DESATURATEWITHBASEALPHA && ( $SELFILLUM || $SELFILLUM_ENVMAPMASK_ALPHA )
// $DESATURATEWITHBASEALPHA && $BASEALPHAENVMAPMASK
// $FLASHLIGHTSHADOWS && ($LIGHTING_PREVIEW == 3)
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW
#include "shaderlib/cshader.h"
class vertexlit_and_unlit_generic_ps30_Static_Index
{
private:
	int m_nDETAILTEXTURE;
#ifdef _DEBUG
	bool m_bDETAILTEXTURE;
#endif
public:
	void SetDETAILTEXTURE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDETAILTEXTURE = i;
#ifdef _DEBUG
		m_bDETAILTEXTURE = true;
#endif
	}
	void SetDETAILTEXTURE( bool i )
	{
		m_nDETAILTEXTURE = i ? 1 : 0;
#ifdef _DEBUG
		m_bDETAILTEXTURE = true;
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
	int m_nDIFFUSELIGHTING;
#ifdef _DEBUG
	bool m_bDIFFUSELIGHTING;
#endif
public:
	void SetDIFFUSELIGHTING( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDIFFUSELIGHTING = i;
#ifdef _DEBUG
		m_bDIFFUSELIGHTING = true;
#endif
	}
	void SetDIFFUSELIGHTING( bool i )
	{
		m_nDIFFUSELIGHTING = i ? 1 : 0;
#ifdef _DEBUG
		m_bDIFFUSELIGHTING = true;
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
	int m_nBASEALPHAENVMAPMASK;
#ifdef _DEBUG
	bool m_bBASEALPHAENVMAPMASK;
#endif
public:
	void SetBASEALPHAENVMAPMASK( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nBASEALPHAENVMAPMASK = i;
#ifdef _DEBUG
		m_bBASEALPHAENVMAPMASK = true;
#endif
	}
	void SetBASEALPHAENVMAPMASK( bool i )
	{
		m_nBASEALPHAENVMAPMASK = i ? 1 : 0;
#ifdef _DEBUG
		m_bBASEALPHAENVMAPMASK = true;
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
	int m_nSELFILLUM_ENVMAPMASK_ALPHA;
#ifdef _DEBUG
	bool m_bSELFILLUM_ENVMAPMASK_ALPHA;
#endif
public:
	void SetSELFILLUM_ENVMAPMASK_ALPHA( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSELFILLUM_ENVMAPMASK_ALPHA = i;
#ifdef _DEBUG
		m_bSELFILLUM_ENVMAPMASK_ALPHA = true;
#endif
	}
	void SetSELFILLUM_ENVMAPMASK_ALPHA( bool i )
	{
		m_nSELFILLUM_ENVMAPMASK_ALPHA = i ? 1 : 0;
#ifdef _DEBUG
		m_bSELFILLUM_ENVMAPMASK_ALPHA = true;
#endif
	}
private:
	int m_nDETAIL_BLEND_MODE;
#ifdef _DEBUG
	bool m_bDETAIL_BLEND_MODE;
#endif
public:
	void SetDETAIL_BLEND_MODE( int i )
	{
		Assert( i >= 0 && i <= 9 );
		m_nDETAIL_BLEND_MODE = i;
#ifdef _DEBUG
		m_bDETAIL_BLEND_MODE = true;
#endif
	}
	void SetDETAIL_BLEND_MODE( bool i )
	{
		m_nDETAIL_BLEND_MODE = i ? 1 : 0;
#ifdef _DEBUG
		m_bDETAIL_BLEND_MODE = true;
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
	int m_nDISTANCEALPHA;
#ifdef _DEBUG
	bool m_bDISTANCEALPHA;
#endif
public:
	void SetDISTANCEALPHA( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDISTANCEALPHA = i;
#ifdef _DEBUG
		m_bDISTANCEALPHA = true;
#endif
	}
	void SetDISTANCEALPHA( bool i )
	{
		m_nDISTANCEALPHA = i ? 1 : 0;
#ifdef _DEBUG
		m_bDISTANCEALPHA = true;
#endif
	}
private:
	int m_nDISTANCEALPHAFROMDETAIL;
#ifdef _DEBUG
	bool m_bDISTANCEALPHAFROMDETAIL;
#endif
public:
	void SetDISTANCEALPHAFROMDETAIL( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDISTANCEALPHAFROMDETAIL = i;
#ifdef _DEBUG
		m_bDISTANCEALPHAFROMDETAIL = true;
#endif
	}
	void SetDISTANCEALPHAFROMDETAIL( bool i )
	{
		m_nDISTANCEALPHAFROMDETAIL = i ? 1 : 0;
#ifdef _DEBUG
		m_bDISTANCEALPHAFROMDETAIL = true;
#endif
	}
private:
	int m_nSOFT_MASK;
#ifdef _DEBUG
	bool m_bSOFT_MASK;
#endif
public:
	void SetSOFT_MASK( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSOFT_MASK = i;
#ifdef _DEBUG
		m_bSOFT_MASK = true;
#endif
	}
	void SetSOFT_MASK( bool i )
	{
		m_nSOFT_MASK = i ? 1 : 0;
#ifdef _DEBUG
		m_bSOFT_MASK = true;
#endif
	}
private:
	int m_nOUTLINE;
#ifdef _DEBUG
	bool m_bOUTLINE;
#endif
public:
	void SetOUTLINE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nOUTLINE = i;
#ifdef _DEBUG
		m_bOUTLINE = true;
#endif
	}
	void SetOUTLINE( bool i )
	{
		m_nOUTLINE = i ? 1 : 0;
#ifdef _DEBUG
		m_bOUTLINE = true;
#endif
	}
private:
	int m_nOUTER_GLOW;
#ifdef _DEBUG
	bool m_bOUTER_GLOW;
#endif
public:
	void SetOUTER_GLOW( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nOUTER_GLOW = i;
#ifdef _DEBUG
		m_bOUTER_GLOW = true;
#endif
	}
	void SetOUTER_GLOW( bool i )
	{
		m_nOUTER_GLOW = i ? 1 : 0;
#ifdef _DEBUG
		m_bOUTER_GLOW = true;
#endif
	}
private:
	int m_nFLASHLIGHTDEPTHFILTERMODE;
#ifdef _DEBUG
	bool m_bFLASHLIGHTDEPTHFILTERMODE;
#endif
public:
	void SetFLASHLIGHTDEPTHFILTERMODE( int i )
	{
		Assert( i >= 0 && i <= 2 );
		m_nFLASHLIGHTDEPTHFILTERMODE = i;
#ifdef _DEBUG
		m_bFLASHLIGHTDEPTHFILTERMODE = true;
#endif
	}
	void SetFLASHLIGHTDEPTHFILTERMODE( bool i )
	{
		m_nFLASHLIGHTDEPTHFILTERMODE = i ? 1 : 0;
#ifdef _DEBUG
		m_bFLASHLIGHTDEPTHFILTERMODE = true;
#endif
	}
private:
	int m_nSHADER_SRGB_READ;
#ifdef _DEBUG
	bool m_bSHADER_SRGB_READ;
#endif
public:
	void SetSHADER_SRGB_READ( int i )
	{
		Assert( i >= 0 && i <= 0 );
		m_nSHADER_SRGB_READ = i;
#ifdef _DEBUG
		m_bSHADER_SRGB_READ = true;
#endif
	}
	void SetSHADER_SRGB_READ( bool i )
	{
		m_nSHADER_SRGB_READ = i ? 1 : 0;
#ifdef _DEBUG
		m_bSHADER_SRGB_READ = true;
#endif
	}
private:
	int m_nDESATURATEWITHBASEALPHA;
#ifdef _DEBUG
	bool m_bDESATURATEWITHBASEALPHA;
#endif
public:
	void SetDESATURATEWITHBASEALPHA( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDESATURATEWITHBASEALPHA = i;
#ifdef _DEBUG
		m_bDESATURATEWITHBASEALPHA = true;
#endif
	}
	void SetDESATURATEWITHBASEALPHA( bool i )
	{
		m_nDESATURATEWITHBASEALPHA = i ? 1 : 0;
#ifdef _DEBUG
		m_bDESATURATEWITHBASEALPHA = true;
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
public:
	// CONSTRUCTOR
	vertexlit_and_unlit_generic_ps30_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bDETAILTEXTURE = false;
#endif // _DEBUG
		m_nDETAILTEXTURE = 0;
#ifdef _DEBUG
		m_bCUBEMAP = false;
#endif // _DEBUG
		m_nCUBEMAP = 0;
#ifdef _DEBUG
		m_bDIFFUSELIGHTING = false;
#endif // _DEBUG
		m_nDIFFUSELIGHTING = 0;
#ifdef _DEBUG
		m_bENVMAPMASK = false;
#endif // _DEBUG
		m_nENVMAPMASK = 0;
#ifdef _DEBUG
		m_bBASEALPHAENVMAPMASK = false;
#endif // _DEBUG
		m_nBASEALPHAENVMAPMASK = 0;
#ifdef _DEBUG
		m_bSELFILLUM = false;
#endif // _DEBUG
		m_nSELFILLUM = 0;
#ifdef _DEBUG
		m_bVERTEXCOLOR = false;
#endif // _DEBUG
		m_nVERTEXCOLOR = 0;
#ifdef _DEBUG
		m_bFLASHLIGHT = false;
#endif // _DEBUG
		m_nFLASHLIGHT = 0;
#ifdef _DEBUG
		m_bSELFILLUM_ENVMAPMASK_ALPHA = false;
#endif // _DEBUG
		m_nSELFILLUM_ENVMAPMASK_ALPHA = 0;
#ifdef _DEBUG
		m_bDETAIL_BLEND_MODE = false;
#endif // _DEBUG
		m_nDETAIL_BLEND_MODE = 0;
#ifdef _DEBUG
		m_bSEAMLESS_BASE = false;
#endif // _DEBUG
		m_nSEAMLESS_BASE = 0;
#ifdef _DEBUG
		m_bSEAMLESS_DETAIL = false;
#endif // _DEBUG
		m_nSEAMLESS_DETAIL = 0;
#ifdef _DEBUG
		m_bDISTANCEALPHA = false;
#endif // _DEBUG
		m_nDISTANCEALPHA = 0;
#ifdef _DEBUG
		m_bDISTANCEALPHAFROMDETAIL = false;
#endif // _DEBUG
		m_nDISTANCEALPHAFROMDETAIL = 0;
#ifdef _DEBUG
		m_bSOFT_MASK = false;
#endif // _DEBUG
		m_nSOFT_MASK = 0;
#ifdef _DEBUG
		m_bOUTLINE = false;
#endif // _DEBUG
		m_nOUTLINE = 0;
#ifdef _DEBUG
		m_bOUTER_GLOW = false;
#endif // _DEBUG
		m_nOUTER_GLOW = 0;
#ifdef _DEBUG
		m_bFLASHLIGHTDEPTHFILTERMODE = false;
#endif // _DEBUG
		m_nFLASHLIGHTDEPTHFILTERMODE = 0;
#ifdef _DEBUG
		m_bSHADER_SRGB_READ = false;
#endif // _DEBUG
		m_nSHADER_SRGB_READ = 0;
#ifdef _DEBUG
		m_bDESATURATEWITHBASEALPHA = false;
#endif // _DEBUG
		m_nDESATURATEWITHBASEALPHA = 0;
#ifdef _DEBUG
		m_bLIGHTING_PREVIEW = false;
#endif // _DEBUG
		m_nLIGHTING_PREVIEW = 0;
#ifdef _DEBUG
		m_bENVMAPFRESNEL = false;
#endif // _DEBUG
		m_nENVMAPFRESNEL = 0;
#ifdef _DEBUG
		m_bFOW = false;
#endif // _DEBUG
		m_nFOW = 0;
#ifdef _DEBUG
		m_bDOPIXELFOG = true;
#endif // _DEBUG
		m_nDOPIXELFOG = !IS_FLAG_SET( MATERIAL_VAR_VERTEXFOG ) ? 1 : 0 ;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bDETAILTEXTURE && m_bCUBEMAP && m_bDIFFUSELIGHTING && m_bENVMAPMASK && m_bBASEALPHAENVMAPMASK && m_bSELFILLUM && m_bVERTEXCOLOR && m_bFLASHLIGHT && m_bSELFILLUM_ENVMAPMASK_ALPHA && m_bDETAIL_BLEND_MODE && m_bSEAMLESS_BASE && m_bSEAMLESS_DETAIL && m_bDISTANCEALPHA && m_bDISTANCEALPHAFROMDETAIL && m_bSOFT_MASK && m_bOUTLINE && m_bOUTER_GLOW && m_bFLASHLIGHTDEPTHFILTERMODE && m_bSHADER_SRGB_READ && m_bDESATURATEWITHBASEALPHA && m_bLIGHTING_PREVIEW && m_bENVMAPFRESNEL && m_bFOW && m_bDOPIXELFOG;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 16 * m_nDETAILTEXTURE ) + ( 32 * m_nCUBEMAP ) + ( 64 * m_nDIFFUSELIGHTING ) + ( 128 * m_nENVMAPMASK ) + ( 256 * m_nBASEALPHAENVMAPMASK ) + ( 512 * m_nSELFILLUM ) + ( 1024 * m_nVERTEXCOLOR ) + ( 2048 * m_nFLASHLIGHT ) + ( 4096 * m_nSELFILLUM_ENVMAPMASK_ALPHA ) + ( 8192 * m_nDETAIL_BLEND_MODE ) + ( 81920 * m_nSEAMLESS_BASE ) + ( 163840 * m_nSEAMLESS_DETAIL ) + ( 327680 * m_nDISTANCEALPHA ) + ( 655360 * m_nDISTANCEALPHAFROMDETAIL ) + ( 1310720 * m_nSOFT_MASK ) + ( 2621440 * m_nOUTLINE ) + ( 5242880 * m_nOUTER_GLOW ) + ( 10485760 * m_nFLASHLIGHTDEPTHFILTERMODE ) + ( 31457280 * m_nSHADER_SRGB_READ ) + ( 31457280 * m_nDESATURATEWITHBASEALPHA ) + ( 62914560 * m_nLIGHTING_PREVIEW ) + ( 251658240 * m_nENVMAPFRESNEL ) + ( 503316480 * m_nFOW ) + ( 1006632960 * m_nDOPIXELFOG ) + 0;
	}
};
#define shaderStaticTest_vertexlit_and_unlit_generic_ps30 psh_forgot_to_set_static_DETAILTEXTURE + psh_forgot_to_set_static_CUBEMAP + psh_forgot_to_set_static_DIFFUSELIGHTING + psh_forgot_to_set_static_ENVMAPMASK + psh_forgot_to_set_static_BASEALPHAENVMAPMASK + psh_forgot_to_set_static_SELFILLUM + psh_forgot_to_set_static_VERTEXCOLOR + psh_forgot_to_set_static_FLASHLIGHT + psh_forgot_to_set_static_SELFILLUM_ENVMAPMASK_ALPHA + psh_forgot_to_set_static_DETAIL_BLEND_MODE + psh_forgot_to_set_static_SEAMLESS_BASE + psh_forgot_to_set_static_SEAMLESS_DETAIL + psh_forgot_to_set_static_DISTANCEALPHA + psh_forgot_to_set_static_DISTANCEALPHAFROMDETAIL + psh_forgot_to_set_static_SOFT_MASK + psh_forgot_to_set_static_OUTLINE + psh_forgot_to_set_static_OUTER_GLOW + psh_forgot_to_set_static_FLASHLIGHTDEPTHFILTERMODE + psh_forgot_to_set_static_SHADER_SRGB_READ + psh_forgot_to_set_static_DESATURATEWITHBASEALPHA + psh_forgot_to_set_static_LIGHTING_PREVIEW + psh_forgot_to_set_static_ENVMAPFRESNEL + psh_forgot_to_set_static_FOW + 0
class vertexlit_and_unlit_generic_ps30_Dynamic_Index
{
private:
	int m_nFLASHLIGHTSHADOWS;
#ifdef _DEBUG
	bool m_bFLASHLIGHTSHADOWS;
#endif
public:
	void SetFLASHLIGHTSHADOWS( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nFLASHLIGHTSHADOWS = i;
#ifdef _DEBUG
		m_bFLASHLIGHTSHADOWS = true;
#endif
	}
	void SetFLASHLIGHTSHADOWS( bool i )
	{
		m_nFLASHLIGHTSHADOWS = i ? 1 : 0;
#ifdef _DEBUG
		m_bFLASHLIGHTSHADOWS = true;
#endif
	}
private:
	int m_nWRITEWATERFOGTODESTALPHA;
#ifdef _DEBUG
	bool m_bWRITEWATERFOGTODESTALPHA;
#endif
public:
	void SetWRITEWATERFOGTODESTALPHA( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nWRITEWATERFOGTODESTALPHA = i;
#ifdef _DEBUG
		m_bWRITEWATERFOGTODESTALPHA = true;
#endif
	}
	void SetWRITEWATERFOGTODESTALPHA( bool i )
	{
		m_nWRITEWATERFOGTODESTALPHA = i ? 1 : 0;
#ifdef _DEBUG
		m_bWRITEWATERFOGTODESTALPHA = true;
#endif
	}
private:
	int m_nUBERLIGHT;
#ifdef _DEBUG
	bool m_bUBERLIGHT;
#endif
public:
	void SetUBERLIGHT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nUBERLIGHT = i;
#ifdef _DEBUG
		m_bUBERLIGHT = true;
#endif
	}
	void SetUBERLIGHT( bool i )
	{
		m_nUBERLIGHT = i ? 1 : 0;
#ifdef _DEBUG
		m_bUBERLIGHT = true;
#endif
	}
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
	vertexlit_and_unlit_generic_ps30_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
	{
#ifdef _DEBUG
		m_bFLASHLIGHTSHADOWS = false;
#endif // _DEBUG
		m_nFLASHLIGHTSHADOWS = 0;
#ifdef _DEBUG
		m_bWRITEWATERFOGTODESTALPHA = false;
#endif // _DEBUG
		m_nWRITEWATERFOGTODESTALPHA = 0;
#ifdef _DEBUG
		m_bUBERLIGHT = false;
#endif // _DEBUG
		m_nUBERLIGHT = 0;
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
		bool bAllDynamicVarsDefined = m_bFLASHLIGHTSHADOWS && m_bWRITEWATERFOGTODESTALPHA && m_bUBERLIGHT && m_bPIXELFOGTYPE;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nFLASHLIGHTSHADOWS ) + ( 2 * m_nWRITEWATERFOGTODESTALPHA ) + ( 4 * m_nUBERLIGHT ) + ( 8 * m_nPIXELFOGTYPE ) + 0;
	}
};
#define shaderDynamicTest_vertexlit_and_unlit_generic_ps30 psh_forgot_to_set_dynamic_FLASHLIGHTSHADOWS + psh_forgot_to_set_dynamic_WRITEWATERFOGTODESTALPHA + psh_forgot_to_set_dynamic_UBERLIGHT + 0
