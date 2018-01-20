#ifndef ENV_PROJECTEDTEXTURE_H
#define ENV_PROJECTEDTEXTURE_H
#ifdef _WIN32
#pragma once
#endif

#define ENV_PROJECTEDTEXTURE_STARTON			(1<<0)
#define ENV_PROJECTEDTEXTURE_ALWAYSUPDATE		(1<<1)

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CEnvProjectedTexture : public CPointEntity
{
	DECLARE_CLASS( CEnvProjectedTexture, CPointEntity );
public:
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();

	CEnvProjectedTexture();
	bool KeyValue( const char *szKeyName, const char *szValue );
	virtual bool GetKeyValue( const char *szKeyName, char *szValue, int iMaxLen );

	// Always transmit to clients
	virtual int UpdateTransmitState();
	virtual void Activate( void );

	void InputTurnOn( inputdata_t &inputdata );
	void InputTurnOff( inputdata_t &inputdata );
	void InputAlwaysUpdateOn( inputdata_t &inputdata );
	void InputAlwaysUpdateOff( inputdata_t &inputdata );
	void InputSetFOV( inputdata_t &inputdata );
	void InputSetTarget( inputdata_t &inputdata );
	void InputSetCameraSpace( inputdata_t &inputdata );
	void InputSetLightOnlyTarget( inputdata_t &inputdata );
	void InputSetLightWorld( inputdata_t &inputdata );
	void InputSetEnableShadows( inputdata_t &inputdata );
	void InputSetLightColor( inputdata_t &inputdata );
	void InputSetSpotlightTexture( inputdata_t &inputdata );
	void InputSetAmbient( inputdata_t &inputdata );
	void InputSetNearZ( inputdata_t &inputdata );
	void InputSetFarZ( inputdata_t &inputdata );
	void InputSetBrightnessScale( inputdata_t &inputdata );

	void InitialThink( void );

	CNetworkHandle( CBaseEntity, m_hTargetEntity );
	CNetworkVar( bool, m_bState );
	CNetworkVar( bool, m_bAlwaysUpdate );
	CNetworkVar( float, m_flLightFOV );
	CNetworkVar( bool, m_bEnableShadows );
	CNetworkVar( bool, m_bSimpleProjection );
	CNetworkVar( bool, m_bLightOnlyTarget );
	CNetworkVar( bool, m_bLightWorld );
	CNetworkVar( bool, m_bCameraSpace );
	CNetworkVar( float, m_flBrightnessScale );
	CNetworkColor32( m_LightColor );
	CNetworkVar( float, m_flColorTransitionTime );
	CNetworkVar( float, m_flAmbient );
	CNetworkString( m_SpotlightTextureName, MAX_PATH );
	CNetworkVar( int, m_nSpotlightTextureFrame );
	CNetworkVar( float, m_flNearZ );
	CNetworkVar( float, m_flFarZ );
	CNetworkVar( int, m_nShadowQuality );
	CNetworkVar( float, m_flProjectionSize );
	CNetworkVar( float, m_flRotation );

	CNetworkVar( float, m_flAttenConst );
	CNetworkVar( float, m_flAttenLinear );
	CNetworkVar( float, m_flAttenQuadratic );
	CNetworkVar( float, m_flAttenFarZ );

	CNetworkVar( bool, m_bUberlight );
	CNetworkVar( float, m_fNearEdge );
	CNetworkVar( float, m_fFarEdge );
	CNetworkVar( float, m_fCutOn );
	CNetworkVar( float, m_fCutOff );
	CNetworkVar( float, m_fShearx );
	CNetworkVar( float, m_fSheary );
	CNetworkVar( float, m_fWidth );
	CNetworkVar( float, m_fWedge );
	CNetworkVar( float, m_fHeight );
	CNetworkVar( float, m_fHedge );
	CNetworkVar( float, m_fRoundness );

	CNetworkVar( bool, m_bVolumetric );
	CNetworkVar( float, m_flNoiseStrength );
	CNetworkVar( int, m_nNumPlanes );
	CNetworkVar( float, m_flPlaneOffset );
	CNetworkVar( float, m_flVolumetricIntensity );
};


#endif	// ENV_PROJECTEDTEXTURE_H