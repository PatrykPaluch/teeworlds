#include "instagib_modifier.h"
#include <base/system.h>
#include <generated/protocol.h>
#include <engine/shared/config.h>
#include <game/server/entities/character.h>
#include <game/server/entities/laser.h>
#include <game/server/player.h>
#include <game/server/gamecontext.h>

#define SHIELD_COOLDOWN (int)(5 * SERVER_TICK_SPEED)
//#define SHIELD_COOLDOWN (int)(1 * SERVER_TICK_SPEED)
#define SHIELD_DURATION (int)(0.5 * SERVER_TICK_SPEED)

void CInstagibModifier::ScanGametypeForActivation(CGameContext* pGameServer, char* pGameTypeStr)
{
	m_pGameServer = pGameServer;
	m_Activated = false;
	if(pGameTypeStr[0] == 'i')
	{
		m_Activated = true;
		char aTmpStr[32];
		str_copy(m_GameType, pGameTypeStr, 32);
		str_copy(aTmpStr, pGameTypeStr+1, 31);
		str_copy(pGameTypeStr, aTmpStr, 32);

		// uppercase except i
		for(int i = 1; i < 32; i++)
		{
			if(m_GameType[i])
				m_GameType[i] = str_uppercase(m_GameType[i]);
		}
	}
}

void CInstagibModifier::OnInit(char *pGameType)
{
	str_copy(pGameType, m_GameType, 32);
}

void CInstagibModifier::OnTick()
{
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(m_ShieldCD[i] > 0)
			m_ShieldCD[i]--;
	}
}

void CInstagibModifier::OnCharacterSpawn(CCharacter* pChar)
{
	m_ShieldCD[pChar->GetPlayer()->GetCID()] = 0;
	pChar->m_aWeapons[WEAPON_HAMMER].m_Got = false;
	pChar->m_aWeapons[WEAPON_GUN].m_Got = false;
	pChar->m_aWeapons[WEAPON_SHOTGUN].m_Got = false;

	pChar->GiveWeapon(WEAPON_GRENADE, -1);
	pChar->GiveWeapon(WEAPON_LASER, -1);
	pChar->m_ActiveWeapon = WEAPON_LASER;
	pChar->m_LastWeapon = WEAPON_GRENADE;
	pChar->m_QueuedWeapon = -1;
}

void CInstagibModifier::OnCharacterFireGrenade(CCharacter* pChar)
{
	const int CID = pChar->GetPlayer()->GetCID();
	const vec2 Pos = pChar->m_Pos;
	const vec2 TargetDir = normalize(vec2(pChar->m_LatestInput.m_TargetX, pChar->m_LatestInput.m_TargetY));

	// find ground
	vec2 ImpactPos;
	if(m_pGameServer->Collision()->IntersectLine(Pos, Pos+TargetDir*110.0f, 0, &ImpactPos))
	{
		// shield jump (rocket jump)
		m_pGameServer->CreateExplosion(ImpactPos, CID, WEAPON_GRENADE, 10);
		int64 Mask = CmaskOne(CID);
		m_pGameServer->CreateSound(ImpactPos, SOUND_GRENADE_EXPLODE, Mask);
	}
	else if(m_ShieldCD[CID] <= 0)
	{
		// fire shield
		m_ShieldCD[CID] = SHIELD_COOLDOWN;
		m_pGameServer->CreateSound(Pos, SOUND_PICKUP_ARMOR);
		m_pGameServer->CreateSound(Pos, SOUND_PICKUP_ARMOR);
		m_pGameServer->CreateSound(Pos, SOUND_PICKUP_ARMOR);
		m_pGameServer->CreateSound(Pos + TargetDir*75.0f, SOUND_PICKUP_ARMOR);
		m_pGameServer->CreateSound(Pos + TargetDir*150.0f, SOUND_PICKUP_ARMOR);
	}
}

bool CInstagibModifier::OnCharacterTakeDamage(CCharacter* pChar, int Weapon, vec2 Force, int FromCID)
{
	if(Weapon == WEAPON_GRENADE)
	{
		return false;
	}

	if(m_pGameServer->m_apPlayers[FromCID])
		m_pGameServer->CreateSound(m_pGameServer->m_apPlayers[FromCID]->m_ViewPos,
								   SOUND_HIT, CmaskOne(FromCID));
	pChar->Die(FromCID, WEAPON_LASER);
	return true;
}

void CInstagibModifier::CharacterDoWeaponSwitch(CCharacter* pChar)
{
	if(pChar->m_QueuedWeapon != -1)
	{
		if(pChar->m_QueuedWeapon != pChar->m_ActiveWeapon)
			pChar->m_ReloadTimer = min(pChar->m_ReloadTimer, 10);
		pChar->SetWeapon(pChar->m_QueuedWeapon);
	}
}

void CInstagibModifier::OnCharacterSnap(CCharacter* pChar, CNetObj_Character* pNetChar)
{
	const vec2 Pos = pChar->m_Pos;
	const int CID = pChar->GetPlayer()->GetCID();
	pNetChar->m_Armor = 10 - ((float)m_ShieldCD[CID]/SHIELD_COOLDOWN * 10);

	if(m_ShieldCD[CID] >= (SHIELD_COOLDOWN-SHIELD_DURATION))
	{
		const vec2 TargetDir = normalize(vec2(pChar->m_LatestInput.m_TargetX,
											  pChar->m_LatestInput.m_TargetY));

		const float ShieldMidAngleCoverage = pi/4;
		const int ShieldCountPerPi = 10;
		const int MidShieldCount = (ShieldMidAngleCoverage*ShieldCountPerPi)/pi;

		for(int i = 0; i < MidShieldCount; i++)
		{
			const float Angle = ShieldMidAngleCoverage/MidShieldCount * (i+1) +
								atan2(TargetDir.x, -TargetDir.y) - pi/2;
			vec2 DirOff(cosf(Angle), sinf(Angle));

			CNetObj_Pickup *pPickup = (CNetObj_Pickup *)m_pGameServer->Server()->SnapNewItem(NETOBJTYPE_PICKUP,
				CID*50+i+1, sizeof(CNetObj_Pickup));
			if(!pPickup)
				return;

			vec2 PickupPos = Pos + normalize(DirOff) * 75.f;
			pPickup->m_X = (int)PickupPos.x;
			pPickup->m_Y = (int)PickupPos.y;
			pPickup->m_Type = PICKUP_ARMOR;

			// other way
			const float Angle2 = -ShieldMidAngleCoverage/MidShieldCount * (i+1) +
								atan2(TargetDir.x, -TargetDir.y) - pi/2;
			vec2 DirOff2(cosf(Angle2), sinf(Angle2));

			CNetObj_Pickup *pPickup2 = (CNetObj_Pickup *)m_pGameServer->Server()->SnapNewItem(NETOBJTYPE_PICKUP,
				CID*50+25+i+1, sizeof(CNetObj_Pickup));
			if(!pPickup2)
				return;

			vec2 PickupPos2 = Pos + DirOff2 * 75.f;
			pPickup2->m_X = (int)PickupPos2.x;
			pPickup2->m_Y = (int)PickupPos2.y;
			pPickup2->m_Type = PICKUP_ARMOR;
		}

		{
			CNetObj_Pickup *pPickup = (CNetObj_Pickup *)m_pGameServer->Server()->SnapNewItem(NETOBJTYPE_PICKUP,
				CID*50, sizeof(CNetObj_Pickup));
			if(!pPickup)
				return;

			vec2 PickupPos = Pos + TargetDir * 75.f;
			pPickup->m_X = (int)PickupPos.x;
			pPickup->m_Y = (int)PickupPos.y;
			pPickup->m_Type = PICKUP_ARMOR;
		}
	}
}

bool CInstagibModifier::LaserDoBounce(CLaser* pLaser)
{
	vec2 From = pLaser->m_Pos;
	vec2 To = pLaser->m_Pos + pLaser->m_Dir * pLaser->m_Energy;

	vec2 At;
	CCharacter* pHitChar = m_pGameServer->m_World.IntersectCharacter(From, To, 0.f, At,
		m_pGameServer->GetPlayerChar(pLaser->m_Owner));

	if(pHitChar)
	{
		const int HitCID = pHitChar->GetPlayer()->GetCID();
		if(m_ShieldCD[HitCID] >= (SHIELD_COOLDOWN-SHIELD_DURATION))
		{
			const vec2 TargetDir = normalize(vec2(pHitChar->m_LatestInput.m_TargetX,
												  pHitChar->m_LatestInput.m_TargetY));
			const vec2 LaserDir = normalize(To-From);

			if(dot(TargetDir, LaserDir) < -0.75f)
			{
				To = From + normalize(At-From) * (distance(At, From) - 75.f);
				// intersected
				pLaser->m_From = pLaser->m_Pos;
				pLaser->m_Pos = To;
				pLaser->m_Dir = -pLaser->m_Dir;

				pLaser->m_Energy -= distance(pLaser->m_From, pLaser->m_Pos) + m_pGameServer->Tuning()->m_LaserBounceCost;
				pLaser->m_Bounces++;

				if(pLaser->m_Bounces > m_pGameServer->Tuning()->m_LaserBounceNum)
					pLaser->m_Energy = -1;

				m_pGameServer->CreateSound(pLaser->m_Pos, SOUND_LASER_BOUNCE);

				m_pGameServer->CreateSound(pHitChar->m_Pos, SOUND_PLAYER_PAIN_SHORT);
				pHitChar->m_EmoteType = EMOTE_PAIN;
				pHitChar->m_EmoteStop = m_pGameServer->Server()->Tick() + 500 * m_pGameServer->Server()->TickSpeed() / 1000;
				return true;
			}
		}
	}
	return false;
}
