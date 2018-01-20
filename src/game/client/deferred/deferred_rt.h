#ifndef DEFERRED_RT_H
#define DEFERRED_RT_H

class ITexture;

float GetDepthMapDepthResolution( float zDelta );
void DefRTsOnModeChanged();
void InitDeferredRTs( bool bInitial = false );

ITexture *GetDefRT_Normals();
ITexture *GetDefRT_Depth();
ITexture *GetDefRT_Albedo();
ITexture *GetDefRT_Specular();
ITexture *GetDefRT_LightCtrl();
ITexture *GetDefRT_Lightaccum();
ITexture *GetDefRT_Lightaccum2();

ITexture *GetDefRT_VolumePrepass();
ITexture *GetDefRT_VolumetricsBuffer( int index );

ITexture *GetDefRT_RadiosityBuffer( int index );
ITexture *GetDefRT_RadiosityNormal( int index );

int GetShadowResolution_Spot();
int GetShadowResolution_Point();

#if DEFCFG_ADAPTIVE_SHADOWMAP_LOD
int GetShadowResolution_Spot_LOD1();
int GetShadowResolution_Spot_LOD2();


int GetShadowResolution_Point_LOD1();
int GetShadowResolution_Point_LOD2();
#endif

#if DEFCFG_ADAPTIVE_SHADOWMAP_LOD
int GetShadowResolution_Point_LOD1()
{
	return deferred_rt_shadowpoint_lod1_res.GetInt();
}

int GetShadowResolution_Point_LOD2()
{
	return deferred_rt_shadowpoint_lod2_res.GetInt();
}
#endif

ITexture *GetShadowColorRT_Ortho( int index );
ITexture *GetShadowDepthRT_Ortho( int index );

ITexture *GetShadowColorRT_Proj( int index );
ITexture *GetShadowDepthRT_Proj( int index );

ITexture *GetShadowColorRT_DP( int index );
ITexture *GetShadowDepthRT_DP( int index );

ITexture *GetProjectableVguiRT( int index );

ITexture *GetRadiosityAlbedoRT_Ortho( int index );
ITexture *GetRadiosityNormalRT_Ortho( int index );

#if CSM_USE_COMPOSITED_TARGET
int GetCMSCompResX();
int GetCMSCompResY();
bool IsCMSCompResLow();
#endif // CSM_USE_COMPOSITED_TARGET

#endif