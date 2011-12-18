/*
* Copyright (C) 2010-2011 Project SkyFire <http://www.projectskyfire.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include"ScriptPCH.h"
#include"baradin_hold.h"

enum Spells
{
    SPELL_BERSERK              = 47008,
    SPELL_CONSUMING_DARKNESS   = 88954,
    SPELL_METEOR_SLASH         = 88942,
    SPELL_FEL_FIRESTORM        = 88972,
    SPELL_FEL_FIRE_AURA        = 88999,
};

enum Events
{
    EVENT_BERSERK = 1,
    EVENT_CONSUMING_DARKNESS,
    EVENT_METEOR_SLASH,
    EVENT_FEL_FIRE_DAMAGE,
};

class boss_argaloth: public CreatureScript
{
    public:
        boss_argaloth() : CreatureScript("boss_argaloth") { }

    struct boss_argalothAI: public BossAI
    {
        boss_argalothAI(Creature* pCreature) : BossAI(pCreature, DATA_ARGALOTH) { }

        uint32 fel_firestorm_casted;

        void Reset()
        {
            _Reset();
            me->RemoveAurasDueToSpell(SPELL_BERSERK);
            events.ScheduleEvent(EVENT_BERSERK, 300 *IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_CONSUMING_DARKNESS, 14 *IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_METEOR_SLASH, 10 *IN_MILLISECONDS);
            fel_firestorm_casted = 0;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->GetHealthPct() < 66 && fel_firestorm_casted == 0)
            {
                DoCast(SPELL_FEL_FIRESTORM);
                events.DelayEvents(3 *IN_MILLISECONDS);
                fel_firestorm_casted = 1;
            }
            if (me->GetHealthPct() < 33 && fel_firestorm_casted == 1)
            {
                DoCast(SPELL_FEL_FIRESTORM);
                events.DelayEvents(3 *IN_MILLISECONDS);
                fel_firestorm_casted = 2;
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CONSUMING_DARKNESS:
                    DoCast(SPELL_CONSUMING_DARKNESS);
                    events.RescheduleEvent(EVENT_CONSUMING_DARKNESS, 22 *IN_MILLISECONDS);
                    break;
                case EVENT_METEOR_SLASH:
                    DoCast(SPELL_METEOR_SLASH);
                    events.RescheduleEvent(EVENT_METEOR_SLASH, 15 *IN_MILLISECONDS);
                    break;
                case EVENT_BERSERK:
                    DoCast(me, SPELL_BERSERK);
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }
     };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_argalothAI(pCreature);
    }
};

class mob_fel_fire : public CreatureScript
{
        public:
            mob_fel_fire() : CreatureScript("mob_fel_fire") { }

        struct mob_fel_fireAI : public ScriptedAI
        {
            mob_fel_fireAI(Creature* pCreature) : ScriptedAI(pCreature) { }

            bool aura;
            uint32 m_uiFelFireDamageTimer;

            void Reset()
            {
                aura = false;
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_uiFelFireDamageTimer = 1000;
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!aura)
                {
                    aura = true;
                    DoCast(me, SPELL_FEL_FIRE_AURA);
                }

                if (m_uiFelFireDamageTimer <= uiDiff)
                {
                    Map* pMap = me->GetMap();
                    Map::PlayerList const &PlayerList = pMap->GetPlayers();

                    if (!PlayerList.isEmpty())
                    {
                        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        {
                            if (Player* pPlayer = i->getSource())
                            {
                                if (me->GetDistance(pPlayer) < 3.0f)
                                {
                                    me->DealDamage(pPlayer, 12000, 0, SPELL_DIRECT_DAMAGE, SPELL_SCHOOL_MASK_FIRE, NULL, false);
                                }
                            }
                        }
                    }
                    m_uiFelFireDamageTimer = 1000;
                }
            }
        };
};

void AddSC_boss_argaloth()
{
    new boss_argaloth();
    new mob_fel_fire();
}
