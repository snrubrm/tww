/**
 * d_a_bo.cpp
 * Enemy - Boko Baba
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_bo.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_camera.h"
#include "f_op/f_op_camera.h"
#include "d/d_kankyo.h"
#include "d/d_particle.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "JSystem/J3DGraphAnimator/J3DNode.h"
#include "c/c_damagereaction.h"

/* 000000EC-000001E8       .text smoke_set__FP8bo_class */
void smoke_set(bo_class* i_this) {
    GXColor color = {0xA0, 0xA0, 0x80, 0xFF};

    i_this->mSmokeCb.remove();
    dComIfGp_particle_setToon(
        dPa_name::ID_AK_JT_ELEMENTSMOKE01,
        &i_this->m39C,
        &i_this->shape_angle,
        NULL,
        0xB9,
        &i_this->mSmokeCb,
        fopAcM_GetRoomNo(i_this)
    );

    JPABaseEmitter* emitter = i_this->mSmokeCb.getEmitter();
    if (emitter != NULL) {
        JGeometry::TVec3<f32> scale;
        scale.x = scale.y = scale.z = 3.0f;
        emitter->setGlobalParticleScale(scale);
        emitter->setRate(30.0f);
        emitter->setMaxFrame(1);
        emitter->setAwayFromCenterSpeed(10.0f);
        emitter->setAwayFromAxisSpeed(10.0f);
        emitter->becomeImmortalEmitter();
        i_this->mSmokeCb.setColor(color);
    }
}

/* 000001E8-00000638       .text nodeCallBack_UP__FP7J3DNodei */
static BOOL nodeCallBack_UP(J3DNode*, int) {
    /* Nonmatching */
}

/* 00000638-000006C8       .text nodeCallBack_DW__FP7J3DNodei */
static BOOL nodeCallBack_DW(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        bo_class* i_this = (bo_class*)model->getUserArea();
        if (i_this) {
            if (jntNo == 9) {
                MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);

                cXyz offset;
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->m304);
            }
        }
    }

    return TRUE;
}

/* 000006C8-0000079C       .text execute__22yodare_ato_PcallBack_cFP14JPABaseEmitterP15JPABaseParticle */
void yodare_ato_PcallBack_c::execute(JPABaseEmitter*, JPABaseParticle* ptcl) {
    JGeometry::TVec3<f32> gpos;
    ptcl->getGlobalPosition(gpos);

    f32 particleY = gpos.y;
    cXyz pos(gpos.x, 20.0f + gpos.y, gpos.z);
    mGndChk.SetPos(&pos);

    pos.y = dComIfG_Bgsp()->GroundCross(&mGndChk);
    if (pos.y > particleY) {
        dComIfGp_particle_set(dPa_name::ID_IT_SN_BKBABA_YODAPOTA00, &pos);
    }
}

/* 0000079C-00000930       .text draw_SUB__FP8bo_class */
void draw_SUB(bo_class* i_this) {
    fopAc_ac_c* actor = i_this;

    if (i_this->m2C0 == 0 || i_this->m2C0 == 2) {
        if (i_this->m2C0 == 2) {
            i_this->m304 = actor->current.pos;
            i_this->mpBrkAnm->play();
        }

        mDoMtx_stack_c::transS(i_this->m304.x, i_this->m304.y + i_this->m394, i_this->m304.z);
        mDoMtx_stack_c::YrotM(i_this->m348);
        mDoMtx_stack_c::XrotM(actor->shape_angle.x);
        mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
        mDoMtx_stack_c::transM(0.0f, -i_this->m394, 0.0f);
        i_this->mpMorfUP->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        i_this->mpMorfUP->calc();

        if (i_this->m2C0 == 0) {
            enemy_fire(&i_this->mEnemyFire);
        }
    }

    if (i_this->m2C0 == 0 || i_this->m2C0 == 1) {
        mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
        mDoMtx_stack_c::YrotM(actor->current.angle.y);
        mDoMtx_stack_c::XrotM(actor->shape_angle.x);
        mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
        i_this->mpMorfDW->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        i_this->mpMorfDW->calc();
    }

    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &actor->current.pos, &actor->tevStr);
}

/* 00000930-00000AD4       .text daBO_Draw__FP8bo_class */
static BOOL daBO_Draw(bo_class*) {
    /* Nonmatching */
}

/* 00000AD4-00000CD4       .text anm_init__FP8bo_classifUcfii */
void anm_init(bo_class* i_this, int bckFileIdx, float morf, unsigned char loopMode, float speed, int soundFileIdx, int modelType) {
    i_this->mCurrBckIdx = bckFileIdx;
    if (modelType == 0) {
        if (soundFileIdx >= 0) {
            i_this->mpMorfUP->setAnm(
                (J3DAnmTransform*)dComIfG_getObjectRes("BO", bckFileIdx),
                loopMode, morf, speed, 0.0f, -1.0f,
                dComIfG_getObjectRes("BO", soundFileIdx)
            );
        } else {
            i_this->mpMorfUP->setAnm(
                (J3DAnmTransform*)dComIfG_getObjectRes("BO", bckFileIdx),
                loopMode, morf, speed, 0.0f, -1.0f,
                NULL
            );
        }
    } else {
        if (soundFileIdx >= 0) {
            i_this->mpMorfDW->setAnm(
                (J3DAnmTransform*)dComIfG_getObjectRes("BO", bckFileIdx),
                loopMode, morf, speed, 0.0f, -1.0f,
                dComIfG_getObjectRes("BO", soundFileIdx)
            );
        } else {
            i_this->mpMorfDW->setAnm(
                (J3DAnmTransform*)dComIfG_getObjectRes("BO", bckFileIdx),
                loopMode, morf, speed, 0.0f, -1.0f,
                NULL
            );
        }
    }
}

/* 00000CD4-00000E24       .text shock_damage_check__FP8bo_class */
void shock_damage_check(bo_class*) {
    /* Nonmatching */
}

/* 00000E24-000013A4       .text head_atari_check__FP8bo_class */
void head_atari_check(bo_class*) {
    /* Nonmatching */
}

/* 000013A4-0000170C       .text nokezori_damage_rtn__FP8bo_class */
void nokezori_damage_rtn(bo_class*) {
    /* Nonmatching */
}

/* 0000170C-00001BB8       .text body_atari_check__FP8bo_class */
void body_atari_check(bo_class*) {
    /* Nonmatching */
}

/* 00001BB8-00002048       .text damage_check__FP8bo_class */
void damage_check(bo_class*) {
    /* Nonmatching */
}

/* 00002048-00002080       .text angle_initial__FP8bo_class */
void angle_initial(bo_class* i_this) {
    i_this->m33A.set(0, 0, 0);
    i_this->m352.set(0, 0, 0);
    i_this->m334.set(0, 0, 0);
    i_this->m340.set(0, 0, 0);
}

/* 00002080-00002170       .text wait_initial__FP8bo_class */
void wait_initial(bo_class*) {
    /* Nonmatching */
}

/* 00002170-000022F8       .text start_bakutsuki_event_camera__FP10fopAc_ac_c */
void start_bakutsuki_event_camera(fopAc_ac_c*) {
    /* Nonmatching */
}

/* 000022F8-0000233C       .text end_event_camera__FP10fopAc_ac_c */
void end_event_camera(fopAc_ac_c* actor) {
    dComIfGp_getCamera(0)->mCamera.EndEventCamera(fopAcM_GetID(actor));
}

/* 0000233C-00002FC4       .text bo_move__FP8bo_class */
void bo_move(bo_class*) {
    /* Nonmatching */
}

/* 00002FC4-0000380C       .text bo2_move__FP8bo_class */
void bo2_move(bo_class*) {
    /* Nonmatching */
}

/* 0000380C-00003AD4       .text bo3_move__FP8bo_class */
void bo3_move(bo_class*) {
    /* Nonmatching */
}

/* 00003AD4-00003E8C       .text bo4_move__FP8bo_class */
void bo4_move(bo_class*) {
    /* Nonmatching */
}

/* 00003E8C-000042B8       .text bo5_move__FP8bo_class */
void bo5_move(bo_class*) {
    /* Nonmatching */
}

/* 000042B8-000048B0       .text daBO_Execute__FP8bo_class */
static BOOL daBO_Execute(bo_class*) {
    /* Nonmatching */
}

/* 000048B0-000048B8       .text daBO_IsDelete__FP8bo_class */
static BOOL daBO_IsDelete(bo_class*) {
    return TRUE;
}

/* 000048B8-00004998       .text daBO_Delete__FP8bo_class */
static BOOL daBO_Delete(bo_class*) {
    /* Nonmatching */
}

/* 00004998-00004D08       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c*) {
    /* Nonmatching */
}

/* 00004D50-000050DC       .text daBO_Create__FP10fopAc_ac_c */
static cPhs_State daBO_Create(fopAc_ac_c*) {
    /* Nonmatching */
    static dCcD_SrcSph head_co_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_UNK2000,
            /* SrcObjAt  Atp     */ 1,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsPlayer_e | cCcD_CoSPrm_VsEnemy_e,
            /* SrcGObjAt Se      */ dCcG_SE_UNK6,
            /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
            /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ 0,
            /* SrcGObjTg Se      */ 0,
            /* SrcGObjTg HitMark */ 0,
            /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e | dCcG_TgSPrm_NoHitMark_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 15.0f,
        }},
    };
    static dCcD_SrcSph foot_co_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ 0,
            /* SrcObjTg  SPrm    */ 0,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsPlayer_e | cCcD_CoSPrm_VsGrpAll_e,
            /* SrcGObjAt Se      */ 0,
            /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
            /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ 0,
            /* SrcGObjTg Se      */ 0,
            /* SrcGObjTg HitMark */ 0,
            /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e | dCcG_TgSPrm_NoHitMark_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 15.0f,
        }},
    };
    static dCcD_SrcCyl body_cyl_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
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
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoHitMark_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGCylS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 0.0f,
            /* Height */ 0.0f,
        }},
    };
}

static actor_method_class l_daBO_Method = {
    (process_method_func)daBO_Create,
    (process_method_func)daBO_Delete,
    (process_method_func)daBO_Execute,
    (process_method_func)daBO_IsDelete,
    (process_method_func)daBO_Draw,
};

actor_process_profile_definition g_profile_BO = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_BO_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(bo_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_BO_e,
    /* Actor SubMtd */ &l_daBO_Method,
    /* Status       */ fopAcStts_SHOWMAP_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
