/**
 * d_a_gnd.cpp
 * Boss - Ganondorf
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_gnd.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_demo.h"
#include "d/d_particle_name.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_audio.h"
#include "m_Do/m_Do_hostIO.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "res/Object/Gnd.h"

static daGnd_HIO_c l_HIO;
static bool hio_set;

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
static BOOL daGnd_Draw(gnd_class*) {
    /* Nonmatching */
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
static void ke_control(gnd_class*, gnd_ke_s*, f32) {
    /* Nonmatching */
}

/* 00000F24-00000F68       .text ke_pos_set__FP9gnd_classP8gnd_ke_si */
static void ke_pos_set(gnd_class* i_this, gnd_ke_s* param_2, int param_3) {
    cXyz* pos = i_this->mLineMat.getPos(param_3);
    for (s32 i = 0; i < 20; pos++, i++) {
        *pos = param_2->mPos[i];
    }
}

/* 00000F68-00001140       .text ke_move__FP9gnd_class */
static void ke_move(gnd_class*) {
    /* Nonmatching */
}

/* 00001140-00001278       .text pos_move__FP9gnd_classSc */
static void pos_move(gnd_class*, s8) {
    /* Nonmatching */
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
static void move0(gnd_class*) {
    /* Nonmatching */
}

/* 00001974-000028D0       .text attack0__FP9gnd_class */
static void attack0(gnd_class*) {
    /* Nonmatching */
}

/* 000028D0-00002B68       .text attack1__FP9gnd_class */
static void attack1(gnd_class*) {
    /* Nonmatching */
}

/* 00002B68-00002D7C       .text attack2__FP9gnd_class */
static void attack2(gnd_class*) {
    /* Nonmatching */
}

/* 00002D7C-00003688       .text attackPZ__FP9gnd_class */
static void attackPZ(gnd_class*) {
    /* Nonmatching */
}

/* 00003688-000038B0       .text attack_last__FP9gnd_class */
static void attack_last(gnd_class*) {
    /* Nonmatching */
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
    if (fopAcM_IsActor(param_1) && fopAcM_GetName(param_1) == fpcNm_HIMO2_e) {
        goto check_speed;
    }
    s16 name = fopAcM_GetName(param_1);
    if (name == fpcNm_BOOMERANG_e) {
        goto check_speed;
    }
    if (name == fpcNm_HOOKSHOT_e) {
        goto check_speed;
    }
    if (name != fpcNm_ARROW_e) {
        return NULL;
    }
check_speed:
    if (((fopAc_ac_c*)param_1)->speedF >= 10.0f) {
        return param_1;
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
static void gnd_move(gnd_class*) {
    /* Nonmatching */
}

/* 000054F0-00006B04       .text demo_camera__FP9gnd_class */
static void demo_camera(gnd_class*) {
    /* Nonmatching */
}

/* 00006B04-00006D0C       .text body_flash__FP9gnd_class */
static void body_flash(gnd_class*) {
    /* Nonmatching */
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
static BOOL useHeapInit(fopAc_ac_c*) {
    /* Nonmatching */
    return TRUE;
}

/* 00007AC0-00007CFC       .text daGnd_Create__FP10fopAc_ac_c */
static cPhs_State daGnd_Create(fopAc_ac_c*) {
    /* Nonmatching */
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
    return cPhs_ERROR_e;
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
