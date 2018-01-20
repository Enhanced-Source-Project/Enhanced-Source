#ifndef LIGHTING_PASS_VOLUM_H
#define LIGHTING_PASS_VOLUM_H


void InitParmsLightPassVolum( const lightPassParms &info, CBaseVSShader *pShader, IMaterialVar **params );
void InitPassLightPassVolum( const lightPassParms &info, CBaseVSShader *pShader, IMaterialVar **params );
void DrawPassLightPassVolum( const lightPassParms &info, CBaseVSShader *pShader, IMaterialVar **params,
	IShaderShadow* pShaderShadow, IShaderDynamicAPI* pShaderAPI,
	VertexCompressionType_t vertexCompression );


#endif