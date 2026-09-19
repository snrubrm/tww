// The original name of this file is unknown, but it must exist because d_a_npc_so's "cut" functions
// are in a separate .text section from the rest of its functions.

#include "d/actor/d_a_npc_so.h"

/* 00004CE8-000051E8       .text cutProc__10daNpc_So_cFv */
void daNpc_So_c::cutProc() {
    typedef void (daNpc_So_c::*cutFunc)();
    struct cut_entry_t {
        cutFunc start;
        cutFunc proc;
    };

    mStaffIdx = dComIfGp_evmng_getMyStaffId("NpcSo");
    if (mStaffIdx == -1) {
        return;
    }

    static char* action_table[] = {
        "SWIM",
        "JUMP",
        "APPEAR",
        "DIVE",
        "DISAPPEAR",
        "SET_ANM",
        "EFFECT",
        "EQUIP",
        "UNEQUIP",
        "EATESA",
        "EATESA1ST",
        "JUMP_MAPOPEN",
        "MINIGAME",
        "TURN",
        "MINIGAME_WARP",
        "MINIGAME_RETURN",
        "PARTNER_SHIP",
        "MINIGAME_WAIT",
        "MINIGAME_END",
        "MINIGAME_PL_TURN",
        "MINIGAME_PL_UP",
    };

    static cut_entry_t cut_tbl[] = {
        {&daNpc_So_c::cutSwimStart, &daNpc_So_c::cutSwimProc},
        {&daNpc_So_c::cutJumpStart, &daNpc_So_c::cutJumpProc},
        {&daNpc_So_c::cutAppearStart, &daNpc_So_c::cutAppearProc},
        {&daNpc_So_c::cutDiveStart, &daNpc_So_c::cutDiveProc},
        {&daNpc_So_c::cutDisappearStart, &daNpc_So_c::cutDisappearProc},
        {&daNpc_So_c::cutSetAnmStart, &daNpc_So_c::cutSetAnmProc},
        {&daNpc_So_c::cutEffectStart, &daNpc_So_c::cutEffectProc},
        {&daNpc_So_c::cutEquipStart, &daNpc_So_c::cutEquipProc},
        {&daNpc_So_c::cutUnequipStart, &daNpc_So_c::cutUnequipProc},
        {&daNpc_So_c::cutEatesaStart, &daNpc_So_c::cutEatesaProc},
        {&daNpc_So_c::cutEatesaFirstStart, &daNpc_So_c::cutEatesaFirstProc},
        {&daNpc_So_c::cutJumpMapopenStart, &daNpc_So_c::cutJumpMapopenProc},
        {&daNpc_So_c::cutMiniGameStart, &daNpc_So_c::cutMiniGameProc},
        {&daNpc_So_c::cutTurnStart, &daNpc_So_c::cutTurnProc},
        {&daNpc_So_c::cutMiniGameWarpStart, &daNpc_So_c::cutMiniGameWarpProc},
        {&daNpc_So_c::cutMiniGameReturnStart, &daNpc_So_c::cutMiniGameReturnProc},
        {&daNpc_So_c::cutPartnerShipStart, &daNpc_So_c::cutPartnerShipProc},
        {&daNpc_So_c::cutMiniGameWaitStart, &daNpc_So_c::cutMiniGameWaitProc},
        {&daNpc_So_c::cutMiniGameEndStart, &daNpc_So_c::cutMiniGameEndProc},
        {&daNpc_So_c::cutMiniGamePlTurnStart, &daNpc_So_c::cutMiniGamePlTurnProc},
        {&daNpc_So_c::cutMiniGamePlUpStart, &daNpc_So_c::cutMiniGamePlUpProc},
    };

    int actIdx = dComIfGp_evmng_getMyActIdx(mStaffIdx, action_table, ARRAY_SIZE(action_table), TRUE, 0);
    if (actIdx == -1) {
        dComIfGp_evmng_cutEnd(mStaffIdx);
        return;
    }

    if (dComIfGp_evmng_getIsAddvance(mStaffIdx)) {
        (this->*cut_tbl[actIdx].start)();
    }
    (this->*cut_tbl[actIdx].proc)();
}

/* 000051E8-000051EC       .text cutSwimStart__10daNpc_So_cFv */
void daNpc_So_c::cutSwimStart() {}

/* 000051EC-00005364       .text cutSwimProc__10daNpc_So_cFv */
void daNpc_So_c::cutSwimProc() {
    /* Nonmatching */
}

/* 00005364-000054A4       .text cutJumpStart__10daNpc_So_cFv */
void daNpc_So_c::cutJumpStart() {
    /* Nonmatching */
}

/* 000054A4-00005598       .text cutJumpProc__10daNpc_So_cFv */
void daNpc_So_c::cutJumpProc() {
    /* Nonmatching */
}

/* 00005598-00005610       .text cutAppearStart__10daNpc_So_cFv */
void daNpc_So_c::cutAppearStart() {
    fopAcM_seStart(this, JA_SE_CM_SO_RISE, 0);
    offsetAppear();
}

/* 00005610-00005668       .text cutAppearProc__10daNpc_So_cFv */
void daNpc_So_c::cutAppearProc() {
    if (std::fabsf(mB34 - mOffset.x) < 10.0f) {
        dComIfGp_evmng_cutEnd(mStaffIdx);
    }
}

/* 00005668-00005688       .text cutDiveStart__10daNpc_So_cFv */
void daNpc_So_c::cutDiveStart() {
    offsetDive();
}

/* 00005688-000056E0       .text cutDiveProc__10daNpc_So_cFv */
void daNpc_So_c::cutDiveProc() {
    if (std::fabsf(mB34 - mOffset.x) < 10.0f) {
        dComIfGp_evmng_cutEnd(mStaffIdx);
    }
}

/* 000056E0-00005778       .text cutDisappearStart__10daNpc_So_cFv */
void daNpc_So_c::cutDisappearStart() {
    offsetDive();
    fopAcM_seStart(this, JA_SE_CM_SO_DIVE, 0);
    fopKyM_createWpillar(&current.pos, scale.x * 1.2f, 1.4f, 0);
}

/* 00005778-000057D0       .text cutDisappearProc__10daNpc_So_cFv */
void daNpc_So_c::cutDisappearProc() {
    if (std::fabsf(mB34 - mOffset.x) < 10.0f) {
        dComIfGp_evmng_cutEnd(mStaffIdx);
    }
}

/* 000057D0-000058D0       .text cutSetAnmStart__10daNpc_So_cFv */
void daNpc_So_c::cutSetAnmStart() {
    /* Nonmatching */
}

/* 000058D0-0000599C       .text cutSetAnmProc__10daNpc_So_cFv */
void daNpc_So_c::cutSetAnmProc() {
    /* Nonmatching */
}

/* 0000599C-00005AA8       .text cutEffectStart__10daNpc_So_cFv */
void daNpc_So_c::cutEffectStart() {
    /* Nonmatching */
}

/* 00005AA8-00005ADC       .text cutEffectProc__10daNpc_So_cFv */
void daNpc_So_c::cutEffectProc() {
    dComIfGp_evmng_cutEnd(mStaffIdx);
}

/* 00005ADC-00005AE0       .text cutEquipStart__10daNpc_So_cFv */
void daNpc_So_c::cutEquipStart() {}

/* 00005AE0-00005B1C       .text cutEquipProc__10daNpc_So_cFv */
void daNpc_So_c::cutEquipProc() {
    mHudeFlag = 1;
    dComIfGp_evmng_cutEnd(mStaffIdx);
}

/* 00005B1C-00005B20       .text cutUnequipStart__10daNpc_So_cFv */
void daNpc_So_c::cutUnequipStart() {}

/* 00005B20-00005B5C       .text cutUnequipProc__10daNpc_So_cFv */
void daNpc_So_c::cutUnequipProc() {
    mHudeFlag = 0;
    dComIfGp_evmng_cutEnd(mStaffIdx);
}

/* 00005B5C-00005B98       .text cutEatesaStart__10daNpc_So_cFv */
void daNpc_So_c::cutEatesaStart() {
    offsetZero();
    mSpeed = 0.0f;
    speedF = 0.0f;
}

/* 00005B98-00005BF8       .text cutEatesaProc__10daNpc_So_cFv */
void daNpc_So_c::cutEatesaProc() {
    esa_class* esa = (esa_class*)fopAcIt_Judge(searchEsa_CB, this);
    if (esa != NULL) {
        esa->field_0x298 = 1;
    } else {
        dComIfGp_evmng_cutEnd(mStaffIdx);
    }
}

/* 00005BF8-00005C6C       .text cutEatesaFirstStart__10daNpc_So_cFv */
void daNpc_So_c::cutEatesaFirstStart() {
    offsetZero();
    daShip_c* ship = dComIfGp_getShipActor();
    mB28 = 650.0f + REG8_F(11);
    mB30 = ship->shape_angle.y;
    mBDA = 1;
    mSpeed = 0.0f;
    speedF = 0.0f;
}

/* 00005C6C-000060FC       .text cutEatesaFirstProc__10daNpc_So_cFv */
void daNpc_So_c::cutEatesaFirstProc() {
    /* Nonmatching */
}

/* 000060FC-00006148       .text cutJumpMapopenStart__10daNpc_So_cFv */
void daNpc_So_c::cutJumpMapopenStart() {
    mBBC = 0;
    offsetZero();
    setAnm(2, false);
    mBDA = 1;
}

/* 00006148-000064B8       .text cutJumpMapopenProc__10daNpc_So_cFv */
void daNpc_So_c::cutJumpMapopenProc() {
    /* Nonmatching */
}

/* 000064B8-00006710       .text cutMiniGameStart__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGameStart() {
    /* Nonmatching */
}

/* 00006710-00006FE4       .text cutMiniGameProc__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGameProc() {
    /* Nonmatching */
}

/* 00006FE4-00006FF0       .text cutTurnStart__10daNpc_So_cFv */
void daNpc_So_c::cutTurnStart() {
    mTimer = 0x1E;
}

/* 00006FF0-00007114       .text cutTurnProc__10daNpc_So_cFv */
void daNpc_So_c::cutTurnProc() {
    /* Nonmatching */
}

/* 00007114-00007324       .text cutMiniGameWarpStart__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGameWarpStart() {
    /* Nonmatching */
}

/* 00007324-00007438       .text cutMiniGameWarpProc__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGameWarpProc() {
    /* Nonmatching */
}

/* 00007438-00007524       .text cutMiniGameReturnStart__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGameReturnStart() {
    /* Nonmatching */
}

/* 00007524-0000781C       .text cutMiniGameReturnProc__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGameReturnProc() {
    /* Nonmatching */
}

/* 0000781C-00007820       .text cutPartnerShipStart__10daNpc_So_cFv */
void daNpc_So_c::cutPartnerShipStart() {}

/* 00007820-00007878       .text cutPartnerShipProc__10daNpc_So_cFv */
void daNpc_So_c::cutPartnerShipProc() {
    fopAc_ac_c* actor = dComIfGp_getShipActor();
    dComIfGp_event_setTalkPartner(actor);
    dComIfGp_evmng_cutEnd(mStaffIdx);
}

/* 00007878-000078E0       .text cutMiniGameWaitStart__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGameWaitStart() {
    camera_process_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    if (REG8_S(1) == 0) {
        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(1);
    }
}

/* 000078E0-00007914       .text cutMiniGameWaitProc__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGameWaitProc() {
    dComIfGp_evmng_cutEnd(mStaffIdx);
}

/* 00007914-00007934       .text cutMiniGameEndStart__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGameEndStart() {
    mTimer = REG12_S(0) + 0x2D;
    mBDE = 0;
}

/* 00007934-0000798C       .text cutMiniGameEndProc__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGameEndProc() {
    initCam();
    moveCam();
    if (cLib_calcTimer(&mTimer) == 0) {
        dComIfGp_evmng_cutEnd(mStaffIdx);
    }
}

/* 0000798C-000079AC       .text cutMiniGamePlTurnStart__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGamePlTurnStart() {
    mTimer = REG12_S(2) + 0xA;
    mBDE = 0;
}

/* 000079AC-00007AC0       .text cutMiniGamePlTurnProc__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGamePlTurnProc() {
    /* Nonmatching */
}

/* 00007AC0-00007B1C       .text cutMiniGamePlUpStart__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGamePlUpStart() {
    mTimer = 0x1E;
    camera_process_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera->mCamera.Stop();
    camera->mCamera.SetTrimSize(1);
}

/* 00007B1C-00007CBC       .text cutMiniGamePlUpProc__10daNpc_So_cFv */
void daNpc_So_c::cutMiniGamePlUpProc() {
    /* Nonmatching */
}

/* 00007CBC-00007E74       .text initCam__10daNpc_So_cFv */
void daNpc_So_c::initCam() {
    /* Nonmatching */
}

/* 00007E74-00008010       .text moveCam__10daNpc_So_cFv */
void daNpc_So_c::moveCam() {
    /* Nonmatching */
}
