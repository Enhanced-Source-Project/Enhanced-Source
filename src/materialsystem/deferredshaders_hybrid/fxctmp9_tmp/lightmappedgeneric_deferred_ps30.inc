// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// $SEAMLESS && $PARALLAX_MAPPING		
// $SEAMLESS && ( $OUTLINE || $SOFTEDGES)
// $BASETEXTURE2 && ( $OUTLINE || $SOFTEDGES)
// $BUMPMAP2 && ( $OUTLINE || $SOFTEDGES)
// $SELFILLUM && ( $OUTLINE || $SOFTEDGES)
// $MASKEDBLENDING && ( $OUTLINE || $SOFTEDGES)
// $FANCY_BLENDING && ( $OUTLINE || $SOFTEDGES)
// $LIGHTING_PREVIEW && ( $OUTLINE || $SOFTEDGES)
// ($FASTPATH == 0) && ( $OUTLINE || $SOFTEDGES)
// (($DETAIL_BLEND_MODE != 12 ) && $BUMPMAP) && ( $OUTLINE || $SOFTEDGES)
// ($WARPLIGHTING) && ( $OUTLINE || $SOFTEDGES)
// ($BUMPMAP) && ( $OUTLINE || $SOFTEDGES)
// ($DETAIL_BLEND_MODE == 7 ) && ( $SOFTEDGES || $BUMPMAP2)
// ($DETAIL_BLEND_MODE == 6 )
// ($DETAIL_BLEND_MODE == 10 ) && ($BUMPMAP == 0 )
// ($DETAIL_BLEND_MODE == 11 ) && ($BUMPMAP != 0 )
// ($FANCY_BLENDING) && ($DETAIL_BLEND_MODE == 7 )
// ($PARALLAX_MAPPING) && ($BASETEXTURE2 || $BUMPMAP2 || $DETAILTEXTURE  || $BASETEXTURE2 )
// ($FANCY_BLENDING) && ($BASETEXTURE2 == 0 ) && ($BUMPMAP2 == 0 )
// ( $FLASHLIGHT == 0 ) && ( $FLASHLIGHTSHADOWS == 1 )
// ($DETAILTEXTURE == 0) && ($DETAIL_BLEND_MODE != 12)
// $BUMPMAP2 && $WARPLIGHTING
// $WARPLIGHTING && ( $DETAIL_BLEND_MODE != 12 )
// $ENVMAPMASK && $BUMPMAP
// $NORMALMAPALPHAENVMAPMASK && $BASEALPHAENVMAPMASK
// $NORMALMAPALPHAENVMAPMASK && $ENVMAPMASK
// $BASEALPHAENVMAPMASK && $ENVMAPMASK
// $BASEALPHAENVMAPMASK && $SELFILLUM
// !$FASTPATH && $FASTPATHENVMAPCONTRAST
// !$FASTPATH && $FASTPATHENVMAPTINT
// !$BUMPMAP && $DIFFUSEBUMPMAP
// !$BUMPMAP && $BUMPMAP2
// $ENVMAPMASK && $BUMPMAP2
// $BASETEXTURENOENVMAP && ( !$BASETEXTURE2 || !$CUBEMAP )
// $BASETEXTURE2NOENVMAP && ( !$BASETEXTURE2 || !$CUBEMAP )
// $BASEALPHAENVMAPMASK && $BUMPMAP
// $PARALLAXMAP && ( $DETAIL_BLEND_MODE != 12 )
// $SEAMLESS && $PARALLAXMAP
// $SEAMLESS && ( $DETAIL_BLEND_MODE != 12 )
// $SEAMLESS && $MASKEDBLENDING
// $BUMPMASK && ( $SEAMLESS ||  ( $DETAILTEXTURE != 12 ) || $SELFILLUM || $BASETEXTURENOENVMAP || $BASETEXTURE2 )
// $FANCY_BLENDING && $BUMPMAP && $DETAILTEXTURE
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
class lightmappedgeneric_deferred_ps30_Static_Index
{
private:
	int m_nMASKEDBLENDING;
#ifdef _DEBUG
	bool m_bMASKEDBLENDING;
#endif
public:
	void SetMASKEDBLENDING( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nMASKEDBLENDING = i;
#ifdef _DEBUG
		m_bMASKEDBLENDING = true;
#endif
	}
	void SetMASKEDBLENDING( bool i )
	{
		m_nMASKEDBLENDING = i ? 1 : 0;
#ifdef _DEBUG
		m_bMASKEDBLENDING = true;
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
	int m_nBUMPMAP2;
#ifdef _DEBUG
	bool m_bBUMPMAP2;
#endif
public:
	void SetBUMPMAP2( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nBUMPMAP2 = i;
#ifdef _DEBUG
		m_bBUMPMAP2 = true;
#endif
	}
	void SetBUMPMAP2( bool i )
	{
		m_nBUMPMAP2 = i ? 1 : 0;
#ifdef _DEBUG
		m_bBUMPMAP2 = true;
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
		Assert( i >= 0 && i <= 2 );
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
	int m_nNORMALMAPALPHAENVMAPMASK;
#ifdef _DEBUG
	bool m_bNORMALMAPALPHAENVMAPMASK;
#endif
public:
	void SetNORMALMAPALPHAENVMAPMASK( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nNORMALMAPALPHAENVMAPMASK = i;
#ifdef _DEBUG
		m_bNORMALMAPALPHAENVMAPMASK = true;
#endif
	}
	void SetNORMALMAPALPHAENVMAPMASK( bool i )
	{
		m_nNORMALMAPALPHAENVMAPMASK = i ? 1 : 0;
#ifdef _DEBUG
		m_bNORMALMAPALPHAENVMAPMASK = true;
#endif
	}
private:
	int m_nDIFFUSEBUMPMAP;
#ifdef _DEBUG
	bool m_bDIFFUSEBUMPMAP;
#endif
public:
	void SetDIFFUSEBUMPMAP( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDIFFUSEBUMPMAP = i;
#ifdef _DEBUG
		m_bDIFFUSEBUMPMAP = true;
#endif
	}
	void SetDIFFUSEBUMPMAP( bool i )
	{
		m_nDIFFUSEBUMPMAP = i ? 1 : 0;
#ifdef _DEBUG
		m_bDIFFUSEBUMPMAP = true;
#endif
	}
private:
	int m_nBASETEXTURENOENVMAP;
#ifdef _DEBUG
	bool m_bBASETEXTURENOENVMAP;
#endif
public:
	void SetBASETEXTURENOENVMAP( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nBASETEXTURENOENVMAP = i;
#ifdef _DEBUG
		m_bBASETEXTURENOENVMAP = true;
#endif
	}
	void SetBASETEXTURENOENVMAP( bool i )
	{
		m_nBASETEXTURENOENVMAP = i ? 1 : 0;
#ifdef _DEBUG
		m_bBASETEXTURENOENVMAP = true;
#endif
	}
private:
	int m_nBASETEXTURE2NOENVMAP;
#ifdef _DEBUG
	bool m_bBASETEXTURE2NOENVMAP;
#endif
public:
	void SetBASETEXTURE2NOENVMAP( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nBASETEXTURE2NOENVMAP = i;
#ifdef _DEBUG
		m_bBASETEXTURE2NOENVMAP = true;
#endif
	}
	void SetBASETEXTURE2NOENVMAP( bool i )
	{
		m_nBASETEXTURE2NOENVMAP = i ? 1 : 0;
#ifdef _DEBUG
		m_bBASETEXTURE2NOENVMAP = true;
#endif
	}
private:
	int m_nWARPLIGHTING;
#ifdef _DEBUG
	bool m_bWARPLIGHTING;
#endif
public:
	void SetWARPLIGHTING( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nWARPLIGHTING = i;
#ifdef _DEBUG
		m_bWARPLIGHTING = true;
#endif
	}
	void SetWARPLIGHTING( bool i )
	{
		m_nWARPLIGHTING = i ? 1 : 0;
#ifdef _DEBUG
		m_bWARPLIGHTING = true;
#endif
	}
private:
	int m_nFANCY_BLENDING;
#ifdef _DEBUG
	bool m_bFANCY_BLENDING;
#endif
public:
	void SetFANCY_BLENDING( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nFANCY_BLENDING = i;
#ifdef _DEBUG
		m_bFANCY_BLENDING = true;
#endif
	}
	void SetFANCY_BLENDING( bool i )
	{
		m_nFANCY_BLENDING = i ? 1 : 0;
#ifdef _DEBUG
		m_bFANCY_BLENDING = true;
#endif
	}
private:
	int m_nPARALLAX_MAPPING;
#ifdef _DEBUG
	bool m_bPARALLAX_MAPPING;
#endif
public:
	void SetPARALLAX_MAPPING( int i )
	{
		Assert( i >= 0 && i <= 0 );
		m_nPARALLAX_MAPPING = i;
#ifdef _DEBUG
		m_bPARALLAX_MAPPING = true;
#endif
	}
	void SetPARALLAX_MAPPING( bool i )
	{
		m_nPARALLAX_MAPPING = i ? 1 : 0;
#ifdef _DEBUG
		m_bPARALLAX_MAPPING = true;
#endif
	}
private:
	int m_nSEAMLESS;
#ifdef _DEBUG
	bool m_bSEAMLESS;
#endif
public:
	void SetSEAMLESS( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSEAMLESS = i;
#ifdef _DEBUG
		m_bSEAMLESS = true;
#endif
	}
	void SetSEAMLESS( bool i )
	{
		m_nSEAMLESS = i ? 1 : 0;
#ifdef _DEBUG
		m_bSEAMLESS = true;
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
		Assert( i >= 0 && i <= 0 );
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
	int m_nSOFTEDGES;
#ifdef _DEBUG
	bool m_bSOFTEDGES;
#endif
public:
	void SetSOFTEDGES( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSOFTEDGES = i;
#ifdef _DEBUG
		m_bSOFTEDGES = true;
#endif
	}
	void SetSOFTEDGES( bool i )
	{
		m_nSOFTEDGES = i ? 1 : 0;
#ifdef _DEBUG
		m_bSOFTEDGES = true;
#endif
	}
private:
	int m_nBUMPMASK;
#ifdef _DEBUG
	bool m_bBUMPMASK;
#endif
public:
	void SetBUMPMASK( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nBUMPMASK = i;
#ifdef _DEBUG
		m_bBUMPMASK = true;
#endif
	}
	void SetBUMPMASK( bool i )
	{
		m_nBUMPMASK = i ? 1 : 0;
#ifdef _DEBUG
		m_bBUMPMASK = true;
#endif
	}
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
	int m_nDETAIL_BLEND_MODE;
#ifdef _DEBUG
	bool m_bDETAIL_BLEND_MODE;
#endif
public:
	void SetDETAIL_BLEND_MODE( int i )
	{
		Assert( i >= 0 && i <= 12 );
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
	int m_nLIGHTING_PREVIEW;
#ifdef _DEBUG
	bool m_bLIGHTING_PREVIEW;
#endif
public:
	void SetLIGHTING_PREVIEW( int i )
	{
		Assert( i >= 0 && i <= 0 );
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
	lightmappedgeneric_deferred_ps30_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bMASKEDBLENDING = false;
#endif // _DEBUG
		m_nMASKEDBLENDING = 0;
#ifdef _DEBUG
		m_bBASETEXTURE2 = false;
#endif // _DEBUG
		m_nBASETEXTURE2 = 0;
#ifdef _DEBUG
		m_bBUMPMAP = false;
#endif // _DEBUG
		m_nBUMPMAP = 0;
#ifdef _DEBUG
		m_bBUMPMAP2 = false;
#endif // _DEBUG
		m_nBUMPMAP2 = 0;
#ifdef _DEBUG
		m_bCUBEMAP = false;
#endif // _DEBUG
		m_nCUBEMAP = 0;
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
		m_bNORMALMAPALPHAENVMAPMASK = false;
#endif // _DEBUG
		m_nNORMALMAPALPHAENVMAPMASK = 0;
#ifdef _DEBUG
		m_bDIFFUSEBUMPMAP = false;
#endif // _DEBUG
		m_nDIFFUSEBUMPMAP = 0;
#ifdef _DEBUG
		m_bBASETEXTURENOENVMAP = false;
#endif // _DEBUG
		m_nBASETEXTURENOENVMAP = 0;
#ifdef _DEBUG
		m_bBASETEXTURE2NOENVMAP = false;
#endif // _DEBUG
		m_nBASETEXTURE2NOENVMAP = 0;
#ifdef _DEBUG
		m_bWARPLIGHTING = false;
#endif // _DEBUG
		m_nWARPLIGHTING = 0;
#ifdef _DEBUG
		m_bFANCY_BLENDING = false;
#endif // _DEBUG
		m_nFANCY_BLENDING = 0;
#ifdef _DEBUG
		m_bPARALLAX_MAPPING = false;
#endif // _DEBUG
		m_nPARALLAX_MAPPING = 0;
#ifdef _DEBUG
		m_bSEAMLESS = false;
#endif // _DEBUG
		m_nSEAMLESS = 0;
#ifdef _DEBUG
		m_bOUTLINE = false;
#endif // _DEBUG
		m_nOUTLINE = 0;
#ifdef _DEBUG
		m_bSOFTEDGES = false;
#endif // _DEBUG
		m_nSOFTEDGES = 0;
#ifdef _DEBUG
		m_bBUMPMASK = false;
#endif // _DEBUG
		m_nBUMPMASK = 0;
#ifdef _DEBUG
		m_bDETAILTEXTURE = false;
#endif // _DEBUG
		m_nDETAILTEXTURE = 0;
#ifdef _DEBUG
		m_bDETAIL_BLEND_MODE = false;
#endif // _DEBUG
		m_nDETAIL_BLEND_MODE = 0;
#ifdef _DEBUG
		m_bSHADER_SRGB_READ = false;
#endif // _DEBUG
		m_nSHADER_SRGB_READ = 0;
#ifdef _DEBUG
		m_bLIGHTING_PREVIEW = false;
#endif // _DEBUG
		m_nLIGHTING_PREVIEW = 0;
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
		bool bAllStaticVarsDefined = m_bMASKEDBLENDING && m_bBASETEXTURE2 && m_bBUMPMAP && m_bBUMPMAP2 && m_bCUBEMAP && m_bENVMAPMASK && m_bBASEALPHAENVMAPMASK && m_bSELFILLUM && m_bNORMALMAPALPHAENVMAPMASK && m_bDIFFUSEBUMPMAP && m_bBASETEXTURENOENVMAP && m_bBASETEXTURE2NOENVMAP && m_bWARPLIGHTING && m_bFANCY_BLENDING && m_bPARALLAX_MAPPING && m_bSEAMLESS && m_bOUTLINE && m_bSOFTEDGES && m_bBUMPMASK && m_bDETAILTEXTURE && m_bDETAIL_BLEND_MODE && m_bSHADER_SRGB_READ && m_bLIGHTING_PREVIEW && m_bDOPIXELFOG;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 32 * m_nMASKEDBLENDING ) + ( 64 * m_nBASETEXTURE2 ) + ( 128 * m_nBUMPMAP ) + ( 384 * m_nBUMPMAP2 ) + ( 768 * m_nCUBEMAP ) + ( 2304 * m_nENVMAPMASK ) + ( 4608 * m_nBASEALPHAENVMAPMASK ) + ( 9216 * m_nSELFILLUM ) + ( 18432 * m_nNORMALMAPALPHAENVMAPMASK ) + ( 36864 * m_nDIFFUSEBUMPMAP ) + ( 73728 * m_nBASETEXTURENOENVMAP ) + ( 147456 * m_nBASETEXTURE2NOENVMAP ) + ( 294912 * m_nWARPLIGHTING ) + ( 589824 * m_nFANCY_BLENDING ) + ( 1179648 * m_nPARALLAX_MAPPING ) + ( 1179648 * m_nSEAMLESS ) + ( 2359296 * m_nOUTLINE ) + ( 2359296 * m_nSOFTEDGES ) + ( 4718592 * m_nBUMPMASK ) + ( 9437184 * m_nDETAILTEXTURE ) + ( 18874368 * m_nDETAIL_BLEND_MODE ) + ( 245366784 * m_nSHADER_SRGB_READ ) + ( 245366784 * m_nLIGHTING_PREVIEW ) + ( 245366784 * m_nDOPIXELFOG ) + 0;
	}
};
#define shaderStaticTest_lightmappedgeneric_deferred_ps30 psh_forgot_to_set_static_MASKEDBLENDING + psh_forgot_to_set_static_BASETEXTURE2 + psh_forgot_to_set_static_BUMPMAP + psh_forgot_to_set_static_BUMPMAP2 + psh_forgot_to_set_static_CUBEMAP + psh_forgot_to_set_static_ENVMAPMASK + psh_forgot_to_set_static_BASEALPHAENVMAPMASK + psh_forgot_to_set_static_SELFILLUM + psh_forgot_to_set_static_NORMALMAPALPHAENVMAPMASK + psh_forgot_to_set_static_DIFFUSEBUMPMAP + psh_forgot_to_set_static_BASETEXTURENOENVMAP + psh_forgot_to_set_static_BASETEXTURE2NOENVMAP + psh_forgot_to_set_static_WARPLIGHTING + psh_forgot_to_set_static_FANCY_BLENDING + psh_forgot_to_set_static_PARALLAX_MAPPING + psh_forgot_to_set_static_SEAMLESS + psh_forgot_to_set_static_OUTLINE + psh_forgot_to_set_static_SOFTEDGES + psh_forgot_to_set_static_BUMPMASK + psh_forgot_to_set_static_DETAILTEXTURE + psh_forgot_to_set_static_DETAIL_BLEND_MODE + psh_forgot_to_set_static_SHADER_SRGB_READ + psh_forgot_to_set_static_LIGHTING_PREVIEW + 0
class lightmappedgeneric_deferred_ps30_Dynamic_Index
{
private:
	int m_nFASTPATHENVMAPCONTRAST;
#ifdef _DEBUG
	bool m_bFASTPATHENVMAPCONTRAST;
#endif
public:
	void SetFASTPATHENVMAPCONTRAST( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nFASTPATHENVMAPCONTRAST = i;
#ifdef _DEBUG
		m_bFASTPATHENVMAPCONTRAST = true;
#endif
	}
	void SetFASTPATHENVMAPCONTRAST( bool i )
	{
		m_nFASTPATHENVMAPCONTRAST = i ? 1 : 0;
#ifdef _DEBUG
		m_bFASTPATHENVMAPCONTRAST = true;
#endif
	}
private:
	int m_nFASTPATH;
#ifdef _DEBUG
	bool m_bFASTPATH;
#endif
public:
	void SetFASTPATH( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nFASTPATH = i;
#ifdef _DEBUG
		m_bFASTPATH = true;
#endif
	}
	void SetFASTPATH( bool i )
	{
		m_nFASTPATH = i ? 1 : 0;
#ifdef _DEBUG
		m_bFASTPATH = true;
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
	int m_nWRITE_DEPTH_TO_DESTALPHA;
#ifdef _DEBUG
	bool m_bWRITE_DEPTH_TO_DESTALPHA;
#endif
public:
	void SetWRITE_DEPTH_TO_DESTALPHA( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nWRITE_DEPTH_TO_DESTALPHA = i;
#ifdef _DEBUG
		m_bWRITE_DEPTH_TO_DESTALPHA = true;
#endif
	}
	void SetWRITE_DEPTH_TO_DESTALPHA( bool i )
	{
		m_nWRITE_DEPTH_TO_DESTALPHA = i ? 1 : 0;
#ifdef _DEBUG
		m_bWRITE_DEPTH_TO_DESTALPHA = true;
#endif
	}
private:
	int m_nFLASHLIGHTSHADOWS;
#ifdef _DEBUG
	bool m_bFLASHLIGHTSHADOWS;
#endif
public:
	void SetFLASHLIGHTSHADOWS( int i )
	{
		Assert( i >= 0 && i <= 0 );
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
	lightmappedgeneric_deferred_ps30_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
	{
#ifdef _DEBUG
		m_bFASTPATHENVMAPCONTRAST = false;
#endif // _DEBUG
		m_nFASTPATHENVMAPCONTRAST = 0;
#ifdef _DEBUG
		m_bFASTPATH = false;
#endif // _DEBUG
		m_nFASTPATH = 0;
#ifdef _DEBUG
		m_bWRITEWATERFOGTODESTALPHA = false;
#endif // _DEBUG
		m_nWRITEWATERFOGTODESTALPHA = 0;
#ifdef _DEBUG
		m_bWRITE_DEPTH_TO_DESTALPHA = false;
#endif // _DEBUG
		m_nWRITE_DEPTH_TO_DESTALPHA = 0;
#ifdef _DEBUG
		m_bFLASHLIGHTSHADOWS = false;
#endif // _DEBUG
		m_nFLASHLIGHTSHADOWS = 0;
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
		bool bAllDynamicVarsDefined = m_bFASTPATHENVMAPCONTRAST && m_bFASTPATH && m_bWRITEWATERFOGTODESTALPHA && m_bWRITE_DEPTH_TO_DESTALPHA && m_bFLASHLIGHTSHADOWS && m_bPIXELFOGTYPE;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nFASTPATHENVMAPCONTRAST ) + ( 2 * m_nFASTPATH ) + ( 4 * m_nWRITEWATERFOGTODESTALPHA ) + ( 8 * m_nWRITE_DEPTH_TO_DESTALPHA ) + ( 16 * m_nFLASHLIGHTSHADOWS ) + ( 16 * m_nPIXELFOGTYPE ) + 0;
	}
};
#define shaderDynamicTest_lightmappedgeneric_deferred_ps30 psh_forgot_to_set_dynamic_FASTPATHENVMAPCONTRAST + psh_forgot_to_set_dynamic_FASTPATH + psh_forgot_to_set_dynamic_WRITEWATERFOGTODESTALPHA + psh_forgot_to_set_dynamic_WRITE_DEPTH_TO_DESTALPHA + psh_forgot_to_set_dynamic_FLASHLIGHTSHADOWS + 0
