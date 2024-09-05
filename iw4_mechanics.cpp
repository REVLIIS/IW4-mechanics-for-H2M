namespace Plugin {

    bool checkForRightyTighty(game::pmove_t* pm)
    {
        if ((pm->oldcmd.buttons & game::BUTTON_USERELOAD) == 0 && ((pm->cmd.buttons & game::BUTTON_USERELOAD) != 0) ||
            ((pm->oldcmd.buttons & game::BUTTON_RELOAD) == 0 && ((pm->cmd.buttons & game::BUTTON_RELOAD) != 0)))
        {
            if ((pm->ps->sprintState.lastSprintEnd - pm->ps->sprintState.lastSprintStart) < 50)
            {
                if (game::PM_Weapon_AllowReload(pm->ps, game::WEAPON_HAND_RIGHT) && !game::PM_Weapon_AllowReload(pm->ps, game::WEAPON_HAND_LEFT))
                {
                    game::PM_SetReloadingState(pm->ps, game::WEAPON_HAND_RIGHT);
                    return true;
                }
            }
        }

        return false;
    }

    void SprintDrop(game::pmove_t* pm)
    {
        game::playerState_s* ps = pm->ps;

        int handIndex = game::BG_PlayerLastWeaponHand(ps);

        for (int i = 0; i <= handIndex; i++)
        {
            if (i == game::WEAPON_HAND_LEFT)
            {
                if (pm->ps->holdingAkimbos && checkForRightyTighty(pm)) //if we are doing a righty tighty, keep left gun in running state
                {
                    continue;
                }
            }

            ps->weapState[i].weaponState = game::WEAPON_SPRINT_DROP;
            ps->weapState[i].weaponTime = game::BG_SprintOutTime(ps->weapon, false, ps->holdingAkimbos);
            ps->weapState[i].weaponDelay = 0;

            if ((BYTE)ps->pm_type < 7u)
            {
                ps->weapState[i].weapAnim = (34 | (ps->weapState[i].weaponState) & 0x800); //set correct sprintout anim when going from sprint end into ads
            }
        }
    }

    void checkForWristTwist(game::pmove_t* pm)
    {
        if ((pm->cmd.buttons & game::BUTTON_USERELOAD) == 0 && ((pm->oldcmd.buttons & game::BUTTON_USERELOAD) != 0) ||
            (pm->cmd.buttons & game::BUTTON_RELOAD) == 0 && ((pm->oldcmd.buttons & game::BUTTON_RELOAD) != 0))
        {
            //if we are allowed to reload our left gun, and NOT allowed to reload right gun, start wrist twist
            if (game::PM_Weapon_AllowReload(pm->ps, game::WEAPON_HAND_LEFT) && !game::PM_Weapon_AllowReload(pm->ps, game::WEAPON_HAND_RIGHT))
            {
                game::PM_SetReloadingState(pm->ps, game::WEAPON_HAND_LEFT);
            }
        }
    }

    void SprintRaise(game::pmove_t* pm)
    {
        game::playerState_s* ps = pm->ps;
        int handIndex = game::BG_PlayerLastWeaponHand(ps);

        for (int i = 0; i <= handIndex; i++)
        {
            ps->weapState[i].weaponState = game::WEAPON_SPRINT_RAISE;
            ps->weapState[i].weaponTime = game::BG_SprintInTime(ps->weapon, false, ps->holdingAkimbos);
            ps->weapState[i].weaponDelay = 0;

            if ((BYTE)ps->pm_flags < 7u)
            {
                ps->weapState[i].weapAnim = (ps->weapState[i].weaponState) & 0x800 | 0x1Fu;
            }

            if (pm->ps->holdingAkimbos)
            {
                if (i == game::PlayerHandIndex::WEAPON_HAND_RIGHT)
                {
                    checkForRightyTighty(pm);
                }
                else if (i == game::PlayerHandIndex::WEAPON_HAND_LEFT)
                {
                    checkForWristTwist(pm);
                }
            }
        }
    }

    //Reversed from IW4
    utils::hook::detour PM_Weapon_CheckForSprint_hook;
    void PM_Weapon_CheckForSprint_stub(game::pmove_t* pm)
    {
        if (!pm->cmd.weaponIndex) {
            return;
        }

        game::playerState_s ps = *pm->ps;
        int weaponStateRight = pm->ps->weapState[game::WEAPON_HAND_RIGHT].weaponState;
        int weaponStateLeft = pm->ps->weapState[game::WEAPON_HAND_LEFT].weaponState;

        if (weaponStateRight != game::WEAPON_FIRING && weaponStateRight != game::WEAPON_RECHAMBERING && weaponStateRight != game::WEAPON_MELEE_INIT && weaponStateRight != game::WEAPON_MELEE_FIRE && weaponStateRight != game::WEAPON_MELEE_END)
        {
            if (weaponStateLeft != game::WEAPON_FIRING && weaponStateLeft != game::WEAPON_RECHAMBERING
                && weaponStateLeft != game::WEAPON_MELEE_INIT && weaponStateLeft != game::WEAPON_MELEE_FIRE && weaponStateLeft != game::WEAPON_MELEE_END
                && weaponStateRight != game::WEAPON_RAISING && weaponStateRight != game::WEAPON_RAISING_ALTSWITCH
                && weaponStateRight != game::WEAPON_DROPPING && weaponStateRight != game::WEAPON_DROPPING_QUICK && weaponStateRight != game::WEAPON_DROPPING_ALT
                && weaponStateRight != game::WEAPON_OFFHAND_INIT && weaponStateRight != game::WEAPON_OFFHAND_PREPARE && weaponStateRight != game::WEAPON_OFFHAND_HOLD && weaponStateRight != game::WEAPON_OFFHAND_HOLD_PRIMED && weaponStateRight != game::WEAPON_OFFHAND_END
                )
            {
                if (((ps.pm_flags & game::PM_FLAG_SPRINTING) != 0) && (weaponStateRight != game::WEAPON_SPRINT_RAISE && weaponStateRight != game::WEAPON_SPRINT_LOOP && weaponStateRight != game::WEAPON_SPRINT_DROP))
                {
                    SprintRaise(pm);
                }
                else if (((ps.pm_flags & game::PM_FLAG_SPRINTING) == 0) && (weaponStateRight == game::WEAPON_SPRINT_RAISE || weaponStateRight == game::WEAPON_SPRINT_LOOP))
                {
                    SprintDrop(pm);
                }
            }
        }
    }


    void start() 
    {
        PM_Weapon_CheckForSprint_hook.create(0x2d9a10_b, PM_Weapon_CheckForSprint_stub);
    }
}
