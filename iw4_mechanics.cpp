namespace Plugin {

    bool checkForRightyTighty(game::pmove_t* pm)
    {
        if ((pm->oldcmd.buttons & game::BUTTON_USERELOAD) == 0 && ((pm->cmd.buttons & game::BUTTON_USERELOAD) != 0) ||
            ((pm->oldcmd.buttons & game::BUTTON_RELOAD) == 0 && ((pm->cmd.buttons & game::BUTTON_RELOAD) != 0)))
        {
            if ((pm->ps->sprintState.lastSprintEnd - pm->ps->sprintState.lastSprintStart) < 50) //Increase to make righty tighty easier
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

    bool checkForWristTwist(game::pmove_t* pm)
    {
        if ((pm->cmd.buttons & game::BUTTON_USERELOAD) == 0 && ((pm->oldcmd.buttons & game::BUTTON_USERELOAD) != 0) ||
            (pm->cmd.buttons & game::BUTTON_RELOAD) == 0 && ((pm->oldcmd.buttons & game::BUTTON_RELOAD) != 0))
        {
            //if we are allowed to reload our left gun, and NOT allowed to reload right gun, start wrist twist
            if (game::PM_Weapon_AllowReload(pm->ps, game::WEAPON_HAND_LEFT) && !game::PM_Weapon_AllowReload(pm->ps, game::WEAPON_HAND_RIGHT))
            {
                game::PM_SetReloadingState(pm->ps, game::WEAPON_HAND_LEFT);
                pm->ps->torsoAnim = 3181; //reload anim, overrides the reset in BG_ClearReloadAnim, makes it so the sprint anim is shown on 3rd person character 
                return true;
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
            if (i == game::WEAPON_HAND_LEFT && checkForRightyTighty(pm)) {
                continue;
            }

            ps->weapState[i].weaponState = game::WEAPON_SPRINT_DROP;
            ps->weapState[i].weaponTime = game::BG_SprintOutTime(ps->weapon, false, ps->holdingAkimbos);
            ps->weapState[i].weaponDelay = 0;

            if ((BYTE)ps->pm_type < 7u)
            {
                ps->weapState[i].weapAnim = (34 | (ps->weapState[i].weaponState) & 0x800);
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
                ps->weapState[i].weapAnim = (ps->weapState[i].weaponState) & 0x800 | 0x1Fu;

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

    //reversed from iw4
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

    /*
        This detour fixes an issue when you're on servers and trying to wrist twist with +usereload
        I added an additional check to see if you're pressing the usereload button when the sprint raise event is happening,
        so if your connection isn't perfect you dont stop halfway trough a wrist twist
        Not fully reversed yet, but wasn't needed for v1.0.0.
    */
    utils::hook::detour PM_SprintEndingButtons_hook;
    bool PM_SprintEndingButtons_stub(game::playerState_s* ps, int8_t forwardMove, int cmdButtons)
    {
        if ((ps->pm_flags & 0x8018) != 0 || forwardMove <= 105) {
            return true;
        }

        int v5 = ps->unknown3 & 1;
        int v6 = (53005 - (v5 != 0)) & 0xFFFFFDCF | 0x30;

        if ((ps->unknown1 & 0x40000000) == 0)
            v6 = (53005 - (v5 != 0)) | 0x30;

        int weaponState = ps->weapState[0].weaponState;

        if ((v6 & cmdButtons) != 0)
        {
            if (ps->holdingAkimbos && (cmdButtons & game::BUTTON_USERELOAD) == 0 && weaponState == game::WEAPON_SPRINT_RAISE) //+usereload high ping fix
                return false;

            return true;
        }

        bool weaponStateChecks = (unsigned int)(weaponState - 14) <= 10 || (unsigned int)(weaponState - 32) <= 1 || (unsigned int)(weaponState - 36) <= 3;

        return weaponStateChecks;
    }

    void start()
    {
        PM_Weapon_CheckForSprint_hook.create(0x2d9a10_b, PM_Weapon_CheckForSprint_stub);
        
        PM_SprintEndingButtons_hook.create(0x2cee40_b, PM_SprintEndingButtons_stub);
    }
}
