/**
 * d_a_obj_YLzou.cpp
 * Object - Hyrule Castle - Link statue
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_obj_YLzou.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"
#include "JSystem/JUtility/JUTAssert.h"

namespace {
    const char l_arcname[] = "YLzou";
    const char l_move_ylzou_demo_name[] = "move_YLzou";
    const char l_go_up_stairs_demo_name[] = "go_up_stairs";
    const char l_go_up_stairs_demo2_name[] = "go_up_stairs2";
    const char* l_demo_name[] = {NULL, l_move_ylzou_demo_name, l_go_up_stairs_demo_name, l_go_up_stairs_demo2_name};
}

/* 000000EC-000002B8       .text set_start_type__12daObjYLzou_cFv */
void daObjYLzou_c::set_start_type() {
    int type = 0;
    int demo = 0;
    u8 opened = 0;
    int action;
    if (!dComIfGs_isEventBit(0x2d04)) {
        if (mSwitch != 0xff && !dComIfGs_isSwitch(mSwitch, home.roomNo)) {
            action = 0;
            demo = 1;
        } else {
            action = 6;
            opened = 1;
        }
    } else if (!dComIfGs_isEventBit(0x3a04)) {
        action = 6;
        opened = 1;
    } else if (!dComIfGs_isEventBit(0x3804)) {
        if (dComIfGs_isEventBit(0x3820) == 1) {
            action = 7;
            demo = 2;
        } else {
            action = 11;
        }
    } else if (!dComIfGs_isEventBit(0x2d02)) {
        action = 6;
        opened = 1;
    } else if (dComIfGs_getTriforceNum() < 8) {
        if (dComIfGs_isEventBit(0x3820) == 1) {
            action = 7;
            demo = 2;
        } else {
            action = 11;
        }
    } else {
        type = 1;
        if (!dComIfGs_isEventBit(0x2c01)) {
            action = 12;
        } else if (!dComIfGs_isEventBit(0x3980)) {
            action = 13;
            demo = 3;
        } else {
            action = 12;
        }
    }
    mActionIdx = action;
    mType = type;
    mDemo = demo;
    if (!opened) {
        dComIfGs_offEventBit(0x3820);
    } else {
        dComIfGs_onEventBit(0x3820);
    }
}

/* 000002B8-00000374       .text set_mtx__12daObjYLzou_cFv */
void daObjYLzou_c::set_mtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + mVibAmplitude * cM_ssin(mVibAngle), current.pos.z);
    mDoMtx_stack_c::XYZrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::scaleM(scale.x, scale.y, scale.z);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

/* 00000374-000003B0       .text init_mtx__12daObjYLzou_cFv */
void daObjYLzou_c::init_mtx() {
    mpModel->setBaseScale(scale);
    set_mtx();
}

/* 000003B0-000003D4       .text solidHeapCB__12daObjYLzou_cFP10fopAc_ac_c */
BOOL daObjYLzou_c::solidHeapCB(fopAc_ac_c* actor) {
    return static_cast<daObjYLzou_c*>(actor)->create_heap();
}

/* 000003D4-000004F4       .text create_heap__12daObjYLzou_cFv */
bool daObjYLzou_c::create_heap() {
    bool result = true;
    static int bdl_table[] = {4, 5};
    static int dzb_table[] = {8, 9};
    J3DModelData* data = (J3DModelData*)dComIfG_getObjectRes(l_arcname, bdl_table[mType]);
    if (!data) {
        JUT_ASSERT(408, 0);
        result = false;
    } else {
        mpModel = mDoExt_J3DModel__create(data, 0x80000, 0x11000022);
        mpBgW = dBgW_NewSet((cBgD_t*)dComIfG_getObjectRes(l_arcname, dzb_table[mType]), 1, (Mtx*)mpModel->getBaseTRMtx());
        if (!mpModel || !mpBgW) {
            result = false;
        }
    }
    return result;
}

/* 000004F4-00000668       .text eff_set_slip_smoke_pos__12daObjYLzou_cFv */
void daObjYLzou_c::eff_set_slip_smoke_pos() {
    static cXyz base_pos[] = {cXyz(-240.0f, 0.0f, -240.0f), cXyz(240.0f, 0.0f, -240.0f)};
    mDoMtx_stack_c::transS(current.pos.x, -1550.0f + current.pos.y, -1200.0f + current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    for (int i = 0; i < 2; ++i) {
        mDoMtx_stack_c::push();
        mDoMtx_stack_c::transM(base_pos[i]);
        mSmoke[i].mPos.x = mDoMtx_stack_c::get()[0][3];
        mSmoke[i].mPos.y = mDoMtx_stack_c::get()[1][3];
        mSmoke[i].mPos.z = mDoMtx_stack_c::get()[2][3];
        mSmoke[i].mAngle.set(0, current.angle.y, 0);
        mDoMtx_stack_c::pop();
    }
}

/* 000006A4-000007AC       .text eff_smoke_slip_start__12daObjYLzou_cFv */
void daObjYLzou_c::eff_smoke_slip_start() {
    static cXyz scl(2.0f, 2.0f, 2.0f);
    eff_set_slip_smoke_pos();
    for (int i = 0; i < 2; ++i) {
        s8 roomNo = fopAcM_GetRoomNo(this);
        JPABaseEmitter* emitter = dComIfGp_particle_setToon(0x2022, &mSmoke[i].mPos, &mSmoke[i].mAngle, &scl, 0xb9, &mSmoke[i], roomNo);
        if (emitter) {
            emitter->setRate(2.0f);
            emitter->setDirectionalSpeed(15.0f);
            emitter->setSpread(0.15f);
            emitter->setLifeTime(30);
        }
    }
}

/* 000007AC-0000080C       .text eff_smoke_slip_end__12daObjYLzou_cFv */
void daObjYLzou_c::eff_smoke_slip_end() {
    for (int i = 0; i < 2; ++i) {
        mSmoke[i].end();
    }
}

/* 0000080C-0000086C       .text eff_smoke_slip_remove__12daObjYLzou_cFv */
void daObjYLzou_c::eff_smoke_slip_remove() {
    for (int i = 0; i < 2; ++i) {
        mSmoke[i].end();
    }
}

/* 0000086C-000008D0       .text eff_smoke_proc__12daObjYLzou_cFv */
void daObjYLzou_c::eff_smoke_proc() {
    if (mSmokeState != mSmokeNext) {
        if (mSmokeNext == 1) {
            eff_smoke_slip_start();
        } else {
            eff_smoke_slip_end();
        }
        mSmokeState = mSmokeNext;
    } else {
        if (mSmokeState == 1) eff_set_slip_smoke_pos();
    }
}

/* 000008D0-000009A4       .text vib_proc__12daObjYLzou_cFv */
void daObjYLzou_c::vib_proc() {
    if (mVibState != mVibNext) {
        mVibState = mVibNext;
    }
    switch (mVibState) {
    case 1:
        mVibAngle += 0x4000;
        mVibAmplitude += 0.05f;
        if (mVibAmplitude > 0.8f) {
            mVibAmplitude = 0.8f;
        }
        break;
    case 2:
        mVibAngle += 0x4000;
        mVibAmplitude -= 0.05f;
        if (mVibAmplitude < 0.4f) {
            mVibAmplitude = 0.4f;
        }
        break;
    default:
        mVibAngle += 0x4000;
        mVibAmplitude -= 0.1f;
        if (mVibAmplitude < 0.0f) {
            mVibAmplitude = 0.0f;
        }
        break;
    }
}

/* 000009A4-00000AE0       .text _create__12daObjYLzou_cFv */
cPhs_State daObjYLzou_c::_create() {
    fopAcM_SetupActor(this, daObjYLzou_c);
    if (fopAcM_IsFirstCreating(this)) {
        mSwitch = param_get_swSave();
        set_start_type();
    }
    cPhs_State phase = dComIfG_resLoad(&mPhase, l_arcname);
    static u32 solidHeapSize_table[] = {0x5840, 0x49e0};
    if (phase == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, solidHeapCB, solidHeapSize_table[mType])) {
            if (dComIfG_Bgsp()->Regist(mpBgW, this)) {
                phase = cPhs_ERROR_e;
            } else {
                fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
                setup_action(mActionIdx);
                init_mtx();
            }
        } else {
            phase = cPhs_ERROR_e;
        }
    }
    return phase;
}

/* 00000AE0-00000B7C       .text _delete__12daObjYLzou_cFv */
bool daObjYLzou_c::_delete() {
    dComIfG_resDelete(&mPhase, l_arcname);
    eff_smoke_slip_remove();
    if (heap && mpBgW) {
        if (mpBgW->ChkUsed()) {
            dComIfG_Bgsp()->Release(mpBgW);
        }
        mpBgW = NULL;
    }
    return true;
}

/* 00000B7C-00000BD8       .text move_ylzou_demo_start_wait_act_proc__12daObjYLzou_cFv */
void daObjYLzou_c::move_ylzou_demo_start_wait_act_proc() {
    if (mSwitch != 0xff && dComIfGs_isSwitch(mSwitch, home.roomNo) == 1) {
        setup_action(1);
    }
}

/* 00000BD8-00000C70       .text demo_regist_wait_act_proc__12daObjYLzou_cFv */
void daObjYLzou_c::demo_regist_wait_act_proc() {
    if (mEventIdx != -1) {
        if (eventInfo.checkCommandDemoAccrpt()) {
            setup_action(mActionIdx + 1);
        } else {
            fopAcM_orderOtherEventId(this, mEventIdx);
        }
    } else {
        mEventIdx = dComIfGp_evmng_getEventIdx(l_demo_name[mDemo]);
    }
}

/* 00000C70-00000D18       .text demo_vib_start_wait_act_proc__12daObjYLzou_cFv */
void daObjYLzou_c::demo_vib_start_wait_act_proc() {
    if (dComIfGp_getPEvtManager()->getEventData(mEventIdx)) {
        int staff = dComIfGp_evmng_getMyStaffId("YLzou");
        if (staff != -1 && !strcmp(dComIfGp_getPEvtManager()->getMyNowCutName(staff), "Vibrate")) {
            setup_action(mActionIdx + 1);
        }
    }
}

/* 00000D18-00000E08       .text demo_vib_act_proc__12daObjYLzou_cFv */
void daObjYLzou_c::demo_vib_act_proc() {
    if (dComIfGp_getPEvtManager()->getEventData(mEventIdx)) {
        int staff = dComIfGp_evmng_getMyStaffId("YLzou");
        if (staff != -1 && !strcmp(dComIfGp_getPEvtManager()->getMyNowCutName(staff), "Move")) {
            setup_action(mActionIdx + 1);
        }
    }
    fopAcM_seStartCurrent(this, 0x6225, 0);
}

/* 00000E08-00000F0C       .text move_ylzou_demo_move_act_proc__12daObjYLzou_cFv */
void daObjYLzou_c::move_ylzou_demo_move_act_proc() {
    if (current.pos.z < -680.0f + home.pos.z) {
        fopAcM_seStartCurrent(this, 0x6a27, 0);
        setup_action(5);
    } else {
        speedF += 0.1f;
        if (speedF > 6.0f) {
            speedF = 6.0f;
        }
        fopAcM_seStartCurrent(this, 0x6226, 0);
    }
}

/* 00000F0C-00001008       .text go_up_stairs_demo_move_act_proc__12daObjYLzou_cFv */
void daObjYLzou_c::go_up_stairs_demo_move_act_proc() {
    if (current.pos.z > home.pos.z) {
        fopAcM_seStartCurrent(this, 0x6a27, 0);
        setup_action(10);
    } else {
        speedF += 0.1f;
        if (speedF > 6.0f) {
            speedF = 6.0f;
        }
        fopAcM_seStartCurrent(this, 0x6226, 0);
    }
}

/* 00001008-000010A8       .text demo_end_wait_act_proc__12daObjYLzou_cFv */
void daObjYLzou_c::demo_end_wait_act_proc() {
    static int next_act_idx[] = {-1, 6, 11, 12};
    if (dComIfGp_evmng_endCheck(mEventIdx)) {
        dComIfGp_event_reset();
        if (mActionIdx == 14) {
            dComIfGs_onEventBit(0x3980);
        }
        fopAcM_OffStatus(this, 0x4000);
        setup_action(next_act_idx[mDemo]);
    }
}

/* 000010A8-000010AC       .text wait_act_proc__12daObjYLzou_cFv */
void daObjYLzou_c::wait_act_proc() {
}

/* 000010AC-000010D8       .text move_ylzou_demo_start_wait_act_init_proc__12daObjYLzou_cFv */
void daObjYLzou_c::move_ylzou_demo_start_wait_act_init_proc() {
    mEventIdx = -1;
    home.pos.set(0.0f, 0.0f, 0.0f);
    current.pos.set(0.0f, 0.0f, 0.0f);
}

/* 000010D8-00001174       .text demo_regist_wait_act_init_proc__12daObjYLzou_cFv */
void daObjYLzou_c::demo_regist_wait_act_init_proc() {
    if (mDemo != 1) {
        dComIfGp_getPEvtManager()->cancelStartDemo();
        fopAcM_OnStatus(this, 0x4000);
    }
    mEventIdx = dComIfGp_evmng_getEventIdx(l_demo_name[mDemo]);
    fopAcM_orderOtherEventId(this, mEventIdx);
}

/* 00001174-000011A4       .text move_ylzou_demo_vib_start_wait_act_init_proc__12daObjYLzou_cFv */
void daObjYLzou_c::move_ylzou_demo_vib_start_wait_act_init_proc() {
    dComIfGs_onEventBit(0x3820);
}

/* 000011A4-0000120C       .text move_ylzou_demo_vib_act_init_proc__12daObjYLzou_cFv */
void daObjYLzou_c::move_ylzou_demo_vib_act_init_proc() {
    dComIfGp_getVibration().StartQuake(6, 1, cXyz(0.0f, 1.0f, 0.0f));
    mVibNext = 1;
}

/* 0000120C-00001228       .text move_ylzou_demo_move_act_init_proc__12daObjYLzou_cFv */
void daObjYLzou_c::move_ylzou_demo_move_act_init_proc() {
    mSmokeNext = 1;
    mVibNext = 2;
    current.angle.y = -0x8000;
}

/* 00001228-000012C8       .text demo_end_wait_act_init_proc__12daObjYLzou_cFv */
void daObjYLzou_c::demo_end_wait_act_init_proc() {
    speedF = 0.0f;
    mSmokeNext = 0;
    if (mDemo != 3) {
        dComIfGp_getVibration().StopQuake(-1);
        dComIfGp_getVibration().StartShock(8, 1, cXyz(0.0f, 1.0f, 0.0f));
    }
    mVibNext = 0;
}

/* 000012C8-000012F4       .text open_wait_act_init_proc__12daObjYLzou_cFv */
void daObjYLzou_c::open_wait_act_init_proc() {
    home.pos.set(0.0f, 0.0f, -680.0f);
    current.pos.set(0.0f, 0.0f, -680.0f);
}

/* 000012F4-00001318       .text close_wait_act_init_proc__12daObjYLzou_cFv */
void daObjYLzou_c::close_wait_act_init_proc() {
    home.pos.set(0.0f, 0.0f, 0.0f);
    current.pos.set(0.0f, 0.0f, 0.0f);
}

/* 00001318-000013A4       .text go_up_stairs_demo_move_start_wait_act_init_proc__12daObjYLzou_cFv */
void daObjYLzou_c::go_up_stairs_demo_move_start_wait_act_init_proc() {
    current.angle.y = 0;
    home.pos.set(0.0f, 0.0f, 0.0f);
    current.pos.set(0.0f, 0.0f, -680.0f);
    dComIfGp_getVibration().StartQuake(6, 1, cXyz(0.0f, 1.0f, 0.0f));
    mVibNext = 1;
}

/* 000013A4-000013B8       .text go_up_stairs_demo_move_act_init_proc__12daObjYLzou_cFv */
void daObjYLzou_c::go_up_stairs_demo_move_act_init_proc() {
    mSmokeNext = 1;
    mVibNext = 2;
}

/* 000013B8-00001740       .text setup_action__12daObjYLzou_cFi */
void daObjYLzou_c::setup_action(int action) {
    static Action act_init_proc[] = {
        &daObjYLzou_c::move_ylzou_demo_start_wait_act_init_proc,
        &daObjYLzou_c::demo_regist_wait_act_init_proc,
        &daObjYLzou_c::move_ylzou_demo_vib_start_wait_act_init_proc,
        &daObjYLzou_c::move_ylzou_demo_vib_act_init_proc,
        &daObjYLzou_c::move_ylzou_demo_move_act_init_proc,
        &daObjYLzou_c::demo_end_wait_act_init_proc,
        &daObjYLzou_c::open_wait_act_init_proc,
        &daObjYLzou_c::demo_regist_wait_act_init_proc,
        &daObjYLzou_c::go_up_stairs_demo_move_start_wait_act_init_proc,
        &daObjYLzou_c::go_up_stairs_demo_move_act_init_proc,
        &daObjYLzou_c::demo_end_wait_act_init_proc,
        &daObjYLzou_c::close_wait_act_init_proc,
        &daObjYLzou_c::close_wait_act_init_proc,
        &daObjYLzou_c::demo_regist_wait_act_init_proc,
        &daObjYLzou_c::demo_end_wait_act_init_proc
    };
    static Action act_proc[] = {
        &daObjYLzou_c::move_ylzou_demo_start_wait_act_proc,
        &daObjYLzou_c::demo_regist_wait_act_proc,
        &daObjYLzou_c::demo_vib_start_wait_act_proc,
        &daObjYLzou_c::demo_vib_act_proc,
        &daObjYLzou_c::move_ylzou_demo_move_act_proc,
        &daObjYLzou_c::demo_end_wait_act_proc,
        &daObjYLzou_c::wait_act_proc,
        &daObjYLzou_c::demo_regist_wait_act_proc,
        &daObjYLzou_c::demo_vib_act_proc,
        &daObjYLzou_c::go_up_stairs_demo_move_act_proc,
        &daObjYLzou_c::demo_end_wait_act_proc,
        &daObjYLzou_c::wait_act_proc,
        &daObjYLzou_c::wait_act_proc,
        &daObjYLzou_c::demo_regist_wait_act_proc,
        &daObjYLzou_c::demo_end_wait_act_proc
    };
    (this->*act_init_proc[action])();
    mAction = act_proc[action];
    mActionIdx = action;
}

/* 00001740-000017D4       .text _execute__12daObjYLzou_cFv */
bool daObjYLzou_c::_execute() {
    fopAcM_posMoveF(this, NULL);
    set_mtx();
    if (mpBgW && mpBgW->ChkUsed()) {
        mpBgW->Move();
    }
    (this->*mAction)();
    eff_smoke_proc();
    vib_proc();
    return true;
}

/* 000017D4-00001834       .text _draw__12daObjYLzou_cFv */
bool daObjYLzou_c::_draw() {
    g_env_light.settingTevStruct(1, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);
    return true;
}

/* 00001834-00001854       .text daObjYLzou_Create__FP10fopAc_ac_c */
static cPhs_State daObjYLzou_Create(fopAc_ac_c* i_this) {
    return ((daObjYLzou_c*)i_this)->_create();
}

/* 00001854-00001878       .text daObjYLzou_Delete__FP12daObjYLzou_c */
static BOOL daObjYLzou_Delete(daObjYLzou_c* i_this) {
    return ((daObjYLzou_c*)i_this)->_delete();
}

/* 00001878-0000189C       .text daObjYLzou_Execute__FP12daObjYLzou_c */
static BOOL daObjYLzou_Execute(daObjYLzou_c* i_this) {
    return ((daObjYLzou_c*)i_this)->_execute();
}

/* 0000189C-000018C0       .text daObjYLzou_Draw__FP12daObjYLzou_c */
static BOOL daObjYLzou_Draw(daObjYLzou_c* i_this) {
    return ((daObjYLzou_c*)i_this)->_draw();
}

/* 000018C0-000018C8       .text daObjYLzou_IsDelete__FP12daObjYLzou_c */
static BOOL daObjYLzou_IsDelete(daObjYLzou_c*) {
    return TRUE;
}

static actor_method_class l_daObjYLzou_Method = {
    (process_method_func)daObjYLzou_Create,
    (process_method_func)daObjYLzou_Delete,
    (process_method_func)daObjYLzou_Execute,
    (process_method_func)daObjYLzou_IsDelete,
    (process_method_func)daObjYLzou_Draw,
};

actor_process_profile_definition g_profile_Obj_YLzou = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0003,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Obj_YLzou_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjYLzou_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Obj_YLzou_e,
    /* Actor SubMtd */ &l_daObjYLzou_Method,
    /* Status       */ fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
