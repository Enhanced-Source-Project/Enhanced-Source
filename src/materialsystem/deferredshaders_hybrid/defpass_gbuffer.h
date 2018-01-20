#ifndef DEFPASS_GBUFFER_H
#define DEFPASS_GBUFFER_H

class CDeferredPerMaterialContextData;

struct defParms_gBuffer
{
	defParms_gBuffer()
	{
		Q_memset( this, 0xFF, sizeof( defParms_gBuffer ) );

		bModel = false;
	};

	// textures
	int iAlbedo;
	int iAlbedo2;
	int iAlbedo3;
	int iAlbedo4;

	int iBumpmap;
	int iBumpmap2;
	int iBumpmap3;
	int iBumpmap4;
	int iPhongmap;

	int iBlendmodulate;
#if 0
	int iBlendmodulate2;
	int iBlendmodulate3;
#endif // 0

	// control
	int iAlphatestRef;
	int iLitface;
	int iPhongExp;
	int iPhongExp2;
	int iSSBump;

	// blending
	int iBlendmodulateTransform;
#if 0
	int iBlendmodulateTransform2;
	int iBlendmodulateTransform3;
#endif // 0

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


void InitParmsGBuffer( const defParms_gBuffer &info, CBaseVSShader *pShader, IMaterialVar **params );
void InitPassGBuffer( const defParms_gBuffer &info, CBaseVSShader *pShader, IMaterialVar **params );
void DrawPassGBuffer( const defParms_gBuffer &info, CBaseVSShader *pShader, IMaterialVar **params,
	IShaderShadow* pShaderShadow, IShaderDynamicAPI* pShaderAPI,
	VertexCompressionType_t vertexCompression, CDeferredPerMaterialContextData *pDeferredContext );


#endif