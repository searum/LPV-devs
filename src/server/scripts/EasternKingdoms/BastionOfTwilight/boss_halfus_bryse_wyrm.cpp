/*
* Copyright (C) 2011 True Blood <http://www.trueblood-servers.com/>
* By Asardial
*/

#include "ScriptPCH.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "the_bastion_of_twilight.h"
#include <cstring>

//enum Texts
#define SAY_AGGRO "The wyrms will eat well tonight !"
#define SAY_DIED "The burden of the damned falls upon you ! Aaaaaah....."

struct Locations
{
    float x, y, z, o;
};

static Locations SpawnPosition[]=
{
	{-341.818390f,-720.371155f,888.091125f,0.00f},
};

/*********************
** Halfus Brise-wyrme
*********************/

/************************
** Orphaned Emerald Whelp
*************************/
class npc_orphaned_emerald_whelp : public CreatureScript{
	public :
		npc_orphaned_emerald_whelp() : CreatureScript("npc_orphaned_emerald_whelp") { }

		struct npc_orphaned_emerald_whelpAI : public ScriptedAI {
			npc_orphaned_emerald_whelpAI(Creature * pCreature) : ScriptedAI(pCreature) 
			{
				pInstance = (InstanceScript*)pCreature->GetInstanceScript();
			}

			InstanceScript* pInstance;

			uint32 uiAtrophicPoisonTimer;

			void Reset()
			{
				uiAtrophicPoisonTimer = 12000;
			}

			void UpdateAI(const uint32 uiDiff)
			{
                if (!UpdateVictim())
					return;

				if(uiAtrophicPoisonTimer <= uiDiff)
				{	if(Unit * Target = SelectTarget(SELECT_TARGET_RANDOM, 0)){
						DoCast(Target,SPELL_ATROPHIC_POISON);
						uiAtrophicPoisonTimer = 12000;}
				} else uiAtrophicPoisonTimer -= uiDiff;
				
				DoMeleeAttackIfReady();
			}
		};

	CreatureAI* GetAI(Creature* pCreature) const
    {
		return new npc_orphaned_emerald_whelpAI(pCreature);
    }
};

/*****************
** Proto Behemoth
*****************/

/***************
** Halfus Drake
***************/
class npc_halfus_dragon : public CreatureScript{
	public:
		npc_halfus_dragon() : CreatureScript("npc_halfus_dragon") { }

		struct npc_halfus_dragonAI : public ScriptedAI {
			npc_halfus_dragonAI(Creature * pCreature) : ScriptedAI(pCreature)
			{
				pInstance = (InstanceScript*)pCreature->GetInstanceScript();
			}

			InstanceScript* pInstance;

			void Reset()
			{
				me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
			}

			void UpdateAI(const uint32 uiDiff)
			{	
				if(me->HasFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE))
				{
					me->SetReactState(REACT_PASSIVE);
				}

				DoMeleeAttackIfReady();
			}
		};

		CreatureAI* GetAI(Creature* pCreature) const
        {
			return new npc_halfus_dragonAI(pCreature);
        }

		bool OnGossipHello(Player* pPlayer, Creature* pCreature){
			char const* _message = "Simple Text!";
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,_message,GOSSIP_SENDER_MAIN ,GOSSIP_ACTION_INFO_DEF+1);
			pPlayer->SEND_GOSSIP_MENU(68,pCreature->GetGUID());

			return true;
		}

		bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
        {
            	pPlayer->PlayerTalkClass->ClearMenus();
				InstanceScript* pInstance;
            	pInstance = (InstanceScript*)pCreature->GetInstanceScript();
				if (!pInstance) return false;

				pPlayer->CLOSE_GOSSIP_MENU();

            	switch (uiAction)
            	{
					case GOSSIP_ACTION_INFO_DEF+1:
						if (Creature * Halfus = Unit::GetCreature(*pCreature,pInstance->GetData64(DATA_WYRMBREAKER)))
						{
							switch(pCreature->GetEntry())
							{
								case NPC_SLATE_DRAKE:
									pCreature->AddAura(SPELL_STONE_TOUCH,Halfus);
									break;
								case NPC_NETHER_SCION:
									pCreature->AddAura(SPELL_NETHER_BLINDNESS,Halfus);
									break;
								case NPC_STORM_RIDER:
                                    pCreature->CastSpell(pCreature->GetPositionX(),pCreature->GetPositionY(),pCreature->GetPositionZ(),SPELL_CYCLONE_WINDS,false);
									break;
							}
							Halfus->AddAura(SPELL_BIND_WILL,pCreature);
                            pCreature->SetAuraStack(SPELL_DRAGON_VENGEANCE,Halfus,Halfus->GetAuraCount(SPELL_DRAGON_VENGEANCE)+1);
							pCreature->SetReactState(REACT_AGGRESSIVE);
							pCreature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
						}
						break;
				}
            	return true;
        }

};

/******************
** NPC Cyclon Winds
******************/
class npc_cyclon_winds : public CreatureScript
{
    public:
        npc_cyclon_winds() : CreatureScript("npc_cyclon_winds") {  }

        struct npc_cyclon_windsAI : public ScriptedAI
        {
            npc_cyclon_windsAI(Creature * pCreature) : ScriptedAI(pCreature)
            { 
                pInstance = (InstanceScript*)pCreature->GetInstanceScript();
            }

            InstanceScript* pInstance;

            void Reset() { }

            void UpdateAI(const uint32 uiDiff) { }

            void MovementInForm(uint32 type, uint32 id)
            {
                if(type == POINT_MOTION_TYPE)
                {
                    switch(id)
                    {
                        case POINT_CYCLON_WIND:
                            Creature * Halfus = ObjectAccessor::GetCreature(*me,pInstance->GetData64(NPC_CYCLON_WIND));
                            me->AddAura(84092,Halfus);
                            me->DestroyForNearbyPlayers();
                    }
                }
            }
        };

        CreatureAI * GetAI(Creature * pCreature) const
        {
            return new npc_cyclon_windsAI(pCreature);
        }
};

/***************
** Cyclon Winds
***************/
class spell_cyclon_winds : public SpellScriptLoader
{
    public:
        spell_cyclon_winds() : SpellScriptLoader("spell_cyclon_winds") { }

        class spell_cyclon_windsSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_cyclon_windsSpellScript);

            bool Validate(SpellEntry const * spellEntry)
            {
                if (!sSpellStore.LookupEntry(spellEntry->Id))
                    return false;
                return true;
            }

            void HandleAfterHit()
            {
                Creature * Cyclon = ObjectAccessor::GetCreature(*GetCaster(),NPC_CYCLON_WIND);
                Cyclon->GetMotionMaster()->MovePoint(POINT_CYCLON_WIND,Positions[1]);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_cyclon_windsSpellScript::HandleAfterHit);
            }
        };

        SpellScript * GetSpellScript() const
        {
            return new spell_cyclon_windsSpellScript();
        }
};

void AddSC_boss_halfus_wyrmbreaker()
{
	new boss_halfus_wyrmbreaker();
	new npc_halfus_dragon();
	new npc_proto_behemoth();
	new npc_orphaned_emerald_whelp();
    new spell_cyclon_winds();
}