#ifndef LIGHTING_PASS_BASIC_H
#define LIGHTING_PASS_BASIC_H


struct lightPassParms
{
	lightPassParms()
	{
		Q_memset( this, 0xFF, sizeof( lightPassParms ) );
	};

	int iLightTypeVar;
	int iWorldProjection;
};


void InitParmsLightPass( const lightPassParms &info, CBaseVSShader *pShader, IMaterialVar **params );
void InitPassLightPass( const lightPassParms &info, CBaseVSShader *pShader, IMaterialVar **params );
void DrawPassLightPass( const lightPassParms &info, CBaseVSShader *pShader, IMaterialVar **params,
	IShaderShadow* pShaderShadow, IShaderDynamicAPI* pShaderAPI,
	VertexCompressionType_t vertexCompression );


#endif