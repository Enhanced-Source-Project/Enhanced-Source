//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "basesdkcombatweapon_shared.h"

#ifndef BASESDKCOMBATWEAPON_H
#define BASESDKCOMBATWEAPON_H
#ifdef _WIN32
#pragma once
#endif

//=========================================================
// Machine gun base class
//=========================================================
abstract_class CSDKMachineGun : public CBaseSDKCombatWeapon
{
public:
	DECLARE_CLASS( CSDKMachineGun, CBaseSDKCombatWeapon );
	DECLARE_DATADESC();

	CSDKMachineGun();
	
	DECLARE_SERVERCLASS();

	void	PrimaryAttack( void );

	// Default calls through to m_hOwner, but plasma weapons can override and shoot projectiles here.
	virtual void	ItemPostFrame( void );
	virtual void	FireBullets( const FireBulletsInfo_t &info );
	virtual float	GetFireRate( void ) = 0;
	virtual int		WeaponRangeAttack1Condition( float flDot, float flDist );
	virtual bool	Deploy( void );

	virtual const Vector &GetBulletSpread( void );

	int				WeaponSoundRealtime( WeaponSound_t shoot_type );

	// utility function
	static void DoMachineGunKick( CBasePlayer *pPlayer, float dampEasy, float maxVerticleKickAngle, float fireDurationTime, float slideLimitTime );

protected:

	int	m_nShotsFired;	// Number of consecutive shots fired

	float	m_flNextSoundTime;	// real-time clock of when to make next sound
};

//=========================================================
// Machine guns capable of switching between full auto and 
// burst fire modes.
//=========================================================
// Mode settings for select fire weapons

enum
{
	FIREMODE_FULLAUTO = 1,
	FIREMODE_SEMI,
	FIREMODE_3RNDBURST,
};

//=========================================================
//	>> CSDKSelectFireMachineGun
//=========================================================
class CSDKSelectFireMachineGun : public CSDKMachineGun
{
	DECLARE_CLASS( CSDKSelectFireMachineGun, CSDKMachineGun );
public:

	CSDKSelectFireMachineGun( void );
	
	DECLARE_SERVERCLASS();

	virtual float	GetBurstCycleRate( void );
	virtual float	GetFireRate( void );

	virtual bool	Deploy( void );
	virtual void	WeaponSound( WeaponSound_t shoot_type, float soundtime = 0.0f );

	DECLARE_DATADESC();

	virtual int		GetBurstSize( void ) { return 3; };

	void			BurstThink( void );

	virtual void	PrimaryAttack( void );
	virtual void	SecondaryAttack( void );

	virtual int		WeaponRangeAttack1Condition( float flDot, float flDist );
	virtual int		WeaponRangeAttack2Condition( float flDot, float flDist );

protected:
	int m_iBurstSize;
	int	m_iFireMode;
};
#endif // BASESDKCOMBATWEAPON_H
