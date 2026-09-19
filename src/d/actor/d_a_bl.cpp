/**
 * d_a_bl.cpp
 * Enemy - Bubble
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_bl.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_path.h"
#include "d/d_s_play.h"
#include "d/d_snap.h"
#include "d/d_material.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_camera.h"
#include "m_Do/m_Do_mtx.h"
#include "m_Do/m_Do_audio.h"
#include "SSystem/SComponent/c_lib.h"
#include "SSystem/SComponent/c_math.h"
#include "res/Object/Bl.h"

/* 00000078-00000178       .text draw_SUB__FP8bl_class */
void draw_SUB(bl_class* i_this) {
    fopAc_ac_c* actor = i_this;
    J3DModel* model = i_this->mpMorf->getModel();

    model->setBaseScale(actor->scale);
    MtxTrans(actor->current.pos.x, actor->current.pos.y + i_this->mYOffset, actor->current.pos.z, 0);
    mDoMtx_YrotM(*calc_mtx, actor->shape_angle.y);
    mDoMtx_XrotM(*calc_mtx, actor->shape_angle.x);
    mDoMtx_ZrotM(*calc_mtx, actor->shape_angle.z);
    MtxTrans(0.0f, -i_this->mYOffset, 0.0f, 1);
    model->setBaseTRMtx(*calc_mtx);

    i_this->mpMorf->calc();
    enemy_fire(&i_this->mEnemyFire);
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &actor->current.pos, &actor->tevStr);
}

/* 00000178-000002F8       .text daBL_Draw__FP8bl_class */
static BOOL daBL_Draw(bl_class* i_this) {
    fopAc_ac_c* actor = i_this;
    J3DModel* model = i_this->mpMorf->getModel();

    g_env_light.setLightTevColorType(model, &actor->tevStr);

    if (i_this->mSwitch != 0xFF && !dComIfGs_isSwitch(i_this->mSwitch, dComIfGp_roomControl_getStayNo())) {
        return TRUE;
    }

    dSnap_RegistFig(DSNAP_TYPE_UNKB4, actor, 1.0f, 1.0f, 1.0f);

    if (i_this->mEnemyIce.mFreezeTimer > 20) {
        dMat_control_c::iceEntryDL(i_this->mpMorf, -1, &i_this->mInvisModel);
        return TRUE;
    }

    if (i_this->mBtkMode == 0) {
        i_this->mpBtkOff->entry(model->getModelData());
    } else {
        i_this->mpBtkOn->entry(model->getModelData());
    }

    i_this->mpMorf->entryDL();

    if (actor->health > 0) {
        dComIfGd_setSimpleShadow2(
            &actor->current.pos, i_this->mAcch.GetGroundH(), 30.0f, i_this->mAcch.m_gnd,
            0, 1.0f, dDlst_shadowControl_c::getSimpleTex()
        );
    }

    if (i_this->mBtkMode == 0) {
        i_this->mpBtkOff->remove(model->getModelData());
    } else {
        i_this->mpBtkOn->remove(model->getModelData());
    }

    return TRUE;
}

/* 000002F8-000003F8       .text smoke_set__FP8bl_class */
void smoke_set(bl_class* i_this) {
    fopAc_ac_c* actor = i_this;

    if (i_this->mSmokeCb.getEmitter() == NULL) {
        dComIfGp_particle_setToon(
            dPa_name::ID_AK_JT_ELEMENTSMOKE01, (cXyz*)i_this->m6BC, &actor->shape_angle, NULL, 0xB9,
            &i_this->mSmokeCb, actor->current.roomNo
        );
    }

    if (i_this->mSmokeCb.getEmitter() != NULL) {
        GXColor color = {0xA0, 0xA0, 0x80, 0xB4};
        JGeometry::TVec3<f32> scale;
        scale.x = scale.y = scale.z = 1.6f;
        i_this->mSmokeCb.getEmitter()->setGlobalParticleScale(scale);
        i_this->mSmokeCb.getEmitter()->setRate(10.0f);
        i_this->mSmokeCb.getEmitter()->setMaxFrame(1);
        i_this->mSmokeCb.getEmitter()->setAwayFromCenterSpeed(8.0f);
        i_this->mSmokeCb.getEmitter()->becomeImmortalEmitter();
        i_this->mSmokeCb.setColor(color);
    }
}

/* 000003F8-000004AC       .text fire_move_set__FP8bl_class */
void fire_move_set(bl_class* i_this) {
    int particleID = dPa_name::ID_AK_SN_BUBBLEBLUE00 + (i_this->mType == 0);

    if (i_this->mFireCb.getEmitter() == NULL) {
        dComIfGp_particle_set(
            particleID, &i_this->current.pos, NULL, NULL, 0xFF, &i_this->mFireCb, i_this->current.roomNo
        );
        if (i_this->mType == 1) {
            i_this->mSph.SetTgSe(dCcG_SE_METAL);
            i_this->mSph.SetTgHitMark(dCcg_TgHitMark_Purple_e);
            i_this->mSph.OnTgShield();
        }
    }
}

/* 000004AC-00000578       .text fire_emitter_clr__FP8bl_class */
void fire_emitter_clr(bl_class* i_this) {
    JPABaseEmitter* emitter = i_this->mFireCb.getEmitter();
    if (emitter != NULL) {
        if (i_this->mFireClrTimer == 0) {
            if (i_this->mType == 0) {
                JGeometry::TVec3<s16> rot;
                rot.x = 0;
                rot.y = dComIfGp_getPlayer(0)->shape_angle.y + 0x8000;
                rot.z = 0;
                emitter->setGlobalRotation(rot);
                i_this->mFireCb.getEmitter()->setRate(3.0f);
                i_this->mFireCb.getEmitter()->setLifeTime(10);
                i_this->mFireCb.getEmitter()->setDirectionalSpeed(50.0f);
                i_this->mFireClrTimer = 0x15;
            } else {
                i_this->mFireClrTimer = 1;
            }
        }
    }

    i_this->mFollowCb.remove();
}

/* 00000578-000006E0       .text fire_kaiten_keisan__FP8bl_class */
void fire_kaiten_keisan(bl_class* i_this) {
    fopAc_ac_c* actor = i_this;
    JPABaseEmitter* emitter = i_this->mFireCb.getEmitter();
    if (emitter != NULL) {
        if (i_this->mType == 0) {
            emitter->setGlobalRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(0));
            mDoAud_seStart(JA_SE_CM_BL_FLAME_BURNING, &actor->eyePos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
        } else {
            mDoAud_seStart(JA_SE_CM_BL_CURSE_BURNING, &actor->eyePos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
            camera_process_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
            mDoMtx_copy(i_this->mpMorf->getModel()->getAnmMtx(0), *calc_mtx);
            mDoMtx_YrotM(*calc_mtx, camera->mAngle.y - actor->shape_angle.y);
            i_this->mFireCb.getEmitter()->setGlobalRTMatrix(*calc_mtx);
        }
    }
}

/* 000006E0-00000820       .text shock_damage_check__FP8bl_class */
BOOL shock_damage_check(bl_class* i_this) {
    fopAc_ac_c* actor = i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (!(i_this->mType & 0x80) && i_this->mBtkMode != 0) {
        return FALSE;
    }

    if (player->checkHammerQuake()) {
        cXyz swordTopPos = player->getSwordTopPos();
        swordTopPos.x -= actor->current.pos.x;
        swordTopPos.z -= actor->current.pos.z;
        f32 distXZ = std::sqrtf(SQUARE(swordTopPos.x) + SQUARE(swordTopPos.z));
        if (distXZ < 1000.0f) {
            i_this->mFireClrTimer = 0;
            fire_emitter_clr(i_this);
            return TRUE;
        }
    }

    return FALSE;
}

/* 00000820-0000094C       .text anm_init__FP8bl_classifUcfi */
void anm_init(bl_class* i_this, int bckFileIdx, f32 morf, u8 loopMode, f32 speed, int soundFileIdx) {
    i_this->mCurrBckIdx = bckFileIdx;
    if (soundFileIdx >= 0) {
        i_this->mpMorf->setAnm(
            (J3DAnmTransform*)dComIfG_getObjectRes("BL", bckFileIdx),
            loopMode, morf, speed, 0.0f, -1.0f,
            dComIfG_getObjectRes("BL", soundFileIdx)
        );
    } else {
        i_this->mpMorf->setAnm(
            (J3DAnmTransform*)dComIfG_getObjectRes("BL", bckFileIdx),
            loopMode, morf, speed, 0.0f, -1.0f,
            NULL
        );
    }
}

/* 0000094C-00000C08       .text skull_atari_check__FP8bl_class */
BOOL skull_atari_check(bl_class* i_this) {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    i_this->mStts.Move();

    if (i_this->health == 0) {
        return TRUE;
    }

    if (shock_damage_check(i_this)) {
        i_this->m2D2 = 3;
        i_this->m306 = 0x22;
        return TRUE;
    }

    if (!i_this->mSph.ChkTgHit()) {
        return FALSE;
    }

    cCcD_Obj* hitObj = i_this->mSph.GetTgHitObj();
    if (hitObj == NULL) {
        return FALSE;
    }

    if (i_this->mHitTimer != 0) {
        return FALSE;
    }

    i_this->current.angle.y = fopAcM_searchPlayerAngleY(i_this) + 0x8000;
    u8 no_kill;
    s16 hit_timer = 8;
    no_kill = 0;
    i_this->mHitTimer = hit_timer;
    i_this->m2D4 = 0;

    switch (hitObj->GetAtType()) {
    case AT_TYPE_SWORD:
        i_this->m2D4 = 6;
        break;
    case AT_TYPE_WIND:
        no_kill = 1;
        i_this->m2D4 = 1;
        i_this->m2D2 = 3;
        i_this->m306 = 0x24;
        break;
    case AT_TYPE_SKULL_HAMMER:
        i_this->m2D4 = 4;
        if (player->getCutType() == daPy_py_c::CUT_TYPE_HAMMER_SIDESWING) {
            i_this->current.angle.y = player->shape_angle.y - 0x4000;
            i_this->m2D4 = 5;
            i_this->speed.y = 30.0f;
            i_this->speedF = 20.0f;
        }
        break;
    case AT_TYPE_BOOMERANG:
    case AT_TYPE_BOKO_STICK:
        i_this->m2D4 = 3;
        break;
    case AT_TYPE_BOMB:
        i_this->speed.y = 30.0f;
        // fallthrough
    case AT_TYPE_FIRE:
        i_this->mEnemyFire.mFireDuration = 0x3E8;
        break;
    case AT_TYPE_HOOKSHOT:
        no_kill = 1;
        i_this->m2D4 = 7;
        i_this->m2D2 = 5;
        i_this->m306 = 0x3C;
        break;
    case AT_TYPE_FIRE_ARROW:
    case AT_TYPE_ICE_ARROW:
    case AT_TYPE_LIGHT_ARROW:
        break;
    }

    cXyz hitPos = *i_this->mSph.GetTgHitPosP();
    if (no_kill == 0) {
        fopAcM_seStart(i_this, JA_SE_OBJ_BREAK_STONE, 0);
        i_this->health = 0;
        i_this->attention_info.flags = 0;
        i_this->m2D2 = 0xA;
        i_this->m306 = 0x67;
    }

    return TRUE;
}

/* 00000C44-00001768       .text blue_body_atari_check__FP8bl_class */
BOOL blue_body_atari_check(bl_class* i_this) {
    fopAc_ac_c* actor = i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    CcAtInfo atInfo;

    i_this->mStts.Move();

    if (shock_damage_check(i_this)) {
        i_this->m2D2 = 3;
        i_this->m306 = 0x22;
        return TRUE;
    }

    if (!i_this->mSph.ChkTgHit()) {
        return FALSE;
    }

    cCcD_Obj* hitObj = i_this->mSph.GetTgHitObj();
    if (hitObj == NULL) {
        return FALSE;
    }

    if (i_this->mHitTimer != 0) {
        return FALSE;
    }

    i_this->current.angle.y = fopAcM_searchPlayerAngleY(actor) + 0x8000;
    u8 no_kill = 0;
    i_this->mHitTimer = 8;
    i_this->m2D4 = 0;

    switch (hitObj->GetAtType()) {
    case AT_TYPE_GRAPPLING_HOOK:
        if (i_this->mBtkMode != 0) {
            i_this->stealItemLeft = i_this->m2EA;
            if (i_this->stealItemLeft > 0) {
                s8 origHealth = i_this->health;
                i_this->health = 10;
                atInfo.mpObj = i_this->mSph.GetTgHitObj();
                atInfo.pParticlePos = NULL;
                cc_at_check(actor, &atInfo);
                i_this->health = origHealth;
            }
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x42);
            if (i_this->m2EA > 0) {
                i_this->m2EA--;
            }
            dComIfGp_particle_set(dPa_name::ID_IT_JN_PIYOHIT00, &i_this->attention_info.position);
            no_kill = 1;
        } else {
            i_this->stealItemLeft = 0;
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x33);
        }
        break;
    case AT_TYPE_SWORD:
        if (i_this->mBtkMode == 0) {
            fopAcM_seStart(actor, JA_SE_LK_SW_HIT_S, 0x33);
        } else {
            fopAcM_seStart(actor, JA_SE_LK_SW_HIT_S, 0x42);
        }
        if (player->getCutType() == daPy_py_c::CUT_TYPE_CUT_EA ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_EB ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_TURN ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_ROLL ||
            player->getCutType() == daPy_py_c::CUT_TYPE_JUMPCUT_SWORD ||
            player->getCutType() == daPy_py_c::CUT_TYPE_JUMPCUT_STICK ||
            player->getCutType() == daPy_py_c::CUT_TYPE_JUMPCUT_MACHETE ||
            player->getCutType() == daPy_py_c::CUT_TYPE_BT_JUMPCUT ||
            player->getCutType() == daPy_py_c::CUT_TYPE_BT_ROLLCUT ||
            player->getCutType() == daPy_py_c::CUT_TYPE_BT_VERTICALJUMPCUT ||
            player->getCutType() == daPy_py_c::CUT_TYPE_JUMPCUT_CLUB ||
            player->getCutType() == daPy_py_c::CUT_TYPE_JUMPCUT_DN_SWORD ||
            player->getCutType() == daPy_py_c::CUT_TYPE_JUMPCUT_SPEAR ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_EXA ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_EXB ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_EXMJ ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_KESA)
        {
            i_this->m2D4 = 6;
        }
        break;
    case AT_TYPE_BOMB:
        if (i_this->mBtkMode == 0) {
            i_this->m2D4 = 2;
            break;
        }
        // fallthrough
    case AT_TYPE_WIND:
        no_kill = 1;
        i_this->m2D4 = 1;
        i_this->m2D2 = 3;
        i_this->m306 = 0x24;
        if (!i_this->mAcch.ChkGroundHit() && i_this->gravity > -2.0f) {
            i_this->m306 = 0x1E;
        }
        break;
    case AT_TYPE_SKULL_HAMMER:
        if (i_this->mBtkMode == 0) {
            fopAcM_seStart(actor, JA_SE_LK_HAMMER_HIT, 0x33);
        } else {
            fopAcM_seStart(actor, JA_SE_LK_HAMMER_HIT, 0x42);
        }
        i_this->m2D4 = 4;
        if (player->getCutType() == daPy_py_c::CUT_TYPE_HAMMER_SIDESWING) {
            i_this->current.angle.y = player->shape_angle.y - 0x4000;
            i_this->m2D4 = 5;
        }
        if (i_this->mBtkMode == 0) {
            i_this->health = 0;
        }
        break;
    case AT_TYPE_BOOMERANG:
    case AT_TYPE_BOKO_STICK:
        no_kill = 1;
        if (i_this->mBtkMode == 0) {
            fopAcM_seStart(actor, JA_SE_LK_W_WEP_HIT, 0x33);
        } else {
            fopAcM_seStart(actor, JA_SE_LK_W_WEP_HIT, 0x42);
        }
        i_this->m2D4 = 3;
        break;
    case AT_TYPE_FIRE:
    case AT_TYPE_FIRE_ARROW:
        if (i_this->mBtkMode == 0) {
            i_this->mEnemyFire.mFireDuration = 100;
            fopAcM_seStart(actor, JA_SE_CM_BL_WIND_HIT, 0);
            fopAcM_monsSeStart(actor, JA_SE_CV_BL_DAMAGE, 0);
            i_this->mSph.OffAtSetBit();
            i_this->mSph.ClrAtSet();
            i_this->attention_info.flags = 0;
            i_this->mFireClrTimer = 0;
            fire_emitter_clr(i_this);
        }
        break;
    case AT_TYPE_LIGHT_ARROW:
        i_this->mEnemyIce.mLightShrinkTimer = 1;
        i_this->mEnemyIce.mParticleScale = 1.0f;
        i_this->mEnemyIce.mYOffset = 40.0f;
        fopAcM_seStart(actor, JA_SE_CM_BL_WIND_HIT, 0);
        fopAcM_monsSeStart(actor, JA_SE_CV_BL_DAMAGE, 0);
        i_this->current.angle.x = 0;
        i_this->current.angle.z = 0;
        i_this->mBtkMode = 0;
        i_this->shape_angle.x = 0;
        i_this->shape_angle.z = 0;
        i_this->gravity = 0.0f;
        i_this->speedF = 0.0f;
        i_this->m2EC = 0;
        i_this->speed.x = 0.0f;
        i_this->speed.y = 0.0f;
        i_this->speed.z = 0.0f;
        i_this->mSph.OffAtSetBit();
        i_this->mSph.ClrAtSet();
        i_this->attention_info.flags = 0;
        i_this->mFireClrTimer = 0;
        fire_emitter_clr(i_this);
        break;
    case AT_TYPE_ICE_ARROW:
        if (i_this->mBtkMode == 0) {
            no_kill = 1;
            i_this->mBtkMode = 0;
            i_this->current.angle.x = 0;
            i_this->current.angle.z = 0;
            i_this->shape_angle.x = 0;
            i_this->shape_angle.z = 0;
            i_this->gravity = 0.0f;
            i_this->speedF = 0.0f;
            i_this->m2EC = 0;
            i_this->speed.x = 0.0f;
            i_this->speed.y = 0.0f;
            i_this->speed.z = 0.0f;
            i_this->mSph.OffAtSetBit();
            i_this->mSph.ClrAtSet();
            i_this->mEnemyIce.mFreezeDuration = 200;
            enemy_fire_remove(&i_this->mEnemyFire);
            i_this->attention_info.flags = 0;
        } else {
            no_kill = 1;
            i_this->m2D4 = 1;
            i_this->m2D2 = 3;
            i_this->m306 = 0x1E;
        }
        break;
    case AT_TYPE_HOOKSHOT:
        no_kill = 1;
        if (i_this->mBtkMode == 0) {
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x33);
        } else {
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x42);
        }
        i_this->m2D4 = 7;
        i_this->m2D2 = 5;
        i_this->m306 = 0x3C;
        break;
    default:
        if (i_this->mBtkMode == 0) {
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x33);
        } else {
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x42);
        }
        break;
    }

    cXyz hitPos = *i_this->mSph.GetTgHitPosP();
    if (i_this->mBtkMode != 0 && i_this->m2D4 != 7 && i_this->m2D4 != 1) {
        dComIfGp_particle_set(dPa_name::ID_AK_JN_NG, &hitPos);
        i_this->m2D2 = 2;
        i_this->m306 = 0x14;
        return TRUE;
    }

    if (no_kill == 0) {
        atInfo.mpObj = i_this->mSph.GetTgHitObj();
        atInfo.pParticlePos = NULL;
        cc_at_check(actor, &atInfo);
        if (i_this->m2D4 == 6 || i_this->m2D4 == 5 || i_this->m2D4 == 4 || i_this->health <= 0) {
            dComIfGp_particle_set(dPa_name::ID_AK_JN_CRITICALHITFLASH, &hitPos);
            cXyz scale(2.0f, 2.0f, 2.0f);
            dComIfGp_particle_set(dPa_name::ID_AK_JN_CRITICALHIT, &hitPos, &player->shape_angle, &scale);
        } else {
            dComIfGp_particle_set(dPa_name::ID_AK_JN_OK, &hitPos, &player->shape_angle);
        }
        i_this->m2D2 = 4;
        i_this->m306 = 0x28;
    }

    return TRUE;
}

/* 00001768-000022BC       .text red_body_atari_check__FP8bl_class */
BOOL red_body_atari_check(bl_class* i_this) {
    fopAc_ac_c* actor = i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    CcAtInfo atInfo;

    i_this->mStts.Move();

    if (shock_damage_check(i_this)) {
        i_this->m2D2 = 3;
        i_this->m306 = 0x22;
        return TRUE;
    }

    if (!i_this->mSph.ChkTgHit()) {
        return FALSE;
    }

    cCcD_Obj* hitObj = i_this->mSph.GetTgHitObj();
    if (hitObj == NULL) {
        return FALSE;
    }

    if (i_this->mHitTimer != 0) {
        return FALSE;
    }

    i_this->current.angle.y = fopAcM_searchPlayerAngleY(actor) + 0x8000;
    u8 no_kill = 0;
    cXyz hitPos = *i_this->mSph.GetTgHitPosP();
    s16 hit_timer = 8;
    i_this->mHitTimer = hit_timer;
    i_this->m2D4 = 0;

    switch (hitObj->GetAtType()) {
    case AT_TYPE_GRAPPLING_HOOK:
        if (i_this->mBtkMode != 0) {
            i_this->stealItemLeft = i_this->m2EA;
            if (i_this->stealItemLeft > 0) {
                s8 origHealth = i_this->health;
                i_this->health = 10;
                atInfo.mpObj = i_this->mSph.GetTgHitObj();
                atInfo.pParticlePos = NULL;
                cc_at_check(actor, &atInfo);
                i_this->health = origHealth;
            }
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x42);
            if (i_this->m2EA > 0) {
                i_this->m2EA--;
            }
            dComIfGp_particle_set(dPa_name::ID_IT_JN_PIYOHIT00, &i_this->attention_info.position);
            no_kill = 1;
        } else {
            i_this->stealItemLeft = 0;
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x33);
        }
        break;
    case AT_TYPE_SWORD:
        i_this->m2D4 = hit_timer;
        fopAcM_seStart(actor, JA_SE_LK_SW_HIT_S, 0x33);
        if (player->getCutType() == daPy_py_c::CUT_TYPE_CUT_EA ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_EB ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_TURN ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_ROLL ||
            player->getCutType() == daPy_py_c::CUT_TYPE_JUMPCUT_SWORD ||
            player->getCutType() == daPy_py_c::CUT_TYPE_JUMPCUT_STICK ||
            player->getCutType() == daPy_py_c::CUT_TYPE_JUMPCUT_MACHETE ||
            player->getCutType() == daPy_py_c::CUT_TYPE_BT_JUMPCUT ||
            player->getCutType() == daPy_py_c::CUT_TYPE_BT_ROLLCUT ||
            player->getCutType() == daPy_py_c::CUT_TYPE_BT_VERTICALJUMPCUT ||
            player->getCutType() == daPy_py_c::CUT_TYPE_JUMPCUT_CLUB ||
            player->getCutType() == daPy_py_c::CUT_TYPE_JUMPCUT_DN_SWORD ||
            player->getCutType() == daPy_py_c::CUT_TYPE_JUMPCUT_SPEAR ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_EXA ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_EXB ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_EXMJ ||
            player->getCutType() == daPy_py_c::CUT_TYPE_CUT_KESA)
        {
            i_this->m2D4 = 6;
        }
        break;
    case AT_TYPE_BOMB:
        if (i_this->mBtkMode == 0) {
            i_this->m2D4 = 2;
            break;
        }
        // fallthrough
    case AT_TYPE_WIND:
        no_kill = 1;
        i_this->m2D4 = 1;
        i_this->m2D2 = 3;
        i_this->m306 = 0x24;
        if (!i_this->mAcch.ChkGroundHit() && i_this->gravity > -2.0f) {
            i_this->m306 = 0x1E;
        }
        break;
    case AT_TYPE_SKULL_HAMMER:
        fopAcM_seStart(actor, JA_SE_LK_HAMMER_HIT, 0x33);
        i_this->m2D4 = 4;
        if (player->getCutType() == daPy_py_c::CUT_TYPE_HAMMER_SIDESWING) {
            i_this->current.angle.y = player->shape_angle.y - 0x4000;
            i_this->m2D4 = 5;
        }
        i_this->health = 0;
        break;
    case AT_TYPE_BOOMERANG:
    case AT_TYPE_BOKO_STICK:
        no_kill = 1;
        if (i_this->mBtkMode == 0) {
            fopAcM_seStart(actor, JA_SE_LK_W_WEP_HIT, 0x33);
        } else {
            fopAcM_seStart(actor, JA_SE_LK_W_WEP_HIT, 0x42);
        }
        i_this->m2D4 = 3;
        dComIfGp_particle_set(dPa_name::ID_AK_JN_NG, &hitPos);
        break;
    case AT_TYPE_NORMAL_ARROW:
    case AT_TYPE_FIRE:
    case AT_TYPE_FIRE_ARROW:
        if (i_this->mBtkMode != 0 && (hitObj->GetAtType() & AT_TYPE_FIRE)) {
            break;
        }
        if (i_this->mBtkMode == 0 && !(hitObj->GetAtType() & AT_TYPE_NORMAL_ARROW)) {
            i_this->mEnemyFire.mFireDuration = 100;
        }
        fopAcM_seStart(actor, JA_SE_CM_BL_WIND_HIT, 0);
        fopAcM_monsSeStart(actor, JA_SE_CV_BL_DAMAGE, 0);
        i_this->mSph.OffAtSetBit();
        i_this->mSph.ClrAtSet();
        i_this->attention_info.flags = 0;
        i_this->mFireClrTimer = 0;
        fire_emitter_clr(i_this);
        break;
    case AT_TYPE_LIGHT_ARROW:
        i_this->mEnemyIce.mLightShrinkTimer = 1;
        i_this->mEnemyIce.mParticleScale = 1.0f;
        i_this->mEnemyIce.mYOffset = 40.0f;
        fopAcM_seStart(actor, JA_SE_CM_BL_WIND_HIT, 0);
        fopAcM_monsSeStart(actor, JA_SE_CV_BL_DAMAGE, 0);
        i_this->current.angle.x = 0;
        i_this->current.angle.z = 0;
        i_this->mBtkMode = 0;
        i_this->shape_angle.x = 0;
        i_this->shape_angle.z = 0;
        i_this->gravity = 0.0f;
        i_this->speedF = 0.0f;
        i_this->m2EC = 0;
        i_this->speed.x = 0.0f;
        i_this->speed.y = 0.0f;
        i_this->speed.z = 0.0f;
        i_this->mSph.OffAtSetBit();
        i_this->mSph.ClrAtSet();
        i_this->attention_info.flags = 0;
        i_this->mFireClrTimer = 0;
        fire_emitter_clr(i_this);
        break;
    case AT_TYPE_ICE_ARROW:
        no_kill = 1;
        i_this->mBtkMode = 0;
        i_this->current.angle.x = 0;
        i_this->current.angle.z = 0;
        i_this->shape_angle.x = 0;
        i_this->shape_angle.z = 0;
        i_this->gravity = 0.0f;
        i_this->speedF = 0.0f;
        i_this->m2EC = 0;
        i_this->speed.x = 0.0f;
        i_this->speed.y = 0.0f;
        i_this->speed.z = 0.0f;
        i_this->mFireClrTimer = 0;
        fire_emitter_clr(i_this);
        i_this->mSph.OffAtSetBit();
        i_this->mSph.ClrAtSet();
        i_this->mEnemyIce.mFreezeDuration = 200;
        enemy_fire_remove(&i_this->mEnemyFire);
        i_this->attention_info.flags = 0;
        break;
    case AT_TYPE_HOOKSHOT:
        no_kill = 1;
        if (i_this->mBtkMode == 0) {
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x33);
        } else {
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x42);
        }
        i_this->m2D4 = 7;
        i_this->m2D2 = 5;
        i_this->m306 = 0x3C;
        break;
    default:
        no_kill = 1;
        if (i_this->mBtkMode == 0) {
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x33);
        } else {
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x42);
        }
        dComIfGp_particle_set(dPa_name::ID_AK_JN_NG, &hitPos);
        break;
    }

    if (no_kill == 0) {
        atInfo.mpObj = i_this->mSph.GetTgHitObj();
        atInfo.pParticlePos = NULL;
        cc_at_check(actor, &atInfo);
        if (i_this->m2D4 == 6 || i_this->m2D4 == 5 || i_this->m2D4 == 4 || i_this->health <= 0) {
            dComIfGp_particle_set(dPa_name::ID_AK_JN_CRITICALHITFLASH, &hitPos);
            cXyz scale(2.0f, 2.0f, 2.0f);
            dComIfGp_particle_set(dPa_name::ID_AK_JN_CRITICALHIT, &hitPos, &player->shape_angle, &scale);
        } else {
            dComIfGp_particle_set(dPa_name::ID_AK_JN_OK, &hitPos, &player->shape_angle);
        }

        if (i_this->mBtkMode != 0) {
            anm_init(i_this, dRes_INDEX_BL_BCK_UCHIWA_e, 1.0f, 0, 1.0f, -1);
            i_this->speedF = 60.0f + REG8_F(10);
            if (i_this->health <= 0) {
                i_this->speedF = 60.0f + REG8_F(11);
                if (i_this->m2D4 == 5) {
                    i_this->current.angle.y = player->shape_angle.y - 0x4000;
                }
            }
            fopAcM_monsSeStart(actor, JA_SE_CV_BL_DAMAGE, 0);
            i_this->m2D2 = 2;
            i_this->m306 = 0x15;
        } else {
            i_this->m2D2 = 4;
            i_this->m306 = 0x28;
        }
    }

    return TRUE;
}

/* 000022BC-00002354       .text bound_sound_set__FP8bl_class */
void bound_sound_set(bl_class* i_this) {
    fopAc_ac_c* actor = i_this;
    u32 vol = actor->speed.y * 3.3f;
    if (vol > 100) {
        vol = 100;
    }
    mDoAud_seStart(JA_SE_CM_BL_BOUND, &actor->eyePos, vol, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
}

/* 00002354-00002458       .text fuwafuwa_keisan__FP8bl_class */
void fuwafuwa_keisan(bl_class* i_this) {
    fopAc_ac_c* actor = i_this;
    f32 maxStep = 3.0f;
    if (i_this->m2D2 == 1) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        i_this->m304 += 0x3E8;
        i_this->m318 = 80.0f + player->current.pos.y;
        i_this->m318 += 10.0f * cM_ssin(i_this->m304);
        maxStep = 6.0f;
    } else {
        i_this->m304 += 0x1F4;
        i_this->m318 = 100.0f + i_this->m320;
        i_this->m318 += 40.0f * cM_ssin(i_this->m304);
    }
    cLib_addCalc2(&actor->current.pos.y, i_this->m318, 1.0f, maxStep);
}

/* 00002458-000024E4       .text BG_check__FP8bl_class */
void BG_check(bl_class* i_this) {
    if (i_this->m306 != 2) {
        i_this->mAcchCir.SetWall(40.0f, 40.0f);
        i_this->mAcch.CrrPos(*dComIfG_Bgsp());
        i_this->mAcch.OnLineCheck();
        if (i_this->m2D1 == 0) {
            if (i_this->mAcch.GetGroundH() == -G_CM3D_F_INF) {
                i_this->m320 = i_this->mAcch.GetGroundH();
            }
        }
    }
}

/* 000024E4-00002804       .text Line_check__FP8bl_class4cXyz */
BOOL Line_check(bl_class* i_this, cXyz destPos) {
    fopAc_ac_c* actor = i_this;
    dBgS_LinChk linChk;
    cXyz centerPos = actor->current.pos;
    centerPos.y += 100.0f;
    destPos.y += 100.0f;
    linChk.Set(&centerPos, &destPos, actor);
    if (!dComIfG_Bgsp()->LineCross(&linChk)) {
        return TRUE;
    }
    return FALSE;
}

/* 00002C3C-00002CC4       .text roll_check__FP8bl_class */
BOOL roll_check(bl_class* i_this) {
    fopAc_ac_c* actor = i_this;
    cLib_addCalcAngleS2(&actor->shape_angle.x, 0, 1, 0x1000);
    cLib_addCalcAngleS2(&actor->shape_angle.z, 0, 1, 0x1000);
    i_this->m2F8[0] = 0;
    i_this->m2F8[1] = 0;
    if (abs(actor->shape_angle.x) < 0x200 && abs(actor->shape_angle.z) < 0x200) {
        return TRUE;
    }
    return FALSE;
}

/* 00002CC4-00003054       .text way_check__FP8bl_classs */
void way_check(bl_class*, short) {
    /* Nonmatching */
}

/* 00003054-000039F0       .text action_dousa__FP8bl_class */
void action_dousa(bl_class*) {
    /* Nonmatching */
}

/* 000039F0-00003F68       .text action_kougeki__FP8bl_class */
void action_kougeki(bl_class*) {
    /* Nonmatching */
}

/* 00003F68-00004128       .text action_sagarimasu__FP8bl_class */
void action_sagarimasu(bl_class* i_this) {
    fopAc_ac_c* actor = i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    switch (i_this->m306) {
    case 0x14:
        anm_init(i_this, dRes_INDEX_BL_BCK_UCHIWA_e, 1.0f, 0, 1.0f, -1);
        actor->speedF = 20.0f + REG8_F(10);
        if (i_this->m2D4 == 4 || i_this->m2D4 == 5) {
            actor->speedF = 60.0f + REG8_F(11);
            if (i_this->m2D4 == 5) {
                actor->current.angle.y = player->shape_angle.y - 0x4000;
            }
        }
        i_this->m306++;
        // fallthrough
    case 0x15:
        cLib_addCalc0(&actor->speedF, 0.8f, 5.0f);
        if (actor->speedF < 0.1f) {
            i_this->m2D2 = 1;
            i_this->m306 = 0xA;
            if (i_this->mBtkMode != 0 && i_this->mType == 0 && actor->health <= 0) {
                cXyz pos = actor->current.pos;
                pos.y += 30.0f;
                fopAcM_createDisappear(actor, &pos, 5, 0, 0xFF);
                actor->attention_info.flags = 0;
                fopAcM_onActor(actor);
                fopAcM_delete(actor);
            }
        }
        break;
    }

    fire_kaiten_keisan(i_this);
    if (i_this->mType == 1) {
        blue_body_atari_check(i_this);
    } else {
        red_body_atari_check(i_this);
    }
}

/* 00004128-000046BC       .text action_kaze_move__FP8bl_class */
void action_kaze_move(bl_class*) {
    /* Nonmatching */
}

/* 000046BC-00004B84       .text action_itaiyo_ne_san__FP8bl_class */
void action_itaiyo_ne_san(bl_class*) {
    /* Nonmatching */
}

/* 00004B84-00004D3C       .text action_hook_atari__FP8bl_class */
void action_hook_atari(bl_class* i_this) {
    fopAc_ac_c* actor = i_this;

    switch (i_this->m306) {
    case 0x3C: {
        int i;
        for (i = 0; i < 4; i++) {
            i_this->m2F8[i] = 0;
        }

        if (i_this->mFireCb.getEmitter() != NULL) {
            i_this->mFireClrTimer = 0;
            fire_emitter_clr(i_this);
            anm_init(i_this, dRes_INDEX_BL_BCK_UCHIWA_e, 1.0f, 0, 1.0f, -1);
            i_this->mSph.OffAtSetBit();
            i_this->mSph.ClrAtSet();
            actor->speedF = 0.0f;
            fopAcM_seStart(actor, JA_SE_CM_BL_WIND_HIT, 0);
            fopAcM_monsSeStart(actor, JA_SE_CV_BL_DAMAGE, 0);
        }

        fopAcM_OnStatus(actor, fopAcStts_SHOWMAP_e);
        i_this->mSph.SetTgSe(0);
        i_this->mSph.SetTgHitMark(dCcG_TgHitMark_None_e);
        i_this->mSph.OffTgShield();
        actor->attention_info.flags = fopAc_Attn_LOCKON_BATTLE_e;
        i_this->m306++;
        break;
    }
    case 0x3D:
        if (!fopAcM_checkHookCarryNow(actor)) {
            i_this->m2D2 = 3;
            i_this->m306 = 0x20;
        }
        break;
    }

    i_this->mFollowCb.remove();
}

/* 00004D3C-00004DBC       .text action_come_wait__FP8bl_class */
void action_come_wait(bl_class* i_this) {
    fopAc_ac_c* actor = i_this;
    switch (i_this->m306) {
    case 0x46:
        if (i_this->mSwitch != 0xFF && dComIfGs_isSwitch(i_this->mSwitch, dComIfGp_roomControl_getStayNo())) {
            fopAcM_OnStatus(actor, fopAcStts_SHOWMAP_e);
            i_this->m2D2 = 3;
            i_this->m306 = 0x26;
        }
        break;
    }
}

/* 00004DBC-00005104       .text action_normal_skull__FP8bl_class */
void action_normal_skull(bl_class*) {
    /* Nonmatching */
}

/* 00005104-00005504       .text daBL_Execute__FP8bl_class */
static BOOL daBL_Execute(bl_class*) {
    /* Nonmatching */
}

/* 00005504-0000550C       .text daBL_IsDelete__FP8bl_class */
static BOOL daBL_IsDelete(bl_class*) {
    return TRUE;
}

/* 0000550C-0000558C       .text daBL_Delete__FP8bl_class */
static BOOL daBL_Delete(bl_class* i_this) {
    i_this->mFollowCb.remove();
    i_this->mFireCb.remove();
    i_this->mSmokeCb.remove();
    enemy_fire_remove(&i_this->mEnemyFire);
    dComIfG_resDelete(&i_this->mPhase, "BL");
    return TRUE;
}

/* 0000558C-0000583C       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c* i_this) {
    bl_class* a_this = (bl_class*)i_this;

    a_this->mpMorf = new mDoExt_McaMorf(
        (J3DModelData*)dComIfG_getObjectRes("BL", dRes_INDEX_BL_BDL_BL_e),
        NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes("BL", dRes_INDEX_BL_BCK_START01_e),
        J3DFrameCtrl::EMode_RESET, 0.0f, 0, -1, 1,
        NULL,
        0x80000,
        0x37441422
    );
    if (a_this->mpMorf == NULL || a_this->mpMorf->getModel() == NULL) {
        return FALSE;
    }

    J3DModel* model = a_this->mpMorf->getModel();

    a_this->mpBtkOn = new mDoExt_btkAnm();
    if (a_this->mpBtkOn == NULL) {
        return FALSE;
    }
    if (a_this->mpBtkOn->init(
            model->getModelData(),
            (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("BL", dRes_INDEX_BL_BTK_ON_e),
            TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, false
        ) == 0) {
        return FALSE;
    }
    if (a_this->mpBtkOn == NULL) {
        return FALSE;
    }

    a_this->mpBtkOff = new mDoExt_btkAnm();
    if (a_this->mpBtkOff == NULL) {
        return FALSE;
    }
    if (a_this->mpBtkOff->init(
            model->getModelData(),
            (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("BL", dRes_INDEX_BL_BTK_OFF_e),
            TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, false
        ) == 0) {
        return FALSE;
    }
    if (a_this->mpBtkOff == NULL) {
        return FALSE;
    }

    a_this->mpMorf->getModel()->setUserArea((u32)a_this);
    return a_this->mInvisModel.create(a_this->mpMorf->getModel()) != 0 ? TRUE : FALSE;
}

/* 00005884-00005D88       .text daBL_Create__FP10fopAc_ac_c */
static cPhs_State daBL_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, bl_class);
    bl_class* a_this = (bl_class*)i_this;

    static dCcD_SrcSph body_co_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_FIRE,
            /* SrcObjAt  Atp     */ 1,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsPlayer_e | cCcD_CoSPrm_VsGrpAll_e,
            /* SrcGObjAt Se      */ dCcG_SE_WOOD,
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

    cPhs_State phase_state = dComIfG_resLoad(&a_this->mPhase, "BL");
    if (phase_state == cPhs_COMPLEATE_e) {
        a_this->mType = fopAcM_GetParam(i_this);
        a_this->m2D1 = fopAcM_GetParam(i_this) >> 24;
        a_this->m2E9 = fopAcM_GetParam(i_this) >> 16;
        a_this->mSwitch = fopAcM_GetParam(i_this) >> 8;
        a_this->m324 = i_this->current.angle.z;
        i_this->current.angle.z = 0;
        i_this->shape_angle.z = 0;

        if (a_this->mType == 0xFF) {
            a_this->mType = 0;
        }
        if (a_this->m2D1 == 0xFF) {
            a_this->m2D1 = 0;
        }
        if (REG8_S(8) != 0) {
            a_this->m2D1 = 1;
        }
        if (REG8_S(9) != 0) {
            a_this->mType = REG8_S(9) - 1;
        }

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x14E0)) {
            return cPhs_ERROR_e;
        }

        if (a_this->mType & 2) {
            a_this->mType ^= 2;
            if (a_this->m2E9 != 0xFF) {
                a_this->mpPath = dPath_GetRoomPath(a_this->m2E9, fopAcM_GetRoomNo(i_this));
                if (a_this->m324 < 4.0f) {
                    a_this->m324 = 4.0f + cM_rndF(2.0f);
                }
            }
            if (REG8_S(9) != 0) {
                a_this->m2E9 = 0;
                a_this->mpPath = dPath_GetRoomPath(a_this->m2E9, fopAcM_GetRoomNo(i_this));
            }
        }

        fopAcM_SetMtx(i_this, a_this->mpMorf->getModel()->getBaseTRMtx());
        i_this->attention_info.flags = 0;
        a_this->mAcch.Set(
            fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this),
            i_this, 1, &a_this->mAcchCir, fopAcM_GetSpeed_p(i_this)
        );
        a_this->mStts.Init(0x50, 1, i_this);
        i_this->max_health = 2;
        i_this->health = 2;

        a_this->mEnemyIce.mpActor = i_this;
        f32 iceSize = 50.0f;
        a_this->mEnemyIce.mWallRadius = iceSize;
        a_this->mEnemyIce.mCylHeight = iceSize;
        a_this->mEnemyFire.mpMcaMorf = a_this->mpMorf;
        a_this->mEnemyFire.mpActor = i_this;

        static u8 fire_j[ARRAY_SIZE(a_this->mEnemyFire.mFlameJntIdxs)] = {
            BL_JNT_HEAD_e,
            BL_JNT_AGO_e,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        };
        static f32 fire_sc[ARRAY_SIZE(a_this->mEnemyFire.mParticleScale)] = {
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        };
        for (int i = 0; i < ARRAY_SIZE(a_this->mEnemyFire.mFlameJntIdxs); i++) {
            a_this->mEnemyFire.mFlameJntIdxs[i] = fire_j[i];
            a_this->mEnemyFire.mParticleScale[i] = fire_sc[i];
        }

        a_this->m328 = 50.0f;
        a_this->m300 = i_this->shape_angle.y;
        a_this->mSph.Set(body_co_sph_src);
        a_this->mSph.SetStts(&a_this->mStts);
        a_this->mSph.OffAtSetBit();
        a_this->mSph.ClrAtSet();
        i_this->attention_info.distances[fopAc_Attn_TYPE_BATTLE_e] = 0x29;
        i_this->itemTableIdx = dComIfGp_CharTbl()->GetNameIndex("bable_r", 0);
        i_this->gbaName = 0xE;

        if (a_this->mType == 1) {
            a_this->mSph.SetAtSpl(dCcG_At_Spl_UNK3);
            a_this->mSph.SetAtType(AT_TYPE_UNK800);
            i_this->itemTableIdx = dComIfGp_CharTbl()->GetNameIndex("bable", 0);
            i_this->gbaName = 0xF;
        }

        i_this->gravity = -3.0f;
        i_this->speed.y += i_this->gravity;
        if (i_this->speed.y < -55.0f) {
            i_this->speed.y = -55.0f;
        }
        a_this->m31C = 1.0f;
        a_this->mYOffset = 24.0f;
        a_this->m2C4 = i_this->current.pos;
        fopAcM_posMove(i_this, a_this->mStts.GetCCMoveP());
        BG_check(a_this);
        a_this->m320 = i_this->current.pos.y;
        if (a_this->m2D1 == 0) {
            a_this->m320 = a_this->mAcch.GetGroundH();
        }

        a_this->m2D2 = 0;
        if (!a_this->mAcch.ChkGroundHit()) {
            i_this->scale.setall(0.0f);
            i_this->gravity = 0.0f;
            i_this->speed.y = 0.0f;
            i_this->current.pos.y = 100.0f + a_this->m320;
            a_this->m306 = 4;
        } else {
            a_this->m306 = 0;
        }

        if (a_this->mSwitch != 0xFF && !dComIfGs_isSwitch(a_this->mSwitch, dComIfGp_roomControl_getStayNo())) {
            fopAcM_OffStatus(i_this, fopAcStts_SHOWMAP_e);
            a_this->m2D2 = 6;
            a_this->m306 = 0x46;
        }

        if (a_this->mType & 0x80) {
            i_this->max_health = 1;
            i_this->health = 1;
            a_this->mBtkMode = 1;
            a_this->mSph.OnCoSetBit();
            i_this->current.angle.y = cM_rndFX(32767.0f);
            i_this->shape_angle.y = i_this->current.angle.y;
            fopAcM_OnStatus(i_this, fopAcStts_UNK4000_e);
            a_this->m2F6 = 4;
            fopAcM_OffStatus(i_this, fopAcStts_SHOWMAP_e);
            i_this->attention_info.flags |= fopAc_Attn_ACTION_CARRY_e;
            a_this->m2D2 = 0xA;
            a_this->m306 = 0x64;
        } else {
            i_this->stealItemLeft = 1;
            a_this->m2EA = i_this->stealItemLeft;
        }

        draw_SUB(a_this);
    }

    return phase_state;
}

static actor_method_class l_daBL_Method = {
    (process_method_func)daBL_Create,
    (process_method_func)daBL_Delete,
    (process_method_func)daBL_Execute,
    (process_method_func)daBL_IsDelete,
    (process_method_func)daBL_Draw,
};

actor_process_profile_definition g_profile_BL = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_BL_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(bl_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_BL_e,
    /* Actor SubMtd */ &l_daBL_Method,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e | fopAcStts_UNK80000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
