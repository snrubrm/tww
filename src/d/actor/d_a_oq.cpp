/**
 * d_a_oq.cpp
 * Enemy - Octorok
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_oq.h"
#include "d/actor/d_a_sea.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_com_inf_game.h"
#include "d/d_jnt_hit.h"
#include "d/d_material.h"
#include "d/d_s_play.h"
#include "d/d_snap.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_kankyo_mng.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "SSystem/SComponent/c_lib.h"
#include "SSystem/SComponent/c_math.h"
#include "res/Object/Oq.h"

static s16 hazure_shoot_timer;

/* 000000EC-00000208       .text nodeCallBack__FP7J3DNodei */
static BOOL nodeCallBack(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        oq_class* i_this = (oq_class*)model->getUserArea();
        if (i_this != NULL) {
            cXyz offset;
            if (jntNo == OQ_JNT_KUTI_e || jntNo == OQ_JNT_KUTI2_e) {
                MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            }
            if (jntNo == OQ_JNT_KUTI2_e) {
                offset.x = 40.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->mMouthPos2);
            }
            if (jntNo == OQ_JNT_KUTI2_e) {
                offset.x = 20.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->mMouthPos);
            }
            if (jntNo == OQ_JNT_KUTI_e || jntNo == OQ_JNT_KUTI2_e) {
                MTXCopy(*calc_mtx, model->getAnmMtx(jntNo));
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }
    return TRUE;
}

/* 00000208-00000310       .text draw_SUB__FP8oq_class */
void draw_SUB(oq_class* i_this) {
    J3DModel* model = i_this->mpMorf->getModel();
    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y + i_this->mDrawOffsetY, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
    mDoMtx_stack_c::scaleM(i_this->scale.x, i_this->scale.y, i_this->scale.z);
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    if (i_this->mType == 0 || i_this->mType == 1 || i_this->mType == 4 || i_this->mType == 5) {
        i_this->mpMorf->calc();
        enemy_fire(&i_this->mEnemyFire);
    }

    if (i_this->mType != 3 && i_this->mType != 2) {
        g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &i_this->current.pos, &i_this->tevStr);
    }
}

/* 00000310-000004DC       .text daOQ_Draw__FP8oq_class */
static BOOL daOQ_Draw(oq_class* i_this) {
    if (i_this->mType == 3 || i_this->mType == 2) {
        return TRUE;
    }

    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.setLightTevColorType(model, &i_this->tevStr);

    if (i_this->mType == 1 || i_this->mType == 4 || i_this->mType == 5) {
        dSnap_RegistFig(DSNAP_TYPE_UNKBA, i_this, 2.75f, 2.75f, 2.75f);
    }
    if (i_this->mType == 0) {
        dSnap_RegistFig(DSNAP_TYPE_UNKBA, i_this, 1.0f, 1.0f, 1.0f);
    }

    if (i_this->mType == 0 || i_this->mType == 1 || i_this->mType == 4 || i_this->mType == 5) {
        if (i_this->mEnemyIce.mFreezeTimer > 20) {
            dMat_control_c::iceEntryDL(i_this->mpMorf, -1, NULL);
            return TRUE;
        }
    }
    if (i_this->mType == 0 || i_this->mType == 1 || i_this->mType == 4 || i_this->mType == 5) {
        if (i_this->mType != 0) {
            i_this->mpBrk->entry(model->getModelData());
            i_this->mpBrk->setFrame(i_this->mBrkFrame);
        }
        i_this->mpMorf->entryDL();
        if (i_this->mType != 0) {
            i_this->mpBrk->remove(model->getModelData());
        }
    } else if (i_this->mType == 6) {
        i_this->mpMorf->updateDL();
        dComIfGd_setSimpleShadow2(
            &i_this->current.pos,
            i_this->mAcch.GetGroundH(),
            40.0f,
            i_this->mAcch.m_gnd,
            0,
            1.0f,
            dDlst_shadowControl_c::getSimpleTex()
        );
    }
    return TRUE;
}

/* 000004DC-00000608       .text anm_init__FP8oq_classifUcfi */
void anm_init(oq_class* i_this, int bckFileIdx, float morf, unsigned char loopMode, float speed, int soundFileIdx) {
    i_this->mBckIdx = bckFileIdx;
    if (soundFileIdx >= 0) {
        void* soundAnm = dComIfG_getObjectRes("OQ", soundFileIdx);
        J3DAnmTransform* bckAnm = (J3DAnmTransform*)dComIfG_getObjectRes("OQ", bckFileIdx);
        i_this->mpMorf->setAnm(bckAnm, loopMode, morf, speed, 0.0f, -1.0f, soundAnm);
    } else {
        J3DAnmTransform* bckAnm = (J3DAnmTransform*)dComIfG_getObjectRes("OQ", bckFileIdx);
        i_this->mpMorf->setAnm(bckAnm, loopMode, morf, speed, 0.0f, -1.0f, NULL);
    }
}

/* 00000608-00000D50       .text body_atari_check__FP8oq_class */
void body_atari_check(oq_class*) {
    /* Nonmatching - dummy pool host for later 100% funcs */
    volatile f32 dummy;
    dummy = 300.0f;
    dummy = 175.0f;
    dummy = 5.0f;
    dummy = 80.0f;
    dummy = 2.0f;
}

/* 00000D8C-00000EC4       .text BG_check__FP8oq_class */
void BG_check(oq_class* i_this) {
    u8 do_crr = 0;
    if (i_this->mType == 0) {
        i_this->mAcchCir.SetWall(80.0f + REG8_F(10), 100.0f + REG8_F(11));
        do_crr = 1;
    }
    if (i_this->mType == 1 || i_this->mType == 4 || i_this->mType == 5) {
        i_this->mAcchCir.SetWall(40.0f + REG8_F(10), 90.0f + REG8_F(11));
        do_crr = 0;
    } else if (i_this->mType == 6) {
        i_this->mAcchCir.SetWall(40.0f, 30.0f);
        do_crr = 1;
    }
    if (do_crr) {
        i_this->current.pos.y -= i_this->mCorrectionOffsetY;
        i_this->old.pos.y -= i_this->mCorrectionOffsetY;
        i_this->mAcch.CrrPos(*dComIfG_Bgsp());
        i_this->current.pos.y += i_this->mCorrectionOffsetY;
        i_this->old.pos.y += i_this->mCorrectionOffsetY;
    }
}

/* 00000EC4-000011D8       .text Line_check__FP8oq_class4cXyz */
BOOL Line_check(oq_class* i_this, cXyz destPos) {
    fopAc_ac_c* actor = i_this;
    dBgS_LinChk linChk;
    cXyz start = actor->current.pos;
    start.y += 40.0f;
    linChk.Set(&start, &destPos, actor);
    if (dComIfG_Bgsp()->LineCross(&linChk)) {
        return TRUE;
    }
    return FALSE;
}

/* 00001610-000016AC       .text shibuki_set__FP8oq_class4cXyzf */
void shibuki_set(oq_class* i_this, cXyz pos, float scale) {
    if (daSea_ChkArea(i_this->current.pos.x, i_this->current.pos.z)) {
        pos.y = daSea_calcWave(i_this->current.pos.x, i_this->current.pos.z);
    } else if (i_this->mAcch.MaskWaterIn()) {
        pos.y = i_this->mAcch.m_wtr.GetHeight();
    }

    fopKyM_createWpillar(&pos, scale, 1.0f, 0);
}

/* 000016AC-00001930       .text sea_water_check__FP8oq_class */
BOOL sea_water_check(oq_class* i_this) {
    fopAc_ac_c* actor = i_this;
    u8 hit = 0;
    i_this->mRipplePos = actor->current.pos;
    i_this->mRipplePos.y = i_this->mHomePos.y;
    actor->gravity = -3.0f;

    if (daSea_ChkArea(actor->current.pos.x, actor->current.pos.z)) {
        f32 waveY = daSea_calcWave(actor->current.pos.x, actor->current.pos.z);
        i_this->mRipplePos.y = waveY;
        if (actor->current.pos.y < 40.0f + waveY) {
            actor->gravity = 0.0f;
            i_this->mBobAngle += 0x800;
            f32* pPosY = &actor->current.pos.y;
            cLib_addCalc2(pPosY, waveY - (45.0f + 15.0f * cM_ssin(i_this->mBobAngle)), 1.0f, 30.0f);
            hit = 1;
        }
    } else if (i_this->mAcch.MaskWaterIn()) {
        hit = 2;
        f32 f3 = 20.0f + i_this->mAcch.m_wtr.GetHeight();
        i_this->mRipplePos.y = i_this->mAcch.m_wtr.GetHeight();
        actor->gravity = -3.0f;
        if (actor->current.pos.y < f3) {
            actor->gravity = 0.0f;
            i_this->mBobAngle += 0x800;
            f32* pPosY = &actor->current.pos.y;
            cLib_addCalc2(pPosY, f3 - (45.0f + 15.0f * cM_ssin(i_this->mBobAngle)), 1.0f, 30.0f);
            hit = 1;
        }
    } else if (i_this->mType == 1 || i_this->mType == 4 || i_this->mType == 5) {
        actor->gravity = 0.0f;
    }

    if (hit) {
        if (hit == 1) {
            if (i_this->mMadeWaterSplash == 0) {
                if (i_this->mType != 5) {
                    cXyz scale(1.0f, 1.0f, 1.0f);
                    i_this->mRippleCb.end();
                    dComIfGp_particle_setShipTail(dPa_name::ID_AK_JN_HAMON00, &i_this->mRipplePos, NULL, &scale, 0xFF, &i_this->mRippleCb);
                    i_this->mRippleCb.setRate(0.0f);
                }
                i_this->mMadeWaterSplash = 1;
                shibuki_set(i_this, i_this->current.pos, i_this->mSplashScale);
            }
        }
        return TRUE;
    }
    return FALSE;
}

/* 00001930-0000198C       .text search_y_check__FP8oq_classs */
void search_y_check(oq_class* i_this, short maxStep) {
    s16 step = maxStep;
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 1, step);
    cLib_addCalcAngleS2(&i_this->shape_angle.y, i_this->current.angle.y, 1, step);
}

/* 0000198C-00001B24       .text moguru_check__FP8oq_class */
BOOL moguru_check(oq_class* i_this) {
    fopAc_ac_c* actor = i_this;
    f32 range;

    switch (i_this->mType) {
    case 0:
        if (i_this->m2C2 == 0) {
            range = 2000.0f;
        } else {
            range = 5000.0f;
        }
        if (i_this->mAcch.ChkWallHit()) {
            fopAcM_seStart(actor, JA_SE_LK_LAST_HIT, 0);
            fopAcM_monsSeStart(actor, JA_SE_CV_OQ_DIE, 0);
            i_this->mAction = 0x14;
            i_this->mMode = 0x1E;
            return TRUE;
        }
        break;
    case 1:
    case 4:
        range = 5000.0f;
        break;
    case 5:
        range = 65.0f + REG12_F(13);
        range *= 100.0f;
        break;
    }
    if (i_this->m2C8 != 0 || fopAcM_searchActorDistance(actor, dComIfGp_getPlayer(0)) > range) {
        i_this->mMode = 0xA;
        return TRUE;
    }
    return FALSE;
}

/* 00001B24-00002270       .text action_dousa__FP8oq_class */
void action_dousa(oq_class*) {
    /* Nonmatching */
}

/* 00002270-00002BEC       .text action_kougeki__FP8oq_class */
void action_kougeki(oq_class*) {
    /* Nonmatching */
}

/* 00002BEC-000032A4       .text action_tama_shoot__FP8oq_class */
void action_tama_shoot(oq_class*) {
    /* Nonmatching */
}

/* 000032A4-0000369C       .text action_itai__FP8oq_class */
void action_itai(oq_class*) {
    /* Nonmatching */
}

/* 0000369C-00003BC4       .text action_wakidasi__FP8oq_class */
void action_wakidasi(oq_class*) {
    /* Nonmatching */
}

/* 00003BC4-00004274       .text daOQ_Execute__FP8oq_class */
static BOOL daOQ_Execute(oq_class*) {
    /* Nonmatching */
}

/* 00004274-0000427C       .text daOQ_IsDelete__FP8oq_class */
static BOOL daOQ_IsDelete(oq_class*) {
    return TRUE;
}

/* 0000427C-00004300       .text daOQ_Delete__FP8oq_class */
static BOOL daOQ_Delete(oq_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "OQ");
    i_this->mRippleCb.end();
    i_this->mFollowCb.remove();
    if (i_this->mType == 0 || i_this->mType == 1 || i_this->mType == 4 || i_this->mType == 5) {
        enemy_fire_remove(&i_this->mEnemyFire);
    }
    return TRUE;
}

/* 00004300-000046C0       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c* i_ac) {
    oq_class* i_this = (oq_class*)i_ac;

    if (i_this->mType != 6) {
        J3DModelData* modelData;
        if (i_this->mType == 0) {
            modelData = (J3DModelData*)dComIfG_getObjectRes("OQ", dRes_INDEX_OQ_BMD_OQ_e);
        } else {
            modelData = (J3DModelData*)dComIfG_getObjectRes("OQ", dRes_INDEX_OQ_BMD_RED_OQ_e);
        }
        i_this->mpMorf = new mDoExt_McaMorf(
            modelData, NULL, NULL,
            (J3DAnmTransform*)dComIfG_getObjectRes("OQ", dRes_INDEX_OQ_BCK_START_e),
            J3DFrameCtrl::EMode_RESET, 1.0f, 0, -1, 1,
            NULL, 0, 0x11020203
        );
        if (i_this->mpMorf == NULL || i_this->mpMorf->getModel() == NULL) {
            return FALSE;
        }
        J3DModel* model = i_this->mpMorf->getModel();
        model->setUserArea((u32)i_this);
        for (u16 i = 0; i < (model = i_this->mpMorf->getModel())->getModelData()->getJointNum(); i++) {
            (model = i_this->mpMorf->getModel())->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
        if (i_this->mType == 0 || i_this->mType == 1 || i_this->mType == 4 || i_this->mType == 5) {
            i_this->mpBrk = new mDoExt_brkAnm();
            if (i_this->mpBrk == NULL) {
                return FALSE;
            }
            if (i_this->mpBrk->init(
                    model->getModelData(),
                    (J3DAnmTevRegKey*)dComIfG_getObjectRes("OQ", dRes_INDEX_OQ_BRK_RED_OQ_e),
                    TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, FALSE
                ) == 0)
            {
                return FALSE;
            }
        }
    } else {
        i_this->mpMorf = new mDoExt_McaMorf(
            (J3DModelData*)dComIfG_getObjectRes("OQ", dRes_INDEX_OQ_BDL_ISI_e),
            NULL, NULL, NULL,
            J3DFrameCtrl::EMode_RESET, 0.0f, 0, -1, 1,
            NULL, 0, 0x11020203
        );
        if (i_this->mpMorf == NULL || i_this->mpMorf->getModel() == NULL) {
            return FALSE;
        }
    }

    if (i_this->mType == 0 || i_this->mType == 1 || i_this->mType == 4 || i_this->mType == 5) {
        if (i_this->mInvisibleModel.create(i_this->mpMorf->getModel()) == 0) {
            return FALSE;
        }
    }
    if (i_this->mType == 0 || i_this->mType == 1 || i_this->mType == 4 || i_this->mType == 5) {
        static Vec head0_offset[] = {
            {-25.0f, 0.0f, 0.0f},
        };
        static Vec head1_offset_tbl[] = {
            {20.0f, 0.0f, 0.0f},
            {60.0f, 0.0f, 0.0f},
        };
        static Vec head2_offset[] = {
            {5.0f, 0.0f, 0.0f},
        };
        static __jnt_hit_data_c search_data[] = {
            {
                /* mShapeType  */ JntHitType_SPH_e,
                /* mJointIndex */ OQ_JNT_FACE1_e,
                /* mRadius     */ 50.0f,
                /* mpOffsets   */ head0_offset,
            },
            {
                /* mShapeType  */ JntHitType_CYL_e,
                /* mJointIndex */ OQ_JNT_FACE2_e,
                /* mRadius     */ 32.0f,
                /* mpOffsets   */ head1_offset_tbl,
            },
            {
                /* mShapeType  */ JntHitType_SPH_e,
                /* mJointIndex */ OQ_JNT_HEAD2_e,
                /* mRadius     */ 43.0f,
                /* mpOffsets   */ head2_offset,
            },
        };
        i_this->mpJntHit = JntHit_create(i_this->mpMorf->getModel(), search_data, 3);
        if (i_this->mpJntHit) {
            fopAcM_SetJntHit(i_ac, i_this->mpJntHit);
        } else {
            return FALSE;
        }
    }
    return TRUE;
}

/* 00004708-00004DBC       .text daOQ_Create__FP10fopAc_ac_c */
static cPhs_State daOQ_Create(fopAc_ac_c* i_actor) {
    /* Nonmatching */
    static dCcD_SrcCyl body_co_cyl_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsPlayer_e | cCcD_CoSPrm_VsGrpAll_e,
            /* SrcGObjAt Se      */ dCcG_SE_METAL,
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
        // cM3dGCylS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 15.0f,
            /* Height */ 0.0f,
        }},
    };
    static dCcD_SrcCyl body_at_cyl_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_STALFOS_MACE,
            /* SrcObjAt  Atp     */ 2,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e,
            /* SrcObjTg  Type    */ 0,
            /* SrcObjTg  SPrm    */ 0,
            /* SrcObjCo  SPrm    */ 0,
            /* SrcGObjAt Se      */ dCcG_SE_METAL,
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
        // cM3dGCylS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 15.0f,
            /* Height */ 0.0f,
        }},
    };
    static dCcD_SrcSph tama_at_co_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_STALFOS_MACE,
            /* SrcObjAt  Atp     */ 1,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e,
            /* SrcObjTg  Type    */ 0,
            /* SrcObjTg  SPrm    */ 0,
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
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e | dCcG_TgSPrm_NoHitMark_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 15.0f,
        }},
    };
    static dCcD_SrcSph tama_tg_co_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_STALFOS_MACE,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e | cCcD_AtSPrm_NoTgHitInfSet_e,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e | cCcD_TgSPrm_IsPlayer_e,
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
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e | dCcG_TgSPrm_NoHitMark_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 15.0f,
        }},
    };
    static u8 fire_j[] = {
        OQ_JNT_FACE1_e,
        OQ_JNT_FACE2_e,
        OQ_JNT_HEAD_e,
        OQ_JNT_HEAD2_e,
        OQ_JNT_KUTI_e,
        OQ_JNT_KUTI2_e,
        OQ_JNT_KATAL_e,
        OQ_JNT_HANDL_e,
        OQ_JNT_KATAR_e,
        OQ_JNT_HANDR_e,
    };
    static f32 fire_sc[] = {
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    };

    oq_class* i_this = (oq_class*)i_actor;
    u32 heapSize = 0x3C80;
    i_this->mType = fopAcM_GetParam(i_this);
    i_this->m2C2 = fopAcM_GetParam(i_this) >> 8;
    i_this->mSearchRange = (f32)((fopAcM_GetParam(i_this) >> 16) & 0xFF);
    fopAcM_ct(i_this, oq_class);

    if (i_this->mType == 0xFF) {
        i_this->mType = 0;
    }
    if (i_this->mSearchRange == 255.0f || i_this->mSearchRange == 0.0f) {
        i_this->mSearchRange = 80000.0f;
    } else {
        i_this->mSearchRange *= 1000.0f;
    }

    if (i_this->mType == 3) {
        i_this->group = 0;
        i_this->mAction = 0x64;
        i_this->mMode = 0x32;
        return cPhs_COMPLEATE_e;
    }

    cPhs_State phase_state = dComIfG_resLoad(&i_this->mPhase, "OQ");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (i_this->mType == 0) {
            heapSize = 0x2E00;
        }
        if (i_this->mType == 6) {
            heapSize = 0x9E0;
        }
        if (i_this->m2C2 == 0xFF || i_this->m2C2 > 1) {
            i_this->m2C2 = 0;
        }
        if (i_this->mType != 3 && i_this->mType != 2) {
            if (!fopAcM_entrySolidHeap(i_this, useHeapInit, heapSize)) {
                return cPhs_ERROR_e;
            }
        }
        if (i_this->mType == 2) {
            fopAc_ac_c* player = dComIfGp_getPlayer(0);
            hazure_shoot_timer = 0x64;
            cXyz offset;
            offset.x = 0.0f;
            offset.y = 0.0f;
            offset.z = 1000.0f;
            mDoMtx_YrotS(*calc_mtx, 0);
            cXyz pos;
            MtxPosition(&offset, &pos);
            pos += player->current.pos;
            pos.y -= 40.0f;
            pos.x += cM_rndFX(200.0f);
            pos.z += cM_rndFX(200.0f);
            csXyz angle = i_this->current.angle;
            angle.y = cM_atan2s(pos.x - player->current.pos.x, pos.z - player->current.pos.z);
            fopAcM_create(fpcNm_OQ_e, 0x104, &pos, fopAcM_GetRoomNo(i_this), &angle, &i_this->scale, 0, NULL);
            return cPhs_ERROR_e;
        }

        i_this->max_health = 1;
        i_this->health = 1;
        fopAcM_SetMtx(i_this, i_this->mpMorf->getModel()->getBaseTRMtx());
        i_this->attention_info.flags = 0;
        i_this->mHomePos = i_this->current.pos;
        i_this->mAcch.Set(
            fopAcM_GetPosition_p(i_this),
            fopAcM_GetOldPosition_p(i_this),
            i_this,
            1,
            &i_this->mAcchCir,
            fopAcM_GetSpeed_p(i_this)
        );
        i_this->mStts.Init(0xC8, 1, i_this);
        i_this->mCorrectionOffsetY = 50.0f;

        if (i_this->mType != 6) {
            if (i_this->mType == 0 || i_this->mType == 1 || i_this->mType == 4 || i_this->mType == 5) {
                i_this->gbaName = 0x1B;
                i_this->mEnemyIce.mpActor = i_this;
                i_this->mEnemyIce.mWallRadius = 170.0f;
                i_this->mEnemyIce.mCylHeight = 80.0f;
                i_this->mEnemyFire.mpMcaMorf = i_this->mpMorf;
                i_this->mEnemyFire.mpActor = i_this;
                for (int i = 0; i < (int)ARRAY_SIZE(i_this->mEnemyFire.mFlameJntIdxs); i++) {
                    i_this->mEnemyFire.mFlameJntIdxs[i] = fire_j[i];
                    i_this->mEnemyFire.mParticleScale[i] = fire_sc[i];
                }
            }
            if (i_this->mType == 4) {
                i_this->mStts.SetWeight(0xFE);
                i_this->mTimer = hazure_shoot_timer;
                i_this->mTimer += (s16)cM_rndF(50.0f);
                hazure_shoot_timer += 0x64;
            }
            if (i_this->mType == 1 || i_this->mType == 4 || i_this->mType == 5) {
                i_this->m30C = 175.0f + REG8_F(12);
                i_this->m2C2 = 1;
            }
            if (i_this->mType == 5) {
                i_this->mRelatedId = i_this->parentActorID;
                if (i_this->mRelatedId == fpcM_ERROR_PROCESS_ID_e) {
                    return cPhs_ERROR_e;
                }
                i_this->attention_info.distances[2] = 4;
            }
            if (REG8_S(9) != 0) {
                i_this->m2C2 ^= 1;
            }
            i_this->gravity = -3.0f;
            fopAcM_setCullSizeBox(i_this, -250.0f, 0.0f, -80.0f, 250.0f, 200.0f, 80.0f);
            i_this->mBodyCoCyl.Set(body_co_cyl_src);
            i_this->mBodyCoCyl.SetStts(&i_this->mStts);
            i_this->mBodyAtCyl.Set(body_at_cyl_src);
            i_this->mBodyAtCyl.SetStts(&i_this->mStts);
            i_this->mBodyCoCyl.OffTgSetBit();
            i_this->mBodyCoCyl.OffCoSetBit();
            i_this->mBodyCoCyl.ClrAtHit();
            i_this->mBodyAtCyl.OffAtSetBit();
            i_this->mBodyAtCyl.ClrAtSet();
            if (i_this->mType == 0) {
                i_this->mBodyCoCyl.OffTgSPrmBit(cCcD_TgSPrm_IsOther_e);
                i_this->mBodyCoCyl.OnTgSPrmBit(cCcD_TgSPrm_IsEnemy_e);
                i_this->stealItemLeft = 1;
                i_this->mAcch.ClrRoofNone();
                i_this->itemTableIdx = dComIfGp_CharTbl()->GetNameIndex("Oq", 0);
                i_this->attention_info.distances[2] = 3;
            } else {
                i_this->gbaName = 0x41;
                i_this->itemTableIdx = dComIfGp_CharTbl()->GetNameIndex("Oqw", 0);
                i_this->attention_info.distances[2] = 0x2A;
            }
            i_this->scale.setall(0.0f);
            i_this->mAction = 0xA;
            i_this->mMode = 0;
        } else if (i_this->m2C2 == 0) {
            i_this->mTamaAtSph.Set(tama_at_co_sph_src);
            i_this->mTamaAtSph.SetStts(&i_this->mStts);
            i_this->mTamaTgSph.Set(tama_tg_co_sph_src);
            i_this->mTamaTgSph.SetStts(&i_this->mStts);
            fopAcM_setCullSizeBox(i_this, -50.0f, -50.0f, -50.0f, 50.0f, 50.0f, 50.0f);
            i_this->speedF = 40.0f;
            i_this->mAction = 0x1E;
            i_this->mMode = 0x28;
        }

        i_this->mTargetAngleY = i_this->current.angle.y;
        if (i_this->mType != 3 && i_this->mType != 2) {
            draw_SUB(i_this);
        }
    }
    return phase_state;
}

static actor_method_class l_daOQ_Method = {
    (process_method_func)daOQ_Create,
    (process_method_func)daOQ_Delete,
    (process_method_func)daOQ_Execute,
    (process_method_func)daOQ_IsDelete,
    (process_method_func)daOQ_Draw,
};

actor_process_profile_definition g_profile_OQ = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_OQ_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(oq_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_OQ_e,
    /* Actor SubMtd */ &l_daOQ_Method,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
