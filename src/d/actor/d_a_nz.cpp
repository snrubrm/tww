/**
 * d_a_nz.cpp
 * Enemy - Rat
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_nz.h"
#include "d/actor/d_a_bomb.h"
#include "d/actor/d_a_item.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "d/d_bg_s_gnd_chk.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_s_play.h"
#include "d/d_snap.h"
#include "d/d_material.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "res/Object/Nz.h"

enum Action { ACTION_NZ_MOVE = 0, ACTION_NZ2_MOVE = 1, ACTION_NZ3_MOVE = 2, ACTION_NZ4_MOVE = 3, ACTION_NZ5_MOVE = 4, ACTION_NZ6_MOVE = 5 };

enum Mode {
    MODE_NZ_MOVE_0 = 0,
    MODE_NZ_MOVE_1 = 1,
    MODE_NZ_MOVE_2 = 2,
    MODE_NZ_MOVE_3 = 3,
    MODE_NZ_MOVE_4 = 4,
    MODE_NZ_MOVE_5 = 5,
    MODE_NZ_MOVE_6 = 6,
    MODE_NZ_MOVE_7 = 7,
    MODE_NZ3_MOVE_10 = 10,
    MODE_NZ3_MOVE_11 = 11,
    MODE_NZ3_MOVE_12 = 12,
    MODE_NZ2_MOVE_20 = 20,
    MODE_NZ2_MOVE_21 = 21,
    MODE_NZ2_MOVE_22 = 22,
    MODE_NZ2_MOVE_23 = 23,
    MODE_NZ4_MOVE_30 = 30,
    MODE_NZ4_MOVE_31 = 31,
    MODE_NZ4_MOVE_32 = 32,
    MODE_NZ4_MOVE_33 = 33,
    MODE_NZ4_MOVE_34 = 34,
    MODE_NZ4_MOVE_35 = 35,
    MODE_NZ4_MOVE_36 = 36,
    MODE_NZ4_MOVE_37 = 37,
    MODE_NZ4_MOVE_38 = 38,
    MODE_NZ4_MOVE_39 = 39,
    MODE_NZ4_MOVE_40 = 40,
    MODE_NZ4_MOVE_41 = 41,
    MODE_NZ5_MOVE_50 = 50,
    MODE_NZ5_MOVE_51 = 51,
    MODE_NZ5_MOVE_52 = 52,
    MODE_NZ5_MOVE_53 = 53,
    MODE_NZ5_MOVE_54 = 54,
    MODE_NZ5_MOVE_55 = 55,
    MODE_NZ5_MOVE_56 = 56,
    MODE_NZ5_MOVE_57 = 57,
    MODE_NZ5_MOVE_60 = 60,
    MODE_NZ5_MOVE_61 = 61,
    MODE_NZ5_MOVE_62 = 62,
    MODE_NZ6_MOVE_70 = 70,
    MODE_NZ6_MOVE_71 = 71,
    MODE_NZ6_MOVE_72 = 72,
    MODE_NZ6_MOVE_73 = 73,
    MODE_NZ6_MOVE_74 = 74,
    MODE_NZ6_MOVE_75 = 75,
    MODE_NZ6_MOVE_76 = 76,
    MODE_NZ6_MOVE_77 = 77,
    MODE_NZ6_MOVE_78 = 78,
};

static daNZ_HIO_c l_HIO;
static fopAc_ac_c* check_info[100];
static s32 get_check_count;
static u16 nz_bmt_idx[] = {0x2A, 0x29};

/* 000000EC-00000138       .text __ct__10daNZ_HIO_cFv */
daNZ_HIO_c::daNZ_HIO_c() {
    m04 = 40.0f;
    m08 = 70.0f;
    m0C = 20.0f;
    m10 = 45.0f;
    m14 = -5.0f;
}

/* 00000180-00000228       .text nodeCallBack_tail__FP7J3DNodei */
static BOOL nodeCallBack_tail(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        s32 idx = 0;
        J3DModel* model = j3dSys.getModel();
        nz_class* i_this = (nz_class*)model->getUserArea();
        if (jntNo == 9) {
            idx = 1;
        }
        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);

            cXyz offset;
            offset.x = 0.0f;
            offset.y = 0.0f;
            offset.z = 0.0f;
            MtxPosition(&offset, &i_this->mTailRoot[idx]);
        }
    }
    return TRUE;
}

/* 00000264-00000874       .text tail_control__FP8nz_class */
void tail_control(nz_class* i_this) {
    f32 fVar1;
    s16 temp_r14;
    s16 temp_r15;
    s32 i;
    s16 temp_r16;
    s16 temp_r15_2;
    f32 fVar22;
    f32 dVar15;
    cXyz local_178;
    cXyz local_184;
    cXyz sp10;

    i_this->m2E2++;
    f32 x = i_this->mTailRoot[1].x - i_this->mTailRoot[0].x;
    f32 y = (i_this->mTailRoot[1].y - i_this->mTailRoot[0].y);
    f32 z = i_this->mTailRoot[1].z - i_this->mTailRoot[0].z;
    temp_r14 = (s16)cM_atan2s(x, z);
    temp_r15 = -cM_atan2s(y, std::sqrtf(SQUARE(x) + SQUARE(z)));
    local_178.x = 0.0f;
    local_178.y = 0.0f;
    local_178.z = 5.0f;
    cMtx_YrotS(*calc_mtx, temp_r14);
    cMtx_XrotM(*calc_mtx, temp_r15);
    MtxPosition(&local_178, &i_this->m574);
    cXyz* pcVar6 = &i_this->mTailPos[1];
    cXyz* pcVar9 = &i_this->mTailDir[1];
    cXyz* pcVar8 = i_this->mLineMat.getPos(0);
    dBgS_GndChk gndChk;
    for (i = 1; i < 10; i++, pcVar6++, pcVar9++) {
        Vec pos;
        pos = *pcVar6;
        pos.y += 30.0f;
        gndChk.SetPos(&pos);
        fVar22 = 5.0f + dComIfG_Bgsp()->GroundCross(&gndChk);
        fVar1 = (1.0f - ((i - 1) * (0.1f + REG0_F(4))));
        sp10.x = pcVar9->x + i_this->m574.x * fVar1;
        sp10.y = pcVar9->y + i_this->m574.y * fVar1;
        sp10.z = pcVar9->z + i_this->m574.z * fVar1;
        dVar15 = ((pcVar6->y + sp10.y) - 2.0f);
        if (dVar15 < fVar22) {
            dVar15 = fVar22;
        }
        y = (dVar15 - pcVar6[-1].y);
        x = sp10.x + (pcVar6->x - pcVar6[-1].x);
        z = sp10.z + (pcVar6->z - pcVar6[-1].z);
        temp_r15_2 = (s16)cM_atan2s(x, z);
        temp_r16 = -cM_atan2s(y, std::sqrtf(SQUARE(x) + SQUARE(z)));
        local_178.x = 0.0f;
        local_178.y = 0.0f;
        local_178.z = 20.0f;
        cMtx_YrotS(*calc_mtx, temp_r15_2);
        cMtx_XrotM(*calc_mtx, temp_r16);
        MtxPosition(&local_178, &local_184);
        *pcVar9 = *pcVar6;
        pcVar6->x = pcVar6[-1].x + local_184.x;
        pcVar6->y = pcVar6[-1].y + local_184.y;
        pcVar6->z = pcVar6[-1].z + local_184.z;
        pcVar9->x = (0.8f * (pcVar6->x - pcVar9->x));
        pcVar9->y = (0.8f * (pcVar6->y - pcVar9->y));
        pcVar9->z = (0.8f * (pcVar6->z - pcVar9->z));
    }
    pcVar6 = i_this->mTailPos;
    for (i = 0; i < 10; i++, pcVar6++, pcVar8++) {
        *pcVar8 = *pcVar6;
    }
}

/* 00000BF4-00000C88       .text tail_draw__FP8nz_class */
void tail_draw(nz_class* i_this) {
    GXColor color = {0xC8, 0xC8, 0xC8, 0xFF};
    i_this->mLineMat.update(10, 5.0f * i_this->scale.x, color, 6, &i_this->tevStr);
    dComIfGd_set3DlineMat(&i_this->mLineMat);
}

/* 00000C88-00000D64       .text nodeCallBack_head__FP7J3DNodei */
static BOOL nodeCallBack_head(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        nz_class* i_this = (nz_class*)model->getUserArea();
        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            cMtx_YrotM(*calc_mtx, i_this->mHeadRot.y);
            cMtx_XrotM(*calc_mtx, i_this->mHeadRot.x);
            cMtx_ZrotM(*calc_mtx, i_this->mHeadRot.z);
            model->setAnmMtx(jntNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 00000D64-00000E60       .text nodeCallBack_hand__FP7J3DNodei */
static BOOL nodeCallBack_hand(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        nz_class* i_this = (nz_class*)model->getUserArea();
        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);

            cXyz offset;
            offset.x = 0.0f;
            offset.y = 0.0f;
            offset.z = 0.0f;
            cXyz pos;
            MtxPosition(&offset, &pos);
            if (jntNo == 0x12) {
                i_this->mHandPos1 = pos;
            } else {
                i_this->mHandPos0 = pos;
            }

            model->setAnmMtx(jntNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 00000E60-00000F40       .text smoke_set__FP8nz_class */
void smoke_set(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;
    if (i_this->mSmokeCb.getEmitter() == NULL) {
        s8 roomNo = fopAcM_GetRoomNo(actor);
        dComIfGp_particle_setToon(
            dPa_name::ID_AK_JT_ELEMENTSMOKE00,
            &i_this->mSmokePos,
            &i_this->mSmokeRot,
            NULL,
            0xB9,
            &i_this->mSmokeCb,
            roomNo
        );
    }

    if (i_this->mSmokeCb.getEmitter() != NULL) {
        JGeometry::TVec3<f32> scale(1.5f, 1.5f, 1.5f);
        i_this->mSmokeCb.getEmitter()->setGlobalScale(scale);
        i_this->mSmokeCb.getEmitter()->setSpread(0.5f);
        i_this->mSmokeCb.getEmitter()->setDirectionalSpeed(15.0f);
        i_this->mSmokeCb.getEmitter()->setMaxFrame(4);
        i_this->mSmokeCb.getEmitter()->setRate(2.0f);
    }
}

/* 00000F40-00001328       .text rakka_line_check__FP8nz_class */
BOOL rakka_line_check(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;
    cXyz local_a4;
    cXyz local_b0;
    dBgS_LinChk linChk;

    cMtx_XrotS(*calc_mtx, 0);
    local_a4.x = 0.0f;
    local_a4.y = REG8_F(11) + -32.0f;
    local_a4.z = 0.0f;
    MtxPosition(&local_a4, &local_b0);
    local_a4 = actor->current.pos;
    local_b0 += actor->current.pos;
    i_this->m3C8[7] = local_b0;
    i_this->m368[7] = actor->current.pos;
    linChk.Set(&local_a4, &local_b0, actor);
    if (dComIfG_Bgsp()->LineCross(&linChk)) {
        i_this->mSmokePos = linChk.GetCross();
        csXyz local_b8(0, 0, 0);
        i_this->mSmokeRot = local_b8;
        smoke_set(i_this);
        return TRUE;
    }
    return FALSE;
}

const u32 unused_4556[] = {
    0xFF000040,
};

/* 00001578-0000178C       .text daNZ_Draw__FP8nz_class */
static BOOL daNZ_Draw(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;

    J3DModel* model = i_this->mpMorf->getModel();
    J3DModelData* modelData = model->getModelData();
    if (i_this->mEnemyIce.mFreezeTimer > 20) {
        dMat_control_c::iceEntryDL(i_this->mpMorf, -1, &i_this->mInvModel);
        return TRUE;
    }
    J3DJoint* joint = modelData->getJointNodePointer(NZ_NZ_JNT_KOSI_e);
    J3DShape* shape00 = modelData->getMaterialNodePointer(0x00)->getShape();
    J3DShape* shape01 = modelData->getMaterialNodePointer(0x01)->getShape();
    J3DShape* shape02 = modelData->getMaterialNodePointer(0x02)->getShape();
    shape00->hide();
    modelData->setMaterialTable(i_this->mpBmt, J3DMatCopyFlag_All);
    g_env_light.setLightTevColorType(model, &actor->tevStr);
    i_this->mpMorf->entryDL();
    dComIfGd_setListMaskOff();
    shape00->show();
    model->getMatPacket(0x00)->unlock();
    shape01->hide();
    shape02->hide();
    joint->entryIn();
    shape01->show();
    shape02->show();
    dComIfGd_setList();
    tail_draw(i_this);
    if (!fopAcM_checkCarryNow(actor)) {
        dComIfGd_setSimpleShadow2(&i_this->m304, i_this->mAcch.GetGroundH(), 20.0f, i_this->mAcch.m_gnd, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }
    if (REG8_S(1) != 0) {
        for (s32 i = 0; i < 16; i++) {
            i++;
        }
    }
    if (REG8_S(2) != 0) {
        cXyz sp8 = actor->current.pos;
        sp8.y -= REG8_F(10) + 200.0f;
    }
    dSnap_RegistFig(DSNAP_TYPE_UNKAD, actor, 1.0f, 1.0f, 1.0f);
    return TRUE;
}

/* 0000178C-00001888       .text item_poi__FP8nz_class */
static void item_poi(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;
    daBomb_c* bomb;
    daItem_c* item;

    bomb = (daBomb_c*)fopAcM_SearchByID(i_this->mHeldID);
    if (bomb != NULL) {
        if (fopAcM_GetName(bomb) == fpcNm_BOMB_e) {
            bomb->current.angle.y = i_this->m580;
            bomb->speedF = 20.0f;
            bomb->speed.y = 45.0f;
            bomb->gravity = -5.0f;
            bomb->setBombOnCoSet();
            if (i_this->m2B5 != 0) {
                i_this->m2E4 = 0;
                bomb->shape_angle.z = i_this->m2E4;
                bomb->setBombFire_ON();
            }
        } else {
            item = (daItem_c*)bomb;
            item->speedF = 20.0f;
            item->speed.y = 20.0f;
            item->gravity = -3.0f;
            item->current.angle.y = actor->current.angle.y;
            if (!item->checkLock()) {
                item->endControl();
                item->releaseLock();
            }
        }
    }
    i_this->mHeldID = fpcM_ERROR_PROCESS_ID_e;
    i_this->m42B = 0;
}

/* 00001888-00001F18       .text naraku_water_check__FP8nz_class */
void naraku_water_check(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;
    cXyz local_b8;
    cXyz local_c4;
    cXyz local_d0;

    if (((i_this->mAcch.GetGroundH() != -G_CM3D_F_INF) && (dComIfG_Bgsp()->ChkPolySafe(i_this->mAcch.m_gnd))) &&
        (dComIfG_Bgsp()->GetGroundCode(i_this->mAcch.m_gnd) == 4))
    {
        i_this->m2D2++;
        if (actor->current.pos.y < -500.0f || (i_this->m2D2 > 100)) {
            item_poi(i_this);
            fopAcM_delete(actor);
            return;
        }
    }
    if (REG8_S(3) == 0) {
        if ((!actor->speedF) && (!actor->speed.y)) {
            if (i_this->m2C3 != 0) {
                i_this->m2C3 = 0;
                i_this->mFollowCb.remove();
                local_b8.setall(1.0f);
                i_this->mSmokePos = i_this->m304;
                if (i_this->mRippleCb.getEmitter() == NULL) {
                    dComIfGp_particle_setShipTail(dPa_name::ID_AK_JN_HAMON00, &i_this->mSmokePos, NULL, &local_b8, 0xFF, &i_this->mRippleCb);
                }
            }
            return;
        }
        dBgS_ObjLinChk linChk;
        local_c4 = actor->current.pos;
        local_d0 = actor->current.pos;
        local_c4.y += REG8_F(10) + 100.0f;
        local_d0.y -= REG8_F(11) + 100.0f;
        linChk.OnWaterGrp();
        linChk.Set(&local_c4, &local_d0, actor);
        if ((!dComIfG_Bgsp()->LineCross(&linChk)) || (abs(actor->current.angle.x) > 0x500)) {
            if (i_this->m2C3 != 0) {
                i_this->mRippleCb.remove();
                i_this->mFollowCb.remove();
                i_this->m2C3 = 0;
            }
            return;
        }
        if (dComIfG_Bgsp()->ChkGrpInf(linChk, 0x100)) {
            if ((i_this->m2C3 == 0) && (i_this->mTimers[4] == 0)) {
                i_this->mTimers[4] = 5;
                i_this->m2C3 = 1;
                i_this->mRippleCb.remove();
                if (i_this->mFollowCb.getEmitter() == NULL) {
                    dComIfGp_particle_set(dPa_name::ID_AK_JN_RATSPLASH00, &i_this->m584, &i_this->m590, NULL, 0xFF, &i_this->mFollowCb);
                }
                if (i_this->mFollowCb.getEmitter() != NULL) {
                    GXColor amb, dif;
                    dKy_get_seacolor(&amb, &dif);
                    i_this->mFollowCb.getEmitter()->setGlobalPrmColor(amb.r, amb.g, amb.b);
                }
            }
            if (i_this->mFollowCb.getEmitter() != NULL) {
                i_this->m584 = actor->current.pos;
                i_this->m584.y += REG8_F(7) + 20.0f;
                i_this->mRippleCb.remove();
            }
        } else if (i_this->m2C3 != 0) {
            i_this->m2C3 = 0;
            i_this->mRippleCb.remove();
            i_this->mFollowCb.remove();
        }
    }
}

/* 0000206C-0000214C       .text s_a_d_sub__FPvPv */
static void* s_a_d_sub(void* i_actor, void*) {
    if (get_check_count < 100) {
        if (fopAcM_IsActor(i_actor)) {
            u8 found = 0;
            if (fopAcM_GetName(i_actor) == fpcNm_BOMB_e) {
                daBomb_c* bomb = (daBomb_c*)i_actor;
                if (bomb->getBombCheck_Flag() == 0) {
                    if (fopAcM_GetParam(bomb) == 1) {
                        if (!bomb->speedF) {
                            found = 1;
                        }
                    }
                }
            }
            if (fopAcM_GetName(i_actor) == fpcNm_ITEM_e) {
                if (((daItem_c*)i_actor)->checkLock()) {
                    found = 1;
                }
            }
            if (found) {
                check_info[get_check_count] = (fopAc_ac_c*)i_actor;
                get_check_count++;
            }
        }
    }
    return NULL;
}

/* 0000214C-000027FC       .text search_get_obj__FP8nz_class */
daBomb_c* search_get_obj(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;
    f32 fVar1;
    f32 fVar2;
    f32 fVar3;
    daBomb_c* bomb;
    f32 dVar11;
    cXyz local_d4;
    cXyz local_e0;
    dBgS_LinChk linChk;

    get_check_count = 0;
    fpcM_Search(s_a_d_sub, i_this);
    if (get_check_count != 0) {
        dVar11 = 50.0f;
        for (s32 i = 0; i < get_check_count;) {
            bomb = (daBomb_c*)check_info[i];
            fVar1 = bomb->current.pos.x - actor->current.pos.x;
            fVar2 = bomb->current.pos.y - actor->current.pos.y;
            fVar3 = bomb->current.pos.z - actor->current.pos.z;
            if (std::sqrtf(SQUARE(fVar1) + SQUARE(fVar3)) < dVar11) {
                if (std::sqrtf(SQUARE(fVar2)) < 100.0f) {
                    local_e0 = bomb->current.pos;
                    local_e0.y += 50.0f;
                    local_d4 = actor->current.pos;
                    local_d4.y += 50.0f;
                    linChk.Set(&local_d4, &local_e0, actor);
                    if (!dComIfG_Bgsp()->LineCross(&linChk)) {
                        if ((fopAcM_GetName(bomb) == fpcNm_BOMB_e) && (!bomb->getBombCheck_Flag())) {
                            bomb->gravity = 0.0f;
                            bomb->speedF = 0.0f;
                            bomb->speed.setall(0.0f);
                            bomb->setBombOffCoSet();
                            bomb->setBombCheck_Flag();
                            return bomb;
                        }
                        daItem_c* item = (daItem_c*)bomb;
                        if (item->checkLock()) {
                            item->setLock();
                            return bomb;
                        }
                    }
                }
            }
            i++;
            if (i == get_check_count) {
                i = 0;
                dVar11 += 50.0f;
                if (dVar11 > 2000.0f) {
                    return NULL;
                }
            }
        }
    }
    return NULL;
}

/* 000027FC-00002864       .text s_ana_sub__FPvPv */
static void* s_ana_sub(void* ac1, void* ac2) {
    nz_class* i_this = (nz_class*)ac2;
    if (fopAcM_IsActor(ac1) && fopAcM_GetName(ac1) == fpcNm_NZG_e) {
        i_this->mNzgID = fopAcM_GetID(ac1);
    }
    return NULL;
}

/* 00002864-00002990       .text anm_init__FP8nz_classifUcfi */
void anm_init(nz_class* i_this, int anmResIdx, float morf, unsigned char loopMode, float playSpeed, int soundResIdx) {
    i_this->mCurrBckIdx = anmResIdx;
    if (soundResIdx >= 0) {
        i_this->mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("NZ", anmResIdx), loopMode, morf, playSpeed, 0.0f, -1.0f, dComIfG_getObjectRes("NZ", soundResIdx));
    } else {
        i_this->mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("NZ", anmResIdx), loopMode, morf, playSpeed, 0.0f, -1.0f, NULL);
    }
}

/* 00002990-00002EC0       .text search_check__FP8nz_class */
s32 search_check(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;
    fopAc_ac_c* pfVar2;
    cXyz local_bc;
    cXyz cStack_c8;

    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    dBgS_LinChk linChk;
    if (fopAcM_searchActorDistanceXZ(actor, player) < 1200.0f) {
        pfVar2 = (fopAc_ac_c*)search_get_obj(i_this);
        if (pfVar2 == NULL) {
            s16 angle = fopAcM_searchPlayerAngleY(actor);
            angle += (s16)cM_rndFX(8000.0f);
            cMtx_YrotS(*calc_mtx, angle);
            local_bc.x = 0.0f;
            local_bc.y = 0.0f;
            local_bc.z = 100.0f;
            MtxPosition(&local_bc, &cStack_c8);
            cStack_c8 += actor->current.pos;
            linChk.Set(&actor->current.pos, &cStack_c8, actor);
            if (dComIfG_Bgsp()->LineCross(&linChk)) {
                return 0;
            }
            i_this->m580 = fopAcM_searchPlayerAngleY(actor);
            return 2;
        }
        i_this->m580 = fopAcM_searchActorAngleY(actor, pfVar2);
        i_this->mHeldID = fopAcM_GetID(pfVar2);
        return 1;
    }
    return 0;
}

/* 00002EC0-00002F3C       .text BG_check__FP8nz_class */
void BG_check(nz_class* i_this) {
    i_this->current.pos.y -= i_this->mYOffset;
    i_this->old.pos.y -= i_this->mYOffset;
    i_this->mAcch.CrrPos(*dComIfG_Bgsp());
    i_this->current.pos.y += i_this->mYOffset;
    i_this->old.pos.y += i_this->mYOffset;
}

/* 00002F3C-00003090       .text shock_damage_check__FP8nz_class */
static BOOL shock_damage_check(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz swordTopPos;

    if ((abs(actor->current.angle.x)) || (abs(actor->current.angle.z))) {
        return FALSE;
    }
    if (player->checkHammerQuake()) {
        swordTopPos = player->getSwordTopPos();
        swordTopPos.x -= actor->current.pos.x;
        swordTopPos.z -= actor->current.pos.z;
        if (std::sqrtf(SQUARE(swordTopPos.x) + SQUARE(swordTopPos.z)) < 1000.0f) {
            item_poi(i_this);
            i_this->m2BC = ACTION_NZ5_MOVE;
            i_this->m2BD = MODE_NZ5_MOVE_60;
            return TRUE;
        }
    }
    return FALSE;
}

/* 00003090-00003240       .text body_atari_check__FP8nz_class */
s32 body_atari_check(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;
    cCcD_Obj* hitObj;
    CcAtInfo atInfo;

    if (i_this->mCyl.ChkTgHit()) {
        hitObj = i_this->mCyl.GetTgHitObj();
        if (hitObj == NULL) {
            return 0;
        }
        i_this->m31C = *i_this->mCyl.GetTgHitPosP();
        item_poi(i_this);
        switch (hitObj->GetAtType()) {
        case AT_TYPE_FIRE:
        case AT_TYPE_FIRE_ARROW:
            i_this->mEnemyFire.mFireDuration = 100;
            break;
        case AT_TYPE_ICE_ARROW:
            i_this->mEnemyIce.mFreezeDuration = 200;
            i_this->m2C2 = 1;
            break;
        case AT_TYPE_LIGHT_ARROW:
            i_this->mEnemyIce.mLightShrinkTimer = 1;
            i_this->mEnemyIce.mParticleScale = 1.0f;
            i_this->mEnemyIce.mYOffset = 0.0f;
            actor->attention_info.flags = 0;
            fopAcM_monsSeStart(actor, JA_SE_CV_NZ_DAMAGE, 0);
        }
        if (i_this->m2C2 == 0) {
            atInfo.pParticlePos = NULL;
            atInfo.mpObj = i_this->mCyl.GetTgHitObj();
            atInfo.mpActor = cc_at_check(actor, &atInfo);
        }
        if (!hitObj->ChkAtType(AT_TYPE_HOOKSHOT)) {
            i_this->m2BC = ACTION_NZ5_MOVE;
            i_this->m2BD = MODE_NZ5_MOVE_50;
            return 1;
        }
    }
    if (shock_damage_check(i_this)) {
        return 2;
    }
    return 0;
}

/* 00003240-000042D8       .text nz_move__FP8nz_class */
void nz_move(nz_class*) {
    /* Nonmatching */
}

/* 000042D8-000044B8       .text money_drop__FP8nz_class */
static void money_drop(nz_class*) {
    static s32 rate_dt[] = {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 1, 0, 1, 0, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 0, 2, 2, 1, 2, 1, 2, 2, 1};

    static s32 ruppy_rate_dt[] = {1, 5, 20};

    cXyz local_78;
    cXyz local_60;
    cXyz cStack_6c;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    int iVar3;
    int iVar4;
    s32 dVar6;
    int i;
    int rupeenum = dComIfGs_getRupee();
    int iVar2 = 4;
    if (rupeenum < 50) {
        iVar2 = 0;
    } else if (rupeenum < 100) {
        iVar2 = 1;
    } else if (rupeenum < 300) {
        iVar2 = 2;
    } else if (rupeenum < 500) {
        iVar2 = 3;
    }
#if VERSION == VERSION_DEMO
    iVar2 *= 8 & 0xF8;
#endif
    dVar6 = (s32)(cM_rnd() * 3.0f) + 5;
    iVar4 = 0;
    i = 0;
    for (; iVar4 < dVar6; iVar4++) {
        iVar3 = rate_dt[(DEMO_SELECT(iVar2, (iVar2 * 8) & 0xF8) + (s32)(7.0f * cM_rnd()))];
        if (dComIfGs_getRupee() > ruppy_rate_dt[iVar3]) {
            cMtx_YrotS(*calc_mtx, cM_rndFX(32000.0f));
            local_60.x = 0.0f;
            local_60.y = 0.0f;
            local_60.z = 40.0f;
            MtxPosition(&local_60, &cStack_6c);
            local_78 = player->current.pos + cStack_6c;
            local_78.y += cM_rndF(60.0f) + 40.0f;
            fopAcM_createItem(&local_78, iVar3 + dItemNo_GREEN_RUPEE_e, -1, -1, fpcM_ERROR_PROCESS_ID_e, NULL, daItemAct_4_e, NULL);
            i += ruppy_rate_dt[iVar3];
        }
    }
    dComIfGp_setItemRupeeCount(-i);
}

/* 000044B8-0000482C       .text nz2_move__FP8nz_class */
void nz2_move(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;

    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    switch (i_this->m2BD) {
    case MODE_NZ2_MOVE_20:
        anm_init(i_this, dRes_INDEX_NZ_BCK_NZ_ATTACK_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f, dRes_INDEX_NZ_BAS_NZ_ATTACK_e);
        fopAcM_monsSeStart(actor, JA_SE_CV_NZ_ATTACK, 0);
        i_this->m2BD++;
        actor->current.angle.x = 0;
        actor->current.angle.z = 0;
        actor->speed.y = 35.0f;
        actor->speedF = 40.0f;
        actor->current.angle.y = fopAcM_searchPlayerAngleY(actor);
        i_this->mCyl.OnAtSPrmBit(cCcD_AtSPrm_Set_e);
        i_this->mCyl.OnAtHitBit();
        i_this->m2C1 = 0;
        i_this->m2CE = 0x1200;
        break;
    case MODE_NZ2_MOVE_21:
        actor->shape_angle.y = actor->current.angle.y;
        actor->shape_angle.x = actor->shape_angle.x + i_this->m2CE;
        actor->shape_angle.z = actor->current.angle.z;
        if ((i_this->m2C1 == 0) && (i_this->mCyl.ChkAtHit()) && (!i_this->mCyl.ChkAtShieldHit())) {
            fopAc_ac_c* hit_ac = i_this->mCyl.GetAtHitAc();
            if ((hit_ac != NULL) && (hit_ac == player)) {
                money_drop(i_this);
                i_this->m2C1 = 1;
            }
        }
        if (i_this->mAcch.ChkWallHit()) {
            actor->speedF = 0.0f;
        }
        if (!i_this->mAcch.ChkGroundHit()) {
            break;
        }
        actor->shape_angle = actor->current.angle;
        i_this->mCyl.ClrAtSet();
        i_this->m2BD++;
        // Fall-through
    case MODE_NZ2_MOVE_22:
        actor->speedF *= 0.5f;
        actor->speed.y = 20.0f;
        actor->shape_angle = actor->current.angle;
        anm_init(i_this, dRes_INDEX_NZ_BCK_NZ_ATTACK2_e, 0.0f, J3DFrameCtrl::EMode_NONE, 1.0f, -1);
        i_this->m2BD++;
        // Fall-through
    case MODE_NZ2_MOVE_23:
        actor->shape_angle = actor->current.angle;
        if (i_this->mAcch.ChkWallHit()) {
            actor->speedF = 0.0f;
        }
        cLib_addCalc0(&actor->speedF, 1.0f, 5.0f);
        if (i_this->mpMorf->isStop()) {
            actor->speed.y = 0.0f;
            i_this->mTimers[0] = cM_rndF(60.0f) + 60.0f;
            actor->current.angle.x = 0;
            actor->current.angle.z = 0;
            i_this->m2BC = ACTION_NZ_MOVE;
            i_this->m2BD = MODE_NZ_MOVE_6;
        }
        break;
    }
    if (body_atari_check(i_this) == 1) {
        actor->scale.setall(1.0f);
        i_this->mStts.SetWeight(100);
        i_this->m2C0 = 0;
        fopAcM_monsSeStart(actor, JA_SE_CV_NZ_DAMAGE, 0);
        actor->gravity = -5.0f;
        actor->shape_angle.x = 0;
        i_this->m2BD = MODE_NZ5_MOVE_52;
    }
}

/* 0000482C-000049E8       .text nz3_move__FP8nz_class */
void nz3_move(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;

    switch (i_this->m2BD) {
    case MODE_NZ3_MOVE_10:
        i_this->m2BD++;
        actor->current.angle.y += 0x8000;
        i_this->m580 = actor->current.angle.y;
        anm_init(i_this, dRes_INDEX_NZ_BCK_NZ_ATTACK_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f, dRes_INDEX_NZ_BAS_NZ_ATTACK_e);
        actor->current.angle.x = 0;
        actor->current.angle.z = 0;
        actor->gravity = -3.0f;
        i_this->mTimers[0] = 10;
        // Fall-through
    case MODE_NZ3_MOVE_11:
        actor->shape_angle.y = actor->current.angle.y;
        actor->shape_angle.x = actor->shape_angle.x + 0x2500;
        actor->shape_angle.z = actor->current.angle.z;
        if ((rakka_line_check(i_this)) || (i_this->mAcch.ChkGroundHit())) {
            anm_init(i_this, dRes_INDEX_NZ_BCK_NZ_ATTACK2_e, 0.0f, J3DFrameCtrl::EMode_NONE, 1.0f, -1);
            actor->shape_angle = actor->current.angle;
            actor->shape_angle.x = 0;
            actor->current.angle.x = 0;
            actor->shape_angle.z = 0;
            actor->current.angle.z = 0;
            actor->speed.y = 0.0f;
            i_this->m2BD++;
        }
        break;
    case MODE_NZ3_MOVE_12:
        if (!i_this->mpMorf->isStop()) {
            break;
        }
        i_this->mSmokeCb.remove();
        i_this->mTimers[0] = cM_rndF(60.0f) + 60.0f;
        i_this->m2BC = ACTION_NZ_MOVE;
        i_this->m2BD = MODE_NZ_MOVE_0;
        break;
    }
}

/* 000049E8-00005B2C       .text nz4_move__FP8nz_class */
void nz4_move(nz_class*) {
    /* Nonmatching */
}

/* 00005B2C-000061E0       .text nz5_move__FP8nz_class */
void nz5_move(nz_class*) {
    /* Nonmatching */
}

/* 000061E0-000062C8       .text bomb_catch_SUB__FP8nz_class */
void bomb_catch_SUB(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;
    if (i_this->mHeldID != fpcM_ERROR_PROCESS_ID_e) {
        daBomb_c* bomb = (daBomb_c*)fopAcM_SearchByID(i_this->mHeldID);
        if (bomb != NULL) {
            if (i_this->m2B6 == 0) {
                bomb->setBombRestTime(0x96);
            }
            bomb->setBombOffCoSet();
            bomb->gravity = 0.0f;
            bomb->speedF = 0.0f;
            bomb->speed.setall(0.0f);
            bomb->current.angle = actor->current.angle;
            bomb->shape_angle = actor->shape_angle;
            bomb->shape_angle.z = i_this->m2E4;
            bomb->scale = actor->scale;
        }
    }
}

/* 000062C8-00006734       .text nezumi_move__FP8nz_classs */
void nezumi_move(nz_class*, short) {
    /* Nonmatching */
}

/* 00006734-000072D8       .text nz6_move__FP8nz_class */
void nz6_move(nz_class*) {
    /* Nonmatching */
}

/* 000072D8-00007B50       .text daNZ_Execute__FP8nz_class */
static BOOL daNZ_Execute(nz_class*) {
    /* Nonmatching */
    return TRUE;
}

/* 00007B50-00007B58       .text daNZ_IsDelete__FP8nz_class */
static BOOL daNZ_IsDelete(nz_class*) {
    return TRUE;
}

/* 00007B58-00007BE0       .text daNZ_Delete__FP8nz_class */
static BOOL daNZ_Delete(nz_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "NZ");
    if (i_this->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }
    i_this->mSmokeCb.remove();
    i_this->mRippleCb.end();
    i_this->mFollowCb.remove();
    enemy_fire_remove(&i_this->mEnemyFire);
    return TRUE;
}

/* 00007BE0-00007EB0       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c* a_this) {
    nz_class* i_this = (nz_class*)a_this;

    i_this->mpMorf = new mDoExt_McaMorf(
        (J3DModelData*)dComIfG_getObjectRes("NZ", dRes_INDEX_NZ_BDL_NZ_e),
        NULL,
        NULL,
        (J3DAnmTransformKey*)dComIfG_getObjectRes("NZ", dRes_INDEX_NZ_BCK_NZ_WAIT_e),
        J3DFrameCtrl::EMode_LOOP,
        1.0f,
        0,
        -1,
        1,
        NULL,
        0x00080000,
        0x37441422
    );
    if ((i_this->mpMorf == NULL) || (i_this->mpMorf->getModel() == NULL)) {
        return FALSE;
    }
    i_this->mpMorf->getModel()->setUserArea((u32)i_this);
    for (u16 i = 0; i < i_this->mpMorf->getModel()->getModelData()->getJointNum(); i++) {
        if ((i == NZ_NZ_JNT_HIP_e) || (i == NZ_NZ_JNT_SHIPPO_e)) {
            i_this->mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_tail);
        }
        if (i == NZ_NZ_JNT_HEAD_e) {
            i_this->mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_head);
        }
        if ((i == NZ_NZ_JNT_UDEL3_e) || (i == NZ_NZ_JNT_UDER3_e)) {
            i_this->mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_hand);
        }
    }
    if (i_this->m2B5 == 0) {
        if (!i_this->mLineMat.init(1, 10, (ResTIMG*)dComIfG_getObjectRes("NZ", dRes_INDEX_NZ_BTI_SIPPO_e), 0)) {
            return FALSE;
        }
    } else {
        if (!i_this->mLineMat.init(1, 10, (ResTIMG*)dComIfG_getObjectRes("NZ", dRes_INDEX_NZ_BTI_BOM_SIPPO_e), 0)) {
            return FALSE;
        }
    }
    i_this->mpBmt = (J3DMaterialTable*)dComIfG_getObjectRes("NZ", nz_bmt_idx[i_this->m2B5 & 1]);
    if (i_this->mpBmt == NULL) {
        return FALSE;
    }
    if (i_this->mInvModel.create(i_this->mpMorf->getModel())) {
        return TRUE;
    }
    return FALSE;
}

/* 00007EB0-000081AC       .text daNZ_CreateInit__FP8nz_class */
void daNZ_CreateInit(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;
    actor->max_health = 1;
    actor->health = 1;
    actor->attention_info.flags = fopAc_Attn_LOCKON_BATTLE_e;

    i_this->mAcchCir.SetWall(60.0f, 60.0f);
    i_this->mAcch.Set(fopAcM_GetPosition_p(actor), fopAcM_GetOldPosition_p(actor), actor, 1, &i_this->mAcchCir, fopAcM_GetSpeed_p(actor), NULL, NULL);
    i_this->mStts.Init(0xFF, 1, actor);

    static dCcD_SrcCyl body_cyl_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_UNK800,
            /* SrcObjAt  Atp     */ 1,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
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
            /* SrcGObjTg SPrm    */ 0,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGCylS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 50.0f,
            /* Height */ 20.0f,
        }},
    };
    i_this->mCyl.Set(body_cyl_src);
    i_this->mCyl.SetStts(&i_this->mStts);

    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseScale(actor->scale);
    mDoMtx_stack_c::transS(actor->current.pos);
    mDoMtx_stack_c::ZXYrotM(actor->shape_angle);
    model->setBaseTRMtx(mDoMtx_stack_c::now);
    i_this->mpMorf->calc();
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &actor->current.pos, &actor->tevStr);
    fopAcM_SetMtx(actor, i_this->mpMorf->getModel()->getBaseTRMtx());

    actor->gravity = -5.0f;
    i_this->mEnemyIce.mpActor = actor;
    i_this->mEnemyIce.mWallRadius = 40.0f;
    i_this->mEnemyIce.mCylHeight = 40.0f;

    i_this->mEnemyFire.mpMcaMorf = i_this->mpMorf;
    i_this->mEnemyFire.mpActor = actor;
    static u8 fire_j[] = {0x00, 0x02, 0x04, 0x07, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x01};
    static f32 fire_sc[] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    for (int i = 0; i < 10; i++) {
        i_this->mEnemyFire.mFlameJntIdxs[i] = fire_j[i];
        i_this->mEnemyFire.mParticleScale[i] = fire_sc[i];
    }

    i_this->m2BC = 0;
    i_this->m2BD = 2;
    actor->itemTableIdx = dComIfGp_CharTbl()->GetNameIndex2("nezumi", 0);

    if (i_this->m2B4 != 0 || i_this->m2B5 != 0) {
        i_this->m31C = actor->current.pos;
        actor->scale.setall(0.0f);
        i_this->mNzgID = fopAcM_GetLinkId(actor);
        if (i_this->mNzgID == fpcM_ERROR_PROCESS_ID_e) {
            i_this->mNzgID = 0;
        }
    }

    actor->gravity = 0.0f;
    actor->gbaName = 0x18;

    if (i_this->m2B5 != 0) {
        actor->itemTableIdx = dComIfGp_CharTbl()->GetNameIndex2("nezumi", 1);
        i_this->mCyl.SetTgType(AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT & ~AT_TYPE_BOMB);
        actor->gravity = -5.0f;
        i_this->mStts.SetWeight(0x64);
        i_this->m31C = actor->current.pos;
        i_this->m2BC = 5;
        i_this->m2BD = 0x46;
    }

    i_this->mTailPos[0] = i_this->mTailRoot[0];
    tail_control(i_this);
    actor->speedF = 20.0f;
    i_this->m580 = actor->current.angle.y;
    i_this->mCyl.OffAtSetBit();
}

/* 000081AC-00008294       .text daNZ_Create__FP10fopAc_ac_c */
static cPhs_State daNZ_Create(fopAc_ac_c* a_this) {
    nz_class* i_this = (nz_class*)a_this;
    fopAcM_ct(a_this, nz_class);

    cPhs_State phase_state = dComIfG_resLoad(&i_this->mPhase, "NZ");
    if (phase_state == cPhs_COMPLEATE_e) {
        i_this->m2B4 = fopAcM_GetParam(a_this);
        i_this->m2B5 = fopAcM_GetParam(a_this) >> 8;
        i_this->m979 = fopAcM_GetParam(a_this) >> 24;
        if (i_this->m2B5 == 0xFF) {
            i_this->m2B5 = 0;
        }
        if (i_this->m2B4 == 0xFF) {
            i_this->m2B4 = 0;
        }
        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x3200)) {
            return cPhs_ERROR_e;
        }
        daNZ_CreateInit(i_this);
    }
    return phase_state;
}

static actor_method_class l_daNZ_Method = {
    (process_method_func)daNZ_Create,
    (process_method_func)daNZ_Delete,
    (process_method_func)daNZ_Execute,
    (process_method_func)daNZ_IsDelete,
    (process_method_func)daNZ_Draw,
};

actor_process_profile_definition g_profile_NZ = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NZ_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(nz_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NZ_e,
    /* Actor SubMtd */ &l_daNZ_Method,
    /* Status       */ fopAcStts_SHOWMAP_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e | fopAcStts_UNK80000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
