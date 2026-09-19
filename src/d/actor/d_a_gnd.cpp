/**
 * d_a_gnd.cpp
 * Boss - Ganondorf
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_gnd.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/d_snap.h"
#include "m_Do/m_Do_graphic.h"
#include "d/d_demo.h"
#include "d/d_particle_name.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_arrow.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_audio.h"
#include "m_Do/m_Do_hostIO.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JUtility/JUTReport.h"
#include "res/Object/Gnd.h"

static cXyz non_pos(-20000.0f, -20000.0f, -20000.0f);
static bool hio_set;
static daGnd_HIO_c l_HIO;

static u32 attack_eff_joint_d[] = {
    0x1A, 0x2C, 0x04, 0x07, 0x1A, 0x2C,
};

static u32 attack_eff_id[] = {
    dPa_name::ID_AK_SN_GNDAATTACK1HANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDAATTACK1HANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDAATTACK1FOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDAATTACK1FOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDAATTACK1BLURL00,
    dPa_name::ID_AK_SP_GNDAATTACK1BLURR00,
    dPa_name::ID_AK_SN_GNDAATTACK2HANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDAATTACK2HANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDAATTACK2FOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDAATTACK2FOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDAATTACK2BLURL00,
    dPa_name::ID_AK_SP_GNDAATTACK2BLURR00,
    dPa_name::ID_AK_SN_GNDAATTACK3HANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDAATTACK3HANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDAATTACK3FOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDAATTACK3FOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDAATTACK3BLURL00,
    dPa_name::ID_AK_SP_GNDAATTACK3BLURR00,
    dPa_name::ID_AK_SN_GNDBATTACK1HANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDBATTACK1HANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDBATTACK1FOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDBATTACK1FOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDBATTACK1BLURL00,
    dPa_name::ID_AK_SP_GNDBATTACK1BLURR00,
    dPa_name::ID_AK_SN_GNDBATTACK2HANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDBATTACK2HANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDBATTACK2FOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDBATTACK2FOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDBATTACK2BLURL00,
    dPa_name::ID_AK_SP_GNDBATTACK2BLURR00,
    0x8396, 0x8397, 0x8398, 0x8399,
    dPa_name::ID_AK_SP_GNDBATTACK3TBLURL00,
    dPa_name::ID_AK_SP_GNDBATTACK3TBLURR00,
    0x839A, 0x839B, 0x839C, 0x839D,
    dPa_name::ID_AK_SP_GNDBATTACK3ABLURL00,
    dPa_name::ID_AK_SP_GNDBATTACK3ABLURR00,
    0x839E, 0x839F, 0x83A0, 0x83A1,
    dPa_name::ID_AK_SP_GNDBATTACK3EBLURL00,
    dPa_name::ID_AK_SP_GNDBATTACK3EBLURR00,
    dPa_name::ID_AK_SN_GNDJATTACKTHANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDJATTACKTHANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDJATTACKTFOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDJATTACKTFOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDJATTACKTBLURL00,
    dPa_name::ID_AK_SP_GNDJATTACKTBLURR00,
    dPa_name::ID_AK_SN_GNDJATTACKJHANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDJATTACKJHANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDJATTACKJFOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDJATTACKJFOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDJATTACKJBLURL00,
    dPa_name::ID_AK_SP_GNDJATTACKJBLURR00,
    dPa_name::ID_AK_SN_GNDJATTACKAHANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDJATTACKAHANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDJATTACKAFOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDJATTACKAFOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDJATTACKABLURL00,
    dPa_name::ID_AK_SP_GNDJATTACKABLURR00,
    dPa_name::ID_AK_SN_GNDJATTACKEHANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDJATTACKEHANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDJATTACKEFOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDJATTACKEFOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDJATTACKEBLURL00,
    dPa_name::ID_AK_SP_GNDJATTACKEBLURR00,
    dPa_name::ID_AK_SN_GNDNATTACK1HANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDNATTACK1HANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDNATTACK1FOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDNATTACK1FOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDNATTACK1BLURL00,
    dPa_name::ID_AK_SP_GNDNATTACK1BLURR00,
    dPa_name::ID_AK_SN_GNDNATTACK2HANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDNATTACK2HANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDNATTACK2FOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDNATTACK2FOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDNATTACK2BLURL00,
    dPa_name::ID_AK_SP_GNDNATTACK2BLURR00,
    dPa_name::ID_AK_SN_GNDCOUNTERLHANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDCOUNTERLHANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDCOUNTERLFOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDCOUNTERLFOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDCOUNTERLBLURL00,
    dPa_name::ID_AK_SP_GNDCOUNTERLBLURR00,
    dPa_name::ID_AK_SN_GNDCOUNTERRHANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDCOUNTERRHANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDCOUNTERRFOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDCOUNTERRFOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDCOUNTERRBLURL00,
    dPa_name::ID_AK_SP_GNDCOUNTERRBLURR00,
    dPa_name::ID_AK_SN_GNDCOUNTERRHANDSPLASHL00,
    dPa_name::ID_AK_SN_GNDCOUNTERRHANDSPLASHR00,
    dPa_name::ID_AK_SN_GNDCOUNTERRFOOTSPLASHL00,
    dPa_name::ID_AK_SN_GNDCOUNTERRFOOTSPLASHR00,
    dPa_name::ID_AK_SP_GNDCOUNTERRBLURL00,
    dPa_name::ID_AK_SP_GNDCOUNTERRBLURR00,
    0, 0, 0, dPa_name::ID_AK_SN_GNDKICKSPLASH00, 0, 0,
};

static u32 ke_set_index[] = {0x1B, 0x1B, 0x2D, 0x2D};
static f32 ke_set_offsetxz[] = {-4.0f, 4.0f, -4.0f, 4.0f};

/* 000000EC-0000023C       .text __ct__11daGnd_HIO_cFv */
daGnd_HIO_c::daGnd_HIO_c() {
    mNo = -1;
    m05 = 0;
    m06 = 0;
    m07 = 1;
    m08 = 45.0f;
    m0C = 1.0f;
    m10 = 0;
    m11 = 0;
    m12 = 1;
    m14 = 5.0f;
    m18 = 629.0f;
    m1C = 1200.0f;
    m20 = 25.0f;
    m24 = 11.0f;
    m28 = -0.5f;
    m2C = 100.0f;
    m30 = -30.0f;
    m34 = 20.0f;
    m38 = -2.0f;
    m3C = 630.0f;
    m40 = 400.0f;
    m44 = 0;
    m45 = 1;
    m46 = 1;
    m48 = 35.0f;
    m4C = 0.0f;
    m50 = 5.0f;
    m54 = 15.0f;
    m58 = 0.0f;
    m5C = 30.0f;
    m60 = -2.0f;
    m64 = 8.0f;
    m68 = 30;
    m6A = 30;
    m6C = 0;
    m6E = 15;
    m70 = 10;
    m72 = 0;
    m74 = 10;
    m76 = 10;
    m78 = 600.0f;
    m7C = 10;
    m7E = 1;
    m80 = 30;
    m82 = 3;
    m84 = 6;
    m86 = 70;
    m88 = 0;
    m89 = 0;
}

/* 0000023C-000003CC       .text j_demo__FP9gnd_class */
static BOOL j_demo(gnd_class* i_this) {
    if (i_this->demoActorID == 0) {
        if (i_this->m15B0 != 0) {
            i_this->m15B0 = 0;
        }
    } else {
        i_this->m15B0 = 1;
        dDemo_actor_c* demo_actor = dComIfGp_demo_getActor(i_this->demoActorID);

        J3DAnmTexPattern* btp = demo_actor->getP_BtpData("Gnd");
        if (btp != NULL) {
            i_this->mpBtpAnm->init(i_this->mpMorf->getModel()->getModelData(), btp, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, true, 0);
        }

        J3DAnmTextureSRTKey* btk = demo_actor->getP_BtkData("Gnd");
        if (btk != NULL) {
            i_this->mpBtkAnm->init(i_this->mpMorf->getModel()->getModelData(), btk, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, true, 0);
        }

        J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)demo_actor->getP_BrkData("Gnd");
        if (brk != NULL) {
            i_this->mpBrkAnm->init(i_this->mpMorf->getModel()->getModelData(), brk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, true, 0);
        }

        dDemo_setDemoData(i_this, 0x6A, i_this->mpMorf, "Gnd", 0, NULL, 0, 0);
    }
    return i_this->m15B0;
}

/* 000003CC-000003EC       .text checkGround__FP9gnd_classf */
static BOOL checkGround(gnd_class* i_this, f32 param_2) {
    return i_this->current.pos.y < 1.0f + param_2;
}

/* 000003EC-00000510       .text setRipple__FP9gnd_class */
static void setRipple(gnd_class* i_this) {
    if (checkGround(i_this, 0.0f)) {
        if (i_this->mAcch.ChkGroundHit() &&
            dComIfG_Bgsp()->GetAttributeCode(i_this->mAcch.m_gnd) == dBgS_Attr_WATER_e &&
            i_this->mRippleCb.getEmitter() == NULL)
        {
            static cXyz scale(1.0f, 1.0f, 1.0f);
            dComIfGp_particle_setShipTail(
                dPa_name::ID_AK_JN_HAMON00, &i_this->current.pos, NULL, &scale, 0xFF, &i_this->mRippleCb
            );
            if (i_this->mRippleCb.getEmitter() != NULL) {
                i_this->mRippleCb.setRate(0.0f);
            }
        }
    } else {
        i_this->mRippleCb.end();
    }
}

static fopAc_ac_c* pz;

/* 0000054C-000005DC       .text splash_set__FP9gnd_class */
static void splash_set(gnd_class* i_this) {
    fopAc_ac_c* actor = i_this;

    JPABaseEmitter* emitter = dComIfGp_particle_set(dPa_name::ID_AK_SN_GNDLANDINGSPLASH00, &actor->current.pos);
    if (emitter != NULL) {
        emitter->setGlobalPrmColor(i_this->mPrmColorR, i_this->mPrmColorG, i_this->mPrmColorB);
    }
}

/* 000005DC-000006B4       .text attack_eff_set__FP9gnd_classi */
static void attack_eff_set(gnd_class* i_this, int param_2) {
    fopAc_ac_c* actor = i_this;

    for (int i = 0; i < 6; i++) {
        u32 id = attack_eff_id[param_2 * 6 + i];
        if (id == 0) {
            continue;
        }

        i_this->mpAttackEff[i] = dComIfGp_particle_set(id, &actor->current.pos);
        if (i <= 3) {
            i_this->mpAttackEff[i]->setGlobalPrmColor(i_this->mPrmColorR, i_this->mPrmColorG, i_this->mPrmColorB);
        }
    }
}

/* 000006B4-00000738       .text attack_eff_move__FP9gnd_class */
static void attack_eff_move(gnd_class* i_this) {
    J3DModel* model = i_this->mpMorf->getModel();
    for (int i = 0; i < 6; i++) {
        if (i_this->mpAttackEff[i] != NULL) {
            i_this->mpAttackEff[i]->setGlobalRTMatrix(model->getAnmMtx(attack_eff_joint_d[i]));
        }
    }
}

/* 00000738-0000077C       .text attack_eff_remove__FP9gnd_class */
static void attack_eff_remove(gnd_class* i_this) {
    for (int i = 0; i < 6; i++) {
        if (i_this->mpAttackEff[i] != NULL) {
            i_this->mpAttackEff[i]->becomeInvalidEmitter();
            i_this->mpAttackEff[i] = NULL;
        }
    }
}

/* 0000077C-00000900       .text anm_init__FP9gnd_classifUcfi */
static void anm_init(gnd_class* i_this, int bckFileIdx, f32 morf, u8 loopMode, f32 speed, int soundFileIdx) {
    if (soundFileIdx >= 0) {
        i_this->mpMorf->setAnm(
            (J3DAnmTransform*)dComIfG_getObjectRes("Gnd", bckFileIdx),
            loopMode,
            morf,
            speed,
            0.0f,
            -1.0f,
            dComIfG_getObjectRes("Gnd", soundFileIdx)
        );
    } else {
        i_this->mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Gnd", bckFileIdx), loopMode, morf, speed, 0.0f, -1.0f, NULL);
    }

    attack_eff_remove(i_this);

    int attack_anm[18] = {
        dRes_INDEX_GND_BCK_AATTACK1_e,
        dRes_INDEX_GND_BCK_AATTACK2_e,
        dRes_INDEX_GND_BCK_AATTACK3_e,
        dRes_INDEX_GND_BCK_BATTACK1_e,
        dRes_INDEX_GND_BCK_BATTACK2_e,
        dRes_INDEX_GND_BCK_BATTACK3_TAME_e,
        dRes_INDEX_GND_BCK_BATTACK3_ATTACK_e,
        dRes_INDEX_GND_BCK_BATTACK3_END_e,
        dRes_INDEX_GND_BCK_JATTACK_TAME_e,
        dRes_INDEX_GND_BCK_JATTACK_JUMP_e,
        dRes_INDEX_GND_BCK_JATTACK_ATTACK_e,
        dRes_INDEX_GND_BCK_JATTACK_END_e,
        dRes_INDEX_GND_BCK_NOM_ATTACK1_e,
        dRes_INDEX_GND_BCK_NOM_ATTACK2_e,
        dRes_INDEX_GND_BCK_COUNTERL_e,
        dRes_INDEX_GND_BCK_COUNTERR_e,
        dRes_INDEX_GND_BCK_COMBO_GANON_e,
        dRes_INDEX_GND_BCK_KERI2_e,
    };

    for (int i = 0; i < 18; i++) {
        if (bckFileIdx == attack_anm[i]) {
            attack_eff_set(i_this, i);
            break;
        }
    }
}

/* 00000900-0000094C       .text z_s_sub__FPvPv */
static void* z_s_sub(void* param_1, void*) {
    if (fopAcM_IsActor(param_1) && fopAcM_GetName(param_1) == fpcNm_PZ_e) {
        return param_1;
    }
    return NULL;
}

/* 0000094C-00000C38       .text daGnd_Draw__FP9gnd_class */
static BOOL daGnd_Draw(gnd_class* i_this) {
    fopAc_ac_c* actor = i_this;

    s16 blure = i_this->m155C;
    if (blure > 1) {
        mDoGph_gInf_c::setBlureRate(blure);
        mDoGph_gInf_c::onBlure();
    } else if (blure == 1) {
        i_this->m155C = 0;
        mDoGph_gInf_c::offBlure();
    }

    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &actor->current.pos, &actor->tevStr);

    s16 add = i_this->m142C;
    if (add != 0) {
        s16 fog_r = actor->tevStr.mFogColor.r + i_this->m1426;
        if (fog_r > 0xFF) {
            fog_r = 0xFF;
        }
        s16 fog_g = actor->tevStr.mFogColor.g + i_this->m1426;
        if (fog_g > 0xFF) {
            fog_g = 0xFF;
        }
        s16 fog_b = actor->tevStr.mFogColor.g + (i_this->m1426 / 2);
        if (fog_b > 0xFF) {
            fog_b = 0xFF;
        }

        s16 limit = REG8_S(5) + 0x14;
        if (add > limit) {
            cLib_addCalcAngleS2(&i_this->m1426, 0x118, 1, 0x1E);
            cLib_addCalc2(&i_this->m1428, -50000.0f, 1.0f, 5000.0f);
        } else {
            cLib_addCalcAngleS2(&i_this->m1426, 0, 1, 0xE);
            cLib_addCalc0(&i_this->m1428, 1.0f, 2500.0f);
        }

        actor->tevStr.mFogColor.r = (u8)fog_r;
        actor->tevStr.mFogColor.g = (u8)fog_g;
        actor->tevStr.mFogColor.b = (u8)fog_b;
        actor->tevStr.mFogStartZ += i_this->m1428;
    }

    g_env_light.setLightTevColorType(model, &actor->tevStr);
    J3DModelData* modelData = model->getModelData();
    i_this->mpBrkAnm->entry(modelData, i_this->mpBrkAnm->getFrame());
    modelData = model->getModelData();
    i_this->mpBtkAnm->entry(modelData, i_this->mpBtkAnm->getFrame());
    modelData = model->getModelData();
    i_this->mpBtpAnm->entry(modelData, i_this->mpBtpAnm->getFrame());
    i_this->mpMorf->entryDL();

    f32 z = actor->current.pos.z;
    cXyz shadow_pos(actor->current.pos.x, 400.0f + actor->current.pos.y + REG0_F(18), z);
    i_this->mShadowId = dComIfGd_setShadow(
        i_this->mShadowId,
        1,
        model,
        &shadow_pos,
        1300.0f + REG0_F(19),
        200.0f,
        actor->current.pos.y,
        i_this->mAcch.GetGroundH(),
        i_this->mAcch.m_gnd,
        &actor->tevStr,
        0,
        1.0f,
        dDlst_shadowControl_c::getSimpleTex()
    );

    dSnap_RegistFig(DSNAP_TYPE_UNKCE, actor, 1.0f, 1.0f, 1.0f);

    if (l_HIO.m07 != 0) {
        GXColor color = {0xFF, 0x64, 0x00, 0xFF};
        i_this->mLineMat.update(20, 2.25f + REG0_F(3), color, 2, &actor->tevStr);
        dComIfGd_set3DlineMat(&i_this->mLineMat);
    }

    return TRUE;
}

/* 00000C38-00000C6C       .text player_view_check__FP9gnd_classs */
static BOOL player_view_check(gnd_class* i_this, s16 param_2) {
    s16 diff = i_this->shape_angle.y - param_2;
    if (diff < 0) {
        diff = -diff;
    }
    if ((u16)diff < 0x4000) {
        return TRUE;
    }
    return FALSE;
}

/* 00000C6C-00000F24       .text ke_control__FP9gnd_classP8gnd_ke_sf */
static void ke_control(gnd_class* i_this, gnd_ke_s* param_2, f32 param_3) {
    cXyz local_118;
    cXyz local_124;
    f32 ground_y = 3.0f + i_this->mAcch.GetGroundH();
    cXyz* pos = &param_2->mPos[1];
    cXyz* vel = &param_2->mVel[1];

    local_118.x = 0.0f;
    local_118.y = 0.0f;
    local_118.z = l_HIO.m08 * param_3 * 0.5f;
    if (param_3 <= 0.05f) {
        local_118.z = 0.0f;
    }

    f32 gravity = -5.0f + REG0_F(1);
    f32 damp = 0.73f + REG0_F(2);
    for (s32 i = 1; i < 20; i++, pos++, vel++) {
        f32 x = vel->x + (pos->x - pos[-1].x);
        f32 y = pos->y + vel->y + gravity;
        if (y < ground_y) {
            y = ground_y;
        }
        f32 y_diff = y - pos[-1].y;
        f32 z = vel->z + (pos->z - pos[-1].z);
        s16 angX = -cM_atan2s(y_diff, z);
        s32 angY = cM_atan2s(x, std::sqrtf(SQUARE(y_diff) + SQUARE(z)));
        cMtx_XrotS(*calc_mtx, angX);
        cMtx_YrotM(*calc_mtx, angY);
        MtxPosition(&local_118, &local_124);
        *vel = *pos;
        pos->x = pos[-1].x + local_124.x;
        pos->y = pos[-1].y + local_124.y;
        pos->z = pos[-1].z + local_124.z;
        vel->x = damp * (pos->x - vel->x);
        vel->y = damp * (pos->y - vel->y);
        vel->z = damp * (pos->z - vel->z);
    }
}

/* 00000F24-00000F68       .text ke_pos_set__FP9gnd_classP8gnd_ke_si */
static void ke_pos_set(gnd_class* i_this, gnd_ke_s* param_2, int param_3) {
    cXyz* pos = i_this->mLineMat.getPos(param_3);
    for (s32 i = 0; i < 20; pos++, i++) {
        *pos = param_2->mPos[i];
    }
}

/* 00000F68-00001140       .text ke_move__FP9gnd_class */
static void ke_move(gnd_class* i_this) {
    cXyz offset;
    cXyz axis;
    gnd_ke_s* ke = i_this->mKe;
    J3DModel* model = i_this->mpMorf->getModel();
    for (s32 i = 0; i < 4; i++, ke++) {
        MTXCopy(model->getAnmMtx(ke_set_index[i]), *calc_mtx);
        f32 z = (*calc_mtx)[2][0];
        f32 y = (*calc_mtx)[1][0];
        f32 x = (*calc_mtx)[0][0];
        axis.x = x;
        axis.y = y;
        axis.z = z;
        f32 len = axis.abs();
        if (dComIfGp_evmng_startCheck("endhr") && dComIfGp_demo_get()->getFrameNoMsg() >= 0x12C) {
            len = 0.0f;
        }
        offset.x = -20.0f + REG0_F(4);
        offset.y = ke_set_offsetxz[i];
        offset.z = ke_set_offsetxz[i];
        MtxPosition(&offset, &ke->mPos[0]);
        ke_control(i_this, ke, len);
        ke_pos_set(i_this, ke, i);
    }
}

/* 00001140-00001278       .text pos_move__FP9gnd_classSc */
static void pos_move(gnd_class* i_this, s8 param_2) {
    fopAc_ac_c* actor = i_this;
    cXyz vec;

    if (param_2 == 0) {
        vec = *(cXyz*)&i_this->m2D4 - actor->current.pos;
        s16 angY = cM_atan2s(vec.x, vec.z);
        cLib_addCalcAngleS2(&actor->current.angle.y, angY, 5, i_this->m2F0 * i_this->m2F4);
        cLib_addCalc2(&i_this->m2F4, 1.0f, 1.0f, 0.05f);
    }

    cLib_addCalc2(&actor->speedF, i_this->m2F8, 1.0f, 6.0f);
    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = actor->speedF;
    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    cXyz sp;
    MtxPosition(&vec, &sp);
    actor->speed.x = sp.x;
    actor->speed.z = sp.z;
    actor->current.pos += actor->speed;
    actor->current.pos.y += actor->speed.y;
    actor->speed.y += actor->gravity;
    actor->gravity = -5.0f;
}

/* 00001278-00001334       .text wait_set__FP9gnd_class */
static void wait_set(gnd_class* i_this) {
    fopAc_ac_c* actor = i_this;

    if (i_this->m3D8 <= 1) {
        anm_init(i_this, dRes_INDEX_GND_BCK_WAIT_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1);
        fopAcM_monsSeStart(actor, JA_SE_CV_GN_LAUGH_WAIT, 0);
    } else {
        anm_init(i_this, dRes_INDEX_GND_BCK_WAIT2_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1);
    }
}

/* 00001334-00001974       .text move0__FP9gnd_class */
static void move0(gnd_class* i_this) {
    fopAc_ac_c* actor = i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    f32 dist = fopAcM_searchActorDistance(actor, player);
    int frame = (int)i_this->mpMorf->getFrame();
    s8 track = 0;

    switch (i_this->m2D0) {
    case 0:
        anm_init(i_this, dRes_INDEX_GND_BCK_WALK_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, dRes_INDEX_GND_BAS_WALK_e);
        i_this->m2D0++;
        break;
    case 1:
        i_this->m2F8 = l_HIO.m14;
        track = 1;
        if (dist < l_HIO.m18) {
            anm_init(i_this, dRes_INDEX_GND_BCK_WAIT2_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1);
            i_this->m2D0 = 2;
        } else if (dist > l_HIO.m1C) {
            anm_init(i_this, dRes_INDEX_GND_BCK_HI_JUMP_START_e, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f, dRes_INDEX_GND_BAS_HI_JUMP_START_e);
            i_this->m2D0 = 3;
            actor->speedF = 0.0f;
            i_this->m2F8 = 0.0f;
        }
        break;
    case 2:
        track = 1;
        i_this->m2F8 = 0.0f;
        if (player_view_check(i_this, fopAcM_searchActorAngleY(actor, dComIfGp_getPlayer(0)))) {
            if (dist > l_HIO.m3C && i_this->m304 == 0) {
                i_this->m2D0 = 0;
            } else if (dist < l_HIO.m2C) {
                i_this->m2D0 = 20;
            } else if (i_this->m304 == 0) {
                if (cM_rndF(100.0f) < l_HIO.m48) {
                    i_this->m2CE = 2;
                    if (cM_rndF(1.0f) < 0.5f) {
                        i_this->m2D0 = 0;
                    } else {
                        i_this->m2D0 = 1;
                    }
                } else {
                    i_this->m2CE = 1;
                    if (i_this->m3D8 == 0 || l_HIO.m11 == 1) {
                        i_this->m2D0 = 0;
                    } else if (i_this->m3D8 == 1 || l_HIO.m11 == 2) {
                        i_this->m2D0 = 5;
                    } else if (cM_rndF(1.0f) < 0.5f) {
                        i_this->m2D0 = 0;
                    } else {
                        i_this->m2D0 = 5;
                    }
                }
            }
        }
        break;
    case 3:
        actor->gravity = l_HIO.m28;
        if (frame == 12) {
            actor->speed.y = l_HIO.m24;
            f32 jump_speed = l_HIO.m20;
            actor->speedF = jump_speed;
            i_this->m2F8 = jump_speed;
            splash_set(i_this);
        }
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, dRes_INDEX_GND_BCK_HI_JUMP_LOOP_e, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1);
            i_this->m2D0++;
        }
        break;
    case 4:
        mDoAud_seStart(JA_SE_CM_GN_ROBE, &actor->eyePos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
        actor->gravity = l_HIO.m28;
        if (actor->speed.y < 0.0f) {
            anm_init(i_this, dRes_INDEX_GND_BCK_HI_JUMP_END1_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f, -1);
            i_this->m2D0++;
        }
        break;
    case 5:
        mDoAud_seStart(JA_SE_CM_GN_ROBE, &actor->eyePos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
        actor->gravity = l_HIO.m28;
        i_this->m2F0 = 0.0f;
        if (checkGround(i_this, 0.0f)) {
            anm_init(i_this, dRes_INDEX_GND_BCK_HI_JUMP_END2_e, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f, -1);
            i_this->m2D0 = 6;
            splash_set(i_this);
        }
        break;
    case 6:
        i_this->m2F8 = 0.0f;
        i_this->m2F0 = 0.0f;
        if (i_this->mpMorf->isStop()) {
            i_this->m2D0 = 2;
            anm_init(i_this, dRes_INDEX_GND_BCK_WAIT2_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1);
        }
        break;
    case 20:
        anm_init(i_this, dRes_INDEX_GND_BCK_JUMP1_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f, dRes_INDEX_GND_BAS_JUMP1_e);
        i_this->m2D0 = 21;
        break;
    case 21:
        track = 1;
        i_this->m2F8 = 0.0f;
        actor->speedF = 0.0f;
        if (i_this->mpMorf->isStop()) {
            actor->speed.y = l_HIO.m34;
            f32 jump_speed = l_HIO.m30;
            actor->speedF = jump_speed;
            i_this->m2F8 = jump_speed;
            anm_init(i_this, dRes_INDEX_GND_BCK_JUMP2_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f, -1);
            i_this->m2D0 = 22;
        }
        break;
    case 22:
        actor->gravity = l_HIO.m38;
        track = 1;
        if (checkGround(i_this, 0.0f)) {
            anm_init(i_this, dRes_INDEX_GND_BCK_JUMP3_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f, dRes_INDEX_GND_BAS_JUMP3_e);
            i_this->m2D0 = 23;
        }
        break;
    case 23:
        i_this->m2F8 = 0.0f;
        if (i_this->mpMorf->isStop()) {
            i_this->m2D0 = 2;
            wait_set(i_this);
            i_this->m304 = 20;
        }
        break;
    }

    if (track == 1) {
        i_this->m2D4.x = player->current.pos.x;
        i_this->m2D4.y = player->current.pos.y;
        i_this->m2D4.z = player->current.pos.z;
    }
    pos_move(i_this, 0);
    i_this->m2F0 = 2000.0f;
    s16 target = actor->current.angle.y;
    cLib_addCalcAngleS2(&actor->shape_angle.y, target, 2, 0x1000);
}

/* 00001974-000028D0       .text attack0__FP9gnd_class */
static void attack0(gnd_class*) {
    /* Nonmatching */
}

/* 000028D0-00002B68       .text attack1__FP9gnd_class */
static void attack1(gnd_class* i_this) {
    fopAc_ac_c* actor = i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    int frame = (int)i_this->mpMorf->getFrame();
    s8 done = 0;

    f32 rad = 110.0f + REG0_F(13);
    i_this->m13C8 = rad;
    i_this->m13C4 = rad;

    switch (i_this->m2D0) {
    case 0:
        anm_init(i_this, dRes_INDEX_GND_BCK_NOM_ATTACK1_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f, dRes_INDEX_GND_BAS_NOM_ATTACK1_e);
        fopAcM_monsSeStart(actor, JA_SE_CV_GN_ATTACK_4, 0);
        i_this->m2D0 = 5;
        break;
    case 1:
        anm_init(i_this, dRes_INDEX_GND_BCK_NOM_ATTACK2_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f, dRes_INDEX_GND_BAS_NOM_ATTACK2_e);
        fopAcM_monsSeStart(actor, JA_SE_CV_GN_ATTACK_6, 0);
        i_this->m2D0 = 6;
        break;
    case 5:
        if (frame >= 0xF && frame <= 0x12) {
            i_this->m13B8 = 1;
        } else {
            i_this->m2D4.x = player->current.pos.x;
            i_this->m2D4.y = player->current.pos.y;
            i_this->m2D4.z = player->current.pos.z;
        }
        if (i_this->mpMorf->isStop()) {
            done = 1;
        }
        break;
    case 6:
        if (frame >= 0xC && frame <= 0x22) {
            i_this->m13B9 = 1;
        } else {
            i_this->m2D4.x = player->current.pos.x;
            i_this->m2D4.y = player->current.pos.y;
            i_this->m2D4.z = player->current.pos.z;
        }
        if (i_this->mpMorf->isStop()) {
            done = 1;
        }
        break;
    }

    i_this->m2F8 = l_HIO.m14;
    pos_move(i_this, 0);
    s16 target = actor->current.angle.y;
    cLib_addCalcAngleS2(&actor->shape_angle.y, target, 2, 0x1000);
    if (done) {
        i_this->m2CE = 0;
        i_this->m2D0 = 0;
        actor->current.angle.y = actor->shape_angle.y;
        i_this->m304 = 0;
    }
}

/* 00002B68-00002D7C       .text attack2__FP9gnd_class */
static void attack2(gnd_class* i_this) {
    fopAc_ac_c* actor = i_this;
    int frame = (int)i_this->mpMorf->getFrame();
    s8 done = 0;

    f32 rad = 110.0f + REG0_F(13);
    i_this->m13C8 = rad;
    i_this->m13C4 = rad;
    i_this->m13CE = 0;

    switch (i_this->m2D0) {
    case 0:
        anm_init(i_this, dRes_INDEX_GND_BCK_COUNTERL_e, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f, dRes_INDEX_GND_BAS_COUNTERL_e);
        fopAcM_monsSeStart(actor, JA_SE_CV_GN_ATTACK_4, 0);
        i_this->m2D0 = 2;
        break;
    case 1:
        anm_init(i_this, dRes_INDEX_GND_BCK_COUNTERR_e, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f, dRes_INDEX_GND_BAS_COUNTERR_e);
        fopAcM_monsSeStart(actor, JA_SE_CV_GN_ATTACK_1, 0);
        i_this->m2D0 = 3;
        break;
    case 2:
        if (frame >= 4) {
            i_this->m13B8 = 1;
        }
        if (i_this->mpMorf->isStop()) {
            done = 1;
        }
        break;
    case 3:
        if (frame >= 6) {
            i_this->m13B9 = 1;
        }
        if (i_this->mpMorf->isStop()) {
            done = 1;
        }
        break;
    }

    if (done) {
        i_this->m2CE = 0;
        i_this->m2D0 = 0;
        actor->current.angle.y = actor->shape_angle.y;
        i_this->m304 = 0;
    }
}

/* 00002D7C-00003688       .text attackPZ__FP9gnd_class */
static void attackPZ(gnd_class*) {
    /* Nonmatching */
}

/* 00003688-000038B0       .text attack_last__FP9gnd_class */
static void attack_last(gnd_class* i_this) {
    fopAc_ac_c* actor = i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    int frame = (int)i_this->mpMorf->getFrame();

    i_this->m13C8 = 110.0f + REG0_F(13);
    i_this->m13CE = 0;

    switch (i_this->m2D0) {
    case 0:
        anm_init(i_this, dRes_INDEX_GND_BCK_LAST_ATTACK_TAME_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f, -1);
        fopAcM_monsSeStart(actor, JA_SE_CV_GN_ATTACK_7, 0);
        i_this->m2D0 = 1;
        break;
    case 1:
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, dRes_INDEX_GND_BCK_LAST_ATTACK_ATTACK_e, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f, dRes_INDEX_GND_BAS_LAST_ATTACK_ATTACK_e);
            i_this->m2D0 = 2;
        }
        if (frame > l_HIO.m7C) {
            i_this->mNextParryOpeningType = 4;
        }
        break;
    case 2:
        if (frame >= l_HIO.m7E && frame <= l_HIO.m80) {
            i_this->mNextParryOpeningType = 4;
        }
        if (frame >= 6 && frame <= 9) {
            i_this->m13B9 = 1;
        }
        if (i_this->mpMorf->isStop()) {
            i_this->m2CE = 0;
            i_this->m2D0 = 0;
            actor->current.angle.y = actor->shape_angle.y;
            i_this->m304 = 0;
        }
        break;
    }

    if (player->checkLastComboWait()) {
        i_this->m2CE = 30;
        i_this->m2D0 = 0;
        i_this->mAction = 5;
        i_this->m13CE = 0;
    }
}

/* 000038B0-000042D0       .text defence0__FP9gnd_class */
static void defence0(gnd_class*) {
    /* Nonmatching */
}

/* 000042D0-00004330       .text finish__FP9gnd_class */
static void finish(gnd_class* i_this) {
    i_this->mAction = 5;
    i_this->m13CE = 0;
    switch (i_this->m2D0) {
    case 0:
        i_this->m155E = 100;
        i_this->m2D0 = 1;
        break;
    case 1:
        i_this->m2D0 = 2;
        break;
    case 2:
        i_this->m2D0 = 3;
        break;
    }
}

/* 00004330-00004760       .text damage__FP9gnd_class */
static void damage(gnd_class*) {
    /* Nonmatching */
}

/* 00004760-0000501C       .text damage_check__FP9gnd_class */
static void damage_check(gnd_class*) {
    /* Nonmatching */
}

/* 0000501C-0000509C       .text shot_s_sub__FPvPv */
static void* shot_s_sub(void* param_1, void*) {
    if ((fopAcM_IsActor(param_1) && fopAcM_GetName(param_1) == fpcNm_HIMO2_e) ||
        fopAcM_GetName(param_1) == fpcNm_BOOMERANG_e ||
        fopAcM_GetName(param_1) == fpcNm_HOOKSHOT_e ||
        fopAcM_GetName(param_1) == fpcNm_ARROW_e)
    {
        if (((fopAc_ac_c*)param_1)->speedF >= 10.0f) {
            return param_1;
        }
    }
    return NULL;
}

/* 0000509C-000050B0       .text demowait__FP9gnd_class */
static void demowait(gnd_class* i_this) {
    i_this->mAction = 5;
    i_this->m13CE = 0;
}

/* 000050B0-000050C8       .text yawait__FP9gnd_class */
static void yawait(gnd_class* i_this) {
    i_this->current.pos.y = 0.0f;
    i_this->m13CE = 0;
}

/* 000050C8-000054F0       .text gnd_move__FP9gnd_class */
static void gnd_move(gnd_class* i_this) {
    fopAc_ac_c* actor = i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    switch (i_this->m2CE) {
    case 0:
        move0(i_this);
        break;
    case 1:
        attack0(i_this);
        break;
    case 2:
        attack1(i_this);
        break;
    case 3:
        attack2(i_this);
        break;
    case 4:
        attackPZ(i_this);
        break;
    case 5:
        attack_last(i_this);
        break;
    case 10:
        defence0(i_this);
        break;
    case 11:
        damage(i_this);
        break;
    case 20:
        demowait(i_this);
        break;
    case 21:
        yawait(i_this);
        break;
    case 30:
        finish(i_this);
        return;
    }

    if (i_this->m3D8 >= 2 && i_this->m2CE != 11) {
        i_this->m30E = 3;
    }

    if (player->getCutType() == 5 || player->getCutType() == 0xF) {
        if (i_this->m3D8 < 2) {
            i_this->m13CE = 0;
            i_this->m3D7 = 0;
        }
    }

    if (i_this->m13CE) {
        s8 found_near = 0;
        fopAc_ac_c* shot = (fopAc_ac_c*)fpcEx_Search(shot_s_sub, i_this);
        if (shot != NULL) {
            cXyz diff = shot->current.pos - actor->eyePos;
            if (fopAcM_GetName(shot) == fpcNm_ARROW_e) {
                f32 dist = diff.abs();
                if (dist < 10.0f * shot->speedF) {
                    if (((daArrow_c*)shot)->mbSetByZelda && i_this->m3D8 < 2) {
                        s16 ang_diff = actor->shape_angle.y - shot->current.angle.y;
                        if (ang_diff < 0) {
                            ang_diff = -ang_diff;
                        }
                        if ((u16)ang_diff < 0x4000) {
                            i_this->m13CE = 0;
                        } else {
                            i_this->m2CE = 10;
                            if (checkGround(i_this, 50.0f)) {
                                i_this->m2D0 = 15;
                            } else {
                                i_this->m2D0 = 0;
                            }
                            i_this->mAction = 30;
                            i_this->m3DB = 0;
                        }
                    }
                }
            } else {
                f32 dist = diff.abs();
                if (dist < 10.0f * shot->speedF) {
                    found_near = 1;
                }
            }
        }

        f32 player_dist = fopAcM_searchActorDistance(actor, dComIfGp_getPlayer(0));
        if (found_near || (player->getCutType() != 0 && player_dist < l_HIO.m40)) {
            if (i_this->m2CE != 10) {
                i_this->m2CE = 10;
                i_this->m2D0 = 0;
                i_this->m3D5 = 0;
                i_this->m3DB = 0;
                i_this->m3DC = 0;
            }
            i_this->m302 = 10;
        }
    }

    damage_check(i_this);
    i_this->m13CE = 1;

    cXyz* cc_move = i_this->mStts.GetCCMoveP();
    if (cc_move != NULL) {
        actor->current.pos.x += cc_move->x;
        actor->current.pos.y += cc_move->y;
        actor->current.pos.z += cc_move->z;
    }

    if (i_this->m310 > 0.01f) {
        cXyz offset;
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = i_this->m310;
        cMtx_YrotS(*calc_mtx, i_this->m314);
        cXyz out;
        MtxPosition(&offset, &out);
        actor->current.pos += out;
        cLib_addCalc0(&i_this->m310, 1.0f, 7.0f + REG0_F(12));
    }
}

static u32 eff_d[] = {
    0x2F, 0x3C, 0x3D, 0x3E, 0x09, 0x0B, 0x0E, 0x11, 0x0D, 0x10, 0x1D, 0x20, 0x23, 0x1F, 0x22,
};
static u32 fl_check_d[] = {
    0x0A, 0x02, 0x02, 0x05, 0x05, 0x0A, 0x0F, 0x14, 0x0D, 0x12, 0x0A, 0x0F, 0x14, 0x0D, 0x12,
};
static f32 fl_scale[] = {
    1.0f, 2.0f, 2.0f, 2.0f, 2.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.8f, 1.0f, 1.0f, 0.8f, 0.8f, 0.8f,
};

static cXyz f_ctr[] = {
    cXyz(-34.5f, 129.7f, -26.3f),
    cXyz(-37.9f, 202.6f, -77.5f),
    cXyz(-11.5f, 446.0f, -235.0f),
    cXyz(7.3f, 293.0f, 55.9f),
    cXyz(-7.3f, -51.6f, 116.5f),
};
static cXyz f_eye[] = {
    cXyz(101.9f, 38.0f, 550.0f),
    cXyz(-10.0f, 7.0f, -644.0f),
    cXyz(-10.0f, 7.0f, -644.0f),
    cXyz(83.6f, 870.4f, -88.0f),
    cXyz(89.0f, 519.0f, -43.3f),
};

/* 000054F0-00006B04       .text demo_camera__FP9gnd_class */
static void demo_camera(gnd_class* i_this) {
    dComIfGp_setNextStage("GTower", 2, 0, 9, 0.0f, 0, 1, 7);
    JUTReport(0x19A, 0x1AE, "K SUB  COUNT  %d", i_this->m155E);
}

/* 00006B04-00006D0C       .text body_flash__FP9gnd_class */
static void body_flash(gnd_class* i_this) {
    J3DModel* model = i_this->mpMorf->getModel();

    if (i_this->m142E != 0) {
        i_this->m142E++;
        if (i_this->m142E > 100) {
            i_this->m142E = 0;
        }
    }

    for (int i = 0; i < 15; i++) {
        if (i_this->m142E == (int)fl_check_d[i]) {
            i_this->mFlashTimer[i] = 100;
        }

        if (i_this->mFlashTimer[i] != 0) {
            i_this->mFlashTimer[i]--;
            MTXCopy(model->getAnmMtx(eff_d[i]), *calc_mtx);
            cXyz pos;
            MtxPosition(&i_this->m14A8[i], &pos);
            f32 scale = (0.01f + REG8_F(0)) * (i_this->mFlashTimer[i] * fl_scale[i]);
            if (i_this->mpFlashEff[i] == NULL) {
                i_this->mpFlashEff[i] = dComIfGp_particle_set(dPa_name::ID_AK_JN_CCTHUNDER00, &pos);
            } else {
                i_this->mpFlashEff[i]->setGlobalTranslation(pos.x, pos.y, pos.z);
                JGeometry::TVec3<f32> gscale(scale, scale, scale);
                i_this->mpFlashEff[i]->setGlobalScale(gscale);
            }
        } else if (i_this->mpFlashEff[i] != NULL) {
            i_this->mpFlashEff[i]->becomeInvalidEmitter();
            i_this->mpFlashEff[i] = NULL;
        }
    }
}

/* 00006D0C-000076B8       .text daGnd_Execute__FP9gnd_class */
static BOOL daGnd_Execute(gnd_class*) {
    /* Nonmatching */
    return TRUE;
}

/* 000076B8-000076C0       .text daGnd_IsDelete__FP9gnd_class */
static BOOL daGnd_IsDelete(gnd_class*) {
    return TRUE;
}

/* 000076C0-00007738       .text daGnd_Delete__FP9gnd_class */
static BOOL daGnd_Delete(gnd_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "Gnd");
    i_this->mRippleCb.end();
    if (i_this->mHioSet != 0) {
        hio_set = false;
        mDoHIO_deleteChild(l_HIO.mNo);
    }
    return TRUE;
}

/* 00007738-00007A78       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c* a_this) {
    gnd_class* i_this = (gnd_class*)a_this;

    i_this->mpMorf = new mDoExt_McaMorf(
        (J3DModelData*)dComIfG_getObjectRes("Gnd", dRes_INDEX_GND_BDL_GND_e),
        NULL,
        NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes("Gnd", dRes_INDEX_GND_BCK_SAMPLE_e),
        J3DFrameCtrl::EMode_LOOP,
        1.0f,
        0,
        -1,
        1,
        NULL,
        0,
        0x11020203
    );
    if (i_this->mpMorf == NULL || i_this->mpMorf->getModel() == NULL) {
        return FALSE;
    }

    i_this->mpBrkAnm = new mDoExt_brkAnm();
    if (i_this->mpBrkAnm == NULL) {
        return FALSE;
    }
    if (i_this->mpBrkAnm->init(
            i_this->mpMorf->getModel()->getModelData(),
            (J3DAnmTevRegKey*)dComIfG_getObjectRes("Gnd", dRes_INDEX_GND_BRK_SAMPLE_e),
            TRUE,
            J3DFrameCtrl::EMode_LOOP,
            1.0f,
            0,
            -1,
            false,
            0
        ) == 0)
    {
        return FALSE;
    }

    i_this->mpBtkAnm = new mDoExt_btkAnm();
    if (i_this->mpBtkAnm == NULL) {
        return FALSE;
    }
    if (i_this->mpBtkAnm->init(
            i_this->mpMorf->getModel()->getModelData(),
            (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Gnd", dRes_INDEX_GND_BTK_SAMPLE_e),
            TRUE,
            J3DFrameCtrl::EMode_NONE,
            1.0f,
            0,
            -1,
            false,
            0
        ) == 0)
    {
        return FALSE;
    }

    i_this->mpBtpAnm = new mDoExt_btpAnm();
    if (i_this->mpBtpAnm == NULL) {
        return FALSE;
    }
    if (i_this->mpBtpAnm->init(
            i_this->mpMorf->getModel()->getModelData(),
            (J3DAnmTexPattern*)dComIfG_getObjectRes("Gnd", dRes_INDEX_GND_BTP_SAMPLE_e),
            TRUE,
            J3DFrameCtrl::EMode_LOOP,
            1.0f,
            0,
            -1,
            false,
            0
        ) == 0)
    {
        return FALSE;
    }

    return i_this->mLineMat.init(4, 20, FALSE) != 0 ? TRUE : FALSE;
}

/* 00007AC0-00007CFC       .text daGnd_Create__FP10fopAc_ac_c */
static cPhs_State daGnd_Create(fopAc_ac_c* a_this) {
    static dCcD_SrcCyl cc_cyl_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ AT_TYPE_SWORD,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsPlayer_e | cCcD_CoSPrm_VsGrpAll_e,
            /* SrcGObjAt Se      */ 0,
            /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
            /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ 0,
            /* SrcGObjTg Se      */ 0,
            /* SrcGObjTg HitMark */ dCcG_TgHitMark_Nrm_e,
            /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGCylS
        {{
            /* Center */ {0.0f, -150.0f, 0.0f},
            /* Radius */ 100.0f,
            /* Height */ 100.0f,
        }},
    };
    static dCcD_SrcSph head_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ AT_TYPE_SWORD,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e | cCcD_TgSPrm_IsOther_e,
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
            /* Radius */ 15.0f,
        }},
    };
    static dCcD_SrcSph chest_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ AT_TYPE_NORMAL_ARROW | AT_TYPE_LIGHT_ARROW,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e | cCcD_TgSPrm_IsOther_e,
            /* SrcObjCo  SPrm    */ 0,
            /* SrcGObjAt Se      */ 0,
            /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
            /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ 0,
            /* SrcGObjTg Se      */ dCcG_SE_METAL,
            /* SrcGObjTg HitMark */ dCcg_TgHitMark_Purple_e,
            /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_Shield_e | dCcG_TgSPrm_NoConHit_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 100.0f,
        }},
    };
    static dCcD_SrcSph wepon_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_UNK8,
            /* SrcObjAt  Atp     */ 4,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_GrpAll_e,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e | cCcD_TgSPrm_IsOther_e,
            /* SrcObjCo  SPrm    */ 0,
            /* SrcGObjAt Se      */ 0,
            /* SrcGObjAt HitMark */ dCcG_AtHitMark_Nrm_e,
            /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK6,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ dCcG_AtSPrm_NoConHit_e,
            /* SrcGObjTg Se      */ dCcG_SE_METAL,
            /* SrcGObjTg HitMark */ dCcg_TgHitMark_Purple_e,
            /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_Shield_e | dCcG_TgSPrm_NoConHit_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 70.0f,
        }},
    };

    gnd_class* i_this = (gnd_class*)a_this;
    fopAcM_ct(a_this, gnd_class);

    cPhs_State phase = dComIfG_resLoad(&i_this->mPhase, "Gnd");
    if (phase == cPhs_COMPLEATE_e) {
        pz = NULL;
        i_this->m2B4 = fopAcM_GetParam(a_this) & 0xF;
        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x96000)) {
            return cPhs_ERROR_e;
        }

        a_this->attention_info.flags = fopAc_Attn_LOCKON_BATTLE_e;
        a_this->attention_info.distances[fopAc_Attn_TYPE_BATTLE_e] = 4;

        if (!hio_set) {
            i_this->mHioSet = 1;
            hio_set = true;
            l_HIO.mNo = mDoHIO_createChild("ガノンｄ", &l_HIO);
        }

        i_this->initBt(400.0f + REG0_F(6), 150.0f + REG0_F(5));
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
        i_this->mAcch.OffSameActorChk();
        i_this->mAcchCir.SetWall(200.0f, 200.0f);
        i_this->mStts.Init(0xFF, 0xFF, a_this);
        i_this->mBodyCyl.Set(cc_cyl_src);
        i_this->mBodyCyl.SetStts(&i_this->mStts);
        i_this->mHeadSph.Set(head_sph_src);
        i_this->mHeadSph.SetStts(&i_this->mStts);
        i_this->mChestSph.Set(chest_sph_src);
        i_this->mChestSph.SetStts(&i_this->mStts);
        for (int i = 0; i < 2; i++) {
            i_this->mWeponSph[i].Set(wepon_sph_src);
            i_this->mWeponSph[i].SetStts(&i_this->mStts);
        }

        daGnd_Execute(i_this);
        a_this->max_health = 0x4B;
        a_this->health = 0x4B;
        if (i_this->m2B4 != 0) {
            mDoAud_bgmStart(JA_BGM_GANON_BATTLE);
        }
    }

    return phase;
}

static actor_method_class l_daGnd_Method = {
    (process_method_func)daGnd_Create,
    (process_method_func)daGnd_Delete,
    (process_method_func)daGnd_Execute,
    (process_method_func)daGnd_IsDelete,
    (process_method_func)daGnd_Draw,
};

actor_process_profile_definition g_profile_GND = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_GND_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(gnd_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_GND_e,
    /* Actor SubMtd */ &l_daGnd_Method,
    /* Status       */ fopAcStts_UNK40000_e | fopAcStts_BOSS_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
