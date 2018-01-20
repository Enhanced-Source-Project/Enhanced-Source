// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// !$NUM_SHADOWED_COOKIE && !$NUM_SHADOWED && !$NUM_COOKIE && !$NUM_SIMPLE
// $USEWORLDTRANSFORM && $NUM_SHADOWED_COOKIE && $NUM_SHADOWED
// $USEWORLDTRANSFORM && $NUM_SHADOWED_COOKIE && $NUM_COOKIE
// $USEWORLDTRANSFORM && $NUM_SHADOWED_COOKIE && $NUM_SIMPLE
// $USEWORLDTRANSFORM && $NUM_SHADOWED && $NUM_COOKIE
// $USEWORLDTRANSFORM && $NUM_SHADOWED && $NUM_SIMPLE
// $USEWORLDTRANSFORM && $NUM_COOKIE && $NUM_SIMPLE
// $USEWORLDTRANSFORM && $NUM_SHADOWED_COOKIE > 1
// $USEWORLDTRANSFORM && $NUM_SHADOWED > 1
// $USEWORLDTRANSFORM && $NUM_COOKIE > 1
// $USEWORLDTRANSFORM && $NUM_SIMPLE > 1
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
class lightingpass_spot_ps30_Static_Index
{
private:
	int m_nUSEWORLDTRANSFORM;
#ifdef _DEBUG
	bool m_bUSEWORLDTRANSFORM;
#endif
public:
	void SetUSEWORLDTRANSFORM( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nUSEWORLDTRANSFORM = i;
#ifdef _DEBUG
		m_bUSEWORLDTRANSFORM = true;
#endif
	}
	void SetUSEWORLDTRANSFORM( bool i )
	{
		m_nUSEWORLDTRANSFORM = i ? 1 : 0;
#ifdef _DEBUG
		m_bUSEWORLDTRANSFORM = true;
#endif
	}
public:
	// CONSTRUCTOR
	lightingpass_spot_ps30_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bUSEWORLDTRANSFORM = false;
#endif // _DEBUG
		m_nUSEWORLDTRANSFORM = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bUSEWORLDTRANSFORM;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 528 * m_nUSEWORLDTRANSFORM ) + 0;
	}
};
#define shaderStaticTest_lightingpass_spot_ps30 psh_forgot_to_set_static_USEWORLDTRANSFORM + 0
class lightingpass_spot_ps30_Dynamic_Index
{
private:
	int m_nNUM_SHADOWED_COOKIE;
#ifdef _DEBUG
	bool m_bNUM_SHADOWED_COOKIE;
#endif
public:
	void SetNUM_SHADOWED_COOKIE( int i )
	{
		Assert( i >= 0 && i <= 2 );
		m_nNUM_SHADOWED_COOKIE = i;
#ifdef _DEBUG
		m_bNUM_SHADOWED_COOKIE = true;
#endif
	}
	void SetNUM_SHADOWED_COOKIE( bool i )
	{
		m_nNUM_SHADOWED_COOKIE = i ? 1 : 0;
#ifdef _DEBUG
		m_bNUM_SHADOWED_COOKIE = true;
#endif
	}
private:
	int m_nNUM_SHADOWED;
#ifdef _DEBUG
	bool m_bNUM_SHADOWED;
#endif
public:
	void SetNUM_SHADOWED( int i )
	{
		Assert( i >= 0 && i <= 3 );
		m_nNUM_SHADOWED = i;
#ifdef _DEBUG
		m_bNUM_SHADOWED = true;
#endif
	}
	void SetNUM_SHADOWED( bool i )
	{
		m_nNUM_SHADOWED = i ? 1 : 0;
#ifdef _DEBUG
		m_bNUM_SHADOWED = true;
#endif
	}
private:
	int m_nNUM_COOKIE;
#ifdef _DEBUG
	bool m_bNUM_COOKIE;
#endif
public:
	void SetNUM_COOKIE( int i )
	{
		Assert( i >= 0 && i <= 3 );
		m_nNUM_COOKIE = i;
#ifdef _DEBUG
		m_bNUM_COOKIE = true;
#endif
	}
	void SetNUM_COOKIE( bool i )
	{
		m_nNUM_COOKIE = i ? 1 : 0;
#ifdef _DEBUG
		m_bNUM_COOKIE = true;
#endif
	}
private:
	int m_nNUM_SIMPLE;
#ifdef _DEBUG
	bool m_bNUM_SIMPLE;
#endif
public:
	void SetNUM_SIMPLE( int i )
	{
		Assert( i >= 0 && i <= 10 );
		m_nNUM_SIMPLE = i;
#ifdef _DEBUG
		m_bNUM_SIMPLE = true;
#endif
	}
	void SetNUM_SIMPLE( bool i )
	{
		m_nNUM_SIMPLE = i ? 1 : 0;
#ifdef _DEBUG
		m_bNUM_SIMPLE = true;
#endif
	}
public:
	// CONSTRUCTOR
	lightingpass_spot_ps30_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
	{
#ifdef _DEBUG
		m_bNUM_SHADOWED_COOKIE = false;
#endif // _DEBUG
		m_nNUM_SHADOWED_COOKIE = 0;
#ifdef _DEBUG
		m_bNUM_SHADOWED = false;
#endif // _DEBUG
		m_nNUM_SHADOWED = 0;
#ifdef _DEBUG
		m_bNUM_COOKIE = false;
#endif // _DEBUG
		m_nNUM_COOKIE = 0;
#ifdef _DEBUG
		m_bNUM_SIMPLE = false;
#endif // _DEBUG
		m_nNUM_SIMPLE = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bNUM_SHADOWED_COOKIE && m_bNUM_SHADOWED && m_bNUM_COOKIE && m_bNUM_SIMPLE;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nNUM_SHADOWED_COOKIE ) + ( 3 * m_nNUM_SHADOWED ) + ( 12 * m_nNUM_COOKIE ) + ( 48 * m_nNUM_SIMPLE ) + 0;
	}
};
#define shaderDynamicTest_lightingpass_spot_ps30 psh_forgot_to_set_dynamic_NUM_SHADOWED_COOKIE + psh_forgot_to_set_dynamic_NUM_SHADOWED + psh_forgot_to_set_dynamic_NUM_COOKIE + psh_forgot_to_set_dynamic_NUM_SIMPLE + 0
