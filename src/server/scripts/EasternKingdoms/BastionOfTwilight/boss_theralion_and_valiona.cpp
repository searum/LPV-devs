/*
* Copyright (C) 2011 True Blood <http://www.trueblood-servers.com/>
* By Asardial
*/

#include "the_bastion_of_twilight.h"
#include "SpellScript.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "ScriptPCH.h"

#define MAX_DAZZLIN_DESTRUCTION 6

/**********
** Theralion
***********/

/**********
** Valionna
***********/

/***************************
** Spell Dazzlin Destruction
****************************/
class spell_dazzling_destruction : public SpellScriptLoader
{
    public:
        spell_dazzling_destruction() : SpellScriptLoader("spell_dazzling_destruction") { }

        class spell_dazzling_destructionSpellScript : public SpellScript
        {
            int32 spell_trigger;
            PrepareSpellScript(spell_dazzling_destructionSpellScript);
            bool Validate(SpellEntry const * spellEntry)
            {
                if (!sSpellStore.LookupEntry(spellEntry->Id))
                    return false;
                return true;
            }

            bool Load()
            {
                spell_trigger = GetSpellInfo()->Effects[EFFECT_0].BasePoints;
                return true;
            }

            void HandleDummy(SpellEffIndex effIndex)
            {
                GetCaster()->CastSpell(GetTargetUnit(),spell_trigger,false);
            }

            void HandleOnHit()
            {
                std::list<Unit*> players;
                std::list<Unit*>::const_iterator itr;
                GetTargetUnit()->GetRaidMember(players,5.0f);
                for(itr=players.begin();itr!=players.end();++itr)
                {
                    if((*itr)->GetTypeId() == TYPEID_PLAYER)
                    {
                        GetCaster()->CastSpell((*itr),SPELL_DESTRUCTION_PROCS,false);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dazzling_destructionSpellScript::HandleDummy,EFFECT_0,SPELL_EFFECT_DUMMY);
                OnHit += SpellHitFn(spell_dazzling_destructionSpellScript::HandleOnHit);
            }
        };

        SpellScript * GetSpellScript() const
        {
            return new spell_dazzling_destructionSpellScript();
        }
};

void AddSC_boss_theralion()
{
    new boss_theralion();
    new boss_valiona();
    new spell_dazzling_destruction();
}