// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// !$MODEL && $MORPHING_VTEX
// !$MODEL && $COMPRESSED_VERTS
// !$MODEL && $SKINNING
// !$MODEL && $MORPHING_VTEX
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
#include "shaderlib/cshader.h"
class defconstruct_vs30_Static_Index
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
private:
	int m_nSENDWORLDPOS;
#ifdef _DEBUG
	bool m_bSENDWORLDPOS;
#endif
public:
	void SetSENDWORLDPOS( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSENDWORLDPOS = i;
#ifdef _DEBUG
		m_bSENDWORLDPOS = true;
#endif
	}
	void SetSENDWORLDPOS( bool i )
	{
		m_nSENDWORLDPOS = i ? 1 : 0;
#ifdef _DEBUG
		m_bSENDWORLDPOS = true;
#endif
	}
public:
	// CONSTRUCTOR
	defconstruct_vs30_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
#ifdef _DEBUG
		m_bUSEWORLDTRANSFORM = false;
#endif // _DEBUG
		m_nUSEWORLDTRANSFORM = 0;
#ifdef _DEBUG
		m_bSENDWORLDPOS = false;
#endif // _DEBUG
		m_nSENDWORLDPOS = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bUSEWORLDTRANSFORM && m_bSENDWORLDPOS;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nUSEWORLDTRANSFORM ) + ( 2 * m_nSENDWORLDPOS ) + 0;
	}
};
#define shaderStaticTest_defconstruct_vs30 vsh_forgot_to_set_static_USEWORLDTRANSFORM + vsh_forgot_to_set_static_SENDWORLDPOS + 0
class defconstruct_vs30_Dynamic_Index
{
public:
	// CONSTRUCTOR
	defconstruct_vs30_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
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
#define shaderDynamicTest_defconstruct_vs30 0
