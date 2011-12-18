/*
* Copyright (C) 2011 True Blood <http://www.trueblood-servers.com/>
* By Asardial
*/

#include "ScriptPCH.h"
#include "lost_city_of_the_tolvir.h"
#include "WorldPacket.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum Spells
{
    // Lockmaw
    SPELL_DUST_FLAIL            = 81642,
    SPELL_SCENT_OF_BLOOD        = 81690,
    SPELL_VENOMOUS_RAGE         = 81706,
    SPELL_VISCOUS_POISON        = 81630,
    // Augh
    SPELL_PARALYTIC_BLOW_DART   = 84799,
};

enum Creatures
{
    // Lockmaw encounter
    CREATURE_LOCKMAW                  = 43614,
    CREATURE_FRENZIED_CROCOLISK       = 43658,
    CREATURE_AUGH                     = 49045,
    CREATURE_AUGH_SCENT               = 45379,
    CREATURE_AUGH_WHIRLWIND           = 45378,
};

const Position SummonLocations[6] =
{
    //Frenzied Crocolisks
    {-11033.29f, -1674.57f, -0.56f, 1.09f},
    {-11029.84f, -1673.09f, -0.37f, 2.33f},
    {-11007.25f, -1666.37f, -0.23f, 2.46f},
    {-11006.83f, -1666.85f, -0.25f, 2.23f},
    {-11031.00f, -1653.59f,  0.86f, 2.42f},
    {-11069.23f, -1667.72f,  0.75f, 0.72f},
};

class boss_lockmaw : public CreatureScript
{
    public:
        boss_lockmaw() : CreatureScript("boss_lockmaw") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
           return new boss_lockmawAI(pCreature);
        }
        struct boss_lockmawAI : public ScriptedAI
        {
            boss_lockmawAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
            }

            InstanceScript* pInstance;
	
            uint32 DustTailTimer;
            uint32 ScentOfBloodTimer;
            uint32 ViscousPoisonTimer;
            uint32 WhirlwindTimer;

            bool Rage;
            std::list<uint64> SummonList;

            void Reset ()
            {
                DustTailTimer = 15000;
                ScentOfBloodTimer = 13000;
                ViscousPoisonTimer = 7000;
                WhirlwindTimer = 35000;

                Rage = false;
                RemoveSummons();
            }

            void RemoveSummons()
            {
                if (SummonList.empty())
                    return;

                for (std::list<uint64>::const_iterator itr = SummonList.begin(); itr != SummonList.end(); ++itr)
                {
                    if (Creature* pTemp = Unit::GetCreature(*me, *itr))
                        if (pTemp)
                            pTemp->DisappearAndDie();
                }
                SummonList.clear();
            }

            void JustSummoned(Creature* pSummon)
            {
                switch (pSummon->GetEntry())
                {
                    case CREATURE_AUGH_SCENT:
                    case CREATURE_AUGH_WHIRLWIND:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
                            pSummon->AI()->AttackStart(pTarget);
                        SummonList.push_back(pSummon->GetGUID());
                        break;
                }
            }

            void JustDied(Unit* /*Kill*/)
            {
                if (IsHeroic())
                   me->SummonCreature(CREATURE_AUGH, SummonLocations[5], TEMPSUMMON_MANUAL_DESPAWN);
            }

            void EnterCombat(Unit* /*Ent*/)
            {
                DoZoneInCombat();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (DustTailTimer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_DUST_FLAIL);
                    DustTailTimer = 25000;
                } else DustTailTimer -= diff;

                if (ScentOfBloodTimer <= diff)
                {
					// Base [4] now for test [1]
                    me->SummonCreature(CREATURE_AUGH_SCENT, SummonLocations[1], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    ScentOfBloodTimer = 45000;
                } else ScentOfBloodTimer -= diff;

                if (WhirlwindTimer <= diff)
                {
					// Base [4] now for test [1]
                    me->SummonCreature(CREATURE_AUGH_WHIRLWIND, SummonLocations[1], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000);
                    WhirlwindTimer = 45000;
                } else WhirlwindTimer -= diff;

                if (ViscousPoisonTimer <= diff)
                {
                    if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        DoCast(pTarget, SPELL_VISCOUS_POISON);
                    ViscousPoisonTimer = 12000;
                } else ViscousPoisonTimer -= diff;

                if (HealthBelowPct(31) && Rage == false)
                {
                    DoCast(me, SPELL_VENOMOUS_RAGE);
                    Rage = true;
                }

                DoMeleeAttackIfReady();
            }
        };
};
	
class npc_augh_scent : public CreatureScript
{
    public:
        npc_augh_scent() : CreatureScript("npc_augh_scent") { }

        struct npc_augh_scentAI : public ScriptedAI
        {
            npc_augh_scentAI(Creature *pCreature) : ScriptedAI(pCreature)
            {
            }

            uint32 ParalyticBlowDartTimer;

            bool Scent;

            void Reset()
            {
                ParalyticBlowDartTimer = 3000;
                Scent = false;
				me->DespawnOrUnsummon(60000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (Scent == false)
                {
                    if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    {
                        Talk(0);
                        DoCast(pTarget, SPELL_SCENT_OF_BLOOD);
                        for(uint8 i=0; i<4; i++)
                        {
                            Creature* Crocolisk = me->SummonCreature(CREATURE_FRENZIED_CROCOLISK, SummonLocations[i], TEMPSUMMON_CORPSE_DESPAWN);
                            Crocolisk->AddThreat(pTarget, 0.0f);
                            DoZoneInCombat(Crocolisk);
                        }
                    }
                    Scent = true;
				}

                if (ParalyticBlowDartTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        DoCast(target, SPELL_PARALYTIC_BLOW_DART, true);
                    ParalyticBlowDartTimer = 15000;
                } else ParalyticBlowDartTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_augh_scentAI(pCreature);
        }
};

class npc_augh_whirlwind : public CreatureScript
{
    public:
        npc_augh_whirlwind() : CreatureScript("npc_augh_whirlwind") { }

        struct npc_augh_whirlwindAI : public ScriptedAI
        {
            npc_augh_whirlwindAI(Creature *pCreature) : ScriptedAI(pCreature)
            {
            }

            bool Targeted;

            void Reset()
            {
                Targeted = false;
				me->DespawnOrUnsummon(60000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (Targeted == false)
                {
                    if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    {
                        DoCast(me, 84784);
                        Talk(0);
                        me->GetMotionMaster()->MoveChase(pTarget);
                    }
                    Targeted = true;
                }
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_augh_whirlwindAI(pCreature);
        }
};

#define spell_vicious 81677
#define spell_vicious_H 89999

class mob_crosilik: public CreatureScript
{
public: 
 mob_crosilik() : CreatureScript("mob_crosilik") { } 

 struct mob_crosilikAI : public ScriptedAI
    {
        mob_crosilikAI(Creature *c) : ScriptedAI(c) {}

		uint32 vicious;
		
        void Reset()
        {
			vicious = 10000;
			me->DespawnOrUnsummon(60000);		   
        }

		void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
            return;

			if (vicious<= diff)
			{
				if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
				DoCast(pTarget, IsHeroic() ? spell_vicious_H : spell_vicious);
				vicious   = 10000; 
			} else vicious -= diff; 
			
			DoMeleeAttackIfReady();
        }
		
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_crosilikAI (pCreature);
    }

};

void AddSC_boss_lockmaw()
{
    new boss_lockmaw();
    new npc_augh_scent();
    new mob_crosilik();
}