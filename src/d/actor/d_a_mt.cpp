/**
 * d_a_mt.cpp
 * Enemy - Magtail
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_mt.h"
#include "d/d_com_inf_game.h"
#include "d/d_cc_d.h"
#include "d/d_particle_name.h"
#include "d/d_s_play.h"
#include "d/d_snap.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_path.h"
#include "f_op/f_op_actor_mng.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "res/Object/Mt.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "m_Do/m_Do_hostIO.h"
#include "JSystem/J3DGraphAnimator/J3DNode.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "SSystem/SComponent/c_lib.h"
#include "SSystem/SComponent/c_math.h"

static daMt_HIO_c l_HIO;
static s32 mt_all_count;
static s32 mt_fight_count;
static s32 j_index;

static u16 mt_tex_anm_idx[] = {0x36, 0x37};
static u16 mt_tex_max_frame[] = {6, 1};
static int brk_data[] = {
    dRes_INDEX_MT_BRK_MG_HEAD1_e,
    dRes_INDEX_MT_BRK_MG_BODY1_e,
    dRes_INDEX_MT_BRK_MG_BODY1_e,
    dRes_INDEX_MT_BRK_MG_BODY1_e,
    dRes_INDEX_MT_BRK_MG_BODY1_e,
    dRes_INDEX_MT_BRK_MG_BODY1_e,
    dRes_INDEX_MT_BRK_MG_BODY1_e,
    dRes_INDEX_MT_BRK_MG_TAIL1_e,
};
static int btk_data[] = {
    dRes_INDEX_MT_BTK_MG_HEAD1_e,
    dRes_INDEX_MT_BTK_MG_BODY1_e,
    dRes_INDEX_MT_BTK_MG_BODY1_e,
    dRes_INDEX_MT_BTK_MG_BODY1_e,
    dRes_INDEX_MT_BTK_MG_BODY1_e,
    dRes_INDEX_MT_BTK_MG_BODY1_e,
    dRes_INDEX_MT_BTK_MG_BODY1_e,
    dRes_INDEX_MT_BTK_MG_TAIL1_e,
};
static int move_ad[] = {0, -6, -12, -18, -24, -30, -36, -42};
static int move_ad2[] = {0, -3, -6, -9, -12, -15, -18, -21};
static u8 br_no[] = {0, 1, 1, 2, 2, 2, 1, 0, 0, 0, 0};
static s16 br_ya[] = {-0x32C8, -0x2328, -0x0FA0, 0, 0, 0, 0, 0, 0, 0, 0};

/* 000000EC-000001E8       .text __ct__10daMt_HIO_cFv */
daMt_HIO_c::daMt_HIO_c() {
    m04 = 0;
    m05 = 0;
    m06 = 0;
    m07 = 0;
    m08 = 0x157C;
    m0C = -7500.0f;
    m10 = 15;
    m14 = 0.5f;
    m18 = 21.0f;
    m1C = 0.8f;
    m20 = 0.8f;
    m24 = 450.0f;
    m28 = 350.0f;
    m2C = 30.0f;
    m30 = 17;
    m32 = 40;
    m34 = 400.0f;
    m38 = 45;
    m3C = 43;
    m3E = 47;
    m3A = 70;
    m40 = 55.0f;
    m44 = 30.0f;
    m48 = 25.0f;
    m4C = 0.65f;
    m50 = -5;
    m52 = 0x2AF8;
    m54 = 0x258;
    m58 = 1.0f;
    m5C = 1.0f;
}

/* 00000230-000002D8       .text anm_init__FP8mt_classifUcfi */
void anm_init(mt_class* i_this, int bckIdx, float morf, unsigned char loopMode, float speed, int) {
    i_this->mpMorf[0]->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Mt", bckIdx), loopMode, morf, speed, 0.0f, -1.0f, NULL);
}

/* 000002D8-00000350       .text mt_a_d_sub__FPvPv */
void* mt_a_d_sub(void* ac1, void*) {
    if (fopAcM_IsActor(ac1) && fopAcM_GetName(ac1) == fpcNm_MT_e) {
        mt_class* i_this = (mt_class*)ac1;
        mt_all_count++;
        if (i_this->m454 == 1) {
            mt_fight_count++;
        }
    }
    return NULL;
}

/* 00000350-00000390       .text mt_check__FP8mt_class */
void mt_check(mt_class* i_this) {
    mt_fight_count = 0;
    mt_all_count = 0;
    fpcEx_Search(mt_a_d_sub, i_this);
}

/* 00000390-00000478       .text mt_bg_check__FP8mt_class */
void mt_bg_check(mt_class* i_this) {
    fopAc_ac_c* actor = i_this;
    f32 tmp = 40.0f + REG0_F(3);

    actor->current.pos.y -= tmp;
    actor->old.pos.y -= tmp;
    fopAcM_OnStatus(actor, fopAcStts_FREEZE_e);
    i_this->mAcch.CrrPos(*dComIfG_Bgsp());
    actor->current.pos.y += tmp;
    actor->old.pos.y += tmp;

    if (actor->home.pos.y - actor->current.pos.y > 1000.0f && actor->current.pos.y - i_this->mAcch.GetGroundH() > 5000.0f) {
        i_this->m1CBC = 1;
        fopAcM_delete(actor);
    }
}

/* 00000478-0000053C       .text tex_anm_set__FP8mt_classUs */
void tex_anm_set(mt_class* i_this, unsigned short idx) {
    J3DAnmTexPattern* btp;

    i_this->m345 = 1;
    btp = (J3DAnmTexPattern*)dComIfG_getObjectRes("Mt", mt_tex_anm_idx[idx]);
    i_this->m346 = mt_tex_max_frame[idx];
    i_this->mpBtp = btp;
    i_this->m344 = 0;
    i_this->mpBtp->setFrame(0.0f);

    u16 materialNum = btp->getUpdateMaterialNum();
    for (u16 i = 0; i < materialNum; i++) {
        i_this->mpTexNoAnm[i].setAnmTexPattern(btp);
    }
}

/* 0000053C-000005EC       .text mt_eye_tex_anm__FP8mt_class */
void mt_eye_tex_anm(mt_class* i_this) {
    if (i_this->m464 != 0) {
        i_this->m464--;
    } else {
        i_this->m464 = (s16)(cM_rndF(100.0f) + 50.0f);
        if (i_this->m345 == 0) {
            tex_anm_set(i_this, 0);
        }
    }

    if (i_this->m345 != 0) {
        if (i_this->m344 < i_this->m346) {
            i_this->m344++;
        } else {
            i_this->m345 = 0;
        }
    }
}

/* 000005EC-00000720       .text nodeCallBack_head__FP7J3DNodei */
static BOOL nodeCallBack_head(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        mt_class* i_this = (mt_class*)model->getUserArea();

        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            if (jntNo == 2) {
                cMtx_YrotM(*calc_mtx, -i_this->mJntRot[0].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[0].z);
            } else if (jntNo == 3) {
                cMtx_YrotM(*calc_mtx, i_this->mJntRot[15].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[15].z);
            } else {
                MtxScale(i_this->m18F0, i_this->m18F0, i_this->m18F0, 1);
            }
            MTXCopy(*calc_mtx, model->getAnmMtx(jntNo));
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 0000075C-0000098C       .text nodeCallBack_body__FP7J3DNodei */
static BOOL nodeCallBack_body(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        mt_class* i_this = (mt_class*)model->getUserArea();

        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            if (jntNo == 2) {
                cMtx_YrotM(*calc_mtx, -i_this->mJntRot[j_index + 1].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[j_index + 1].z);
            } else if (jntNo == 3) {
                cMtx_YrotM(*calc_mtx, -i_this->mJntRot[j_index + 2].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[j_index + 2].z);
            } else if (jntNo == 4) {
                cMtx_YrotM(*calc_mtx, i_this->mJntRot[j_index + 16].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[j_index + 16].z);
            } else if (jntNo == 5) {
                cMtx_YrotM(*calc_mtx, i_this->mJntRot[j_index + 17].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[j_index + 17].z);
                j_index += 2;
            }
            MTXCopy(*calc_mtx, model->getAnmMtx(jntNo));
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 0000098C-00000B28       .text nodeCallBack_tail__FP7J3DNodei */
static BOOL nodeCallBack_tail(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        mt_class* i_this = (mt_class*)model->getUserArea();

        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            if (jntNo == 2) {
                cMtx_YrotM(*calc_mtx, -i_this->mJntRot[13].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[13].z);
            } else if (jntNo == 3) {
                cMtx_YrotM(*calc_mtx, -i_this->mJntRot[14].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[14].z);
            } else if (jntNo == 4) {
                cMtx_YrotM(*calc_mtx, i_this->mJntRot[28].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[28].z);
            } else if (jntNo == 5) {
                cMtx_YrotM(*calc_mtx, i_this->mJntRot[29].x);
                cMtx_ZrotM(*calc_mtx, -i_this->mJntRot[29].z);
            }
            MTXCopy(*calc_mtx, model->getAnmMtx(jntNo));
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 00000B28-0000171C       .text body_control2__FP8mt_class */
void body_control2(mt_class* i_this) {
    /* Nonmatching */
    volatile f32 dummy;
    volatile f64 dummy_d;
    dummy = 3.0f;
    dummy = -200.0f;
    dummy = 10.0f;
    dummy_d = 0.5;
    dummy_d = 3.0;
    dummy = -5.0f;
    dummy = 35.0f;
    dummy = 5.0f;
    dummy = 20.0f;
    dummy = 0.01f;
    dummy = 2000.0f;
    dummy = (f32)i_this->m2E8;
    dummy = -10.0f;
    dummy = 32.0f;
    dummy = -17.0f;
    dummy = -30.0f;
    dummy = 2.0f;
    dummy = 60.0f;
    dummy = 2.5f;
    dummy = 0.025f;
    dummy = 80.0f;
    dummy = 3000.0f;
}

/* 00001B54-00001E44       .text wall_check_sub__FP8mt_classP4cXyzP4cXyz */
BOOL wall_check_sub(mt_class* i_this, cXyz* i_start, cXyz* i_end) {
    mt_class* a = i_this;
    cXyz* b = i_start;
    cXyz* c = i_end;
    dBgS_LinChk linChk;

    linChk.Set(b, c, a);
    if (dComIfG_Bgsp()->LineCross(&linChk)) {
        return TRUE;
    }
    return FALSE;
}

/* 00001E44-00001F10       .text body_wall_check__FP8mt_class */
void body_wall_check(mt_class* i_this) {
    for (int i = 1; i < 8; i++) {
        cXyz sp14 = i_this->m500[i];
        sp14.y += 50.0f;
        cXyz sp08 = i_this->m4A0[i];
        sp08.y += 50.0f;
        if (wall_check_sub(i_this, &sp14, &sp08)) {
            i_this->m4A0[i].x = i_this->m500[i].x;
            i_this->m4A0[i].z = i_this->m500[i].z;
        }
    }
}

/* 00001F10-000022D8       .text body_control1__FP8mt_class */
void body_control1(mt_class* i_this) {
    fopAc_ac_c* actor = i_this;

    i_this->m6F4[i_this->mBF4] = actor->current.pos;
    i_this->m9F4[i_this->mBF4] = actor->shape_angle;
    i_this->mB74[i_this->mBF4] = i_this->m468;

    for (int i = 0; i < 8; i++) {
        int idx;
        if (i_this->mC00 != 0) {
            idx = (i_this->mBF4 + move_ad2[i]) & 0x3F;
        } else {
            idx = (i_this->mBF4 + move_ad[i]) & 0x3F;
        }

        J3DModel* model = i_this->mpMorf[i]->getModel();
        model->setBaseScale(actor->scale);

        mDoMtx_stack_c::transS(i_this->m6F4[idx].x, i_this->m6F4[idx].y, i_this->m6F4[idx].z);
        mDoMtx_stack_c::YrotM(i_this->m9F4[idx].y);
        mDoMtx_stack_c::XrotM(i_this->m9F4[idx].x);
        mDoMtx_stack_c::ZrotM(i_this->m9F4[idx].z);
        mDoMtx_stack_c::YrotM(i_this->mB74[idx]);

        if (i == 0) {
            mDoMtx_stack_c::scaleM(l_HIO.m1C, l_HIO.m1C, l_HIO.m1C);
        } else {
            mDoMtx_stack_c::scaleM(i_this->m600[i], i_this->m600[i] * i_this->m620[i], 1.0f);
        }
        mDoMtx_stack_c::transM(0.0f, 0.0f, i_this->m470);
        model->setBaseTRMtx(mDoMtx_stack_c::get());

        if (i == 0) {
            cXyz offset;
            offset.x = 0.0f;
            offset.y = 0.0f;
            offset.z = 30.0f + REG0_F(9);
            mDoMtx_stack_c::multVec(&offset, &actor->eyePos);
            i_this->mEyeSph.SetC(actor->eyePos);
            i_this->mEyeSph.SetR(l_HIO.m44);
            dComIfG_Ccsp()->Set(&i_this->mEyeSph);

            offset.x = 0.0f;
            offset.y = 0.0f;
            offset.z = 100.0f + REG6_F(9);
            cXyz sphPos;
            mDoMtx_stack_c::multVec(&offset, &sphPos);
            i_this->mSph[0].SetC(sphPos);
            i_this->mSph[0].SetR(50.0f);
        } else {
            i_this->mSph[i].SetC(i_this->m6F4[idx]);
            if (i_this->m460 != 0) {
                i_this->mSph[i].SetR(-200.0f);
            } else {
                i_this->mSph[i].SetR(l_HIO.m48);
            }
        }
        dComIfG_Ccsp()->Set(&i_this->mSph[i]);

        if (i_this->mC01 != 0) {
            i_this->m4A0[i] = i_this->m6F4[idx];
            i_this->m560[i] = i_this->m9F4[idx];
        }
    }

    if (i_this->m48E == 0) {
        i_this->mBF4++;
    }
    i_this->mBF4 &= 0x3F;

    if (i_this->mC01 != 0) {
        i_this->mC01 = 0;
        i_this->m454 = 1;
        i_this->m455 = 0;
        i_this->m456 = l_HIO.m10;
        i_this->m48E = 0;
        anm_init(i_this, 10, 20.0f, 2, 1.0f, 0);
    }

    cLib_addCalc2(&i_this->m470, -10.0f, 1.0f, 1.0f);
}

/* 000022D8-000028BC       .text body_control3__FP8mt_class */
void body_control3(mt_class*) {
    /* Nonmatching */
}

/* 000028BC-00002AB0       .text body_control4__FP8mt_class */
void body_control4(mt_class* i_this) {
    cXyz* p4A0 = &i_this->m4A0[1];
    csXyz* p560 = &i_this->m560[1];
    cXyz* p590 = &i_this->m590[1];

    J3DModel* model = i_this->mpMorf[0]->getModel();
    mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    for (int i = 1; i < 8; i++, p4A0++, p560++, p590++) {
        if (i_this->m5F0[i] != 0) {
            i_this->m5F0[i]--;
        } else {
            dComIfGp_particle_setSimple(dPa_name::ID_IT_SN_O_MAGT_HAHEN_B, p4A0);
            p4A0->x += p590->x;
            p4A0->y += p590->y;
            p4A0->z += p590->z;
            p590->y -= 2.5f;
            p560->x += 0x1800;
            p560->y += 0x1000;
            if (p590->y < 0.0f) {
                cLib_addCalc0(&i_this->m600[i], 1.0f, 0.025f);
            }
        }

        J3DModel* model_i = i_this->mpMorf[i]->getModel();
        mDoMtx_stack_c::transS(p4A0->x, p4A0->y, p4A0->z);
        mDoMtx_stack_c::YrotM(p560->y);
        mDoMtx_stack_c::XrotM(p560->x);
        mDoMtx_stack_c::ZrotM(p560->z);
        mDoMtx_stack_c::scaleM(i_this->m600[i], i_this->m600[i], i_this->m600[i]);
        if (i == 7) {
            mDoMtx_stack_c::scaleM(0.0f, 0.0f, 0.0f);
        }
        model_i->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

/* 00002AB0-00003008       .text body_control5__FP8mt_class */
void body_control5(mt_class* i_this) {
    fopAc_ac_c* actor = i_this;

    i_this->m4A0[0] = actor->current.pos;
    i_this->m560[0] = actor->shape_angle;

    cXyz* p4A0 = i_this->m4A0;
    csXyz* p560 = i_this->m560;
    f32 ground_y = l_HIO.m18 + i_this->mAcch.GetGroundH();

    for (int i = 0; i < 8; i++, p4A0++, p560++) {
        if (i > 0) {
            cXyz wave;
            wave.x = i_this->m474 * ((50.0f + REG0_F(4)) * cM_ssin(i_this->m46A * (REG0_S(5) + 3500) + i * (REG0_S(6) + 7000)));
            wave.y = i_this->m474 * ((80.0f + REG0_F(5)) * cM_ssin(i_this->m46A * (REG0_S(7) + 4500) + i * (REG0_S(8) + 6000)));
            wave.z = -30.0f + REG0_F(3);
            cXyz wave_pos;
            mDoMtx_YrotS(*calc_mtx, actor->shape_angle.y);
            MtxPosition(&wave, &wave_pos);

            f32 y = (p4A0->y - 10.0f) + wave_pos.y;
            if (y < ground_y) {
                y = ground_y;
            }

            f32 dx = wave_pos.x + (p4A0->x - p4A0[-1].x);
            f32 dy = y - p4A0[-1].y;
            f32 dz = wave_pos.z + (p4A0->z - p4A0[-1].z);

            int angY = (s16)cM_atan2s(dx, dz);
            f32 dist = std::sqrtf(dx * dx + dz * dz);
            int angX = (s16)-cM_atan2s(dy, dist);

            cXyz offset;
            offset.x = 0.0f;
            offset.y = 0.0f;
            offset.z = 35.0f + REG0_F(7);
            mDoMtx_YrotS(*calc_mtx, angY);
            mDoMtx_XrotM(*calc_mtx, angX);
            cXyz step;
            MtxPosition(&offset, &step);

            p560->y = angY + 0x8000;
            p560->x = -angX;
            p4A0->x = p4A0[-1].x + step.x;
            p4A0->y = p4A0[-1].y + step.y;
            p4A0->z = p4A0[-1].z + step.z;
        }

        J3DModel* model = i_this->mpMorf[i]->getModel();
        model->setBaseScale(actor->scale);

        mDoMtx_stack_c::transS(p4A0->x, p4A0->y, p4A0->z);
        mDoMtx_stack_c::YrotM(p560->y);
        mDoMtx_stack_c::XrotM(p560->x);
        mDoMtx_stack_c::ZrotM(p560->z);
        if (i == 0) {
            mDoMtx_stack_c::YrotM(i_this->m468);
        }
        if (i == 0) {
            mDoMtx_stack_c::scaleM(l_HIO.m1C, l_HIO.m1C, l_HIO.m1C);
        } else {
            mDoMtx_stack_c::scaleM(i_this->m600[i], i_this->m600[i] * i_this->m620[i], 1.0f);
            if (i == 7) {
                mDoMtx_stack_c::scaleM(i_this->m18F0, i_this->m18F0, i_this->m18F0);
            }
        }
        mDoMtx_stack_c::transM(0.0f, 0.0f, i_this->m470);
        model->setBaseTRMtx(mDoMtx_stack_c::get());

        if (i == 0) {
            cXyz offset;
            offset.x = 0.0f;
            offset.y = 0.0f;
            offset.z = 30.0f + REG0_F(9);
            mDoMtx_stack_c::multVec(&offset, &actor->eyePos);
            i_this->mEyeSph.SetC(actor->eyePos);
            i_this->mEyeSph.SetR(30.0f);
            dComIfG_Ccsp()->Set(&i_this->mEyeSph);
        } else {
            i_this->mSph[i].OffTgSetBit();
            i_this->mSph[i].SetC(*p4A0);
        }

        i_this->mSph[i].OffAtSetBit();
        i_this->mSph[i].OffTgSetBit();
        dComIfG_Ccsp()->Set(&i_this->mSph[i]);
    }

    cLib_addCalc2(&i_this->m470, 20.0f, 1.0f, 1.0f);
    i_this->m468 = (s16)(i_this->m474 * ((3000.0f + REG0_F(7)) * cM_ssin(i_this->m46A * (REG0_S(0) + 3000))));
}

/* 00003008-00003210       .text br_draw__FP8mt_class */
void br_draw(mt_class* i_this) {
    fopAc_ac_c* actor = i_this;

    if (i_this->m18D4 == 0) {
        return;
    }

    MtxTrans(actor->eyePos.x, actor->eyePos.y, actor->eyePos.z, 0);
    cMtx_YrotM(*calc_mtx, actor->shape_angle.y);
    cMtx_XrotM(*calc_mtx, actor->shape_angle.x);
    cMtx_ZrotM(*calc_mtx, actor->shape_angle.z);

    f32 scale = l_HIO.m1C * (2.0f + REG0_F(4));
    MtxPush();
    cMtx_YrotM(*calc_mtx, br_ya[i_this->m18D4 - 1]);
    cMtx_XrotM(*calc_mtx, -0x4000);
    MtxScale(scale, scale, scale, 1);

    J3DModel* model = i_this->br_modelL[br_no[i_this->m18D4 - 1]];
    g_env_light.setLightTevColorType(model, &actor->tevStr);
    MTXCopy(*calc_mtx, model->getBaseTRMtx());
    mDoExt_modelUpdateDL(model);
    MtxPull();

    cMtx_ZrotM(*calc_mtx, -0x8000);
    cMtx_YrotM(*calc_mtx, br_ya[i_this->m18D4 - 1]);
    cMtx_XrotM(*calc_mtx, -0x4000);
    MtxScale(scale, scale, scale, 1);

    model = i_this->br_modelR[br_no[i_this->m18D4 - 1]];
    g_env_light.setLightTevColorType(model, &actor->tevStr);
    MTXCopy(*calc_mtx, model->getBaseTRMtx());
    mDoExt_modelUpdateDL(model);
}

/* 00003210-00003360       .text daMt_shadowDraw__FP8mt_class */
static void daMt_shadowDraw(mt_class* i_this) {
    fopAc_ac_c* actor = i_this;

    if (!fopAcM_checkCarryNow(actor)) {
        cXyz pos(actor->current.pos.x, actor->current.pos.y + 150.0f + REG0_F(18), actor->current.pos.z);
        i_this->mShadowId = dComIfGd_setShadow(
            i_this->mShadowId,
            1,
            i_this->mpMorf[0]->getModel(),
            &pos,
            800.0f + REG0_F(19),
            40.0f + REG0_F(17),
            actor->current.pos.y,
            i_this->mAcch.GetGroundH(),
            i_this->mAcch.m_gnd,
            &actor->tevStr,
            0,
            1.0f,
            &dDlst_shadowControl_c::mSimpleTexObj
        );
        for (int i = 1; i < 8; i++) {
            dComIfGd_addRealShadow(i_this->mShadowId, i_this->mpMorf[i]->getModel());
        }
    } else {
        fopAc_ac_c* player = dComIfGp_getLinkPlayer();
        u32 shadowId = *(u32*)((u8*)player + 0x3614);
        if (shadowId != 0) {
            for (int i = 0; i < 8; i++) {
                dComIfGd_addRealShadow(shadowId, i_this->mpMorf[i]->getModel());
            }
        }
    }
}

/* 00003360-0000361C       .text daMt_Draw__FP8mt_class */
static BOOL daMt_Draw(mt_class* i_this) {
    cXyz sp14(0.0f, 0.0f, 0.0f);

    if (i_this->m2BB != 0) {
        return TRUE;
    }

    j_index = 0;
    for (int i = 0; i < 8; i++) {
        J3DModel* model = i_this->mpMorf[i]->getModel();
        if (i_this->m1906 == 0) {
            cXyz sp08;
            MTXMultVec(model->getBaseTRMtx(), &sp14, &sp08);
            g_env_light.settingTevStruct(0, &sp08, &i_this->tevStr);
        } else {
            i_this->scale.x = i_this->scale.y = i_this->scale.z = i_this->m1AA4;
            model->setBaseScale(i_this->scale);
        }

        g_env_light.setLightTevColorType(model, &i_this->tevStr);
        i_this->btk[i]->entry(model->getModelData());
        i_this->brk[i]->entry(model->getModelData());

        if (i_this->m2E4 == 0) {
            int brk_frame = i_this->m2E8 + i * l_HIO.m50;
            while (brk_frame < 0) {
                brk_frame += 41;
            }
            i_this->brk[i]->setFrame(brk_frame);

            int btk_frame = i_this->m2EC + i * l_HIO.m50;
            while (btk_frame < 0) {
                btk_frame += 31;
            }
            i_this->btk[i]->setFrame(btk_frame);
        } else {
            i_this->brk[i]->setFrame(i_this->m2E8);
            i_this->btk[i]->setFrame(i_this->m2EC);
        }

        if (i == 0) {
            model->getModelData()->setTexNoAnimator(i_this->mpBtp, i_this->mpTexNoAnm);
            i_this->mpBtp->setFrame(i_this->m344);
        }

        i_this->mpMorf[i]->updateDL();
    }

    br_draw(i_this);
    daMt_shadowDraw(i_this);
    fopAc_ac_c* actor = i_this;
    dSnap_RegistFig(DSNAP_TYPE_UNKAF, actor, 1.0f, 1.0f, 1.0f);
    return TRUE;
}

/* 0000361C-000037B0       .text bakuha__FP8mt_class */
void bakuha(mt_class* i_this) {
    fopAc_ac_c* actor = i_this;

    fopAcM_createDisappear(actor, &actor->eyePos, 10, daDisItem_IBALL_e);
    if (i_this->m2B6 == 0) {
        if (i_this->m2B9 != 0) {
            dComIfGs_onSwitch(i_this->m2B9, fopAcM_GetRoomNo(actor));
        }
    }

    i_this->m454 = 3;
    i_this->m45A = REG0_S(0) + 57;

    for (int i = 1; i < 8; i++) {
        i_this->m590[i].x = cM_rndFX(30.0f + REG0_F(4));
        i_this->m590[i].y = REG0_F(5) + (20.0f + cM_rndF(10.0f));
        i_this->m590[i].z = cM_rndFX(30.0f + REG0_F(4));
        i_this->m5F0[i] = (s16)cM_rndF(3.0f);
        if (i_this->m18F8 == 3) {
            i_this->m5F0[i] += 5;
        }
    }

    dComIfGp_particle_set(dPa_name::ID_IT_SN_MAGT_HAHEN_A, &actor->current.pos);
}

/* 000037B0-000042C4       .text mt_move__FP8mt_class */
void mt_move(mt_class*) {
    /* Nonmatching */
}

/* 000042C4-00005088       .text mt_fight__FP8mt_class */
void mt_fight(mt_class*) {
    /* Nonmatching */
}

/* 00005088-00005A04       .text mt_move_maru__FP8mt_class */
void mt_move_maru(mt_class*) {
    /* Nonmatching */
}

/* 00005B9C-00005C54       .text water_damage_se_set__FP8mt_class */
void water_damage_se_set(mt_class* i_this) {
    fopAc_ac_c* actor = i_this;
    fopAcM_seStart(actor, JA_SE_CM_MAGTAIL_WATER, 0);
    fopAcM_monsSeStart(actor, JA_SE_CV_MG_WATER, 0);
    i_this->m348 = 1;
}

/* 00005C54-0000614C       .text damage_check__FP8mt_class */
void damage_check(mt_class*) {
    /* Nonmatching */
}

/* 00006188-000074D4       .text daMt_Execute__FP8mt_class */
static BOOL daMt_Execute(mt_class*) {
    /* Nonmatching */
    return TRUE;
}

/* 00007CC4-00007CCC       .text daMt_IsDelete__FP8mt_class */
static BOOL daMt_IsDelete(mt_class*) {
    return TRUE;
}

/* 00007CCC-00007E18       .text daMt_Delete__FP8mt_class */
static BOOL daMt_Delete(mt_class* i_this) {
    fopAc_ac_c* actor = i_this;
    dComIfG_resDelete(&i_this->mPhs, "Mt");

    if (i_this->mp450 != NULL) {
        *(u32*)&i_this->mp450->shape_angle &= ~0x40;
        fopAc_ac_c* other = i_this->mp450;
        *(s32*)((u8*)other + 0x60) = -1;
        *(u32*)&other->shape_angle |= 1;
        i_this->mp450 = NULL;
    }

    for (int i = 0; i < 8; i++) {
        i_this->mPa[i].remove();
    }

    if (i_this->m1CBC != 0 && i_this->m2B6 != 0 && i_this->m2BA != 0 && !dComIfGs_isSwitch(i_this->m2BA, fopAcM_GetRoomNo(actor))) {
        fopAcM_prm_class* append = fopAcM_CreateAppend();
        append->base.position = actor->home.pos;
        append->base.angle = actor->home.angle;
        append->base.parameters = fopAcM_GetParam(actor);
        append->room_no = actor->current.roomNo;
        fpcSCtRq_Request(fpcLy_CurrentLayer(), fpcNm_MT_e, NULL, NULL, append);
    }

    return TRUE;
}

/* 00007E18-00008400       .text CallbackCreateHeap__FP10fopAc_ac_c */
static BOOL CallbackCreateHeap(fopAc_ac_c* i_this) {
    mt_class* actor = (mt_class*)i_this;

    static int bmd_data[] = {
        dRes_INDEX_MT_BDL_MG_HEAD_e,
        dRes_INDEX_MT_BDL_MG_BODY_e,
        dRes_INDEX_MT_BDL_MG_BODY_e,
        dRes_INDEX_MT_BDL_MG_BODY_e,
        dRes_INDEX_MT_BDL_MG_BODY_e,
        dRes_INDEX_MT_BDL_MG_BODY_e,
        dRes_INDEX_MT_BDL_MG_BODY_e,
        dRes_INDEX_MT_BDL_MG_TAIL_e,
    };
    static f32 scale_data[] = {1.0f, 1.0f, 1.0f, 0.975f, 0.925f, 0.825f, 0.75f, 0.525f};

    for (int i = 0; i < 8; i++) {
        actor->mpMorf[i] = new mDoExt_McaMorf(
            (J3DModelData*)dComIfG_getObjectRes("Mt", bmd_data[i]),
            NULL,
            NULL,
            NULL,
            J3DFrameCtrl::EMode_LOOP,
            1.0f,
            0,
            -1,
            1,
            NULL,
            0x80000,
            0x37440402
        );
        if (actor->mpMorf[i] == NULL || actor->mpMorf[i]->getModel() == NULL) {
            return FALSE;
        }

        J3DModel* model = actor->mpMorf[i]->getModel();
        J3DModelData* modelData = model->getModelData();

        actor->btk[i] = new mDoExt_btkAnm();
        JUT_ASSERT(0x11C0, actor->btk[i]);
        if (!actor->btk[i]->init(
                model->getModelData(),
                (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Mt", btk_data[i]),
                TRUE,
                J3DFrameCtrl::EMode_LOOP,
                1.0f,
                0,
                -1,
                false,
                FALSE
            ))
        {
            return FALSE;
        }

        actor->brk[i] = new mDoExt_brkAnm();
        JUT_ASSERT(0x11CD, actor->brk[i]);
        if (!actor->brk[i]->init(
                model->getModelData(),
                (J3DAnmTevRegKey*)dComIfG_getObjectRes("Mt", brk_data[i]),
                TRUE,
                J3DFrameCtrl::EMode_LOOP,
                1.0f,
                0,
                -1,
                false,
                FALSE
            ))
        {
            return FALSE;
        }

        if (i == 0) {
            anm_init(actor, dRes_INDEX_MT_BCK_WAIT1_e, 20.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, 0);

            J3DAnmTexPattern* btp = NULL;
            for (int j = 0; j < 2; j++) {
                btp = (J3DAnmTexPattern*)dComIfG_getObjectRes("Mt", mt_tex_anm_idx[j]);
                btp->searchUpdateMaterialID(model->getModelData());
            }

            u16 materialNum = btp->getUpdateMaterialNum();
            actor->mpTexNoAnm = new J3DTexNoAnm[materialNum];
            for (u16 j = 0; j < materialNum; j++) {
                actor->mpTexNoAnm[j].setAnmIndex(j);
            }
            tex_anm_set(actor, 0);
        }

        model->setUserArea((u32)actor);

        for (u16 jntNo = 0; jntNo < modelData->getJointNum(); jntNo++) {
            if (i == 0) {
                if (jntNo < 2) {
                } else if (jntNo > 5) {
                } else {
                    modelData->getJointNodePointer(jntNo)->setCallBack(nodeCallBack_head);
                }
            } else if (i == 7) {
                if (jntNo < 2) {
                } else if (jntNo > 5) {
                } else {
                    modelData->getJointNodePointer(jntNo)->setCallBack(nodeCallBack_tail);
                }
            } else {
                if (jntNo < 2) {
                } else if (jntNo > 5) {
                } else {
                    modelData->getJointNodePointer(jntNo)->setCallBack(nodeCallBack_body);
                }
            }
        }

        actor->m600[i] = scale_data[i];
    }

    static int br_bmd[] = {
        dRes_INDEX_MT_BDL_KBA_e,
        dRes_INDEX_MT_BDL_KBB_e,
        dRes_INDEX_MT_BDL_KBC_e,
    };

    for (int i = 0; i < 3; i++) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Mt", br_bmd[i]);
        JUT_ASSERT(0x127A, modelData != 0);
        actor->br_modelL[i] = mDoExt_J3DModel__create(modelData, 0, 0x11020203);
        JUT_ASSERT(0x127D, actor->br_modelL[i] != 0);
        actor->br_modelR[i] = mDoExt_J3DModel__create(modelData, 0, 0x11020203);
        JUT_ASSERT(0x127F, actor->br_modelR[i] != 0);
        actor->br_modelL[i]->setBaseScale(actor->scale);
        actor->br_modelR[i]->setBaseScale(actor->scale);
    }

    return TRUE;
}

/* 000084AC-000088A8       .text daMt_Create__FP10fopAc_ac_c */
static cPhs_State daMt_Create(fopAc_ac_c* i_this) {
    fopAcM_SetupActor(i_this, mt_class);
    mt_class* a_this = (mt_class*)i_this;

    static dCcD_SrcSph sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ AT_TYPE_UNK800,
            /* SrcObjAt  Atp     */ 1,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e,
            /* SrcObjTg  Type    */ AT_TYPE_ALL,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsPlayer_e | cCcD_CoSPrm_VsGrpAll_e,
            /* SrcGObjAt Se      */ dCcG_SE_UNK2,
            /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
            /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
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
            /* Radius */ 30.0f,
        }},
    };
    static dCcD_SrcSph eye_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e,
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
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 30.0f,
        }},
    };

    cPhs_State phase_state = dComIfG_resLoad(&a_this->mPhs, "Mt");
    if (phase_state == cPhs_COMPLEATE_e) {
        i_this->gbaName = 5;

        a_this->m2B4 = fopAcM_GetParam(i_this);
        if (a_this->m2B4 == 0xFF) {
            a_this->m2B4 = 0;
        }
        a_this->m2B5 = (fopAcM_GetParam(i_this) >> 8) & 0x7F;
        a_this->m2B6 = (fopAcM_GetParam(i_this) >> 15) & 1;
        a_this->m2B7 = fopAcM_GetParam(i_this) >> 16;
        a_this->m2B8 = fopAcM_GetParam(i_this) >> 24;

        if (a_this->m2B6 == 0) {
            a_this->m2B9 = i_this->current.angle.z;
            if (a_this->m2B9 != 0) {
                if (dComIfGs_isSwitch(a_this->m2B9, fopAcM_GetRoomNo(i_this))) {
                    return cPhs_ERROR_e;
                }
            }
        } else {
            a_this->m2BA = i_this->current.angle.z;
        }
        i_this->current.angle.z = 0;

        i_this->itemTableIdx = dComIfGp_CharTbl()->GetNameIndex("magtail", 0);

        if (!fopAcM_entrySolidHeap(i_this, CallbackCreateHeap, 0x1BFC8)) {
            return cPhs_ERROR_e;
        }

        a_this->initBt(162.5f, 200.0f);

        if (a_this->m2B4 >= 10) {
            switch (a_this->m2B5) {
            case 1:
                a_this->m488 = 1000;
                break;
            case 2:
                a_this->m488 = 500;
                break;
            case 3:
                a_this->m488 = 250;
                break;
            case 11:
                a_this->m488 = -1000;
                break;
            case 12:
                a_this->m488 = -500;
                break;
            case 13:
                a_this->m488 = -250;
                break;
            }
        } else if (a_this->m2B7 != 0xFF) {
            a_this->mpPath = dPath_GetRoomPath(a_this->m2B7, fopAcM_GetRoomNo(i_this));
            if (a_this->mpPath == NULL) {
                return cPhs_ERROR_e;
            }
            a_this->m2BC = a_this->m2B7 + 1;
            a_this->m2BE = 1;
            dPnt* pnt = a_this->mpPath->m_points;
            a_this->m47C.x = pnt->m_position.x;
            a_this->m47C.y = pnt->m_position.y;
            a_this->m47C.z = pnt->m_position.z;
        }

        if (a_this->m2B8 != 0xFF) {
            a_this->m2BB = a_this->m2B8 + 1;
        }

        i_this->cullMtx = a_this->mpMorf[1]->getModel()->getBaseTRMtx();
        i_this->cull.box.min.x = -200.0f;
        i_this->cull.box.min.y = -200.0f;
        i_this->cull.box.min.z = -200.0f;
        i_this->cull.box.max.x = 200.0f;
        i_this->cull.box.max.y = 200.0f;
        i_this->cull.box.max.z = 200.0f;
        i_this->gravity = -3.0f;
        a_this->m46A = cM_rndF(32768.0f);

        for (int i = 0; i < 0x40; i++) {
            a_this->m6F4[i] = i_this->current.pos;
            a_this->m9F4[i] = i_this->current.angle;
        }

        a_this->mAcch.Set(&i_this->current.pos, &i_this->old.pos, i_this, 1, &a_this->mAcchCir, &i_this->speed, NULL, NULL);
        a_this->mAcchCir.SetWall(50.0f, 19.0f + REG0_F(0));
        a_this->mStts.Init(250, 2, i_this);

        for (int i = 0; i < 8; i++) {
            a_this->mSph[i].Set(sph_src);
            a_this->mSph[i].SetStts(&a_this->mStts);
        }
        a_this->mSph[0].SetAtAtp(2);

        a_this->mEyeSph.Set(eye_sph_src);
        a_this->mEyeSph.SetStts(&a_this->mStts);

        a_this->m18F0 = 1.0f;
        a_this->m18FB = 2;
        i_this->max_health = 8;
        i_this->health = 8;
        a_this->mp1900 = i_this;
        a_this->m1AB1 = a_this->m2B9;

        daMt_Execute(a_this);
    }
    return phase_state;
}

static actor_method_class l_daMt_Method = {
    (process_method_func)daMt_Create,
    (process_method_func)daMt_Delete,
    (process_method_func)daMt_Execute,
    (process_method_func)daMt_IsDelete,
    (process_method_func)daMt_Draw,
};

actor_process_profile_definition g_profile_MT = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_MT_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(mt_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_MT_e,
    /* Actor SubMtd */ &l_daMt_Method,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_FREEZE_e | fopAcStts_UNK40000_e | fopAcStts_UNK8000000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
