//====== Copyright © Sandern Corporation, All rights reserved. ===========//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "videocfgext.h"
#include "filesystem.h"

#include "videocfg/videocfg.h"

#include "materialsystem/imaterialsystemhardwareconfig.h"
#include "materialsystem/materialsystem_config.h"

// NOTE: This has to be the last file included!
#include "tier0/memdbgon.h"

extern ConVar deferred_lighting_enabled;
extern ConVar cl_flora_disable;

void ReadVideoCfgExt()
{
	KeyValues *pConfig = new KeyValues("VideoExtConfig");
	if( !pConfig )
		return;

	if( pConfig->LoadFromFile( filesystem, "cfg/videoext.txt", "MOD" ) || pConfig->LoadFromFile( filesystem, "cfg/videoext_default.txt", "MOD" ) )
	{
		deferred_lighting_enabled.SetValue(
			pConfig->GetBool( "setting.deferred_lighting", deferred_lighting_enabled.GetBool() ) );

	//	cl_flora_disable.SetValue(
	//		pConfig->GetBool( "setting.flora_disable", cl_flora_disable.GetBool() ) );
	}

	pConfig->deleteThis();
}

void SaveVideoCfgExt()
{
	KeyValues *pConfig = new KeyValues("VideoExtConfig");
	if( !pConfig )
		return;

	pConfig->SetBool( "setting.deferred_lighting", deferred_lighting_enabled.GetBool() );
	//pConfig->SetBool( "setting.flora_disable", cl_flora_disable.GetBool() );

	pConfig->SaveToFile( filesystem, "cfg/videoext.txt", "MOD");

	pConfig->deleteThis();
}

CON_COMMAND( debug_print_dxlevel, "" )
{
	ConVarRef mat_dxlevel("mat_dxlevel");

	Msg( "nDXLevel: %d, nMaxDXLevel: %d, mat_dxlevel: %d, hdr type: %d, HasFastVertexTextures: %d\n", 
		g_pMaterialSystemHardwareConfig->GetDXSupportLevel(), 
		g_pMaterialSystemHardwareConfig->GetMaxDXSupportLevel(), 
		mat_dxlevel.GetInt(),
		g_pMaterialSystemHardwareConfig->GetHDRType(),
		g_pMaterialSystemHardwareConfig->HasFastVertexTextures()
		);
}

CON_COMMAND( debug_dxlevel_force, "" )
{
	MaterialSystem_Config_t config = g_pMaterialSystem->GetCurrentConfigForVideoCard();
	Msg("Current dxlevel: %d\n", config.dxSupportLevel );

	config.dxSupportLevel = atoi(args[1]);
	g_pMaterialSystem->OverrideConfig( config, true );

	config = g_pMaterialSystem->GetCurrentConfigForVideoCard();
	Msg("New dxlevel: %d\n", config.dxSupportLevel );
}

CON_COMMAND( debug_print_threaded_mode, "" )
{
	Msg("Threaded mode is: ");
	switch( materials->GetThreadMode() )
	{
	case MATERIAL_SINGLE_THREADED:
		Msg("MATERIAL_SINGLE_THREADED\n");
		break;
	case MATERIAL_QUEUED_SINGLE_THREADED:
		Msg("MATERIAL_QUEUED_SINGLE_THREADED\n");
		break;
	case MATERIAL_QUEUED_THREADED:
		Msg("MATERIAL_QUEUED_THREADED\n");
		break;
	default:
		Msg("UNKNOWN!\n");
		break;
	}
}
