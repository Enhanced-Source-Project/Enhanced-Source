//=========== Copyright © 2013 - 2014, rHetorical, All rights reserved. =============
//
// Purpose: A useable player simular to the HL2 Player minus the stuff ties with 
// HL2.
//		
//====================================================================================

#include "cbase.h"
#include "sdk_player.h"
#include "effect_dispatch_data.h"
#include "te_effect_dispatch.h" 
#include "predicted_viewmodel.h"
#include "player.h"
#include "simtimer.h"
#include "player_pickup.h"
#include "game.h"
#include "gamerules.h"
#include "trains.h"
#include "in_buttons.h" 
#include "globalstate.h"
#include "KeyValues.h"
#include "eventqueue.h"
#include "engine/IEngineSound.h"
#include "ai_basenpc.h"
#include "SoundEmitterSystem/isoundemittersystembase.h"
#include "vphysics/player_controller.h"
#include "datacache/imdlcache.h"
#include "soundenvelope.h"
#include "ai_speech.h"		
#include "sceneentity.h"
#include "hintmessage.h"
#include "items.h"

// Our Player walk speed value.
ConVar player_walkspeed("player_walkspeed", "190");
ConVar player_normspeed( "player_normspeed", "120" );

// Show annotations?
ConVar hud_show_annotations( "hud_show_annotations", "1" );

// The delay from when we last got hurt to generate.
// This is already defined in ASW
#ifdef SWARM_DLL
extern ConVar sv_regeneration_wait_time;
#else
ConVar sv_regeneration_wait_time ("sv_regeneration_wait_time", "1.0", FCVAR_REPLICATED );
#endif

// Link us!
LINK_ENTITY_TO_CLASS( player, CSDKPlayer );

IMPLEMENT_SERVERCLASS_ST (CSDKPlayer, DT_SDKPlayer) 
	SendPropBool( SENDINFO(m_bPlayerPickedUpObject) ),
	SendPropInt( SENDINFO( m_iShotsFired ), 8, SPROP_UNSIGNED ),
END_SEND_TABLE()

BEGIN_DATADESC( CSDKPlayer )

	DEFINE_FIELD( m_bPlayerPickedUpObject, FIELD_BOOLEAN ),

	DEFINE_AUTO_ARRAY( m_vecMissPositions, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_nNumMissPositions, FIELD_INTEGER ),

//#ifdef PLAYER_HEALTH_REGEN
	DEFINE_FIELD( m_fTimeLastHurt, FIELD_TIME )
//#endif

END_DATADESC()

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------
// Basics
//-----------------------
CSDKPlayer::CSDKPlayer()
{
	m_nNumMissPositions = 0;

	// Set up the hints.
	m_pHintMessageQueue = new CHintMessageQueue(this);
	m_iDisplayHistoryBits = 0;
	m_bShowHints = true;

	if ( m_pHintMessageQueue )
	{
		m_pHintMessageQueue->Reset();
	}
	
	// We did not fire any shots.
	m_iShotsFired = 0;
}

CSDKPlayer::~CSDKPlayer()
{
	delete m_pHintMessageQueue;
	m_pHintMessageQueue = NULL;

	m_flNextMouseoverUpdate = gpGlobals->curtime;

}

void CSDKPlayer::Precache( void )
{
	BaseClass::Precache();

	// Sounds
	PrecacheScriptSound( SOUND_HINT );
	PrecacheScriptSound( SOUND_USE );
	PrecacheScriptSound( SOUND_USE_DENY );

	// Last, precache the player model or else the game will crash when the player dies.
	PrecacheModel ( "models/player.mdl" );
}

void CSDKPlayer::Spawn()
{
	// Dying without a player model crashes the client
	SetModel("models/player.mdl");
	SetMaxSpeed( PLAYER_WALK_SPEED );
	BaseClass::Spawn();
	StartWalking();	


#ifdef PLAYER_MOUSEOVER_HINTS
	m_iDisplayHistoryBits &= ~DHM_ROUND_CLEAR;
	SetLastSeenEntity ( NULL );
#endif

	// We did not fire any shots.
	m_iShotsFired = 0;

	GiveDefaultItems();

	GetPlayerProxy();

}

//------------------------------------------------------------------------------
// Purpose :
// Input   :
// Output  :
//------------------------------------------------------------------------------
Class_T  CSDKPlayer::Classify ( void )
{
	return CLASS_PLAYER;
}

void CSDKPlayer::PreThink()
{
	BaseClass::PreThink();

	if ( m_pHintMessageQueue )
		m_pHintMessageQueue->Update();
}

void CSDKPlayer::PostThink()
{
	BaseClass::PostThink();

	// Keep the model upright; pose params will handle pitch aiming.
	QAngle angles = GetLocalAngles();
	angles[PITCH] = 0;
	SetLocalAngles(angles);

	if ( m_flNextMouseoverUpdate < gpGlobals->curtime )
	{
		m_flNextMouseoverUpdate = gpGlobals->curtime + 0.2f;
		if ( m_bShowHints )
		{
			#ifdef PLAYER_MOUSEOVER_HINTS
			UpdateMouseoverHints();
			#endif
		}
	}

#ifdef PLAYER_HEALTH_REGEN
	// Regenerate heath after 3 seconds
	if ( IsAlive() && GetHealth() < GetMaxHealth() )
	{
		// Color to overlay on the screen while the player is taking damage
		color32 hurtScreenOverlay = {64,0,0,64};

		if ( gpGlobals->curtime > m_fTimeLastHurt + sv_regeneration_wait_time.GetFloat() )
		{
			TakeHealth( 1, DMG_GENERIC );
			m_bIsRegenerating = true;

			if ( GetHealth() >= GetMaxHealth() )
			{
				m_bIsRegenerating = false;
			}
		}
		else
		{
			m_bIsRegenerating = false;
			UTIL_ScreenFade( this, hurtScreenOverlay, 1.0f, 0.1f, FFADE_IN|FFADE_PURGE );
		}
	}
#else
	// HACK!: Make it so that when we fall, we show red!
	if ( IsAlive() && GetHealth() < GetMaxHealth() )
	{
		// Color to overlay on the screen while the player is taking damage
		color32 hurtScreenOverlay = {64,0,0,64};

		if ( gpGlobals->curtime > m_fTimeLastHurt + sv_regeneration_wait_time.GetFloat() )
		{
			if ( GetHealth() >= GetMaxHealth() )
			{
				// Nothing at all???
			}
		}
		else
		{
			UTIL_ScreenFade( this, hurtScreenOverlay, 1.0f, 0.1f, FFADE_IN|FFADE_PURGE );
		}
	}
#endif

}

//-----------------------------------------------------------------------------
// Purpose: Makes a splash when the player transitions between water states
//-----------------------------------------------------------------------------
void CSDKPlayer::Splash( void )
{
	CEffectData data;
	data.m_fFlags = 0;
	data.m_vOrigin = GetAbsOrigin();
	data.m_vNormal = Vector(0,0,1);
	data.m_vAngles = QAngle( 0, 0, 0 );
	
	if ( GetWaterType() & CONTENTS_SLIME )
	{
		data.m_fFlags |= FX_WATER_IN_SLIME;
	}

	float flSpeed = GetAbsVelocity().Length();
	if ( flSpeed < 300 )
	{
		data.m_flScale = random->RandomFloat( 10, 12 );
		DispatchEffect( "waterripple", data );
	}
	else
	{
		data.m_flScale = random->RandomFloat( 6, 8 );
		DispatchEffect( "watersplash", data );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSDKPlayer::UpdateClientData( void )
{
	if (m_DmgTake || m_DmgSave || m_bitsHUDDamage != m_bitsDamageType)
	{
		// Comes from inside me if not set
		Vector damageOrigin = GetLocalOrigin();
		// send "damage" message
		// causes screen to flash, and pain compass to show direction of damage
		damageOrigin = m_DmgOrigin;

		// only send down damage type that have hud art
		int iShowHudDamage = g_pGameRules->Damage_GetShowOnHud();
		int visibleDamageBits = m_bitsDamageType & iShowHudDamage;

		m_DmgTake = clamp( m_DmgTake, 0, 255 );
		m_DmgSave = clamp( m_DmgSave, 0, 255 );

		// If we're poisoned, but it wasn't this frame, don't send the indicator
		// Without this check, any damage that occured to the player while they were
		// recovering from a poison bite would register as poisonous as well and flash
		// the whole screen! -- jdw
		if ( visibleDamageBits & DMG_POISON )
		{
			float flLastPoisonedDelta = gpGlobals->curtime - m_tbdPrev;
			if ( flLastPoisonedDelta > 0.1f )
			{
				visibleDamageBits &= ~DMG_POISON;
			}
		}

		CSingleUserRecipientFilter user( this );
		user.MakeReliable();
		UserMessageBegin( user, "Damage" );
			WRITE_BYTE( m_DmgSave );
			WRITE_BYTE( m_DmgTake );
			WRITE_LONG( visibleDamageBits );
			WRITE_FLOAT( damageOrigin.x );	//BUG: Should be fixed point (to hud) not floats
			WRITE_FLOAT( damageOrigin.y );	//BUG: However, the HUD does _not_ implement bitfield messages (yet)
			WRITE_FLOAT( damageOrigin.z );	//BUG: We use WRITE_VEC3COORD for everything else
		MessageEnd();
	
		m_DmgTake = 0;
		m_DmgSave = 0;
		m_bitsHUDDamage = m_bitsDamageType;
		
		// Clear off non-time-based damage indicators
		int iTimeBasedDamage = g_pGameRules->Damage_GetTimeBased();
		m_bitsDamageType &= iTimeBasedDamage;
	}

	BaseClass::UpdateClientData();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------
// Viewmodel and weapon stuff!
//-----------------------
void CSDKPlayer::CreateViewModel( int index )
{
	BaseClass::CreateViewModel( index );
	return;
	Assert( index >= 0 && index < MAX_VIEWMODELS );

	if ( GetViewModel( index ) )
		return;

	CPredictedViewModel *vm = ( CPredictedViewModel * )CreateEntityByName( "predicted_viewmodel" );
	if ( vm )
	{
		vm->SetAbsOrigin( GetAbsOrigin() );
		vm->SetOwner( this );
		vm->SetIndex( index );
		DispatchSpawn( vm );
		vm->FollowEntity( this, false );
		m_hViewModel.Set( index, vm );
	}
}

bool CSDKPlayer::Weapon_Switch( CBaseCombatWeapon *pWeapon, int viewmodelindex )
{
	bool bRet = BaseClass::Weapon_Switch( pWeapon, viewmodelindex );

	return bRet;
}

void CSDKPlayer::Weapon_Equip( CBaseCombatWeapon *pWeapon )
{
	BaseClass::Weapon_Equip( pWeapon );
}

extern int	gEvilImpulse101;
//-----------------------------------------------------------------------------
// Purpose: Player reacts to bumping a weapon. 
// Input  : pWeapon - the weapon that the player bumped into.
// Output : Returns true if player picked up the weapon
//-----------------------------------------------------------------------------
bool CSDKPlayer::BumpWeapon( CBaseCombatWeapon *pWeapon )
{
	CBaseCombatCharacter *pOwner = pWeapon->GetOwner();

	// Can I have this weapon type?
	if ( !IsAllowedToPickupWeapons() )
		return false;

	if ( pOwner || !Weapon_CanUse( pWeapon ) || !g_pGameRules->CanHavePlayerItem( this, pWeapon ) )
	{
		if ( gEvilImpulse101 )
		{
			UTIL_Remove( pWeapon );
		}
		return false;
	}

	// Don't let the player fetch weapons through walls (use MASK_SOLID so that you can't pickup through windows)
	if( !pWeapon->FVisible( this, MASK_SOLID ) && !(GetFlags() & FL_NOTARGET) )
	{
		return false;
	}

	// ----------------------------------------
	// If I already have it just take the ammo
	// ----------------------------------------
	if (Weapon_OwnsThisType( pWeapon->GetClassname(), pWeapon->GetSubType())) 
	{
		//Only remove the weapon if we attained ammo from it
		if ( Weapon_EquipAmmoOnly( pWeapon ) == false )
			return false;

		// Only remove me if I have no ammo left
		// Can't just check HasAnyAmmo because if I don't use clips, I want to be removed, 
		if ( pWeapon->UsesClipsForAmmo1() && pWeapon->HasPrimaryAmmo() )
			return false;

		UTIL_Remove( pWeapon );
		return false;
	}
	// -------------------------
	// Otherwise take the weapon
	// -------------------------
	else 
	{
		//Make sure we're not trying to take a new weapon type we already have
		if ( Weapon_SlotOccupied( pWeapon ) )
		{
			CBaseCombatWeapon *pActiveWeapon = Weapon_GetSlot( WEAPON_PRIMARY_SLOT );

			if ( pActiveWeapon != NULL && pActiveWeapon->HasAnyAmmo() == false && Weapon_CanSwitchTo( pWeapon ) )
			{
				Weapon_Equip( pWeapon );
				return true;
			}

			//Attempt to take ammo if this is the gun we're holding already
			if ( Weapon_OwnsThisType( pWeapon->GetClassname(), pWeapon->GetSubType() ) )
			{
				Weapon_EquipAmmoOnly( pWeapon );
			}

			return false;
		}
	}

	pWeapon->CheckRespawn();
	Weapon_Equip( pWeapon );

	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------
// Walking
//-----------------------
void CSDKPlayer::StartWalking( void )
{
	SetMaxSpeed( PLAYER_WALK_SPEED );
	m_fIsWalking = true;
}

void CSDKPlayer::StopWalking( void )
{
	SetMaxSpeed( PLAYER_WALK_SPEED );
	m_fIsWalking = false;
}

void CSDKPlayer::HandleSpeedChanges( void )
{
	bool bIsWalking = IsWalking();
	bool bWantWalking;
	
	bWantWalking = true;
	
	if( bIsWalking != bWantWalking )
	{
		if ( bWantWalking )
		{
			StartWalking();
		}
		else
		{
			StopWalking();
		}
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------
// Use + Pickup
//-----------------------
bool CSDKPlayer::CanPickupObject( CBaseEntity *pObject, float massLimit, float sizeLimit )
{
	// reep: Ported from the base since in the base this is HL2 exclusive. Yeah, don't you LOVE base code? 

	//Must be valid
	if ( pObject == NULL )
		return false;

	//Must move with physics
	if ( pObject->GetMoveType() != MOVETYPE_VPHYSICS )
		return false;

	IPhysicsObject *pList[VPHYSICS_MAX_OBJECT_LIST_COUNT];
	int count = pObject->VPhysicsGetObjectList( pList, ARRAYSIZE(pList) );

	//Must have a physics object
	if (!count)
		return false;

	float objectMass = 0;
	bool checkEnable = false;
	for ( int i = 0; i < count; i++ )
	{
		objectMass += pList[i]->GetMass();
		if ( !pList[i]->IsMoveable() )
		{
			checkEnable = true;
		}
		if ( pList[i]->GetGameFlags() & FVPHYSICS_NO_PLAYER_PICKUP )
			return false;
		if ( pList[i]->IsHinged() )
			return false;
	}

	//Must be under our threshold weight
	if ( massLimit > 0 && objectMass > massLimit )
		return false;
	/*
	// reep: Could not get it to recognize classes. Think this is for the physcannon anyway. 
	if ( checkEnable )
	{
		#include "props.h"
		#include "vphysics/player_controller.h"
		#include "physobj.h"
		// Allow pickup of phys props that are motion enabled on player pickup
		CPhysicsProp *pProp = dynamic_cast<CPhysicsProp*>(pObject);
		CPhysBox *pBox = dynamic_cast<CPhysBox*>(pObject);
		if ( !pProp && !pBox )
			return false;

		if ( pProp && !(pProp->HasSpawnFlags( SF_PHYSPROP_ENABLE_ON_PHYSCANNON )) )
			return false;

		if ( pBox && !(pBox->HasSpawnFlags( SF_PHYSBOX_ENABLE_ON_PHYSCANNON )) )
			return false;
	}
	*/

	if ( sizeLimit > 0 )
	{
		const Vector &size = pObject->CollisionProp()->OBBSize();
		if ( size.x > sizeLimit || size.y > sizeLimit || size.z > sizeLimit )
			return false;
	}

	return true;
}

void CSDKPlayer::PickupObject( CBaseEntity *pObject, bool bLimitMassAndSize )
{
	// can't pick up what you're standing on
	if ( GetGroundEntity() == pObject )
	{
		DevMsg("Failed to pickup object: Player is standing on object!\n");
		PlayUseDenySound();
		return;
	}

	if ( bLimitMassAndSize == true )
	{
		if ( CanPickupObject( pObject, PLAYER_MAX_LIFT_MASS, PLAYER_MAX_LIFT_SIZE ) == false )
		{
			DevMsg("Failed to pickup object: Object too heavy!\n");
			PlayUseDenySound();
			return;
		}
	}

	// Can't be picked up if NPCs are on me
	if ( pObject->HasNPCsOnIt() )
		return;

	// Bool is to tell the client that we have an object. This is incase you want to change the crosshair 
	// or something for your project.
	m_bPlayerPickedUpObject = true;

	PlayerPickupObject( this, pObject );

}

void CSDKPlayer::ForceDropOfCarriedPhysObjects( CBaseEntity *pOnlyIfHoldingThis )
{
	m_bPlayerPickedUpObject = false;
	BaseClass::ForceDropOfCarriedPhysObjects( pOnlyIfHoldingThis );
}

void CSDKPlayer::PlayerUse ( void )
{
	// Was use pressed or released?
	if ( ! ((m_nButtons | m_afButtonPressed | m_afButtonReleased) & IN_USE) )
		return;

	if ( m_afButtonPressed & IN_USE )
	{
		// Currently using a latched entity?
		if ( ClearUseEntity() )
		{
			if (m_bPlayerPickedUpObject)
			{
				m_bPlayerPickedUpObject = false;
			}
			return;
		}
		else
		{
			if ( m_afPhysicsFlags & PFLAG_DIROVERRIDE )
			{
				m_afPhysicsFlags &= ~PFLAG_DIROVERRIDE;
				m_iTrain = TRAIN_NEW|TRAIN_OFF;
				return;
			}
		}

		// Tracker 3926:  We can't +USE something if we're climbing a ladder
		if ( GetMoveType() == MOVETYPE_LADDER )
		{
			return;
		}
	}

	if( m_flTimeUseSuspended > gpGlobals->curtime )
	{
		// Something has temporarily stopped us being able to USE things.
		// Obviously, this should be used very carefully.(sjb)
		return;
	}

	CBaseEntity *pUseEntity = FindUseEntity();

	bool usedSomething = false;

	// Found an object
	if ( pUseEntity )
	{
		//!!!UNDONE: traceline here to prevent +USEing buttons through walls			
		int caps = pUseEntity->ObjectCaps();
		variant_t emptyVariant;

		if ( m_afButtonPressed & IN_USE )
		{
			// Robin: Don't play sounds for NPCs, because NPCs will allow respond with speech.
			if ( !pUseEntity->MyNPCPointer() )
			{
				EmitSound( SOUND_USE );
			}
		}

		if ( ( (m_nButtons & IN_USE) && (caps & FCAP_CONTINUOUS_USE) ) ||
			 ( (m_afButtonPressed & IN_USE) && (caps & (FCAP_IMPULSE_USE|FCAP_ONOFF_USE)) ) )
		{
			if ( caps & FCAP_CONTINUOUS_USE )
				m_afPhysicsFlags |= PFLAG_USING;

			pUseEntity->AcceptInput( "Use", this, this, emptyVariant, USE_TOGGLE );

			usedSomething = true;
		}
		// UNDONE: Send different USE codes for ON/OFF.  Cache last ONOFF_USE object to send 'off' if you turn away
		else if ( (m_afButtonReleased & IN_USE) && (pUseEntity->ObjectCaps() & FCAP_ONOFF_USE) )	// BUGBUG This is an "off" use
		{
			pUseEntity->AcceptInput( "Use", this, this, emptyVariant, USE_TOGGLE );

			usedSomething = true;
		}
	}

	else if ( m_afButtonPressed & IN_USE )
	{
		// Signal that we want to play the deny sound, unless the user is +USEing on a ladder!
		// The sound is emitted in ItemPostFrame, since that occurs after GameMovement::ProcessMove which
		// lets the ladder code unset this flag.
		m_bPlayUseDenySound = true;
	}

	// Debounce the use key
	if ( usedSomething && pUseEntity )
	{
		m_Local.m_nOldButtons |= IN_USE;
		m_afButtonPressed &= ~IN_USE;
	}
}

void CSDKPlayer::ClearUsePickup()
{
	m_bPlayerPickedUpObject = false;
}

void CSDKPlayer::PlayUseDenySound()
{
	m_bPlayUseDenySound = true;
}

void CSDKPlayer::ItemPostFrame()
{
	BaseClass::ItemPostFrame();

	if ( m_bPlayUseDenySound )
	{
		m_bPlayUseDenySound = false;
		EmitSound( SOUND_USE_DENY );
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------
// Damage
//-----------------------
bool CSDKPlayer::PassesDamageFilter( const CTakeDamageInfo &info )
{
	CBaseEntity *pAttacker = info.GetAttacker();
	if( pAttacker && pAttacker->MyNPCPointer() && pAttacker->MyNPCPointer()->IsPlayerAlly() )
	{
		return false;
	}

	if( m_hPlayerProxy && !m_hPlayerProxy->PassesDamageFilter( info ) )
	{
		return false;
	}

	return BaseClass::PassesDamageFilter( info );
}

int	CSDKPlayer::OnTakeDamage( const CTakeDamageInfo &info )
{
	CTakeDamageInfo inputInfoCopy( info );

	// If you shoot yourself, make it hurt but push you less
	if ( inputInfoCopy.GetAttacker() == this && inputInfoCopy.GetDamageType() == DMG_BULLET )
	{
		inputInfoCopy.ScaleDamage( 5.0f );
		inputInfoCopy.ScaleDamageForce( 0.05f );
	}

	int ret = BaseClass::OnTakeDamage( inputInfoCopy );
	m_DmgOrigin = info.GetDamagePosition();

#ifdef PLAYER_IGNORE_FALLDAMAGE
	// ignore fall damage if instructed to do so by input
	if ( ( info.GetDamageType() & DMG_FALL ) )
	{
		inputInfoCopy.SetDamage(0.0f);
		return 0;
	}
#endif

#ifdef PLAYER_HEALTH_REGEN
	if ( GetHealth() < 100 )
	{
		m_fTimeLastHurt = gpGlobals->curtime;
	}
#endif

	return ret;
}

int CSDKPlayer::OnTakeDamage_Alive( const CTakeDamageInfo &info )
{
	// set damage type sustained
	m_bitsDamageType |= info.GetDamageType();

	if ( !CBaseCombatCharacter::OnTakeDamage_Alive( info ) )
		return 0;

	CBaseEntity * attacker = info.GetAttacker();

	if ( !attacker )
		return 0;

	Vector vecDir = vec3_origin;
	if ( info.GetInflictor() )
	{
		vecDir = info.GetInflictor()->WorldSpaceCenter() - Vector ( 0, 0, 10 ) - WorldSpaceCenter();
		VectorNormalize( vecDir );
	}

	if ( info.GetInflictor() && (GetMoveType() == MOVETYPE_WALK) && 
		( !attacker->IsSolidFlagSet(FSOLID_TRIGGER)) )
	{
		Vector force = vecDir;// * -DamageForce( WorldAlignSize(), info.GetBaseDamage() );
		if ( force.z > 250.0f )
		{
			force.z = 250.0f;
		}
		ApplyAbsVelocityImpulse( force );
	}

	// Burnt
	if ( info.GetDamageType() & DMG_BURN )
	{
		EmitSound( "Player.BurnPain" );
	}

	// fire global game event

	IGameEvent * event = gameeventmanager->CreateEvent( "player_hurt" );
	if ( event )
	{
		event->SetInt("userid", GetUserID() );
		event->SetInt("health", MAX(0, m_iHealth) );
		event->SetInt("priority", 5 );	// HLTV event priority, not transmitted

		if ( attacker->IsPlayer() )
		{
			CBasePlayer *player = ToBasePlayer( attacker );
			event->SetInt("attacker", player->GetUserID() ); // hurt by other player
		}
		else
		{
			event->SetInt("attacker", 0 ); // hurt by "world"
		}

		gameeventmanager->FireEvent( event );
	}

	// Insert a combat sound so that nearby NPCs hear battle
	if ( attacker->IsNPC() )
	{
		CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), 512, 0.5, this );
	}

	return 1;
}

void CSDKPlayer::OnDamagedByExplosion( const CTakeDamageInfo &info )
{
	if ( info.GetInflictor() && info.GetInflictor()->ClassMatches( "mortarshell" ) )
	{
		// No ear ringing for mortar
		UTIL_ScreenShake( info.GetInflictor()->GetAbsOrigin(), 4.0, 1.0, 0.5, 1000, SHAKE_START, false );
		return;
	}
	BaseClass::OnDamagedByExplosion( info );
}

#ifndef SWARM_DLL
void CSDKPlayer::FirePlayerProxyOutput( const char *pszOutputName, variant_t variant, CBaseEntity *pActivator, CBaseEntity *pCaller )
{
	if ( GetPlayerProxy() == NULL )
		return;

	GetPlayerProxy()->FireNamedOutput( pszOutputName, variant, pActivator, pCaller );
}
#endif

void CSDKPlayer::Event_Killed( const CTakeDamageInfo &info )
{
	BaseClass::Event_Killed( info );

	FirePlayerProxyOutput( "PlayerDied", variant_t(), this, this );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------
// Item Gifting
//-----------------------
extern int	gEvilImpulse101;

void CSDKPlayer::CheatImpulseCommands( int iImpulse )
{
	switch ( iImpulse )
	{
	case 101:
		{
			if( sv_cheats->GetBool() )
			{
				GiveAllItems();
			}
		}
		break;

	default:
		BaseClass::CheatImpulseCommands( iImpulse );
	}
}

void CSDKPlayer::GiveAllItems( void )
{
	// Impluse 101
}

void CSDKPlayer::GiveDefaultItems( void )
{
	// If you want the player to always start with something, give it
	// to them here.
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------
// Hint Systems (OBSOLETE)
//-----------------------
void CSDKPlayer::HintMessage( const char *pMessage, bool bDisplayIfDead, bool bOverrideClientSettings, bool bQuiet )
{
	if (!hud_show_annotations.GetBool())
		return;

	if ( !bDisplayIfDead && !IsAlive() || !IsNetClient() || !m_pHintMessageQueue )
		return;

	//Are we gonna play a sound?
	if(!bQuiet)
	{
		EmitSound(SOUND_HINT);
	}

	if ( bOverrideClientSettings || m_bShowHints )
		m_pHintMessageQueue->AddMessage( pMessage );
}

// All other mouse over hints.
#ifdef PLAYER_MOUSEOVER_HINTS
void CSDKPlayer::UpdateMouseoverHints()
{
	// Don't show if we are DEAD!
	if ( !IsAlive())
		return;

	if(PlayerHasObject())
		return;

	// keep writing.....
}
#endif

CLogicPlayerProxy *CSDKPlayer::GetPlayerProxy( void )
{
	CLogicPlayerProxy *pProxy = dynamic_cast< CLogicPlayerProxy* > ( m_hPlayerProxy.Get() );

	if ( pProxy == NULL )
	{
		pProxy = (CLogicPlayerProxy*)gEntList.FindEntityByClassname(NULL, "logic_playerproxy" );

		if ( pProxy == NULL )
			return NULL;

		pProxy->m_hPlayer = this;
		m_hPlayerProxy = pProxy;
	}

	return pProxy;
}