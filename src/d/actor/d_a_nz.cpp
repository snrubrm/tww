/**
 * d_a_nz.cpp
 * Enemy - Rat
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_nz.h"
#include "d/actor/d_a_bomb.h"
#include "d/actor/d_a_item.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/J3DGraphAnimator/J3DModel.h"

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
void tail_control(nz_class*) {
    /* Nonmatching */
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
void rakka_line_check(nz_class*) {
    /* Nonmatching */
}

/* 00001578-0000178C       .text daNZ_Draw__FP8nz_class */
static BOOL daNZ_Draw(nz_class*) {
    /* Nonmatching */
    return TRUE;
}

/* 0000178C-00001888       .text item_poi__FP8nz_class */
void item_poi(nz_class* i_this) {
    fopAc_ac_c* actor = i_this;
    fopAc_ac_c* held = fopAcM_SearchByID(i_this->mHeldID);
    if (held != NULL) {
        if (fopAcM_GetName(held) == fpcNm_BOMB_e) {
            daBomb_c* bomb = (daBomb_c*)held;
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
            held->speedF = 20.0f;
            held->speed.y = 20.0f;
            held->gravity = -3.0f;
            held->current.angle.y = actor->current.angle.y;
            if (!((daItem_c*)held)->checkLock()) {
                ((daItem_c*)held)->endControl();
                ((daItem_c*)held)->releaseLock();
            }
        }
    }
    i_this->mHeldID = fpcM_ERROR_PROCESS_ID_e;
    i_this->m42B = 0;
}

/* 00001888-00001F18       .text naraku_water_check__FP8nz_class */
void naraku_water_check(nz_class*) {
    /* Nonmatching */
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
void search_get_obj(nz_class*) {
    /* Nonmatching */
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
void search_check(nz_class*) {
    /* Nonmatching */
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
void shock_damage_check(nz_class*) {
    /* Nonmatching */
}

/* 00003090-00003240       .text body_atari_check__FP8nz_class */
void body_atari_check(nz_class*) {
    /* Nonmatching */
}

/* 00003240-000042D8       .text nz_move__FP8nz_class */
void nz_move(nz_class*) {
    /* Nonmatching */
}

/* 000042D8-000044B8       .text money_drop__FP8nz_class */
void money_drop(nz_class*) {
    /* Nonmatching */
}

/* 000044B8-0000482C       .text nz2_move__FP8nz_class */
void nz2_move(nz_class*) {
    /* Nonmatching */
}

/* 0000482C-000049E8       .text nz3_move__FP8nz_class */
void nz3_move(nz_class*) {
    /* Nonmatching */
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
        (J3DModelData*)dComIfG_getObjectRes("NZ", 0x23),
        NULL,
        NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes("NZ", 0x26),
        J3DFrameCtrl::EMode_LOOP,
        1.0f,
        0,
        -1,
        1,
        NULL,
        0x80000,
        0x37441422
    );
    if (i_this->mpMorf == NULL || i_this->mpMorf->getModel() == NULL) {
        return FALSE;
    }

    J3DModel* model = i_this->mpMorf->getModel();
    model->setUserArea((u32)i_this);
    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        if (i == 1 || i == 9) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_tail);
        }
        if (i == 0xB) {
            i_this->mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_head);
        }
        if (i == 0x12 || i == 0x15) {
            i_this->mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_hand);
        }
    }

    if (i_this->m2B5 == 0) {
        ResTIMG* img = (ResTIMG*)dComIfG_getObjectRes("NZ", 0x2E);
        if (!i_this->mLineMat.init(1, 10, img, FALSE)) {
            return FALSE;
        }
    } else {
        ResTIMG* img = (ResTIMG*)dComIfG_getObjectRes("NZ", 0x2D);
        if (!i_this->mLineMat.init(1, 10, img, FALSE)) {
            return FALSE;
        }
    }

    i_this->mpBmt = (J3DMaterialTable*)dComIfG_getObjectRes("NZ", nz_bmt_idx[i_this->m2B5 & 1]);
    if (i_this->mpBmt == NULL) {
        return FALSE;
    }

    return i_this->mInvModel.create(i_this->mpMorf->getModel()) != FALSE;
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
    i_this->m97C = actor;
    i_this->mB1C = 40.0f;
    i_this->mB18 = 40.0f;

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
