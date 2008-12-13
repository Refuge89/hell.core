/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* ScriptData
SDName: Boss_Jin'do the Hexxer
SD%Complete: 85
SDComment: Mind Control not working because of core bug. Shades visible for all.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "def_zulgurub.h"

#define SPELL_BRAINWASHTOTEM            24262
#define SPELL_POWERFULLHEALINGWARD      24309               //We will not use this spell. We will summon a totem by script cause the spell totems will not cast.
#define SPELL_HEX                       24053
#define SPELL_DELUSIONSOFJINDO          24306
#define SPELL_SHADEOFJINDO              24308               //We will not use this spell. We will summon a shade by script.

//Healing Ward Spell
#define SPELL_HEAL                      38588               //Totems are not working right. Right heal spell ID is 24311 but this spell is not casting...

//Shade of Jindo Spell
#define SPELL_SHADOWSHOCK               19460
#define SPELL_INVISIBLE                 24699

#define SAY_AGGRO         "Welcome to da great show friends! Step right up to die!"

#define SOUND_AGGRO       8425

struct TRINITY_DLL_DECL boss_jindoAI : public ScriptedAI
{
    boss_jindoAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    uint32 BrainWashTotem_Timer;
    uint32 HealingWard_Timer;
    uint32 Hex_Timer;
    uint32 Delusions_Timer;
    uint32 Teleport_Timer;

    Creature *Shade;
    Creature *Skeletons;
    Creature *HealingWard;

    ScriptedInstance *pInstance;

    void Reset()
    {
        BrainWashTotem_Timer = 20000;
        HealingWard_Timer = 16000;
        Hex_Timer = 8000;
        Delusions_Timer = 10000;
        Teleport_Timer = 5000;
    }

    void Aggro(Unit *who)
    {
        DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_AGGRO);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //BrainWashTotem_Timer
        if (BrainWashTotem_Timer < diff)
        {
            DoCast(m_creature, SPELL_BRAINWASHTOTEM);
            BrainWashTotem_Timer = 18000 + rand()%8000;
        }else BrainWashTotem_Timer -= diff;

        //HealingWard_Timer
        if (HealingWard_Timer < diff)
        {
            //DoCast(m_creature, SPELL_POWERFULLHEALINGWARD);
            HealingWard = m_creature->SummonCreature(14987, m_creature->GetPositionX()+3, m_creature->GetPositionY()-2, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,30000);
            HealingWard_Timer = 14000 + rand()%6000;
        }else HealingWard_Timer -= diff;

        //Hex_Timer
        if (Hex_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_HEX);

            if(m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-80);

            Hex_Timer = 12000 + rand()%8000;
        }else Hex_Timer -= diff;

        //Casting the delusion curse with a shade. So shade will attack the same target with the curse.
        if (Delusions_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            DoCast(target, SPELL_DELUSIONSOFJINDO);

            Shade = m_creature->SummonCreature(14986, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            Shade->AI()->AttackStart(target);

            Delusions_Timer = 4000 + rand()%8000;
        }else Delusions_Timer -= diff;

        //Teleporting a random gamer and spawning 9 skeletons that will attack this gamer
        if (Teleport_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target && target->GetTypeId() == TYPEID_PLAYER)
            {
                DoTeleportPlayer(target, -11583.7783,-1249.4278,77.5471,4.745);

                if(m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                    m_creature->getThreatManager().modifyThreatPercent(target,-100);

                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()+2, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()-2, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()+4, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()-4, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()+2, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()-2, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()+4, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()-4, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()+3, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                Skeletons->AI()->AttackStart(target);
            }

            Teleport_Timer = 15000 + rand()%8000;
        }else Teleport_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Healing Ward
struct TRINITY_DLL_DECL mob_healing_wardAI : public ScriptedAI
{
    mob_healing_wardAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    uint32 Heal_Timer;

    ScriptedInstance *pInstance;

    void Reset()
    {
        Heal_Timer = 2000;
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI (const uint32 diff)
    {
        //Heal_Timer
        if(Heal_Timer < diff)
        {
            if(pInstance)
            {
                Unit *pJindo = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_JINDO));
                DoCast(pJindo, SPELL_HEAL);
            }
            Heal_Timer = 3000;
        }else Heal_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Shade of Jindo
struct TRINITY_DLL_DECL mob_shade_of_jindoAI : public ScriptedAI
{
    mob_shade_of_jindoAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    uint32 ShadowShock_Timer;

    ScriptedInstance *pInstance;

    void Reset()
    {
        ShadowShock_Timer = 1000;
        m_creature->CastSpell(m_creature, SPELL_INVISIBLE,true);
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI (const uint32 diff)
    {

        //ShadowShock_Timer
        if(ShadowShock_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHADOWSHOCK);
            ShadowShock_Timer = 2000;
        }else ShadowShock_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_jindo(Creature *_Creature)
{
    return new boss_jindoAI (_Creature);
}

CreatureAI* GetAI_mob_healing_ward(Creature *_Creature)
{
    return new mob_healing_wardAI (_Creature);
}

CreatureAI* GetAI_mob_shade_of_jindo(Creature *_Creature)
{
    return new mob_shade_of_jindoAI (_Creature);
}

void AddSC_boss_jindo()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_jindo";
    newscript->GetAI = GetAI_boss_jindo;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_healing_ward";
    newscript->GetAI = GetAI_mob_healing_ward;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_shade_of_jindo";
    newscript->GetAI = GetAI_mob_shade_of_jindo;
    newscript->RegisterSelf();
}
