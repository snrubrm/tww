/**
 * d_a_obj_tntrap.cpp
 * Object - Thunder barrier
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_obj_tntrap.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_ship.h"
#include "f_op/f_op_actor_mng.h"
#include "JSystem/JUtility/JUTAssert.h"

namespace {
    const char l_arcname[] = "TnTrap";
    const dCcD_SrcTri l_tri_src = {
        {
            /* Flags             */ 0,
            /* SrcObjAt Type     */ 0x800,
            /* SrcObjAt Atp      */ 1,
            /* SrcObjAt SPrm     */ 5,
            /* SrcObjTg Type     */ 0,
            /* SrcObjTg SPrm     */ 0,
            /* SrcObjCo SPrm     */ 0,
            /* SrcGObjAt Se      */ 0,
            /* SrcGObjAt HitMark */ 0,
            /* SrcGObjAt Spl     */ 11,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ 0,
            /* SrcGObjTg Se      */ 0,
            /* SrcGObjTg HitMark */ 0,
            /* SrcGObjTg Spl     */ 0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ 0,
            /* SrcGObjCo SPrm    */ 0,
        },
        {{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}}
    };
    const Vec l_tri_vtx[] = {
        {300.0f, -70.0f, 0.0f}, {-300.0f, -70.0f, 0.0f},
        {-300.0f, 55.0f, 0.0f}, {300.0f, 55.0f, 0.0f},
        {300.0f, 180.0f, 0.0f}, {-300.0f, 180.0f, 0.0f}
    };
    const Vec l_offset_ball[] = {{-300.0f, 90.0f, 0.0f}, {300.0f, 90.0f, 0.0f}};
    const Vec l_offset_thunder[] = {{0.0f, 25.0f, 0.0f}, {0.0f, 85.0f, 0.0f}, {0.0f, 145.0f, 0.0f}};
}

/* 00000078-000002AC       .text chk_appear__13daObjTnTrap_cFv */
bool daObjTnTrap_c::chk_appear() {
    bool result = false;
    mSwitch = param_get_swSave();
    mSwitch2 = param_get_swSave2();
    mArg = param_get_arg0();
    mMapType = param_get_mapType();
    switch (mMapType) {
    case 0:
        if (dComIfGs_isEventBit(0x3a04) == 1) {
            if (mSwitch != 0xff && dComIfGs_isSwitch(mSwitch, home.roomNo) == 1) {
                if (dComIfGs_getTriforceNum() == 8 && mArg == 0) {
                    if (dComIfGs_isEventBit(0x2c01) == 1) {
                        if (mSwitch2 != 0xff && !dComIfGs_isSwitch(mSwitch2, home.roomNo)) {
                            mType = 2;
                            result = true;
                        }
                    } else {
                        mType = 1;
                        result = true;
                    }
                }
            } else {
                mType = 0;
                result = true;
            }
        }
        break;
    case 1:
        if (mSwitch != 0xff && !dComIfGs_isSwitch(mSwitch, home.roomNo)) {
            mType = 3;
            result = true;
        }
        break;
    case 2:
        if (mSwitch != 0xff) {
            if (!dComIfGs_isSwitch(mSwitch, home.roomNo)) {
                mType = 5;
                result = true;
            }
        } else {
            mType = 5;
            result = true;
        }
        break;
    default:
        JUT_ASSERT(380, 0);
        break;
    }
    return result;
}

/* 000002AC-00000344       .text set_mtx__13daObjTnTrap_cFv */
void daObjTnTrap_c::set_mtx() {
    mDoMtx_stack_c::transS(home.pos);
    mDoMtx_stack_c::XYZrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, -9000.0f, -94.0f);
    mDoMtx_stack_c::scaleM(scale.x, 100.0f, scale.z);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

/* 00000344-00000368       .text solidHeapCB__13daObjTnTrap_cFP10fopAc_ac_c */
BOOL daObjTnTrap_c::solidHeapCB(fopAc_ac_c* actor) {
    return static_cast<daObjTnTrap_c*>(actor)->create_heap();
}

/* 00000368-000003E4       .text create_heap__13daObjTnTrap_cFv */
bool daObjTnTrap_c::create_heap() {
    bool result = true;
    mpBgW = dBgW_NewSet((cBgD_t*)dComIfG_getObjectRes(l_arcname, 3), 1, &mBgMtx);
    if (!mpBgW) {
        result = false;
    }
    return result;
}

/* 000003E4-000005F8       .text particle_set__13daObjTnTrap_cFif */
void daObjTnTrap_c::particle_set(int layer, float offset) {
    if (mParticleSet[layer] == 1) {
        if (mOffsetY[layer] != offset) {
            particle_delete(layer);
        } else {
            return;
        }
    }
    cXyz pos;
    for (int i = 0; i < 2; ++i) {
        if (!mpBall[layer][i]) {
            pos.x = l_offset_ball[i].x;
            pos.y = offset + l_offset_ball[i].y;
            pos.z = l_offset_ball[i].z;
            mpBall[layer][i] = dComIfGp_particle_set(0x82ea, &home.pos, &shape_angle);
            mpBall[layer][i]->setEmitterTranslation(pos);
        }
    }
    for (int i = 0; i < 3; ++i) {
        if (!mpThunder[layer][i]) {
            pos.x = l_offset_thunder[i].x;
            pos.y = offset + l_offset_thunder[i].y;
            pos.z = l_offset_thunder[i].z;
            mpThunder[layer][i] = dComIfGp_particle_set(0x82eb, &home.pos, &shape_angle);
            mpThunder[layer][i]->setEmitterTranslation(pos);
        }
    }
    mOffsetY[layer] = offset;
    mParticleSet[layer] = 1;
}

/* 000005F8-000006A4       .text particle_delete__13daObjTnTrap_cFi */
void daObjTnTrap_c::particle_delete(int layer) {
    int i;
    if (mParticleSet[layer] == 1) {
        for (i = 0; i < 2; ++i) {
            if (mpBall[layer][i]) {
                mpBall[layer][i]->becomeInvalidEmitter();
                mpBall[layer][i] = NULL;
            }
        }
        for (i = 0; i < 3; ++i) {
            if (mpThunder[layer][i]) {
                mpThunder[layer][i]->becomeInvalidEmitter();
                mpThunder[layer][i] = NULL;
            }
        }
    }
    mParticleSet[layer] = 0;
}

/* 000006A4-0000072C       .text set_se__13daObjTnTrap_cFv */
void daObjTnTrap_c::set_se() {
    switch (mActionIdx) {
    case 1:
    case 2:
    case 3:
    case 4:
        fopAcM_seStartCurrent(this, 0x6239, 0);
        break;
    }
}

/* 0000072C-000008A0       .text set_tri__13daObjTnTrap_cFi */
void daObjTnTrap_c::set_tri(int layer) {
    static int table_idx[4][3] = {{0, 1, 2}, {0, 2, 3}, {3, 2, 5}, {3, 5, 4}};
    mDoMtx_stack_c::transS(home.pos.x, home.pos.y + mOffsetY[layer], home.pos.z);
    mDoMtx_stack_c::XYZrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    Vec vertices[3];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            vertices[j] = l_tri_vtx[table_idx[i][j]];
            mDoMtx_stack_c::multVec(&vertices[j], &vertices[j]);
        }
        mTri[layer][i].setPos(&vertices[0], &vertices[1], &vertices[2]);
    }
}

/* 000008A0-00000A10       .text chk_event_flg__13daObjTnTrap_cFv */
bool daObjTnTrap_c::chk_event_flg() {
    bool result = true;
    int action;
    switch (mType) {
    case 0:
        if (mSwitch != 0xff && dComIfGs_isSwitch(mSwitch, home.roomNo) == 1) {
            action = 4;
            if (mArg == 0) {
                action = 2;
                dComIfGs_onEventBit(0x3b40);
            }
            setup_action(action);
        }
        break;
    case 1:
        break;
    case 2:
        if (mActionIdx == 1) {
            setup_action(2);
        }
        break;
    case 3:
        if (mSwitch != 0xff && dComIfGs_isSwitch(mSwitch, home.roomNo) == 1) {
            daShip_c* const ship = dComIfGp_getShipActor();
            if (ship) {
                ship->offStateFlg(daShip_c::daSFLG_UNK800000_e);
                fopAcM_delete(this);
                result = false;
            }
        }
        break;
    case 5:
        if (mSwitch != 0xff && dComIfGs_isSwitch(mSwitch, home.roomNo) == 1) {
            fopAcM_delete(this);
            result = false;
        }
        break;
    }
    return result;
}

/* 00000A10-00000A98       .text set_em_set_offsetY__13daObjTnTrap_cFv */
void daObjTnTrap_c::set_em_set_offsetY() {
    if (mType == 5) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        if (player) {
            mOffsetY[0] = 180.0f * (int)((player->eyePos.y - home.pos.y) / 180.0f);
        }
    } else {
        mOffsetY[0] = 0.0f;
    }
}

/* 00000A98-00000C78       .text _create__13daObjTnTrap_cFv */
cPhs_State daObjTnTrap_c::_create() {
    cPhs_State phase = cPhs_ERROR_e;
    fopAcM_SetupActor(this, daObjTnTrap_c);
    if (fopAcM_IsFirstCreating(this)) {
        mAppear = chk_appear();
    }
    if (mAppear == 1) {
        phase = dComIfG_resLoad(&mPhase, l_arcname);
    }
    if (phase == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, solidHeapCB, 0x2e0)) {
            if (mType != 3 && dComIfG_Bgsp()->Regist(mpBgW, this)) {
                phase = cPhs_ERROR_e;
            } else {
                set_em_set_offsetY();
                set_mtx();
                mStts.Init(0xff, 0xff, this);
                for (int i = 0; i < 2; ++i) {
                    for (int j = 0; j < 4; ++j) {
                        mTri[i][j].Set(l_tri_src);
                        mTri[i][j].SetStts(&mStts);
                    }
                    set_tri(i);
                }
                int action = 0;
                if (mType == 3) {
                    action = 6;
                }
                setup_action(action);
            }
        } else {
            phase = cPhs_ERROR_e;
        }
    }
    return phase;
}

/* 00000F8C-00001050       .text _delete__13daObjTnTrap_cFv */
bool daObjTnTrap_c::_delete() {
    if (mAppear == 1) {
        dComIfG_resDelete(&mPhase, l_arcname);
        if (heap && mpBgW) {
            if (mpBgW->ChkUsed()) {
                dComIfG_Bgsp()->Release(mpBgW);
            }
            mpBgW = NULL;
        }
        for (int i = 0; i < 2; ++i) {
            particle_delete(i);
        }
    }
    return true;
}

/* 00001050-00001150       .text trap_off_wait_act_proc__13daObjTnTrap_cFv */
bool daObjTnTrap_c::trap_off_wait_act_proc() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (player && (player->current.pos - home.pos).absXZ() < 500.0f) {
        setup_action(1);
    }
    return chk_event_flg();
}

/* 00001150-00001384       .text trap_on_wait_act_proc__13daObjTnTrap_cFv */
bool daObjTnTrap_c::trap_on_wait_act_proc() {
    fopAc_ac_c* const player = dComIfGp_getPlayer(0);
    if (player) {
        if ((player->current.pos - home.pos).absXZ() > 500.0f) {
            setup_action(0);
        } else if (mType == 5) {
            f32 distance[2];
            int i;
            for (i = 0; i < 2; ++i) {
                distance[i] = player->current.pos.y - (90.0f + (home.pos.y + mOffsetY[i]));
                if (std::fabsf(distance[i]) > 150.0f) {
                    particle_delete(i);
                }
            }
            for (i = 0; i < 2; ++i) {
                if (std::fabsf(distance[i]) > 80.0f) {
                    f32 offset;
                    if (distance[i] > 0.0f) {
                        offset = 180.0f + mOffsetY[i];
                    } else {
                        offset = mOffsetY[i] - 180.0f;
                    }
                    if (std::fabsf(player->current.pos.y - (90.0f + (home.pos.y + offset))) <= 150.0f) {
                        particle_set(i ^ 1, offset);
                    }
                }
            }
        }
    }
    return chk_event_flg();
}

/* 00001384-00001448       .text demo_regist_wait_act_proc__13daObjTnTrap_cFv */
bool daObjTnTrap_c::demo_regist_wait_act_proc() {
    if (mEventIdx != -1) {
        if (eventInfo.checkCommandDemoAccrpt()) {
            setup_action(3);
        } else {
            fopAcM_orderOtherEventId(this, mEventIdx);
        }
    } else {
        if (mType == 2) {
            mEventIdx = dComIfGp_evmng_getEventIdx("break_tntrap2");
        } else {
            mEventIdx = dComIfGp_evmng_getEventIdx("break_tntrap");
        }
    }
    return true;
}

/* 00001448-000014F0       .text demo_wait_act_proc__13daObjTnTrap_cFv */
bool daObjTnTrap_c::demo_wait_act_proc() {
    if (dComIfGp_getPEvtManager()->getEventData(mEventIdx)) {
        int staff = dComIfGp_evmng_getMyStaffId("TnTrap");
        if (staff != -1 && !strcmp(dComIfGp_getPEvtManager()->getMyNowCutName(staff), "Delete")) {
            setup_action(5);
        }
    }
    return true;
}

/* 000014F0-000015B4       .text demo_wait2_act_proc__13daObjTnTrap_cFv */
bool daObjTnTrap_c::demo_wait2_act_proc() {
    bool result = true;
    s16 event = dComIfGp_evmng_getEventIdx("break_tntrap");
    if (dComIfGp_getPEvtManager()->getEventData(event)) {
        int staff = dComIfGp_evmng_getMyStaffId("TnTrap");
        if (staff != -1 && !strcmp(dComIfGp_getPEvtManager()->getMyNowCutName(staff), "Delete2")) {
            fopAcM_delete(this);
            result = false;
        }
    }
    return result;
}

/* 000015B4-000016A8       .text demo_end_wait_act_proc__13daObjTnTrap_cFv */
bool daObjTnTrap_c::demo_end_wait_act_proc() {
    bool result = true;
    if (dComIfGp_evmng_endCheck(mEventIdx)) {
        dComIfGp_event_reset();
        switch (mType) {
        case 0:
            mDoAud_seStart(0x806, NULL, 0, 0);
            break;
        case 1:
            break;
        case 2:
            if (mSwitch2 != 0xff) dComIfGs_onSwitch(mSwitch2, home.roomNo);
            break;
        }
        fopAcM_delete(this);
        result = false;
    }
    return result;
}

/* 000016A8-00001740       .text hide_wait_act_proc__13daObjTnTrap_cFv */
bool daObjTnTrap_c::hide_wait_act_proc() {
    if (mSwitch2 != 0xff && dComIfGs_isSwitch(mSwitch2, home.roomNo) == 1) {
        daShip_c* ship = dComIfGp_getShipActor();
        if (ship) {
            ship->onStateFlg(daShip_c::daSFLG_UNK800000_e);
            if (!dComIfG_Bgsp()->Regist(mpBgW, this)) {
                setup_action(0);
            }
        }
    }
    return false;
}

/* 00001740-00001744       .text dummy_proc__13daObjTnTrap_cFv */
void daObjTnTrap_c::dummy_proc() {
}

/* 00001744-00001790       .text trap_off_wait_act_init_proc__13daObjTnTrap_cFv */
void daObjTnTrap_c::trap_off_wait_act_init_proc() {
    for (int i = 0; i < 2; ++i) {
        particle_delete(i);
    }
}

/* 00001790-000017CC       .text trap_on_wait_act_init_proc__13daObjTnTrap_cFv */
void daObjTnTrap_c::trap_on_wait_act_init_proc() {
    set_em_set_offsetY();
    particle_set(0, mOffsetY[0]);
}

/* 000017CC-00001860       .text demo_regist_wait_act_init_proc__13daObjTnTrap_cFv */
void daObjTnTrap_c::demo_regist_wait_act_init_proc() {
    mOffsetY[0] = 0.0f;
    particle_set(0, 0.0f);
    if (mType == 2) {
        mEventIdx = dComIfGp_evmng_getEventIdx("break_tntrap2");
    } else {
        mEventIdx = dComIfGp_evmng_getEventIdx("break_tntrap");
    }
}

/* 00001860-00001890       .text demo_wait2_act_init_proc__13daObjTnTrap_cFv */
void daObjTnTrap_c::demo_wait2_act_init_proc() {
    mOffsetY[0] = 0.0f;
    particle_set(0, 0.0f);
}

/* 00001890-000018DC       .text demo_end_wait_act_init_proc__13daObjTnTrap_cFv */
void daObjTnTrap_c::demo_end_wait_act_init_proc() {
    for (int i = 0; i < 2; ++i) {
        particle_delete(i);
    }
}

/* 000018DC-00001AE4       .text setup_action__13daObjTnTrap_cFi */
void daObjTnTrap_c::setup_action(int action) {
    static Action act_proc[] = {
        &daObjTnTrap_c::trap_off_wait_act_proc,
        &daObjTnTrap_c::trap_on_wait_act_proc,
        &daObjTnTrap_c::demo_regist_wait_act_proc,
        &daObjTnTrap_c::demo_wait_act_proc,
        &daObjTnTrap_c::demo_wait2_act_proc,
        &daObjTnTrap_c::demo_end_wait_act_proc,
        &daObjTnTrap_c::hide_wait_act_proc
    };
    static InitAction act_init_proc[] = {
        &daObjTnTrap_c::trap_off_wait_act_init_proc,
        &daObjTnTrap_c::trap_on_wait_act_init_proc,
        &daObjTnTrap_c::demo_regist_wait_act_init_proc,
        &daObjTnTrap_c::dummy_proc,
        &daObjTnTrap_c::demo_wait2_act_init_proc,
        &daObjTnTrap_c::demo_end_wait_act_init_proc,
        &daObjTnTrap_c::dummy_proc
    };
    (this->*act_init_proc[action])();
    mAction = act_proc[action];
    mActionIdx = action;
}

/* 00001AE4-00001BE8       .text _execute__13daObjTnTrap_cFv */
bool daObjTnTrap_c::_execute() {
    int i;
    if (mpBgW && mpBgW->ChkUsed()) {
        mpBgW->Move();
    }
    for (i = 0; i < 2; ++i) {
        set_tri(i);
    }
    mStts.Move();
    if ((this->*mAction)() == 1) {
        for (i = 0; i < 2; ++i) {
            for (int j = 0; j < 4; ++j) {
                dComIfG_Ccsp()->Set(&mTri[i][j]);
            }
        }
        set_se();
    }
    return true;
}

/* 00001BE8-00001BF0       .text _draw__13daObjTnTrap_cFv */
bool daObjTnTrap_c::_draw() {
    return true;
}

/* 00001BF0-00001C10       .text daObjTnTrap_Create__FP10fopAc_ac_c */
static cPhs_State daObjTnTrap_Create(fopAc_ac_c* i_this) {
    return ((daObjTnTrap_c*)i_this)->_create();
}

/* 00001C10-00001C34       .text daObjTnTrap_Delete__FP13daObjTnTrap_c */
static BOOL daObjTnTrap_Delete(daObjTnTrap_c* i_this) {
    return ((daObjTnTrap_c*)i_this)->_delete();
}

/* 00001C34-00001C58       .text daObjTnTrap_Execute__FP13daObjTnTrap_c */
static BOOL daObjTnTrap_Execute(daObjTnTrap_c* i_this) {
    return ((daObjTnTrap_c*)i_this)->_execute();
}

/* 00001C58-00001C7C       .text daObjTnTrap_Draw__FP13daObjTnTrap_c */
static BOOL daObjTnTrap_Draw(daObjTnTrap_c* i_this) {
    return ((daObjTnTrap_c*)i_this)->_draw();
}

/* 00001C7C-00001C84       .text daObjTnTrap_IsDelete__FP13daObjTnTrap_c */
static BOOL daObjTnTrap_IsDelete(daObjTnTrap_c*) {
    return TRUE;
}

static actor_method_class l_daObjTnTrap_Method = {
    (process_method_func)daObjTnTrap_Create,
    (process_method_func)daObjTnTrap_Delete,
    (process_method_func)daObjTnTrap_Execute,
    (process_method_func)daObjTnTrap_IsDelete,
    (process_method_func)daObjTnTrap_Draw,
};

actor_process_profile_definition g_profile_Obj_TnTrap = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Obj_TnTrap_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjTnTrap_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Obj_TnTrap_e,
    /* Actor SubMtd */ &l_daObjTnTrap_Method,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
