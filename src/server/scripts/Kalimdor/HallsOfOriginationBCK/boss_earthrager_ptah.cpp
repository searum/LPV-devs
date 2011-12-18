#include "ScriptPCH.h"
#include "WorldPacket.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "halls_of_origination.h"

enum ScriptTexts
{
    SAY_AGGRO                   = -1643019,
    SAY_DEATH                   = -1643020,
    SAY_SLAY_1                  = -1643021,
    SAY_SLAY_2                  = -1643022,
};

enum Spells
{
    SPELL_FLAME_BOLT = 77370,
    SPELL_RAGING_SMASH = 83650,
	SPELL_EARTH_POINT = 75339,
	SPELL_DUST_MOVE = 75547,
	SPELL_VORTEX_DUST = 93570,
};

enum Events
{
    EVENT_FLAME_BOLT,
    EVENT_RAGING_SMASH,
    EVENT_EARTH_POINT,
    EVENT_SUMMON,	
	EVENT_DUST_MOVE,
	EVENT_VORTEX_DUST,
};

enum SummonIds
{
    NPC_HORROR   = 40810,
	NPC_SCARAB   = 40458,
};

const Position aSpawnLocations[3] =
{
    {-530.561584f, -370.613525f, 156.935913f, 5.081081f},
    {-484.478302f, -371.117584f, 155.954208f, 4.429200f},
    {-507.319977f, -381.939392f, 154.764664f, 4.700163f},
};

class boss_earthrager_ptah : public CreatureScript
{
    public:
        boss_earthrager_ptah() : CreatureScript("boss_earthrager_ptah") {}

        struct boss_earthrager_ptahAI : public ScriptedAI
		{
			boss_earthrager_ptahAI(Creature* pCreature) : ScriptedAI(pCreature), Summons(me)
			{
				pInstance = pCreature->GetInstanceScript();
			}

			InstanceScript* pInstance;
            EventMap events;
			SummonList Summons;
			bool check_in;

            void Reset()
            {
                events.Reset();

                if (pInstance && (pInstance->GetData(DATA_EARTHRAGER_PTAH_EVENT) != DONE && !check_in))
                   pInstance->SetData(DATA_EARTHRAGER_PTAH_EVENT, NOT_STARTED);
                check_in = false;
            }

			void EnterCombat(Unit * /*who*/)
			{
				EnterPhaseGround();
				DoScriptText(SAY_AGGRO, me);
				if (pInstance)
                    pInstance->SetData(DATA_EARTHRAGER_PTAH_EVENT, IN_PROGRESS);
			}
			
			void JustDied(Unit* /*killer*/)
			{
				DoScriptText(SAY_DEATH, me);
				Summons.DespawnAll();
                if (pInstance)
                    pInstance->SetData(DATA_EARTHRAGER_PTAH_EVENT, DONE);
			}

			void KilledUnit(Unit* /*Killed*/)
			{
				DoScriptText(SAY_SLAY_1, me);
			}
			
			void JustSummoned(Creature *pSummoned)
			{
				pSummoned->SetInCombatWithZone();
				if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
                pSummoned->AI()->AttackStart(pTarget);

				Summons.Summon(pSummoned);
			}
			
			void SummonedCreatureDespawn(Creature *summon)
			{
				Summons.Despawn(summon);
			}
						
			void EnterPhaseGround()
			{
				events.ScheduleEvent(EVENT_FLAME_BOLT, 7500);
				events.ScheduleEvent(EVENT_RAGING_SMASH, urand(4000, 10000));
				events.ScheduleEvent(EVENT_EARTH_POINT, 8000);
				events.ScheduleEvent(EVENT_SUMMON, 50000);
				events.ScheduleEvent(EVENT_DUST_MOVE, 15000);
				events.ScheduleEvent(EVENT_VORTEX_DUST, urand(14000, 20000));
			}
			
			void UpdateAI(const uint32 diff)
			{
                if (!UpdateVictim())
                    return;
					
					events.Update(diff);

					while (uint32 eventId = events.ExecuteEvent())
					{
						switch(eventId)
						{
                        case EVENT_FLAME_BOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, true))
                                DoCast(target, SPELL_FLAME_BOLT);
                                events.ScheduleEvent(EVENT_FLAME_BOLT, 7500);
								return;
							case EVENT_RAGING_SMASH:
								DoCast(me->getVictim(), SPELL_RAGING_SMASH);
								events.ScheduleEvent(EVENT_RAGING_SMASH, urand(4000, 10000));
								return;
							case EVENT_EARTH_POINT:
								DoScriptText(SAY_SLAY_2, me);
								if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM,1,10.0f,true)) 
								DoCast(target, SPELL_EARTH_POINT);
								events.ScheduleEvent(EVENT_EARTH_POINT, 8000);
								return;
							case EVENT_SUMMON:
								me->SummonCreature(NPC_HORROR, aSpawnLocations[0].GetPositionX(), aSpawnLocations[0].GetPositionY(), aSpawnLocations[0].GetPositionZ(), 0.0f, TEMPSUMMON_CORPSE_DESPAWN);
								me->SummonCreature(NPC_SCARAB, aSpawnLocations[1].GetPositionX(), aSpawnLocations[1].GetPositionY(), aSpawnLocations[1].GetPositionZ(), 0.0f, TEMPSUMMON_CORPSE_DESPAWN);
								events.ScheduleEvent(EVENT_SUMMON, 50000);
								return;
							case EVENT_DUST_MOVE:
								if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, true))
								DoCast(target, SPELL_DUST_MOVE);
								events.ScheduleEvent(EVENT_DUST_MOVE, 15000);
								return;
							case EVENT_VORTEX_DUST:
								if (IsHeroic())
								{
									DoCast(me, SPELL_VORTEX_DUST);
								}
								events.ScheduleEvent(EVENT_VORTEX_DUST, urand(14000, 20000));
								return;
						}
					}
				DoMeleeAttackIfReady();
			}
		};

    CreatureAI* GetAI(Creature* creature) const
    {
		return new boss_earthrager_ptahAI(creature);
    }
};

void AddSC_boss_earthrager_ptah() 
{
    new boss_earthrager_ptah();
}