//========= Copyright © 1996-2008, Valve Corporation, All rights reserved. ============//
//
// Purpose: Here are tags to enable/disable things throughout the source code.
//
//=============================================================================//

#ifndef SDK_SHAREDDEFS_H
#define SDK_SHAREDDEFS_H
#ifdef _WIN32
#pragma once
#endif

#ifdef SWARM_DLL
// Quick swap! NUM_AI_CLASSES = LAST_SHARED_ENTITY_CLASS in ASW!
#define NUM_AI_CLASSES LAST_SHARED_ENTITY_CLASS
#endif

// Fell free to edit! You can comment/uncomment these out or add more defines.  

//========================
// GENERAL
//========================

// For GetGameDescription(). Mostly for MP.
#define GAMENAME "Template"

//========================
// PLAYER RELATED OPTIONS
//========================
// PLAYER_HEALTH_REGEN : Regen the player's health much like it does in Portal/COD
// #define PLAYER_HEALTH_REGEN
// PLAYER_MOUSEOVER_HINTS : When the player has their crosshair over whatever we put in UpdateMouseoverHints() it will do whatever you put there.
// this is how the hint system works in CSS. Since we have an instructor system, this methoid is obsolete, but the setup is still here.
// #define PLAYER_MOUSEOVER_HINTS
// PLAYER_IGNORE_FALLDAMAGE : Ignore fall damage.
// #define PLAYER_IGNORE_FALLDAMAGE
// PLAYER_DISABLE_THROWING : Disables throwing in the player pickup controller. (Like how it is in Portal 2.)
// #define PLAYER_DISABLE_THROWING
//------------------

//========================
// GAME UI
//========================
// GAMEUI_MULTI_MOVIES : Play random BIK movies in the main menu instead of one fixed one!
#define GAMEUI_MULTI_MOVIES 
// GAMEUI_MULTI_LOADSCREENS : Display a random loading screen no matter what map we are playing.
#define GAMEUI_MULTI_LOADSCREENS
//------------------

//========================
// WORLD
//========================
//#define WORLD_USE_HL2_GRAVITY : Use gravity settings much like HL2 or Portal.
#define WORLD_USE_HL2_GRAVITY
//------------------

#endif // SDK_SHAREDDEFS_H