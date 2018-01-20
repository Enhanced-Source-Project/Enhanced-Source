#ifndef C_VGUI_EDITOR_PROPS_H
#define C_VGUI_EDITOR_PROPS_H

#include "vgui_controls/frame.h"


class CVGUILightEditor_Properties : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE( CVGUILightEditor_Properties, vgui::Frame );

public:

	CVGUILightEditor_Properties( vgui::Panel *pParent );
	~CVGUILightEditor_Properties();

	void SendPropertiesToRoot();

	enum PROPERTYMODE
	{
		PROPERTYMODE_LIGHT = 0,
		PROPERTYMODE_GLOBAL,
	};

	PROPERTYMODE GetPropertyMode();
	void SetPropertyMode( PROPERTYMODE mode );
	void OnRequestPropertyLayout( PROPERTYMODE mode );

protected:

	void OnSizeChanged( int newWide, int newTall );

	void PerformLayout();

	MESSAGE_FUNC_PTR( OnPropertyChanged, "PropertyChanged", panel );
	MESSAGE_FUNC( OnLightSelectionChanged, "LightSelectionChanged" );
	MESSAGE_FUNC( OnFileLoaded, "FileLoaded" );

private:

	PROPERTYMODE m_iPropMode;

	vgui::PanelListPanel *m_pListRoot;

	CUtlVector< Panel* > m_hProperties;

	void FlushProperties();
	void UpdatePropertyVisibility();

	void CreateProperties();
	void CreateProperties_LightEntity();
	void CreateProperties_GlobalLight();
	void AddPropertyToList( Panel *panel );

	void PropertiesReadAll();
	void RefreshGlobalLightData();

	KeyValues *AllocKVFromVisible();
};


#endif