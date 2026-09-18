/**
 * d_a_obj_mmrr.cpp
 * Object - Earth Temple mirror
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_obj_mmrr.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_s_lin_chk.h"
#include "f_op/f_op_actor_mng.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "res/Object/Mmirror.h"

namespace daObjMmrr {
namespace {
struct Attr_c {
    Vec vertices[5][3];
    f32 mirrorY;
    f32 mirrorZ;
    s16 mirrorAngle;
    Vec lightOffset;
    f32 lightRadius;
    f32 lightLength;
};
const Attr_c L_attr = {
    {
        {{0.0f, 0.0f, 0.0f}, {-50.0f, -230.0f, 0.0f}, {50.0f, -230.0f, 0.0f}},
        {{0.0f, 0.0f, 0.0f}, {-60.0f, -20.0f, 0.0f}, {-90.0f, -80.0f, 0.0f}},
        {{0.0f, 0.0f, 0.0f}, {90.0f, -80.0f, 0.0f}, {60.0f, -20.0f, 0.0f}},
        {{0.0f, 0.0f, 0.0f}, {-90.0f, -80.0f, 0.0f}, {-50.0f, -230.0f, 0.0f}},
        {{0.0f, 0.0f, 0.0f}, {50.0f, -230.0f, 0.0f}, {90.0f, -80.0f, 0.0f}},
    },
    445.0f, 0.0f, -2000, {0.0f, 320.0f, 40.0f}, 120.0f, 10000.0f,
};
}
const char Act_c::M_arcname[] = "Mmirror";
const dCcD_SrcTri Act_c::M_tri_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ 0,
        /* SrcObjTg  Type    */ AT_TYPE_LIGHT,
        /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsOther_e,
        /* SrcObjCo  SPrm    */ 0,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ 0,
        /* SrcGObjAt Spl     */ 0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ 0,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ 0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ 0,
        /* SrcGObjCo SPrm    */ 0,
    },
    {{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
};
const dCcD_SrcCps Act_c::M_cps_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ AT_TYPE_LIGHT,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_GrpAll_e,
        /* SrcObjTg  Type    */ 0,
        /* SrcObjTg  SPrm    */ 0,
        /* SrcObjCo  SPrm    */ 0,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ 0,
        /* SrcGObjAt Spl     */ 0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ 0,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ 0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ 0,
        /* SrcGObjCo SPrm    */ 0,
    },
    {{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 50.0f}},
};
}

/* 00000078-000000AC       .text end__Q29daObjMmrr5Eff_cFv */
void daObjMmrr::Eff_c::end() {
    if (mpEmitter) {
        mpEmitter->setGlobalAlpha(0);
    }
    dPa_followEcallBack::end();
}

/* 000000AC-000000EC       .text remove__Q29daObjMmrr5Eff_cFv */
void daObjMmrr::Eff_c::remove() {
    if (mpEmitter) {
        mpEmitter->setGlobalAlpha(0);
    }
    dPa_followEcallBack::remove();
}

/* 000000EC-00000110       .text solidHeapCB__Q29daObjMmrr5Act_cFP10fopAc_ac_c */
BOOL daObjMmrr::Act_c::solidHeapCB(fopAc_ac_c* actor) {
    return static_cast<Act_c*>(actor)->create_heap();
}

/* 00000110-00000360       .text create_heap__Q29daObjMmrr5Act_cFv */
bool daObjMmrr::Act_c::create_heap() {
    J3DModelData* bdl_Mmrr;
    J3DModelData* bdl_Yssmr00;

    bdl_Mmrr = static_cast<J3DModelData*>(dComIfG_getObjectRes(M_arcname, dRes_INDEX_MMIRROR_BDL_MMRR_e));
    JUT_ASSERT(488, bdl_Mmrr != 0);
    mpMirrorModel = mDoExt_J3DModel__create(bdl_Mmrr, 0x80000, 0x11000222);
    bdl_Yssmr00 = static_cast<J3DModelData*>(dComIfG_getObjectRes(M_arcname, dRes_INDEX_MMIRROR_BDL_YSSMR00_e));
    JUT_ASSERT(497, bdl_Yssmr00 != 0);
    mpLightModel = mDoExt_J3DModel__create(bdl_Yssmr00, 0x80000, 0x11000222);
    J3DAnmTextureSRTKey* btk_Mmrr = static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes(M_arcname, dRes_INDEX_MMIRROR_BTK_MMRR_e));
    JUT_ASSERT(508, btk_Mmrr != 0);
    BOOL mirror = mMirrorBtk.init(bdl_Mmrr, btk_Mmrr, TRUE, 2, 1.0f, 0, -1, false, FALSE);
    J3DAnmTextureSRTKey* btk_Yssmr00 = static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes(M_arcname, dRes_INDEX_MMIRROR_BTK_YSSMR00_e));
    JUT_ASSERT(515, btk_Yssmr00 != 0);
    BOOL light = mLightBtk.init(bdl_Yssmr00, btk_Yssmr00, TRUE, 2, 1.0f, 0, -1, false, FALSE);
    return mpMirrorModel && mpLightModel && mirror && light;
}

/* 00000360-00000434       .text init_cc__Q29daObjMmrr5Act_cFv */
void daObjMmrr::Act_c::init_cc() {
    for (int i = 0; i < 5; ++i) {
        mTriStts[i].Init(0xff, 0xff, this);
        mTri[i].Set(M_tri_src);
        mTri[i].SetStts(&mTriStts[i]);
        mTri[i].OnTgNoHitMark();
        mLightStts.Init(0xff, 0xff, this);
        mLightCps.Set(M_cps_src);
        mLightCps.SetStts(&mLightStts);
    }
    set_cc_rec_pos();
    set_cc_trans_pos();
}

/* 00000434-000005DC       .text set_cc_rec_pos__Q29daObjMmrr5Act_cFv */
void daObjMmrr::Act_c::set_cc_rec_pos() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, L_attr.mirrorY, L_attr.mirrorZ);
    mDoMtx_stack_c::XrotM(L_attr.mirrorAngle);
    for (int i = 0; i < 5; ++i) {
        cXyz pos, a, b, c;
        pos = L_attr.vertices[i][0];
        mDoMtx_stack_c::multVec(&pos, &a);
        pos = L_attr.vertices[i][1];
        mDoMtx_stack_c::multVec(&pos, &b);
        pos = L_attr.vertices[i][2];
        mDoMtx_stack_c::multVec(&pos, &c);
        mTri[i].setPos(&a, &b, &c);
    }
}

/* 000005DC-000009FC       .text set_cc_trans_pos__Q29daObjMmrr5Act_cFv */
void daObjMmrr::Act_c::set_cc_trans_pos() {
    cXyz offset(0.0f, 0.0f, L_attr.lightLength);
    cM3dGCpsS cps;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(L_attr.lightOffset.x, L_attr.lightOffset.y, L_attr.lightOffset.z);
    mDoMtx_stack_c::multVec(&cXyz::Zero, &cps.mStart);
    mDoMtx_stack_c::multVec(&offset, &cps.mEnd);
    cps.mRadius = L_attr.lightRadius;
    dBgS_MirLightLinChk check;
    check.Set((cXyz*)&cps.mStart, (cXyz*)&cps.mEnd, this);
    if (dComIfG_Bgsp()->LineCross(&check)) {
        cps.mEnd = check.GetCross();
    }
    mLightCps.cM3dGCps::Set(cps);
    mLightCps.CalcAtVec();
    mLightCps.GetAtVecP()->normalizeRS();
    mLightScale = cXyz(cps.mStart).abs(cps.mEnd) / L_attr.lightLength;
}

/* 00000F88-0000102C       .text set_cull__Q29daObjMmrr5Act_cFv */
void daObjMmrr::Act_c::set_cull() {
    f32 y, z;
    if (mLit) {
        y = -L_attr.lightLength * cM_ssin(L_attr.mirrorAngle);
        z = L_attr.lightLength * cM_scos(L_attr.mirrorAngle);
    } else {
        y = 0.0f;
        z = 0.0f;
    }
    fopAcM_setCullSizeBox(this, -160.0f, -1.0f, -160.0f, 160.0f, 680.0f + y, 160.0f + z);
}

/* 0000102C-0000122C       .text _create__Q29daObjMmrr5Act_cFv */
cPhs_State daObjMmrr::Act_c::_create() {
    fopAcM_SetupActor(this, Act_c);
    cPhs_State phase = dComIfG_resLoad(&mPhase, M_arcname);
    if (phase == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, solidHeapCB, 0x1a80)) {
            fopAcM_SetMtx(this, mpMirrorModel->getBaseTRMtx());
            init_mtx();
            init_cc();
            mBF8 = 0;
            mLightAmount = 0.0f;
            mLit = 0;
            set_cull();
        } else phase = cPhs_ERROR_e;
    }
    return phase;
}

/* 0000122C-0000126C       .text _delete__Q29daObjMmrr5Act_cFv */
bool daObjMmrr::Act_c::_delete() {
    eff_remove();
    dComIfG_resDelete(&mPhase, M_arcname);
    return true;
}

/* 0000126C-0000131C       .text set_mtx__Q29daObjMmrr5Act_cFv */
void daObjMmrr::Act_c::set_mtx() {
    cXyz lightScale(scale.x, scale.y, scale.z * mLightScale);
    mpLightModel->setBaseScale(lightScale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpMirrorModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mpLightModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 0000131C-00001358       .text init_mtx__Q29daObjMmrr5Act_cFv */
void daObjMmrr::Act_c::init_mtx() {
    mpMirrorModel->setBaseScale(scale);
    set_mtx();
}

/* 00001358-00001418       .text chk_light__Q29daObjMmrr5Act_cFv */
bool daObjMmrr::Act_c::chk_light() {
    bool result = false;
    if (dComIfGp_getDetect().chk_light(&eyePos)) {
        result = true;
    } else {
        for (int i = 0; i < 5; ++i) {
            dCcD_Tri* const tri = &mTri[i];
            if (tri->ChkTgHit()) {
                if (tri->GetNP()->inprod(*tri->GetTgRVecP()) < 0.0f) {
                    result = true;
                }
                tri->ClrTgHit();
            }
        }
    }
    return result;
}

/* 00001418-00001480       .text eff_start__Q29daObjMmrr5Act_cFv */
void daObjMmrr::Act_c::eff_start() {
    dComIfGp_particle_set(0x8294, &current.pos, &shape_angle, NULL, 0xff, &mEffect);
}

/* 00001480-000014AC       .text eff_stop__Q29daObjMmrr5Act_cFv */
void daObjMmrr::Act_c::eff_stop() {
    mEffect.end();
}

/* 000014AC-000014D0       .text eff_remove__Q29daObjMmrr5Act_cFv */
void daObjMmrr::Act_c::eff_remove() {
    mEffect.remove();
}

/* 000014D0-000016E8       .text _execute__Q29daObjMmrr5Act_cFv */
bool daObjMmrr::Act_c::_execute() {
    attention_info.position.x = current.pos.x;
    attention_info.position.y = 260.0f + current.pos.y;
    attention_info.position.z = current.pos.z;
    eyePos = attention_info.position;
    if (chk_light()) {
        cLib_chaseF(&mLightAmount, 1.0f, 0.2f);
    } else {
        cLib_chaseF(&mLightAmount, 0.0f, 0.2f);
    }
    const bool wasLit = mLit;
    mLit = mLightAmount > 0.999f;
    mMirrorBtk.play();
    mLightBtk.play();
    if (mBF8) {
        set_cc_rec_pos();
    }
    if (mBF8 || mLit) {
        set_cc_trans_pos();
    }
    set_mtx();
    set_cull();
    if (mLit) {
        if (!wasLit) {
            fopAcM_seStart(this, JA_SE_OBJ_MIRROR_REFLECT, 0);
            eff_start();
        }
        fopAcM_seStart(this, JA_SE_OBJ_MIRROR_LIGHT, 0);
    } else if (wasLit) {
        eff_stop();
    }
    for (int i = 0; i < 5; ++i) {
        dComIfG_Ccsp()->Set(&mTri[i]);
    }
    if (mLit) {
        dComIfG_Ccsp()->Set(&mLightCps);
    }
    mBF8 = 0;
    return true;
}

/* 000016E8-000017E8       .text _draw__Q29daObjMmrr5Act_cFv */
bool daObjMmrr::Act_c::_draw() {
    const bool lit = mLit;
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpMirrorModel, &tevStr);
    mMirrorBtk.entry(mpMirrorModel->getModelData());
    if (lit) {
        g_env_light.setLightTevColorType(mpLightModel, &tevStr);
        mLightBtk.entry(mpLightModel->getModelData());
    }
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpMirrorModel);
    dComIfGd_setList();
    if (lit) {
        mDoExt_modelUpdateDL(mpLightModel);
    }
    return true;
}
namespace daObjMmrr {
namespace {
/* 000017E8-00001808       .text Mthd_Create__Q29daObjMmrr26@unnamed@d_a_obj_mmrr_cpp@FPv */
cPhs_State Mthd_Create(void* i_this) {
    return ((daObjMmrr::Act_c*)i_this)->_create();
}

/* 00001808-0000182C       .text Mthd_Delete__Q29daObjMmrr26@unnamed@d_a_obj_mmrr_cpp@FPv */
BOOL Mthd_Delete(void* i_this) {
    return ((daObjMmrr::Act_c*)i_this)->_delete();
}

/* 0000182C-00001850       .text Mthd_Execute__Q29daObjMmrr26@unnamed@d_a_obj_mmrr_cpp@FPv */
BOOL Mthd_Execute(void* i_this) {
    return ((daObjMmrr::Act_c*)i_this)->_execute();
}

/* 00001850-00001874       .text Mthd_Draw__Q29daObjMmrr26@unnamed@d_a_obj_mmrr_cpp@FPv */
BOOL Mthd_Draw(void* i_this) {
    return ((daObjMmrr::Act_c*)i_this)->_draw();
}

/* 00001874-0000187C       .text Mthd_IsDelete__Q29daObjMmrr26@unnamed@d_a_obj_mmrr_cpp@FPv */
BOOL Mthd_IsDelete(void*) {
    return TRUE;
}

static actor_method_class Mthd_Table = {
    (process_method_func)Mthd_Create,
    (process_method_func)Mthd_Delete,
    (process_method_func)Mthd_Execute,
    (process_method_func)Mthd_IsDelete,
    (process_method_func)Mthd_Draw,
};
}; // namespace
}; // namespace daObjMmrr

actor_process_profile_definition g_profile_Obj_Mmrr = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Obj_Mmrr_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjMmrr::Act_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Obj_Mmrr_e,
    /* Actor SubMtd */ &daObjMmrr::Mthd_Table,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
