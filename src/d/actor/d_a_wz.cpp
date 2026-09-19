/**
 * d_a_wz.cpp
 * Enemy - Wizzrobe / Mini-Boss - Wizzrobe (Wind Temple)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_wz.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_sea.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_cc_uty.h"
#include "d/d_com_inf_game.h"
#include "d/d_material.h"
#include "d/d_particle_name.h"
#include "d/d_path.h"
#include "d/d_s_play.h"
#include "d/d_snap.h"
#include "f_op/f_op_actor_mng.h"
#include "f_pc/f_pc_manager.h"
#include "m_Do/m_Do_audio.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "JSystem/J3DGraphAnimator/J3DNode.h"
#include "SSystem/SComponent/c_lib.h"
#include "SSystem/SComponent/c_math.h"
#include "c/c_damagereaction.h"
#include "res/Object/Wz.h"
#include "res/Object/Wzb.h"

enum daWZ_Joint {
    WZ_JNT_ROD = 0x11,
    WZ_JNT_MINI = 0x13,
};

enum daWZ_RodJoint {
    WZ_ROD_JNT_TIP = 0x2,
};

static u8 come_flag;

/* 00000078-00000140       .text nodeCallBack__FP7J3DNodei */
static BOOL nodeCallBack(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        wz_class* i_this = (wz_class*)model->getUserArea();
        if (i_this != NULL) {
            if (jntNo == WZ_JNT_ROD) {
                MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
                cXyz offset;
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->mStaffPos);
                MTXCopy(*calc_mtx, model->getAnmMtx(jntNo));
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }
    return TRUE;
}

/* 00000140-00000208       .text rod_nodeCallBack__FP7J3DNodei */
static BOOL rod_nodeCallBack(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        wz_class* i_this = (wz_class*)model->getUserArea();
        if (i_this != NULL) {
            if (jntNo == WZ_ROD_JNT_TIP) {
                MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
                cXyz offset;
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->mRodTipPos);
                MTXCopy(*calc_mtx, model->getAnmMtx(jntNo));
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }
    return TRUE;
}

/* 00000208-00000360       .text draw_SUB__FP8wz_class */
void draw_SUB(wz_class* i_this) {
    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseScale(i_this->scale);
    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpMorf->calc();

    if (i_this->mBehaviorType < WZ_TYPE_DAMAGE_BALL_FIRE) {
        if (i_this->mIsMiniBoss) {
            MtxP miniSrc = i_this->mpMorf->getModel()->getAnmMtx(WZ_JNT_MINI);
            MTXCopy(miniSrc, i_this->mpMiniMorf->getModel()->getBaseTRMtx());
            i_this->mpMiniMorf->calc();
        }

        i_this->mpRodMorf->getModel()->setBaseScale(i_this->mRodScale);
        MtxP rodSrc = i_this->mpMorf->getModel()->getAnmMtx(WZ_JNT_ROD);
        MTXCopy(rodSrc, i_this->mpRodMorf->getModel()->getBaseTRMtx());
        i_this->mpRodMorf->calc();
        enemy_fire(&i_this->mEnemyFire);
    }

    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &i_this->current.pos, &i_this->tevStr);
}

/* 00000360-000006D0       .text hontai_draw__FP8wz_class */
void hontai_draw(wz_class* i_this) {
    J3DModel* model = i_this->mpMorf->getModel();
    J3DModelData* modelData = model->getModelData();
    J3DModel* rodModel = i_this->mpRodMorf->getModel();

    if (i_this->mBehaviorType < WZ_TYPE_DAMAGE_BALL_FIRE) {
        if (i_this->mEnableSpawnSwitch != 0xFF) {
            if (!dComIfGs_isSwitch(i_this->mEnableSpawnSwitch, dComIfGp_roomControl_getStayNo())) {
                return;
            }
        }
        if (!i_this->mIsMiniBoss) {
            if (i_this->mAlpha >= 0x80) {
                dSnap_RegistFig(DSNAP_TYPE_UNKC4, i_this, 1.0f, 1.0f, 1.0f);
            }
        }
    }

    g_env_light.setLightTevColorType(model, &i_this->tevStr);
    g_env_light.setLightTevColorType(rodModel, &i_this->tevStr);

    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        modelData->getMaterialNodePointer(i)->getTevKColor(3)->mColor.a = i_this->mAlpha;
    }

    if (i_this->mIsMiniBoss) {
        J3DModel* miniModel = i_this->mpMiniMorf->getModel();
        J3DModelData* miniModelData = miniModel->getModelData();
        g_env_light.setLightTevColorType(miniModel, &i_this->tevStr);
        for (u16 i = 0; i < miniModelData->getMaterialNum(); i++) {
            miniModelData->getMaterialNodePointer(i)->getTevKColor(3)->mColor.a = i_this->mAlpha;
        }
        if (i_this->mAlpha >= 0x80) {
            dSnap_RegistFig(DSNAP_TYPE_UNKC5, i_this, 1.0f, 1.0f, 1.0f);
        }
    }

    if (i_this->mEnemyIce.mFreezeTimer > 20) {
        dMat_control_c::iceEntryDL(i_this->mpMorf, -1, NULL);
        if (i_this->mIsMiniBoss) {
            dMat_control_c::iceEntryDL(i_this->mpMiniMorf, -1, NULL);
        }
        return;
    }

    i_this->mpBrk->entry(model->getModelData());
    f32 frame = i_this->mBehaviorType + i_this->mIsMiniBoss;
    if (i_this->m351) {
        frame = 3.0f;
    }
    i_this->mpBrk->setFrame(frame);
    i_this->mpMorf->entryDL();
    if (i_this->mIsMiniBoss) {
        i_this->mpMiniMorf->entryDL();
        i_this->mMiniInvisibleModel.entry();
    }
    i_this->mpBrk->remove(model->getModelData());
    i_this->mInvisibleModel.entry();

    if (i_this->mAlpha != 0) {
        i_this->mpRodMorf->entryDL();
        cXyz shadowPos(i_this->current.pos.x, i_this->current.pos.y + REG8_F(18), i_this->current.pos.z);
        i_this->mShadowId = dComIfGd_setShadow(
            i_this->mShadowId,
            1,
            i_this->mpMorf->getModel(),
            &shadowPos,
            700.0f + REG8_F(19),
            40.0f,
            i_this->current.pos.y,
            i_this->mAcch.GetGroundH(),
            i_this->mAcch.m_gnd,
            &i_this->tevStr,
            0,
            1.0f,
            dDlst_shadowControl_c::getSimpleTex()
        );
        if (i_this->mShadowId != 0) {
            dComIfGd_addRealShadow(i_this->mShadowId, i_this->mpRodMorf->getModel());
            if (i_this->mIsMiniBoss) {
                dComIfGd_addRealShadow(i_this->mShadowId, i_this->mpMiniMorf->getModel());
            }
        }
    }
}

/* 000006D0-0000074C       .text summon_door_draw__FP8wz_class */
void summon_door_draw(wz_class* i_this) {
    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.setLightTevColorType(model, &i_this->tevStr);
    i_this->mpBtk->entry(model->getModelData());
    i_this->mpMorf->updateDL();
    i_this->mpBtk->remove(model->getModelData());
}

/* 0000074C-00000814       .text damage_ball_draw__FP8wz_class */
static void damage_ball_draw(wz_class* i_this) {
    if (i_this->mBallAlpha != 0) {
        MtxTrans(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z, false);
        MtxScale(0.6f, 0.6f, 0.6f, true);
        cMtx_YrotM(*calc_mtx, i_this->shape_angle.y);
        cMtx_copy(*calc_mtx, i_this->mBallMtx);
        GXColor color;
        color.r = 0xEB;
        color.g = 0x50;
        color.b = 0;
        color.a = 0;
        dComIfGd_setAlphaModelColor(color);
        dComIfGd_setAlphaModel(dDlst_alphaModel_c::TYPE_TWO_SPHERES, i_this->mBallMtx, i_this->mBallAlpha);
    }
}

/* 00000814-000008C0       .text daWZ_Draw__FP8wz_class */
static BOOL daWZ_Draw(wz_class* i_this) {
    if (i_this->mBehaviorType == WZ_TYPE_DAMAGE_BALL_FIRE || i_this->mBehaviorType == WZ_TYPE_DAMAGE_BALL_ICE) {
        if (i_this->scale.x != 0.0f) {
            dComIfGd_setSimpleShadow2(
                &i_this->current.pos,
                i_this->mAcch.GetGroundH(),
                30.0f,
                i_this->mAcch.m_gnd,
                0,
                1.0f,
                dDlst_shadowControl_c::getSimpleTex()
            );
        }
        if (i_this->mBallAlpha != 0) {
            damage_ball_draw(i_this);
        }
        return TRUE;
    }

    if (i_this->mBehaviorType < WZ_TYPE_DAMAGE_BALL_FIRE) {
        hontai_draw(i_this);
    } else {
        summon_door_draw(i_this);
    }
    return TRUE;
}

/* 000008C0-000009EC       .text anm_init__FP8wz_classifUcfi */
void anm_init(wz_class* i_this, int bckFileIdx, float morf, unsigned char loopMode, float speed, int soundFileIdx) {
    i_this->mBckIdx = bckFileIdx;
    if (soundFileIdx >= 0) {
        void* soundAnm = dComIfG_getObjectRes("WZ", soundFileIdx);
        J3DAnmTransform* bckAnm = (J3DAnmTransform*)dComIfG_getObjectRes("WZ", bckFileIdx);
        i_this->mpMorf->setAnm(bckAnm, loopMode, morf, speed, 0.0f, -1.0f, soundAnm);
    } else {
        J3DAnmTransform* bckAnm = (J3DAnmTransform*)dComIfG_getObjectRes("WZ", bckFileIdx);
        i_this->mpMorf->setAnm(bckAnm, loopMode, morf, speed, 0.0f, -1.0f, NULL);
    }
}

/* 000009EC-00000A58       .text rod_size_set__FP8wz_classUc */
void rod_size_set(wz_class* i_this, unsigned char param) {
    if (param == 0) {
        cLib_addCalc2(&i_this->mRodScale.x, 1.0f, 0.2f, 0.5f);
    } else {
        cLib_addCalc0(&i_this->mRodScale.x, 0.2f, 0.5f);
    }
    f32 rodX = i_this->mRodScale.x;
    i_this->mRodScale.z = rodX;
    i_this->mRodScale.y = rodX;
}

/* 00000A58-00001258       .text body_atari_check__FP8wz_class */
BOOL body_atari_check(wz_class* i_this) {
    fopAc_ac_c* actor = i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    i_this->mStts.Move();
    i_this->m34E = 0;
    if (i_this->mBodyCyl.ChkTgHit()) {
        cCcD_Obj* hitObj = i_this->mBodyCyl.GetTgHitObj();
        if (hitObj == NULL) {
            return FALSE;
        }
        if (i_this->m34F != 0) {
            return FALSE;
        }

        i_this->m34F = 1;
        CcAtInfo atInfo;
        atInfo.pParticlePos = NULL;
        u8 skipAtCheck = 0;
        switch (hitObj->GetAtType()) {
        case AT_TYPE_GRAPPLING_HOOK:
            if (i_this->stealItemLeft > 0) {
                s8 oldHealth = i_this->health;
                i_this->health = 10;
                atInfo.mpObj = i_this->mBodyCyl.GetTgHitObj();
                cc_at_check(i_this, &atInfo);
                i_this->m353++;
                i_this->health = oldHealth;
            }
            dComIfGp_particle_set(dPa_name::ID_IT_JN_PIYOHIT00, &i_this->attention_info.position);
            skipAtCheck = 1;
            i_this->m34E = 9;
            fopAcM_seStart(actor, JA_SE_LK_W_WEP_HIT, 0x44);
            fopAcM_monsSeStart(actor, JA_SE_CV_WZ_NO_DAMAGE, 0);
            break;
        case AT_TYPE_SWORD: {
            fopAcM_seStart(actor, JA_SE_LK_SW_HIT_S, 0x20);
            i_this->m34E = 0;
            u8 cutType = player->getCutType();
            if (cutType == daPy_py_c::CUT_TYPE_CUT_EA) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_CUT_EB) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_CUT_TURN) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_CUT_ROLL) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_JUMPCUT_SWORD) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_JUMPCUT_STICK) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_JUMPCUT_MACHETE) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_BT_JUMPCUT) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_BT_ROLLCUT) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_BT_VERTICALJUMPCUT) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_JUMPCUT_CLUB) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_JUMPCUT_DN_SWORD) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_JUMPCUT_SPEAR) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_CUT_EXA) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_CUT_EXB) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_CUT_EXMJ) {
                goto sword_smash;
            }
            if (cutType == daPy_py_c::CUT_TYPE_CUT_KESA) {
                goto sword_smash;
            }
            break;
        sword_smash:
            i_this->m34E = 1;
            break;
        }
        case AT_TYPE_WIND:
            skipAtCheck = 1;
            i_this->m34E = 3;
            fopAcM_seStart(actor, JA_SE_LK_W_WEP_HIT, 0x44);
            fopAcM_monsSeStart(actor, JA_SE_CV_WZ_NO_DAMAGE, 0);
            break;
        case AT_TYPE_BOOMERANG:
            skipAtCheck = 1;
            i_this->m34E = 4;
            dComIfGp_particle_set(dPa_name::ID_IT_JN_PIYOHIT00, &i_this->attention_info.position);
            fopAcM_seStart(actor, JA_SE_LK_W_WEP_HIT, 0x44);
            fopAcM_monsSeStart(actor, JA_SE_CV_WZ_NO_DAMAGE, 0);
            break;
        case AT_TYPE_BOKO_STICK:
            fopAcM_seStart(actor, JA_SE_LK_W_WEP_HIT, 0x20);
            break;
        case AT_TYPE_SKULL_HAMMER:
            fopAcM_seStart(actor, JA_SE_LK_HAMMER_HIT, 0x20);
            i_this->m34E = 7;
            if (player->getCutType() == daPy_py_c::CUT_TYPE_HAMMER_SIDESWING) {
                i_this->m34E = 8;
            }
            break;
        case AT_TYPE_BOMB:
            i_this->m34E = 6;
            break;
        case AT_TYPE_FIRE:
        case AT_TYPE_FIRE_ARROW:
            i_this->mEnemyFire.mFireDuration = 100;
            break;
        case AT_TYPE_ICE_ARROW:
            i_this->mParticleCb2.remove();
            i_this->mParticleCb3.remove();
            i_this->mParticleCb4.remove();
            i_this->mRodScale.setall(0.0f);
            i_this->mEnemyIce.m00C = 2;
            i_this->mEnemyIce.mFreezeDuration = 200;
            i_this->attention_info.flags = 0;
            skipAtCheck = 1;
            break;
        case AT_TYPE_LIGHT_ARROW:
            i_this->mEnemyIce.mLightShrinkTimer = 1;
            i_this->mEnemyIce.mParticleScale = 1.0f;
            i_this->mEnemyIce.mYOffset = 80.0f;
            i_this->attention_info.flags = 0;
            i_this->mBodyCyl.OffTgSetBit();
            i_this->mBodyCyl.ClrTgHit();
            rod_size_set(i_this, 1);
            i_this->mParticleCb2.remove();
            i_this->mParticleCb3.remove();
            i_this->mParticleCb4.remove();
            break;
        case AT_TYPE_NORMAL_ARROW:
            i_this->m34E = 5;
        default:
            i_this->m34E = 0;
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x20);
            break;
        }

        if (skipAtCheck == 0) {
            cXyz hitPos = *i_this->mBodyCyl.GetTgHitPosP();
            cXyz scale;
            atInfo.mpObj = i_this->mBodyCyl.GetTgHitObj();
            cc_at_check(i_this, &atInfo);
            if (i_this->m34E == 1) {
                goto critical_hit;
            }
            if (i_this->m34E == 7) {
                goto critical_hit;
            }
            if (i_this->m34E == 8) {
                goto critical_hit;
            }
            if (i_this->health > 0) {
                goto ok_hit;
            }
        critical_hit:
            dComIfGp_particle_set(dPa_name::ID_AK_JN_CRITICALHITFLASH, &hitPos);
            scale.setall(2.0f);
            dComIfGp_particle_set(dPa_name::ID_AK_JN_CRITICALHIT, &hitPos, &player->shape_angle, &scale);
            goto after_hit;
        ok_hit:
            dComIfGp_particle_set(dPa_name::ID_AK_JN_OK, &hitPos, &player->shape_angle);
        after_hit:;
        }

        i_this->mAction = 1;
        i_this->mMode = 0xA;
        return TRUE;
    }

    i_this->m34F = 0;
    return FALSE;
}

/* 00001258-000012F0       .text BG_check__FP8wz_class */
void BG_check(wz_class* i_this) {
    i_this->mAcchCir.SetWall(i_this->mWallHeight, i_this->mWallRadius);
    i_this->mCorrectionOffsetY = 20.0f;
    i_this->current.pos.y -= i_this->mCorrectionOffsetY;
    i_this->old.pos.y -= i_this->mCorrectionOffsetY;
    i_this->mAcch.CrrPos(*dComIfG_Bgsp());
    i_this->current.pos.y += i_this->mCorrectionOffsetY;
    i_this->old.pos.y += i_this->mCorrectionOffsetY;
}

/* 00001728-00001780       .text fuwafuwa_calc__FP8wz_class */
void fuwafuwa_calc(wz_class* i_this) {
    i_this->mFuwafuwaAngle += 0x7D0;
    i_this->current.pos.y = 15.0f + i_this->mHomePos.y + 5.0f * cM_ssin(i_this->mFuwafuwaAngle);
}

/* 00001780-00001B38       .text weapon_shoot__FP8wz_classUc */
void weapon_shoot(wz_class* i_this, unsigned char type) {
    fopAc_ac_c* actor = i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    csXyz angle = i_this->shape_angle;
    angle.x = i_this->shape_angle.x;
    angle.y = i_this->shape_angle.y;
    angle.z = i_this->shape_angle.z;

    fopAcM_monsSeStart(actor, JA_SE_CV_WZ_ATTACK, 0);

    switch (type) {
    case 0: {
        f32 dx = player->current.pos.x - i_this->mRodTipPos.x;
        f32 dy = 50.0f + player->current.pos.y - i_this->mRodTipPos.y;
        f32 dz = player->current.pos.z - i_this->mRodTipPos.z;
        angle.x = -cM_atan2s(dy, std::sqrtf(dx * dx + dz * dz));

        s16 yawOff = -0xBB8;
        for (int i = 0; i < 3; i++) {
            angle.y = yawOff + cM_atan2s(dx, dz);
            fopAcM_create(
                fpcNm_WZ_e,
                0xFFFFFF0A,
                &i_this->mRodTipPos,
                fopAcM_GetRoomNo(actor),
                &angle,
                NULL,
                -1,
                NULL
            );
            yawOff += 0xBB8;
        }
        fopAcM_seStart(actor, JA_SE_CM_WZ_ST_FIREBALL, 0);
        break;
    }
    case 1:
        if (i_this->m439 != 0xFF) {
            if (i_this->mpPath != NULL) {
                cXyz scale = i_this->scale;
                f32 dx = i_this->mPathTarget.x - i_this->mRodTipPos.x;
                f32 dy = 300.0f + i_this->mPathTarget.y - i_this->mRodTipPos.y;
                f32 dz = i_this->mPathTarget.z - i_this->mRodTipPos.z;
                angle.x = -cM_atan2s(dy, std::sqrtf(dx * dx + dz * dz));
                angle.y = cM_atan2s(dx, dz);
                scale.setall(1.0f);
                fopAcM_seStart(actor, JA_SE_CM_WZ_ST_SUMMONBALL, 0);
                fpc_ProcID id = fopAcM_createChild(
                    fpcNm_WZ_e,
                    fopAcM_GetID(actor),
                    0xFFFFFF0B,
                    &i_this->mRodTipPos,
                    fopAcM_GetRoomNo(actor),
                    &angle,
                    &scale,
                    0
                );
                if (id != fpcM_ERROR_PROCESS_ID_e) {
                    i_this->mHasChildActor = 1;
                }
            }
        }
        break;
    }
}

/* 00001B38-00002528       .text action_dousa__FP8wz_class */
void action_dousa(wz_class*) {
    /* Nonmatching */
}

/* 00002528-00002E3C       .text action_itai__FP8wz_class */
void action_itai(wz_class*) {
    /* Nonmatching */
    volatile const char* unused_s[] = {
        "kazeMB",
        "sea",
        "WZB",
        "__dummy",
        "wiz_r",
        "wiz_s",
    };
    (void)unused_s;
}

/* 00002E3C-000047C8       .text action_demo__FP8wz_class */
void action_demo(wz_class*) {
    /* Nonmatching */
}

/* 000047C8-00004810       .text next_tama_move__FP8wz_class4cXyz */
void next_tama_move(wz_class* i_this, cXyz pos) {
    i_this->mTamaTarget = pos;
    i_this->speedF = 0.0f;
    i_this->mBallSph.OffAtSetBit();
    i_this->mBallSph.OffAtSetBit();
    i_this->mMode = 0x68;
}

/* 00004810-00004BF4       .text summon_call_sub__FP8wz_class */
void summon_call_sub(wz_class* i_this) {
    static s16 enemy_name_dt[] = {
        fpcNm_KI_e, 0x7FFF, fpcNm_MT_e, 0x7FFF, fpcNm_MT_e, fpcNm_KI_e, fpcNm_KI_e, 0x7FFF,
        fpcNm_KI_e, 0x7FFF, fpcNm_BL_e, 0x7FFF, fpcNm_KI_e, 0x7FFF, fpcNm_BL_e, fpcNm_KI_e,
        fpcNm_BL_e, 0x7FFF, fpcNm_BL_e, 0x7FFF, fpcNm_BL_e, 0x7FFF, fpcNm_BL_e, 0x7FFF,
        fpcNm_KS_e, 0x7FFF, fpcNm_KS_e, 0x7FFF, fpcNm_KS_e, 0x7FFF, fpcNm_KS_e, 0x7FFF,
        fpcNm_CC_e, 0x7FFF, fpcNm_CC_e, 0x7FFF, fpcNm_CC_e, 0x7FFF, fpcNm_CC_e, 0x7FFF,
        fpcNm_BL_e, 0x7FFF, fpcNm_BL_e, 0x7FFF, fpcNm_BL_e, fpcNm_BL_e, fpcNm_PW_e, fpcNm_BL_e,
        fpcNm_ST_e, 0x7FFF, fpcNm_RD_e, 0x7FFF, fpcNm_ST_e, 0x7FFF, fpcNm_RD_e, 0x7FFF,
        fpcNm_MO2_e, 0x7FFF, fpcNm_TN_e, 0x7FFF, fpcNm_MO2_e, 0x7FFF, fpcNm_TN_e, 0x7FFF,
        fpcNm_PH_e, 0x7FFF, fpcNm_KI_e, 0x7FFF, fpcNm_PH_e, 0x7FFF, fpcNm_BB_e, 0x7FFF,
        fpcNm_KI_e, 0x7FFF, fpcNm_MT_e, 0x7FFF, fpcNm_MT_e, fpcNm_KI_e, fpcNm_KI_e, 0x7FFF,
        fpcNm_KI_e, 0x7FFF, fpcNm_BL_e, 0x7FFF, fpcNm_KI_e, 0x7FFF, fpcNm_BL_e, fpcNm_KI_e,
        fpcNm_KS_e, 0x7FFF, fpcNm_KS_e, 0x7FFF, fpcNm_KS_e, 0x7FFF, fpcNm_KS_e, 0x7FFF,
        fpcNm_CC_e, 0x7FFF, fpcNm_CC_e, 0x7FFF, fpcNm_CC_e, 0x7FFF, fpcNm_CC_e, 0x7FFF,
        fpcNm_BK_e, 0x7FFF, fpcNm_BK_e, fpcNm_MO2_e, fpcNm_BK_e, fpcNm_BK_e, fpcNm_BK_e, fpcNm_MO2_e,
        fpcNm_BB_e, 0x7FFF, fpcNm_KI_e, 0x7FFF, fpcNm_CC_e, fpcNm_CC_e, fpcNm_KI_e, fpcNm_KS_e,
    };
    static u32 enemy_arg_dt[] = {
        0xFFFFFF03, 0x00000000, 0xFFFFFF00, 0x00000000, 0xFFFFFF00, 0xFFFFFF03, 0xFFFFFF03, 0x00000000,
        0xFFFFFF03, 0x00000000, 0x0100FF00, 0x00000000, 0xFFFFFF03, 0x00000000, 0x0100FF00, 0xFFFFFF03,
        0x0100FF01, 0x00000000, 0x0100FF01, 0x00000000, 0x0100FF01, 0x00000000, 0x0100FF01, 0x00000000,
        0x00000003, 0x00000000, 0x00000003, 0x00000000, 0x00000003, 0x00000000, 0x00000003, 0x00000000,
        0xFFFF0000, 0x00000000, 0xFFFF0000, 0x00000000, 0xFFFF0000, 0x00000000, 0xFFFF0000, 0x00000000,
        0x0100FF00, 0x00000000, 0x0100FF00, 0x00000000, 0x0100FF01, 0x0100FF00, 0xFF000000, 0x0100FF01,
        0xFFFFFF00, 0x00000000, 0xFFFFFFFE, 0x00000000, 0xFFFFFF00, 0x00000000, 0xFFFFFFFE, 0x00000000,
        0xFFFFFF00, 0x00000000, 0xFFFFFF20, 0x00000000, 0xFFFFFF00, 0x00000000, 0xFFFFFF20, 0x00000000,
        0xFFF0F000, 0x00000000, 0xFFFF0003, 0x00000000, 0xFFF0F000, 0x00000000, 0xFFFFFF03, 0x00000000,
        0xFFFFFF03, 0x00000000, 0xFFFFFF00, 0x00000000, 0xFFFFFF00, 0xFFFFFF03, 0xFFFFFF03, 0x00000000,
        0xFFFFFF03, 0x00000000, 0x0100FF00, 0x00000000, 0xFFFFFF03, 0x00000000, 0x0100FF00, 0xFFFFFF03,
        0x00000003, 0x00000000, 0x00000003, 0x00000000, 0x00000003, 0x00000000, 0x00000003, 0x00000000,
        0xFFFF0000, 0x00000000, 0xFFFF0000, 0x00000000, 0xFFFF0000, 0x00000000, 0xFFFF0000, 0x00000000,
        0xFFFFFF00, 0x00000000, 0xFFFFFF00, 0xFFFFFF00, 0xFFFFFF00, 0xFFFFFF20, 0xFFFFFF00, 0xFFFFFF00,
        0xFFFFFF03, 0x00000000, 0xFFFFFF03, 0x00000000, 0xFFFF0A00, 0xFFFF0B00, 0xFFFF0003, 0x00000003,
    };
    static u32 birth_dt[] = {
        8, 0, 2, 0, 1, 3, 6, 0,
        8, 0, 2, 0, 6, 0, 1, 3,
        2, 0, 1, 0, 1, 0, 1, 0,
        20, 0, 15, 0, 15, 0, 20, 0,
        8, 0, 10, 0, 8, 0, 8, 0,
        2, 0, 2, 0, 1, 1, 1, 2,
        1, 0, 1, 0, 1, 0, 1, 0,
        1, 0, 1, 0, 1, 0, 1, 0,
        2, 0, 6, 0, 3, 0, 2, 0,
        4, 0, 1, 0, 1, 2, 3, 0,
        4, 0, 1, 0, 3, 0, 1, 2,
        10, 0, 8, 0, 8, 0, 10, 0,
        4, 0, 5, 0, 4, 0, 4, 0,
        1, 0, 1, 1, 1, 1, 1, 1,
        2, 0, 4, 0, 2, 2, 4, 8,
    };
    static u32 cc_birth_dt[] = {0, 1, 4, 3};

    fopAc_ac_c* actor = i_this;
    cXyz pos = actor->current.pos;
    wz_class* parent = (wz_class*)fopAcM_SearchByID(i_this->mRelatedId);
    if (parent == NULL) {
        return;
    }
    if (i_this->mTimer != 1) {
        return;
    }

    int tableIndex = (parent->mSummonTableType << 3) + (parent->mSummonWave << 1);
    int birthNum = birth_dt[tableIndex];
    csXyz angle;
    angle.x = 0;
    angle.y = 0;
    angle.z = 0;
    angle.y = fopAcM_searchActorAngleY(actor, dComIfGp_getPlayer(0));
    pos = actor->current.pos;
    if (tableIndex == 0xC0) {
        angle.x = 0x80;
    }

    int spawned = 0;
    int i;
    for (i = spawned; i < 20 && spawned < birthNum; i++) {
        if (i_this->mChildIds[i] == fpcM_ERROR_PROCESS_ID_e) {
            u32 arg = enemy_arg_dt[tableIndex];
            if (enemy_name_dt[tableIndex] == fpcNm_CC_e) {
                if ((arg & 0xA00) == 0) {
                    int rnd = (int)cM_rndF(2.99f);
                    arg |= (cc_birth_dt[rnd] + 0xA) << 8;
                }
            }
            i_this->mChildIds[i] = fopAcM_create(
                enemy_name_dt[tableIndex],
                arg,
                &pos,
                fopAcM_GetRoomNo(actor),
                &angle,
                NULL,
                -1,
                NULL
            );
            if (i_this->mChildIds[i] != fpcM_ERROR_PROCESS_ID_e) {
                i_this->mChildAlive[i] = 1;
                pos = actor->current.pos;
                spawned++;
                pos.x += cM_rndFX(100.0f);
                pos.y += cM_rndFX(100.0f);
                pos.z += cM_rndFX(100.0f);
            }
        }
    }

    parent->mSummonWave++;
    parent->mSummonWave &= 3;
    if (parent->mSummonTableType == 7 || parent->mSummonTableType == 13) {
        parent->mSummonCount++;
        if (parent->mSummonCount > 7) {
            parent->mSummonTableType = 10;
        }
    }

    int nextIndex = tableIndex + 1;
    if (enemy_name_dt[nextIndex] == 0x7FFF) {
        return;
    }
    int nextBirth = birth_dt[nextIndex];
    angle.x = 0;
    angle.y = 0;
    angle.z = 0;
    angle.y = fopAcM_searchActorAngleY(actor, dComIfGp_getPlayer(0));
    pos = actor->current.pos;

    int spawned2 = 0;
    int i2 = 0;
    for (; i2 < 20 && spawned2 < nextBirth; i2++) {
        if (i_this->mChildIds[i2] == fpcM_ERROR_PROCESS_ID_e) {
            u32 arg = enemy_arg_dt[nextIndex];
            if (tableIndex + 1 == fpcNm_CC_e) {
                arg |= 0xA00;
                int rnd = (int)cM_rndF(3.19f);
                arg |= cc_birth_dt[rnd] << 8;
            }
            i_this->mChildIds[i2] = fopAcM_create(
                enemy_name_dt[nextIndex],
                arg,
                &pos,
                fopAcM_GetRoomNo(actor),
                &angle,
                NULL,
                -1,
                NULL
            );
            if (i_this->mChildIds[i2] != fpcM_ERROR_PROCESS_ID_e) {
                i_this->mChildAlive[i2] = 1;
                pos = actor->current.pos;
                spawned2++;
                pos.x += cM_rndFX(100.0f);
                pos.y += cM_rndFX(100.0f);
                pos.z += cM_rndFX(100.0f);
            }
        }
    }
}

/* 00004BF4-00004D18       .text sea_water_check__FP8wz_class */
BOOL sea_water_check(wz_class* i_this) {
    u8 hit = 0;
    i_this->mEffectPos = i_this->current.pos;

    if (daSea_ChkArea(i_this->current.pos.x, i_this->current.pos.z)) {
        f32 waveY = daSea_calcWave(i_this->current.pos.x, i_this->current.pos.z);
        if (i_this->current.pos.y < 50.0f + waveY) {
            hit = 1;
        }
    } else if (i_this->mAcch.MaskWaterIn()) {
        f32 waterY = i_this->mAcch.m_wtr.GetHeight();
        if (i_this->current.pos.y < 50.0f + waterY) {
            hit = 1;
        }
    }

    if (hit) {
        cXyz scale(2.0f, 2.0f, 2.0f);
        dComIfGp_particle_set(dPa_name::ID_AK_JN_EVAPORATION00, &i_this->current.pos, &i_this->current.angle, &scale);
        return TRUE;
    }
    return FALSE;
}

/* 00004D18-00005760       .text action_tama_dousa__FP8wz_class */
void action_tama_dousa(wz_class*) {
    /* Nonmatching */
}

/* 00005760-00005B64       .text action_summon_dousa__FP8wz_class */
void action_summon_dousa(wz_class* i_this) {
    fopAc_ac_c* actor = i_this;
    u8 deadCount = 0;

    switch (i_this->mMode) {
    case 0xC8: {
        for (int i = 0; i < 4; i++) {
            i_this->m3DE[i] = 0;
        }
        for (int i = 0; i < 5; i++) {
            JPABaseEmitter* emitter = dComIfGp_particle_set(dPa_name::ID_AK_SN_WIZSUMMONWIND00, &actor->current.pos);
            f32 sx = actor->scale.x;
            f32 sy = actor->scale.y;
            f32 sz = actor->scale.z;
            if (emitter != NULL) {
                emitter->setGlobalDynamicsScale(sx, sy, sz);
                emitter->setGlobalParticleScale(JGeometry::TVec3<f32>(sx, sy, sz));
            }
        }
        fopAcM_seStart(actor, JA_SE_OBJ_WZ_SUMMON_B_SMN, 0);
        {
            JPABaseEmitter* emitter = dComIfGp_particle_set(dPa_name::ID_AK_SN_WIZSUMMONSMOKE00, &actor->current.pos);
            f32 sx = actor->scale.x;
            f32 sy = actor->scale.y;
            f32 sz = actor->scale.z;
            if (emitter != NULL) {
                emitter->setGlobalDynamicsScale(sx, sy, sz);
                emitter->setGlobalParticleScale(JGeometry::TVec3<f32>(sx, sy, sz));
            }
        }
        i_this->mTimer = 0x28;
        for (int i = 0; i < 20; i++) {
            i_this->mChildIds[i] = fpcM_ERROR_PROCESS_ID_e;
            i_this->mChildAlive[i] = 0;
        }
        i_this->mMode++;
        break;
    }
    case 0xC9:
        if (i_this->mBehaviorType == WZ_TYPE_SUMMON_DOOR) {
            summon_call_sub(i_this);
        }
        {
            BOOL stopped = i_this->mpBtk->isStop();
            if (stopped) {
                i_this->mMode++;
                if (i_this->mBehaviorType == WZ_TYPE_SUMMON_DOOR_2) {
                    fopAcM_delete(actor);
                }
            }
        }
        break;
    case 0xCA:
        for (int i = 0; i < 20; i++) {
            if (i_this->mChildAlive[i] != 0) {
                fopAc_ac_c* child = fopAcM_SearchByID(i_this->mChildIds[i]);
                s16 outOfBounds = 0;
                if (child != NULL) {
                    if (strcmp(dComIfGp_getStartStageName(), "kazeMB") == 0) {
                        f32 x = child->current.pos.x;
                        f32 z = child->current.pos.z;
                        f32 distXZ = std::sqrtf(x * x + z * z);
                        if (distXZ > 1800.0f || child->current.pos.y < -200.0f || child->current.pos.y > 2100.0f) {
                            outOfBounds = 1;
                        }
                    }
                }
                if (child == NULL || outOfBounds != 0) {
                    i_this->mChildAlive[i] = 0;
                    i_this->mChildIds[i] = fpcM_ERROR_PROCESS_ID_e;
                }
            } else {
                deadCount++;
            }
        }
        if (deadCount == 20) {
            wz_class* parent = (wz_class*)fopAcM_SearchByID(i_this->mRelatedId);
            if (parent != NULL) {
                parent->mHasChildActor = 0;
            }
            fopAcM_delete(actor);
        }
        break;
    }

    i_this->mpMorf->play(NULL, 0, 0);
    i_this->mpBtk->play();
}

/* 00005B64-00006108       .text daWZ_Execute__FP8wz_class */
static BOOL daWZ_Execute(wz_class* i_this) {
    if (i_this->mBehaviorType < WZ_TYPE_DAMAGE_BALL_FIRE) {
        if (i_this->mEnableSpawnSwitch != 0xFF) {
            if (!dComIfGs_isSwitch(i_this->mEnableSpawnSwitch, dComIfGp_roomControl_getStayNo())) {
                return TRUE;
            }
        }
    }

    if (REG12_S(9) == 0) {
        if (i_this->m352 != 0) {
            fopAc_ac_c* related = fopAcM_SearchByID(i_this->mRelatedId);
            if (related == NULL) {
                i_this->mHasChildActor = 0;
                i_this->mRelatedId = fpcM_ERROR_PROCESS_ID_e;
                i_this->m352 = 0;
            }
        }
    }

    i_this->mPLight.mPos = i_this->current.pos;
    switch (i_this->mBehaviorType) {
    case WZ_TYPE_DAMAGE_BALL_FIRE:
        i_this->mPLight.mColor.r = 300.0f + REG0_F(3);
        i_this->mPLight.mColor.g = 50.0f + REG0_F(4);
        i_this->mPLight.mColor.b = REG0_F(5);
        i_this->mPLight.mPower = 550.0f + REG0_F(9);
        i_this->mPLight.mFluctuation = 200.0f + REG0_F(10);
        break;
    case WZ_TYPE_DAMAGE_BALL_ICE:
        i_this->mPLight.mColor.r = 300.0f + REG0_F(6);
        i_this->mPLight.mColor.g = 300.0f + REG0_F(7);
        i_this->mPLight.mColor.b = 20.0f + REG0_F(8);
        i_this->mPLight.mPower = 550.0f + REG0_F(9);
        i_this->mPLight.mFluctuation = 200.0f + REG0_F(10);
        break;
    }

    if (i_this->mBehaviorType < WZ_TYPE_DAMAGE_BALL_FIRE) {
        fopAcM_setGbaName(i_this, 0x3C, 0x11, 0x2F);
        if (enemy_ice(&i_this->mEnemyIce)) {
            i_this->mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
            i_this->mpMorf->calc();
            if (i_this->mIsMiniBoss) {
                MtxP miniSrc = i_this->mpMorf->getModel()->getAnmMtx(WZ_JNT_MINI);
                MTXCopy(miniSrc, i_this->mpMiniMorf->getModel()->getBaseTRMtx());
                i_this->mpMiniMorf->calc();
            }
            enemy_fire_remove(&i_this->mEnemyFire);
            rod_size_set(i_this, 1);
            BG_check(i_this);
            return TRUE;
        }
    }

    for (int i = 0; i < 5; i++) {
        if ((&i_this->mTimer)[i] != 0) {
            (&i_this->mTimer)[i]--;
        }
    }

    switch (i_this->mAction) {
    case 0:
        action_dousa(i_this);
        break;
    case 0x64:
        action_tama_dousa(i_this);
        break;
    case 1:
        action_itai(i_this);
        break;
    case 0xC8:
        action_summon_dousa(i_this);
        break;
    case 3:
        action_demo(i_this);
        break;
    }

    if (i_this->mBehaviorType < WZ_TYPE_DAMAGE_BALL_FIRE) {
        if (i_this->mAction != 1 && i_this->mAction != 3) {
            cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 1, 0x500);
            cLib_addCalcAngleS2(&i_this->shape_angle.y, i_this->current.angle.y, 1, 0x500);
        }
        i_this->mpMorf->play(NULL, 0, 0);
    }

    cMtx_YrotS(*calc_mtx, i_this->current.angle.y);
    cMtx_XrotM(*calc_mtx, i_this->current.angle.x);
    cXyz offset;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = i_this->speedF;
    cXyz moved;
    MtxPosition(&offset, &moved);
    i_this->speed.x = moved.x;
    if (i_this->mBehaviorType >= WZ_TYPE_DAMAGE_BALL_FIRE) {
        i_this->speed.y = moved.y;
    }
    i_this->speed.z = moved.z;
    i_this->speed.y += i_this->gravity;
    if (i_this->speed.y < -100.0f) {
        i_this->speed.y = -100.0f;
    }

    fopAcM_posMove(i_this, NULL);
    i_this->eyePos = i_this->current.pos;

    if (i_this->mBehaviorType < WZ_TYPE_DAMAGE_BALL_FIRE) {
        BG_check(i_this);
        if (i_this->mAction != 1 && i_this->mAction != 3 && i_this->mMode != 7) {
            fuwafuwa_calc(i_this);
        }
        i_this->attention_info.position = i_this->current.pos;
        i_this->attention_info.position.y += 190.0f;
        i_this->eyePos.y += 120.0f + REG12_F(18);
        i_this->mBodyCyl.SetC(i_this->current.pos);
        i_this->mBodyCyl.SetH(170.0f);
        i_this->mBodyCyl.SetR(50.0f);
        dComIfG_Ccsp()->Set(&i_this->mBodyCyl);
    } else if (i_this->mBehaviorType != WZ_TYPE_SUMMON_DOOR && i_this->mBehaviorType != WZ_TYPE_SUMMON_DOOR_2) {
        BG_check(i_this);
        i_this->mBallSph.SetC(i_this->current.pos);
        i_this->mBallSph.SetR(i_this->mBallRadius);
        dComIfG_Ccsp()->Set(&i_this->mBallSph);
        dComIfG_Ccsp()->SetMass(&i_this->mBallSph, 1);
    }

    draw_SUB(i_this);
    i_this->mEffectPos = i_this->current.pos;
    i_this->mEffectAngle = i_this->shape_angle;
    return TRUE;
}

/* 00006108-00006110       .text daWZ_IsDelete__FP8wz_class */
static BOOL daWZ_IsDelete(wz_class*) {
    return TRUE;
}

/* 00006110-0000627C       .text daWZ_Delete__FP8wz_class */
static BOOL daWZ_Delete(wz_class* i_this) {
    if (i_this->health == -128) {
        if (i_this->mIsMiniBoss) {
            mDoAud_subBgmStop();
        }
        if (strcmp(dComIfGp_getStartStageName(), "kazeMB") != 0) {
            if (i_this->mDisableSpawnOnDeathSwitch != 0xFF) {
                dComIfGs_onSwitch(i_this->mDisableSpawnOnDeathSwitch, fopAcM_GetRoomNo(i_this));
            }
        }
    }

    if (i_this->mBehaviorType != WZ_TYPE_SUMMON_DOOR && i_this->mBehaviorType != WZ_TYPE_SUMMON_DOOR_2) {
        dComIfG_resDelete(&i_this->mPhase, "WZ");
    } else {
        dComIfG_resDelete(&i_this->mPhase, "WZB");
    }

    if (i_this->mBehaviorType == WZ_TYPE_DAMAGE_BALL_FIRE || i_this->mBehaviorType == WZ_TYPE_DAMAGE_BALL_ICE) {
        dKy_plight_cut(&i_this->mPLight);
    }

    i_this->mParticleCb0.remove();
    i_this->mParticleCb1.remove();
    i_this->mParticleCb2.remove();
    i_this->mParticleCb3.remove();
    i_this->mParticleCb4.remove();

    if (i_this->mBehaviorType < WZ_TYPE_DAMAGE_BALL_FIRE) {
        enemy_fire_remove(&i_this->mEnemyFire);
    }
    return TRUE;
}

/* 0000627C-00006684       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c* i_this) {
    wz_class* a_this = (wz_class*)i_this;

    a_this->mpMorf = new mDoExt_McaMorf(
        (J3DModelData*)dComIfG_getObjectRes("WZ", dRes_INDEX_WZ_BDL_WZ_e),
        NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes("WZ", dRes_INDEX_WZ_BCK_TAME1_e),
        J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, 1,
        NULL,
        0x80000,
        0x37441422
    );
    J3DModel* model;
    if (a_this->mpMorf == NULL || (model = a_this->mpMorf->getModel()) == NULL) {
        return FALSE;
    }

    model->setUserArea((u32)a_this);
    for (u16 i = 0; i < (model = a_this->mpMorf->getModel())->getModelData()->getJointNum(); i++) {
        (model = a_this->mpMorf->getModel())->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    if (a_this->mBehaviorType > WZ_TYPE_DAMAGE_BALL_FIRE) {
        return TRUE;
    }

    if (a_this->mIsMiniBoss) {
        a_this->mpMiniMorf = new mDoExt_McaMorf(
            (J3DModelData*)dComIfG_getObjectRes("WZ", dRes_INDEX_WZ_BDL_WZ_KAMEN_e),
            NULL, NULL,
            NULL,
            J3DFrameCtrl::EMode_NONE, 0.0f, 0, -1, 1,
            NULL,
            0x80000,
            0x37441422
        );
        J3DModel* miniModel;
        if (a_this->mpMiniMorf == NULL || (miniModel = a_this->mpMiniMorf->getModel()) == NULL) {
            return FALSE;
        }
        if (!a_this->mMiniInvisibleModel.create(miniModel)) {
            return FALSE;
        }
    }

    a_this->mpRodMorf = new mDoExt_McaMorf(
        (J3DModelData*)dComIfG_getObjectRes("WZ", dRes_INDEX_WZ_BDL_WZ_STICK_e),
        NULL, NULL,
        NULL,
        J3DFrameCtrl::EMode_LOOP, 0.0f, 0, -1, 1,
        NULL,
        0,
        0x11020203
    );
    J3DModel* rodModel;
    if (a_this->mpRodMorf == NULL || (rodModel = a_this->mpRodMorf->getModel()) == NULL) {
        return FALSE;
    }

    rodModel->setUserArea((u32)a_this);
    for (u16 i = 0; i < (rodModel = a_this->mpRodMorf->getModel())->getModelData()->getJointNum(); i++) {
        (rodModel = a_this->mpRodMorf->getModel())->getModelData()->getJointNodePointer(i)->setCallBack(rod_nodeCallBack);
    }

    J3DModel* brkModel = a_this->mpMorf->getModel();
    a_this->mpBrk = new mDoExt_brkAnm();
    if (a_this->mpBrk == NULL) {
        return FALSE;
    }
    if (a_this->mpBrk->init(
            brkModel->getModelData(),
            (J3DAnmTevRegKey*)dComIfG_getObjectRes("WZ", dRes_INDEX_WZ_BRK_WZ_e),
            TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, FALSE
        ) == 0)
    {
        return FALSE;
    }

    return a_this->mInvisibleModel.create(a_this->mpMorf->getModel()) != 0 ? TRUE : FALSE;
}

/* 000066CC-00006888       .text useHeapInit2__FP10fopAc_ac_c */
static BOOL useHeapInit2(fopAc_ac_c* i_this) {
    wz_class* a_this = (wz_class*)i_this;

    a_this->mpMorf = new mDoExt_McaMorf(
        (J3DModelData*)dComIfG_getObjectRes("WZB", dRes_INDEX_WZB_BDL_YSUMN00_e),
        NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes("WZB", dRes_INDEX_WZB_BCK_YSUMN00_e),
        J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, 1,
        NULL,
        0x80000,
        0x19000222
    );
    J3DModel* model;
    if (a_this->mpMorf == NULL || (model = a_this->mpMorf->getModel()) == NULL) {
        return FALSE;
    }

    a_this->mpBtk = new mDoExt_btkAnm();
    if (a_this->mpBtk == NULL) {
        return FALSE;
    }
    return a_this->mpBtk->init(
        model->getModelData(),
        (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("WZB", dRes_INDEX_WZB_BTK_YSUMN00_e),
        TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, FALSE
    ) != 0 ? TRUE : FALSE;
}

/* 00006888-000071C4       .text daWZ_Create__FP10fopAc_ac_c */
static cPhs_State daWZ_Create(fopAc_ac_c* i_actor) {
    static dCcD_SrcCyl body_co_cyl_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
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
    static dCcD_SrcSph ball_co_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_FIRE,
            /* SrcObjAt  Atp     */ 2,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
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
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 15.0f,
        }},
    };
    static u8 fire_j[] = {
        0x01, 0x02, 0x06, 0x07, 0x08, 0x0C, 0x0D, 0x11, 0x13, 0x15,
    };
    static f32 fire_sc[] = {
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    };
    wz_class* i_this = (wz_class*)i_actor;
    fopAcM_ct(i_this, wz_class);

    i_this->mBehaviorType = fopAcM_GetParam(i_this);
    i_this->mDisableSpawnOnDeathSwitch = fopAcM_GetParam(i_this) >> 8;
    i_this->mEnableSpawnSwitch = fopAcM_GetParam(i_this) >> 16;
    i_this->mPathIndex = fopAcM_GetParam(i_this) >> 24;
    i_this->mEnemySummonTableIndex = i_this->current.angle.z;
    if (i_this->mBehaviorType == 0xFF) {
        i_this->mBehaviorType = 0;
    }
    if (i_this->mBehaviorType == 2) {
        i_this->mBehaviorType = 1;
        i_this->mIsMiniBoss = true;
    }
    if (i_this->mBehaviorType == 3) {
        i_this->mBehaviorType = 0;
        i_this->m351 = true;
    }
    if (i_this->mEnemySummonTableIndex == 0xFF) {
        i_this->mEnemySummonTableIndex = 0;
    }

    cPhs_State phase_state;
    if (i_this->mBehaviorType != WZ_TYPE_SUMMON_DOOR && i_this->mBehaviorType != WZ_TYPE_SUMMON_DOOR_2) {
        phase_state = dComIfG_resLoad(&i_this->mPhase, "WZ");
    } else {
        phase_state = dComIfG_resLoad(&i_this->mPhase, "WZB");
    }

    if (i_this->mBehaviorType > WZ_TYPE_DAMAGE_BALL_FIRE) {
        i_this->mBallSph.SetAtType(AT_TYPE_MACHETE);
        i_this->mRelatedId = i_this->parentActorID;
        if (i_this->mRelatedId == fpcM_ERROR_PROCESS_ID_e) {
            return cPhs_ERROR_e;
        }
        wz_class* parent = (wz_class*)fopAcM_SearchByID(i_this->mRelatedId);
        if (parent == NULL) {
            return cPhs_ERROR_e;
        }
        if (i_this->mBehaviorType == WZ_TYPE_SUMMON_DOOR) {
            i_this->mRelatedId = parent->mRelatedId;
            if (fopAcM_SearchByID(i_this->mRelatedId) == NULL) {
                return cPhs_ERROR_e;
            }
        }
        i_this->mPathTarget = parent->mPathTarget;
        i_this->mPathPointIdx = parent->mPathPointIdx;
        i_this->mHasChildActor = parent->mHasChildActor;
        i_this->mSummonWave = parent->mSummonWave;
        i_this->mSummonTableType = parent->mSummonTableType;
    }

    if (phase_state != cPhs_COMPLEATE_e) {
        if (phase_state == cPhs_ERROR_e) {
            if (i_this->mBehaviorType == WZ_TYPE_SUMMON_DOOR) {
                wz_class* parent = (wz_class*)fopAcM_SearchByID(i_this->mRelatedId);
                parent->mHasChildActor = 0;
            }
        }
        return phase_state;
    }

    if (i_this->mBehaviorType < WZ_TYPE_DAMAGE_BALL_FIRE) {
            if (i_this->mDisableSpawnOnDeathSwitch != 0xFF) {
                if (dComIfGs_isSwitch(i_this->mDisableSpawnOnDeathSwitch, dComIfGp_roomControl_getStayNo())) {
                    return cPhs_ERROR_e;
                }
            }
        }

        if (i_this->mBehaviorType != WZ_TYPE_SUMMON_DOOR && i_this->mBehaviorType != WZ_TYPE_SUMMON_DOOR_2) {
            u32 heapSize = 0x3440;
            if (i_this->mBehaviorType >= WZ_TYPE_DAMAGE_BALL_FIRE) {
                heapSize = 0x3640;
            }
            if (!fopAcM_entrySolidHeap(i_this, useHeapInit, heapSize)) {
                return cPhs_ERROR_e;
            }
        } else {
            if (!fopAcM_entrySolidHeap(i_this, useHeapInit2, 0xCC0)) {
                wz_class* parent = (wz_class*)fopAcM_SearchByID(i_this->mRelatedId);
                parent->mHasChildActor = 0;
                return cPhs_ERROR_e;
            }
            if (i_this->mBehaviorType == WZ_TYPE_SUMMON_DOOR_2) {
                fopAcM_OnStatus(i_this, fopAcStts_UNK4000_e);
            }
            J3DModelData* modelData = i_this->mpMorf->getModel()->getModelData();
            J3DTexture* texture = modelData->getTexture();
            if (texture == NULL) {
                wz_class* parent = (wz_class*)fopAcM_SearchByID(i_this->mRelatedId);
                parent->mHasChildActor = 0;
                return cPhs_ERROR_e;
            }
            JUTNameTab* textureName = modelData->getTextureName();
            if (textureName == NULL) {
                wz_class* parent = (wz_class*)fopAcM_SearchByID(i_this->mRelatedId);
                parent->mHasChildActor = 0;
                return cPhs_ERROR_e;
            }
            for (u16 i = 0; i < texture->getNum(); i++) {
                if (strcmp(textureName->getName(i), "__dummy") == 0) {
                    texture->setResTIMG(i, *mDoGph_gInf_c::getFrameBufferTimg());
                }
            }
            mDoExt_modelTexturePatch(modelData);
        }

        i_this->current.angle.z = 0;
        i_this->shape_angle.z = 0;
        i_this->attention_info.flags = 0;
        fopAcM_SetMtx(i_this, i_this->mpMorf->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(i_this, -100.0f, -50.0f, -50.0f, 100.0f, 200.0f, 100.0f);

        if (i_this->mBehaviorType < WZ_TYPE_DAMAGE_BALL_FIRE) {
            i_this->stealItemLeft = 3;
            i_this->max_health = 4;
            i_this->health = 4;
            i_this->attention_info.distances[2] = 4;
            switch (i_this->mBehaviorType) {
            case 0:
                i_this->itemTableIdx = dComIfGp_CharTbl()->GetNameIndex("wiz_r", 0);
                break;
            case 1:
                i_this->max_health = 8;
                i_this->health = 8;
                i_this->itemTableIdx = dComIfGp_CharTbl()->GetNameIndex("wiz_s", 0);
                i_this->mHasChildActor = 0;
                i_this->mSummonTableType = i_this->mEnemySummonTableIndex;
                if (i_this->mIsMiniBoss == true) {
                    i_this->mSummonTableType = 7;
                    fopAcM_OnStatus(i_this, fopAcStts_BOSS_e);
                    i_this->max_health = 12;
                    i_this->health = 12;
                } else {
                    i_this->mSummonWave = (s16)cM_rndF(3.99f);
                    i_this->mSummonWave &= 3;
                }
                break;
            }
            if (REG8_S(1) != 0) {
                i_this->max_health = 0x7F;
                i_this->health = 0x7F;
            }
            if (i_this->mPathIndex != 0xFF) {
                i_this->mpRoomPath = dPath_GetRoomPath(i_this->mPathIndex, fopAcM_GetRoomNo(i_this));
                if (i_this->mpRoomPath != NULL) {
                    int rnd = (int)cM_rndF(i_this->mpRoomPath->m_num);
                    if (rnd == i_this->mpRoomPath->m_num) {
                        rnd--;
                    }
                    i_this->current.pos = i_this->mpRoomPath->m_points[rnd].m_position;
                    i_this->mpPath = dPath_GetNextRoomPath(i_this->mpRoomPath, fopAcM_GetRoomNo(i_this));
                }
            }
            i_this->mHomePos = i_this->current.pos;
            draw_SUB(i_this);
            i_this->mBodyCyl.Set(body_co_cyl_src);
            i_this->mBodyCyl.SetStts(&i_this->mStts);
            i_this->mBodyCyl.OffCoSetBit();
            i_this->mBodyCyl.ClrCoHit();
            i_this->mTargetAngleY = fopAcM_searchActorAngleY(i_this, dComIfGp_getPlayer(0));
            i_this->current.angle.y = i_this->mTargetAngleY;
            i_this->shape_angle.y = i_this->mTargetAngleY;
            i_this->mWallHeight = 100.0f;
            i_this->mWallRadius = 110.0f;
            i_this->mEnemyIce.mpActor = i_this;
            i_this->mEnemyIce.mCylHeight = 50.0f;
            i_this->mEnemyIce.mWallRadius = 80.0f;
            i_this->mEnemyFire.mpMcaMorf = i_this->mpMorf;
            i_this->mEnemyFire.mpActor = i_this;
            for (int i = 0; i < (int)ARRAY_SIZE(i_this->mEnemyFire.mFlameJntIdxs); i++) {
                i_this->mEnemyFire.mFlameJntIdxs[i] = fire_j[i];
                i_this->mEnemyFire.mParticleScale[i] = fire_sc[i];
            }
            i_this->mAlpha = 0;
            i_this->mRodScale.setall(0.0f);
            if (strcmp(dComIfGp_getStartStageName(), "kazeMB") == 0) {
                i_this->mAction = 3;
                if (i_this->mIsMiniBoss) {
                    i_this->current.pos.setall(0.0f);
                    come_flag = 0;
                    i_this->mMode = 0x32;
                } else {
                    fopAcM_OnStatus(i_this, fopAcStts_UNK4000_e);
                    i_this->mMode = 0x3C;
                }
            } else {
                i_this->mAction = 0;
                i_this->mMode = 0;
            }
        } else if (i_this->mBehaviorType != WZ_TYPE_SUMMON_DOOR && i_this->mBehaviorType != WZ_TYPE_SUMMON_DOOR_2) {
            i_this->mWallHeight = 0.0f;
            i_this->mWallRadius = 40.0f;
            i_this->mBallSph.Set(ball_co_sph_src);
            i_this->mBallSph.SetStts(&i_this->mStts);
            i_this->mTimers[1] = 0x12C;
            i_this->mAction = 0x64;
            i_this->mMode = 0x64;
        } else {
            i_this->mAction = 0xC8;
            i_this->mMode = 0xC8;
        }

        if (i_this->mBehaviorType != WZ_TYPE_SUMMON_DOOR && i_this->mBehaviorType != WZ_TYPE_SUMMON_DOOR_2) {
            i_this->mAcch.Set(
                fopAcM_GetPosition_p(i_this),
                fopAcM_GetOldPosition_p(i_this),
                i_this,
                1,
                &i_this->mAcchCir,
                fopAcM_GetSpeed_p(i_this)
            );
            i_this->mStts.Init(0xFE, 1, i_this);
            BG_check(i_this);
        }

        if (i_this->mBehaviorType == WZ_TYPE_DAMAGE_BALL_FIRE || i_this->mBehaviorType == WZ_TYPE_DAMAGE_BALL_ICE) {
            dKy_plight_set(&i_this->mPLight);
        }

    return phase_state;
}


static actor_method_class l_daWZ_Method = {
    (process_method_func)daWZ_Create,
    (process_method_func)daWZ_Delete,
    (process_method_func)daWZ_Execute,
    (process_method_func)daWZ_IsDelete,
    (process_method_func)daWZ_Draw,
};

actor_process_profile_definition g_profile_WZ = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_WZ_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(wz_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_WZ_e,
    /* Actor SubMtd */ &l_daWZ_Method,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
