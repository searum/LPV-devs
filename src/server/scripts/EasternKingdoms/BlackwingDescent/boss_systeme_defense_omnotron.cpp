/*
* Copyright (C) 2011 True Blood <http://www.trueblood-servers.com/>
* By Asardial
*/

#include "ScriptPCH.h"
#include "blackwing_descend.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "WorldPacket.h"

#define SAY_AGGRO "Hmm... the Omnotron Defense System. Centuries ago these constructs were considered the Dwarves greatest technical achievements. With so many counters to each construct's attacks. I''ll have to rectify these designs for them to provide me any entertainement."
#define SAY_STORY "Stupid Dwarnes and you fascination with runes! Why would you create something that would help your enemy?"
#define SAY_LIGHTNING "Lightning conductor works much better in a crowd,yes?"
#define SAY_TOXITRON_CHEMICAL "Were you planning on using Toxitron's chemical to damage the other construcs?Clever plan, let me ruin that for you."
#define SAY_TOO_BAD "You would like to move away from that, wouldn't you?Too bad."
#define SAY_DEATH "Defense systems obliterated. Powering down...."

#define SAY_SHIELD_POISON "Toxitron unit shield system onlines."
#define SAY_SHIELD_FLAMME "Magmatron unit shield system onlines."
#define SAY_SHIELD_ELECTRON "Electron unit shield system onlines."
#define SAY_SHIELD_ARCANOTRON "Arcanotron unit shield system onlines."
#define SAY_ACTIVATION_TOXITRON "Toxitron unit activated."
#define SAY_ACTIVATION_MAGMATRON "Magmatron unit activated."
#define SAY_ACTIVATION_ELECTRON "Electron unit activated."
#define SAY_ACTIVATION_ARCANOTRON "Arcanotron unit activated."
#define SAY_REROUTING_ENERGY "Defense systems obliterated. Powering down...."

enum Spells
{
	// Omnitron
	SPELL_SHUTING_DOWN = 78746,
	SPELL_ACTIVATED = 78740,
	// Electron
	SPELL_LIGHTNING_CONDUCTOR = 79888,
	SPELL_ELECTRICAL_DISCHARGE = 91465,
	SPELL_UNSTABLE_SHIELD = 91447,
	// Toxitron
	SPELL_SUMMON_CHEMICAL_BOMB = 80159,
	SPELL_CHEMICAL_BOMB = 80157, // ZONE
	SPELL_POISON_SOAKED_SHELL = 91501,
	SPELL_POISON_PROTOCOL = 91513,
	// Magmatron
	SPELL_BARRIER = 79582,
	SPELL_ACQUIRING_TARGET = 79501,
	SPELL_FLAMETHROWER = 79504,
	SPELL_INCINERATION_SECURITY_MEASURE = 92944, // id temporaire ,vrai id spell 79023 ZONE
	// Arcanotron
	SPELL_SUMMON_POWER_GENERATOR = 79626, 
	SPELL_POWER_GENERATOR = 79624,
	SPELL_ARCANE_ANNIHILATOR = 79710, // Decast
	SPELL_POWER_CONVERSION = 79729,
	// Bombe de Poison
	SPELL_FIXER = 80094,
	SPELL_BOMBE = 80092, // ZONE
	SPELL_FLAC = 80097, // ZONE
	// Summon Spell
	// Chemical Bomb
	SPELL_MIND_FOG_AURA = 76230,
	SPELL_MIND_FOG_VISUAL = 76231,
	SPELL_FOG_DEGAT = 91554,
	// Mode Héroïque
	SPELL_GRIP_OF_DEATH = 91849, // ZONE
	SPELL_ENCASING_SHADOWS = 92023, // Nefarian tien la cible de Magmatron Target
	SPELL_OVERCHARGED_POWER_GENERATOR = 91857,
	SPELL_ARCANE_BLOWBACK = 91879, // Surcharge ZONE
	SPELL_SHADOW_INFUSION = 92048, // ZONE
};

// Summon Power Generator = 42733
// Summon Chemical Bomb = 42934
// Omnitron System = 42186

enum Summons
{
	MOB_POISON_BOMB = 42897, // Aucune imune
};

Creature* pElectron;
Creature* pMagmatron;
Creature* pToxitron;
Creature* pArcanotron;

/************
** Electron
************/

/************
** Magmatron
************/

/************
** Toxitron
************/

/*************
** Arcanotron
*************/

/**************
** Poison Bomb
**************/
class npc_poison_bomb : public CreatureScript
{
    public:
        npc_poison_bomb() : CreatureScript("npc_poison_bomb") { }

        struct npc_poison_bombAI : public ScriptedAI
        {
			npc_poison_bombAI(Creature * pCreature) : ScriptedAI(pCreature) {}
			
			uint32 uiFixe;
			bool uiBombeFixe;
			
			void Reset()
			{
				uiFixe = 2000;
				uiBombeFixe = false;
			}
			
			void Entercombat(Unit * /*who*/)
			{
				me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
				DoZoneInCombat();
			}

			void UpdateAI(const uint32 uiDiff)
			{
                if (!UpdateVictim())
                    return;
				
				if(uiFixe <= uiDiff)
				{
					if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_FIXER);
					uiFixe = 2000;
				} else uiFixe -= uiDiff;
				
				if(!uiBombeFixe) // Gestion si cible a proximité
				{
					DoCast(me, SPELL_BOMBE, true);
					DoCast(me, SPELL_FLAC, true);
					uiBombeFixe = true;
				}
				
				me->DespawnOrUnsummon(60000);
			}
		};

    CreatureAI* GetAI(Creature* creature) const
	{
        return new npc_poison_bombAI(creature);
    }
};

/******************
** Power Generator
******************/
class mob_power_generator : public CreatureScript
{
public:
    mob_power_generator() : CreatureScript("mob_power_generator") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_power_generatorAI(pCreature);
    }

    struct mob_power_generatorAI : public ScriptedAI
    {
        mob_power_generatorAI(Creature* c) : ScriptedAI(c)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->setFaction(16); // Same faction as bosses
            DoCast(me, SPELL_OVERCHARGED_POWER_GENERATOR);
			if (IsHeroic())
			{
				DoCast(me, SPELL_SHADOW_INFUSION);
			}

            me->DespawnOrUnsummon(60000);
        }
    };
};

/****************
** Chemical Bomb
****************/
class mob_chemical_bomb : public CreatureScript
{
public:
    mob_chemical_bomb() : CreatureScript("mob_chemical_bomb") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_chemical_bombAI(pCreature);
    }

    struct mob_chemical_bombAI : public ScriptedAI
    {
        mob_chemical_bombAI(Creature* c) : ScriptedAI(c)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->setFaction(16); // Same faction as bosses
            DoCast(SPELL_CHEMICAL_BOMB);
			DoCast(me, SPELL_FOG_DEGAT);
			me->CastSpell(me,SPELL_MIND_FOG_AURA, false);
            me->CastSpell(me,SPELL_MIND_FOG_VISUAL, false);
			if (IsHeroic())
			{
				if (Is25ManRaid())
                {
                    std::list<Unit*> targets;
					SelectTargetList(targets, 25, SELECT_TARGET_RANDOM, 0.0f, true);
                    if (!targets.empty())
                    for (std::list<Unit*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                        DoCast(*itr, SPELL_GRIP_OF_DEATH);
                } 
                else
                {
					std::list<Unit*> targets;
					SelectTargetList(targets, 10, SELECT_TARGET_RANDOM, 0.0f, true);
					if (!targets.empty())
					for (std::list<Unit*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
						DoCast(*itr, SPELL_GRIP_OF_DEATH);
                }
			}

            me->DespawnOrUnsummon(60000);
        }
    };
};

void AddSC_boss_omnitron_defense_system()
{
	new boss_magmatron();
	new boss_toxitron();
	new boss_electron();
	new boss_arcanotron();
	new npc_poison_bomb();
	new mob_power_generator();
	new mob_chemical_bomb();
}