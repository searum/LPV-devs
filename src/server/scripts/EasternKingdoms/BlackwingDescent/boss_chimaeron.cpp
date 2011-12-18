/*
* Copyright (C) 2011 True Blood <http://www.trueblood-servers.com/>
* By Asardial
*/

#include "ScriptPCH.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

#define TARGETS_10 2
#define TARGETS_25 5

enum Spells
{
	// Chimaeron
	SPELL_DOUBLE_ATTACK = 88826,
	SPELL_RONFLE = 82706,
	//P1
	SPELL_GELEE_CAUSTIQUE = 82935, // ZONE
	SPELL_MASSACRE = 82848, // ZONE
	SPELL_QUERRELLE = 88872,
	SPELL_BRECHE = 82881, // DEBUFF SWITCH TANK
	//P2
	SPELL_MORTALITE = 82934, // SOINS REDUIT DE 99%
	SPELL_DARK_MOC = 91307, // SEULEMENT HERO
	// Bill-o-Tron
	SPELL_MIXTURE = 82705, // ZONE
	SPELL_DEFAILLANCE = 88853, // DANGER
};

enum Creatures
{
	DATA_CHIMAERON = 43296,
	DATA_BILL_O_TRON = 44418,
};

enum Events
{
	// Chimaeron
	EVENT_DOUBLE_ATTACK,
	EVENT_GELEE_CAUSTIQUE,
	EVENT_MASSACRE,
	EVENT_QUERRELLE,
	EVENT_BRECHE,
	EVENT_DARK_MOC,
	EVENT_FRENZY,
	// Bill-o-tron
	EVENT_DEFAILLANCE,
	EVENT_MIXTURE,
};

/***********
** Chimaeron
************/

/*************
** Bill-O-Tron
**************/

void AddSC_boss_chimaeron()
{
    new boss_chimaeron();
	new npc_billotron();
}