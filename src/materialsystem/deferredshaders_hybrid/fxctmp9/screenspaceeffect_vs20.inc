// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
#include "shaderlib/cshader.h"
class screenspaceeffect_vs20_Static_Index
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
	int m_nTRANSFORMVERTS;
#ifdef _DEBUG
	bool m_bTRANSFORMVERTS;
#endif
public:
	void SetTRANSFORMVERTS( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nTRANSFORMVERTS = i;
#ifdef _DEBUG
		m_bTRANSFORMVERTS = true;
#endif
	}
	void SetTRANSFORMVERTS( bool i )
	{
		m_nTRANSFORMVERTS = i ? 1 : 0;
#ifdef _DEBUG
		m_bTRANSFORMVERTS = true;
#endif
	}
public:
	// CONSTRUCTOR
	screenspaceeffect_vs20_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bVERTEXCOLOR = true;
#endif // _DEBUG
		m_nVERTEXCOLOR = 0;
#ifdef _DEBUG
		m_bTRANSFORMVERTS = true;
#endif // _DEBUG
		m_nTRANSFORMVERTS = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bVERTEXCOLOR && m_bTRANSFORMVERTS;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nVERTEXCOLOR ) + ( 2 * m_nTRANSFORMVERTS ) + 0;
	}
};
#define shaderStaticTest_screenspaceeffect_vs20 0
class screenspaceeffect_vs20_Dynamic_Index
{
public:
	// CONSTRUCTOR
	screenspaceeffect_vs20_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
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
#define shaderDynamicTest_screenspaceeffect_vs20 0
