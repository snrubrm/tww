/**
 * d_a_gm.cpp
 * Mini-Boss - Mothula / Enemy - Mothula Larva
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_gm.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_material.h"
#include "d/d_s_play.h"
#include "d/d_snap.h"
#include "d/d_particle_name.h"
#include "d/d_camera.h"
#include "f_op/f_op_camera.h"
#include "d/d_bg_s_lin_chk.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "c/c_damagereaction.h"
#include "SSystem/SComponent/c_lib.h"
#include "SSystem/SComponent/c_math.h"
#include "res/Object/Gm.h"

/* 000000EC-0000032C       .text nodeCallBack__FP7J3DNodei */
static BOOL nodeCallBack(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DModel* model = j3dSys.getModel();
        gm_class* i_this = (gm_class*)model->getUserArea();
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        if (i_this != NULL && i_this->m2CD == 0) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            cXyz offset;

            switch (jntNo) {
            case GM_JNT_HARAC_e:
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->mHaraPos);
                break;
            case GM_JNT_MUNE_e:
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->mMunePos);
                cMtx_YrotM(*calc_mtx, i_this->mMuneRot.y);
                cMtx_XrotM(*calc_mtx, i_this->mMuneRot.x);
                cMtx_ZrotM(*calc_mtx, i_this->mMuneRot.z);
                break;
            case GM_JNT_ATAMA_e:
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->mAtamaPos);
                offset.x = 70.0f;
                MtxPosition(&offset, &i_this->mAtamaOffsetPos);
                break;
            case GM_JNT_HANESLA_e:
            case GM_JNT_HANESRA_e:
            case GM_JNT_HANEULA_e:
            case GM_JNT_HANEURA_e: {
                s32 wingNo = (jntNo - GM_JNT_HANESLA_e) >> 1;
                cMtx_YrotM(*calc_mtx, i_this->mWingAngle[wingNo].y);
                cMtx_XrotM(*calc_mtx, i_this->mWingAngle[wingNo].x);
                cMtx_ZrotM(*calc_mtx, i_this->mWingAngle[wingNo].z);
                break;
            }
            }

            if (jntNo != GM_JNT_HANESLA_e && jntNo != GM_JNT_HANESRA_e &&
                jntNo != GM_JNT_HANEULA_e && jntNo != GM_JNT_HANEURA_e)
            {
                cMtx_YrotM(*calc_mtx, i_this->mExtraJointRot.y);
                cMtx_XrotM(*calc_mtx, i_this->mExtraJointRot.x);
                cMtx_ZrotM(*calc_mtx, i_this->mExtraJointRot.z);
            }

            MTXCopy(*calc_mtx, model->getAnmMtx(jntNo));
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 00000368-0000048C       .text draw_SUB__FP8gm_class */
void draw_SUB(gm_class* i_this) {
    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseScale(i_this->scale);
    mDoMtx_stack_c::transS(
        i_this->current.pos.x + i_this->mDrawOffset.x,
        i_this->current.pos.y + i_this->mDrawOffset.y,
        i_this->current.pos.z + i_this->mDrawOffset.z
    );
    mDoMtx_stack_c::XrotM(i_this->m43E);
    mDoMtx_stack_c::ZrotM(i_this->m442);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::YrotM(i_this->m320);
    mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    if (i_this->m2CD == 0) {
        i_this->mpMorf->calc();
        enemy_fire(&i_this->mEnemyFire);
    }

    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &i_this->current.pos, &i_this->tevStr);
}

/* 0000048C-00000908       .text daGM_Draw__FP8gm_class */
static BOOL daGM_Draw(gm_class* i_this) {
    fopAc_ac_c* actor = i_this;
    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.setLightTevColorType(model, &actor->tevStr);
    dSnap_RegistFig(DSNAP_TYPE_UNKBE, actor, actor->attention_info.position, actor->shape_angle.y, 1.0f, 1.0f, 1.0f);

    if (i_this->m2CD == 0 && i_this->mEnemyIce.mFreezeTimer > 20) {
        dMat_control_c::iceEntryDL(i_this->mpMorf, -1, &i_this->mInvisibleModel);
        return TRUE;
    }

    if (i_this->m2CD == 0) {
        int shapeIdx = 0;
        for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
            J3DMaterial* mat = model->getModelData()->getJointNodePointer(i)->getMesh();
            while (mat != NULL) {
                J3DShape* shape = mat->getShape();
                switch (shapeIdx) {
                case 2:
                    if ((i_this->mWingBits & 0x08) == 0) {
                        shape->show();
                    } else {
                        shape->hide();
                    }
                    break;
                case 3:
                    if ((i_this->mWingBits & 0x02) == 0) {
                        shape->show();
                    } else {
                        shape->hide();
                    }
                    break;
                case 4:
                    if ((i_this->mWingBits & 0x01) == 0) {
                        shape->show();
                    } else {
                        shape->hide();
                    }
                    break;
                case 5:
                    if ((i_this->mWingBits & 0x04) == 0) {
                        shape->show();
                    } else {
                        shape->hide();
                    }
                    break;
                default:
                    shape->show();
                    break;
                }
                mat = mat->getNext();
                shapeIdx++;
            }
        }

        switch (i_this->mBrkMode) {
        case 1:
            i_this->mpBrkDamage->entry(model->getModelData());
            break;
        case 2:
            i_this->mpBrkAtack->entry(model->getModelData());
            break;
        case 3:
            i_this->mpBrkDead->entry(model->getModelData());
            break;
        default:
            i_this->mpBrkGm->entry(model->getModelData());
            break;
        }

        i_this->mpMorf->entryDL();

        switch (i_this->mBrkMode) {
        case 1:
            i_this->mpBrkDamage->remove(model->getModelData());
            break;
        case 2:
            i_this->mpBrkAtack->remove(model->getModelData());
            break;
        case 3:
            i_this->mpBrkDead->remove(model->getModelData());
            break;
        default:
            i_this->mpBrkGm->remove(model->getModelData());
            break;
        }
    } else {
        i_this->mpMorf->updateDL();
    }

    J3DModelData* modelData = model->getModelData();
    int i;
    J3DModel* shadowModel = i_this->mpShadowModel;
    for (i = 0; i < modelData->getJointNum(); i++) {
        MTXCopy(model->getAnmMtx(i), shadowModel->getAnmMtx(i));
    }
    for (i = 0; i < modelData->getWEvlpMtxNum(); i++) {
        MTXCopy(model->getWeightAnmMtx(i), shadowModel->getWeightAnmMtx(i));
    }

    f32 posZ = actor->current.pos.z + i_this->mDrawOffset.z;
    cXyz shadowPos(
        actor->current.pos.x + i_this->mDrawOffset.x,
        REG8_F(18) + (10.0f + (actor->current.pos.y + i_this->mDrawOffset.y)),
        posZ
    );
    i_this->mShadowId = dComIfGd_setRealShadow(
        i_this->mShadowId,
        1,
        i_this->mpShadowModel,
        &shadowPos,
        900.0f + REG8_F(19),
        (actor->current.pos.y + i_this->mDrawOffset.y) - i_this->mAcch.GetGroundH(),
        NULL
    );

    if (REG8_S(1) == 0) {
        return TRUE;
    }

    if (i_this->m360[0].x || i_this->m360[0].z) {
        if (i_this->m360[1].x || i_this->m360[1].z) {
            int j;
            for (j = 0; j < 3; j++) {
                cXyz tmp = actor->current.pos + i_this->mDrawOffset;
            }
        }
    }

    return TRUE;
}

/* 00000908-00000A34       .text anm_init__FP8gm_classifUcfi */
void anm_init(gm_class* i_this, int bckFileIdx, float morf, unsigned char loopMode, float speed, int soundFileIdx) {
    i_this->mBckIdx = bckFileIdx;
    if (soundFileIdx >= 0) {
        void* soundAnm = dComIfG_getObjectRes("GM", soundFileIdx);
        J3DAnmTransform* bckAnm = (J3DAnmTransform*)dComIfG_getObjectRes("GM", bckFileIdx);
        i_this->mpMorf->setAnm(bckAnm, loopMode, morf, speed, 0.0f, -1.0f, soundAnm);
    } else {
        J3DAnmTransform* bckAnm = (J3DAnmTransform*)dComIfG_getObjectRes("GM", bckFileIdx);
        i_this->mpMorf->setAnm(bckAnm, loopMode, morf, speed, 0.0f, -1.0f, NULL);
    }
}

/* 00000A34-00000D00       .text wing_cut_stat__FP8gm_class */
BOOL wing_cut_stat(gm_class* i_this) {
    fopAc_ac_c* actor = i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    static cXyz wing_birth_pos_dt[] = {
        cXyz(94.0f, -72.0f, -30.0f),
        cXyz(-94.0f, -72.0f, -30.0f),
        cXyz(144.0f, 72.0f, -30.0f),
        cXyz(-144.0f, 72.0f, -30.0f),
    };

    int i = 0;
    int off = 0;
    int bit = 1;
    while (i < 4) {
        if ((i_this->mWingBits & bit) == 0) {
            u32 parameters = (i + 1) | 0x100;
            if (i_this->m2D0 == 4) {
                parameters = (i + 1) | 0x200;
            }

            cMtx_YrotS(*calc_mtx, actor->shape_angle.y);

            cXyz* src = (cXyz*)((char*)wing_birth_pos_dt + off);
            cXyz offset;
            offset.x = src->x;
            offset.y = src->y;
            offset.z = src->z;
            cXyz pos;
            MtxPosition(&offset, &pos);
            pos += actor->current.pos + i_this->mDrawOffset;

            csXyz angle;
            angle.x = actor->shape_angle.x;
            angle.y = actor->shape_angle.y;
            angle.z = actor->shape_angle.z;
            angle.y += (s16)0x8000;

            fopAcM_create(
                fpcNm_GM_e,
                parameters,
                &pos,
                fopAcM_GetRoomNo(actor),
                &angle,
                &actor->scale,
                0,
                NULL
            );
            dComIfGp_particle_set(dPa_name::ID_AK_JN_OK, &pos, &player->shape_angle);
            i_this->mWingBits |= bit;

            if (actor->health > 0) {
                if (i_this->m2D1 != 10) {
                    return TRUE;
                }
            }
        }

        bit <<= 1;
        i++;
        off += (int)sizeof(cXyz);
    }

    if (actor->health <= 0 || i_this->m2D1 == 10) {
        return TRUE;
    }
    return FALSE;
}

/* 00000D3C-0000170C       .text body_atari_check__FP8gm_class */
void body_atari_check(gm_class*) {
    /* Nonmatching */
    // Dummy pool so later 100% funcs keep orig @4193 / @stringBase0 offsets.
    volatile f32 dummy_250 = 250.0f;
    volatile f32 dummy_80 = 80.0f;
    volatile f32 dummy_2 = 2.0f;
    volatile f32 dummy_1000 = 1000.0f;
    volatile f32 dummy_60 = 60.0f;
    volatile f32 dummy_150 = 150.0f;
    volatile f32 dummy_600 = 600.0f;
    (void)dummy_250;
    (void)dummy_80;
    (void)dummy_2;
    (void)dummy_1000;
    (void)dummy_60;
    (void)dummy_150;
    (void)dummy_600;
    dComIfG_getObjectRes("gmos", 0);
    dComIfG_getObjectRes("gmos_f", 0);
    dComIfG_getObjectRes("gmos_g", 0);
}

/* 0000170C-000017D8       .text BG_check__FP8gm_class */
void BG_check(gm_class* i_this) {
    i_this->mAcchCir.SetWall(i_this->mWallH, i_this->mWallR);

    i_this->current.pos += i_this->mDrawOffset;
    i_this->old.pos.y += i_this->mDrawOffset.y;
    i_this->current.pos.y -= i_this->mCorrectionY;
    i_this->old.pos.y -= i_this->mCorrectionY;
    i_this->mAcch.CrrPos(*dComIfG_Bgsp());
    i_this->current.pos.y += i_this->mCorrectionY;
    i_this->old.pos.y += i_this->mCorrectionY;
    i_this->current.pos -= i_this->mDrawOffset;
    i_this->old.pos.y -= i_this->mDrawOffset.y;
}

/* 000017D8-00001B84       .text Line_check__FP8gm_class4cXyz */
BOOL Line_check(gm_class* i_this, cXyz destPos) {
    fopAc_ac_c* actor = i_this;
    dBgS_LinChk linChk;

    cMtx_XrotS(*calc_mtx, i_this->m43E);
    cMtx_ZrotM(*calc_mtx, i_this->m442);
    cXyz offset;
    offset.x = 0.0f;
    offset.y = 100.0f;
    offset.z = 0.0f;
    cXyz pos;
    MtxPosition(&offset, &pos);
    pos += i_this->mAtamaPos;
    pos += i_this->mDrawOffset;
    pos.y += 100.0f;
    destPos.y += 100.0f;
    linChk.Set(&pos, &destPos, actor);
    if (!dComIfG_Bgsp()->LineCross(&linChk)) {
        i_this->mLineStart = pos;
        i_this->mLineEnd = destPos;
        return FALSE;
    }
    return TRUE;
}

/* 00001FBC-0000212C       .text ks_set_rtn__FP8gm_class */
BOOL ks_set_rtn(gm_class* i_this) {
    u32 parameters = 4;
    if (i_this->m2CE == 1) {
        if (i_this->m31E >= 10) {
            return TRUE;
        }
    } else {
        if (i_this->m31E >= 20) {
            return TRUE;
        }
    }

    if (i_this->mAction == 83) {
        parameters = 5;
    }

    fopAcM_seStart(i_this, JA_SE_CM_GM_KUROBO_OUT, 0);

    csXyz angle = i_this->current.angle;
    angle.y = angle.y + cM_rndFX(4096.0f);

    fpc_ProcID id = fopAcM_createChild(
        fpcNm_KS_e,
        fopAcM_GetID(i_this),
        parameters,
        &i_this->mHaraPos,
        fopAcM_GetRoomNo(i_this),
        &angle,
        &i_this->scale,
        0,
        NULL
    );
    if (id != fpcM_ERROR_PROCESS_ID_e) {
        i_this->m31E++;
        return FALSE;
    }
    return TRUE;
}

/* 0000212C-00002204       .text wing_ret_set__FP8gm_class */
void wing_ret_set(gm_class* i_this) {
    s16 target = 0;
    for (int i = 0; i < 4; i++) {
        cLib_addCalcAngleS2(&i_this->mWingAngle[i].y, target, 1, 0x200);
        cLib_addCalcAngleS2(&i_this->mWingAngle[i].x, target, 1, 0x200);
        cLib_addCalcAngleS2(&i_this->mWingAngle[i].z, target, 1, 0x200);
    }
    cLib_addCalcAngleS2(&i_this->mExtraJointRot.y, target, 1, 0x200);
    cLib_addCalcAngleS2(&i_this->mExtraJointRot.x, target, 1, 0x200);
    cLib_addCalcAngleS2(&i_this->mExtraJointRot.z, target, 1, 0x200);
    cLib_addCalcAngleS2(&i_this->mMuneRot.y, target, 1, 0x200);
}

/* 00002204-000022C4       .text fuwafuwa_set__FP8gm_class */
void fuwafuwa_set(gm_class* i_this) {
    if (i_this->current.pos.y < 150.0f + dComIfGp_getPlayer(0)->current.pos.y) {
        return;
    }

    i_this->mTimers[0] += 1000;
    i_this->mFuwafuwa.y = 100.0f * cM_ssin(i_this->mTimers[0]);
    if (i_this->mAction == 3 || i_this->mAction == 4) {
        i_this->mFuwafuwa.x = 0.0f;
        i_this->mFuwafuwa.y = 0.0f;
        i_this->mFuwafuwa.z = 0.0f;
    }
    cLib_addCalc2(&i_this->mDrawOffset.y, i_this->mFuwafuwa.y, 1.0f, 10.0f);
}

/* 000022C4-00002474       .text fly_move__FP8gm_class */
BOOL fly_move(gm_class* i_this) {
    fopAc_ac_c* actor = i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    s16 ang = 0x2000;

    if (i_this->m2E2 == 0) {
        if (cM_rnd() < 0.5f) {
            ang = -0x2000;
        }

        s16 camY = dCam_getAngleY((camera_class*)dComIfGp_getCamera(0));
        for (int i = 0; i < 2; i++) {
            s16 rot = camY + ang;
            cMtx_YrotS(*calc_mtx, rot);
            cXyz offset;
            offset.x = 0.0f;
            offset.y = 0.0f;
            offset.z = 300.0f;
            cXyz pos;
            MtxPosition(&offset, &pos);
            pos += player->current.pos;
            pos.y = actor->current.pos.y + i_this->mDrawOffset.y;

            if (!Line_check(i_this, pos)) {
                f32 dx = pos.x - (actor->current.pos.x + i_this->mDrawOffset.x);
                f32 dz = pos.z - (actor->current.pos.z + i_this->mDrawOffset.z);
                actor->speedF = 10.0f;
                i_this->m2E2 = 20;
                i_this->m2E2 += (s16)cM_rndF(20.0f);
                i_this->m334 = 0.0f;
                i_this->m434 = cM_atan2s(dx, dz);
                return TRUE;
            }

            ang += 0x8000;
        }
    }

    return FALSE;
}

/* 00002474-00002AB8       .text action_dousa__FP8gm_class */
void action_dousa(gm_class*) {
    /* Nonmatching */
}

/* 00002AB8-00002C9C       .text action_hane_rakka__FP8gm_class */
void action_hane_rakka(gm_class* i_this) {
    fopAc_ac_c* actor = i_this;

    switch (i_this->mAction) {
    case 10: {
        for (int i = 0; i < 23; i++) {
            i_this->mTimers[i] = 0;
        }
        actor->gravity = -1.0f;
        actor->speed.y = 7.0f;
        if (i_this->m2CD != 2) {
            actor->speedF = 40.0f;
            if (i_this->mWingBits == 1 || i_this->mWingBits == 3) {
                actor->current.angle.y += (s16)(16384.0f + cM_rndFX(4096.0f));
                i_this->mTimers[0] = 0x1000;
            } else {
                actor->current.angle.y -= (s16)(16384.0f + cM_rndFX(4096.0f));
                i_this->mTimers[0] = -0x1000;
            }
            i_this->mTimers[1] = 0x300;
        }
        i_this->mAction++;
        /* fallthrough */
    }
    case 11:
        if (i_this->mAcch.ChkGroundHit() || i_this->mSpawnPos.y - 1000.0f > actor->current.pos.y) {
            actor->shape_angle.x = 0;
            i_this->mTimers[0] = 0;
            i_this->mTimers[1] = 0;
            i_this->mAction++;
        }
        break;
    case 12:
        if (actor->scale.x < 0.1f) {
            fopAcM_delete(actor);
        }
        break;
    }

    cLib_addCalc0(&actor->scale.x, 1.0f, 0.02f);
    f32 scale = actor->scale.x;
    actor->scale.y = scale;
    actor->scale.z = scale;
    cLib_addCalc0(&actor->speedF, 1.0f, 5.0f);
    if (actor->speed.y < 0.0f) {
        actor->shape_angle.y += i_this->mTimers[0];
        actor->shape_angle.x += i_this->mTimers[1];
    }
}

static csXyz wing_trans_angle_dt[4];

/* 00002C9C-000030FC       .text action_uchiwa_dousa__FP8gm_class */
void action_uchiwa_dousa(gm_class* i_this) {
    fopAc_ac_c* actor = i_this;

    switch (i_this->mAction) {
    case 20: {
        anm_init(i_this, dRes_INDEX_GM_BCK_PAUSE_e, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f, -1);
        for (int i = 0; i < 23; i++) {
            i_this->mTimers[i] = 0;
        }
        actor->current.angle.y = fopAcM_searchActorAngleY(actor, dComIfGp_getPlayer(0)) + 0x8000;
        fopAcM_seStart(actor, JA_SE_CM_GM_HIT_WIND, 0);
        fopAcM_monsSeStart(actor, JA_SE_CV_GM_HIT_WIND, 0);
        i_this->mWallR = 250.0f;
        i_this->mWeaponSph.OffAtSetBit();
        i_this->mWeaponSph.OffAtSetBit();
        i_this->mWingCyl.OffAtSetBit();
        i_this->mWingCyl.OffAtSetBit();
        actor->speedF = 60.0f;
        actor->speed.y = 24.0f;
        if (i_this->m2D1 == 3) {
            actor->speedF = 20.0f;
            actor->speed.y = 22.0f;
        }
        i_this->mTimers[3] = 5000;
        i_this->mpMorf->setPlaySpeed(0.0f);
        i_this->mAction++;
        /* fallthrough */
    }
    case 21: {
        cLib_addCalc0(&actor->speedF, 1.0f, 3.0f);
        cLib_addCalc0(&actor->speed.y, 1.0f, 2.0f);
        for (int i = 0; i < 4; i++) {
            i_this->mWingAngle[i].y = wing_trans_angle_dt[i].y;
            i_this->mWingAngle[i].x = wing_trans_angle_dt[i].x;
            i_this->mWingAngle[i].z = wing_trans_angle_dt[i].z;
        }
        i_this->mTimers[0] += 400;
        i_this->mTimers[1] += 400;
        i_this->mTimers[2] += 400;
        i_this->mExtraJointRot.y = 4000.0f * cM_ssin(i_this->mTimers[0]);
        i_this->mExtraJointRot.x = 4000.0f * cM_scos(i_this->mTimers[1]);
        i_this->mExtraJointRot.z = 4000.0f * cM_scos(i_this->mTimers[2]);
        if (i_this->m2D1 != 3) {
            actor->shape_angle.z += i_this->mTimers[3];
            s16 target = 0;
            cLib_addCalcAngleS2(&i_this->mTimers[3], target, 1, 0x100);
        }
        if (actor->speed.y < 0.1f && actor->speedF < 0.1f) {
            actor->speedF = 0.0f;
            i_this->mAction++;
        }
        break;
    }
    case 22:
        actor->gravity = -0.08f;
        i_this->mTimers[0] += 400;
        i_this->mTimers[1] += 400;
        i_this->mTimers[2] += 400;
        i_this->mExtraJointRot.y = 4000.0f * cM_ssin(i_this->mTimers[0]);
        i_this->mExtraJointRot.x = 4000.0f * cM_scos(i_this->mTimers[1]);
        i_this->mExtraJointRot.z = 4000.0f * cM_scos(i_this->mTimers[2]);
        if (actor->current.pos.y < 150.0f + i_this->mAcch.GetGroundH()) {
            actor->speed.y = 0.0f;
            actor->gravity = 0.0f;
            i_this->m2D0 = 0;
            i_this->mAction = 0;
        }
        break;
    }

    s16 muneTarget = 0x2000;
    cLib_addCalcAngleS2(&i_this->mMuneRot.y, muneTarget, 1, 0x1000);
}

/* 000030FC-00003E54       .text action_totugeki__FP8gm_class */
void action_totugeki(gm_class*) {
    /* Nonmatching */
}

/* 00003E54-00004204       .text action_kabehari__FP8gm_class */
void action_kabehari(gm_class* i_this) {
    fopAc_ac_c* actor = i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz offset;
    cXyz pos;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;

    switch (i_this->mAction) {
    case 40:
        i_this->mWallR = 70.0f;
        for (int i = 0; i < 23; i++) {
            i_this->mTimers[i] = 0;
        }
        i_this->mWingCyl.OffAtSetBit();
        i_this->mWingCyl.OffAtSetBit();
        i_this->m348 = player->current.pos;
        i_this->m348.y = 450.0f + player->current.pos.y;
        i_this->m320 = 0;
        i_this->m2E4 = 50;
        i_this->mAction++;
        /* fallthrough */
    case 41: {
        s16 pitchTarget = -0x4000;
        cLib_addCalcAngleS2(&actor->shape_angle.x, pitchTarget, 1, 0x200);
        f32 dx = actor->current.pos.x - pos.x;
        f32 dy = actor->current.pos.y - i_this->m348.y;
        f32 dz = actor->current.pos.z - pos.z;
        f32 dist = std::sqrtf(dx * dx + dy * dy + dz * dz);
        if (dist < 4.0f || i_this->mAcch.ChkWallHit() || i_this->m2E4 == 0) {
            if (i_this->mBckIdx != dRes_INDEX_GM_BCK_TOMARU_e) {
                anm_init(i_this, dRes_INDEX_GM_BCK_TOMARU_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1);
            }
            i_this->mWallR = 10.0f;
            actor->shape_angle.x = -0x4000;
            i_this->mAction = 42;
        }
        break;
    }
    case 42: {
        s16 spinTarget = -0x8000;
        cLib_addCalcAngleS2(&i_this->m320, spinTarget, 1, 0x500);
        s16 dang = cLib_distanceAngleS(i_this->m320, spinTarget);
        if (dang < 0x100) {
            i_this->m2E2 = 10;
            i_this->mAction = 43;
        }
        break;
    }
    case 43: {
        s16 spinTarget = -0x8000;
        cLib_addCalcAngleS2(&i_this->m320, spinTarget, 1, 0x500);
        if (i_this->m2E2 == 0) {
            if (i_this->m322 <= 1 && (i_this->mWingBits & 7) != 7) {
                actor->shape_angle.y = fopAcM_searchActorAngleY(actor, dComIfGp_getPlayer(0));
                actor->current.angle.y = actor->shape_angle.y;
                i_this->m2D0 = 3;
                i_this->mAction = 33;
            } else {
                i_this->m2D0 = 0;
                i_this->mAction = 0;
            }
        }
        break;
    }
    }

    if (i_this->m2D0 == 4) {
        s16 rot = actor->current.angle.y + 0x8000;
        cMtx_YrotS(*calc_mtx, rot);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 60.0f;
        MtxPosition(&offset, &pos);
        pos += i_this->m384[2];
        cLib_addCalc2(&actor->current.pos.x, pos.x, 1.0f, 10.0f);
        cLib_addCalc2(&actor->current.pos.y, i_this->m348.y, 1.0f, 10.0f);
        cLib_addCalc2(&actor->current.pos.z, pos.z, 1.0f, 10.0f);
        offset = i_this->m384[0] - i_this->m384[1];
        actor->current.angle.y = cM_atan2s(offset.x, offset.z) + 0x4000;
        s16 shapeTarget = actor->current.angle.y;
        cLib_addCalcAngleS2(&actor->shape_angle.y, shapeTarget, 1, 0x1000);
    }
}

/* 00004204-000046F0       .text action_fly_damage__FP8gm_class */
void action_fly_damage(gm_class* i_this) {
    fopAc_ac_c* actor = i_this;

    switch (i_this->mAction) {
    case 50:
        i_this->mBodyCyl.SetCoVsGrp(cCcD_CoSPrm_VsGrpAll_e);
        if (i_this->m2D1 != 4) {
            i_this->mpBrkDamage->setFrame(0.0f);
            i_this->mBrkMode = 1;
        } else {
            i_this->mpBrkGm->setFrame(0.0f);
            i_this->mBrkMode = 0;
        }
        anm_init(i_this, dRes_INDEX_GM_BCK_PAUSE_e, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f, -1);
        actor->speedF = 30.0f;
        i_this->m320 = 0;
        /* fallthrough */
    case 52: {
        i_this->mWeaponSph.OffAtSetBit();
        i_this->mWeaponSph.OffAtSetBit();
        i_this->mWingCyl.OffAtSetBit();
        i_this->mWingCyl.OffAtSetBit();
        for (int i = 0; i < 23; i++) {
            i_this->mTimers[i] = 0;
        }
        actor->current.angle.y = fopAcM_searchActorAngleY(actor, dComIfGp_getPlayer(0)) + 0x8000;
        f32 sign = 1.0f;
        if (cM_rnd() < 0.5f) {
            sign = -1.0f;
        }
        i_this->mTimers[16] += (s16)(10000.0f * sign);
        sign = 1.0f;
        if (cM_rnd() < 0.5f) {
            sign = -1.0f;
        }
        i_this->mTimers[17] += (s16)(10000.0f * sign);
        sign = 1.0f;
        if (cM_rnd() < 0.5f) {
            sign = -1.0f;
        }
        i_this->mTimers[18] += (s16)(2000.0f * sign);
        i_this->mTimers[19] = 5000;
        i_this->mTimers[20] = 5000;
        i_this->mTimers[21] = 7000;
        actor->speed.setall(0.0f);
        actor->gravity = 0.0f;
        i_this->mAction = 51;
        /* fallthrough */
    }
    case 51: {
        for (int i = 0; i < 4; i++) {
            i_this->mTimers[4 + i * 3] += i_this->mTimers[16];
            i_this->mTimers[5 + i * 3] += i_this->mTimers[17];
            i_this->mTimers[6 + i * 3] += i_this->mTimers[18];
            i_this->mWingAngle[i].y = 5000.0f * cM_ssin(i_this->mTimers[4 + i * 3]);
            i_this->mWingAngle[i].x = 5000.0f * cM_scos(i_this->mTimers[5 + i * 3]);
            i_this->mWingAngle[i].z = 3000.0f * cM_scos(i_this->mTimers[6 + i * 3]);
        }
        i_this->mTimers[0] += i_this->mTimers[19];
        i_this->mTimers[1] += i_this->mTimers[20];
        i_this->mTimers[2] += i_this->mTimers[21];
        i_this->mExtraJointRot.y = -3500.0f * cM_ssin(i_this->mTimers[0]);
        i_this->mExtraJointRot.x = -3500.0f * cM_scos(i_this->mTimers[1]);
        i_this->mExtraJointRot.z = -2000.0f * cM_scos(i_this->mTimers[2]);
        for (int i = 0; i < 6; i++) {
            s16 target = 0;
            cLib_addCalcAngleS2(&i_this->mTimers[16 + i], target, 1, 80);
        }
        cLib_addCalc0(&actor->speedF, 1.0f, 2.0f);
        if (actor->speedF < 0.2f) {
            actor->speedF = 0.0f;
            i_this->mpBrkGm->setFrame(0.0f);
            i_this->mBrkMode = 0;
            if (i_this->mBckIdx == dRes_INDEX_GM_BCK_PAUSE_e && (i_this->mWingBits & 7) != 7) {
                i_this->m2D0 = 3;
                i_this->mAction = 37;
            } else {
                i_this->m2D0 = 0;
                i_this->mAction = 0;
            }
        }
        break;
    }
    }

    if (actor->current.pos.y < 100.0f + i_this->mAcch.GetGroundH()) {
        cLib_addCalc2(&actor->current.pos.y, 100.0f + i_this->mAcch.GetGroundH(), 1.0f, 3.0f);
    }
}

/* 000046F0-00005C78       .text action_ground_attack__FP8gm_class */
void action_ground_attack(gm_class*) {
    /* Nonmatching */
}

/* 00005C78-00006A98       .text action_demo__FP8gm_class */
void action_demo(gm_class*) {
    /* Nonmatching */
}

/* 00006A98-00007770       .text daGM_Execute__FP8gm_class */
static BOOL daGM_Execute(gm_class*) {
    /* Nonmatching */
}

/* 00007770-00007778       .text daGM_IsDelete__FP8gm_class */
static BOOL daGM_IsDelete(gm_class*) {
    return TRUE;
}

/* 00007778-000078C4       .text daGM_Delete__FP8gm_class */
static BOOL daGM_Delete(gm_class* i_this) {
    dComIfG_resDelete(&i_this->mPhs, "GM");
    if (i_this->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }
    enemy_fire_remove(&i_this->mEnemyFire);
    if (i_this->health == -128) {
        if (i_this->mSwitchNo != 0xFF && i_this->mSwitchEnable != 0) {
            dComIfGs_onSwitch(i_this->mSwitchNo, fopAcM_GetRoomNo(i_this));
        }
        fopAcM_onActor(i_this);
    }
    if (i_this->mpEmitters[0] != NULL) {
        i_this->mpEmitters[0]->becomeInvalidEmitter();
        i_this->mpEmitters[0] = NULL;
    }
    if (i_this->mpEmitters[1] != NULL) {
        i_this->mpEmitters[1]->becomeInvalidEmitter();
        i_this->mpEmitters[1] = NULL;
    }
    if (i_this->mpEmitters[2] != NULL) {
        i_this->mpEmitters[2]->becomeInvalidEmitter();
        i_this->mpEmitters[2] = NULL;
    }
    if (i_this->mpEmitters[3] != NULL) {
        i_this->mpEmitters[3]->becomeInvalidEmitter();
        i_this->mpEmitters[3] = NULL;
    }
    return TRUE;
}

/* 000078C4-00007E74       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c* a_this) {
    gm_class* i_this = (gm_class*)a_this;

    if (i_this->m2CD == 0) {
        i_this->mpMorf = new mDoExt_McaMorf(
            (J3DModelData*)dComIfG_getObjectRes("GM", dRes_INDEX_GM_BDL_GM_e),
            NULL, NULL, NULL,
            J3DFrameCtrl::EMode_RESET, 1.0f, 0, -1, 1,
            NULL, 0x80000, 0x37441422
        );
    } else {
        switch (i_this->mWingBits) {
        case 1:
        case 2:
            i_this->mpMorf = new mDoExt_McaMorf(
                (J3DModelData*)dComIfG_getObjectRes("GM", dRes_INDEX_GM_BDL_HANESL_e),
                NULL, NULL, NULL,
                J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, 0,
                NULL, 0, 0x11020203
            );
            break;
        case 3:
        case 4:
            i_this->mpMorf = new mDoExt_McaMorf(
                (J3DModelData*)dComIfG_getObjectRes("GM", dRes_INDEX_GM_BDL_HANEUL_e),
                NULL, NULL, NULL,
                J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, 0,
                NULL, 0, 0x11020203
            );
            break;
        }
    }

    if (i_this->mpMorf == NULL || i_this->mpMorf->getModel() == NULL) {
        return FALSE;
    }

    if (i_this->m2CD == 0) {
        i_this->mpMorf->getModel()->setUserArea((u32)i_this);
        for (u16 i = 0; i < i_this->mpMorf->getModel()->getModelData()->getJointNum(); i++) {
            i_this->mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
    }

    J3DModel* model = i_this->mpMorf->getModel();
    J3DModelData* modelData = model->getModelData();
    if (i_this->m2CD == 0) {
        i_this->mpBrkGm = new mDoExt_brkAnm();
        if (i_this->mpBrkGm == NULL) {
            return FALSE;
        }
        if (i_this->mpBrkGm->init(
                model->getModelData(),
                (J3DAnmTevRegKey*)dComIfG_getObjectRes("GM", dRes_INDEX_GM_BRK_GM_e),
                TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, FALSE
            ) == 0)
        {
            return FALSE;
        }

        i_this->mpBrkAtack = new mDoExt_brkAnm();
        if (i_this->mpBrkAtack == NULL) {
            return FALSE;
        }
        if (i_this->mpBrkAtack->init(
                model->getModelData(),
                (J3DAnmTevRegKey*)dComIfG_getObjectRes("GM", dRes_INDEX_GM_BRK_ATACK_e),
                TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, FALSE
            ) == 0)
        {
            return FALSE;
        }

        i_this->mpBrkDamage = new mDoExt_brkAnm();
        if (i_this->mpBrkDamage == NULL) {
            return FALSE;
        }
        if (i_this->mpBrkDamage->init(
                model->getModelData(),
                (J3DAnmTevRegKey*)dComIfG_getObjectRes("GM", dRes_INDEX_GM_BRK_DAMAGE_e),
                TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, FALSE
            ) == 0)
        {
            return FALSE;
        }

        i_this->mpBrkDead = new mDoExt_brkAnm();
        if (i_this->mpBrkDead == NULL) {
            return FALSE;
        }
        if (i_this->mpBrkDead->init(
                model->getModelData(),
                (J3DAnmTevRegKey*)dComIfG_getObjectRes("GM", dRes_INDEX_GM_BRK_DEAD_e),
                TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, FALSE
            ) == 0)
        {
            return FALSE;
        }

        if (!i_this->mInvisibleModel.create(i_this->mpMorf->getModel())) {
            return FALSE;
        }
    }

    i_this->mpShadowModel = mDoExt_J3DModel__create(modelData, 0x20000, 0x11020203);
    if (i_this->mpShadowModel == NULL) {
        return FALSE;
    }
    return TRUE;
}

/* 00007EBC-0000842C       .text daGM_Create__FP10fopAc_ac_c */
static cPhs_State daGM_Create(fopAc_ac_c* a_this) {
    static dCcD_SrcSph weapon_co_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_UNK800,
            /* SrcObjAt  Atp     */ 1,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e | cCcD_AtSPrm_VsOther_e,
            /* SrcObjTg  Type    */ 0,
            /* SrcObjTg  SPrm    */ 0,
            /* SrcObjCo  SPrm    */ 0,
            /* SrcGObjAt Se      */ dCcG_SE_UNK2,
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
    static dCcD_SrcCyl body_co_cyl_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_SPIKE,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
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
    static dCcD_SrcCyl wing_co_cyl_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_SPIKE,
            /* SrcObjAt  Atp     */ 2,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e | cCcD_AtSPrm_VsOther_e,
            /* SrcObjTg  Type    */ 0,
            /* SrcObjTg  SPrm    */ 0,
            /* SrcObjCo  SPrm    */ 0,
            /* SrcGObjAt Se      */ dCcG_SE_METAL,
            /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
            /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK6,
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
    static dCcD_SrcCyl wind_co_cyl_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_WIND,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e | cCcD_AtSPrm_VsOther_e | cCcD_AtSPrm_NoTgHitInfSet_e,
            /* SrcObjTg  Type    */ 0,
            /* SrcObjTg  SPrm    */ 0,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_VsOther_e,
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
    static u8 fire_j[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    };
    static f32 fire_sc[] = {
        2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f,
    };

    gm_class* i_this = (gm_class*)a_this;
    fopAcM_ct(a_this, gm_class);

    cPhs_State phase_state = dComIfG_resLoad(&i_this->mPhs, "GM");
    if (phase_state == cPhs_COMPLEATE_e) {
        i_this->mWingBits = fopAcM_GetParam(a_this);
        i_this->m2CD = fopAcM_GetParam(a_this) >> 8;
        i_this->m2CE = fopAcM_GetParam(a_this) >> 16;
        i_this->mSwitchNo = fopAcM_GetParam(a_this) >> 24;
        if (i_this->mWingBits == 0xFF) {
            i_this->mWingBits = 0;
        }
        if (i_this->m2CD == 0xFF) {
            i_this->m2CD = 0;
        }
        if (i_this->m2CE == 0xFF) {
            i_this->m2CE = 0;
        }

        if (i_this->m2CD == 0) {
            i_this->mSwitchEnable = a_this->current.angle.z;
            a_this->current.angle.z = 0;
            a_this->shape_angle.z = 0;
            a_this->current.angle.x = 0;
            a_this->shape_angle.x = 0;
        }

        if (i_this->mSwitchNo != 0xFF) {
            if (dComIfGs_isSwitch(i_this->mSwitchNo, dComIfGp_roomControl_getStayNo())) {
                return cPhs_ERROR_e;
            }
        }

        s16 reg8s9 = REG8_S(9);
        if (reg8s9 != 0) {
            i_this->m2CE = reg8s9 - 1;
        }

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x7400)) {
            return cPhs_ERROR_e;
        }

        a_this->max_health = 12;
        a_this->health = 12;
        fopAcM_SetMtx(a_this, i_this->mpMorf->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(a_this, -200.0f, -150.0f, -200.0f, 200.0f, 250.0f, 150.0f);

        i_this->mAcch.Set(
            fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this),
            a_this, 1, &i_this->mAcchCir,
            fopAcM_GetSpeed_p(a_this), NULL, NULL
        );
        i_this->mStts.Init(0x64, 1, a_this);
        i_this->mCorrectionY = 0.0f;
        i_this->m434 = a_this->current.angle.y;
        i_this->mSpawnPos = a_this->current.pos;

        a_this->itemTableIdx = dComIfGp_CharTbl()->GetNameIndex("gmos", 0);
        if (i_this->m2CD == 0) {
            i_this->mBodyCyl.Set(body_co_cyl_src);
            i_this->mBodyCyl.SetStts(&i_this->mStts);
            i_this->mWeaponSph.Set(weapon_co_sph_src);
            i_this->mWeaponSph.SetStts(&i_this->mStts);
            i_this->mWingCyl.Set(wing_co_cyl_src);
            i_this->mWingCyl.SetStts(&i_this->mStts);
            i_this->mWindCyl.Set(wind_co_cyl_src);
            i_this->mWindCyl.SetStts(&i_this->mStts);
            i_this->mWeaponSph.OffAtSetBit();
            i_this->mWeaponSph.OffAtSetBit();
            i_this->mWingCyl.OffAtSetBit();
            i_this->mWingCyl.OffAtSetBit();

            a_this->attention_info.flags = fopAc_Attn_LOCKON_BATTLE_e;
            a_this->attention_info.distances[fopAc_Attn_TYPE_BATTLE_e] = 4;

            if (i_this->m2CE == 1) {
                i_this->initBt(120.0f + REG8_F(4), 100.0f + REG8_F(5));
                i_this->setBtAttackData(0.0f, 10.0f, 800.0f + REG8_F(6), 1);
                a_this->attention_info.distances[fopAc_Attn_TYPE_BATTLE_e] = 3;
                a_this->stealItemLeft = 1;
            } else {
                i_this->initBt(200.0f + REG8_F(4), 100.0f + REG8_F(5));
                i_this->setBtAttackData(0.0f, 10.0f, 880.0f + REG8_F(6), 1);
                a_this->stealItemLeft = 3;
                i_this->mWeaponSph.SetAtSpl(dCcG_At_Spl_UNK5);
            }

            i_this->setBtNowFrame(1000.0f);
            a_this->scale.setall(0.0f);
            i_this->m2D0 = 20;
            i_this->mAction = 200;

            i_this->mEnemyIce.mpActor = a_this;
            i_this->mEnemyIce.mWallRadius = 40.0f;
            i_this->mEnemyIce.mCylHeight = 40.0f;
            i_this->mEnemyIce.m1B0 = 1;
            i_this->mEnemyFire.mpMcaMorf = i_this->mpMorf;
            i_this->mEnemyFire.mpActor = a_this;
            for (int i = 0; i < 10; i++) {
                i_this->mEnemyFire.mFlameJntIdxs[i] = fire_j[i];
                i_this->mEnemyFire.mParticleScale[i] = fire_sc[i];
            }

            draw_SUB(i_this);

            if (i_this->m2CE == 0) {
                a_this->actor_status |= fopAcStts_UNK4000_e;
                a_this->actor_status |= fopAcStts_BOSS_e;
                a_this->actor_status &= ~fopAcStts_SHOWMAP_e;
            } else if (i_this->m2CE == 2) {
                a_this->itemTableIdx = dComIfGp_CharTbl()->GetNameIndex("gmos_f", 0);
                a_this->scale.setall(1.0f);
                i_this->m2D0 = 0;
                i_this->mAction = 0;
            } else if (i_this->m2CE == 1) {
                a_this->itemTableIdx = dComIfGp_CharTbl()->GetNameIndex("gmos_g", 0);
                i_this->mWallH = 60.0f;
                i_this->mWallR = 70.0f;
                a_this->scale.setall(1.0f);
                i_this->mWingBits = 15;
                a_this->max_health = 2;
                a_this->health = 2;
                i_this->m2D0 = 10;
                i_this->mAction = 60;
            }
        } else {
            a_this->attention_info.flags = 0;
            i_this->mWallR = 100.0f;
            if (i_this->mWingBits == 2 || i_this->mWingBits == 4) {
                a_this->shape_angle.y += (s16)0x8000;
            }
            i_this->m2D0 = 1;
            i_this->mAction = 10;
        }
    }

    return phase_state;
}

static actor_method_class l_daGM_Method = {
    (process_method_func)daGM_Create,
    (process_method_func)daGM_Delete,
    (process_method_func)daGM_Execute,
    (process_method_func)daGM_IsDelete,
    (process_method_func)daGM_Draw,
};

actor_process_profile_definition g_profile_GM = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_GM_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(gm_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_GM_e,
    /* Actor SubMtd */ &l_daGM_Method,
    /* Status       */ fopAcStts_SHOWMAP_e | fopAcStts_UNK40000_e | fopAcStts_UNK80000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
