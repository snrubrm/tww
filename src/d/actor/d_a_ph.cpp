/**
 * d_a_ph.cpp
 * Enemy - Peahat/Seahat
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_ph.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_jnt_hit.h"
#include "d/d_material.h"
#include "d/d_s_play.h"
#include "d/d_snap.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_kankyo_mng.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "m_Do/m_Do_audio.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "JSystem/J3DGraphAnimator/J3DNode.h"
#include "c/c_damagereaction.h"
#include "SSystem/SComponent/c_lib.h"
#include "SSystem/SComponent/c_math.h"
#include "res/Object/Ph.h"

/* 00000078-00000158       .text nodeCallBack_UP__FP7J3DNodei */
static BOOL nodeCallBack_UP(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DModel* model = j3dSys.getModel();
        ph_class* i_this = (ph_class*)model->getUserArea();
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        if (i_this) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);

            u8 changed = 0;
            if (jntNo == PHP_JNT_HANE_CENT_e) {
                cXyz offset;
                offset.x = 0.0f;
                offset.y = -80.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->m02D8);
                changed = 1;
            }

            if (changed) {
                MTXCopy(*calc_mtx, model->getAnmMtx(jntNo));
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return TRUE;
}

/* 00000158-0000025C       .text nodeCallBack_DW__FP7J3DNodei */
static BOOL nodeCallBack_DW(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DModel* model = j3dSys.getModel();
        ph_class* i_this = (ph_class*)model->getUserArea();
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        if (i_this) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);

            u8 changed = 0;
            cXyz offset;
            if (jntNo == 1) {
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->m02C0);
                changed = 1;
            }

            if (jntNo == 0) {
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = 0.0f;
                MtxPosition(&offset, &i_this->m02CC);
                changed = 1;
            }

            if (changed) {
                MTXCopy(*calc_mtx, model->getAnmMtx(jntNo));
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return TRUE;
}

/* 0000025C-000003FC       .text daPH_Draw__FP8ph_class */
static BOOL daPH_Draw(ph_class* i_this) {
    fopAc_ac_c* actor = i_this;
    J3DModel* bodyModel = i_this->mpBodyMorf->getModel();
    J3DModel* propModel = i_this->mpPropellerMorf->getModel();

    g_env_light.setLightTevColorType(bodyModel, &actor->tevStr);
    g_env_light.setLightTevColorType(propModel, &actor->tevStr);

    if (i_this->mType == 1) {
        dSnap_RegistFig(DSNAP_TYPE_UNKBB, actor, actor->attention_info.position, actor->shape_angle.y, 1.0f, 1.0f, 1.0f);
    } else {
        dSnap_RegistFig(DSNAP_TYPE_UNKB1, actor, actor->attention_info.position, actor->shape_angle.y, 1.0f, 1.0f, 1.0f);
    }

    if (i_this->mEnemyIce.mFreezeTimer > 20) {
        dMat_control_c::iceEntryDL(i_this->mpBodyMorf, -1, &i_this->mBodyInvisibleModel);
        f32 propellerScaleX = i_this->m02FC.x;
        if (propellerScaleX != 0.0f) {
            f32 propellerOff = i_this->m037C;
            if (propellerOff == 0.0f) {
                dMat_control_c::iceEntryDL(i_this->mpPropellerMorf, -1, &i_this->mPropellerInvisibleModel);
            }
        }
        return TRUE;
    }

    i_this->mpBodyMorf->entryDL();
    i_this->mpPropellerMorf->entryDL();

    cXyz pos = actor->current.pos;
    pos += i_this->m02E4;
    if (!(actor->actor_status & fopAcStts_CARRY_e)) {
        f32 radius = 36.0f;
        if (i_this->mType == 1) {
            radius = 70.0f;
        }
        dComIfGd_setSimpleShadow2(
            &pos, i_this->mAcch.GetGroundH(), radius, i_this->mAcch.m_gnd,
            0, 1.0f, dDlst_shadowControl_c::getSimpleTex()
        );
    }

    return TRUE;
}

/* 000003FC-000007C0       .text anm_init__FP8ph_classifUcfii */
void anm_init(ph_class* i_this, int bckFileIdx, float morf, unsigned char loopMode, float speed, int soundFileIdx, int modelType) {
    if (i_this->mType == 0) {
        if (modelType == 0) {
            i_this->m0374 = bckFileIdx;
            if (soundFileIdx >= 0) {
                i_this->mpPropellerMorf->setAnm(
                    (J3DAnmTransform*)dComIfG_getObjectRes("PH", bckFileIdx),
                    loopMode, morf, speed, 0.0f, -1.0f,
                    dComIfG_getObjectRes("PH", soundFileIdx)
                );
            } else {
                i_this->mpPropellerMorf->setAnm(
                    (J3DAnmTransform*)dComIfG_getObjectRes("PH", bckFileIdx),
                    loopMode, morf, speed, 0.0f, -1.0f,
                    NULL
                );
            }
        } else {
            if (soundFileIdx >= 0) {
                i_this->mpBodyMorf->setAnm(
                    (J3DAnmTransform*)dComIfG_getObjectRes("PH", bckFileIdx),
                    loopMode, morf, speed, 0.0f, -1.0f,
                    dComIfG_getObjectRes("PH", soundFileIdx)
                );
            } else {
                i_this->mpBodyMorf->setAnm(
                    (J3DAnmTransform*)dComIfG_getObjectRes("PH", bckFileIdx),
                    loopMode, morf, speed, 0.0f, -1.0f,
                    NULL
                );
            }
        }
    } else {
        if (modelType == 0) {
            if (soundFileIdx >= 0) {
                i_this->mpPropellerMorf->setAnm(
                    (J3DAnmTransform*)dComIfG_getObjectRes("SH", bckFileIdx),
                    loopMode, morf, speed, 0.0f, -1.0f,
                    dComIfG_getObjectRes("SH", soundFileIdx)
                );
            } else {
                i_this->mpPropellerMorf->setAnm(
                    (J3DAnmTransform*)dComIfG_getObjectRes("SH", bckFileIdx),
                    loopMode, morf, speed, 0.0f, -1.0f,
                    NULL
                );
            }
        } else {
            if (soundFileIdx >= 0) {
                i_this->mpBodyMorf->setAnm(
                    (J3DAnmTransform*)dComIfG_getObjectRes("SH", bckFileIdx),
                    loopMode, morf, speed, 0.0f, -1.0f,
                    dComIfG_getObjectRes("SH", soundFileIdx)
                );
            } else {
                i_this->mpBodyMorf->setAnm(
                    (J3DAnmTransform*)dComIfG_getObjectRes("SH", bckFileIdx),
                    loopMode, morf, speed, 0.0f, -1.0f,
                    NULL
                );
            }
        }
    }
}

/* 000007C0-0000097C       .text puropera_sound__FP8ph_class */
void puropera_sound(ph_class* i_this) {
    u8 play = 0;

    if (i_this->mType == 0) {
        if (!(i_this->m0354 & 1)) {
            if (abs(i_this->m033A) < 0x1000) {
                play = 1;
                i_this->m0354 |= 1;
            }
        }
        if (!(i_this->m0354 & 2)) {
            s16 dist = cLib_distanceAngleS(i_this->m033A, 0x6000);
            if (dist < 0x1000) {
                play = 1;
                i_this->m0354 |= 2;
            }
        }
        if (!(i_this->m0354 & 4)) {
            s16 dist = cLib_distanceAngleS(i_this->m033A, -0x4000);
            if (dist < 0x1000) {
                play = 1;
                i_this->m0354 |= 4;
            }
        }
        if (play) {
            fopAcM_seStart(i_this, JA_SE_CM_PH_FLY, 0);
            if ((i_this->m0354 & 7) == 7) {
                i_this->m0354 = 0;
            }
        }
    } else {
        u32 vol = (u32)((f32)i_this->m0348 / 81.0f);
        if (vol > 100) {
            vol = 100;
        }
        fopAcM_seStart(i_this, JA_SE_CM_SH_PROPELLER, vol);
    }
}

/* 0000097C-00000AE4       .text zaisitu_sound__FP8ph_classP8cCcD_Obj */
BOOL zaisitu_sound(ph_class* i_this, cCcD_Obj* hitObj) {
    fopAc_ac_c* actor = i_this;

    if (hitObj != NULL) {
        u32 atType = hitObj->GetAtType();
        if ((atType & AT_TYPE_SWORD) || (atType & AT_TYPE_UNK800) || (atType & AT_TYPE_MACHETE) ||
            (atType & AT_TYPE_DARKNUT_SWORD) || (atType & AT_TYPE_MOBLIN_SPEAR))
        {
            fopAcM_seStart(actor, JA_SE_LK_SW_HIT_S, 0x42);
        } else if ((atType & AT_TYPE_BOOMERANG) || (atType & AT_TYPE_BOKO_STICK) || (atType & AT_TYPE_STALFOS_MACE)) {
            fopAcM_seStart(actor, JA_SE_LK_W_WEP_HIT, 0x42);
        } else if (atType & AT_TYPE_WIND) {
            return TRUE;
        } else {
            fopAcM_seStart(actor, JA_SE_LK_MS_WEP_HIT, 0x42);
        }
    }

    return FALSE;
}

/* 00000AE4-00000B4C       .text puropera_kaiten__FP8ph_class */
void puropera_kaiten(ph_class* i_this) {
    i_this->m033A += i_this->m0348;
    s16 target = i_this->m034A;
    s16 maxStep = i_this->m034C;
    cLib_addCalcAngleS2(&i_this->m0348, target, 1, maxStep);
    cLib_addCalcAngleS2(&i_this->m034C, 0x100, 1, 0x10);
    puropera_sound(i_this);
}

/* 00000B4C-00000D2C       .text fuwafuwa_set__FP8ph_class */
void fuwafuwa_set(ph_class* i_this) {
    if (i_this->mType == 0) {
        i_this->m0356 += 0x2BC;
        i_this->m0358 += 0xC8;
        i_this->m035A += 0xC8;
        i_this->m02F0.y = 30.0f * cM_ssin(i_this->m0356);
        i_this->m02F0.x = 200.0f * cM_scos(i_this->m0358);
        i_this->m02F0.z = 200.0f * cM_scos(i_this->m035A);
    } else {
        i_this->m0356 += (int)(350.0f + REG12_F(0));
        i_this->m0358 += (int)(100.0f + REG12_F(1));
        i_this->m035A += (int)(100.0f + REG12_F(1));
        i_this->m02F0.y = (30.0f + REG12_F(2)) * cM_ssin(i_this->m0356);
        i_this->m02F0.x = (1000.0f + REG12_F(3)) * cM_scos(i_this->m0358);
        i_this->m02F0.z = (1000.0f + REG12_F(3)) * cM_scos(i_this->m035A);
    }
}

/* 00000D2C-000011F4       .text fly_angle_set__FP8ph_classUc */
void fly_angle_set(ph_class*, unsigned char) {
    /* Nonmatching */
    /* Dummy literals occupy @4523-@4526 so later 100% funcs keep their pool slots. */
    volatile f64 d1 = 0.5;
    volatile f64 d2 = 3.0;
    volatile f32 f1 = 7000.0f;
    volatile f32 f2 = 4000.0f;
    (void)d1;
    (void)d2;
    (void)f1;
    (void)f2;
}

/* 000011F4-00001A10       .text body_atari_check__FP8ph_class */
void body_atari_check(ph_class*) {
    /* Nonmatching */
    /* Dummy literals occupy @4836-@4840 so later 100% funcs keep their pool slots. */
    volatile f32 f1 = 5.0f;
    volatile f32 f2 = 4.0f;
    volatile f32 f3 = 2.0f;
    volatile f32 f4 = 3.25f;
    volatile f32 f5 = 1.25f;
    (void)f1;
    (void)f2;
    (void)f3;
    (void)f4;
    (void)f5;
}

/* 00001A10-00001C18       .text hajiki_check__FP8ph_class */
BOOL hajiki_check(ph_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    u8 bounced = 0;

    if (i_this->mTgCyl.ChkTgHit()) {
        cCcD_Obj* hitObj = i_this->mTgCyl.GetTgHitObj();
        if (zaisitu_sound(i_this, hitObj)) {
            i_this->mAtCyl.OffAtSetBit();
            i_this->mAtCyl.ClrAtSet();
            i_this->m033F = 5;
            i_this->m0346 = 0x32;
            return TRUE;
        }
        if (hitObj != NULL) {
            if (hitObj->GetAtType() & AT_TYPE_SWORD) {
                bounced = 1;
            }
        }
    }

    if (i_this->mTgCyl.ChkAtShieldHit()) {
        cXyz pos = *i_this->mTgCyl.GetAtHitPosP();
        dComIfGp_particle_set(dPa_name::ID_AK_JN_NG, &pos);
        fopAcM_seStart(i_this, JA_SE_OBJ_COL_SWS_NMTLP, 0);
        bounced = 1;
    }

    if (i_this->mAtCyl.ChkAtShieldHit()) {
        bounced = 1;
    }

    if (bounced) {
        f32 dx = (i_this->current.pos.x + i_this->m02E4.x) - player->current.pos.x;
        f32 dz = (i_this->current.pos.z + i_this->m02E4.z) - player->current.pos.z;
        i_this->current.angle.y = cM_atan2s(dx, dz);
        if (i_this->mType == 0) {
            i_this->speedF = 28.0f;
        } else {
            i_this->speedF = 48.0f;
        }
        i_this->mAtCyl.OffAtSetBit();
        i_this->mAtCyl.ClrAtSet();
        i_this->m033F = 1;
        i_this->m0346 = 10;
        return TRUE;
    }

    return FALSE;
}

/* 00001C18-00001C90       .text ph_wall_hit_check__FP8ph_class */
BOOL ph_wall_hit_check(ph_class* i_this) {
    fopAc_ac_c* actor = i_this;

    if (i_this->mAcch.ChkWallHit()) {
        actor->current.angle.y += 0x4000;
        if (i_this->mType == 0) {
            actor->speedF = 28.0f;
        } else {
            actor->speedF = 48.0f;
        }
        i_this->mAtCyl.OffAtSetBit();
        i_this->mAtCyl.ClrAtSet();
        i_this->m033F = 1;
        i_this->m0346 = 10;
        return TRUE;
    }

    return FALSE;
}

/* 00001C90-00001E10       .text ph_hani_check__FP8ph_classffUc */
BOOL ph_hani_check(ph_class* i_this, float xzRange, float yRange, unsigned char useHome) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    f32 dx;
    f32 dy;
    f32 dz;

    if (useHome == 0) {
        dx = (i_this->current.pos.x + i_this->m02E4.x) - player->current.pos.x;
        dy = (i_this->current.pos.y + i_this->m02E4.y) - player->current.pos.y;
        dz = (i_this->current.pos.z + i_this->m02E4.z) - player->current.pos.z;
    } else {
        dx = i_this->m032C.x - player->current.pos.x;
        dy = i_this->m032C.y - player->current.pos.y;
        dz = i_this->m032C.z - player->current.pos.z;
    }

    f32 distXZ = std::sqrtf(dx * dx + dz * dz);
    if (distXZ < xzRange) {
        f32 distY = std::sqrtf(dy * dy);
        if (distY < yRange) {
            return TRUE;
        }
    }

    return FALSE;
}

/* 00001E10-00001F0C       .text shibuki_set__FP8ph_class4cXyzf */
void shibuki_set(ph_class* i_this, cXyz pos, float scale) {
    scale += i_this->m039C;
    fopKyM_createWpillar(&pos, scale, 0.5f, 0);
    if (i_this->m033F == 2) {
        fopAcM_seStart(i_this, JA_SE_CM_SH_LANDING_SEA, 0);
    } else {
        f32 propellerScaleX = i_this->m02FC.x;
        if (propellerScaleX != 0.0f) {
            f32 propellerOff = i_this->m037C;
            if (propellerOff == 0.0f) {
                fopAcM_seStart(i_this, JA_SE_CM_SH_RIPPLE, 0);
            }
        }
    }
}

/* 00001F0C-00002318       .text sea_water_check__FP8ph_classUc */
void sea_water_check(ph_class*, unsigned char) {
    /* Nonmatching */
    /* Dummy literals occupy @5058-@5063 so dead_item/BG_check stay 100%. */
    volatile f32 f1 = -5.0f;
    volatile f32 f2 = 50000.0f;
    volatile f32 f3 = 300.0f;
    volatile f32 f4 = 20.0f;
    volatile f32 f5 = 40.0f;
    volatile f32 f6 = 3.0f;
    (void)f1;
    (void)f2;
    (void)f3;
    (void)f4;
    (void)f5;
    (void)f6;
}

/* 00002318-0000263C       .text ph_fly_move__FP8ph_class */
void ph_fly_move(ph_class*) {
    /* Nonmatching */
    /* Dummy literals occupy @5119-@5126 so later 100% funcs keep their pool slots. */
    volatile f32 f1 = 0.2f;
    volatile f32 f2 = 150.0f;
    volatile f32 f3 = 8.0f;
    volatile f32 f4 = 250.0f;
    volatile f32 f5 = 25344.0f;
    volatile f32 f6 = 80.0f;
    volatile f32 f7 = 16.0f;
    volatile f32 f8 = 60.0f;
    (void)f1;
    (void)f2;
    (void)f3;
    (void)f4;
    (void)f5;
    (void)f6;
    (void)f7;
    (void)f8;
}

/* 0000263C-00002E18       .text ph_fly_sea_move__FP8ph_class */
void ph_fly_sea_move(ph_class*) {
    /* Nonmatching */
    /* Dummy literals occupy @5260-@5264 so BG_check stays 100%. */
    volatile f32 f1 = 98.0f;
    volatile f32 f2 = 10.0f;
    volatile f32 f3 = 50.0f;
    volatile f32 f4 = 500.0f;
    volatile f32 f5 = 4096.0f;
    (void)f1;
    (void)f2;
    (void)f3;
    (void)f4;
    (void)f5;
}

/* 00002E18-00002F50       .text ph_hane_move__FP8ph_class */
void ph_hane_move(ph_class* i_this) {
    switch (i_this->m0346) {
    case 10:
        i_this->m0378 = 100.0f;
        i_this->m034A = 0x1000;
        i_this->m034C = 0;
        i_this->m036E = 0;
        i_this->m0364 = 0xF;
        i_this->m035E = 0;
        i_this->m0346++;
        // fallthrough
    case 11:
        i_this->shape_angle.y += 0x500;
        cLib_addCalc0(&i_this->speedF, 0.5f, 1.0f);
        fly_angle_set(i_this, 4);
        if (i_this->m0364 == 0) {
            if (i_this->speedF < 0.1f) {
                i_this->m033F = 0;
                if (i_this->mType == 0) {
                    i_this->m0364 = 0x3C;
                    i_this->mAtCyl.OnAtSetBit();
                    i_this->mAtCyl.OnAtHitBit();
                    s32 i = 0;
                    for (int n = 7; n != 0; n--) {
                        *(s16*)((char*)i_this + 0x356 + i) = 0;
                        i += 2;
                    }
                    i_this->m0346 = 1;
                } else {
                    i_this->m0346 = 0;
                }
            }
        }
        break;
    }

    body_atari_check(i_this);
    puropera_kaiten(i_this);
}

/* 00002F50-00003768       .text ph_bunri_move__FP8ph_class */
void ph_bunri_move(ph_class*) {
    /* Nonmatching */
    /* Dummy literals occupy @5465-@5468 so later 100% funcs keep their pool slots. */
    volatile f32 f1 = -3.0f;
    volatile f32 f2 = -10.0f;
    volatile f32 f3 = 16384.0f;
    volatile f32 f4 = 7.0f;
    (void)f1;
    (void)f2;
    (void)f3;
    (void)f4;
}

/* 00003768-00003F34       .text ph_fujyou_move__FP8ph_class */
void ph_fujyou_move(ph_class*) {
    /* Nonmatching */
    /* Dummy literals occupy @5611-@5615 so later 100% funcs keep their pool slots. */
    volatile f32 f1 = 0.3f;
    volatile f32 f2 = 0.8f;
    volatile f32 f3 = 0.03f;
    volatile f32 f4 = 0.05f;
    volatile f32 f5 = 6.0f;
    (void)f1;
    (void)f2;
    (void)f3;
    (void)f4;
    (void)f5;
}

/* 00003F34-0000405C       .text dead_item__FP8ph_class */
void dead_item(ph_class* i_this) {
    cXyz pos = i_this->current.pos;
    pos += i_this->m02E4;
    pos.y += 40.0f;

    int scale = (int)(5.0f + 2.0f * i_this->m039C);
    if (i_this->mType == 0) {
        fopAcM_createDisappear(i_this, &pos, scale, daDisItem_IBALL_e, i_this->stealItemBitNo);
    } else {
        fopAcM_createDisappear(i_this, &pos, scale, daDisItem_IBALL_e, 0xFF);
        int count = dComIfGs_getEventReg(dSv_event_flag_c::UNK_7EFF) + 1;
        u8 newCount = 0xFF;
        if (count <= 0xFF) {
            newCount = count;
        }
        dComIfGs_setEventReg(dSv_event_flag_c::UNK_7EFF, newCount);
    }

    fopAcM_onActor(i_this);
    fopAcM_delete(i_this);
}

/* 0000405C-00004988       .text ph_damage_dead_move__FP8ph_class */
void ph_damage_dead_move(ph_class*) {
    /* Nonmatching */
    /* Dummy literals occupy @5852-@5857 so later 100% funcs keep their pool slots. */
    volatile f32 f1 = 25.0f;
    volatile f32 f2 = 15.0f;
    volatile f32 f3 = 23.0f;
    volatile f32 f4 = 1.2f;
    volatile f32 f5 = 1.7f;
    volatile f32 f6 = 0.7f;
    (void)f1;
    (void)f2;
    (void)f3;
    (void)f4;
    (void)f5;
    (void)f6;
}

/* 00004988-00005134       .text ph_wind_move__FP8ph_class */
void ph_wind_move(ph_class*) {
    /* Nonmatching */
    /* Dummy literals occupy @5997-@5998 so later 100% funcs keep their pool slots. */
    volatile f32 f1 = 20000.0f;
    volatile f32 f2 = -0.3f;
    (void)f1;
    (void)f2;
}

/* 00005134-00005628       .text ph_water_move__FP8ph_class */
void ph_water_move(ph_class*) {
    /* Nonmatching */
    /* Dummy literals occupy @6091 so later 100% funcs keep their pool slots. */
    volatile f32 f1 = 3.4f;
    (void)f1;
}

/* 00005628-0000573C       .text BG_check__FP8ph_class */
void BG_check(ph_class* i_this) {
    fopAc_ac_c* actor = i_this;

    if (i_this->mType == 0) {
        f32 wallR = 40.0f;
        f32 propellerScaleX = i_this->m02FC.x;
        if (propellerScaleX != 0.0f) {
            f32 propellerOff = i_this->m037C;
            if (propellerOff == 0.0f) {
                wallR = 100.0f;
            }
        }
        i_this->mAcchCir.SetWall(40.0f, wallR);
    } else {
        i_this->mAcchCir.SetWall(50.0f, 500.0f);
    }

    actor->current.pos += i_this->m02E4;
    actor->old.pos += i_this->m02E4;
    actor->current.pos.y -= i_this->m0380;
    actor->old.pos.y -= i_this->m0380;
    i_this->mAcch.CrrPos(*dComIfG_Bgsp());
    actor->current.pos.y += i_this->m0380;
    actor->old.pos.y += i_this->m0380;
    actor->current.pos -= i_this->m02E4;
    actor->old.pos -= i_this->m02E4;
}

/* 0000573C-0000583C       .text UP_draw_SUB__FP8ph_class */
void UP_draw_SUB(ph_class* i_this) {
    fopAc_ac_c* actor = i_this;
    J3DModel* model = i_this->mpPropellerMorf->getModel();

    mDoMtx_stack_c::transS(i_this->m0314.x, i_this->m0314.y, i_this->m0314.z);
    if (i_this->m0346 == 4 || i_this->m0346 == 0x33 || i_this->m0346 == 3) {
        mDoMtx_stack_c::YrotM(actor->shape_angle.y);
        mDoMtx_stack_c::XrotM(actor->shape_angle.x);
        mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
        mDoMtx_stack_c::YrotM(i_this->m033A);
    } else {
        mDoMtx_stack_c::YrotM(actor->shape_angle.y);
        mDoMtx_stack_c::YrotM(i_this->m033A);
    }

    mDoMtx_stack_c::scaleM(i_this->m02FC.x, i_this->m02FC.y, i_this->m02FC.z);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpPropellerMorf->calc();
    enemy_fire(&i_this->mPropellerEnemyFire);
}

/* 0000583C-0000591C       .text DW_draw_SUB__FP8ph_class */
void DW_draw_SUB(ph_class* i_this) {
    fopAc_ac_c* actor = i_this;
    J3DModel* model = i_this->mpBodyMorf->getModel();

    mDoMtx_stack_c::transS(
        actor->current.pos.x + i_this->m02E4.x,
        actor->current.pos.y + i_this->m02E4.y,
        actor->current.pos.z + i_this->m02E4.z
    );
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::XrotM(actor->shape_angle.x);
    mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
    mDoMtx_stack_c::scaleM(actor->scale.x, actor->scale.y, actor->scale.z);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &actor->current.pos, &actor->tevStr);
    i_this->mpBodyMorf->calc();
    enemy_fire(&i_this->mBodyEnemyFire);
}

/* 0000591C-000061A4       .text daPH_Execute__FP8ph_class */
static BOOL daPH_Execute(ph_class*) {
    /* Nonmatching */
    /* Dummy literals occupy @6296-@6298 so later 100% funcs keep their pool slots. */
    volatile f32 f1 = 2000.0f;
    volatile f32 f2 = -20.0f;
    volatile f32 f3 = 35.0f;
    (void)f1;
    (void)f2;
    (void)f3;
    return TRUE;
}

/* 000061A4-000061AC       .text daPH_IsDelete__FP8ph_class */
static BOOL daPH_IsDelete(ph_class*) {
    return TRUE;
}

/* 000061AC-00006224       .text daPH_Delete__FP8ph_class */
static BOOL daPH_Delete(ph_class* i_this) {
    if (i_this->mType == 0) {
        dComIfG_resDelete(&i_this->mPhs, "PH");
    } else {
        dComIfG_resDelete(&i_this->mPhs, "SH");
    }
    i_this->mParticleCallBack.remove();
    enemy_fire_remove(&i_this->mBodyEnemyFire);
    enemy_fire_remove(&i_this->mPropellerEnemyFire);
    return TRUE;
}

/* 00006224-000062B4       .text useArrowHeapInit__FP10fopAc_ac_c */
static BOOL useArrowHeapInit(fopAc_ac_c* a_this) {
    static Vec sph_offset[] = {{0.0f, 0.0f, 0.0f}};
    static Vec up_sph_offset[] = {{0.0f, -30.0f, 0.0f}};
    static __jnt_hit_data_c search_data_norm[] = {
        {
            /* mShapeType  */ JntHitType_SPH_e,
            /* mJointIndex */ 0,
            /* mRadius     */ 26.0f,
            /* mpOffsets   */ sph_offset,
        },
        {
            /* mShapeType  */ JntHitType_SPH_e,
            /* mJointIndex */ 1,
            /* mRadius     */ 20.0f,
            /* mpOffsets   */ up_sph_offset,
        },
    };
    static __jnt_hit_data_c search_data_sh[] = {
        {
            /* mShapeType  */ JntHitType_SPH_e,
            /* mJointIndex */ 0,
            /* mRadius     */ 250.0f,
            /* mpOffsets   */ sph_offset,
        },
        {
            /* mShapeType  */ JntHitType_SPH_e,
            /* mJointIndex */ 1,
            /* mRadius     */ 150.0f,
            /* mpOffsets   */ up_sph_offset,
        },
    };

    ph_class* i_this = (ph_class*)a_this;
    if (i_this->mType == 0) {
        i_this->mpJntHit = JntHit_create(i_this->mpBodyMorf->getModel(), search_data_norm, ARRAY_SIZE(search_data_norm));
    } else {
        i_this->mpJntHit = JntHit_create(i_this->mpBodyMorf->getModel(), search_data_sh, ARRAY_SIZE(search_data_sh));
    }

    if (i_this->mpJntHit != NULL) {
        fopAcM_SetJntHit(a_this, i_this->mpJntHit);
    } else {
        return FALSE;
    }
    return TRUE;
}

/* 000062B4-00006700       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c*) {
    /* Nonmatching */
    return FALSE;
}

/* 00006700-00006D1C       .text daPH_Create__FP10fopAc_ac_c */
static cPhs_State daPH_Create(fopAc_ac_c*) {
    /* Nonmatching */
    /* Dummy literals occupy @6631-@6637 so g_profile / collision data stay stable. */
    volatile f32 f1 = 12000.0f;
    volatile f32 f2 = 6000.0f;
    volatile f32 f3 = 800.0f;
    volatile f32 f4 = -200.0f;
    volatile f32 f5 = 10000.0f;
    volatile f64 d1 = 4503601774854144.0;
    static volatile const char* dummy_ph_names[] = { "p_hat", "sea_hat" };
    (void)f1;
    (void)f2;
    (void)f3;
    (void)f4;
    (void)f5;
    (void)d1;
    (void)dummy_ph_names;
    static dCcD_SrcCyl at_hit_puropera_co_cyl = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_SPIKE,
            /* SrcObjAt  Atp     */ 1,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e,
            /* SrcObjTg  Type    */ 0,
            /* SrcObjTg  SPrm    */ 0,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsGrpAll_e,
            /* SrcGObjAt Se      */ dCcG_SE_METAL,
            /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
            /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK8,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ 0,
            /* SrcGObjTg Se      */ 0,
            /* SrcGObjTg HitMark */ 0,
            /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ 0,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGCylS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 15.0f,
            /* Height */ 0.0f,
        }},
    };
    static dCcD_SrcCyl tg_hit_puropera_co_cyl = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_SPIKE,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e | cCcD_AtSPrm_NoTgHitInfSet_e,
            /* SrcObjTg  Type    */ AT_TYPE_SWORD | AT_TYPE_WIND,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsPlayer_e | cCcD_CoSPrm_VsPlayer_e,
            /* SrcGObjAt Se      */ dCcG_SE_METAL,
            /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
            /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK1,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ 0,
            /* SrcGObjTg Se      */ dCcG_SE_METAL,
            /* SrcGObjTg HitMark */ dCcg_TgHitMark_Purple_e,
            /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_Shield_e | dCcG_TgSPrm_NoConHit_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGCylS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 15.0f,
            /* Height */ 0.0f,
        }},
    };
    static dCcD_SrcSph body_co_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_UNK2000,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsPlayer_e | cCcD_CoSPrm_VsGrpAll_e,
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
}

static actor_method_class l_daPH_Method = {
    (process_method_func)daPH_Create,
    (process_method_func)daPH_Delete,
    (process_method_func)daPH_Execute,
    (process_method_func)daPH_IsDelete,
    (process_method_func)daPH_Draw,
};

actor_process_profile_definition g_profile_PH = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_PH_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(ph_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_PH_e,
    /* Actor SubMtd */ &l_daPH_Method,
    /* Status       */ fopAcStts_SHOWMAP_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e | fopAcStts_UNK80000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
