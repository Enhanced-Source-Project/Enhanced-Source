// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// !$NUM_SHADOWED_COOKIE && !$NUM_SHADOWED
// $NUM_SHADOWED_COOKIE && $NUM_SHADOWED
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW
#include "shaderlib/cshader.h"
class volumpass_point_ps30_Static_Index
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
	volumpass_point_ps30_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
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
		return ( 2020 * m_nUSEWORLDTRANSFORM ) + 0;
	}
};
#define shaderStaticTest_volumpass_point_ps30 psh_forgot_to_set_static_USEWORLDTRANSFORM + 0
class volumpass_point_ps30_Dynamic_Index
{
private:
	int m_nNUM_SHADOWED_COOKIE;
#ifdef _DEBUG
	bool m_bNUM_SHADOWED_COOKIE;
#endif
public:
	void SetNUM_SHADOWED_COOKIE( int i )
	{
		Assert( i >= 0 && i <= 1 );
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
		Assert( i >= 0 && i <= 1 );
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
	int m_nVOLUME_LOD;
#ifdef _DEBUG
	bool m_bVOLUME_LOD;
#endif
public:
	void SetVOLUME_LOD( int i )
	{
		Assert( i >= 0 && i <= 4 );
		m_nVOLUME_LOD = i;
#ifdef _DEBUG
		m_bVOLUME_LOD = true;
#endif
	}
	void SetVOLUME_LOD( bool i )
	{
		m_nVOLUME_LOD = i ? 1 : 0;
#ifdef _DEBUG
		m_bVOLUME_LOD = true;
#endif
	}
private:
	int m_nVOLUME_SAMPLES;
#ifdef _DEBUG
	bool m_bVOLUME_SAMPLES;
#endif
public:
	void SetVOLUME_SAMPLES( int i )
	{
		Assert( i >= 0 && i <= 100 );
		m_nVOLUME_SAMPLES = i;
#ifdef _DEBUG
		m_bVOLUME_SAMPLES = true;
#endif
	}
	void SetVOLUME_SAMPLES( bool i )
	{
		m_nVOLUME_SAMPLES = i ? 1 : 0;
#ifdef _DEBUG
		m_bVOLUME_SAMPLES = true;
#endif
	}
public:
	// CONSTRUCTOR
	volumpass_point_ps30_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
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
		m_bVOLUME_LOD = false;
#endif // _DEBUG
		m_nVOLUME_LOD = 0;
#ifdef _DEBUG
		m_bVOLUME_SAMPLES = false;
#endif // _DEBUG
		m_nVOLUME_SAMPLES = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bNUM_SHADOWED_COOKIE && m_bNUM_SHADOWED && m_bVOLUME_LOD && m_bVOLUME_SAMPLES;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nNUM_SHADOWED_COOKIE ) + ( 2 * m_nNUM_SHADOWED ) + ( 4 * m_nVOLUME_LOD ) + ( 20 * m_nVOLUME_SAMPLES ) + 0;
	}
};
#define shaderDynamicTest_volumpass_point_ps30 psh_forgot_to_set_dynamic_NUM_SHADOWED_COOKIE + psh_forgot_to_set_dynamic_NUM_SHADOWED + psh_forgot_to_set_dynamic_VOLUME_LOD + psh_forgot_to_set_dynamic_VOLUME_SAMPLES + 0
