/*
* Copyright (C) 2011 True Blood <http://www.trueblood-servers.com/>
* By Asardial
*/

#include "ScriptPCH.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

/*
Add field sound into the characters table...
uint32 Player::GetSound()
{
QueryResult result = CharacterDatabase.PQuery("SELECT sound FROM characters WHERE guid = %u",this->GetGUID());
if(!result)
{
sLog.outDebug("Player %u, %s, error while loading the sound from DB",this->GetGUID(),this->GetName());
return 0;
}
uint32 sound = result->Fetch()->GetUint32();
return sound;

}
*/

enum Spells
{
    SPELL_MODULATION = 77612,
    SPELL_SEARING_FLAME = 77840,
    SPELL_SONAR_PULSE = 77672,
    SPELL_SONIC_BREATH = 78075,
    SPELL_TRACKING = 78092,
    SPELL_ATRAMEDES_STUN = 77611,
};

enum
{
    ATRAMEDES_ENTRY = 41442,
};

/*************
** Antramedes
*************/

/******************
** Antramedes Gong
******************/
class atramedes_gong : public CreatureScript
{
public:
    atramedes_gong() : CreatureScript("atramedes_gong") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if(Creature* un = pCreature->FindNearestCreature(ATRAMEDES_ENTRY,90.0f))
        {
        un->StopMoving();
        un->CastStop();
        pCreature->CastSpell(un,SPELL_ATRAMEDES_STUN,true);
        pCreature->DisappearAndDie();
        }
        return true;
    }

};
void AddSC_boss_atramedes()
{
    new atramedes_gong();
    new boss_atramedes();
}