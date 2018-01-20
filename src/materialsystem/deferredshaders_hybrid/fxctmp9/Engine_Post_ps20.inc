// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// ( $CONVERT_FROM_LINEAR == 0 ) && ( $CONVERT_TO_LINEAR == 1 )
// ( $TOOL_MODE == 0 ) && ( $CONVERT_TO_LINEAR == 1 )
// ( $LOCAL_CONTRAST_ENABLE == 0 ) && ( $BLURRED_VIGNETTE_ENABLE == 1 )
// ( $TOOL_MODE == 0 ) && $TV_GAMMA
// ( $TOOL_MODE == 0 ) && $DESATURATEENABLE
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW
#include "shaderlib/cshader.h"
class engine_post_ps20_Static_Index
{
private:
	int m_nTOOL_MODE;
#ifdef _DEBUG
	bool m_bTOOL_MODE;
#endif
public:
	void SetTOOL_MODE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nTOOL_MODE = i;
#ifdef _DEBUG
		m_bTOOL_MODE = true;
#endif
	}
	void SetTOOL_MODE( bool i )
	{
		m_nTOOL_MODE = i ? 1 : 0;
#ifdef _DEBUG
		m_bTOOL_MODE = true;
#endif
	}
private:
	int m_nDEPTH_BLUR_ENABLE;
#ifdef _DEBUG
	bool m_bDEPTH_BLUR_ENABLE;
#endif
public:
	void SetDEPTH_BLUR_ENABLE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDEPTH_BLUR_ENABLE = i;
#ifdef _DEBUG
		m_bDEPTH_BLUR_ENABLE = true;
#endif
	}
	void SetDEPTH_BLUR_ENABLE( bool i )
	{
		m_nDEPTH_BLUR_ENABLE = i ? 1 : 0;
#ifdef _DEBUG
		m_bDEPTH_BLUR_ENABLE = true;
#endif
	}
public:
	// CONSTRUCTOR
	engine_post_ps20_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bTOOL_MODE = false;
#endif // _DEBUG
		m_nTOOL_MODE = 0;
#ifdef _DEBUG
		m_bDEPTH_BLUR_ENABLE = false;
#endif // _DEBUG
		m_nDEPTH_BLUR_ENABLE = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bTOOL_MODE && m_bDEPTH_BLUR_ENABLE;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 8 * m_nTOOL_MODE ) + ( 16 * m_nDEPTH_BLUR_ENABLE ) + 0;
	}
};
#define shaderStaticTest_engine_post_ps20 psh_forgot_to_set_static_TOOL_MODE + psh_forgot_to_set_static_DEPTH_BLUR_ENABLE + 0
class engine_post_ps20_Dynamic_Index
{
private:
	int m_nAA_ENABLE;
#ifdef _DEBUG
	bool m_bAA_ENABLE;
#endif
public:
	void SetAA_ENABLE( int i )
	{
		Assert( i >= 0 && i <= 0 );
		m_nAA_ENABLE = i;
#ifdef _DEBUG
		m_bAA_ENABLE = true;
#endif
	}
	void SetAA_ENABLE( bool i )
	{
		m_nAA_ENABLE = i ? 1 : 0;
#ifdef _DEBUG
		m_bAA_ENABLE = true;
#endif
	}
private:
	int m_nCOL_CORRECT_NUM_LOOKUPS;
#ifdef _DEBUG
	bool m_bCOL_CORRECT_NUM_LOOKUPS;
#endif
public:
	void SetCOL_CORRECT_NUM_LOOKUPS( int i )
	{
		Assert( i >= 0 && i <= 3 );
		m_nCOL_CORRECT_NUM_LOOKUPS = i;
#ifdef _DEBUG
		m_bCOL_CORRECT_NUM_LOOKUPS = true;
#endif
	}
	void SetCOL_CORRECT_NUM_LOOKUPS( bool i )
	{
		m_nCOL_CORRECT_NUM_LOOKUPS = i ? 1 : 0;
#ifdef _DEBUG
		m_bCOL_CORRECT_NUM_LOOKUPS = true;
#endif
	}
private:
	int m_nVOMIT_ENABLE;
#ifdef _DEBUG
	bool m_bVOMIT_ENABLE;
#endif
public:
	void SetVOMIT_ENABLE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nVOMIT_ENABLE = i;
#ifdef _DEBUG
		m_bVOMIT_ENABLE = true;
#endif
	}
	void SetVOMIT_ENABLE( bool i )
	{
		m_nVOMIT_ENABLE = i ? 1 : 0;
#ifdef _DEBUG
		m_bVOMIT_ENABLE = true;
#endif
	}
public:
	// CONSTRUCTOR
	engine_post_ps20_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
	{
#ifdef _DEBUG
		m_bAA_ENABLE = false;
#endif // _DEBUG
		m_nAA_ENABLE = 0;
#ifdef _DEBUG
		m_bCOL_CORRECT_NUM_LOOKUPS = false;
#endif // _DEBUG
		m_nCOL_CORRECT_NUM_LOOKUPS = 0;
#ifdef _DEBUG
		m_bVOMIT_ENABLE = false;
#endif // _DEBUG
		m_nVOMIT_ENABLE = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bAA_ENABLE && m_bCOL_CORRECT_NUM_LOOKUPS && m_bVOMIT_ENABLE;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nAA_ENABLE ) + ( 1 * m_nCOL_CORRECT_NUM_LOOKUPS ) + ( 4 * m_nVOMIT_ENABLE ) + 0;
	}
};
#define shaderDynamicTest_engine_post_ps20 psh_forgot_to_set_dynamic_AA_ENABLE + psh_forgot_to_set_dynamic_COL_CORRECT_NUM_LOOKUPS + psh_forgot_to_set_dynamic_VOMIT_ENABLE + 0
