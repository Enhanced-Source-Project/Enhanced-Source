#ifndef C_LIGHT_EDITOR_H
#define C_LIGHT_EDITOR_H

#include "deferred/def_light_t.h"

struct def_light_t;

#define LIGHT_BOX_SIZE 8.0f
#define LIGHT_BOX_VEC Vector( LIGHT_BOX_SIZE, LIGHT_BOX_SIZE, LIGHT_BOX_SIZE )

#define HELPER_COLOR_MAX 0.8f
#define HELPER_COLOR_MIN 0.1f
#define HELPER_COLOR_LOW 0.4f
#define SELECTED_AXIS_COLOR Vector( HELPER_COLOR_MAX, HELPER_COLOR_MAX, HELPER_COLOR_MIN )

#define SELECTION_PICKER_SIZE 2.0f
#define SELECTION_PICKER_VEC Vector( SELECTION_PICKER_SIZE, SELECTION_PICKER_SIZE, SELECTION_PICKER_SIZE )

class def_light_editor_t : public def_light_t
{
public:
	def_light_editor_t();
	int iEditorId;
};

class CLightingEditor : public CAutoGameSystemPerFrame
{
	typedef CAutoGameSystemPerFrame BaseClass;
public:

	CLightingEditor();
	~CLightingEditor();

	bool Init();

	void LevelInitPostEntity();
	void LevelShutdownPreEntity();

	void Update( float ft );

	void OnRender();

	void SetEditorActive( bool bActive, bool bView = true, bool bLights = true );
	bool IsEditorActive();
	bool IsEditorLightingActive();

	bool IsLightingEditorAllowed();

	void GetEditorView( Vector *origin, QAngle *angles );
	void SetEditorView( const Vector *origin, const QAngle *angles );

	Vector &GetMoveDirForModify();
	void AbortEditorMovement( bool bStompVelocity );

	KeyValues *VmfToKeyValues( const char *pszVmf );
	void KeyValuesToVmf( KeyValues *pKV, CUtlBuffer &vmf );

	void LoadVmf( const char *pszVmf );
	void SaveCurrentVmf();
	const char *GetCurrentVmfPath();

	enum EDITORINTERACTION_MODE
	{
		EDITORINTERACTION_SELECT = 0,
		EDITORINTERACTION_ADD,
		EDITORINTERACTION_TRANSLATE,
		EDITORINTERACTION_ROTATE,

		EDITORINTERACTION_COUNT,
	};

	void SetEditorInteractionMode( EDITORINTERACTION_MODE mode );
	EDITORINTERACTION_MODE GetEditorInteractionMode();

	enum EDITOR_SELECTEDAXIS
	{
		EDITORAXIS_NONE = 0,
		EDITORAXIS_X,
		EDITORAXIS_Y,
		EDITORAXIS_Z,
		EDITORAXIS_SCREEN,
		EDITORAXIS_PLANE_XY,
		EDITORAXIS_PLANE_XZ,
		EDITORAXIS_PLANE_YZ,

		EDITORAXIS_COUNT,
		EDITORAXIS_FIRST = EDITORAXIS_X,
		EDITORAXIS_FIRST_PLANE = EDITORAXIS_PLANE_XY,
	};

	EDITOR_SELECTEDAXIS GetCurrentSelectedAxis();
	void SetCurrentSelectedAxis( EDITOR_SELECTEDAXIS axis );
	void UpdateCurrentSelectedAxis( int x, int y );

	enum EDITOR_DBG_MODES
	{
		EDITOR_DBG_OFF = 0,
		EDITOR_DBG_LIGHTING,
		EDITOR_DBG_DEPTH,
		EDITOR_DBG_NORMALS,
	};

	EDITOR_DBG_MODES GetDebugMode();
	void SetDebugMode( EDITOR_DBG_MODES mode );

	void AddLightFromScreen( int x, int y, KeyValues *pData );
	bool SelectLight( int x, int y, bool bGroup, bool bToggle );
	bool SelectLights( int *pi2_BoundsMin, int *pi2_BoundsMax,
		bool bGroup, bool bToggle );
	void MoveSelectedLights( Vector delta );
	void RotateSelectedLights( VMatrix matRotate );

	void ClearSelection();
	bool IsLightSelected( def_light_t *l );
	void DeleteSelectedLights();
	void SetSelectionCenterLocked( bool locked );

	KeyValues *GetKVFromLight( def_light_t *pLight );
	void GetKVFromAll( CUtlVector< KeyValues* > &listOut );
	void GetKVFromSelection( CUtlVector< KeyValues* > &listOut );
	void ApplyKVToSelection( KeyValues *pKVChanges );

	int GetNumSelected();
	bool IsAnythingSelected();
	Vector GetSelectionCenter();
	QAngle GetSelectionOrientation();

	Vector GetViewOrigin();
	QAngle GetViewAngles();
	Vector GetPickingRay( int x, int y );

	void ApplyLightStateToKV( lightData_Global_t state );
	void ApplyKVToGlobalLight( KeyValues *pKVChanges );
	KeyValues *GetKVGlobalLight();

	const lightData_Global_t &GetGlobalState();

private:

	bool m_bActive;
	bool m_bLightsActive;

	bool m_bSelectionCenterLocked;
	Vector m_vecSelectionCenterCache;

	void ApplyEditorLightsToWorld( bool bVisible );
	void FlushEditorLights();

	void AddEditorLight( def_light_t *pDef );

	void AddLightToSelection( def_light_t *l );
	void RemoveLightFromSelection( def_light_t *l );

	char m_szCurrentVmf[MAX_PATH*4];
	void ParseVmfFile( KeyValues *pKeyValues );
	void ApplyLightsToCurrentVmfFile();

	CUtlVector< def_light_t* > m_hEditorLights;
	CUtlVector< def_light_t* > m_hSelectedLights;

	KeyValues *m_pKVVmf;
	KeyValues *m_pKVGlobalLight; //* to child above
	lightData_Global_t m_EditorGlobalState;

	Vector m_vecEditorView_Origin;
	QAngle m_angEditorView_Angles;
	Vector m_vecMoveDir;
	Vector m_vecEditorView_Velocity;

	EDITORINTERACTION_MODE m_iInteractionMode;

	CMaterialReference m_matSprite_Light;
	CMaterialReference m_matSelect;
	CMaterialReference m_matHelper;

	EDITOR_SELECTEDAXIS m_iCurrentSelectedAxis;
	EDITOR_DBG_MODES m_iDebugMode;

	void RenderSprites();
	void RenderSelection();

	void RenderHelpers();
	void RenderTranslate();
	void RenderRotate();
};

CLightingEditor *GetLightingEditor();

#endif