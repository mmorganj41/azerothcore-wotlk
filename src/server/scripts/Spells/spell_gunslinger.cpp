/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Scripts for spells with SPELLFAMILY_HUNTER, SPELLFAMILY_PET and SPELLFAMILY_GENERIC spells used by hunter players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_hun_".
 */

#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "Pet.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"

/// @todo: this import is not necessary for compilation and marked as unused by the IDE
//  however, for some reasons removing it would cause a damn linking issue
//  there is probably some underlying problem with imports which should properly addressed
//  see: https://github.com/azerothcore/azerothcore-wotlk/issues/9766
#include "GridNotifiersImpl.h"

enum GunslingerSpells
{
    // Ours

    // Theirs
};

// -42243 - Volley (Trigger one)
class spell_gun_volley_trigger : public SpellScript
{
    PrepareSpellScript(spell_gun_volley_trigger);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        // It's here because Volley is an AOE spell so there is no specific target to be attacked
        // Let's select one of our targets
        if (!targets.empty())
        {
            _target = *(targets.begin());
        }
    }

    void HandleFinish()
    {
        if (!_target)
        {
            return;
        }

        Unit* caster = GetCaster();
        if (!caster || !caster->IsPlayer())
        {
            return;
        }

        for (Unit::ControlSet::iterator itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
        {
            if (Unit* pet = *itr)
            {
                if (pet->IsAlive() && pet->GetTypeId() == TYPEID_UNIT)
                {
                    pet->ToCreature()->AI()->OwnerAttacked(_target->ToUnit());
                }
            }
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gun_volley_trigger::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        AfterCast += SpellCastFn(spell_gun_volley_trigger::HandleFinish);
    }

private:
    WorldObject* _target = nullptr;
};



void AddSC_gunslinger_spell_scripts()
{
    RegisterSpellScript(spell_gun_volley_trigger);
}
