/**
 * d_a_npc_so.cpp
 * NPC - Fishman (blue fish)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_so.h"
#include "d/actor/d_a_esa.h"
#include "d/actor/d_a_tag_so.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_cc_d.h"
#include "d/d_lib.h"
#include "d/d_kankyo.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "SSystem/SComponent/c_math.h"

static daNpc_So_HIO_c l_HIO;

const u32 daNpc_So_c::m_heapsize = 0x1C00;
const char daNpc_So_c::m_arc_name[] = "So";

const dCcD_SrcSph daNpc_So_c::m_sph_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ 0,
        /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_UNK8 & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_UNK400000,
        /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsOther_e,
        /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsEnemy_e | cCcD_CoSPrm_NoCrr_e,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
        /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ dCcG_SE_UNK23,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e | dCcG_TgSPrm_NoHitMark_e,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGSphS
    {{
        /* Center */ {0.0f, 0.0f, 0.0f},
        /* Radius */ 60.0f,
    }},
};

/* 000000EC-000002A4       .text __ct__14daNpc_So_HIO_cFv */
daNpc_So_HIO_c::daNpc_So_HIO_c() {
    m2C = 0;
    m2D = 0;
    m2E = 0;
    m2F = 0;
    m30 = 0;
    m31 = 0;
    m34 = 3.0f;
    m38 = 0.9f;
    m3C = 100.0f;
    m40 = 2000.0f;
    m54 = 20000.0f;
    m5C = 30.0f;
    m44 = 0.7f;
    m48 = 2000.0f;
    m4C = 2.0f;
    m50 = 150.0f;
    m64 = 0x1F40;
    m66 = 0xFA0;
    m68 = -0x1F40;
    m6A = -0xFA0;
    m6C = 10.0f;
    mNpc.m04 = -33.0f;
    mNpc.mMaxHeadX = 0x1FFE;
    mNpc.mMaxHeadY = 0x1000;
    mNpc.mMaxBackboneX = 0x1000;
    mNpc.mMaxBackboneY = 0x2000;
    mNpc.mMinHeadX = -0xBB8;
    mNpc.mMinHeadY = -0x1000;
    mNpc.mMinBackboneX = -0x1000;
    mNpc.mMinBackboneY = -0x2000;
    mNpc.mMaxTurnStep = 0x250;
    mNpc.mMaxHeadTurnVel = 0x150;
    mNpc.mAttnYOffset = 50.0f;
    mNpc.mMaxAttnAngleY = 0x7FFF;
    mNpc.m22 = 0;
    mNpc.mMaxAttnDistXZ = 400.0f;
    m58 = 250.0f;
    m60 = 700.0f;
    m70 = 600.0f;
    m74 = 300.0f;
    m78 = 100.0f;
    m7C = 5;
    m80 = -1.4f;
    m84 = 18.0f;
    m88 = 120.0f;
    m8C = 300.0f;
    m90 = 15.0f;
    m94 = 0;
}

/* 000002A4-000002D0       .text searchEsa_CB__FPvPv */
static void* searchEsa_CB(void* i_actor, void* i_this) {
    return static_cast<daNpc_So_c*>(i_this)->_searchEsa(static_cast<fopAc_ac_c*>(i_actor));
}

/* 000002D0-000002F8       .text _searchEsa__10daNpc_So_cFP10fopAc_ac_c */
fopAc_ac_c* daNpc_So_c::_searchEsa(fopAc_ac_c* actor) {
    if (fopAcM_GetName(actor) == fpcNm_ESA_e) {
        esa_class* esa = static_cast<esa_class*>(actor);
        if (esa->field_0x298 == 0) {
            return actor;
        }
    }
    return NULL;
}

/* 000002F8-00000344       .text nodeControl_CB__FP7J3DNodei */
static BOOL nodeControl_CB(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DModel* model = j3dSys.getModel();
        daNpc_So_c* i_this = reinterpret_cast<daNpc_So_c*>(model->getUserArea());
        if (i_this) {
            i_this->_nodeControl(node, model);
        }
    }
    return TRUE;
}

/* 00000344-00000474       .text _nodeControl__10daNpc_So_cFP7J3DNodeP8J3DModel */
void daNpc_So_c::_nodeControl(J3DNode* node, J3DModel* model) {
    J3DJoint* joint = static_cast<J3DJoint*>(node);
    int jntNo = joint->getJntNo();
    mDoMtx_stack_c::copy(model->getAnmMtx(jntNo));

    if (m_jnt.getHeadJntNum() == jntNo) {
        cXyz offsetAtt(0.0f, 0.0f, 0.0f);
        cXyz offsetEye(24.0f, -16.0f, 0.0f);
        mDoMtx_stack_c::multVec(&offsetAtt, &mSoAttnPos);
        mDoMtx_YrotM(mDoMtx_stack_c::now, m_jnt.getHead_y());
        mDoMtx_ZrotM(mDoMtx_stack_c::now, m_jnt.getHead_x());
        mDoMtx_stack_c::multVec(&offsetEye, &mSoEyePos);
    } else if (m_jnt.getBackboneJntNum() == jntNo) {
        mDoMtx_XrotM(mDoMtx_stack_c::now, m_jnt.getBackbone_y());
        mDoMtx_ZrotM(mDoMtx_stack_c::now, m_jnt.getBackbone_x());
    }

    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
}

/* 000004B0-000004DC       .text searchTagSo_CB__FPvPv */
static void* searchTagSo_CB(void* i_actor, void* i_this) {
    return static_cast<daNpc_So_c*>(i_this)->_searchTagSo(static_cast<fopAc_ac_c*>(i_actor));
}

/* 000004DC-0000054C       .text _searchTagSo__10daNpc_So_cFP10fopAc_ac_c */
fopAc_ac_c* daNpc_So_c::_searchTagSo(fopAc_ac_c* actor) {
    if (fopAcM_GetName(actor) == fpcNm_TAG_SO_e) {
        daTag_So_c* tag = static_cast<daTag_So_c*>(actor);
        if (mTagId == tag->m290 && tag->m298 != 1) {
            mTagRadius = tag->mRadius;
            mHidePos = tag->current.pos;
            current.pos = mHidePos;
            return actor;
        }
    }
    return NULL;
}

/* 0000054C-00000578       .text searchMinigameTagSo_CB__FPvPv */
static void* searchMinigameTagSo_CB(void* i_actor, void* i_this) {
    return static_cast<daNpc_So_c*>(i_this)->_searchMinigameTagSo(static_cast<fopAc_ac_c*>(i_actor));
}

/* 00000578-000005C8       .text _searchMinigameTagSo__10daNpc_So_cFP10fopAc_ac_c */
fopAc_ac_c* daNpc_So_c::_searchMinigameTagSo(fopAc_ac_c* actor) {
    if (fopAcM_GetName(actor) == fpcNm_TAG_SO_e) {
        daTag_So_c* tag = static_cast<daTag_So_c*>(actor);
        if (tag->m298 == 1) {
            mMinigamePos = tag->current.pos;
            mMinigameAngle = tag->shape_angle.y;
            mMinigameTagFound = 1;
            return actor;
        }
    }
    return NULL;
}

/* 000005C8-000005E8       .text daNpc_So_XyCheckCB__FPvi */
static s16 daNpc_So_XyCheckCB(void* i_this, int i_itemBtn) {
    return static_cast<daNpc_So_c*>(i_this)->XyCheckCB(i_itemBtn);
}

/* 000005E8-00000680       .text XyCheckCB__10daNpc_So_cFi */
s16 daNpc_So_c::XyCheckCB(int i_itemBtn) {
    if (fopAcIt_Judge(searchEsa_CB, this) != NULL) {
        return FALSE;
    }
    s32 type = mType;
    if (type != 2) {
        return FALSE;
    }
    if (dComIfGp_checkPlayerStatus0(0, daPyStts0_SHIP_RIDE_e) &&
        dComIfGp_getSelectItem(i_itemBtn) == dItemNo_BIRD_BAIT_5_e)
    {
        return TRUE;
    }
    return FALSE;
}

/* 00000680-000006A0       .text daNpc_So_XyEventCB__FPvi */
static s16 daNpc_So_XyEventCB(void* i_this, int i_itemBtn) {
    return static_cast<daNpc_So_c*>(i_this)->XyEventCB(i_itemBtn);
}

/* 000006A0-000006EC       .text XyEventCB__10daNpc_So_cFi */
s16 daNpc_So_c::XyEventCB(int) {
    mEventIdx = dComIfGp_evmng_getEventIdx("SO_ESA_XY", 0xFF);
    return mEventIdx;
}

/* 000006EC-0000070C       .text createHeap_CB__FP10fopAc_ac_c */
static BOOL createHeap_CB(fopAc_ac_c* i_this) {
    return static_cast<daNpc_So_c*>(i_this)->_createHeap();
}

/* 0000070C-00000A20       .text _createHeap__10daNpc_So_cFv */
BOOL daNpc_So_c::_createHeap() {
    /* Nonmatching */
    volatile f32 dummy_f32;
    volatile f64 dummy_f64;
    dummy_f32 = 1.0f;
    dummy_f32 = 7.0f;
    dummy_f64 = 0.5;
    dummy_f64 = 3.0;
    dummy_f32 = 40.0f;
    dummy_f32 = 5.0f;
    dummy_f32 = -1.0f;
    dummy_f32 = 1.4f;
    dummy_f32 = 1.2f;
    dummy_f32 = 80.0f;
    dummy_f32 = 0.62f;
    dummy_f32 = 0.6f;
    dummy_f32 = 650.0f;
    dummy_f32 = 260.0f;
    dummy_f32 = 0.05f;
    dummy_f32 = 8.0f;
    dummy_f32 = 200.0f;
    dummy_f32 = 12.0f;
    dummy_f32 = 20.0f;
    dummy_f32 = 0.1f;
    dummy_f64 = 1.0;
    dummy_f32 = -200.0f;
    dummy_f32 = 1000.0f;
    dummy_f32 = 0.5f;
    dummy_f32 = 4.0f;
    dummy_f32 = -0.8f;
    dummy_f32 = 30000.0f;
    dummy_f32 = -300.0f;
    dummy_f32 = -50.0f;
    dummy_f32 = 90.0f;
    dummy_f32 = -400.0f;
    dummy_f32 = 0.3f;
    dummy_f32 = -150.0f;
    dummy_f32 = -40.0f;
    dummy_f32 = 0.4f;
    dummy_f32 = 22.0f;
    dummy_f32 = 110.0f;
    return FALSE;
}

/* 00000A20-00000A84       .text jntHitCreateHeap__10daNpc_So_cFv */
BOOL daNpc_So_c::jntHitCreateHeap() {
    static Vec cyl_offset_B[] = {
        {15.0f, 0.0f, 0.0f},
        {-15.0f, 0.0f, 0.0f},
    };
    static __jnt_hit_data_c search_data[] = {
        {JntHitType_SPH_THROW_e, 0, 2.0f, cyl_offset_B},
        {JntHitType_SPH_DELETE_e, 0, 2.0f, cyl_offset_B},
    };

    mpJntHit = JntHit_create(mpMorf2->getModel(), search_data, 2);
    if (mpJntHit) {
        fopAcM_SetJntHit(this, mpJntHit);
    } else {
        return FALSE;
    }
    return TRUE;
}

/* 00000A84-00000C8C       .text checkTgHit__10daNpc_So_cFv */
void daNpc_So_c::checkTgHit() {
    /* Nonmatching */
}

/* 00000C8C-00000CB0       .text offsetZero__10daNpc_So_cFv */
void daNpc_So_c::offsetZero() {
    mOffset.z = 0.1f;
    mOffset.y = 20.0f;
    mOffset.x = -1.0f;
}

/* 00000CB0-00000CD4       .text offsetDive__10daNpc_So_cFv */
void daNpc_So_c::offsetDive() {
    mOffset.z = 0.3f;
    mOffset.y = 10.0f;
    mOffset.x = -150.0f;
}

/* 00000CD4-00000CF8       .text offsetSwim__10daNpc_So_cFv */
void daNpc_So_c::offsetSwim() {
    mOffset.z = 0.3f;
    mOffset.y = 10.0f;
    mOffset.x = -40.0f;
}

/* 00000CF8-00000D1C       .text offsetAppear__10daNpc_So_cFv */
void daNpc_So_c::offsetAppear() {
    mOffset.z = 0.4f;
    mOffset.y = 22.0f;
    mOffset.x = 110.0f;
}

/* 00000D1C-00000E40       .text getMsg__10daNpc_So_cFv */
u32 daNpc_So_c::getMsg() {
    /* Nonmatching */
    return 0;
}

/* 00000E40-00001214       .text next_msgStatus__10daNpc_So_cFPUl */
u16 daNpc_So_c::next_msgStatus(u32*) {
    /* Nonmatching */
    return 0;
}

/* 00001214-000013A0       .text lookBack__10daNpc_So_cFv */
void daNpc_So_c::lookBack() {
    /* Nonmatching */
}

/* 000013A0-00001430       .text setAttention__10daNpc_So_cFv */
void daNpc_So_c::setAttention() {
    attention_info.position = mSoAttnPos;
    attention_info.position.y += l_HIO.mNpc.mAttnYOffset;
    f32 waterY = dLib_getWaterY(attention_info.position, mAcch2);
    if (attention_info.position.y <= waterY) {
        attention_info.position.y = waterY;
    }
    eyePos = mSoEyePos;
}

/* 00001430-00001524       .text setAnm__10daNpc_So_cFScb */
void daNpc_So_c::setAnm(signed char, bool) {
    /* Nonmatching */
}

/* 00001524-00001644       .text setAnmSwimSpeed__10daNpc_So_cFv */
void daNpc_So_c::setAnmSwimSpeed() {
    /* Nonmatching */
}

/* 00001644-000016E8       .text setMtx__10daNpc_So_cFv */
void daNpc_So_c::setMtx() {
    J3DModel* model = mpMorf2->getModel();
    model->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, mB34, 0.0f);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 000016E8-00001718       .text modeWaitInit__10daNpc_So_cFv */
void daNpc_So_c::modeWaitInit() {
    speedF = 0.0f;
    mSpeed = 0.0f;
    offsetAppear();
}

/* 00001718-0000171C       .text modeWait__10daNpc_So_cFv */
void daNpc_So_c::modeWait() {}

/* 0000171C-0000175C       .text modeHideInit__10daNpc_So_cFv */
void daNpc_So_c::modeHideInit() {
    mBDB = 1;
    offsetDive();
    mTagRadius = 0.0f;
}

/* 0000175C-00001778       .text modeHide__10daNpc_So_cFv */
void daNpc_So_c::modeHide() {
    current.pos = mHidePos;
}

/* 00001778-00001880       .text modeJumpInit__10daNpc_So_cFv */
void daNpc_So_c::modeJumpInit() {
    /* Nonmatching */
}

/* 00001880-000019F0       .text modeJump__10daNpc_So_cFv */
void daNpc_So_c::modeJump() {
    /* Nonmatching */
}

/* 000019F0-00001A6C       .text modeSwimInit__10daNpc_So_cFv */
void daNpc_So_c::modeSwimInit() {
    attention_info.flags = fopAc_Attn_TALKFLAG_NOTALK_e | fopAc_Attn_ACTION_SPEAK_e | fopAc_Attn_LOCKON_TALK_e;
    mA90 = cM_rndF(90.0f) + 30.0f;
    setAnm(2, false);
    m_jnt.onBackBoneLock();
    offsetDive();
}

/* 00001A6C-00001DB8       .text modeSwim__10daNpc_So_cFv */
void daNpc_So_c::modeSwim() {
    /* Nonmatching */
}

/* 00001DB8-00001DFC       .text modeNearSwimInit__10daNpc_So_cFv */
void daNpc_So_c::modeNearSwimInit() {
    setAnm(2, false);
    offsetDive();
    m_jnt.onBackBoneLock();
}

/* 00001DFC-00002144       .text modeNearSwim__10daNpc_So_cFv */
void daNpc_So_c::modeNearSwim() {
    /* Nonmatching */
}

/* 00002144-0000217C       .text modeEventFirstWaitInit__10daNpc_So_cFv */
void daNpc_So_c::modeEventFirstWaitInit() {
    mA90 = 150;
    mSpeed = 0.0f;
    speedF = 0.0f;
    offsetDive();
}

/* 0000217C-000022D0       .text modeEventFirstWait__10daNpc_So_cFv */
void daNpc_So_c::modeEventFirstWait() {
    /* Nonmatching */
}

/* 000022D0-00002314       .text modeEventFirstInit__10daNpc_So_cFv */
void daNpc_So_c::modeEventFirstInit() {
    offsetSwim();
    mSpeed = 0.0f;
    speedF = 0.0f;
    m_jnt.offBackBoneLock();
}

/* 00002314-000023C8       .text modeEventFirst__10daNpc_So_cFv */
void daNpc_So_c::modeEventFirst() {
    /* Nonmatching */
}

/* 000023C8-0000240C       .text modeEventFirstEndInit__10daNpc_So_cFv */
void daNpc_So_c::modeEventFirstEndInit() {
    offsetAppear();
    mSpeed = 0.0f;
    speedF = 0.0f;
    m_jnt.offBackBoneLock();
}

/* 0000240C-000024C0       .text modeEventFirstEnd__10daNpc_So_cFv */
void daNpc_So_c::modeEventFirstEnd() {
    /* Nonmatching */
}

/* 000024C0-00002504       .text modeEventEsaInit__10daNpc_So_cFv */
void daNpc_So_c::modeEventEsaInit() {
    offsetSwim();
    mSpeed = 0.0f;
    speedF = 0.0f;
    m_jnt.offBackBoneLock();
}

/* 00002504-00002594       .text modeEventEsa__10daNpc_So_cFv */
void daNpc_So_c::modeEventEsa() {
    /* Nonmatching */
}

/* 00002594-000025B0       .text modeEventMapopenInit__10daNpc_So_cFv */
void daNpc_So_c::modeEventMapopenInit() {
    mSpeed = 0.0f;
    speedF = 0.0f;
    m_jnt.offBackBoneLock();
}

/* 000025B0-00002670       .text modeEventMapopen__10daNpc_So_cFv */
void daNpc_So_c::modeEventMapopen() {
    /* Nonmatching */
}

/* 00002670-000026B4       .text modeEventBowInit__10daNpc_So_cFv */
void daNpc_So_c::modeEventBowInit() {
    offsetSwim();
    mSpeed = 0.0f;
    speedF = 0.0f;
    m_jnt.offBackBoneLock();
}

/* 000026B4-000027F8       .text modeEventBow__10daNpc_So_cFv */
void daNpc_So_c::modeEventBow() {
    /* Nonmatching */
}

/* 000027F8-0000283C       .text modeTalkInit__10daNpc_So_cFv */
void daNpc_So_c::modeTalkInit() {
    offsetAppear();
    setAnm(3, false);
    m_jnt.offBackBoneLock();
}

/* 0000283C-0000289C       .text modeTalk__10daNpc_So_cFv */
void daNpc_So_c::modeTalk() {
    if (talk(1) == fopMsgStts_BOX_CLOSED_e) {
        dComIfGp_event_onEventFlag(dEvtFlag_UNK8_e);
        modeProcInit(MODE_DISAPPEAR_e);
    }
}

/* 0000289C-0000296C       .text modeDisappearInit__10daNpc_So_cFv */
void daNpc_So_c::modeDisappearInit() {
    /* Nonmatching */
}

/* 0000296C-00002A90       .text modeDisappear__10daNpc_So_cFv */
void daNpc_So_c::modeDisappear() {
    /* Nonmatching */
}

/* 00002A90-00002B60       .text modeDebugInit__10daNpc_So_cFv */
void daNpc_So_c::modeDebugInit() {
    /* Nonmatching */
}

/* 00002B60-00002B64       .text modeDebug__10daNpc_So_cFv */
void daNpc_So_c::modeDebug() {}

/* 00002B64-00002B68       .text modeGetRupeeInit__10daNpc_So_cFv */
void daNpc_So_c::modeGetRupeeInit() {}

/* 00002B68-00002C6C       .text modeGetRupee__10daNpc_So_cFv */
void daNpc_So_c::modeGetRupee() {
    /* Nonmatching */
}

/* 00002C6C-00002CC0       .text modeEventTriForceInit__10daNpc_So_cFv */
void daNpc_So_c::modeEventTriForceInit() {
    mSpeed = 0.0f;
    speedF = 0.0f;
    m_jnt.offBackBoneLock();
    offsetAppear();
    setAnm(1, false);
}

/* 00002CC0-00002D80       .text modeEventTriForce__10daNpc_So_cFv */
void daNpc_So_c::modeEventTriForce() {
    /* Nonmatching */
}

/* 00002D80-00003110       .text modeProc__10daNpc_So_cFQ210daNpc_So_c6Proc_ei */
void daNpc_So_c::modeProc(daNpc_So_c::Proc_e, int) {
    /* Nonmatching */
}

/* 00003110-00003264       .text eventOrder__10daNpc_So_cFv */
void daNpc_So_c::eventOrder() {
    /* Nonmatching */
}

/* 00003264-0000330C       .text checkOrder__10daNpc_So_cFv */
void daNpc_So_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        mB70 = 0;
    } else if (eventInfo.checkCommandTalk()) {
        if (mB70 == 1 || mB70 == 2) {
            if (dComIfGp_event_chkTalkXY()) {
                modeProcInit(MODE_EVENT_ESA_e);
            }
            mB70 = 0;
        }
    }
}

/* 0000330C-000033F4       .text setScale__10daNpc_So_cFv */
void daNpc_So_c::setScale() {
    /* Nonmatching */
}

/* 000033F4-00003844       .text _execute__10daNpc_So_cFv */
bool daNpc_So_c::_execute() {
    /* Nonmatching */
    return true;
}

/* 00003844-000038E0       .text debugDraw__10daNpc_So_cFv */
void daNpc_So_c::debugDraw() {
    /* Nonmatching */
}

/* 000038E0-00003954       .text hudeDraw__10daNpc_So_cFv */
void daNpc_So_c::hudeDraw() {
    g_env_light.setLightTevColorType(mpHudeModel, &tevStr);
    mDoMtx_stack_c::copy(mpMorf2->getModel()->getAnmMtx(11));
    mpHudeModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoExt_modelUpdateDL(mpHudeModel);
}

/* 00003954-00003B00       .text _draw__10daNpc_So_cFv */
bool daNpc_So_c::_draw() {
    /* Nonmatching */
    return true;
}

/* 00003B00-00003DF8       .text createInit__10daNpc_So_cFv */
void daNpc_So_c::createInit() {
    /* Nonmatching */
}

/* 00003DF8-00003E24       .text getArg__10daNpc_So_cFv */
void daNpc_So_c::getArg() {
    mPrmAngleX = home.angle.x;
    s16 tmp = mPrmAngleX;
    u32 utmp = tmp;
    if (utmp == 0xFFFF || tmp == 0) {
        mPrmAngleX = 1;
    }
}

/* 00003E24-00003F38       .text _create__10daNpc_So_cFv */
cPhs_State daNpc_So_c::_create() {
    /* Nonmatching */
    return cPhs_ERROR_e;
}

/* 000046E4-00004754       .text _delete__10daNpc_So_cFv */
bool daNpc_So_c::_delete() {
    if (dComIfGp_getMiniGameType() == 8) {
        dComIfGp_endMiniGame(8);
    }
    dComIfG_resDelete(&mPhs, m_arc_name);
    mRipple.end();
    return true;
}

/* 00004754-00004774       .text daNpc_SoCreate__FPv */
static cPhs_State daNpc_SoCreate(void* i_this) {
    return ((daNpc_So_c*)i_this)->_create();
}

/* 00004774-00004798       .text daNpc_SoDelete__FPv */
static BOOL daNpc_SoDelete(void* i_this) {
    return ((daNpc_So_c*)i_this)->_delete();
}

/* 00004798-000047BC       .text daNpc_SoExecute__FPv */
static BOOL daNpc_SoExecute(void* i_this) {
    return ((daNpc_So_c*)i_this)->_execute();
}

/* 000047BC-000047E0       .text daNpc_SoDraw__FPv */
static BOOL daNpc_SoDraw(void* i_this) {
    return ((daNpc_So_c*)i_this)->_draw();
}

/* 000047E0-000047E8       .text daNpc_SoIsDelete__FPv */
static BOOL daNpc_SoIsDelete(void*) {
    return TRUE;
}

#include "d/actor/d_a_npc_so_cut.inc"

static actor_method_class daNpc_SoMethodTable = {
    (process_method_func)daNpc_SoCreate,
    (process_method_func)daNpc_SoDelete,
    (process_method_func)daNpc_SoExecute,
    (process_method_func)daNpc_SoIsDelete,
    (process_method_func)daNpc_SoDraw,
};

actor_process_profile_definition g_profile_NPC_SO = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0003,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_SO_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_So_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_SO_e,
    /* Actor SubMtd */ &daNpc_SoMethodTable,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
