/*
* Copyright (C) 2011 True Blood <http://www.trueblood-servers.com/>
* By Asardial
*/

#include "ScriptPCH.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

//enum Texts
#define SAY_AGGRO_FELUDIUS ""
#define SAY_AGGRO_IGNACIUS ""
#define SAY_AGGRO_TERRASTRA ""
#define SAY_AGGRO_ARION ""
#define SAY_AGGRO ""
#define SAY_DEATH ""

#define SAY_BURN ""
#define SAY_FREEZE ""
#define SAY_WIND ""
#define SAY_LIGHT ""

enum Spells
{
	//Ignacious
	SPELL_EGIDE_FLAME = 92512, // BOUCLIER
	SPELL_FLAMME_UP = 82643, // ZONE DECAST
	SPELL_RUEE_ENFER = 92524, // ZONE ALEATOIRE
	SPELL_FLAMME_TORRENT = 82777, // UNIQUEMENT SUR LE TANK
	SPELL_SANG_ARDENT = 82660, // ALEATOIRE
	//Feludius
	SPELL_GLACIATION = 82746, // ZONE CAC DIED
	SPELL_HYDROLANCE = 92509, // ALEATOIRE DECAST
	SPELL_BOMB_WATER = 82669, // ZONE
	SPELL_FROZEN_HEART = 82665, // ALEATOIRE
	//Terrastra
	SPELL_
	//Arion
	SPELL_
	//Monstruosité en élémentium
	SPELL_
};

enum sData
{
	DATA_ASCENDANT_COUNCIL,
	MAX_ENCOUNTER,
};

enum Creatures
{
	DATA_ELEMENTIU_MONSTROSITY	= 43735,
	DATA_IGNACIUS	= 43686,
	DATA_FELUDIUS	= 43687,
	DATA_TERRASTRA	= 43689,
	DATA_ARION	= 43688,
};

enum Events
{

};

// Phase 1 Ignacious et Feludius, 
// Phase 2 Terrastra et Arion,
// Phase 3 Monstruosité en élémentium
enum ePhases
{
    PHASE_ALL = 0,
    PHASE_1   = 1,
    PHASE_2   = 2,
    PHASE_3   = 3,

    PHASE_1_MASK  = 1 << PHASE_1,
    PHASE_2_MASK  = 1 << PHASE_2,
    PHASE_3_MASK  = 1 << PHASE_3
};

// Spawn de la Monstruosité en élémentium
static const Position SpawnPosMonstro = {-1008.500000f, -582.734985f, 831.901001f, 3.125290f};

/**************
** Base Conseil
**************/

/**********
** Ignacius
***********/

/**********
** Feludius
***********/

/***********
** Terrastra
************/

/********
** Arion
*********/

/***************************
** Monstrosité en Elémentium
****************************/


void AddSC_boss_assembly_of_ascendents()
{
    new boss_elementiu_monstrosity();
	new boss_feludius();
	new boss_ignacius();
	//new boss_terrastra();
	//new boss_arion();
}