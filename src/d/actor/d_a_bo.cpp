/**
 * d_a_bo.cpp
 * Enemy - Boko Baba
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_bo.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_camera.h"
#include "f_op/f_op_camera.h"
#include "d/d_kankyo.h"
#include "d/d_particle.h"
#include "d/d_s_play.h"
#include "d/d_snap.h"
#include "d/d_material.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "m_Do/m_Do_audio.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "JSystem/J3DGraphAnimator/J3DNode.h"
#include "c/c_damagereaction.h"
#include "SSystem/SComponent/c_math.h"
#include "res/Object/Bo.h"

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
static BOOL nodeCallBack_UP(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        s32 changed = 0;
        J3DModel* model = j3dSys.getModel();
        bo_class* i_this = (bo_class*)model->getUserArea();
        if (i_this) {
            daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
            cXyz offset;
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);

            if (jntNo <= 10) {
                f32 scale = jntNo * 0.025f;
                mDoMtx_YrotM(*calc_mtx, i_this->m33A.y * scale);
                mDoMtx_XrotM(*calc_mtx, i_this->m33A.x * scale);
                mDoMtx_ZrotM(*calc_mtx, i_this->m33A.z * scale);
                changed = 1;
            }

            if (jntNo == 1) {
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->m2F8);
                changed = 1;
            }

            if (jntNo == 10) {
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->m328);
                changed = 1;
            }

            if (jntNo == 11) {
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->m2E0);
                changed = 1;
            }

            if (jntNo == 4) {
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->m310);
                changed = 1;
            }

            if (jntNo >= 5 && jntNo <= 7) {
                f32 scale = (s16)(8 - jntNo);
                scale *= 1.75f;
                mDoMtx_YrotM(*calc_mtx, i_this->m352.y * scale);
                mDoMtx_ZrotM(*calc_mtx, i_this->m352.z * scale);
                changed = 1;
            }

            if (jntNo == 8) {
                mDoMtx_YrotM(*calc_mtx, i_this->m334.y + REG12_S(3));
                mDoMtx_XrotM(*calc_mtx, i_this->m334.z + REG12_S(5));
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->m31C);
                changed = 1;
            }

            if (jntNo == 12) {
                if (i_this->m2CC != 0) {
                    cXyz pos;
                    csXyz angle;
                    offset.x = 0.0f;
                    offset.y = 0.0f;
                    offset.z = 0.0f;
                    MtxPosition(&offset, &pos);
                    f32 dx = i_this->m31C.x - pos.x;
                    f32 dz = i_this->m31C.z - pos.z;
                    if (i_this->m2CD != 0) {
                        angle.x = -0x5FB4;
                        angle.y = cM_atan2s(dx, dz) + 0x7FFF;
                        angle.z = 0;
                        player->setPlayerPosAndAngle(&pos, &angle);
                    } else {
                        angle.x = 0x5BCC;
                        angle.y = cM_atan2s(dx, dz);
                        angle.z = 0;
                        player->setPlayerPosAndAngle(&pos, &angle);
                    }
                    changed = 1;
                }
            }

            if (changed) {
                MTXCopy(*calc_mtx, model->getAnmMtx(jntNo));
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return TRUE;
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
static BOOL daBO_Draw(bo_class* i_this) {
    fopAc_ac_c* actor = i_this;

    if (REG13_S(5) != 0) {
        return TRUE;
    }

    u8 iceDrawn = 0;
    if (i_this->m2C0 == 0 || i_this->m2C0 == 2) {
        g_env_light.setLightTevColorType(i_this->mpMorfUP->getModel(), &actor->tevStr);
    }

    if (i_this->m2C0 == 0 || i_this->m2C0 == 1) {
        if (i_this->mEnemyIce.mLightShrinkTimer == 0) {
            g_env_light.setLightTevColorType(i_this->mpMorfDW->getModel(), &actor->tevStr);
        }
    }

    dSnap_RegistFig(DSNAP_TYPE_UNKB2, actor, actor->current.pos, i_this->m348, 1.0f, 1.0f, 1.0f);

    if (i_this->m2C0 == 0) {
        if (i_this->mEnemyIce.mFreezeTimer > 20) {
            dMat_control_c::iceEntryDL(i_this->mpMorfUP, -1, &i_this->mInvisModel);
            iceDrawn = 1;
        }
    }

    if (iceDrawn == 0) {
        if (i_this->m2C0 == 0 || i_this->m2C0 == 2) {
            J3DModel* model = i_this->mpMorfUP->getModel();
            if (i_this->m2C0 == 2) {
                i_this->mpBrkAnm->entry(model->getModelData());
            }
            i_this->mpMorfUP->entryDL();
            if (i_this->m2C0 == 2) {
                i_this->mpBrkAnm->remove(model->getModelData());
            }
        }
    }

    if (i_this->m2C0 == 0 || i_this->m2C0 == 1) {
        if (i_this->mEnemyIce.mLightShrinkTimer == 0) {
            i_this->mpMorfDW->entryDL();
        }
    }

    return TRUE;
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
BOOL shock_damage_check(bo_class* i_this) {
    fopAc_ac_c* actor = i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (player->checkHammerQuake()) {
        cXyz swordTopPos = player->getSwordTopPos();
        swordTopPos.x -= actor->current.pos.x;
        swordTopPos.z -= actor->current.pos.z;
        f32 distXZ = std::sqrtf(SQUARE(swordTopPos.x) + SQUARE(swordTopPos.z));
        if (distXZ < 1000.0f) {
            fopAcM_seStart(actor, JA_SE_LK_HAMMER_HIT, 0x31);
            return TRUE;
        }
    }

    return FALSE;
}

/* 00000E24-000013A4       .text head_atari_check__FP8bo_class */
void head_atari_check(bo_class*) {
    /* Nonmatching */
    /* Dummy literals occupy @4175 0x4C-0x58 so wait_initial/start_bakutsuki stay 100%. */
    volatile f32 f1 = 8.0f;
    volatile f32 f2 = 2.0f;
    volatile f32 f3 = 34.0f;
    volatile f32 f4 = 80.0f;
    (void)f1;
    (void)f2;
    (void)f3;
    (void)f4;
}

/* 000013A4-0000170C       .text nokezori_damage_rtn__FP8bo_class */
void nokezori_damage_rtn(bo_class* i_this) {
    fopAc_ac_c* actor = i_this;
    s16 knockAngle = i_this->m370 - i_this->m348;

    anm_init(i_this, dRes_INDEX_BO_BCK_DAMAGE1_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f, -1, 0);

    mDoMtx_YrotS(*calc_mtx, knockAngle);
    cXyz offset;
    cXyz knock;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 6250.0f;
    MtxPosition(&offset, &knock);
    i_this->m340.z = knock.x;
    i_this->m340.y = knock.z;
    i_this->m334.set(0, 0, 0);
    actor->scale.y = 1.75f;
    i_this->m37C = 0x4B0;

    cCcD_Obj* hitObj = i_this->mHeadSph.GetTgHitObj();
    if (hitObj != NULL) {
        u32 atType = hitObj->GetAtType();
        if (atType & AT_TYPE_SWORD) {
            fopAcM_monsSeStart(actor, JA_SE_CV_BV_NO_DAMAGE, 0);
            fopAcM_seStart(actor, JA_SE_LK_SW_CRT_HIT, 0x20);
        } else if (atType & AT_TYPE_BOKO_STICK) {
            fopAcM_monsSeStart(actor, JA_SE_CV_BV_NO_DAMAGE, 0);
            fopAcM_seStart(actor, JA_SE_LK_W_WEP_CRT_HIT, 0x20);
        } else if (atType & AT_TYPE_SKULL_HAMMER) {
            fopAcM_monsSeStart(actor, JA_SE_CV_BV_NO_DAMAGE, 0);
            fopAcM_seStart(actor, JA_SE_LK_HAMMER_HIT, 0x31);
        } else if (!(atType & AT_TYPE_BOOMERANG)) {
            fopAcM_monsSeStart(actor, JA_SE_CV_BV_NO_DAMAGE, 0);
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_CRT_HIT, 0x20);
        }
    }

    i_this->m398 = 1.0f;
    i_this->m372 = 0;
    i_this->m376 = 0;
    i_this->m36A = 3;
    if (i_this->m2D0 == 0) {
        i_this->m2C4 = 0;
        i_this->m2C5 = 5;
    } else {
        i_this->m2C4 = 4;
        i_this->m2C5 = 0x32;
    }
}

/* 0000170C-00001BB8       .text body_atari_check__FP8bo_class */
void body_atari_check(bo_class*) {
    /* Nonmatching */
}

/* 00001BB8-00002048       .text damage_check__FP8bo_class */
void damage_check(bo_class*) {
    /* Nonmatching */
    /* Dummy literals occupy @4175 0x64-0x68 so wait_initial/start_bakutsuki stay 100%. */
    volatile f32 f1 = 2250.0f;
    volatile f32 f2 = -10000.0f;
    (void)f1;
    (void)f2;
}

/* 00002048-00002080       .text angle_initial__FP8bo_class */
void angle_initial(bo_class* i_this) {
    i_this->m33A.set(0, 0, 0);
    i_this->m352.set(0, 0, 0);
    i_this->m334.set(0, 0, 0);
    i_this->m340.set(0, 0, 0);
}

/* 00002080-00002170       .text wait_initial__FP8bo_class */
void wait_initial(bo_class* i_this) {
    if (cM_rnd() < 0.5f) {
        anm_init(i_this, dRes_INDEX_BO_BCK_WAIT_L1_e, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1, 0);
    } else {
        anm_init(i_this, dRes_INDEX_BO_BCK_WAIT_R1_e, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1, 0);
    }

    i_this->m366 = cM_rndF(60.0f) + 60.0f;
    i_this->mHeadSph.OffAtSetBit();
    i_this->mHeadSph.ClrAtSet();
    i_this->mHeadSph.SetAtSpl(dCcG_At_Spl_UNK0);
    i_this->mHeadSph.OnCoSetBit();
    angle_initial(i_this);
    i_this->m2C4 = 0;
    i_this->m2C5 = 2;
}

/* 00002170-000022F8       .text start_bakutsuki_event_camera__FP10fopAc_ac_c */
void start_bakutsuki_event_camera(fopAc_ac_c* actor) {
    static cXyz cam_pos(40.0f, 10.0f, 280.0f);
    static cXyz ctr_pos(0.0f, 50.0f, 50.0f);
    static f32 cam_fovy = 75.0f;
    static int cam_timer = 30;

    dComIfGp_getCamera(0)->mCamera.StartEventCamera(
        5,
        fopAcM_GetID(actor),
        "RelActor", "@STARTER",
        "RelUseMask", "--or",
        "Timer", &cam_timer,
        "Center", &ctr_pos,
        "Eye", &cam_pos,
        "Fovy", &cam_fovy,
        0
    );
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
static BOOL daBO_Delete(bo_class* i_this) {
    i_this->mSmokeCb.remove();

    if (i_this->m3D0 != NULL) {
        i_this->m3D0->becomeInvalidEmitter();
        i_this->m3D0 = NULL;
    }

    if (i_this->m3D4 != NULL) {
        i_this->m3D4->deleteAllParticle();
        i_this->m3D4->becomeInvalidEmitter();
        i_this->m3D4 = NULL;
    }

    if (i_this->m3D8 != NULL) {
        i_this->m3D8->deleteAllParticle();
        i_this->m3D8->becomeInvalidEmitter();
        i_this->m3D8 = NULL;
    }

    enemy_fire_remove(&i_this->mEnemyFire);
    dComIfG_resDelete(&i_this->mPhase, "BO");
    return TRUE;
}

/* 00004998-00004D08       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c* i_this) {
    bo_class* a_this = (bo_class*)i_this;

    int bckIdx = dRes_INDEX_BO_BCK_DERU_U1_e;
    if (a_this->m2C0 == 2) {
        bckIdx = dRes_INDEX_BO_BCK_DAMAGE1_e;
    }

    a_this->mpMorfUP = new mDoExt_McaMorf(
        (J3DModelData*)dComIfG_getObjectRes("BO", dRes_INDEX_BO_BDL_BO_UE1_e),
        NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes("BO", bckIdx),
        J3DFrameCtrl::EMode_RESET, 0.0f, 0, -1, 1,
        NULL,
        0x80000,
        0x37441422
    );
    if (a_this->mpMorfUP == NULL || a_this->mpMorfUP->getModel() == NULL) {
        return FALSE;
    }

    J3DModel* model;
    a_this->mpMorfUP->getModel()->setUserArea((u32)a_this);
    for (u16 i = 0; i < (model = a_this->mpMorfUP->getModel())->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_UP);
    }

    if (a_this->m2C0 == 2) {
        a_this->mpBrkAnm = new mDoExt_brkAnm();
        if (a_this->mpBrkAnm == NULL) {
            return FALSE;
        }
        if (a_this->mpBrkAnm->init(
                model->getModelData(),
                (J3DAnmTevRegKey*)dComIfG_getObjectRes("BO", dRes_INDEX_BO_BRK_TO_BOKO1_e),
                TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, false
            ) == 0) {
            return FALSE;
        }
    } else {
        a_this->mpMorfDW = new mDoExt_McaMorf(
            (J3DModelData*)dComIfG_getObjectRes("BO", dRes_INDEX_BO_BDL_BO_SITA1_e),
            NULL, NULL,
            (J3DAnmTransform*)dComIfG_getObjectRes("BO", dRes_INDEX_BO_BCK_DERU_S1_e),
            J3DFrameCtrl::EMode_RESET, 0.0f, 0, -1, 1,
            NULL,
            0x80000,
            0x33221202
        );
        if (a_this->mpMorfDW == NULL || a_this->mpMorfDW->getModel() == NULL) {
            return FALSE;
        }

        a_this->mpMorfDW->getModel()->setUserArea((u32)a_this);
        for (u16 i = 0; i < (model = a_this->mpMorfDW->getModel())->getModelData()->getJointNum(); i++) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_DW);
        }
    }

    if (a_this->m2C0 == 0) {
        if (a_this->mInvisModel.create(a_this->mpMorfUP->getModel()) == 0) {
            return FALSE;
        }
    }

    return TRUE;
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
