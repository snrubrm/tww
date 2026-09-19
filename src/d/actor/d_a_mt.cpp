/**
 * d_a_mt.cpp
 * Enemy - Magtail
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_mt.h"
#include "d/d_com_inf_game.h"
#include "d/d_cc_d.h"
#include "d/d_s_play.h"
#include "d/d_snap.h"
#include "d/d_bg_s_lin_chk.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "m_Do/m_Do_hostIO.h"
#include "JSystem/J3DGraphAnimator/J3DNode.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "SSystem/SComponent/c_lib.h"

static daMt_HIO_c l_HIO;
static s32 mt_all_count;
static s32 mt_fight_count;
static s32 j_index;

static u16 mt_tex_anm_idx[] = {0x36, 0x37};
static u16 mt_tex_max_frame[] = {6, 1};
static u8 br_no[] = {0, 1, 1, 2, 2, 2, 1, 0, 0, 0, 0};
static s16 br_ya[] = {-0x32C8, -0x2328, -0x0FA0, 0, 0, 0, 0, 0, 0, 0, 0};

/* 000000EC-000001E8       .text __ct__10daMt_HIO_cFv */
daMt_HIO_c::daMt_HIO_c() {
    m04 = 0;
    m05 = 0;
    m06 = 0;
    m07 = 0;
    m08 = 0x157C;
    m0C = -7500.0f;
    m10 = 15;
    m14 = 0.5f;
    m18 = 21.0f;
    m1C = 0.8f;
    m20 = 0.8f;
    m24 = 450.0f;
    m28 = 350.0f;
    m2C = 30.0f;
    m30 = 17;
    m32 = 40;
    m34 = 400.0f;
    m38 = 45;
    m3C = 43;
    m3E = 47;
    m3A = 70;
    m40 = 55.0f;
    m44 = 30.0f;
    m48 = 25.0f;
    m4C = 0.65f;
    m50 = -5;
    m52 = 0x2AF8;
    m54 = 0x258;
    m58 = 1.0f;
    m5C = 1.0f;
}

/* 00000230-000002D8       .text anm_init__FP8mt_classifUcfi */
void anm_init(mt_class* i_this, int bckIdx, float morf, unsigned char loopMode, float speed, int) {
    i_this->mpMorf[0]->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Mt", bckIdx), loopMode, morf, speed, 0.0f, -1.0f, NULL);
}

/* 000002D8-00000350       .text mt_a_d_sub__FPvPv */
void* mt_a_d_sub(void* ac1, void*) {
    if (fopAcM_IsActor(ac1) && fopAcM_GetName(ac1) == fpcNm_MT_e) {
        mt_class* i_this = (mt_class*)ac1;
        mt_all_count++;
        if (i_this->m454 == 1) {
            mt_fight_count++;
        }
    }
    return NULL;
}

/* 00000350-00000390       .text mt_check__FP8mt_class */
void mt_check(mt_class* i_this) {
    mt_fight_count = 0;
    mt_all_count = 0;
    fpcEx_Search(mt_a_d_sub, i_this);
}

/* 00000390-00000478       .text mt_bg_check__FP8mt_class */
void mt_bg_check(mt_class* i_this) {
    fopAc_ac_c* actor = i_this;
    f32 tmp = 40.0f + REG0_F(3);

    actor->current.pos.y -= tmp;
    actor->old.pos.y -= tmp;
    fopAcM_OnStatus(actor, fopAcStts_FREEZE_e);
    i_this->mAcch.CrrPos(*dComIfG_Bgsp());
    actor->current.pos.y += tmp;
    actor->old.pos.y += tmp;

    if (actor->home.pos.y - actor->current.pos.y > 1000.0f && actor->current.pos.y - i_this->mAcch.GetGroundH() > 5000.0f) {
        i_this->m1CBC = 1;
        fopAcM_delete(actor);
    }
}

/* 00000478-0000053C       .text tex_anm_set__FP8mt_classUs */
void tex_anm_set(mt_class* i_this, unsigned short idx) {
    J3DAnmTexPattern* btp;

    i_this->m345 = 1;
    btp = (J3DAnmTexPattern*)dComIfG_getObjectRes("Mt", mt_tex_anm_idx[idx]);
    i_this->m346 = mt_tex_max_frame[idx];
    i_this->mpBtp = btp;
    i_this->m344 = 0;
    i_this->mpBtp->setFrame(0.0f);

    u16 materialNum = btp->getUpdateMaterialNum();
    for (u16 i = 0; i < materialNum; i++) {
        i_this->mpTexNoAnm[i].setAnmTexPattern(btp);
    }
}

/* 0000053C-000005EC       .text mt_eye_tex_anm__FP8mt_class */
void mt_eye_tex_anm(mt_class* i_this) {
    if (i_this->m464 != 0) {
        i_this->m464--;
    } else {
        i_this->m464 = (s16)(cM_rndF(100.0f) + 50.0f);
        if (i_this->m345 == 0) {
            tex_anm_set(i_this, 0);
        }
    }

    if (i_this->m345 != 0) {
        if (i_this->m344 < i_this->m346) {
            i_this->m344++;
        } else {
            i_this->m345 = 0;
        }
    }
}

/* 000005EC-00000720       .text nodeCallBack_head__FP7J3DNodei */
static BOOL nodeCallBack_head(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        mt_class* i_this = (mt_class*)model->getUserArea();

        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            if (jntNo == 2) {
                cMtx_YrotM(*calc_mtx, -i_this->mJntRot[0].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[0].z);
            } else if (jntNo == 3) {
                cMtx_YrotM(*calc_mtx, i_this->mJntRot[15].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[15].z);
            } else {
                MtxScale(i_this->m18F0, i_this->m18F0, i_this->m18F0, 1);
            }
            MTXCopy(*calc_mtx, model->getAnmMtx(jntNo));
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 0000075C-0000098C       .text nodeCallBack_body__FP7J3DNodei */
static BOOL nodeCallBack_body(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        mt_class* i_this = (mt_class*)model->getUserArea();

        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            if (jntNo == 2) {
                cMtx_YrotM(*calc_mtx, -i_this->mJntRot[j_index + 1].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[j_index + 1].z);
            } else if (jntNo == 3) {
                cMtx_YrotM(*calc_mtx, -i_this->mJntRot[j_index + 2].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[j_index + 2].z);
            } else if (jntNo == 4) {
                cMtx_YrotM(*calc_mtx, i_this->mJntRot[j_index + 16].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[j_index + 16].z);
            } else if (jntNo == 5) {
                cMtx_YrotM(*calc_mtx, i_this->mJntRot[j_index + 17].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[j_index + 17].z);
                j_index += 2;
            }
            MTXCopy(*calc_mtx, model->getAnmMtx(jntNo));
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 0000098C-00000B28       .text nodeCallBack_tail__FP7J3DNodei */
static BOOL nodeCallBack_tail(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        mt_class* i_this = (mt_class*)model->getUserArea();

        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            if (jntNo == 2) {
                cMtx_YrotM(*calc_mtx, -i_this->mJntRot[13].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[13].z);
            } else if (jntNo == 3) {
                cMtx_YrotM(*calc_mtx, -i_this->mJntRot[14].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[14].z);
            } else if (jntNo == 4) {
                cMtx_YrotM(*calc_mtx, i_this->mJntRot[28].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[28].z);
            } else if (jntNo == 5) {
                cMtx_YrotM(*calc_mtx, i_this->mJntRot[29].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[29].z);
            }
            MTXCopy(*calc_mtx, model->getAnmMtx(jntNo));
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 00000B28-0000171C       .text body_control2__FP8mt_class */
void body_control2(mt_class* i_this) {
    /* Nonmatching */
    volatile f32 dummy;
    volatile f64 dummy_d;
    dummy = 3.0f;
    dummy = -200.0f;
    dummy = 10.0f;
    dummy_d = 0.5;
    dummy_d = 3.0;
    dummy = -5.0f;
    dummy = 35.0f;
    dummy = 5.0f;
    dummy = 20.0f;
    dummy = 0.01f;
    dummy = 2000.0f;
    dummy = (f32)i_this->m2E8;
    dummy = -10.0f;
    dummy = 32.0f;
    dummy = -17.0f;
    dummy = -30.0f;
    dummy = 2.0f;
    dummy = 60.0f;
    dummy = 2.5f;
    dummy = 0.025f;
    dummy = 80.0f;
    dummy = 3000.0f;
}

/* 00001B54-00001E44       .text wall_check_sub__FP8mt_classP4cXyzP4cXyz */
BOOL wall_check_sub(mt_class* i_this, cXyz* i_start, cXyz* i_end) {
    mt_class* a = i_this;
    cXyz* b = i_start;
    cXyz* c = i_end;
    dBgS_LinChk linChk;

    linChk.Set(b, c, a);
    if (dComIfG_Bgsp()->LineCross(&linChk)) {
        return TRUE;
    }
    return FALSE;
}

/* 00001E44-00001F10       .text body_wall_check__FP8mt_class */
void body_wall_check(mt_class* i_this) {
    for (int i = 1; i < 8; i++) {
        cXyz sp14 = i_this->m500[i];
        sp14.y += 50.0f;
        cXyz sp08 = i_this->m4A0[i];
        sp08.y += 50.0f;
        if (wall_check_sub(i_this, &sp14, &sp08)) {
            i_this->m4A0[i].x = i_this->m500[i].x;
            i_this->m4A0[i].z = i_this->m500[i].z;
        }
    }
}

/* 00001F10-000022D8       .text body_control1__FP8mt_class */
void body_control1(mt_class*) {
    /* Nonmatching */
}

/* 000022D8-000028BC       .text body_control3__FP8mt_class */
void body_control3(mt_class*) {
    /* Nonmatching */
}

/* 000028BC-00002AB0       .text body_control4__FP8mt_class */
void body_control4(mt_class*) {
    /* Nonmatching */
}

/* 00002AB0-00003008       .text body_control5__FP8mt_class */
void body_control5(mt_class*) {
    /* Nonmatching */
}

/* 00003008-00003210       .text br_draw__FP8mt_class */
void br_draw(mt_class* i_this) {
    fopAc_ac_c* actor = i_this;

    if (i_this->m18D4 == 0) {
        return;
    }

    MtxTrans(actor->eyePos.x, actor->eyePos.y, actor->eyePos.z, 0);
    cMtx_YrotM(*calc_mtx, actor->shape_angle.y);
    cMtx_XrotM(*calc_mtx, actor->shape_angle.x);
    cMtx_ZrotM(*calc_mtx, actor->shape_angle.z);

    f32 scale = l_HIO.m1C * (2.0f + REG0_F(4));
    MtxPush();
    cMtx_YrotM(*calc_mtx, br_ya[i_this->m18D4 - 1]);
    cMtx_XrotM(*calc_mtx, -0x4000);
    MtxScale(scale, scale, scale, 1);

    J3DModel* model = i_this->mpBrModel[br_no[i_this->m18D4 - 1]];
    g_env_light.setLightTevColorType(model, &actor->tevStr);
    MTXCopy(*calc_mtx, model->getBaseTRMtx());
    mDoExt_modelUpdateDL(model);
    MtxPull();

    cMtx_ZrotM(*calc_mtx, -0x8000);
    cMtx_YrotM(*calc_mtx, br_ya[i_this->m18D4 - 1]);
    cMtx_XrotM(*calc_mtx, -0x4000);
    MtxScale(scale, scale, scale, 1);

    model = i_this->mpBrModel2[br_no[i_this->m18D4 - 1]];
    g_env_light.setLightTevColorType(model, &actor->tevStr);
    MTXCopy(*calc_mtx, model->getBaseTRMtx());
    mDoExt_modelUpdateDL(model);
}

/* 00003210-00003360       .text daMt_shadowDraw__FP8mt_class */
static void daMt_shadowDraw(mt_class* i_this) {
    fopAc_ac_c* actor = i_this;

    if (!fopAcM_checkCarryNow(actor)) {
        cXyz pos(actor->current.pos.x, actor->current.pos.y + 150.0f + REG0_F(18), actor->current.pos.z);
        i_this->mShadowId = dComIfGd_setShadow(
            i_this->mShadowId,
            1,
            i_this->mpMorf[0]->getModel(),
            &pos,
            800.0f + REG0_F(19),
            40.0f + REG0_F(17),
            actor->current.pos.y,
            i_this->mAcch.GetGroundH(),
            i_this->mAcch.m_gnd,
            &actor->tevStr,
            0,
            1.0f,
            &dDlst_shadowControl_c::mSimpleTexObj
        );
        for (int i = 1; i < 8; i++) {
            dComIfGd_addRealShadow(i_this->mShadowId, i_this->mpMorf[i]->getModel());
        }
    } else {
        fopAc_ac_c* player = dComIfGp_getLinkPlayer();
        u32 shadowId = *(u32*)((u8*)player + 0x3614);
        if (shadowId != 0) {
            for (int i = 0; i < 8; i++) {
                dComIfGd_addRealShadow(shadowId, i_this->mpMorf[i]->getModel());
            }
        }
    }
}

/* 00003360-0000361C       .text daMt_Draw__FP8mt_class */
static BOOL daMt_Draw(mt_class* i_this) {
    cXyz sp14(0.0f, 0.0f, 0.0f);

    if (i_this->m2BB != 0) {
        return TRUE;
    }

    j_index = 0;
    for (int i = 0; i < 8; i++) {
        J3DModel* model = i_this->mpMorf[i]->getModel();
        if (i_this->m1906 == 0) {
            cXyz sp08;
            MTXMultVec(model->getBaseTRMtx(), &sp14, &sp08);
            g_env_light.settingTevStruct(0, &sp08, &i_this->tevStr);
        } else {
            i_this->scale.x = i_this->scale.y = i_this->scale.z = i_this->m1AA4;
            model->setBaseScale(i_this->scale);
        }

        g_env_light.setLightTevColorType(model, &i_this->tevStr);
        i_this->mpBtk[i]->entry(model->getModelData());
        i_this->mpBrk[i]->entry(model->getModelData());

        if (i_this->m2E4 == 0) {
            int brk_frame = i_this->m2E8 + i * l_HIO.m50;
            while (brk_frame < 0) {
                brk_frame += 41;
            }
            i_this->mpBrk[i]->setFrame(brk_frame);

            int btk_frame = i_this->m2EC + i * l_HIO.m50;
            while (btk_frame < 0) {
                btk_frame += 31;
            }
            i_this->mpBtk[i]->setFrame(btk_frame);
        } else {
            i_this->mpBrk[i]->setFrame(i_this->m2E8);
            i_this->mpBtk[i]->setFrame(i_this->m2EC);
        }

        if (i == 0) {
            model->getModelData()->setTexNoAnimator(i_this->mpBtp, i_this->mpTexNoAnm);
            i_this->mpBtp->setFrame(i_this->m344);
        }

        i_this->mpMorf[i]->updateDL();
    }

    br_draw(i_this);
    daMt_shadowDraw(i_this);
    fopAc_ac_c* actor = i_this;
    dSnap_RegistFig(DSNAP_TYPE_UNKAF, actor, 1.0f, 1.0f, 1.0f);
    return TRUE;
}

/* 0000361C-000037B0       .text bakuha__FP8mt_class */
void bakuha(mt_class*) {
    /* Nonmatching */
}

/* 000037B0-000042C4       .text mt_move__FP8mt_class */
void mt_move(mt_class*) {
    /* Nonmatching */
}

/* 000042C4-00005088       .text mt_fight__FP8mt_class */
void mt_fight(mt_class*) {
    /* Nonmatching */
}

/* 00005088-00005A04       .text mt_move_maru__FP8mt_class */
void mt_move_maru(mt_class*) {
    /* Nonmatching */
}

/* 00005B9C-00005C54       .text water_damage_se_set__FP8mt_class */
void water_damage_se_set(mt_class* i_this) {
    fopAc_ac_c* actor = i_this;
    fopAcM_seStart(actor, JA_SE_CM_MAGTAIL_WATER, 0);
    fopAcM_monsSeStart(actor, JA_SE_CV_MG_WATER, 0);
    i_this->m348 = 1;
}

/* 00005C54-0000614C       .text damage_check__FP8mt_class */
void damage_check(mt_class*) {
    /* Nonmatching */
}

/* 00006188-000074D4       .text daMt_Execute__FP8mt_class */
static BOOL daMt_Execute(mt_class*) {
    /* Nonmatching */
    return TRUE;
}

/* 00007CC4-00007CCC       .text daMt_IsDelete__FP8mt_class */
static BOOL daMt_IsDelete(mt_class*) {
    return TRUE;
}

/* 00007CCC-00007E18       .text daMt_Delete__FP8mt_class */
static BOOL daMt_Delete(mt_class* i_this) {
    fopAc_ac_c* actor = i_this;
    dComIfG_resDelete(&i_this->mPhs, "Mt");

    if (i_this->mp450 != NULL) {
        *(u32*)&i_this->mp450->shape_angle &= ~0x40;
        fopAc_ac_c* other = i_this->mp450;
        *(s32*)((u8*)other + 0x60) = -1;
        *(u32*)&other->shape_angle |= 1;
        i_this->mp450 = NULL;
    }

    for (int i = 0; i < 8; i++) {
        i_this->mPa[i].remove();
    }

    if (i_this->m1CBC != 0 && i_this->m2B6 != 0 && i_this->m2BA != 0 && !dComIfGs_isSwitch(i_this->m2BA, fopAcM_GetRoomNo(actor))) {
        fopAcM_prm_class* append = fopAcM_CreateAppend();
        append->base.position = actor->home.pos;
        append->base.angle = actor->home.angle;
        append->base.parameters = fopAcM_GetParam(actor);
        append->room_no = actor->current.roomNo;
        fpcSCtRq_Request(fpcLy_CurrentLayer(), fpcNm_MT_e, NULL, NULL, append);
    }

    return TRUE;
}

/* 00007E18-00008400       .text CallbackCreateHeap__FP10fopAc_ac_c */
static BOOL CallbackCreateHeap(fopAc_ac_c*) {
    /* Nonmatching */
    return TRUE;
}

/* 000084AC-000088A8       .text daMt_Create__FP10fopAc_ac_c */
static cPhs_State daMt_Create(fopAc_ac_c*) {
    /* Nonmatching */
    static dCcD_SrcSph eye_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
            /* SrcObjCo  SPrm    */ 0,
            /* SrcGObjAt Se      */ 0,
            /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
            /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ 0,
            /* SrcGObjTg Se      */ 0,
            /* SrcGObjTg HitMark */ 0,
            /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 30.0f,
        }},
    };
    return cPhs_ERROR_e;
}

static actor_method_class l_daMt_Method = {
    (process_method_func)daMt_Create,
    (process_method_func)daMt_Delete,
    (process_method_func)daMt_Execute,
    (process_method_func)daMt_IsDelete,
    (process_method_func)daMt_Draw,
};

actor_process_profile_definition g_profile_MT = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_MT_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(mt_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_MT_e,
    /* Actor SubMtd */ &l_daMt_Method,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_FREEZE_e | fopAcStts_UNK40000_e | fopAcStts_UNK8000000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
