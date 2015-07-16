//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: The client side of weapons if they don't have a client side.
//
//=============================================================================//

#include "cbase.h"
#include "c_weapon__stubs.h"
#include "basecombatweapon_shared.h"
#include "c_basesdkcombatweapon.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#ifdef IMPORT_HL2
STUB_WEAPON_CLASS( weapon_pistol, WeaponPistol, C_BaseSDKCombatWeapon );
STUB_WEAPON_CLASS( weapon_smg, WeaponSMG, C_SDKSelectFireMachineGun );
STUB_WEAPON_CLASS( weapon_shotgun, WeaponShotgun, C_BaseSDKCombatWeapon );
STUB_WEAPON_CLASS( weapon_crowbar, WeaponCrowbar, C_BaseSDKBludgeonWeapon);
STUB_WEAPON_CLASS( weapon_rpg, WeaponRPG, C_BaseSDKCombatWeapon);
STUB_WEAPON_CLASS( weapon_357, Weapon357, C_BaseSDKCombatWeapon);
STUB_WEAPON_CLASS( weapon_ar1, WeaponAR1, C_SDKMachineGun);
STUB_WEAPON_CLASS( weapon_ar2, WeaponAR2, C_SDKMachineGun);
STUB_WEAPON_CLASS( weapon_bugbait, WeaponBugbait, C_BaseSDKCombatWeapon);
STUB_WEAPON_CLASS( weapon_crossbow, WeaponCrossbow, C_BaseSDKCombatWeapon);
#endif

#ifdef IMPORT_EXAMPLE
STUB_WEAPON_CLASS( weapon_puntgun, WeaponPuntGun, C_BaseSDKCombatWeapon);
#endif


