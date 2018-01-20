#ifndef CDEF_LIGHT_GLOBAL_H
#define CDEF_LIGHT_GLOBAL_H

struct lightData_Global_t;

class CDeferredLightGlobal : public CBaseEntity
{
	DECLARE_CLASS( CDeferredLightGlobal, CBaseEntity );
	DECLARE_NETWORKCLASS();
#ifdef GAME_DLL
	DECLARE_DATADESC();
#endif

public:

	CDeferredLightGlobal();
	~CDeferredLightGlobal();

#ifdef GAME_DLL
	virtual void Activate();

	virtual int UpdateTransmitState();
#else
	lightData_Global_t GetState();

#endif

	inline Vector GetColor_Diffuse();
	inline Vector GetColor_Ambient_High();
	inline Vector GetColor_Ambient_Low();

	inline float GetFadeTime();

	inline bool IsEnabled();
	inline bool HasShadow();
	inline bool ShouldFade();

private:

#ifdef GAME_DLL
	string_t m_str_Diff;
	string_t m_str_Ambient_High;
	string_t m_str_Ambient_Low;
#endif

	CNetworkVector( m_vecColor_Diff );
	CNetworkVector( m_vecColor_Ambient_High );
	CNetworkVector( m_vecColor_Ambient_Low );

	float m_flFadeTime;
	CNetworkVar( int, m_iDefFlags );
};

extern CDeferredLightGlobal *GetGlobalLight();

Vector CDeferredLightGlobal::GetColor_Diffuse()
{
	return m_vecColor_Diff;
}

Vector CDeferredLightGlobal::GetColor_Ambient_High()
{
	return m_vecColor_Ambient_High;
}

Vector CDeferredLightGlobal::GetColor_Ambient_Low()
{
	return m_vecColor_Ambient_Low;
}

float CDeferredLightGlobal::GetFadeTime()
{
	return m_flFadeTime;
}

bool CDeferredLightGlobal::IsEnabled()
{
	return ( m_iDefFlags & DEFLIGHTGLOBAL_ENABLED ) != 0;
}

bool CDeferredLightGlobal::HasShadow()
{
	return ( m_iDefFlags & DEFLIGHTGLOBAL_SHADOW_ENABLED ) != 0;
}

bool CDeferredLightGlobal::ShouldFade()
{
	return ( m_iDefFlags & DEFLIGHTGLOBAL_TRANSITION_FADE ) != 0;
}

#endif