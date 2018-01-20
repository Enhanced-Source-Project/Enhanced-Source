#ifndef DEFPASS_SHADOW_H
#define DEFPASS_SHADOW_H

class CDeferredPerMaterialContextData;

struct defParms_shadow
{
	defParms_shadow()
	{
		Q_memset( this, 0xFF, sizeof( defParms_shadow ) );

		bModel = false;
	};

	// textures
	int iAlbedo;
	int iAlbedo2;
	int iAlbedo3;
	int iAlbedo4;

	// control
	int iAlphatestRef;

	// blending
	int iMultiblend;
	int nSpecTexture;
	int nSpecTexture2;
	int nSpecTexture3;
	int nSpecTexture4;

	int nRotation;
	int nRotation2;
	int nRotation3;
	int nRotation4;
	int nScale;
	int nScale2;
	int nScale3;
	int nScale4;

	// Tree Sway!
	int m_nTreeSway;
	int m_nTreeSwayHeight;
	int m_nTreeSwayStartHeight;
	int m_nTreeSwayRadius;
	int m_nTreeSwayStartRadius;
	int m_nTreeSwaySpeed;
	int m_nTreeSwaySpeedHighWindMultiplier;
	int m_nTreeSwayStrength;
	int m_nTreeSwayScrumbleSpeed;
	int m_nTreeSwayScrumbleStrength;
	int m_nTreeSwayScrumbleFrequency;
	int m_nTreeSwayFalloffExp;
	int m_nTreeSwayScrumbleFalloffExp;
	int m_nTreeSwaySpeedLerpStart;
	int m_nTreeSwaySpeedLerpEnd;

	// config
	bool bModel;
};


void InitParmsShadowPass( const defParms_shadow &info, CBaseVSShader *pShader, IMaterialVar **params );
void InitPassShadowPass( const defParms_shadow &info, CBaseVSShader *pShader, IMaterialVar **params );
void DrawPassShadowPass( const defParms_shadow &info, CBaseVSShader *pShader, IMaterialVar **params,
	IShaderShadow* pShaderShadow, IShaderDynamicAPI* pShaderAPI,
	VertexCompressionType_t vertexCompression, CDeferredPerMaterialContextData *pDeferredContext );


#endif