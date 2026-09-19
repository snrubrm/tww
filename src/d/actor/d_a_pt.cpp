/**
 * d_a_pt.cpp
 * Enemy - Miniblin
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_pt.h"
#include "c/c_damagereaction.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/d_snap.h"
#include "d/d_particle_name.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "m_Do/m_Do_hostIO.h"
#include "SSystem/SComponent/c_lib.h"
#include "SSystem/SComponent/c_math.h"
#include "res/Object/Pt.h"

static bool hio_set;
static daPt_HIO_c l_HIO;
static cXyz p_check_d[6];
static cXyz p_off_d[6];

/* 000000EC-00000110       .text __ct__10daPt_HIO_cFv */
daPt_HIO_c::daPt_HIO_c() {
    mNo = -1;
    m05 = 0;
    m06 = 0;
}

/* 00000110-00000238       .text anm_init__FP8pt_classifUcfi */
void anm_init(pt_class* i_this, int bckFileIdx, float morf, unsigned char loopMode, float speed, int soundFileIdx) {
    if (soundFileIdx >= 0) {
        void* soundAnm = dComIfG_getObjectRes("Pt", soundFileIdx);
        J3DAnmTransform* bckAnm = (J3DAnmTransform*)dComIfG_getObjectRes("Pt", bckFileIdx);
        i_this->mpMorf->setAnm(bckAnm, loopMode, morf, speed, 0.0f, -1.0f, soundAnm);
    } else {
        J3DAnmTransform* bckAnm = (J3DAnmTransform*)dComIfG_getObjectRes("Pt", bckFileIdx);
        i_this->mpMorf->setAnm(bckAnm, loopMode, morf, speed, 0.0f, -1.0f, NULL);
    }
}

/* 00000238-00000360       .text daPt_Draw__FP8pt_class */
static BOOL daPt_Draw(pt_class* i_this) {
    if (i_this->mHide == 0) {
        J3DModel* model = i_this->mpMorf->getModel();
        g_env_light.setLightTevColorType(model, &i_this->tevStr);
        i_this->mpBtp->entry(model->getModelData());
        i_this->mpBrk->entry(model->getModelData());
        i_this->mpMorf->entryDL();

        if (i_this->m326 != 0) {
            cXyz shadow_pos(i_this->current.pos.x, i_this->current.pos.y + 100.0f, i_this->current.pos.z);
            i_this->mShadowId = dComIfGd_setShadow(
                i_this->mShadowId,
                1,
                model,
                &shadow_pos,
                400.0f,
                50.0f,
                i_this->current.pos.y,
                i_this->mAcch.GetGroundH(),
                i_this->mAcch.m_gnd,
                &i_this->tevStr,
                0,
                1.0f,
                dDlst_shadowControl_c::getSimpleTex()
            );
            i_this->m326 = 0;
        }
    }

    dSnap_RegistFig(DSNAP_TYPE_UNKAB, i_this, 1.0f, 1.0f, 1.0f);
    return TRUE;
}

/* 0000039C-00000484       .text smoke_set__FP8pt_classSc */
void smoke_set(pt_class* i_this, signed char param_2) {
    fopAc_ac_c* actor = i_this;
    if (i_this->mSmokeFlag == 0) {
        i_this->mSmokeCb.remove();
        s8 roomNo = actor->current.roomNo;
        JPABaseEmitter* emitter = dComIfGp_particle_setToon(
            dPa_name::ID_AK_JT_ELEMENTSMOKE00,
            &actor->current.pos,
            &actor->shape_angle,
            NULL,
            0xB9,
            &i_this->mSmokeCb,
            roomNo
        );
        if (emitter != NULL) {
            emitter->setRate(3.0f);
            emitter->setSpread(0.5f);
            JGeometry::TVec3<f32> scale;
            scale.x = scale.y = scale.z = 1.0f;
            emitter->setGlobalScale(scale);
            scale.x = scale.y = scale.z = 1.5f;
            emitter->setGlobalParticleScale(scale);
            i_this->mSmokeFlag = param_2;
        }
    }
}

/* 00000484-0000093C       .text damage_check__FP8pt_class */
void damage_check(pt_class*) {
    /* Nonmatching - dummy pool host for later 100% funcs */
    volatile f32 dummy;
    volatile f64 dummy_d;
    dummy = 40.0f;
    dummy_d = 0.5;
    dummy_d = 3.0;
    dummy = 350.0f;
    dummy = 20.0f;
    dummy = 30.0f;
    dummy = 90.0f;
    dummy = 2000.0f;
    dummy = 60.0f;
    dummy = 80.0f;
    dummy = 110.0f;
    dummy = 10.0f;
    dummy = 2.0f;
    dummy = -5.0f;
    dummy = -10.0f;
    dummy = 4000.0f;
    dummy = 200.0f;
    dummy = -2.0f;
    dummy = -400.0f;
    dummy = -50.0f;
    dummy = 0.1f;
    dummy = 0.02f;
    dummy = 5.0f;
    dummy = 500.0f;
    dummy = -100.0f;
    dummy = 4.0f;
    dummy = 300.0f;
    dummy = 0.8f;
    dummy = 11.0f;
    dummy = 14.0f;
    dummy = 7.0f;
    dummy = -120.0f;
    dummy = 700.0f;
    dummy_d = 4503599627370496.0;
    dummy = 0.05f;
    dummy = 1.3f;
    dummy = 6000.0f;
    dummy = 32768.0f;
    dummy = -1000000000.0f;
    dummy = 0.01f;
    dummy = -3000.0f;
    dummy = 0.9f;
    dummy = 20000.0f;
    dummy = 50000.0f;
    dummy_d = 4503601774854144.0;
    dummy = 3.999f;
    dummy = 0.6f;
    dummy = -3.0f;
}

/* 0000093C-00000D74       .text get_z_ang__FP8pt_class */
void get_z_ang(pt_class*) {
    /* Nonmatching */
}

/* 000011AC-000018EC       .text next_pos_set__FP8pt_class */
void next_pos_set(pt_class*) {
    /* Nonmatching */
}

/* 000018EC-00001DC0       .text pt_move__FP8pt_class */
void pt_move(pt_class*) {
    /* Nonmatching */
}

/* 00001DC0-000020D4       .text view_check__FP8pt_class */
void view_check(pt_class*) {
    /* Nonmatching */
}

/* 000020D4-00002528       .text pt_attack__FP8pt_class */
void pt_attack(pt_class*) {
    /* Nonmatching */
}

/* 00002528-00002AC8       .text pt_wait__FP8pt_class */
void pt_wait(pt_class*) {
    /* Nonmatching */
}

/* 00002AC8-00002F30       .text pt_koke__FP8pt_class */
void pt_koke(pt_class*) {
    /* Nonmatching */
}

/* 00002F30-00003028       .text pt_ples__FP8pt_class */
BOOL pt_ples(pt_class* i_this) {
    i_this->m30E = 5;
    i_this->current.angle.z = 0;
    i_this->current.angle.x = 0;

    switch (i_this->mMode) {
    case 0:
        i_this->m308 = 0x23;
        i_this->mMode = 1;
        anm_init(i_this, dRes_INDEX_PT_BCK_WAIT_e, 1.0f, J3DFrameCtrl::EMode_NONE, 5.0f, -1);
        // fallthrough
    case 1:
        cLib_addCalc2(&i_this->scale.y, 0.1f, 1.0f, 0.5f);
        cLib_addCalc2(&i_this->scale.x, 1.3f, 0.8f, 0.5f);
        if (i_this->m308 == 0) {
            return TRUE;
        }
        break;
    }

    i_this->m326 = 1;
    i_this->current.pos.y += i_this->speed.y;
    i_this->speed.y -= 7.0f;
    return FALSE;
}

/* 00003028-000031DC       .text pt_bat__FP8pt_class */
BOOL pt_bat(pt_class* i_this) {
    fopAc_ac_c* actor = i_this;
    i_this->m30E = 5;

    switch (i_this->mMode) {
    case 0:
        i_this->m308 = 0x96;
        i_this->mMode = 1;
        anm_init(i_this, dRes_INDEX_PT_BCK_WAIT_e, 1.0f, J3DFrameCtrl::EMode_NONE, 5.0f, -1);
        {
            s16 angle = fopAcM_searchActorAngleY(actor, dComIfGp_getPlayer(0));
            i_this->m324 = angle + 0x8000 + (s16)cM_rndFX(6000.0f);
            mDoMtx_YrotS(*calc_mtx, i_this->m324);
            cXyz offset;
            offset.x = 0.0f;
            offset.y = 50.0f + cM_rndF(20.0f);
            offset.z = 100.0f;
            MtxPosition(&offset, &actor->speed);
            actor->current.angle.y = (s16)cM_rndFX(32768.0f);
        }
        // fallthrough
    case 1:
        actor->current.angle.y += 0x400;
        actor->current.angle.x += 0x300;
        if (i_this->m308 == 0) {
            return TRUE;
        }
        break;
    }

    i_this->m326 = 1;
    actor->current.pos += actor->speed;
    actor->speed.y -= 5.0f;
    if (actor->speed.y < -120.0f) {
        actor->speed.y = -120.0f;
    }

    if (i_this->mAcch.ChkWallHit() || (actor->speed.y < 0.0f && i_this->mAcch.ChkGroundHit())) {
        return TRUE;
    }
    return FALSE;
}

/* 000031DC-000036C0       .text water_check__FP8pt_class */
void water_check(pt_class*) {
    /* Nonmatching */
}

/* 00003B00-00003B4C       .text esa_s_sub__FPvPv */
void* esa_s_sub(void* i_actor, void*) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == fpcNm_ESA_e) {
        return i_actor;
    }
    return NULL;
}

/* 00003B4C-00003F5C       .text action__FP8pt_class */
void action(pt_class*) {
    /* Nonmatching */
}

/* 00003F5C-000043C8       .text daPt_Execute__FP8pt_class */
static BOOL daPt_Execute(pt_class*) {
    /* Nonmatching */
    return TRUE;
}

/* 000043C8-000043D0       .text daPt_IsDelete__FP8pt_class */
static BOOL daPt_IsDelete(pt_class*) {
    return TRUE;
}

/* 000043D0-00004540       .text daPt_Delete__FP8pt_class */
static BOOL daPt_Delete(pt_class* i_this) {
    fopAc_ac_c* actor = i_this;
    dComIfG_resDelete(&i_this->mPhase, "Pt");
    if (i_this->mHioSet) {
        hio_set = false;
        mDoHIO_deleteChild(l_HIO.mNo);
    }
    i_this->mSmokeCb.remove();
    enemy_fire_remove(&i_this->mEnemyFire);

    if (i_this->mDoRespawn != 0) {
        if (i_this->mEnableSpawnSwitch == 0xFF || dComIfGs_isSwitch(i_this->mEnableSpawnSwitch, fopAcM_GetRoomNo(actor))) {
            if (i_this->mDisableRespawnSwitch == 0xFF || !dComIfGs_isSwitch(i_this->mDisableRespawnSwitch, dStage_roomControl_c::getStayNo())) {
                fopAcM_prm_class* append = fopAcM_CreateAppend();
                append->base.position = actor->home.pos;
                append->base.angle = actor->home.angle;
                if (i_this->mRespawnDelay != 7) {
                    append->base.angle.x = i_this->mRespawnDelay * 0x14 + 0x14;
                }
                append->base.parameters = fopAcM_GetParam(actor) | 0x10;
                append->room_no = fopAcM_GetRoomNo(actor);
                fopAcM_Create(fpcNm_PT_e, NULL, append);
            }
        }
    }
    return TRUE;
}

/* 00004540-000047D4       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c* a_this) {
    pt_class* i_this = (pt_class*)a_this;

    i_this->mpMorf = new mDoExt_McaMorf(
        (J3DModelData*)dComIfG_getObjectRes("Pt", dRes_INDEX_PT_BDL_PT_e),
        NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes("Pt", dRes_INDEX_PT_BCK_WAIT_e),
        J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, 1,
        NULL,
        0,
        0x11020203
    );
    if ((i_this->mpMorf == NULL) || (i_this->mpMorf->getModel() == NULL)) {
        return FALSE;
    }

    i_this->mpBtp = new mDoExt_btpAnm();
    if (i_this->mpBtp == NULL) {
        return FALSE;
    }
    if (i_this->mpBtp->init(
            i_this->mpMorf->getModel()->getModelData(),
            (J3DAnmTexPattern*)dComIfG_getObjectRes("Pt", dRes_INDEX_PT_BTP_MABATAKI_e),
            TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, FALSE
        ) == 0)
    {
        return FALSE;
    }

    i_this->mpBrk = new mDoExt_brkAnm();
    if (i_this->mpBrk == NULL) {
        return FALSE;
    }
    if (i_this->mpBrk->init(
            i_this->mpMorf->getModel()->getModelData(),
            (J3DAnmTevRegKey*)dComIfG_getObjectRes("Pt", dRes_INDEX_PT_BRK_PT_e),
            TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, FALSE
        ) == 0)
    {
        return FALSE;
    }

    i_this->m2CC = cM_rndF(3.999f);
    return TRUE;
}

/* 0000481C-00004B1C       .text daPt_Create__FP10fopAc_ac_c */
static cPhs_State daPt_Create(fopAc_ac_c* a_this) {
    static dCcD_SrcSph cc_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
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
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 40.0f,
        }},
    };
    static dCcD_SrcSph at_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_UNK800,
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
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 25.0f,
        }},
    };
    static u8 fire_j[] = {0x08, 0xFF, 0x07, 0xFA, 0x0B, 0xFF, 0x0D, 0xFF, 0x00, 0x00};
    static f32 fire_sc[] = {1.0f, 0.0f, 0.7f, 0.0f, 0.6f, 0.0f, 0.6f, 0.0f, 0.0f, 0.0f};

    pt_class* i_this = (pt_class*)a_this;
    fopAcM_ct(a_this, pt_class);

    cPhs_State phase_state = dComIfG_resLoad(&i_this->mPhase, "Pt");
    if (phase_state == cPhs_COMPLEATE_e) {
        i_this->mBehaviorType = fopAcM_GetParam(a_this) & 0xF;
        i_this->mRespawnDelay = (fopAcM_GetParam(a_this) >> 5) & 7;
        i_this->mNoticeRange = (fopAcM_GetParam(a_this) >> 8) & 0xFF;
        if (fopAcM_GetParam(a_this) & 0x10) {
            i_this->mHide = 1;
        }
        i_this->mInitialSpawnDelay = a_this->current.angle.x;
        a_this->current.angle.x = 0;
        i_this->mDisableRespawnSwitch = (fopAcM_GetParam(a_this) >> 16) & 0xFF;
        i_this->mEnableSpawnSwitch = fopAcM_GetParam(a_this) >> 24;
        if (i_this->mEnableSpawnSwitch != 0xFF && !dComIfGs_isSwitch(i_this->mEnableSpawnSwitch, dStage_roomControl_c::getStayNo())) {
            i_this->mHide = 1;
        }

        a_this->itemTableIdx = dComIfGp_CharTbl()->GetNameIndex("Puti", 0);
        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x4B000)) {
            return cPhs_ERROR_e;
        }

        if (!hio_set) {
            i_this->mHioSet = true;
            hio_set = true;
            l_HIO.mNo = mDoHIO_createChild("プチブリン", &l_HIO);
        }

        fopAcM_OnStatus(a_this, fopAcStts_CULL_e);
        a_this->attention_info.flags = fopAc_Attn_LOCKON_BATTLE_e;
        i_this->m2D2 = 0;
        fopAcM_SetMtx(a_this, i_this->mpMorf->getModel()->getBaseTRMtx());
        i_this->mAcch.Set(
            fopAcM_GetPosition_p(a_this),
            fopAcM_GetOldPosition_p(a_this),
            a_this,
            1,
            &i_this->mAcchCir,
            fopAcM_GetSpeed_p(a_this),
            NULL,
            NULL
        );
        i_this->mAcchCir.SetWall(50.0f, 50.0f);
        a_this->health = 2;
        a_this->max_health = 2;
        i_this->mStts.Init(0x64, 0, a_this);
        i_this->mSph.Set(cc_sph_src);
        i_this->mSph.SetStts(&i_this->mStts);
        i_this->mAtSph.Set(at_sph_src);
        i_this->mAtSph.SetStts(&i_this->mStts);
        a_this->stealItemLeft = 2;

        i_this->mEnemyIce.mpActor = a_this;
        i_this->mEnemyIce.mWallRadius = REG0_F(4) + 30.0f;
        i_this->mEnemyIce.mCylHeight = REG0_F(5) + 30.0f;
        i_this->mEnemyIce.mParticleScale = REG0_F(8) + 0.6f;
        i_this->mEnemyIce.mYOffset = REG0_F(9) + 40.0f;
        i_this->mEnemyFire.mpMcaMorf = i_this->mpMorf;
        i_this->mEnemyFire.mpActor = a_this;
        for (int i = 0; i < ARRAY_SIZE(i_this->mEnemyFire.mFlameJntIdxs); i++) {
            i_this->mEnemyFire.mFlameJntIdxs[i] = fire_j[i];
            i_this->mEnemyFire.mParticleScale[i] = fire_sc[i];
        }

        daPt_Execute(i_this);
    }
    return phase_state;
}

static actor_method_class l_daPt_Method = {
    (process_method_func)daPt_Create,
    (process_method_func)daPt_Delete,
    (process_method_func)daPt_Execute,
    (process_method_func)daPt_IsDelete,
    (process_method_func)daPt_Draw,
};

actor_process_profile_definition g_profile_PT = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_PT_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(pt_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_PT_e,
    /* Actor SubMtd */ &l_daPt_Method,
    /* Status       */ fopAcStts_UNK40000_e | fopAcStts_UNK80000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
