/*
* Copyright (C) 2011 True Blood <http://www.trueblood-servers.com/>
* By Asardial
*/

#include "ObjectMgr.h"
#include "ScriptPCH.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "MapManager.h"

/*
DELETE FROM `creature_text` WHERE `entry` = 41378;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`)
VALUES
('41379', '0', '0', '', '1', '0', '0', '0', '0', '23372', 'VO_BD_Nefarian_MaloriakIntro0'),
('41378', '1', '0', 'There can be no disruptions! Mustn''t keep the master waiting! Mustn''t fail again!', '1', '0', '0', '0', '0', '19847', 'VO_BD_Maloriak_Event01'),
('41378', '2', '0', 'I present to you experiment number 25463-D!', '1', '0', '0', '0', '0', '19865', 'VO_BD_Maloriak_Event18'),
('41378', '2', '1', 'How well does the mortal shell handle extreme temperature change? Must find out! For science!', '1', '0', '0', '0', '0', '19853', 'VO_BD_Maloriak_Event07'),
('41378', '2', '2', 'Too early, but no choice... They must be tested! Face now my prime subjects!', '1', '0', '0', '0', '0', '19858', 'VO_BD_Maloriak_Event11'),
('41379', '2', '3', '', '1', '0', '0', '0', '0', '23370', 'VO_BD_Nefarian_Maloriak01'),
('41378', '3', '0', 'Strip the flesh, harvest the organs!', '1', '0', '0', '0', '0', '19849', 'VO_BD_Maloriak_Event03'),
('41378', '3', '1', 'Nothing goes to waste...', '1', '0', '0', '0', '0', '19848', 'VO_BD_Maloriak_Event02'),
('41378', '4', '0', 'There will never be another like me...', '1', '0', '0', '0', '0', '19850', 'VO_BD_Maloriak_Event04'),
('41379', '5', '0', '', '1', '0', '0', '0', '0', '23371', 'VO_BD_Nefarian_MaloriakDead01'),
('41378', '6', '0', 'Mix and stir, apply heat...', '1', '0', '0', '0', '0', '19851', 'VO_BD_Maloriak_Event05'),
('41378', '7', '0', 'This one''s a little unstable, but what''s progress without failure?', '1', '0', '0', '0', '0', '19852', 'VO_BD_Maloriak_Event06'),
('41378', '8', '0', 'If one goes to the limits, the results become unpredictable.', '1', '0', '0', '0', '0', '19854', 'VO_BD_Maloriak_Event08');
*/

enum Texts
{
	SAY_INTRO = 0,
	SAY_AGGRO = 1,
	SAY_EVENT_1 = 2,
	SAY_EVENT_2 = 2,
	SAY_EVENT_3 = 2,
	SAY_KILL_1 = 3,
	SAY_KILL_2 = 3,
	SAY_DEATH = 4,
	SAY_DEATH_MALO = 5,
	SAY_FLAME = 6,
	SAY_GEL = 7,
	SAY_PHASE_2 = 8,
};

enum Spells
{
	// Berserk
	SPELL_BERSERK = 64238,
	// Abération
	SPELL_CROISSANCE = 77987, // Catalyseur de croissance
	// Sujet Primordial
	SPELL_FIXE = 78617, // Fixer
	SPELL_POURFENDRE = 78034, // Pourfendre
	SPELL_SAUVAGE = 77987, // Catalyseur de croissance
	// Maloriak
	// Phase 1
	SPELL_ORAGE_ARCANIQUE = 77896, // Peu être decast
	SPELL_REMEDE_10 = 77912, // tooutes les 1 min
	SPELL_REMEDE_10_H = 92966,
	SPELL_REMEDE_25 = 92965,
	SPELL_REMEDE_25_H = 92967,
	//Fiole Rouge
	SPELL_FLAME_CONSO_10 = 77786, // Flammes Consumantes
	SPELL_FLAME_CONSO_10_H = 92972,
	SPELL_FLAME_CONSO_25 = 92971,
	SPELL_FLAME_CONSO_25_H = 92973,
	SPELL_DEFLAG_LAVA_10 = 77679, // Déflagration Brûlante
	SPELL_DEFLAG_LAVA_10_H = 92969,
	SPELL_DEFLAG_LAVA_25 = 92968,
	SPELL_DEFLAG_LAVA_25_H = 92970,
	// Summon Abération
	SPELL_ABERATION_LIBRE = 92703, // Libération des aberrations
	//Fiole Bleu
	SPELL_FRISSON_MORDANT = 77760, // Frisson mordant
	SPELL_GEL_INSTANT_10 = 77699, // Gel instantané
	SPELL_GEL_INSTANT_10_H = 92979,
	SPELL_GEL_INSTANT_25 = 92978,
	SPELL_GEL_INSTANT_25_H = 92980,
	//Fiole Verte - 100s
	SPELL_GEL_DELIRE = 77615, // Gelée débilitante
	// Phase 2 - 25% PV
	SPELL_SUMMON_JET_MAGMA = 78094,
	SPELL_JET_MAGMA = 78095, // Jets de magma
	SPELL_ZERO_ABSOLUE_AURA = 78201,
	SPELL_ZERO_ABSOLUE = 78208, // Zéro absolu
	SPELL_NOVA_ACIDE_10 = 78225, // Nova d'acide - - Spell non fonctionnel
	SPELL_NOVA_ACIDE_10_H = 93012,
	SPELL_NOVA_ACIDE_25 = 93011,
	SPELL_NOVA_ACIDE_25_H = 93013,
	SPELL_LIBRE_ALL = 77991, // Libération de tous les serviteurs
	// Mode Heroique
	SPELL_INSENSSIBLE = 92716, // Imprégné d'ombre
	SPELL_DARKNESS_10_H = 92982, // Linceul de ténèbres
	SPELL_DARKNESS_25_H = 92983,
	// Vil rata spell Hero
	SPELL_DARK_VASE_10_H = 92987, // Sombre vase
	SPELL_DARK_VASE_25_H = 92988,
};

enum Creatures
{
	DATA_MALORIAK = 41378,
};

enum Summons
{
	NPC_ABERATION = 41440, // Abération
	NPC_SUJET = 41841, // Sujet Primordial - imune au Taunt
	NPC_JET_MAGMA = 41901,
	NPC_ZERO_ABSOLUT = 41961,
	NPC_VIL_RATA = 49811,
};

const Position VilRataSpawnLocations[] = 
{
	{-121.145935f, -452.829834f, 73.717377f, 0.00f},
	{-91.068756f, -451.493683f, 73.682419f, 0.00f},
	{-106.184677f, -444.416748f, 73.629280f, 0.00f},
};

const Position ZeroSpawnLocations[] = 
{
	{-88.197243f, -451.317993f, 73.640305f, 3.774451f},
	{-91.065231f, -435.231476f, 73.374084f, 1.763831f},
	{-120.973938f, -436.207062f, 73.396347f, 3.173616f},
	{-123.469627f, -447.577393f, 73.614868f, 4.442031f},
	{-112.321480f, -455.868835f, 73.578918f, 6.244518f},
	{-99.715759f, -455.604706f, 73.707710f, 3.106851f},
	{-74.278595f, -439.878510f, 73.387581f, 1.555689f},
	{-138.144348f, -439.713989f, 73.384941f, 4.771893f},
};

const Position aSpawnLocations[] =
{
	{-137.344864f, -437.528046f, 73.374397f, 6.282065f},
	{-74.835655f, -437.843536f, 73.378448f, 3.061933f},
};

enum Events
{
	EVENT_BERSERK,
	EVENT_JETFLAME_TRIGGER,
	// Maloriak
	EVENT_ORAGE_ARCANIQUE,
	EVENT_REMEDY,	
	// Phase 1 only
	EVENT_SUMMON_P1,
	EVENT_SCORCHING_BLAST,
	EVENT_CONSUMING_FLAMMES,
	EVENT_FLASH_FREEZE,
	EVENT_BITING_CHILL,
	EVENT_GEL_DELIRE,
	// Phase 2 only
	EVENT_SUMMON_P2,
	EVENT_JET_MAGMA,
	EVENT_ZERO_ABSOLUE,
	EVENT_NOVA_ACIDE,
	// Mode Heroic
	EVENT_DARKNESS,
};

enum Phases
{
	PHASE_ALL       = 0,
	PHASE_ONE       = 1,
	PHASE_TWO       = 2,

	PHASE_ONE_MASK      = 1 << PHASE_ONE,
};

/**********
** Maloriak
***********/

/***********
** Jet Magma
************/

/**************
** Zero Absolut
***************/

/***********
** Vil Rata
************/

/***********
** Aberation
************/

/******************
** Sujet Primordial
*******************/

/******************
** Maloriak Remede
******************/
class spell_gen_maloriak_remedy : public SpellScriptLoader
{
public:
    spell_gen_maloriak_remedy() : SpellScriptLoader("spell_gen_maloriak_remedy") { }

    class spell_gen_maloriak_remedy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_maloriak_remedy_AuraScript);
        void HandleTick(AuraEffect const* aurEff)
        {
            Unit* target = GetTarget();
            if (target && target->GetTypeId() == TYPEID_UNIT)
            {
                int32 baseAmount = aurEff->GetBaseAmount();
                //sLog->outBasic("spell_remedy: amount %d, baseamount %d, tick %u", aurEff->GetAmount(), baseAmount, aurEff->GetTickNumber());
                if (baseAmount > 0)
                    const_cast<AuraEffect*>(aurEff)->SetAmount(baseAmount * aurEff->GetTickNumber());
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_maloriak_remedy_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript *GetAuraScript() const
    {
        return new spell_gen_maloriak_remedy_AuraScript();
    }
};

void AddSC_boss_maloriak()
{
	new boss_maloriak();
	new npc_jet_magma();
	new npc_zero_absolut();
	new mob_vil_rata();
	new mob_aberation();
	new mob_sujet();
	new spell_gen_maloriak_remedy();
};