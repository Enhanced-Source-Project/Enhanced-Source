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
class radiosity_gen_global_ps30_Static_Index
{
public:
	// CONSTRUCTOR
	radiosity_gen_global_ps30_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
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
#define shaderStaticTest_radiosity_gen_global_ps30 0
class radiosity_gen_global_ps30_Dynamic_Index
{
private:
	int m_nCASCADE;
#ifdef _DEBUG
	bool m_bCASCADE;
#endif
public:
	void SetCASCADE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nCASCADE = i;
#ifdef _DEBUG
		m_bCASCADE = true;
#endif
	}
	void SetCASCADE( bool i )
	{
		m_nCASCADE = i ? 1 : 0;
#ifdef _DEBUG
		m_bCASCADE = true;
#endif
	}
public:
	// CONSTRUCTOR
	radiosity_gen_global_ps30_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
	{
#ifdef _DEBUG
		m_bCASCADE = false;
#endif // _DEBUG
		m_nCASCADE = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bCASCADE;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nCASCADE ) + 0;
	}
};
#define shaderDynamicTest_radiosity_gen_global_ps30 psh_forgot_to_set_dynamic_CASCADE + 0
