#ifndef D_A_NZ_H
#define D_A_NZ_H

#include "f_op/f_op_actor.h"
#include "d/d_cc_d.h"
#include "d/d_bg_s_acch.h"
#include "d/d_particle.h"
#include "c/c_damagereaction.h"
#include "SSystem/SComponent/c_phase.h"
#include "m_Do/m_Do_hostIO.h"
#include "m_Do/m_Do_ext.h"

class J3DMaterialTable;

class nz_class : public fopAc_ac_c {
public:
    /* 0x290 */ u8 m290[0x2AC - 0x290];
    /* 0x2AC */ request_of_phase_process_class mPhase;
    /* 0x2B4 */ u8 m2B4;
    /* 0x2B5 */ u8 m2B5;
    /* 0x2B6 */ u8 m2B6;
    /* 0x2B7 */ u8 m2B7;
    /* 0x2B8 */ mDoExt_McaMorf* mpMorf;
    /* 0x2BC */ u8 m2BC;
    /* 0x2BD */ u8 m2BD;
    /* 0x2BE */ u8 m2BE[0x2C4 - 0x2BE];
    /* 0x2C4 */ fpc_ProcID mHeldID;
    /* 0x2C8 */ fpc_ProcID mNzgID;
    /* 0x2CC */ u8 m2CC[0x2E2 - 0x2CC];
    /* 0x2E2 */ s16 m2E2;
    /* 0x2E4 */ int m2E4;
    /* 0x2E8 */ int mCurrBckIdx;
    /* 0x2EC */ u8 m2EC[0x2F8 - 0x2EC];
    /* 0x2F8 */ csXyz mHeadRot;
    /* 0x2FE */ u8 m2FE[0x31C - 0x2FE];
    /* 0x31C */ cXyz m31C;
    /* 0x328 */ cXyz mHandPos0;
    /* 0x334 */ cXyz mHandPos1;
    /* 0x340 */ f32 mYOffset;
    /* 0x344 */ u8 m344[0x368 - 0x344];
    /* 0x368 */ cXyz m368[8];
    /* 0x3C8 */ cXyz m3C8[8];
    /* 0x428 */ u8 m428[0x42B - 0x428];
    /* 0x42B */ u8 m42B;
    /* 0x42C */ u8 m42C[0x430 - 0x42C];
    /* 0x430 */ mDoExt_3DlineMat1_c mLineMat;
    /* 0x46C */ cXyz mTailPos[10];
    /* 0x4E4 */ cXyz mTailDir[10];
    /* 0x55C */ cXyz mTailRoot[2];
    /* 0x574 */ cXyz m574;
    /* 0x580 */ s16 m580;
    /* 0x582 */ u8 m582[0x598 - 0x582];
    /* 0x598 */ cXyz mSmokePos;
    /* 0x5A4 */ csXyz mSmokeRot;
    /* 0x5AA */ u8 m5AA[0x5AC - 0x5AA];
    /* 0x5AC */ dPa_rippleEcallBack mRippleCb;
    /* 0x5C0 */ dPa_smokeEcallBack mSmokeCb;
    /* 0x5E0 */ dPa_followEcallBack mFollowCb;
    /* 0x5F4 */ J3DMaterialTable* mpBmt;
    /* 0x5F8 */ dBgS_AcchCir mAcchCir;
    /* 0x638 */ dBgS_ObjAcch mAcch;
    /* 0x7FC */ dCcD_Stts mStts;
    /* 0x838 */ dCcD_Cyl mCyl;
    /* 0x968 */ u8 m968[0x979 - 0x968];
    /* 0x979 */ u8 m979;
    /* 0x97A */ u8 m97A[0x97C - 0x97A];
    /* 0x97C */ fopAc_ac_c* m97C;
    /* 0x980 */ u8 m980[0x9AC - 0x980];
    /* 0x9AC */ dCcD_Stts mStts2;
    /* 0x9E8 */ dCcD_Cyl mCyl2;
    /* 0xB18 */ f32 mB18;
    /* 0xB1C */ f32 mB1C;
    /* 0xB20 */ u8 mB20[0xB30 - 0xB20];
    /* 0xB30 */ dBgS_AcchCir mAcchCir2;
    /* 0xB70 */ dBgS_ObjAcch mAcch2;
    /* 0xD34 */ enemyfire mEnemyFire;
    /* 0xF5C */ mDoExt_invisibleModel mInvModel;
};

STATIC_ASSERT(sizeof(nz_class) == 0xF64);

class daNZ_HIO_c : public mDoHIO_entry_c {
public:
    daNZ_HIO_c();
    virtual ~daNZ_HIO_c() {}

public:
    /* 0x04 */ f32 m04;
    /* 0x08 */ f32 m08;
    /* 0x0C */ f32 m0C;
    /* 0x10 */ f32 m10;
    /* 0x14 */ f32 m14;
};

STATIC_ASSERT(sizeof(daNZ_HIO_c) == 0x18);

#endif /* D_A_NZ_H */
