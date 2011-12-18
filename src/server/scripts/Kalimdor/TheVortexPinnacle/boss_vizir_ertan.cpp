#include "ScriptPCH.h"

#define SAY_AGGRO "Filthy beasts! Your presence in Skywall will not be tolerated!"
#define SAY_RANDOM "Ravaged!"

enum Spells
{
    SPELL_N_BOLT                     = 86331, //Lightning Bolt
    SPELL_H_BOLT                     = 93990, //Lightning Bolt HEROIC
    SPELL_H_TEMP                     = 86340, //TEMP HEROIC
    SPELL_BUFF                       = 86292,
};

enum Events
{
	EVENT_BOLT,
	EVENT_BUFF,
	EVENT_SAY,
};

class boss_vizir_ertan: public CreatureScript
{
    public:
        boss_vizir_ertan() : CreatureScript("boss_vizir_ertan") { }

        struct boss_vizir_ertanAI : public ScriptedAI
        {
			boss_vizir_ertanAI(Creature * pCreature) : ScriptedAI(pCreature) {}
			
			InstanceScript* pInstance;
			EventMap events;
			
			void EnterCombat(Unit * /*who*/)
			{
				EnterPhaseGround();
				me->MonsterYell(SAY_AGGRO, 0, 0);
                if (getDifficulty() == DUNGEON_DIFFICULTY_HEROIC) 
                {
					DoCast(me,SPELL_H_TEMP);
                }
			}
						
			void EnterPhaseGround()
			{
				events.ScheduleEvent(EVENT_SAY, 60000);
				events.ScheduleEvent(EVENT_BUFF, 90000);
				events.ScheduleEvent(EVENT_BOLT, 5000);
			}
			
			void UpdateAI(const uint32 diff)
			{
				if (!me->getVictim())
                    return;
					
					events.Update(diff);

					while (uint32 eventId = events.ExecuteEvent())
					{
						switch(eventId)
						{
							case EVENT_SAY:
								me->MonsterYell(SAY_RANDOM, 0, 0);
								events.ScheduleEvent(EVENT_SAY, 60000);
								return;
							case EVENT_BUFF:
								DoCast(me, SPELL_BUFF);
								events.ScheduleEvent(EVENT_BUFF, 90000);
								return;
						}
					}
					
                if (!UpdateVictim())
                    return;
					
					events.Update(diff);

					while (uint32 eventId = events.ExecuteEvent())
					{
						switch(eventId)
						{
							case EVENT_BOLT:
								DoCast(DUNGEON_MODE(SPELL_N_BOLT,SPELL_H_BOLT));
								events.ScheduleEvent(EVENT_BOLT, 5000);
								return;
						}
					}
				DoMeleeAttackIfReady();
			}
		};

    CreatureAI* GetAI(Creature* pCreature) const
    {
		return new boss_vizir_ertanAI(pCreature);
    }
};

void AddSC_boss_vizir_ertan()
{
    new boss_vizir_ertan();
};