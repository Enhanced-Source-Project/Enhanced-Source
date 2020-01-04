//===== Copyright 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Responsible for drawing the scene
//
//===========================================================================//

#include "cbase.h"
#include "view.h"
#include "iviewrender.h"
#include "view_shared.h"
#include "ivieweffects.h"
#include "iinput.h"
#include "model_types.h"
#include "clientsideeffects.h"
#include "particlemgr.h"
#include "viewrender.h"
#include "iclientmode.h"
#include "voice_status.h"
#include "glow_overlay.h"
#include "materialsystem/imesh.h"
#include "materialsystem/ITexture.h"
#include "materialsystem/IMaterial.h"
#include "materialsystem/IMaterialVar.h"
#include "materialsystem/imaterialsystem.h"
#include "DetailObjectSystem.h"
#include "tier0/vprof.h"
#include "tier1/mempool.h"
#include "vstdlib/jobthread.h"
#include "datacache/imdlcache.h"
#include "engine/IEngineTrace.h"
#include "engine/ivmodelinfo.h"
#include "tier0/icommandline.h"
#include "view_scene.h"
#include "particles_ez.h"
#include "engine/IStaticPropMgr.h"
#include "engine/ivdebugoverlay.h"
#include "c_pixel_visibility.h"
#include "precache_register.h"
#include "c_rope.h"
#include "c_effects.h"
#include "smoke_fog_overlay.h"
#include "materialsystem/imaterialsystemhardwareconfig.h"
#include "vgui_int.h"
#include "ienginevgui.h"
#include "ScreenSpaceEffects.h"
#include "toolframework_client.h"
#include "c_func_reflective_glass.h"
#include "keyvalues.h"
#include "renderparm.h"
#include "modelrendersystem.h"
#include "vgui/ISurface.h"
#include "tier1/callqueue.h"


#include "rendertexture.h"
#include "viewpostprocess.h"
#include "viewdebug.h"

#ifdef SHADEREDITOR
#include "shadereditor/shadereditorsystem.h"
#endif
#include "deferred/deferred_shared_common.h"


// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern ConVar r_visocclusion;
extern ConVar vcollide_wireframe;
extern ConVar mat_motion_blur_enabled;
extern ConVar r_depthoverlay;

//-----------------------------------------------------------------------------
// Convars related to controlling rendering
//-----------------------------------------------------------------------------
extern ConVar cl_maxrenderable_dist;

extern ConVar r_entityclips; //FIXME: Nvidia drivers before 81.94 on cards that support user clip planes will have problems with this, require driver update? Detect and disable?

// Matches the version in the engine
extern ConVar r_drawopaqueworld;
extern ConVar r_drawtranslucentworld;
extern ConVar r_3dsky;
extern ConVar r_skybox;
extern ConVar r_drawviewmodel;
extern ConVar r_drawtranslucentrenderables;
extern ConVar r_drawopaquerenderables;

extern ConVar r_flashlightdepth_drawtranslucents;

// FIXME: This is not static because we needed to turn it off for TF2 playtests
extern ConVar r_DrawDetailProps;

extern ConVar r_worldlistcache;

//-----------------------------------------------------------------------------
// Convars related to fog color
//-----------------------------------------------------------------------------
extern void GetFogColor( fogparams_t *pFogParams, float *pColor, bool ignoreOverride = false, bool ignoreHDRColorScale = false );
extern float GetFogMaxDensity( fogparams_t *pFogParams, bool ignoreOverride = false );
extern bool GetFogEnable( fogparams_t *pFogParams, bool ignoreOverride = false );
extern float GetFogStart( fogparams_t *pFogParams, bool ignoreOverride = false );
extern float GetFogEnd( fogparams_t *pFogParams, bool ignoreOverride = false );
extern float GetSkyboxFogStart( bool ignoreOverride = false );
extern float GetSkyboxFogEnd( bool ignoreOverride = false );
extern float GetSkyboxFogMaxDensity( bool ignoreOverride = false );
extern void GetSkyboxFogColor( float *pColor, bool ignoreOverride = false, bool ignoreHDRColorScale = false );
// set any of these to use the maps fog
extern ConVar fog_enableskybox;

extern void PositionHudPanels( CUtlVector< vgui::VPANEL > &list, const CViewSetup &view );

//-----------------------------------------------------------------------------
// Water-related convars
//-----------------------------------------------------------------------------
extern ConVar r_debugcheapwater;
#ifndef _X360
extern ConVar r_waterforceexpensive;
#endif
extern ConVar r_waterforcereflectentities;
extern ConVar r_WaterDrawRefraction;
extern ConVar r_WaterDrawReflection;
extern ConVar r_ForceWaterLeaf;
extern ConVar mat_drawwater;
extern ConVar mat_clipz;


//-----------------------------------------------------------------------------
// Other convars
//-----------------------------------------------------------------------------
extern ConVar r_eyewaterepsilon;

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
extern int g_CurrentViewID;
extern bool g_bRenderingScreenshot;

//static FrustumCache_t s_FrustumCache;
extern FrustumCache_t *FrustumCache( void );


//-----------------------------------------------------------------------------
// Describes a pruned set of leaves to be rendered this view. Reference counted
// because potentially shared by a number of views
//-----------------------------------------------------------------------------

extern void FlushWorldLists();

void DrawCube( CMeshBuilder &meshBuilder, Vector vecPos, float flRadius, float *pfl2Texcoords )
{
	const float flOffsets[24][3] = {
		-flRadius, -flRadius, -flRadius,
		flRadius, -flRadius, -flRadius,
		flRadius, flRadius, -flRadius,
		-flRadius, flRadius, -flRadius,

		flRadius, flRadius, flRadius,
		flRadius, flRadius, -flRadius,
		flRadius, -flRadius, -flRadius,
		flRadius, -flRadius, flRadius,

		-flRadius, flRadius, flRadius,
		-flRadius, -flRadius, flRadius,
		-flRadius, -flRadius, -flRadius,
		-flRadius, flRadius, -flRadius,

		flRadius, flRadius, flRadius,
		-flRadius, flRadius, flRadius,
		-flRadius, flRadius, -flRadius,
		flRadius, flRadius, -flRadius,

		flRadius, -flRadius, flRadius,
		flRadius, -flRadius, -flRadius,
		-flRadius, -flRadius, -flRadius,
		-flRadius, -flRadius, flRadius,

		flRadius, flRadius, flRadius,
		flRadius, -flRadius, flRadius,
		-flRadius, -flRadius, flRadius,
		-flRadius, flRadius, flRadius,
	};

	for ( int i = 0; i < 24; i++ )
	{
		meshBuilder.Position3f( vecPos.x + flOffsets[i][0],
			vecPos.y + flOffsets[i][1],
			vecPos.z + flOffsets[i][2] );
		meshBuilder.TexCoord2fv( 0, pfl2Texcoords );
		meshBuilder.AdvanceVertex();
	}
}

IMesh *CDeferredViewRender::GetRadiosityScreenGrid( const int iCascade )
{
	if ( m_pMesh_RadiosityScreenGrid[iCascade] == NULL )
	{
		Assert( m_pMesh_RadiosityScreenGrid[iCascade] == NULL );

		const bool bFar = iCascade == 1;

		m_pMesh_RadiosityScreenGrid[iCascade] = CreateRadiosityScreenGrid(
			Vector2D( 0, (bFar?0.5f:0) ),
			bFar ? RADIOSITY_BUFFER_GRID_STEP_SIZE_FAR : RADIOSITY_BUFFER_GRID_STEP_SIZE_CLOSE );
	}

	Assert( m_pMesh_RadiosityScreenGrid[iCascade] != NULL );

	return m_pMesh_RadiosityScreenGrid[iCascade];
}

IMesh *CDeferredViewRender::CreateRadiosityScreenGrid( const Vector2D &vecViewportBase,
	const float flWorldStepSize )
{
	VertexFormat_t format = VERTEX_POSITION
		| VERTEX_TEXCOORD_SIZE( 0, 4 )
		| VERTEX_TEXCOORD_SIZE( 1, 4 )
		| VERTEX_TANGENT_S;

	const float flTexelGridMargin = 1.5f / RADIOSITY_BUFFER_SAMPLES_XY;
	const float flTexelHalf[2] = { 0.5f / RADIOSITY_BUFFER_VIEWPORT_SX,
		0.5f / RADIOSITY_BUFFER_VIEWPORT_SY };

	const float flLocalCoordSingle = 1.0f / RADIOSITY_BUFFER_GRIDS_PER_AXIS;
	const float flLocalCoords[4][2] = {
		0, 0,
		flLocalCoordSingle, 0,
		flLocalCoordSingle, flLocalCoordSingle,
		0, flLocalCoordSingle,
	};

	CMatRenderContextPtr pRenderContext( materials );
	IMesh *pRet = pRenderContext->CreateStaticMesh(
		format, TEXTURE_GROUP_OTHER );
	
	CMeshBuilder meshBuilder;
	meshBuilder.Begin( pRet, MATERIAL_QUADS,
		RADIOSITY_BUFFER_GRIDS_PER_AXIS * RADIOSITY_BUFFER_GRIDS_PER_AXIS );

	float flGridOrigins[RADIOSITY_BUFFER_SAMPLES_Z][2];
	for ( int i = 0; i < RADIOSITY_BUFFER_SAMPLES_Z; i++ )
	{
		int x = i % RADIOSITY_BUFFER_GRIDS_PER_AXIS;
		int y = i / RADIOSITY_BUFFER_GRIDS_PER_AXIS;

		flGridOrigins[i][0] = x * flLocalCoordSingle + flTexelHalf[0];
		flGridOrigins[i][1] = y * flLocalCoordSingle + flTexelHalf[1];
	}

	const float flGridSize = flWorldStepSize * RADIOSITY_BUFFER_SAMPLES_XY;
	const float flLocalGridSize[4][2] = {
		0, 0,
		flGridSize, 0,
		flGridSize, flGridSize,
		0, flGridSize,
	};
	const float flLocalGridLimits[4][2] = {
		-flTexelGridMargin, -flTexelGridMargin,
		1 + flTexelGridMargin, -flTexelGridMargin,
		1 + flTexelGridMargin, 1 + flTexelGridMargin,
		-flTexelGridMargin, 1 + flTexelGridMargin,
	};

	for ( int x = 0; x < RADIOSITY_BUFFER_GRIDS_PER_AXIS; x++ )
	{
		for ( int y = 0; y < RADIOSITY_BUFFER_GRIDS_PER_AXIS; y++ )
		{
			const int iIndexLocal = x + y * RADIOSITY_BUFFER_GRIDS_PER_AXIS;
			const int iIndicesOne[2] = { MIN( RADIOSITY_BUFFER_SAMPLES_Z - 1, iIndexLocal + 1 ), MAX( 0, iIndexLocal - 1 ) };

			for ( int q = 0; q < 4; q++ )
			{
				meshBuilder.Position3f(
					(x * flLocalCoordSingle + flLocalCoords[q][0]) * 2 - flLocalCoordSingle * RADIOSITY_BUFFER_GRIDS_PER_AXIS,
					flLocalCoordSingle * RADIOSITY_BUFFER_GRIDS_PER_AXIS - (y * flLocalCoordSingle + flLocalCoords[q][1]) * 2,
					0 );

				meshBuilder.TexCoord4f( 0,
					(flGridOrigins[iIndexLocal][0] + flLocalCoords[q][0]) * RADIOSITY_UVRATIO_X + vecViewportBase.x,
					(flGridOrigins[iIndexLocal][1] + flLocalCoords[q][1]) * RADIOSITY_UVRATIO_Y + vecViewportBase.y,
					flLocalGridLimits[q][0],
					flLocalGridLimits[q][1] );

				meshBuilder.TexCoord4f( 1,
					(flGridOrigins[iIndicesOne[0]][0] + flLocalCoords[q][0]) * RADIOSITY_UVRATIO_X + vecViewportBase.x,
					(flGridOrigins[iIndicesOne[0]][1] + flLocalCoords[q][1]) * RADIOSITY_UVRATIO_Y + vecViewportBase.y,
					(flGridOrigins[iIndicesOne[1]][0] + flLocalCoords[q][0]) * RADIOSITY_UVRATIO_X + vecViewportBase.x,
					(flGridOrigins[iIndicesOne[1]][1] + flLocalCoords[q][1]) * RADIOSITY_UVRATIO_Y + vecViewportBase.y );

				meshBuilder.TangentS3f( flLocalGridSize[q][0],
					flLocalGridSize[q][1],
					iIndexLocal * flWorldStepSize );

				meshBuilder.AdvanceVertex();
			}
		}
	}

	meshBuilder.End();

	return pRet;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
class CBaseWorldViewDeferred : public CRendering3dView
{
	DECLARE_CLASS( CBaseWorldViewDeferred, CRendering3dView );
protected:
	CBaseWorldViewDeferred(CViewRender *pMainView) : CRendering3dView( pMainView ) {}

	virtual bool	AdjustView( float waterHeight );

	void			DrawSetup( float waterHeight, int flags, float waterZAdjust, int iForceViewLeaf = -1, bool bShadowDepth = false );
	void			DrawExecute( float waterHeight, view_id_t viewID, float waterZAdjust, bool bShadowDepth = false );

	virtual void	PushView( float waterHeight );
	virtual void	PopView();

	// BUGBUG this causes all sorts of problems
	virtual bool	ShouldCacheLists(){ return false; };

	virtual void	DrawWorldDeferred( float waterZAdjust );
	virtual void	DrawOpaqueRenderablesDeferred( bool bNoDecals );

protected:

	void PushComposite();
	void PopComposite();
};


//-----------------------------------------------------------------------------
// Draws the scene when there's no water or only cheap water
//-----------------------------------------------------------------------------
class CSimpleWorldViewDeferred : public CBaseWorldViewDeferred
{
	DECLARE_CLASS( CSimpleWorldViewDeferred, CBaseWorldViewDeferred );
public:
	CSimpleWorldViewDeferred(CViewRender *pMainView) : CBaseWorldViewDeferred( pMainView ) {}

	void			Setup( const CViewSetup &view, int nClearFlags, bool bDrawSkybox, const VisibleFogVolumeInfo_t &fogInfo, const WaterRenderInfo_t& info, ViewCustomVisibility_t *pCustomVisibility = NULL );
	void			Draw();

	virtual bool	ShouldCacheLists(){ return true; };

private: 
	VisibleFogVolumeInfo_t m_fogInfo;

};

class CGBufferView : public CBaseWorldViewDeferred
{
	DECLARE_CLASS( CGBufferView, CBaseWorldViewDeferred );
public:
	CGBufferView(CViewRender *pMainView) : CBaseWorldViewDeferred( pMainView )
	{
	}

	void			Setup( const CViewSetup &view, bool bDrewSkybox );
	void			Draw();

	virtual void	PushView( float waterHeight );
	virtual void	PopView();

	static void PushGBuffer( bool bInitial, float zScale = 1.0f, bool bClearDepth = true );
	static void PopGBuffer();

private: 
	VisibleFogVolumeInfo_t m_fogInfo;
	bool m_bDrewSkybox;
};

class CSkyboxViewDeferred : public CGBufferView
{
	DECLARE_CLASS( CSkyboxViewDeferred, CRendering3dView );
public:
	CSkyboxViewDeferred(CViewRender *pMainView) : 
		CGBufferView( pMainView ),
		m_pSky3dParams( NULL )
	  {
	  }

	bool			Setup( const CViewSetup &view, bool bGBuffer, SkyboxVisibility_t *pSkyboxVisible );
	void			Draw();

protected:

	virtual SkyboxVisibility_t	ComputeSkyboxVisibility();
	bool			GetSkyboxFogEnable();

	void			Enable3dSkyboxFog( void );
	void			DrawInternal( view_id_t iSkyBoxViewID = VIEW_3DSKY, bool bInvokePreAndPostRender = true, ITexture *pRenderTarget = NULL );

	sky3dparams_t *	PreRender3dSkyboxWorld( SkyboxVisibility_t nSkyboxVisible );
	sky3dparams_t *m_pSky3dParams;

	bool		m_bGBufferPass;
};

class CPostLightingView : public CBaseWorldViewDeferred
{
	DECLARE_CLASS( CPostLightingView, CBaseWorldViewDeferred );
public:
	CPostLightingView(CViewRender *pMainView) : CBaseWorldViewDeferred( pMainView )
	{
	}

	void			Setup( const CViewSetup &view );
	void			Draw();

	virtual void	PushView( float waterHeight );
	virtual void	PopView();

	virtual void	DrawWorldDeferred( float waterZAdjust );
	virtual void	DrawOpaqueRenderablesDeferred( bool bNoDecals );

	static void		PushDeferredShadingFrameBuffer();
	static void		PopDeferredShadingFrameBuffer();

private: 
	VisibleFogVolumeInfo_t m_fogInfo;
};

abstract_class CBaseShadowView : public CBaseWorldViewDeferred
{
	DECLARE_CLASS( CBaseShadowView, CBaseWorldViewDeferred );
public:
	CBaseShadowView(CViewRender *pMainView) : CBaseWorldViewDeferred( pMainView )
	{
		m_bOutputRadiosity = false;
	};

	void			Setup( const CViewSetup &view,
						ITexture *pDepthTexture,
						ITexture *pDummyTexture );
	void			SetupRadiosityTargets(
						ITexture *pAlbedoTexture,
						ITexture *pNormalTexture );

	void SetRadiosityOutputEnabled( bool bEnabled );

	void			Draw();
	virtual bool	AdjustView( float waterHeight );
	virtual void	PushView( float waterHeight );
	virtual void	PopView();

	virtual void	CalcShadowView() = 0;
	virtual void	CommitData(){};

	virtual int		GetShadowMode() = 0;

private:

	ITexture *m_pDepthTexture;
	ITexture *m_pDummyTexture;
	ITexture *m_pRadAlbedoTexture;
	ITexture *m_pRadNormalTexture;
	ViewCustomVisibility_t shadowVis;

	bool m_bOutputRadiosity;
};

class COrthoShadowView : public CBaseShadowView
{
	DECLARE_CLASS( COrthoShadowView, CBaseShadowView );
public:
	COrthoShadowView(CViewRender *pMainView, const int &index)
		: CBaseShadowView( pMainView )
	{
			iCascadeIndex = index;
	}

	virtual void	CalcShadowView();
	virtual void	CommitData();

	virtual int		GetShadowMode(){
		return DEFERRED_SHADOW_MODE_ORTHO;
	};

private:
	int iCascadeIndex;
};

class CDualParaboloidShadowView : public CBaseShadowView
{
	DECLARE_CLASS( CDualParaboloidShadowView, CBaseShadowView );
public:
	CDualParaboloidShadowView(CViewRender *pMainView,
		def_light_t *pLight,
		const bool &bSecondary)
		: CBaseShadowView( pMainView )
	{
			m_pLight = pLight;
			m_bSecondary = bSecondary;
	}
	virtual bool	AdjustView( float waterHeight );
	virtual void	PushView( float waterHeight );
	virtual void	PopView();

	virtual void	CalcShadowView();

	virtual int		GetShadowMode(){
		return DEFERRED_SHADOW_MODE_DPSM;
	};

private:
	bool m_bSecondary;
	def_light_t *m_pLight;
};

class CSpotLightShadowView : public CBaseShadowView
{
	DECLARE_CLASS( CSpotLightShadowView, CBaseShadowView );
public:
	CSpotLightShadowView(CViewRender *pMainView,
		def_light_t *pLight, int index )
		: CBaseShadowView( pMainView )
	{
			m_pLight = pLight;
			m_iIndex = index;
	}

	virtual void	CalcShadowView();
	virtual void	CommitData();

	virtual int		GetShadowMode(){
		return DEFERRED_SHADOW_MODE_PROJECTED;
	};

private:
	def_light_t *m_pLight;
	int m_iIndex;
};

//-----------------------------------------------------------------------------
// Computes draw flags for the engine to build its world surface lists
//-----------------------------------------------------------------------------
static inline unsigned long BuildEngineDrawWorldListFlags( unsigned nDrawFlags )
{
	unsigned long nEngineFlags = 0;

	if ( ( nDrawFlags & DF_SKIP_WORLD ) == 0 )
	{
		nEngineFlags |= DRAWWORLDLISTS_DRAW_WORLD_GEOMETRY;
	}

	if ( ( nDrawFlags & DF_SKIP_WORLD_DECALS_AND_OVERLAYS ) == 0 )
	{
		nEngineFlags |= DRAWWORLDLISTS_DRAW_DECALS_AND_OVERLAYS;
	}

	if ( nDrawFlags & DF_DRAWSKYBOX )
	{
		nEngineFlags |= DRAWWORLDLISTS_DRAW_SKYBOX;
	}

	if ( nDrawFlags & DF_RENDER_ABOVEWATER )
	{
		nEngineFlags |= DRAWWORLDLISTS_DRAW_STRICTLYABOVEWATER;
		nEngineFlags |= DRAWWORLDLISTS_DRAW_INTERSECTSWATER;
	}

	if ( nDrawFlags & DF_RENDER_UNDERWATER )
	{
		nEngineFlags |= DRAWWORLDLISTS_DRAW_STRICTLYUNDERWATER;
		nEngineFlags |= DRAWWORLDLISTS_DRAW_INTERSECTSWATER;
	}

	if ( nDrawFlags & DF_RENDER_WATER )
	{
		nEngineFlags |= DRAWWORLDLISTS_DRAW_WATERSURFACE;
	}

	if( nDrawFlags & DF_CLIP_SKYBOX )
	{
		nEngineFlags |= DRAWWORLDLISTS_DRAW_CLIPSKYBOX;
	}

	if( nDrawFlags & DF_SHADOW_DEPTH_MAP )
	{
		nEngineFlags |= DRAWWORLDLISTS_DRAW_SHADOWDEPTH;
		nEngineFlags &= ~DRAWWORLDLISTS_DRAW_DECALS_AND_OVERLAYS;
	}

	if( nDrawFlags & DF_RENDER_REFRACTION )
	{
		nEngineFlags |= DRAWWORLDLISTS_DRAW_REFRACTION;
	}

	if( nDrawFlags & DF_RENDER_REFLECTION )
	{
		nEngineFlags |= DRAWWORLDLISTS_DRAW_REFLECTION;
	}

	return nEngineFlags;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static void SetClearColorToFogColor()
{
	unsigned char ucFogColor[3];
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->GetFogColor( ucFogColor );
	if( g_pMaterialSystemHardwareConfig->GetHDRType() == HDR_TYPE_INTEGER )
	{
		// @MULTICORE (toml 8/16/2006): Find a way to not do this twice in eye above water case
		float scale = LinearToGammaFullRange( pRenderContext->GetToneMappingScaleLinear().x );
		ucFogColor[0] *= scale;
		ucFogColor[1] *= scale;
		ucFogColor[2] *= scale;
	}
	pRenderContext->ClearColor4ub( ucFogColor[0], ucFogColor[1], ucFogColor[2], 255 );
}

//-----------------------------------------------------------------------------
// Precache of necessary materials
//-----------------------------------------------------------------------------

PRECACHE_REGISTER_BEGIN( GLOBAL, PrecacheDeferredPostProcessingEffects )
	//PRECACHE( MATERIAL, "dev/blurfiltery_and_add_nohdr" )
PRECACHE_REGISTER_END( )


//-----------------------------------------------------------------------------
// Methods to set the current view/guard access to view parameters
//-----------------------------------------------------------------------------
extern void AllowCurrentViewAccess( bool allow );
extern bool IsCurrentViewAccessAllowed();

extern void SetupCurrentView( const Vector &vecOrigin, const QAngle &angles, view_id_t viewID, bool bDrawWorldNormal = false, bool bCullFrontFaces = false );

extern view_id_t CurrentViewID();

//-----------------------------------------------------------------------------
// Purpose: Portal views are considered 'Main' views. This function tests a view id 
//			against all view ids used by portal renderables, as well as the main view.
//-----------------------------------------------------------------------------
extern bool IsMainView ( view_id_t id );

extern void FinishCurrentView();


//#if !defined( INFESTED_DLL )
//static CViewRender g_ViewRender;
//IViewRender *GetViewRenderInstance()
//{
//	return &g_ViewRender;
//}
//#endif


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CDeferredViewRender::CDeferredViewRender()
{
	m_pMesh_RadiosityScreenGrid[0] = NULL;
	m_pMesh_RadiosityScreenGrid[1] = NULL;
}

void CDeferredViewRender::Init()
{
	BaseClass::Init();
}

void CDeferredViewRender::Shutdown()
{
	CMatRenderContextPtr pRenderContext( materials );
	for ( int i = 0; i < 2; i++ )
	{
		if ( m_pMesh_RadiosityScreenGrid[ i ] != NULL )
			pRenderContext->DestroyStaticMesh( m_pMesh_RadiosityScreenGrid[ i ] );

		m_pMesh_RadiosityScreenGrid[ i ] = NULL;
	}

	for ( int i = 0; i < 2; i++ )
	{
		FOR_EACH_VEC( m_hRadiosityDebugMeshList[ i ], iMesh )
		{
			Assert( m_hRadiosityDebugMeshList[i][iMesh] != NULL );

			pRenderContext->DestroyStaticMesh( m_hRadiosityDebugMeshList[ i ][ iMesh ] );
			m_hRadiosityDebugMeshList[ i ].Remove( iMesh );
			iMesh--;
		}
	}

	BaseClass::Shutdown();
}

void CDeferredViewRender::LevelInit()
{
	BaseClass::LevelInit();
}

void CDeferredViewRender::LevelShutdown()
{
	BaseClass::LevelShutdown();
}

//-----------------------------------------------------------------------------
// Purpose: Renders world and all entities, etc.
//-----------------------------------------------------------------------------
void CDeferredViewRender::ViewDrawSceneDeferred( const CViewSetup &view, int nClearFlags, view_id_t viewID, bool bDrawViewModel )
{
	VPROF( "CViewRender::ViewDrawScene" );

	bool bDrew3dSkybox = false;
	SkyboxVisibility_t nSkyboxVisible = SKYBOX_NOT_VISIBLE;

	ViewDrawGBuffer( view, bDrew3dSkybox, nSkyboxVisible, bDrawViewModel );

	PerformLighting( view );

#if DEFCFG_DEFERRED_SHADING
	ViewCombineDeferredShading( view, viewID );
#else
	ViewDrawComposite( view, bDrew3dSkybox, nSkyboxVisible, nClearFlags, viewID, bDrawViewModel );
#endif

#if DEFCFG_ENABLE_RADIOSITY
	if ( deferred_radiosity_debug.GetBool() )
		DebugRadiosity( view );
#endif

#if DEFCFG_DEFERRED_SHADING == 1
	CPostLightingView::PushDeferredShadingFrameBuffer();
#endif

#ifdef SHADEREDITOR
	g_ShaderEditorSystem->UpdateSkymask( bDrew3dSkybox, view.x, view.y, view.width, view.height );
#endif

	GetLightingManager()->RenderVolumetrics( view );

	// Disable fog for the rest of the stuff
	DisableFog();

	// UNDONE: Don't do this with masked brush models, they should probably be in a separate list
	// render->DrawMaskEntities()

	// Here are the overlays...
	CGlowOverlay::DrawOverlays( view.m_bCacheFullSceneState );

	// issue the pixel visibility tests
	PixelVisibility_EndCurrentView();

	// Draw rain..
	DrawPrecipitation();

	// Make sure sound doesn't stutter
	engine->Sound_ExtraUpdate();

	// Debugging info goes over the top
	CDebugViewRender::Draw3DDebuggingInfo( view );

	// Draw client side effects
	// NOTE: These are not sorted against the rest of the frame
	clienteffects->DrawEffects( gpGlobals->frametime );	

	// Mark the frame as locked down for client fx additions
	SetFXCreationAllowed( false );

	// Invoke post-render methods
	IGameSystem::PostRenderAllSystems();

#if DEFCFG_DEFERRED_SHADING == 1
	CPostLightingView::PopDeferredShadingFrameBuffer();

	ViewOutputDeferredShading( view );
#endif

	FinishCurrentView();

	// Set int rendering parameters back to defaults
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_ENABLE_FIXED_LIGHTING, 0 );

	if ( view.m_bCullFrontFaces )
	{
		pRenderContext->FlipCulling( false );
	}
}

void CDeferredViewRender::ViewDrawGBuffer( const CViewSetup &view, bool &bDrew3dSkybox, SkyboxVisibility_t &nSkyboxVisible,
	bool bDrawViewModel )
{
	MDLCACHE_CRITICAL_SECTION();

	int oldViewID = g_CurrentViewID;
	g_CurrentViewID = VIEW_DEFERRED_GBUFFER;

	CSkyboxViewDeferred *pSkyView = new CSkyboxViewDeferred( this );
	if ( ( bDrew3dSkybox = pSkyView->Setup( view, true, &nSkyboxVisible ) ) != false )
		AddViewToScene( pSkyView );

	SafeRelease( pSkyView );

	// Start view
	unsigned int visFlags;
	SetupVis( view, visFlags, NULL );

	CRefPtr<CGBufferView> pGBufferView = new CGBufferView( this );
	pGBufferView->Setup( view, bDrew3dSkybox );
	AddViewToScene( pGBufferView );

	DrawViewModels( view, bDrawViewModel, true );

	g_CurrentViewID = oldViewID;
}

void CDeferredViewRender::ViewDrawComposite( const CViewSetup &view, bool &bDrew3dSkybox, SkyboxVisibility_t &nSkyboxVisible,
		int nClearFlags, view_id_t viewID, bool bDrawViewModel )
{
	DrawSkyboxComposite( view, bDrew3dSkybox );

	// this allows the refract texture to be updated once per *scene* on 360
	// (e.g. once for a monitor scene and once for the main scene)
	g_viewscene_refractUpdateFrame = gpGlobals->framecount - 1;

	m_BaseDrawFlags = 0;

	SetupCurrentView( view.origin, view.angles, viewID, view.m_bDrawWorldNormal, view.m_bCullFrontFaces );

	// Invoke pre-render methods
	IGameSystem::PreRenderAllSystems();

	// Start view
	unsigned int visFlags;
	SetupVis( view, visFlags, NULL );

	if ( !bDrew3dSkybox && 
		( nSkyboxVisible == SKYBOX_NOT_VISIBLE ) ) //&& ( visFlags & IVRenderView::VIEW_SETUP_VIS_EX_RETURN_FLAGS_USES_RADIAL_VIS ) )
	{
		// This covers the case where we don't see a 3dskybox, yet radial vis is clipping
		// the far plane.  Need to clear to fog color in this case.
		nClearFlags |= VIEW_CLEAR_COLOR;
		SetClearColorToFogColor( );
	}
	else
		nClearFlags |= VIEW_CLEAR_DEPTH;

	bool drawSkybox = r_skybox.GetBool();
	if ( bDrew3dSkybox || ( nSkyboxVisible == SKYBOX_NOT_VISIBLE ) )
		drawSkybox = false;

	ParticleMgr()->IncrementFrameCode();

	DrawWorldComposite( view, nClearFlags, drawSkybox );

#ifdef SHADEREDITOR
	VisibleFogVolumeInfo_t fogVolumeInfo;
	render->GetVisibleFogVolume( view.origin, &fogVolumeInfo );
	WaterRenderInfo_t info;
	DetermineWaterRenderInfo( fogVolumeInfo, info );
	g_ShaderEditorSystem->CustomViewRender( &g_CurrentViewID, fogVolumeInfo, info );
#endif

	DrawViewModels( view, bDrawViewModel, false );
}

void CDeferredViewRender::ViewCombineDeferredShading( const CViewSetup &view, view_id_t viewID )
{
#if DEFCFG_DEFERRED_SHADING == 1

	DrawLightPassFullscreen( GetDeferredManager()->GetDeferredMaterial( DEF_MAT_SCREENSPACE_SHADING ),
		view.width, view.height );

	g_viewscene_refractUpdateFrame = gpGlobals->framecount - 1;

	m_BaseDrawFlags = 0;

	SetupCurrentView( view.origin, view.angles, viewID, view.m_bDrawWorldNormal, view.m_bCullFrontFaces );

	IGameSystem::PreRenderAllSystems();

	ParticleMgr()->IncrementFrameCode();

	MDLCACHE_CRITICAL_SECTION();

	CRefPtr<CPostLightingView> pPostLightingView = new CPostLightingView( this );
	pPostLightingView->Setup( view );
	AddViewToScene( pPostLightingView );

	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->ClearBuffers( false, true );

#else

#endif
}

void CDeferredViewRender::ViewOutputDeferredShading( const CViewSetup &view )
{
#if DEFCFG_DEFERRED_SHADING

	DrawLightPassFullscreen( GetDeferredManager()->GetDeferredMaterial( DEF_MAT_SCREENSPACE_COMBINE ),
		view.width, view.height );

#endif
}

void CDeferredViewRender::DrawSkyboxComposite( const CViewSetup &view, const bool &bDrew3dSkybox )
{
	if ( !bDrew3dSkybox )
		return;

	CSkyboxViewDeferred *pSkyView = new CSkyboxViewDeferred( this );
	SkyboxVisibility_t nSkyboxVisible = SKYBOX_NOT_VISIBLE;
	if ( pSkyView->Setup( view, false, &nSkyboxVisible ) )
	{
		AddViewToScene( pSkyView );
#ifdef SHADEREDITOR
		g_ShaderEditorSystem->UpdateSkymask( bDrew3dSkybox, view.x, view.y, view.width, view.height );
#endif
	}

	SafeRelease( pSkyView );
	Assert( nSkyboxVisible == SKYBOX_3DSKYBOX_VISIBLE );
}

void CDeferredViewRender::DrawWorldComposite( const CViewSetup &view, int nClearFlags, bool bDrawSkybox )
{
	MDLCACHE_CRITICAL_SECTION();

	VisibleFogVolumeInfo_t fogVolumeInfo;

	render->GetVisibleFogVolume( view.origin, &fogVolumeInfo );

	WaterRenderInfo_t info;
	DetermineWaterRenderInfo( fogVolumeInfo, info );

	CRefPtr<CSimpleWorldViewDeferred> pNoWaterView = new CSimpleWorldViewDeferred( this );
	pNoWaterView->Setup( view, nClearFlags, bDrawSkybox, fogVolumeInfo, info );
	AddViewToScene( pNoWaterView );
}

static lightData_Global_t GetActiveGlobalLightState()
{
	lightData_Global_t data;
	CLightingEditor *pEditor = GetLightingEditor();

	if ( pEditor->IsEditorLightingActive() && pEditor->GetKVGlobalLight() != NULL )
	{
		data = pEditor->GetGlobalState();
	}
	else if ( GetGlobalLight() != NULL )
	{
		data = GetGlobalLight()->GetState();
	}

	return data;
}

void CDeferredViewRender::PerformLighting( const CViewSetup &view )
{
	bool bResetLightAccum = false;
	const bool bRadiosityEnabled = DEFCFG_ENABLE_RADIOSITY != 0 && deferred_radiosity_enable.GetBool();

	if ( bRadiosityEnabled )
		BeginRadiosity( view );

	if ( GetGlobalLight() != NULL )
	{
		struct defData_setGlobalLightState
		{
		public:
			lightData_Global_t state;

			static void Fire( defData_setGlobalLightState d )
			{
				GetDeferredExt()->CommitLightData_Global( d.state );
			};
		};

		defData_setGlobalLightState lightDataState;
		lightDataState.state = GetActiveGlobalLightState();

		if ( !GetLightingEditor()->IsEditorLightingActive() &&
			deferred_override_globalLight_enable.GetBool() )
		{
			lightDataState.state.bShadow = deferred_override_globalLight_shadow_enable.GetBool();
			UTIL_StringToVector( lightDataState.state.diff.AsVector3D().Base(), deferred_override_globalLight_diffuse.GetString() );
			UTIL_StringToVector( lightDataState.state.ambh.AsVector3D().Base(), deferred_override_globalLight_ambient_high.GetString() );
			UTIL_StringToVector( lightDataState.state.ambl.AsVector3D().Base(), deferred_override_globalLight_ambient_low.GetString() );

			lightDataState.state.bEnabled = ( lightDataState.state.diff.LengthSqr() > 0.01f ||
				lightDataState.state.ambh.LengthSqr() > 0.01f ||
				lightDataState.state.ambl.LengthSqr() > 0.01f );
		}

		QUEUE_FIRE( defData_setGlobalLightState, Fire, lightDataState );

		if ( lightDataState.state.bEnabled )
		{
			bool bShadowedGlobal = lightDataState.state.bShadow;

			if ( bShadowedGlobal )
			{
				Vector origins[2] = { view.origin, view.origin + lightDataState.state.vecLight.AsVector3D() * 1024 };
				render->ViewSetupVis( false, 2, origins );

				RenderCascadedShadows( view, bRadiosityEnabled );
			}
		}
		else
			bResetLightAccum = true;
	}
	else
		bResetLightAccum = true;

	CViewSetup lightingView = view;

	if ( building_cubemaps.GetBool() )
		engine->GetScreenSize( lightingView.width, lightingView.height );

	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->PushRenderTargetAndViewport( GetDefRT_Lightaccum() );

	if ( bResetLightAccum )
	{
		pRenderContext->ClearColor4ub( 0, 0, 0, 0 );
		pRenderContext->ClearBuffers( true, false );
	}
	else
		DrawLightPassFullscreen( GetDeferredManager()->GetDeferredMaterial( DEF_MAT_LIGHT_GLOBAL ), lightingView.width, lightingView.height );

	pRenderContext.SafeRelease();

	GetLightingManager()->RenderLights( lightingView, this );

	if ( bRadiosityEnabled )
		EndRadiosity( view );

	pRenderContext.GetFrom( materials );
	pRenderContext->PopRenderTargetAndViewport();
}

static int GetSourceRadBufferIndex( const int index )
{
	Assert( index == 0 || index == 1 );

	const bool bFar = index == 1;
	const int iNumSteps = bFar ? deferred_radiosity_propagate_count_far.GetInt() : deferred_radiosity_propagate_count.GetInt()
		+ bFar ? deferred_radiosity_blur_count_far.GetInt() : deferred_radiosity_blur_count.GetInt();
	return ( iNumSteps % 2 == 0 ) ? 0 : 1;
}

void CDeferredViewRender::BeginRadiosity( const CViewSetup &view )
{
	Vector fwd;
	AngleVectors( view.angles, &fwd );

	float flAmtVertical = abs( DotProduct( fwd, Vector( 0, 0, 1 ) ) );
	flAmtVertical = RemapValClamped( flAmtVertical, 0, 1, 1, 0.5f );

	for ( int iCascade = 0; iCascade < 2; iCascade++ )
	{
		const bool bFar = iCascade == 1;
		const Vector gridSize( RADIOSITY_BUFFER_SAMPLES_XY, RADIOSITY_BUFFER_SAMPLES_XY,
								RADIOSITY_BUFFER_SAMPLES_Z );
		const Vector gridSizeHalf = gridSize / 2;
		const float gridStepSize = bFar ? RADIOSITY_BUFFER_GRID_STEP_SIZE_FAR
			: RADIOSITY_BUFFER_GRID_STEP_SIZE_CLOSE;
		const float flGridDistance = bFar ? RADIOSITY_BUFFER_GRID_STEP_DISTANCEMULT_FAR
			: RADIOSITY_BUFFER_GRID_STEP_DISTANCEMULT_CLOSE;

		Vector vecFwd;
		AngleVectors( view.angles, &vecFwd );

		m_vecRadiosityOrigin[iCascade] = view.origin
			+ vecFwd * gridStepSize * RADIOSITY_BUFFER_SAMPLES_XY * flGridDistance * flAmtVertical;

		for ( int i = 0; i < 3; i++ )
			m_vecRadiosityOrigin[iCascade][i] -= fmod( m_vecRadiosityOrigin[iCascade][i], gridStepSize );

		m_vecRadiosityOrigin[iCascade] -= gridSizeHalf * gridStepSize;

		const int iSourceBuffer = GetSourceRadBufferIndex( iCascade );
		static int iLastSourceBuffer[2] = { iSourceBuffer, GetSourceRadBufferIndex( 1 ) };

		const int clearSizeY = RADIOSITY_BUFFER_RES_Y / 2;
		const int clearOffset = (iCascade == 1) ? clearSizeY : 0;

		CMatRenderContextPtr pRenderContext( materials );

		pRenderContext->PushRenderTargetAndViewport( GetDefRT_RadiosityBuffer( iSourceBuffer ), NULL,
			0, clearOffset, RADIOSITY_BUFFER_RES_X, clearSizeY );
		pRenderContext->ClearColor3ub( 0, 0, 0 );
		pRenderContext->ClearBuffers( true, false );
		pRenderContext->PopRenderTargetAndViewport();

		if ( iLastSourceBuffer[iCascade] != iSourceBuffer )
		{
			iLastSourceBuffer[iCascade] = iSourceBuffer;

			pRenderContext->PushRenderTargetAndViewport( GetDefRT_RadiosityBuffer( 1 - iSourceBuffer ), NULL,
				0, clearOffset, RADIOSITY_BUFFER_RES_X, clearSizeY );
			pRenderContext->ClearColor3ub( 0, 0, 0 );
			pRenderContext->ClearBuffers( true, false );
			pRenderContext->PopRenderTargetAndViewport();

			pRenderContext->PushRenderTargetAndViewport( GetDefRT_RadiosityNormal( 1 - iSourceBuffer ), NULL,
				0, clearOffset, RADIOSITY_BUFFER_RES_X, clearSizeY );
			pRenderContext->ClearColor3ub( 127, 127, 127 );
			pRenderContext->ClearBuffers( true, false );
			pRenderContext->PopRenderTargetAndViewport();
		}

		pRenderContext->PushRenderTargetAndViewport( GetDefRT_RadiosityNormal( iSourceBuffer ), NULL,
			0, clearOffset, RADIOSITY_BUFFER_RES_X, clearSizeY );
		pRenderContext->ClearColor3ub( 127, 127, 127 );
		pRenderContext->ClearBuffers( true, false );
		pRenderContext->PopRenderTargetAndViewport();
	}

	UpdateRadiosityPosition();
}

void CDeferredViewRender::UpdateRadiosityPosition()
{
	struct defData_setupRadiosity
	{
	public:
		radiosityData_t data;

		static void Fire( defData_setupRadiosity d )
		{
			GetDeferredExt()->CommitRadiosityData( d.data );
		};
	};

	defData_setupRadiosity radSetup;
	radSetup.data.vecOrigin[0] = m_vecRadiosityOrigin[0];
	radSetup.data.vecOrigin[1] = m_vecRadiosityOrigin[1];

	QUEUE_FIRE( defData_setupRadiosity, Fire, radSetup );
}

void CDeferredViewRender::PerformRadiosityGlobal( const int iRadiosityCascade, const CViewSetup &view )
{
	const int iSourceBuffer = GetSourceRadBufferIndex( iRadiosityCascade );
	const int iOffsetY = (iRadiosityCascade == 1) ? RADIOSITY_BUFFER_RES_Y/2 : 0;

	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_RADIOSITY_CASCADE, iRadiosityCascade );

	pRenderContext->PushRenderTargetAndViewport( GetDefRT_RadiosityBuffer( iSourceBuffer ), NULL,
		0, iOffsetY, RADIOSITY_BUFFER_VIEWPORT_SX, RADIOSITY_BUFFER_VIEWPORT_SY );
	pRenderContext->SetRenderTargetEx( 1, GetDefRT_RadiosityNormal( iSourceBuffer ) );

	pRenderContext->Bind( GetDeferredManager()->GetDeferredMaterial( DEF_MAT_LIGHT_RADIOSITY_GLOBAL ) );
	GetRadiosityScreenGrid( iRadiosityCascade )->Draw();

	pRenderContext->PopRenderTargetAndViewport();
}

void CDeferredViewRender::EndRadiosity( const CViewSetup &view )
{
	const int iNumPropagateSteps[2] = { deferred_radiosity_propagate_count.GetInt(),
		deferred_radiosity_propagate_count_far.GetInt() };
	const int iNumBlurSteps[2] = { deferred_radiosity_blur_count.GetInt(),
		deferred_radiosity_blur_count_far.GetInt() };

	IMaterial *pPropagateMat[2] = {
		GetDeferredManager()->GetDeferredMaterial( DEF_MAT_LIGHT_RADIOSITY_PROPAGATE_0 ),
		GetDeferredManager()->GetDeferredMaterial( DEF_MAT_LIGHT_RADIOSITY_PROPAGATE_1 ),
	};

	IMaterial *pBlurMat[2] = {
		GetDeferredManager()->GetDeferredMaterial( DEF_MAT_LIGHT_RADIOSITY_BLUR_0 ),
		GetDeferredManager()->GetDeferredMaterial( DEF_MAT_LIGHT_RADIOSITY_BLUR_1 ),
	};

	for ( int iCascade = 0; iCascade < 2; iCascade++ )
	{
		bool bSecondDestBuffer = GetSourceRadBufferIndex( iCascade ) == 0;
		const int iOffsetY = (iCascade==1) ? RADIOSITY_BUFFER_RES_Y / 2 : 0;

		for ( int i = 0; i < iNumPropagateSteps[iCascade]; i++ )
		{
			const int index = bSecondDestBuffer ? 1 : 0;
			CMatRenderContextPtr pRenderContext( materials );
			pRenderContext->PushRenderTargetAndViewport( GetDefRT_RadiosityBuffer( index ), NULL,
				0, iOffsetY, RADIOSITY_BUFFER_VIEWPORT_SX, RADIOSITY_BUFFER_VIEWPORT_SY );
			pRenderContext->SetRenderTargetEx( 1, GetDefRT_RadiosityNormal( index ) );

			pRenderContext->Bind( pPropagateMat[ 1 - index ] );

			GetRadiosityScreenGrid( iCascade )->Draw();

			pRenderContext->PopRenderTargetAndViewport();
			bSecondDestBuffer = !bSecondDestBuffer;
		}

		for ( int i = 0; i < iNumBlurSteps[iCascade]; i++ )
		{
			const int index = bSecondDestBuffer ? 1 : 0;
			CMatRenderContextPtr pRenderContext( materials );
			pRenderContext->PushRenderTargetAndViewport( GetDefRT_RadiosityBuffer( index ), NULL,
				0, iOffsetY, RADIOSITY_BUFFER_VIEWPORT_SX, RADIOSITY_BUFFER_VIEWPORT_SY );
			pRenderContext->SetRenderTargetEx( 1, GetDefRT_RadiosityNormal( index ) );

			pRenderContext->Bind( pBlurMat[ 1 - index ] );

			GetRadiosityScreenGrid( iCascade )->Draw();

			pRenderContext->PopRenderTargetAndViewport();
			bSecondDestBuffer = !bSecondDestBuffer;
		}
	}

#if ( DEFCFG_DEFERRED_SHADING == 0 )
	DrawLightPassFullscreen( GetDeferredManager()->GetDeferredMaterial( DEF_MAT_LIGHT_RADIOSITY_BLEND ),
		view.width, view.height );
#endif
}

void CDeferredViewRender::DebugRadiosity( const CViewSetup &view )
{
#if 0
	Vector tmp[3] = { vecRadiosityOrigin,
		vecRadiosityOrigin,
		vecRadiosityOrigin };

	const int directions[3][2] = {
		1, 2,
		0, 2,
		0, 1,
	};

	const Vector vecCross[3] = {
		Vector( 1, 0, 0 ),
		Vector( 0, 1, 0 ),
		Vector( 0, 0, 1 ),
	};

	const int iColors[3][3] = {
		255, 0, 0,
		0, 255, 0,
		0, 0, 255,
	};

	for ( int i = 0; i < 3; i++ )
	{
		for ( int x = 0; x < gridSize; x++ )
		{
			Vector tmp2 = tmp[i];

			for ( int y = 0; y < gridSize; y++ )
			{
				debugoverlay->AddLineOverlayAlpha( tmp2, tmp2 + vecCross[i] * gridStepSize * gridSize,
					iColors[i][0], iColors[i][1], iColors[i][2], 32,
					true, -1 );

				tmp2[ directions[i][1] ] += gridStepSize;
			}

			tmp[i][ directions[i][0] ] += gridStepSize;
		}
	}
#endif

	IMaterial *pMatDbgRadGrid = GetDeferredManager()->GetDeferredMaterial( DEF_MAT_LIGHT_RADIOSITY_DEBUG );

	if ( m_hRadiosityDebugMeshList[0].Count() == 0 )
	{
		for ( int iCascade = 0; iCascade < 2; iCascade++ )
		{
			const bool bFar = iCascade == 1;
			const float flGridSize = bFar ? RADIOSITY_BUFFER_GRID_STEP_SIZE_FAR : RADIOSITY_BUFFER_GRID_STEP_SIZE_CLOSE;
			const float flCubesize = flGridSize * 0.15f;
			const Vector directions[3] = {
				Vector( flGridSize, 0, 0 ),
				Vector( 0, flGridSize, 0 ),
				Vector( 0, 0, flGridSize ),
			};

			const float flUVOffsetY = bFar ? 0.5f : 0.0f;

			int nMaxVerts, nMaxIndices;
			CMatRenderContextPtr pRenderContext( materials );
			CMeshBuilder meshBuilder;

			IMesh *pMesh = pRenderContext->CreateStaticMesh( VERTEX_POSITION | VERTEX_TEXCOORD_SIZE( 0, 2 ),
				TEXTURE_GROUP_OTHER,
				pMatDbgRadGrid );
			m_hRadiosityDebugMeshList[iCascade].AddToTail( pMesh );

			IMesh *pMeshDummy = pRenderContext->GetDynamicMesh( true, NULL, NULL, pMatDbgRadGrid );
			pRenderContext->GetMaxToRender( pMeshDummy, false, &nMaxVerts, &nMaxIndices );
			pMeshDummy->Draw();

			int nMaxCubes = nMaxIndices / 36;
			if ( nMaxCubes > nMaxVerts / 24 )
				nMaxCubes = nMaxVerts / 24;

			int nRenderRemaining = nMaxCubes;
			meshBuilder.Begin( pMesh, MATERIAL_QUADS, nMaxCubes * 6 );

			const Vector2D flUVTexelSize( 1.0f / RADIOSITY_BUFFER_RES_X,
				1.0f / RADIOSITY_BUFFER_RES_Y );
			const Vector2D flUVTexelSizeHalf = flUVTexelSize * 0.5f;
			const Vector2D flUVGridSize =
				Vector2D( RADIOSITY_UVRATIO_X, RADIOSITY_UVRATIO_Y )
				* 1.0f / RADIOSITY_BUFFER_GRIDS_PER_AXIS;

			for ( int x = 0; x < RADIOSITY_BUFFER_SAMPLES_XY; x++ )
			for ( int y = 0; y < RADIOSITY_BUFFER_SAMPLES_XY; y++ )
			for ( int z = 0; z < RADIOSITY_BUFFER_SAMPLES_Z; z++ )
			{
				if ( nRenderRemaining <= 0 )
				{
					nRenderRemaining = nMaxCubes;
					meshBuilder.End();
					pMesh = pRenderContext->CreateStaticMesh( VERTEX_POSITION | VERTEX_TEXCOORD_SIZE( 0, 2 ),
								TEXTURE_GROUP_OTHER,
								GetDeferredManager()->GetDeferredMaterial( DEF_MAT_LIGHT_RADIOSITY_DEBUG ) );
					m_hRadiosityDebugMeshList[iCascade].AddToTail( pMesh );
					meshBuilder.Begin( pMesh, MATERIAL_QUADS, nMaxCubes * 6 );
				}

				int grid_x = z % RADIOSITY_BUFFER_GRIDS_PER_AXIS;
				int grid_y = z / RADIOSITY_BUFFER_GRIDS_PER_AXIS;

				float flUV[2] = {
					grid_x * flUVGridSize.x + x * flUVTexelSize.x + flUVTexelSizeHalf.x,
					grid_y * flUVGridSize.y + y * flUVTexelSize.y + flUVTexelSizeHalf.y + flUVOffsetY,
				};

				DrawCube( meshBuilder, directions[ 0 ] * x
					+ directions[ 1 ] * y
					+ directions[ 2 ] * z,
					flCubesize,
					flUV );

				nRenderRemaining--;
			}

			if ( nRenderRemaining != nMaxCubes )
				meshBuilder.End();
		}
	}

	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->Bind( pMatDbgRadGrid );

	for ( int iCascade = 0; iCascade < 2; iCascade++ )
	{
		VMatrix pos;
		pos.SetupMatrixOrgAngles( m_vecRadiosityOrigin[iCascade], vec3_angle );

		pRenderContext->MatrixMode( MATERIAL_MODEL );
		pRenderContext->PushMatrix();
		pRenderContext->LoadMatrix( pos );

		for ( int i = 0; i < m_hRadiosityDebugMeshList[iCascade].Count(); i++ )
			m_hRadiosityDebugMeshList[iCascade][ i ]->Draw();

		pRenderContext->MatrixMode( MATERIAL_MODEL );
		pRenderContext->PopMatrix();
	}
}

void CDeferredViewRender::RenderCascadedShadows( const CViewSetup &view, const bool bEnableRadiosity )
{
	for ( int i = 0; i < SHADOW_NUM_CASCADES; i++ )
	{
		const cascade_t &cascade = GetCascadeInfo(i);
		const bool bDoRadiosity = bEnableRadiosity && cascade.bOutputRadiosityData;
		const int iRadTarget = cascade.iRadiosityCascadeTarget;

#if CSM_USE_COMPOSITED_TARGET == 0
		int textureIndex = i;
#else
		int textureIndex = 0;
#endif

		CRefPtr<COrthoShadowView> pOrthoDepth = new COrthoShadowView( this, i );
		pOrthoDepth->Setup( view, GetShadowDepthRT_Ortho( textureIndex ), GetShadowColorRT_Ortho( textureIndex ) );
		if ( bDoRadiosity )
		{
			pOrthoDepth->SetRadiosityOutputEnabled( true );
			pOrthoDepth->SetupRadiosityTargets( GetRadiosityAlbedoRT_Ortho( textureIndex ),
				GetRadiosityNormalRT_Ortho( textureIndex ) );
		}
		AddViewToScene( pOrthoDepth );

		if ( bDoRadiosity )
			PerformRadiosityGlobal( iRadTarget, view );
	}
}

void CDeferredViewRender::DrawLightShadowView( const CViewSetup &view, int iDesiredShadowmap, def_light_t *l )
{
	CViewSetup setup;
	setup.origin = l->pos;
	setup.angles = l->ang;
	setup.m_bOrtho = false;
	setup.m_flAspectRatio = 1;
	setup.x = setup.y = 0;

	Vector origins[2] = { view.origin, l->pos };
	render->ViewSetupVis( false, 2, origins );

	switch ( l->iLighttype )
	{
	default:
		Assert( 0 );
		break;
	case DEFLIGHTTYPE_POINT:
		{
			CRefPtr<CDualParaboloidShadowView> pDPView0 = new CDualParaboloidShadowView( this, l, false );
			pDPView0->Setup( setup, GetShadowDepthRT_DP( iDesiredShadowmap ), GetShadowColorRT_DP( iDesiredShadowmap ) );
			AddViewToScene( pDPView0 );

			CRefPtr<CDualParaboloidShadowView> pDPView1 = new CDualParaboloidShadowView( this, l, true );
			pDPView1->Setup( setup, GetShadowDepthRT_DP( iDesiredShadowmap ), GetShadowColorRT_DP( iDesiredShadowmap ) );
			AddViewToScene( pDPView1 );
		}
		break;
	case DEFLIGHTTYPE_SPOT:
		{
			CRefPtr<CSpotLightShadowView> pProjView = new CSpotLightShadowView( this, l, iDesiredShadowmap );
			
			pProjView->Setup( setup, GetShadowDepthRT_Proj( iDesiredShadowmap ), GetShadowColorRT_Proj( iDesiredShadowmap ) );
			AddViewToScene( pProjView );
		}
		break;
	}
}

void CDeferredViewRender::DrawViewModels( const CViewSetup &view, bool drawViewmodel, bool bGBuffer )
{
	VPROF( "CViewRender::DrawViewModel" );

	bool bShouldDrawPlayerViewModel = ShouldDrawViewModel( drawViewmodel );
	bool bShouldDrawToolViewModels = ToolsEnabled();

	if ( !bShouldDrawPlayerViewModel && !bShouldDrawToolViewModels )
		return;

	CMatRenderContextPtr pRenderContext( materials );
	MDLCACHE_CRITICAL_SECTION();


	PIXEVENT( pRenderContext, "DrawViewModels()" );

	// Restore the matrices
	pRenderContext->MatrixMode( MATERIAL_PROJECTION );
	pRenderContext->PushMatrix();

	CViewSetup viewModelSetup( view );
	viewModelSetup.zNear = view.zNearViewmodel;
	viewModelSetup.zFar = view.zFarViewmodel;
	viewModelSetup.fov = view.fovViewmodel;
	viewModelSetup.m_flAspectRatio = engine->GetScreenAspectRatio( view.width, view.height );

	render->Push3DView( viewModelSetup, 0, NULL, GetFrustum() );

	if ( bGBuffer )
	{
		const float flViewmodelScale = view.zFarViewmodel / view.zFar;
		CGBufferView::PushGBuffer( false, flViewmodelScale, false );
	}
	else
	{
		pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_RENDER_STAGE,
			DEFERRED_RENDER_STAGE_COMPOSITION );
	}


	const bool bUseDepthHack = true;

	// FIXME: Add code to read the current depth range
	float depthmin = 0.0f;
	float depthmax = 1.0f;

	// HACK HACK:  Munge the depth range to prevent view model from poking into walls, etc.
	// Force clipped down range
	if( bUseDepthHack )
		pRenderContext->DepthRange( 0.0f, 0.1f );
	
	CViewModelRenderablesList list;
	ClientLeafSystem()->CollateViewModelRenderables( &list );
	CViewModelRenderablesList::RenderGroups_t &opaqueList = list.m_RenderGroups[ CViewModelRenderablesList::VM_GROUP_OPAQUE ];
	CViewModelRenderablesList::RenderGroups_t &translucentList = list.m_RenderGroups[ CViewModelRenderablesList::VM_GROUP_TRANSLUCENT ];

	if ( ToolsEnabled() && ( !bShouldDrawPlayerViewModel || !bShouldDrawToolViewModels ) )
	{
		int nOpaque = opaqueList.Count();
		for ( int i = nOpaque-1; i >= 0; --i )
		{
			IClientRenderable *pRenderable = opaqueList[ i ].m_pRenderable;
			bool bEntity = pRenderable->GetIClientUnknown()->GetBaseEntity() ? true : false;
			if ( ( bEntity && !bShouldDrawPlayerViewModel ) || ( !bEntity && !bShouldDrawToolViewModels ) )
			{
				opaqueList.FastRemove( i );
			}
		}

		int nTranslucent = translucentList.Count();
		for ( int i = nTranslucent-1; i >= 0; --i )
		{
			IClientRenderable *pRenderable = translucentList[ i ].m_pRenderable;
			bool bEntity = pRenderable->GetIClientUnknown()->GetBaseEntity() ? true : false;
			if ( ( bEntity && !bShouldDrawPlayerViewModel ) || ( !bEntity && !bShouldDrawToolViewModels ) )
			{
				translucentList.FastRemove( i );
			}
		}
	}

	// Update refract for opaque models & draw
	bool bUpdatedRefractForOpaque = UpdateRefractIfNeededByList( opaqueList );
	DrawRenderablesInList( opaqueList );

	if ( !bGBuffer )
	{
		// Update refract for translucent models (if we didn't already update it above) & draw
		if ( !bUpdatedRefractForOpaque ) // Only do this once for better perf
		{
			UpdateRefractIfNeededByList( translucentList );
		}
		DrawRenderablesInList( translucentList, STUDIO_TRANSPARENCY );
	}
	else
	{
		pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_RENDER_STAGE,
			DEFERRED_RENDER_STAGE_INVALID );
	}

	// Reset the depth range to the original values
	if( bUseDepthHack )
		pRenderContext->DepthRange( depthmin, depthmax );

	if ( bGBuffer )
	{
		CGBufferView::PopGBuffer();
	}

	render->PopView( GetFrustum() );

	// Restore the matrices
	pRenderContext->MatrixMode( MATERIAL_PROJECTION );
	pRenderContext->PopMatrix();
}

//-----------------------------------------------------------------------------
// Purpose: This renders the entire 3D view and the in-game hud/viewmodel
// Input  : &view - 
//			whatToDraw - 
//-----------------------------------------------------------------------------
// This renders the entire 3D view.
void CDeferredViewRender::RenderView( const CViewSetup &view, const CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw )
{
	m_UnderWaterOverlayMaterial.Shutdown();					// underwater view will set

	ASSERT_LOCAL_PLAYER_RESOLVABLE();
	int slot = GET_ACTIVE_SPLITSCREEN_SLOT();

	CViewSetup worldView = view;

	CLightingEditor *pLightEditor = GetLightingEditor();

	if ( pLightEditor->IsEditorActive() && !building_cubemaps.GetBool() )
		pLightEditor->GetEditorView( &worldView.origin, &worldView.angles );
	else
		pLightEditor->SetEditorView( &worldView.origin, &worldView.angles );

	m_CurrentView = worldView;

	C_BaseAnimating::AutoAllowBoneAccess boneaccess( true, true );
	VPROF( "CViewRender::RenderView" );

	{
		// HACK: server-side weapons use the viewmodel model, and client-side weapons swap that out for
		// the world model in DrawModel.  This is too late for some bone setup work that happens before
		// DrawModel, so here we just iterate all weapons we know of and fix them up ahead of time.
		MDLCACHE_CRITICAL_SECTION();
		CUtlLinkedList< CBaseCombatWeapon * > &weaponList = C_BaseCombatWeapon::GetWeaponList();
		FOR_EACH_LL( weaponList, it )
		{
			C_BaseCombatWeapon *weapon = weaponList[it];
			if ( !weapon->IsDormant() )
			{
				weapon->EnsureCorrectRenderingModel();
			}
		}
	}

	CMatRenderContextPtr pRenderContext( materials );
	ITexture *saveRenderTarget = pRenderContext->GetRenderTarget();
	pRenderContext.SafeRelease(); // don't want to hold for long periods in case in a locking active share thread mode

	{
		RenderPreScene( worldView );

		// Must be first 
		render->SceneBegin();

		g_pColorCorrectionMgr->UpdateColorCorrection();

		// Send the current tonemap scalar to the material system
		UpdateMaterialSystemTonemapScalar();

		// clear happens here probably
		SetupMain3DView( slot, worldView, hudViewSetup, nClearFlags, saveRenderTarget );

		g_pClientShadowMgr->UpdateSplitscreenLocalPlayerShadowSkip();

		ProcessDeferredGlobals( worldView );
		GetLightingManager()->LightSetup( worldView );

		PreViewDrawScene( worldView );

		// Force it to clear the framebuffer if they're in solid space.
		if ( ( nClearFlags & VIEW_CLEAR_COLOR ) == 0 )
		{
			MDLCACHE_CRITICAL_SECTION();
			if ( enginetrace->GetPointContents( worldView.origin ) == CONTENTS_SOLID )
			{
				nClearFlags |= VIEW_CLEAR_COLOR;
			}
		}

		ViewDrawSceneDeferred( worldView, nClearFlags, VIEW_MAIN, whatToDraw & RENDERVIEW_DRAWVIEWMODEL );

		// We can still use the 'current view' stuff set up in ViewDrawScene
		AllowCurrentViewAccess( true );

		// must happen before teardown
		pLightEditor->OnRender();

		GetLightingManager()->LightTearDown();

		PostViewDrawScene( worldView );

		engine->DrawPortals();

		DisableFog();

		// Finish scene
		render->SceneEnd();

		// Draw lightsources if enabled
		//render->DrawLights();

		//RenderPlayerSprites();

		// Image-space motion blur and depth of field
		#if defined( _X360 )
		{
			CMatRenderContextPtr pRenderContext( materials );
			pRenderContext->PushVertexShaderGPRAllocation( 16 ); //Max out pixel shader threads
			pRenderContext.SafeRelease();
		}
		#endif

		if ( !building_cubemaps.GetBool() )
		{
			if ( IsDepthOfFieldEnabled() )
			{
				pRenderContext.GetFrom( materials );
				{
					PIXEVENT( pRenderContext, "DoDepthOfField()" );
					DoDepthOfField( worldView );
				}
				pRenderContext.SafeRelease();
			}

			if ( ( worldView.m_nMotionBlurMode != MOTION_BLUR_DISABLE ) && ( mat_motion_blur_enabled.GetInt() ) )
			{
				pRenderContext.GetFrom( materials );
				{
					PIXEVENT( pRenderContext, "DoImageSpaceMotionBlur()" );
					DoImageSpaceMotionBlur( worldView );
				}
				pRenderContext.SafeRelease();
			}
		}

		#if defined( _X360 )
		{
			CMatRenderContextPtr pRenderContext( materials );
			pRenderContext->PopVertexShaderGPRAllocation();
			pRenderContext.SafeRelease();
		}
		#endif

		// Now actually draw the viewmodel
		//DrawViewModels( view, whatToDraw & RENDERVIEW_DRAWVIEWMODEL );

		DrawUnderwaterOverlay();

		PixelVisibility_EndScene();

		#if defined( _X360 )
		{
			CMatRenderContextPtr pRenderContext( materials );
			pRenderContext->PushVertexShaderGPRAllocation( 16 ); //Max out pixel shader threads
			pRenderContext.SafeRelease();
		}
		#endif

		// Draw fade over entire screen if needed
		byte color[4];
		bool blend;
		GetViewEffects()->GetFadeParams( &color[0], &color[1], &color[2], &color[3], &blend );

		// Store off color fade params to be applied in fullscreen postprocess pass
		SetViewFadeParams( color[0], color[1], color[2], color[3], blend );

		// Draw an overlay to make it even harder to see inside smoke particle systems.
		DrawSmokeFogOverlay();

		// Overlay screen fade on entire screen
		PerformScreenOverlay( worldView.x, worldView.y, worldView.width, worldView.height );

		// Prevent sound stutter if going slow
		engine->Sound_ExtraUpdate();	

		if ( g_pMaterialSystemHardwareConfig->GetHDRType() != HDR_TYPE_NONE )
		{
			pRenderContext.GetFrom( materials );
			pRenderContext->SetToneMappingScaleLinear(Vector(1,1,1));
			pRenderContext.SafeRelease();
		}

		if ( !building_cubemaps.GetBool() && worldView.m_bDoBloomAndToneMapping )
		{
			pRenderContext.GetFrom( materials );
			{
				static bool bAlreadyShowedLoadTime = false;
				
				if ( ! bAlreadyShowedLoadTime )
				{
					bAlreadyShowedLoadTime = true;
					if ( CommandLine()->CheckParm( "-timeload" ) )
					{
						Warning( "time to initial render = %f\n", Plat_FloatTime() );
					}
				}

				PIXEVENT( pRenderContext, "DoEnginePostProcessing()" );

				bool bFlashlightIsOn = false;
				C_BasePlayer *pLocal = C_BasePlayer::GetLocalPlayer();
				if ( pLocal )
				{
					bFlashlightIsOn = pLocal->IsEffectActive( EF_DIMLIGHT );
				}
				DoEnginePostProcessing( worldView.x, worldView.y, worldView.width, worldView.height, bFlashlightIsOn );
			}
			pRenderContext.SafeRelease();
		}

#ifdef SHADEREDITOR
		g_ShaderEditorSystem->CustomPostRender();
#endif

		// And here are the screen-space effects

		if ( IsPC() )
		{
			// Grab the pre-color corrected frame for editing purposes
			engine->GrabPreColorCorrectedFrame( worldView.x, worldView.y, worldView.width, worldView.height );
		}

		PerformScreenSpaceEffects( worldView.x, worldView.y, worldView.width, worldView.height );


		#if defined( _X360 )
		{
			CMatRenderContextPtr pRenderContext( materials );
			pRenderContext->PopVertexShaderGPRAllocation();
			pRenderContext.SafeRelease();
		}
		#endif

		GetClientMode()->DoPostScreenSpaceEffects( &worldView );

		CleanupMain3DView( worldView );

		if ( m_FreezeParams[ slot ].m_bTakeFreezeFrame )
		{
			pRenderContext = materials->GetRenderContext();
			if ( IsX360() )
			{
				// 360 doesn't create the Fullscreen texture
				pRenderContext->CopyRenderTargetToTextureEx( GetFullFrameFrameBufferTexture( 1 ), 0, NULL, NULL );
			}
			else
			{
				pRenderContext->CopyRenderTargetToTextureEx( GetFullscreenTexture(), 0, NULL, NULL );
			}
			pRenderContext.SafeRelease();
			m_FreezeParams[ slot ].m_bTakeFreezeFrame = false;
		}

		pRenderContext = materials->GetRenderContext();
		pRenderContext->SetRenderTarget( saveRenderTarget );
		pRenderContext.SafeRelease();

		// Draw the overlay
		if ( m_bDrawOverlay )
		{	   
			// This allows us to be ok if there are nested overlay views
			CViewSetup currentView = m_CurrentView;
			CViewSetup tempView = m_OverlayViewSetup;
			tempView.fov = ScaleFOVByWidthRatio( tempView.fov, tempView.m_flAspectRatio / ( 4.0f / 3.0f ) );
			tempView.m_bDoBloomAndToneMapping = false;				// FIXME: Hack to get Mark up and running
			tempView.m_nMotionBlurMode = MOTION_BLUR_DISABLE;		// FIXME: Hack to get Mark up and running
			m_bDrawOverlay = false;
			RenderView( tempView, hudViewSetup, m_OverlayClearFlags, m_OverlayDrawFlags );
			m_CurrentView = currentView;
		}
	}

	// Clear a row of pixels at the edge of the viewport if it isn't at the edge of the screen
	if ( VGui_IsSplitScreen() )
	{
		CMatRenderContextPtr pRenderContext( materials );
		pRenderContext->PushRenderTargetAndViewport();

		int nScreenWidth, nScreenHeight;
		g_pMaterialSystem->GetBackBufferDimensions( nScreenWidth, nScreenHeight );

		// NOTE: view.height is off by 1 on the PC in a release build, but debug is correct! I'm leaving this here to help track this down later.
		// engine->Con_NPrintf( 25 + hh, "view( %d, %d, %d, %d ) GetBackBufferDimensions( %d, %d )\n", view.x, view.y, view.width, view.height, nScreenWidth, nScreenHeight );

		if ( worldView.x != 0 ) // if left of viewport isn't at 0
		{
			pRenderContext->Viewport( worldView.x, worldView.y, 1, worldView.height );
			pRenderContext->ClearColor3ub( 0, 0, 0 );
			pRenderContext->ClearBuffers( true, false );
		}

		if ( ( worldView.x + worldView.width ) != nScreenWidth ) // if right of viewport isn't at edge of screen
		{
			pRenderContext->Viewport( worldView.x + worldView.width - 1, worldView.y, 1, worldView.height );
			pRenderContext->ClearColor3ub( 0, 0, 0 );
			pRenderContext->ClearBuffers( true, false );
		}

		if ( worldView.y != 0 ) // if top of viewport isn't at 0
		{
			pRenderContext->Viewport( worldView.x, worldView.y, worldView.width, 1 );
			pRenderContext->ClearColor3ub( 0, 0, 0 );
			pRenderContext->ClearBuffers( true, false );
		}

		if ( ( worldView.y + worldView.height ) != nScreenHeight ) // if bottom of viewport isn't at edge of screen
		{
			pRenderContext->Viewport( worldView.x, worldView.y + worldView.height - 1, worldView.width, 1 );
			pRenderContext->ClearColor3ub( 0, 0, 0 );
			pRenderContext->ClearBuffers( true, false );
		}

		pRenderContext->PopRenderTargetAndViewport();
		pRenderContext->Release();
	}

	// Draw the 2D graphics
	m_CurrentView = hudViewSetup;
	pRenderContext = materials->GetRenderContext();
	if ( true )
	{
		PIXEVENT( pRenderContext, "2D Client Rendering" );

		render->Push2DView( hudViewSetup, 0, saveRenderTarget, GetFrustum() );

		Render2DEffectsPreHUD( hudViewSetup );

		if ( whatToDraw & RENDERVIEW_DRAWHUD )
		{
			VPROF_BUDGET( "VGui_DrawHud", VPROF_BUDGETGROUP_OTHER_VGUI );
			// paint the vgui screen
			VGui_PreRender();

			CUtlVector< vgui::VPANEL > vecHudPanels;

			vecHudPanels.AddToTail( VGui_GetClientDLLRootPanel() );

			// This block is suspect - why are we resizing fullscreen panels to be the size of the hudViewSetup
			// which is potentially only half the screen
			if ( GET_ACTIVE_SPLITSCREEN_SLOT() == 0 )
			{
				vecHudPanels.AddToTail( VGui_GetFullscreenRootVPANEL() );

#if defined( TOOLFRAMEWORK_VGUI_REFACTOR )
				vecHudPanels.AddToTail( enginevgui->GetPanel( PANEL_GAMEUIDLL ) );
#endif
				vecHudPanels.AddToTail( enginevgui->GetPanel( PANEL_CLIENTDLL_TOOLS ) );
			}

			PositionHudPanels( vecHudPanels, hudViewSetup );

			// The crosshair, etc. needs to get at the current setup stuff
			AllowCurrentViewAccess( true );

			// Draw the in-game stuff based on the actual viewport being used
			render->VGui_Paint( PAINT_INGAMEPANELS );

			AllowCurrentViewAccess( false );

			VGui_PostRender();

			GetClientMode()->PostRenderVGui();
			pRenderContext->Flush();
		}

		CDebugViewRender::Draw2DDebuggingInfo( hudViewSetup );

		Render2DEffectsPostHUD( hudViewSetup );

		// We can no longer use the 'current view' stuff set up in ViewDrawScene
		AllowCurrentViewAccess( false );

		if ( IsPC() )
		{
			CDebugViewRender::GenerateOverdrawForTesting();
		}

		render->PopView( GetFrustum() );
	}
	pRenderContext.SafeRelease();

	FlushWorldLists();

	m_CurrentView = worldView;

#ifdef PARTICLE_USAGE_DEMO
	ParticleUsageDemo();
#endif
}

struct defData_setGlobals
{
public:
	Vector orig, fwd;
	float zDists[2];
	VMatrix frustumDeltas;
#if DEFCFG_BILATERAL_DEPTH_TEST
	VMatrix worldCameraDepthTex;
#endif

	static void Fire( defData_setGlobals d )
	{
		IDeferredExtension *pDef = GetDeferredExt();
		pDef->CommitOrigin( d.orig );
		pDef->CommitViewForward( d.fwd );
		pDef->CommitZDists( d.zDists[0], d.zDists[1] );
		pDef->CommitFrustumDeltas( d.frustumDeltas );
#if DEFCFG_BILATERAL_DEPTH_TEST
		pDef->CommitWorldToCameraDepthTex( d.worldCameraDepthTex );
#endif
	};
};

void CDeferredViewRender::ProcessDeferredGlobals( const CViewSetup &view )
{
	VMatrix matPerspective, matView, matViewProj, screen2world;
	matView.Identity();
	matView.SetupMatrixOrgAngles( vec3_origin, view.angles );

	MatrixSourceToDeviceSpace( matView );
#ifdef SHADEREDITOR
	//g_ShaderEditorSystem->SetMainViewMatrix( matView );
#endif

	matView = matView.Transpose3x3();
	Vector viewPosition;

	Vector3DMultiply( matView, view.origin, viewPosition );
	matView.SetTranslation( -viewPosition );
	MatrixBuildPerspectiveX( matPerspective, view.fov, view.m_flAspectRatio,
		view.zNear, view.zFar );
	MatrixMultiply( matPerspective, matView, matViewProj );

	MatrixInverseGeneral( matViewProj, screen2world );

	GetLightingManager()->SetRenderConstants( screen2world, view );

	Vector frustum_c0, frustum_cc, frustum_1c;
	float projDistance = 1.0f;
	Vector3DMultiplyPositionProjective( screen2world, Vector(0,projDistance,projDistance), frustum_c0 );
	Vector3DMultiplyPositionProjective( screen2world, Vector(0,0,projDistance), frustum_cc );
	Vector3DMultiplyPositionProjective( screen2world, Vector(projDistance,0,projDistance), frustum_1c );

	frustum_c0 -= view.origin;
	frustum_cc -= view.origin;
	frustum_1c -= view.origin;

	Vector frustum_up = frustum_c0 - frustum_cc;
	Vector frustum_right = frustum_1c - frustum_cc;

	frustum_cc /= view.zFar;
	frustum_right /= view.zFar;
	frustum_up /= view.zFar;

	defData_setGlobals data;
	data.orig = view.origin;
	AngleVectors( view.angles, &data.fwd );
	data.zDists[0] = view.zNear;
	data.zDists[1] = view.zFar;

	data.frustumDeltas.Identity();
	data.frustumDeltas.SetBasisVectors( frustum_cc, frustum_right, frustum_up );
	data.frustumDeltas = data.frustumDeltas.Transpose3x3();

#if DEFCFG_BILATERAL_DEPTH_TEST
	VMatrix matWorldToCameraDepthTex;
	MatrixBuildScale( matWorldToCameraDepthTex, 0.5f, -0.5f, 1.0f );
	matWorldToCameraDepthTex[0][3] = matWorldToCameraDepthTex[1][3] = 0.5f;
	MatrixMultiply( matWorldToCameraDepthTex, matViewProj, matWorldToCameraDepthTex );

	data.worldCameraDepthTex = matWorldToCameraDepthTex.Transpose();
#endif

	QUEUE_FIRE( defData_setGlobals, Fire, data );
}

//-----------------------------------------------------------------------------
// Returns true if the view plane intersects the water
//-----------------------------------------------------------------------------
extern bool DoesViewPlaneIntersectWater( float waterZ, int leafWaterDataID );



//-----------------------------------------------------------------------------
// Fakes per-entity clip planes on cards that don't support user clip planes.
//  Achieves the effect by drawing an invisible box that writes to the depth buffer
//  around the clipped area. It's not perfect, but better than nothing.
//-----------------------------------------------------------------------------
static void DrawClippedDepthBox( IClientRenderable *pEnt, float *pClipPlane )
{
//#define DEBUG_DRAWCLIPPEDDEPTHBOX //uncomment to draw the depth box as a colorful box

	static const int iQuads[6][5] = {	{ 0, 4, 6, 2, 0 }, //always an extra copy of first index at end to make some algorithms simpler
										{ 3, 7, 5, 1, 3 },
										{ 1, 5, 4, 0, 1 },
										{ 2, 6, 7, 3, 2 },
										{ 0, 2, 3, 1, 0 },
										{ 5, 7, 6, 4, 5 } };

	static const int iLines[12][2] = {	{ 0, 1 },
										{ 0, 2 },
										{ 0, 4 },
										{ 1, 3 },
										{ 1, 5 },
										{ 2, 3 },
										{ 2, 6 },
										{ 3, 7 },
										{ 4, 6 },
										{ 4, 5 },
										{ 5, 7 },
										{ 6, 7 } };


#ifdef DEBUG_DRAWCLIPPEDDEPTHBOX
	static const float fColors[6][3] = {	{ 1.0f, 0.0f, 0.0f },
											{ 0.0f, 1.0f, 1.0f },
											{ 0.0f, 1.0f, 0.0f },
											{ 1.0f, 0.0f, 1.0f },
											{ 0.0f, 0.0f, 1.0f },
											{ 1.0f, 1.0f, 0.0f } };
#endif

	
	

	Vector vNormal = *(Vector *)pClipPlane;
	float fPlaneDist = pClipPlane[3];

	Vector vMins, vMaxs;
	pEnt->GetRenderBounds( vMins, vMaxs );

	Vector vOrigin = pEnt->GetRenderOrigin();
	QAngle qAngles = pEnt->GetRenderAngles();
	
	Vector vForward, vUp, vRight;
	AngleVectors( qAngles, &vForward, &vRight, &vUp );

	Vector vPoints[8];
	vPoints[0] = vOrigin + (vForward * vMins.x) + (vRight * vMins.y) + (vUp * vMins.z);
	vPoints[1] = vOrigin + (vForward * vMaxs.x) + (vRight * vMins.y) + (vUp * vMins.z);
	vPoints[2] = vOrigin + (vForward * vMins.x) + (vRight * vMaxs.y) + (vUp * vMins.z);
	vPoints[3] = vOrigin + (vForward * vMaxs.x) + (vRight * vMaxs.y) + (vUp * vMins.z);
	vPoints[4] = vOrigin + (vForward * vMins.x) + (vRight * vMins.y) + (vUp * vMaxs.z);
	vPoints[5] = vOrigin + (vForward * vMaxs.x) + (vRight * vMins.y) + (vUp * vMaxs.z);
	vPoints[6] = vOrigin + (vForward * vMins.x) + (vRight * vMaxs.y) + (vUp * vMaxs.z);
	vPoints[7] = vOrigin + (vForward * vMaxs.x) + (vRight * vMaxs.y) + (vUp * vMaxs.z);

	int iClipped[8];
	float fDists[8];
	for( int i = 0; i != 8; ++i )
	{
		fDists[i] = vPoints[i].Dot( vNormal ) - fPlaneDist;
		iClipped[i] = (fDists[i] > 0.0f) ? 1 : 0;
	}

	Vector vSplitPoints[8][8]; //obviously there are only 12 lines, not 64 lines or 64 split points, but the indexing is way easier like this
	int iLineStates[8][8]; //0 = unclipped, 2 = wholly clipped, 3 = first point clipped, 4 = second point clipped

	//categorize lines and generate split points where needed
	for( int i = 0; i != 12; ++i )
	{
		const int *pPoints = iLines[i];
		int iLineState = (iClipped[pPoints[0]] + iClipped[pPoints[1]]);
		if( iLineState != 1 ) //either both points are clipped, or neither are clipped
		{
			iLineStates[pPoints[0]][pPoints[1]] = 
				iLineStates[pPoints[1]][pPoints[0]] = 
					iLineState;
		}
		else
		{
			//one point is clipped, the other is not
			if( iClipped[pPoints[0]] == 1 )
			{
				//first point was clipped, index 1 has the negative distance
				float fInvTotalDist = 1.0f / (fDists[pPoints[0]] - fDists[pPoints[1]]);
				vSplitPoints[pPoints[0]][pPoints[1]] = 
					vSplitPoints[pPoints[1]][pPoints[0]] =
						(vPoints[pPoints[1]] * (fDists[pPoints[0]] * fInvTotalDist)) - (vPoints[pPoints[0]] * (fDists[pPoints[1]] * fInvTotalDist));
				
				Assert( fabs( vNormal.Dot( vSplitPoints[pPoints[0]][pPoints[1]] ) - fPlaneDist ) < 0.01f );

				iLineStates[pPoints[0]][pPoints[1]] = 3;
				iLineStates[pPoints[1]][pPoints[0]] = 4;
			}
			else
			{
				//second point was clipped, index 0 has the negative distance
				float fInvTotalDist = 1.0f / (fDists[pPoints[1]] - fDists[pPoints[0]]);
				vSplitPoints[pPoints[0]][pPoints[1]] = 
					vSplitPoints[pPoints[1]][pPoints[0]] =
						(vPoints[pPoints[0]] * (fDists[pPoints[1]] * fInvTotalDist)) - (vPoints[pPoints[1]] * (fDists[pPoints[0]] * fInvTotalDist));

				Assert( fabs( vNormal.Dot( vSplitPoints[pPoints[0]][pPoints[1]] ) - fPlaneDist ) < 0.01f );

				iLineStates[pPoints[0]][pPoints[1]] = 4;
				iLineStates[pPoints[1]][pPoints[0]] = 3;
			}
		}
	}

	extern CMaterialReference g_material_WriteZ;
	CMatRenderContextPtr pRenderContext( materials );
	
#ifdef DEBUG_DRAWCLIPPEDDEPTHBOX
	pRenderContext->Bind( materials->FindMaterial( "debug/debugvertexcolor", TEXTURE_GROUP_OTHER ), NULL );
#else
	pRenderContext->Bind( g_material_WriteZ, NULL );
#endif

	CMeshBuilder meshBuilder;
	IMesh* pMesh = pRenderContext->GetDynamicMesh( false );
	meshBuilder.Begin( pMesh, MATERIAL_TRIANGLES, 18 ); //6 sides, possible one cut per side. Any side is capable of having 3 tri's. Lots of padding for things that aren't possible

	//going to draw as a collection of triangles, arranged as a triangle fan on each side
	for( int i = 0; i != 6; ++i )
	{
		const int *pPoints = iQuads[i];

		//can't start the fan on a wholly clipped line, so seek to one that isn't
		int j = 0;
		do
		{
			if( iLineStates[pPoints[j]][pPoints[j+1]] != 2 ) //at least part of this line will be drawn
				break;

			++j;
		} while( j != 3 );

		if( j == 3 ) //not enough lines to even form a triangle
			continue;

		float *pStartPoint = static_cast<float *>(0);
		float *pTriangleFanPoints[4]; //at most, one of our fans will have 5 points total, with the first point being stored separately as pStartPoint
		int iTriangleFanPointCount = 1; //the switch below creates the first for sure
		
		//figure out how to start the fan
		switch( iLineStates[pPoints[j]][pPoints[j+1]] )
		{
		case 0: //uncut
			pStartPoint = &vPoints[pPoints[j]].x;
			pTriangleFanPoints[0] = &vPoints[pPoints[j+1]].x;
			break;

		case 4: //second index was clipped
			pStartPoint = &vPoints[pPoints[j]].x;
			pTriangleFanPoints[0] = &vSplitPoints[pPoints[j]][pPoints[j+1]].x;
			break;

		case 3: //first index was clipped
			pStartPoint = &vSplitPoints[pPoints[j]][pPoints[j+1]].x;
			pTriangleFanPoints[0] = &vPoints[pPoints[j + 1]].x;
			break;

		default:
			Assert( false );
			break;
		};

		for( ++j; j != 3; ++j ) //add end points for the rest of the indices, we're assembling a triangle fan
		{
			switch( iLineStates[pPoints[j]][pPoints[j+1]] )
			{
			case 0: //uncut line, normal endpoint
				pTriangleFanPoints[iTriangleFanPointCount] = &vPoints[pPoints[j+1]].x;
				++iTriangleFanPointCount;
				break;

			case 2: //wholly cut line, no endpoint
				break;

			case 3: //first point is clipped, normal endpoint
				//special case, adds start and end point
				pTriangleFanPoints[iTriangleFanPointCount] = &vSplitPoints[pPoints[j]][pPoints[j+1]].x;
				++iTriangleFanPointCount;

				pTriangleFanPoints[iTriangleFanPointCount] = &vPoints[pPoints[j+1]].x;
				++iTriangleFanPointCount;
				break;

			case 4: //second point is clipped
				pTriangleFanPoints[iTriangleFanPointCount] = &vSplitPoints[pPoints[j]][pPoints[j+1]].x;
				++iTriangleFanPointCount;
				break;

			default:
				Assert( false );
				break;
			};
		}
		
		//special case endpoints, half-clipped lines have a connecting line between them and the next line (first line in this case)
		switch( iLineStates[pPoints[j]][pPoints[j+1]] )
		{
		case 3:
		case 4:
			pTriangleFanPoints[iTriangleFanPointCount] = &vSplitPoints[pPoints[j]][pPoints[j+1]].x;
			++iTriangleFanPointCount;
			break;
		};

		Assert( iTriangleFanPointCount <= 4 );

		//add the fan to the mesh
		int iLoopStop = iTriangleFanPointCount - 1;
		for( int k = 0; k != iLoopStop; ++k )
		{
			meshBuilder.Position3fv( pStartPoint );
#ifdef DEBUG_DRAWCLIPPEDDEPTHBOX
			float fHalfColors[3] = { fColors[i][0] * 0.5f, fColors[i][1] * 0.5f, fColors[i][2] * 0.5f };
			meshBuilder.Color3fv( fHalfColors );
#endif
			meshBuilder.AdvanceVertex();
			
			meshBuilder.Position3fv( pTriangleFanPoints[k] );
#ifdef DEBUG_DRAWCLIPPEDDEPTHBOX
			meshBuilder.Color3fv( fColors[i] );
#endif
			meshBuilder.AdvanceVertex();

			meshBuilder.Position3fv( pTriangleFanPoints[k+1] );
#ifdef DEBUG_DRAWCLIPPEDDEPTHBOX
			meshBuilder.Color3fv( fColors[i] );
#endif
			meshBuilder.AdvanceVertex();
		}
	}

	meshBuilder.End();
	pMesh->Draw();
	pRenderContext->Flush( false );
}

//-----------------------------------------------------------------------------
// Unified bit of draw code for opaque and translucent renderables
//-----------------------------------------------------------------------------
static inline void DrawRenderable( IClientRenderable *pEnt, int flags, const RenderableInstance_t &instance )
{
	float *pRenderClipPlane = NULL;
	if( r_entityclips.GetBool() )
		pRenderClipPlane = pEnt->GetRenderClipPlane();

	if( pRenderClipPlane )	
	{
		CMatRenderContextPtr pRenderContext( materials );
		if( !materials->UsingFastClipping() ) //do NOT change the fast clip plane mid-scene, depth problems result. Regular user clip planes are fine though
			pRenderContext->PushCustomClipPlane( pRenderClipPlane );
		else
			DrawClippedDepthBox( pEnt, pRenderClipPlane );
		Assert( view->GetCurrentlyDrawingEntity() == NULL );
		view->SetCurrentlyDrawingEntity( pEnt->GetIClientUnknown()->GetBaseEntity() );
		bool bBlockNormalDraw = false;
		if( !bBlockNormalDraw )
			pEnt->DrawModel( flags, instance );
		view->SetCurrentlyDrawingEntity( NULL );

		if( !materials->UsingFastClipping() )	
			pRenderContext->PopCustomClipPlane();
	}
	else
	{
		Assert( view->GetCurrentlyDrawingEntity() == NULL );
		view->SetCurrentlyDrawingEntity( pEnt->GetIClientUnknown()->GetBaseEntity() );
		bool bBlockNormalDraw = false;
		if( !bBlockNormalDraw )
			pEnt->DrawModel( flags, instance );
		view->SetCurrentlyDrawingEntity( NULL );
	}
}

//-----------------------------------------------------------------------------
// Draws all opaque renderables in leaves that were rendered
//-----------------------------------------------------------------------------
static inline void DrawOpaqueRenderable( IClientRenderable *pEnt, bool bTwoPass, bool bNoDecals )
{
	ASSERT_LOCAL_PLAYER_RESOLVABLE();
	float color[3];

	Assert( !IsSplitScreenSupported() || pEnt->ShouldDrawForSplitScreenUser( GET_ACTIVE_SPLITSCREEN_SLOT() ) );
	Assert( (pEnt->GetIClientUnknown() == NULL) || (pEnt->GetIClientUnknown()->GetIClientEntity() == NULL) || (pEnt->GetIClientUnknown()->GetIClientEntity()->IsBlurred() == false) );
	pEnt->GetColorModulation( color );
	render->SetColorModulation(	color );

	int flags = STUDIO_RENDER;
	if ( bTwoPass )
	{
		flags |= STUDIO_TWOPASS;
	}

	if ( bNoDecals )
	{
		flags |= STUDIO_SKIP_DECALS;
	}

	RenderableInstance_t instance;
	instance.m_nAlpha = 255;
	DrawRenderable( pEnt, flags, instance );
}

//-------------------------------------


static void SetupBonesOnBaseAnimating( C_BaseAnimating *&pBaseAnimating )
{
	pBaseAnimating->SetupBones( NULL, -1, -1, gpGlobals->curtime );
}


static void DrawOpaqueRenderables_DrawBrushModels( int nCount, CClientRenderablesList::CEntry **ppEntities, bool bNoDecals )
{
	for( int i = 0; i < nCount; ++i )
	{
		Assert( !ppEntities[i]->m_TwoPass );
		DrawOpaqueRenderable( ppEntities[i]->m_pRenderable, false, bNoDecals );
	}
}

static void DrawOpaqueRenderables_DrawStaticProps( int nCount, CClientRenderablesList::CEntry **ppEntities )
{
	if ( nCount == 0 )
		return;

	float one[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	render->SetColorModulation(	one );
	render->SetBlend( 1.0f );
	
	const int MAX_STATICS_PER_BATCH = 512;
	IClientRenderable *pStatics[ MAX_STATICS_PER_BATCH ];
	RenderableInstance_t pInstances[ MAX_STATICS_PER_BATCH ];
	
	int numScheduled = 0, numAvailable = MAX_STATICS_PER_BATCH;

	for( int i = 0; i < nCount; ++i )
	{
		CClientRenderablesList::CEntry *itEntity = ppEntities[i];
		if ( itEntity->m_pRenderable )
			NULL;
		else
			continue;

		pInstances[ numScheduled ] = itEntity->m_InstanceData;
		pStatics[ numScheduled ++ ] = itEntity->m_pRenderable;
		if ( -- numAvailable > 0 )
			continue; // place a hint for compiler to predict more common case in the loop
		
		staticpropmgr->DrawStaticProps( pStatics, pInstances, numScheduled, false, vcollide_wireframe.GetBool() );
		numScheduled = 0;
		numAvailable = MAX_STATICS_PER_BATCH;
	}
	
	if ( numScheduled )
		staticpropmgr->DrawStaticProps( pStatics, pInstances, numScheduled, false, vcollide_wireframe.GetBool() );
}

static void DrawOpaqueRenderables_Range( int nCount, CClientRenderablesList::CEntry **ppEntities, bool bNoDecals )
{
	for ( int i = 0; i < nCount; ++i )
	{
		CClientRenderablesList::CEntry *itEntity = ppEntities[i]; 
		if ( itEntity->m_pRenderable )
		{
			DrawOpaqueRenderable( itEntity->m_pRenderable, ( itEntity->m_TwoPass != 0 ), bNoDecals );
		}
	}
}

extern ConVar cl_modelfastpath;
extern ConVar cl_skipslowpath;
extern ConVar r_drawothermodels;
static void	DrawOpaqueRenderables_ModelRenderables( int nCount, ModelRenderSystemData_t* pModelRenderables )
{
	g_pModelRenderSystem->DrawModels( pModelRenderables, nCount, MODEL_RENDER_MODE_NORMAL );
}

static void	DrawOpaqueRenderables_NPCs( int nCount, CClientRenderablesList::CEntry **ppEntities, bool bNoDecals )
{
	DrawOpaqueRenderables_Range( nCount, ppEntities, bNoDecals );
}

//-----------------------------------------------------------------------------
// Renders all translucent entities in the render list
//-----------------------------------------------------------------------------
static inline void DrawTranslucentRenderable( IClientRenderable *pEnt, const RenderableInstance_t &instance, bool twoPass )
{
	ASSERT_LOCAL_PLAYER_RESOLVABLE();

	Assert( !IsSplitScreenSupported() || pEnt->ShouldDrawForSplitScreenUser( GET_ACTIVE_SPLITSCREEN_SLOT() ) );

	// Renderable list building should already have caught this
	Assert( instance.m_nAlpha > 0 );

	// Determine blending amount and tell engine
	float blend = (float)( instance.m_nAlpha / 255.0f );

	// Tell engine
	render->SetBlend( blend );

	float color[3];
	pEnt->GetColorModulation( color );
	render->SetColorModulation(	color );

	int flags = STUDIO_RENDER | STUDIO_TRANSPARENCY;
	if ( twoPass )
		flags |= STUDIO_TWOPASS;

	DrawRenderable( pEnt, flags, instance );
}

void CBaseWorldViewDeferred::DrawWorldDeferred( float waterZAdjust )
{
	DrawWorld( waterZAdjust );
}

void CBaseWorldViewDeferred::DrawOpaqueRenderablesDeferred( bool bNoDecals )
{
	VPROF("CViewRender::DrawOpaqueRenderables" );

	if( !r_drawopaquerenderables.GetBool() )
		return;

	if( !m_pMainView->ShouldDrawEntities() )
		return;

	render->SetBlend( 1 );

	//
	// Prepare to iterate over all leaves that were visible, and draw opaque things in them.	
	//
	RopeManager()->ResetRenderCache();
	g_pParticleSystemMgr->ResetRenderCache();

	// Categorize models by type
	int nOpaqueRenderableCount = m_pRenderablesList->m_RenderGroupCounts[RENDER_GROUP_OPAQUE];
	CUtlVector< CClientRenderablesList::CEntry* > brushModels( (CClientRenderablesList::CEntry **)stackalloc( nOpaqueRenderableCount * sizeof( CClientRenderablesList::CEntry* ) ), nOpaqueRenderableCount );
	CUtlVector< CClientRenderablesList::CEntry* > staticProps( (CClientRenderablesList::CEntry **)stackalloc( nOpaqueRenderableCount * sizeof( CClientRenderablesList::CEntry* ) ), nOpaqueRenderableCount );
	CUtlVector< CClientRenderablesList::CEntry* > otherRenderables( (CClientRenderablesList::CEntry **)stackalloc( nOpaqueRenderableCount * sizeof( CClientRenderablesList::CEntry* ) ), nOpaqueRenderableCount );
	CClientRenderablesList::CEntry *pOpaqueList = m_pRenderablesList->m_RenderGroups[RENDER_GROUP_OPAQUE];
	for ( int i = 0; i < nOpaqueRenderableCount; ++i )
	{
		switch( pOpaqueList[i].m_nModelType )
		{
		case RENDERABLE_MODEL_BRUSH:		brushModels.AddToTail( &pOpaqueList[i] ); break; 
		case RENDERABLE_MODEL_STATIC_PROP:	staticProps.AddToTail( &pOpaqueList[i] ); break; 
		default:							otherRenderables.AddToTail( &pOpaqueList[i] ); break; 
		}
	}

	//
	// First do the brush models
	//
	DrawOpaqueRenderables_DrawBrushModels( brushModels.Count(), brushModels.Base(), bNoDecals );

	// Move all static props to modelrendersystem
	bool bUseFastPath = ( cl_modelfastpath.GetInt() != 0 );

	//
	// Sort everything that's not a static prop
	//
	int nStaticPropCount = staticProps.Count();
	int numOpaqueEnts = otherRenderables.Count();
	CUtlVector< CClientRenderablesList::CEntry* > arrRenderEntsNpcsFirst( (CClientRenderablesList::CEntry **)stackalloc( numOpaqueEnts * sizeof( CClientRenderablesList::CEntry ) ), numOpaqueEnts );
	CUtlVector< ModelRenderSystemData_t > arrModelRenderables( (ModelRenderSystemData_t *)stackalloc( ( numOpaqueEnts + nStaticPropCount ) * sizeof( ModelRenderSystemData_t ) ), numOpaqueEnts + nStaticPropCount );

	// Queue up RENDER_GROUP_OPAQUE_ENTITY entities to be rendered later.
	CClientRenderablesList::CEntry *itEntity;
	if( r_drawothermodels.GetBool() )
	{
		for ( int i = 0; i < numOpaqueEnts; ++i )
		{
			itEntity = otherRenderables[i];
			if ( !itEntity->m_pRenderable )
				continue;

			IClientUnknown *pUnknown = itEntity->m_pRenderable->GetIClientUnknown();
			IClientModelRenderable *pModelRenderable = pUnknown->GetClientModelRenderable();
			C_BaseEntity *pEntity = pUnknown->GetBaseEntity();

			// FIXME: Strangely, some static props are in the non-static prop bucket
			// which is what the last case in this if statement is for
			if ( bUseFastPath && pModelRenderable )
			{
				ModelRenderSystemData_t data;
				data.m_pRenderable = itEntity->m_pRenderable;
				data.m_pModelRenderable = pModelRenderable;
				data.m_InstanceData = itEntity->m_InstanceData;
				arrModelRenderables.AddToTail( data );
				otherRenderables.FastRemove( i );
				--i; --numOpaqueEnts;
				continue;
			}

			if ( !pEntity )
				continue;

			if ( pEntity->IsNPC() )
			{
				arrRenderEntsNpcsFirst.AddToTail( itEntity );
				otherRenderables.FastRemove( i );
				--i; --numOpaqueEnts;
				continue;
			}
		}
	}

	// Queue up the static props to be rendered later.
	for ( int i = 0; i < nStaticPropCount; ++i )
	{
		itEntity = staticProps[i];
		if ( !itEntity->m_pRenderable )
			continue;

		IClientUnknown *pUnknown = itEntity->m_pRenderable->GetIClientUnknown();
		IClientModelRenderable *pModelRenderable = pUnknown->GetClientModelRenderable();
		if ( !bUseFastPath || !pModelRenderable )
			continue;

		ModelRenderSystemData_t data;
		data.m_pRenderable = itEntity->m_pRenderable;
		data.m_pModelRenderable = pModelRenderable;
		data.m_InstanceData = itEntity->m_InstanceData;
		arrModelRenderables.AddToTail( data );

		staticProps.FastRemove( i );
		--i; --nStaticPropCount;
	}

	//
	// Draw model renderables now (ie. models that use the fast path)
	//					 
	DrawOpaqueRenderables_ModelRenderables( arrModelRenderables.Count(), arrModelRenderables.Base() );

	// Turn off z pass here. Don't want non-fastpath models with potentially large dynamic VB requirements overwrite
	// stuff in the dynamic VB ringbuffer. We're calling End360ZPass again in DrawExecute, but that's not a problem.
	// Begin360ZPass/End360ZPass don't have to be matched exactly.
	End360ZPass();

	//
	// Draw static props + opaque entities that aren't using the fast path.
	//
	DrawOpaqueRenderables_Range( otherRenderables.Count(), otherRenderables.Base(), bNoDecals );
	DrawOpaqueRenderables_DrawStaticProps( staticProps.Count(), staticProps.Base() );

	//
	// Draw NPCs now
	//
	DrawOpaqueRenderables_NPCs( arrRenderEntsNpcsFirst.Count(), arrRenderEntsNpcsFirst.Base(), bNoDecals );

	//
	// Ropes and particles
	//
	RopeManager()->DrawRenderCache( false );
	g_pParticleSystemMgr->DrawRenderCache( false );
}



static ConVar r_unlimitedrefract( "r_unlimitedrefract", "0" );
extern ConVar cl_tlucfastpath;
extern ConVar cl_colorfastpath;


//-----------------------------------------------------------------------------
// Standard 3d skybox view
//-----------------------------------------------------------------------------
SkyboxVisibility_t CSkyboxViewDeferred::ComputeSkyboxVisibility()
{
	if ( ( enginetrace->GetPointContents( origin ) & CONTENTS_SOLID ) != 0 )
		return SKYBOX_NOT_VISIBLE;

	return engine->IsSkyboxVisibleFromPoint( origin );
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool CSkyboxViewDeferred::GetSkyboxFogEnable()
{
	C_BasePlayer *pbp = C_BasePlayer::GetLocalPlayer();
	if( !pbp )
	{
		return false;
	}
	CPlayerLocalData	*local		= &pbp->m_Local;

	static ConVarRef fog_override( "fog_override" );
	if( fog_override.GetInt() )
	{
		if( fog_enableskybox.GetInt() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return !!local->m_skybox3d.fog.enable;
	}
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CSkyboxViewDeferred::Enable3dSkyboxFog( void )
{
	C_BasePlayer *pbp = C_BasePlayer::GetLocalPlayer();
	if( !pbp )
	{
		return;
	}
	CPlayerLocalData	*local		= &pbp->m_Local;

	CMatRenderContextPtr pRenderContext( materials );

	if( GetSkyboxFogEnable() )
	{
		float fogColor[3];
		GetSkyboxFogColor( fogColor );
		float scale = 1.0f;
		if ( local->m_skybox3d.scale > 0.0f )
		{
			scale = 1.0f / local->m_skybox3d.scale;
		}
		pRenderContext->FogMode( MATERIAL_FOG_LINEAR );
		pRenderContext->FogColor3fv( fogColor );
		pRenderContext->FogStart( GetSkyboxFogStart() * scale );
		pRenderContext->FogEnd( GetSkyboxFogEnd() * scale );
		pRenderContext->FogMaxDensity( GetSkyboxFogMaxDensity() );
	}
	else
	{
		pRenderContext->FogMode( MATERIAL_FOG_NONE );
	}
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
sky3dparams_t *CSkyboxViewDeferred::PreRender3dSkyboxWorld( SkyboxVisibility_t nSkyboxVisible )
{
	if ( ( nSkyboxVisible != SKYBOX_3DSKYBOX_VISIBLE ) && r_3dsky.GetInt() != 2 )
		return NULL;

	// render the 3D skybox
	if ( !r_3dsky.GetInt() )
		return NULL;

	C_BasePlayer *pbp = C_BasePlayer::GetLocalPlayer();

	// No local player object yet...
	if ( !pbp )
		return NULL;

	CPlayerLocalData* local = &pbp->m_Local;
	if ( local->m_skybox3d.area == 255 )
		return NULL;

	return &local->m_skybox3d;
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CSkyboxViewDeferred::DrawInternal( view_id_t iSkyBoxViewID, bool bInvokePreAndPostRender, ITexture *pRenderTarget )
{
	bInvokePreAndPostRender = !m_bGBufferPass;

	if ( m_bGBufferPass )
	{
		m_DrawFlags |= DF_SKIP_WORLD_DECALS_AND_OVERLAYS;

#if DEFCFG_DEFERRED_SHADING
		m_DrawFlags |= DF_DRAWSKYBOX;
#endif
	}

	unsigned char **areabits = render->GetAreaBits();
	unsigned char *savebits;
	unsigned char tmpbits[ 32 ];
	savebits = *areabits;
	memset( tmpbits, 0, sizeof(tmpbits) );

	// set the sky area bit
	tmpbits[m_pSky3dParams->area>>3] |= 1 << (m_pSky3dParams->area&7);

	*areabits = tmpbits;

	// if you can get really close to the skybox geometry it's possible that you'll be able to clip into it
	// with this near plane.  If so, move it in a bit.  It's at 2.0 to give us more precision.  That means you 
	// need to keep the eye position at least 2 * scale away from the geometry in the skybox
	zNear = 2.0;
	zFar = 10000.0f; //MAX_TRACE_LENGTH;

	float skyScale = 1.0f;
	// scale origin by sky scale and translate to sky origin
	{
		skyScale = (m_pSky3dParams->scale > 0) ? m_pSky3dParams->scale : 1.0f;
		float scale = 1.0f / skyScale;

		Vector vSkyOrigin = m_pSky3dParams->origin;
		VectorScale( origin, scale, origin );
		VectorAdd( origin, vSkyOrigin, origin );

		if( m_bCustomViewMatrix )
		{
			Vector vTransformedSkyOrigin;
			VectorRotate( vSkyOrigin, m_matCustomViewMatrix, vTransformedSkyOrigin ); //Rotate instead of transform because we haven't scale the existing offset yet

			//scale existing translation, and tack on the skybox offset (subtract because it's a view matrix)
			m_matCustomViewMatrix.m_flMatVal[0][3] = (m_matCustomViewMatrix.m_flMatVal[0][3] * scale) - vTransformedSkyOrigin.x;
			m_matCustomViewMatrix.m_flMatVal[1][3] = (m_matCustomViewMatrix.m_flMatVal[1][3] * scale) - vTransformedSkyOrigin.y;
			m_matCustomViewMatrix.m_flMatVal[2][3] = (m_matCustomViewMatrix.m_flMatVal[2][3] * scale) - vTransformedSkyOrigin.z;
		}
	}

	if ( !m_bGBufferPass )
		Enable3dSkyboxFog();

	// BUGBUG: Fix this!!!  We shouldn't need to call setup vis for the sky if we're connecting
	// the areas.  We'd have to mark all the clusters in the skybox area in the PVS of any 
	// cluster with sky.  Then we could just connect the areas to do our vis.
	//m_bOverrideVisOrigin could hose us here, so call direct
	render->ViewSetupVis( false, 1, &m_pSky3dParams->origin.Get() );
	render->Push3DView( (*this), m_ClearFlags, pRenderTarget, GetFrustum() );

	if ( m_bGBufferPass )
		PushGBuffer( true, skyScale );
	else
		PushComposite();

	// Store off view origin and angles
	SetupCurrentView( origin, angles, iSkyBoxViewID );

#if defined( _X360 )
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->PushVertexShaderGPRAllocation( 32 );
	pRenderContext.SafeRelease();
#endif

	// Invoke pre-render methods
	if ( bInvokePreAndPostRender )
	{
		IGameSystem::PreRenderAllSystems();
	}

	BuildWorldRenderLists( true, -1, ShouldCacheLists() );

	BuildRenderableRenderLists( m_bGBufferPass ? VIEW_SHADOW_DEPTH_TEXTURE : iSkyBoxViewID );

	DrawWorld( 0.0f );

	// Iterate over all leaves and render objects in those leaves
	DrawOpaqueRenderables( false );

	if ( !m_bGBufferPass )
	{
		// Iterate over all leaves and render objects in those leaves
		DrawTranslucentRenderables( true, false );
		DrawNoZBufferTranslucentRenderables();
	}

	if ( !m_bGBufferPass )
	{
		m_pMainView->DisableFog();

		CGlowOverlay::UpdateSkyOverlays( zFar, m_bCacheFullSceneState );

		PixelVisibility_EndCurrentView();
	}

	// restore old area bits
	*areabits = savebits;

	// Invoke post-render methods
	if( bInvokePreAndPostRender )
	{
		IGameSystem::PostRenderAllSystems();
		FinishCurrentView();
	}

	if ( m_bGBufferPass )
		PopGBuffer();
	else
		PopComposite();

	render->PopView( GetFrustum() );

#if defined( _X360 )
	pRenderContext.GetFrom( materials );
	pRenderContext->PopVertexShaderGPRAllocation();
#endif
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool CSkyboxViewDeferred::Setup( const CViewSetup &view, bool bGBuffer, SkyboxVisibility_t *pSkyboxVisible )
{
	BaseClass::Setup( view );

	// The skybox might not be visible from here
	*pSkyboxVisible = ComputeSkyboxVisibility();
	m_pSky3dParams = PreRender3dSkyboxWorld( *pSkyboxVisible );

	if ( !m_pSky3dParams )
	{
		return false;
	}

	m_bGBufferPass = bGBuffer;
	// At this point, we've cleared everything we need to clear
	// The next path will need to clear depth, though.
	m_ClearFlags = VIEW_CLEAR_DEPTH; //*pClearFlags;
	//*pClearFlags &= ~( VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH | VIEW_CLEAR_STENCIL | VIEW_CLEAR_FULL_TARGET );
	//*pClearFlags |= VIEW_CLEAR_DEPTH; // Need to clear depth after rednering the skybox

	m_DrawFlags = DF_RENDER_UNDERWATER | DF_RENDER_ABOVEWATER | DF_RENDER_WATER;
	if( !m_bGBufferPass && r_skybox.GetBool() )
	{
		m_DrawFlags |= DF_DRAWSKYBOX;
	}

	return true;
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CSkyboxViewDeferred::Draw()
{
	VPROF_BUDGET( "CViewRender::Draw3dSkyboxworld", "3D Skybox" );

	DrawInternal();
}



void CGBufferView::Setup( const CViewSetup &view, bool bDrewSkybox )
{
	m_fogInfo.m_bEyeInFogVolume = false;
	m_bDrewSkybox = bDrewSkybox;

	BaseClass::Setup( view );
	m_bDrawWorldNormal = true;

	m_ClearFlags = 0;
	m_DrawFlags = DF_DRAW_ENTITITES;

	m_DrawFlags |= DF_RENDER_UNDERWATER | DF_RENDER_ABOVEWATER;

#if DEFCFG_DEFERRED_SHADING
	if ( !bDrewSkybox )
		m_DrawFlags |= DF_DRAWSKYBOX;
#endif
}

void CGBufferView::Draw()
{
	VPROF( "CViewRender::ViewDrawScene_NoWater" );

	CMatRenderContextPtr pRenderContext( materials );
	PIXEVENT( pRenderContext, "CSimpleWorldViewDeferred::Draw" );

#if defined( _X360 )
	pRenderContext->PushVertexShaderGPRAllocation( 32 ); //lean toward pixel shader threads
#endif

	SetupCurrentView( origin, angles, VIEW_DEFERRED_GBUFFER );

	DrawSetup( 0, m_DrawFlags, 0 );

	const bool bOptimizedGbuffer = DEFCFG_DEFERRED_SHADING == 0;
	DrawExecute( 0, CurrentViewID(), 0, bOptimizedGbuffer );

	pRenderContext.GetFrom( materials );
	pRenderContext->ClearColor4ub( 0, 0, 0, 255 );

#if defined( _X360 )
	pRenderContext->PopVertexShaderGPRAllocation();
#endif
}

void CGBufferView::PushView( float waterHeight )
{
	PushGBuffer( !m_bDrewSkybox );
}

void CGBufferView::PopView()
{
	PopGBuffer();
}

void CGBufferView::PushGBuffer( bool bInitial, float zScale, bool bClearDepth )
{
	ITexture *pNormals = GetDefRT_Normals();
	ITexture *pDepth = GetDefRT_Depth();

	CMatRenderContextPtr pRenderContext( materials );

	pRenderContext->ClearColor4ub( 0, 0, 0, 0 );

	if ( bInitial )
	{
		pRenderContext->PushRenderTargetAndViewport( pDepth );
		pRenderContext->ClearBuffers( true, false );
		pRenderContext->PopRenderTargetAndViewport();
	}

#if DEFCFG_DEFERRED_SHADING == 1
	pRenderContext->PushRenderTargetAndViewport( GetDefRT_Albedo() );
#else
	pRenderContext->PushRenderTargetAndViewport( pNormals );
#endif

	if ( bClearDepth )
		pRenderContext->ClearBuffers( false, true );

	pRenderContext->SetRenderTargetEx( 1, pDepth );

#if DEFCFG_DEFERRED_SHADING == 1
	pRenderContext->SetRenderTargetEx( 2, pNormals );
	pRenderContext->SetRenderTargetEx( 3, GetDefRT_Specular() );
#endif

	pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_RENDER_STAGE,
		DEFERRED_RENDER_STAGE_GBUFFER );

	struct defData_setZScale
	{
	public:
		float zScale;

		static void Fire( defData_setZScale d )
		{
			GetDeferredExt()->CommitZScale( d.zScale );
		};
	};

	defData_setZScale data;
	data.zScale = zScale;
	QUEUE_FIRE( defData_setZScale, Fire, data );
}

void CGBufferView::PopGBuffer()
{
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_RENDER_STAGE,
		DEFERRED_RENDER_STAGE_INVALID );

	pRenderContext->PopRenderTargetAndViewport();
}




//-----------------------------------------------------------------------------
// Pops a water render target
//-----------------------------------------------------------------------------
bool CBaseWorldViewDeferred::AdjustView( float waterHeight )
{
	if( m_DrawFlags & DF_RENDER_REFRACTION )
	{
		ITexture *pTexture = GetWaterRefractionTexture();

		// Use the aspect ratio of the main view! So, don't recompute it here
		x = y = 0;
		width = pTexture->GetActualWidth();
		height = pTexture->GetActualHeight();

		return true;
	}

	if( m_DrawFlags & DF_RENDER_REFLECTION )
	{
		ITexture *pTexture = GetWaterReflectionTexture();

		// Use the aspect ratio of the main view! So, don't recompute it here
		x = y = 0;
		width = pTexture->GetActualWidth();
		height = pTexture->GetActualHeight();
		angles[0] = -angles[0];
		angles[2] = -angles[2];
		origin[2] -= 2.0f * ( origin[2] - (waterHeight));
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Pops a water render target
//-----------------------------------------------------------------------------
void CBaseWorldViewDeferred::PushView( float waterHeight )
{
	float spread = 2.0f;
	if( m_DrawFlags & DF_FUDGE_UP )
	{
		waterHeight += spread;
	}
	else
	{
		waterHeight -= spread;
	}

	MaterialHeightClipMode_t clipMode = MATERIAL_HEIGHTCLIPMODE_DISABLE;
	if ( ( m_DrawFlags & DF_CLIP_Z ) && mat_clipz.GetBool() )
	{
		if( m_DrawFlags & DF_CLIP_BELOW )
		{
			clipMode = MATERIAL_HEIGHTCLIPMODE_RENDER_ABOVE_HEIGHT;
		}
		else
		{
			clipMode = MATERIAL_HEIGHTCLIPMODE_RENDER_BELOW_HEIGHT;
		}
	}

	CMatRenderContextPtr pRenderContext( materials );

	if( m_DrawFlags & DF_RENDER_REFRACTION )
	{
		pRenderContext->SetFogZ( waterHeight );
		pRenderContext->SetHeightClipZ( waterHeight );
		pRenderContext->SetHeightClipMode( clipMode );

		// Have to re-set up the view since we reset the size
		render->Push3DView( *this, m_ClearFlags, GetWaterRefractionTexture(), GetFrustum() );

		return;
	}

	if( m_DrawFlags & DF_RENDER_REFLECTION )
	{
		ITexture *pTexture = GetWaterReflectionTexture();

		pRenderContext->SetFogZ( waterHeight );

		bool bSoftwareUserClipPlane = g_pMaterialSystemHardwareConfig->UseFastClipping();
		if( bSoftwareUserClipPlane && ( origin[2] > waterHeight - r_eyewaterepsilon.GetFloat() ) )
		{
			waterHeight = origin[2] + r_eyewaterepsilon.GetFloat();
		}

		pRenderContext->SetHeightClipZ( waterHeight );
		pRenderContext->SetHeightClipMode( clipMode );

		render->Push3DView( *this, m_ClearFlags, pTexture, GetFrustum() );
		return;
	}

	if ( m_ClearFlags & ( VIEW_CLEAR_DEPTH | VIEW_CLEAR_COLOR | VIEW_CLEAR_STENCIL ) )
	{
		if ( m_ClearFlags & VIEW_CLEAR_OBEY_STENCIL )
		{
			pRenderContext->ClearBuffersObeyStencil( ( m_ClearFlags & VIEW_CLEAR_COLOR ) ? true : false, ( m_ClearFlags & VIEW_CLEAR_DEPTH ) ? true : false );
		}
		else
		{
			pRenderContext->ClearBuffers( ( m_ClearFlags & VIEW_CLEAR_COLOR ) ? true : false, ( m_ClearFlags & VIEW_CLEAR_DEPTH ) ? true : false, ( m_ClearFlags & VIEW_CLEAR_STENCIL ) ? true : false );
		}
	}

	pRenderContext->SetHeightClipMode( clipMode );
	if ( clipMode != MATERIAL_HEIGHTCLIPMODE_DISABLE )
	{   
		pRenderContext->SetHeightClipZ( waterHeight );
	}
}


//-----------------------------------------------------------------------------
// Pops a water render target
//-----------------------------------------------------------------------------
void CBaseWorldViewDeferred::PopView()
{
	CMatRenderContextPtr pRenderContext( materials );

	pRenderContext->SetHeightClipMode( MATERIAL_HEIGHTCLIPMODE_DISABLE );
	if( m_DrawFlags & (DF_RENDER_REFRACTION | DF_RENDER_REFLECTION) )
	{
		if ( IsX360() )
		{
			// these renders paths used their surfaces, so blit their results
			if ( m_DrawFlags & DF_RENDER_REFRACTION )
			{
				pRenderContext->CopyRenderTargetToTextureEx( GetWaterRefractionTexture(), NULL, NULL );
			}
			if ( m_DrawFlags & DF_RENDER_REFLECTION )
			{
				pRenderContext->CopyRenderTargetToTextureEx( GetWaterReflectionTexture(), NULL, NULL );
			}
		}

		render->PopView( GetFrustum() );
	}
}


//-----------------------------------------------------------------------------
// Draws the world + entities
//-----------------------------------------------------------------------------
void CBaseWorldViewDeferred::DrawSetup( float waterHeight, int nSetupFlags, float waterZAdjust, int iForceViewLeaf, bool bShadowDepth )
{
	int savedViewID = g_CurrentViewID;
	g_CurrentViewID = bShadowDepth ? VIEW_DEFERRED_SHADOW : VIEW_MAIN;

	bool bViewChanged = AdjustView( waterHeight );

	if ( bViewChanged )
	{
		render->Push3DView( *this, 0, NULL, GetFrustum() );
	}

	bool bDrawEntities = ( nSetupFlags & DF_DRAW_ENTITITES ) != 0;
	bool bDrawReflection = ( nSetupFlags & DF_RENDER_REFLECTION ) != 0;
	BuildWorldRenderLists( bDrawEntities, iForceViewLeaf, ShouldCacheLists(), false, bDrawReflection ? &waterHeight : NULL );

	PruneWorldListInfo();

	if ( bDrawEntities )
	{
		bool bOptimized = bShadowDepth || savedViewID == VIEW_DEFERRED_GBUFFER;
		BuildRenderableRenderLists( bOptimized ? VIEW_SHADOW_DEPTH_TEXTURE : savedViewID );
	}

	if ( bViewChanged )
	{
		render->PopView( GetFrustum() );
	}

	g_CurrentViewID = savedViewID;
}


void CBaseWorldViewDeferred::DrawExecute( float waterHeight, view_id_t viewID, float waterZAdjust, bool bShadowDepth )
{
	// @MULTICORE (toml 8/16/2006): rethink how, where, and when this is done...
	//g_pClientShadowMgr->ComputeShadowTextures( *this, m_pWorldListInfo->m_LeafCount, m_pWorldListInfo->m_pLeafDataList );

	// Make sure sound doesn't stutter
	engine->Sound_ExtraUpdate();

	int savedViewID = g_CurrentViewID;
	g_CurrentViewID = viewID;

	// Update our render view flags.
	int iDrawFlagsBackup = m_DrawFlags;
	m_DrawFlags |= m_pMainView->GetBaseDrawFlags();

	PushView( waterHeight );

	CMatRenderContextPtr pRenderContext( materials );

#if defined( _X360 )
	pRenderContext->PushVertexShaderGPRAllocation( 32 );
#endif

	ITexture *pSaveFrameBufferCopyTexture = pRenderContext->GetFrameBufferCopyTexture( 0 );
	pRenderContext->SetFrameBufferCopyTexture( GetPowerOfTwoFrameBufferTexture() );
	pRenderContext.SafeRelease();


	Begin360ZPass();
	m_DrawFlags |= DF_SKIP_WORLD_DECALS_AND_OVERLAYS;
	DrawWorldDeferred( waterZAdjust );
	m_DrawFlags &= ~DF_SKIP_WORLD_DECALS_AND_OVERLAYS;
	if ( m_DrawFlags & DF_DRAW_ENTITITES )
	{
		DrawOpaqueRenderablesDeferred( m_bDrawWorldNormal );
	}
	End360ZPass();		// DrawOpaqueRenderables currently already calls End360ZPass. No harm in calling it again to make sure we're always ending it

	// Only draw decals on opaque surfaces after now. Benefit is two-fold: Early Z benefits on PC, and
	// we're pulling out stuff that uses the dynamic VB from the 360 Z pass
	// (which can lead to rendering corruption if we overflow the dyn. VB ring buffer).
	if ( !bShadowDepth )
	{
		m_DrawFlags |= DF_SKIP_WORLD;
		DrawWorldDeferred( waterZAdjust );
		m_DrawFlags &= ~DF_SKIP_WORLD;
	}
		
	if ( !m_bDrawWorldNormal )
	{
		if ( m_DrawFlags & DF_DRAW_ENTITITES )
		{
			DrawTranslucentRenderables( false, false );
			if ( !bShadowDepth )
				DrawNoZBufferTranslucentRenderables();
		}
		else
		{
			// Draw translucent world brushes only, no entities
			DrawTranslucentWorldInLeaves( false );
		}
	}

	pRenderContext.GetFrom( materials );
	pRenderContext->SetFrameBufferCopyTexture( pSaveFrameBufferCopyTexture );
	PopView();

	m_DrawFlags = iDrawFlagsBackup;

	g_CurrentViewID = savedViewID;

#if defined( _X360 )
	pRenderContext->PopVertexShaderGPRAllocation();
#endif
}

void CBaseWorldViewDeferred::PushComposite()
{
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_RENDER_STAGE,
		DEFERRED_RENDER_STAGE_COMPOSITION );
}

void CBaseWorldViewDeferred::PopComposite()
{
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_RENDER_STAGE,
		DEFERRED_RENDER_STAGE_INVALID );
}

//-----------------------------------------------------------------------------
// Draws the scene when there's no water or only cheap water
//-----------------------------------------------------------------------------
void CSimpleWorldViewDeferred::Setup( const CViewSetup &view, int nClearFlags, bool bDrawSkybox,
	const VisibleFogVolumeInfo_t &fogInfo, const WaterRenderInfo_t &waterInfo, ViewCustomVisibility_t *pCustomVisibility )
{
	BaseClass::Setup( view );

	m_ClearFlags = nClearFlags;
	m_DrawFlags = DF_DRAW_ENTITITES;

	if ( !waterInfo.m_bOpaqueWater )
	{
		m_DrawFlags |= DF_RENDER_UNDERWATER | DF_RENDER_ABOVEWATER;
	}
	else
	{
		bool bViewIntersectsWater = DoesViewPlaneIntersectWater( fogInfo.m_flWaterHeight, fogInfo.m_nVisibleFogVolume );
		if( bViewIntersectsWater )
		{
			// have to draw both sides if we can see both.
			m_DrawFlags |= DF_RENDER_UNDERWATER | DF_RENDER_ABOVEWATER;
		}
		else if ( fogInfo.m_bEyeInFogVolume )
		{
			m_DrawFlags |= DF_RENDER_UNDERWATER;
		}
		else
		{
			m_DrawFlags |= DF_RENDER_ABOVEWATER;
		}
	}
	if ( waterInfo.m_bDrawWaterSurface )
	{
		m_DrawFlags |= DF_RENDER_WATER;
	}

	if ( !fogInfo.m_bEyeInFogVolume && bDrawSkybox )
	{
		m_DrawFlags |= DF_DRAWSKYBOX;
	}

	m_pCustomVisibility = pCustomVisibility;
	m_fogInfo = fogInfo;
}

//-----------------------------------------------------------------------------
// Draws the scene when there's no water or only cheap water
//-----------------------------------------------------------------------------
void CSimpleWorldViewDeferred::Draw()
{
	VPROF( "CViewRender::ViewDrawScene_NoWater" );

	CMatRenderContextPtr pRenderContext( materials );
	PIXEVENT( pRenderContext, "CSimpleWorldViewDeferred::Draw" );

#if defined( _X360 )
	pRenderContext->PushVertexShaderGPRAllocation( 32 ); //lean toward pixel shader threads
#endif
	pRenderContext.SafeRelease();

	PushComposite();

	DrawSetup( 0, m_DrawFlags, 0 );

	if ( !m_fogInfo.m_bEyeInFogVolume )
	{
		EnableWorldFog();
	}
	else
	{
		m_ClearFlags |= VIEW_CLEAR_COLOR;

		SetFogVolumeState( m_fogInfo, false );

		pRenderContext.GetFrom( materials );

		unsigned char ucFogColor[3];
		pRenderContext->GetFogColor( ucFogColor );
		pRenderContext->ClearColor4ub( ucFogColor[0], ucFogColor[1], ucFogColor[2], 255 );
	}

	pRenderContext.SafeRelease();

	DrawExecute( 0, CurrentViewID(), 0 );

	PopComposite();

	pRenderContext.GetFrom( materials );
	pRenderContext->ClearColor4ub( 0, 0, 0, 255 );

#if defined( _X360 )
	pRenderContext->PopVertexShaderGPRAllocation();
#endif
}


void CPostLightingView::Setup( const CViewSetup &view )
{
	m_fogInfo.m_bEyeInFogVolume = false;

	BaseClass::Setup( view );

	m_ClearFlags = 0;

	m_DrawFlags = DF_DRAW_ENTITITES | DF_SKIP_WORLD_DECALS_AND_OVERLAYS;
	m_DrawFlags |= DF_RENDER_UNDERWATER | DF_RENDER_ABOVEWATER;
}

void CPostLightingView::Draw()
{
	VPROF( "CViewRender::ViewDrawScene_NoWater" );

	CMatRenderContextPtr pRenderContext( materials );
	PIXEVENT( pRenderContext, "CSimpleWorldViewDeferred::Draw" );

#if defined( _X360 )
	pRenderContext->PushVertexShaderGPRAllocation( 32 ); //lean toward pixel shader threads
#endif

	ITexture *pTexAlbedo = GetDefRT_Albedo();
	pRenderContext->CopyRenderTargetToTexture( pTexAlbedo );
	pRenderContext->PushRenderTargetAndViewport( pTexAlbedo );
	pRenderContext.SafeRelease();
	PushComposite();

	SetupCurrentView( origin, angles, VIEW_MAIN );

	DrawSetup( 0, m_DrawFlags, 0 );

	DrawExecute( 0, CurrentViewID(), 0, false );

	PopComposite();

	pRenderContext.GetFrom( materials );
	pRenderContext->PopRenderTargetAndViewport();

#if defined( _X360 )
	pRenderContext->PopVertexShaderGPRAllocation();
#endif
}

void CPostLightingView::PushView( float waterHeight )
{
	//PushGBuffer( !m_bDrewSkybox );
	BaseClass::PushView( waterHeight );
}

void CPostLightingView::PopView()
{
	BaseClass::PopView();
}

void CPostLightingView::DrawWorldDeferred( float waterZAdjust )
{
#if 0
	int iOldDrawFlags = m_DrawFlags;

	m_DrawFlags &= ~DF_DRAW_ENTITITES;
	m_DrawFlags &= ~DF_RENDER_UNDERWATER;
	m_DrawFlags &= ~DF_RENDER_ABOVEWATER;
	m_DrawFlags &= ~DF_DRAW_ENTITITES;

	BaseClass::DrawWorldDeferred( waterZAdjust );

	m_DrawFlags = iOldDrawFlags;
#endif
}

void CPostLightingView::DrawOpaqueRenderablesDeferred( bool bNoDecals )
{
}

void CPostLightingView::PushDeferredShadingFrameBuffer()
{
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->PushRenderTargetAndViewport( GetDefRT_Albedo() );
}

void CPostLightingView::PopDeferredShadingFrameBuffer()
{
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->PopRenderTargetAndViewport();
}

void CBaseShadowView::Setup( const CViewSetup &view, ITexture *pDepthTexture, ITexture *pDummyTexture )
{
	m_pDepthTexture = pDepthTexture;
	m_pDummyTexture = pDummyTexture;

	BaseClass::Setup( view );

	m_bDrawWorldNormal = true;

	m_DrawFlags = DF_DRAW_ENTITITES | DF_RENDER_UNDERWATER | DF_RENDER_ABOVEWATER;
	m_ClearFlags = 0;

	CalcShadowView();

	m_pCustomVisibility = &shadowVis;
	shadowVis.AddVisOrigin( origin );
}

void CBaseShadowView::SetupRadiosityTargets( ITexture *pAlbedoTexture, ITexture *pNormalTexture )
{
	m_pRadAlbedoTexture = pAlbedoTexture;
	m_pRadNormalTexture = pNormalTexture;
}

void CBaseShadowView::Draw()
{
	int oldViewID = g_CurrentViewID;
	SetupCurrentView( origin, angles, VIEW_DEFERRED_SHADOW );

	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_RENDER_STAGE,
		DEFERRED_RENDER_STAGE_SHADOWPASS );
	pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_SHADOW_MODE,
		GetShadowMode() );
	pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_SHADOW_RADIOSITY,
		m_bOutputRadiosity ? 1 : 0 );
	pRenderContext.SafeRelease();
	
	DrawSetup( 0, m_DrawFlags, 0, -1, true );

	DrawExecute( 0, CurrentViewID(), 0, true );

	pRenderContext.GetFrom( materials );
		pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_SHADOW_RADIOSITY,
		0 );
	pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_RENDER_STAGE,
		DEFERRED_RENDER_STAGE_INVALID );

	g_CurrentViewID = oldViewID;
}

bool CBaseShadowView::AdjustView( float waterHeight )
{
	CommitData();

	return true;
}

void CBaseShadowView::PushView( float waterHeight )
{
	render->Push3DView( *this, 0, m_pDummyTexture, GetFrustum(), m_pDepthTexture );

	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->PushRenderTargetAndViewport( m_pDummyTexture, m_pDepthTexture, x, y, width, height );

#if defined( DEBUG ) || defined( SHADOWMAPPING_USE_COLOR )
	pRenderContext->ClearColor4ub( 255, 255, 255, 255 );
	pRenderContext->ClearBuffers( true, true );
#else
	pRenderContext->ClearBuffers( false, true );
#endif

	if ( m_bOutputRadiosity )
	{
		Assert( !IsErrorTexture( m_pRadAlbedoTexture ) );
		Assert( !IsErrorTexture( m_pRadNormalTexture ) );

		pRenderContext->SetRenderTargetEx( 1, m_pRadAlbedoTexture );
		pRenderContext->SetRenderTargetEx( 2, m_pRadNormalTexture );
	}
}

void CBaseShadowView::PopView()
{
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->PopRenderTargetAndViewport();

	render->PopView( GetFrustum() );
}

void CBaseShadowView::SetRadiosityOutputEnabled( bool bEnabled )
{
	m_bOutputRadiosity = bEnabled;
}

void COrthoShadowView::CalcShadowView()
{
	const cascade_t &m_data = GetCascadeInfo( iCascadeIndex );
	Vector mainFwd;
	AngleVectors( angles, &mainFwd );

	lightData_Global_t state = GetActiveGlobalLightState();
	QAngle lightAng;
	VectorAngles( -state.vecLight.AsVector3D(), lightAng );

	Vector viewFwd, viewRight, viewUp;
	AngleVectors( lightAng, &viewFwd, &viewRight, &viewUp );

	const float halfOrthoSize = m_data.flProjectionSize * 0.5f;

	origin += -viewFwd * m_data.flOriginOffset +
		viewUp * halfOrthoSize -
		viewRight * halfOrthoSize +
		mainFwd * halfOrthoSize;

	angles = lightAng;

	x = 0;
	y = 0;
	height = m_data.iResolution;
	width = m_data.iResolution;

	m_bOrtho = true;
	m_OrthoLeft = 0;
	m_OrthoTop = -m_data.flProjectionSize;
	m_OrthoRight = m_data.flProjectionSize;
	m_OrthoBottom = 0;

	zNear = zNearViewmodel = 0;
	zFar = zFarViewmodel = m_data.flFarZ;
	m_flAspectRatio = 0;

	float mapping_world = m_data.flProjectionSize / m_data.iResolution;
	origin -= fmod( DotProduct( viewRight, origin ), mapping_world ) * viewRight;
	origin -= fmod( DotProduct( viewUp, origin ), mapping_world ) * viewUp;

	origin -= fmod( DotProduct( viewFwd, origin ), GetDepthMapDepthResolution( zFar - zNear ) ) * viewFwd;

#if CSM_USE_COMPOSITED_TARGET
	x = m_data.iViewport_x;
	y = m_data.iViewport_y;
#endif
}

void COrthoShadowView::CommitData()
{
	struct sendShadowDataOrtho
	{
		shadowData_ortho_t data;
		int index;
		static void Fire( sendShadowDataOrtho d )
		{
			IDeferredExtension *pDef = GetDeferredExt();
			pDef->CommitShadowData_Ortho( d.index, d.data );
		};
	};

	const cascade_t &data = GetCascadeInfo( iCascadeIndex );

	Vector fwd, right, down;
	AngleVectors( angles, &fwd, &right, &down );
	down *= -1.0f;

	Vector vecScale( m_OrthoRight, abs( m_OrthoTop ), zFar - zNear );

	sendShadowDataOrtho shadowData;
	shadowData.index = iCascadeIndex;

#if CSM_USE_COMPOSITED_TARGET
	shadowData.data.iRes_x = CSM_COMP_RES_X;
	shadowData.data.iRes_y = CSM_COMP_RES_Y;
#else
	shadowData.data.iRes_x = width;
	shadowData.data.iRes_y = height;
#endif

	shadowData.data.vecSlopeSettings.Init(
		data.flSlopeScaleMin, data.flSlopeScaleMax, data.flNormalScaleMax, 1.0f / zFar
		);
	shadowData.data.vecOrigin.Init( origin );

	Vector4D matrix_scale_offset( 0.5f, -0.5f, 0.5f, 0.5f );

#if CSM_USE_COMPOSITED_TARGET
	shadowData.data.vecUVTransform.Init( x / (float)CSM_COMP_RES_X,
		y / (float)CSM_COMP_RES_Y,
		width / (float) CSM_COMP_RES_X,
		height / (float) CSM_COMP_RES_Y );
#endif

	VMatrix a,b,c,d,screenToTexture;
	render->GetMatricesForView( *this, &a, &b, &c, &d );
	MatrixBuildScale( screenToTexture, matrix_scale_offset.x,
		matrix_scale_offset.y,
		1.0f );

	screenToTexture[0][3] = matrix_scale_offset.z;
	screenToTexture[1][3] = matrix_scale_offset.w;

	MatrixMultiply( screenToTexture, c, shadowData.data.matWorldToTexture );

	QUEUE_FIRE( sendShadowDataOrtho, Fire, shadowData );

	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_SHADOW_INDEX, iCascadeIndex );
}


bool CDualParaboloidShadowView::AdjustView( float waterHeight )
{
	BaseClass::AdjustView( waterHeight );

	// HACK: when pushing our actual view the renderer fails building the worldlist right!
	// So we can't. Shit.
	return false;
}

void CDualParaboloidShadowView::PushView( float waterHeight )
{
	BaseClass::PushView( waterHeight );
}

void CDualParaboloidShadowView::PopView()
{
	BaseClass::PopView();
}

void CDualParaboloidShadowView::CalcShadowView()
{
	float flRadius = m_pLight->flRadius;

	m_bOrtho = true;
	m_OrthoTop = m_OrthoLeft = -flRadius;
	m_OrthoBottom = m_OrthoRight = flRadius;

	int dpsmRes = GetShadowResolution_Point();

	width = dpsmRes;
	height = dpsmRes;

	zNear = zNearViewmodel = 0;
	zFar = zFarViewmodel = flRadius;

	if ( m_bSecondary )
	{
		y = dpsmRes;

		Vector fwd, up;
		AngleVectors( angles, &fwd, NULL, &up );
		VectorAngles( -fwd, up, angles );
	}
}


void CSpotLightShadowView::CalcShadowView()
{
	float flRadius = m_pLight->flRadius;

	int spotRes = GetShadowResolution_Spot();

	width = spotRes;
	height = spotRes;

	zNear = zNearViewmodel = DEFLIGHT_SPOT_ZNEAR;
	zFar = zFarViewmodel = flRadius;

	fov = fovViewmodel = m_pLight->GetFOV();
}

void CSpotLightShadowView::CommitData()
{
	struct sendShadowDataProj
	{
		shadowData_proj_t data;
		int index;
		static void Fire( sendShadowDataProj d )
		{
			IDeferredExtension *pDef = GetDeferredExt();
			pDef->CommitShadowData_Proj( d.index, d.data );
		};
	};

	Vector fwd;
	AngleVectors( angles, &fwd );

	sendShadowDataProj data;
	data.index = m_iIndex;
	data.data.vecForward.Init( fwd );
	data.data.vecOrigin.Init( origin );
	// slope min, slope max, normal max, depth
	//data.data.vecSlopeSettings.Init( 0.005f, 0.02f, 3, zFar );
	data.data.vecSlopeSettings.Init( 0.001f, 0.005f, 3, 0 );

	QUEUE_FIRE( sendShadowDataProj, Fire, data );

	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->SetIntRenderingParameter( INT_RENDERPARM_DEFERRED_SHADOW_INDEX, m_iIndex );
}

