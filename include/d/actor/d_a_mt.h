#ifndef D_A_MT_H
#define D_A_MT_H

#include "f_op/f_op_actor.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_particle.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_hostIO.h"
#include "SSystem/SComponent/c_phase.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"

class daMt_HIO_c : public mDoHIO_entry_c {
public:
    daMt_HIO_c();
    virtual ~daMt_HIO_c() {}

    void genMessage(JORMContext* ctx) { UNUSED(ctx); }

public:
    /* 0x04 */ u8 m04;
    /* 0x05 */ u8 m05;
    /* 0x06 */ u8 m06;
    /* 0x07 */ u8 m07;
    /* 0x08 */ s16 m08;
    /* 0x0C */ f32 m0C;
    /* 0x10 */ s16 m10;
    /* 0x14 */ f32 m14;
    /* 0x18 */ f32 m18;
    /* 0x1C */ f32 m1C;
    /* 0x20 */ f32 m20;
    /* 0x24 */ f32 m24;
    /* 0x28 */ f32 m28;
    /* 0x2C */ f32 m2C;
    /* 0x30 */ s16 m30;
    /* 0x32 */ s16 m32;
    /* 0x34 */ f32 m34;
    /* 0x38 */ s16 m38;
    /* 0x3A */ s16 m3A;
    /* 0x3C */ s16 m3C;
    /* 0x3E */ s16 m3E;
    /* 0x40 */ f32 m40;
    /* 0x44 */ f32 m44;
    /* 0x48 */ f32 m48;
    /* 0x4C */ f32 m4C;
    /* 0x50 */ s16 m50;
    /* 0x52 */ s16 m52;
    /* 0x54 */ s16 m54;
    /* 0x58 */ f32 m58;
    /* 0x5C */ f32 m5C;
};  // Size: 0x60

STATIC_ASSERT(sizeof(daMt_HIO_c) == 0x60);

class mt_class : public fopEn_enemy_c {
public:
    /* 0x02AC */ request_of_phase_process_class mPhs;
    /* 0x02B4 */ u8 m2B4;
    /* 0x02B5 */ u8 m2B5;
    /* 0x02B6 */ u8 m2B6;
    /* 0x02B7 */ u8 m2B7;
    /* 0x02B8 */ u8 m2B8;
    /* 0x02B9 */ u8 m2B9;
    /* 0x02BA */ u8 m2BA;
    /* 0x02BB */ u8 m2BB;
    /* 0x02BC */ u8 m2BC[0x02C4 - 0x02BC];
    /* 0x02C4 */ mDoExt_McaMorf* mpMorf[8];
    /* 0x02E4 */ u8 m2E4;
    /* 0x02E5 */ u8 m2E5[0x02E8 - 0x02E5];
    /* 0x02E8 */ int m2E8;
    /* 0x02EC */ int m2EC;
    /* 0x02F0 */ mDoExt_btkAnm* mpBtk[8];
    /* 0x0310 */ mDoExt_brkAnm* mpBrk[8];
    /* 0x0330 */ u8 m330[0x033C - 0x0330];
    /* 0x033C */ J3DAnmTexPattern* mpBtp;
    /* 0x0340 */ J3DTexNoAnm* mpTexNoAnm;
    /* 0x0344 */ u8 m344;
    /* 0x0345 */ u8 m345;
    /* 0x0346 */ u8 m346;
    /* 0x0347 */ u8 m347;
    /* 0x0348 */ u8 m348;
    /* 0x0349 */ u8 m349[0x0350 - 0x0349];
    /* 0x0350 */ cXyz m350[8];
    /* 0x03B0 */ dPa_followEcallBack mPa[8];
    /* 0x0450 */ fopAc_ac_c* mp450;
    /* 0x0454 */ u8 m454;
    /* 0x0455 */ u8 m455;
    /* 0x0456 */ u8 m456[0x0460 - 0x0456];
    /* 0x0460 */ s16 m460;
    /* 0x0462 */ u8 m462[0x0464 - 0x0462];
    /* 0x0464 */ s16 m464;
    /* 0x0466 */ u8 m466[0x04A0 - 0x0466];
    /* 0x04A0 */ cXyz m4A0[8];
    /* 0x0500 */ cXyz m500[8];
    /* 0x0560 */ csXyz m560[8];
    /* 0x0590 */ cXyz m590[8];
    /* 0x05F0 */ u8 m5F0[0x0640 - 0x05F0];
    /* 0x0640 */ csXyz mJntRot[0x1E];
    /* 0x06F4 */ cXyz m6F4[0x40];
    /* 0x09F4 */ csXyz m9F4[0x40];
    /* 0x0B74 */ u8 mB74[0x0C08 - 0x0B74];
    /* 0x0C08 */ dBgS_AcchCir mAcchCir;
    /* 0x0C48 */ dBgS_ObjAcch mAcch;
    /* 0x0E0C */ dCcD_Stts mStts;
    /* 0x0E48 */ dCcD_Sph mSph[8];
    /* 0x17A8 */ dCcD_Sph mEyeSph;
    /* 0x18D4 */ u8 m18D4;
    /* 0x18D5 */ u8 m18D5[0x18D8 - 0x18D5];
    /* 0x18D8 */ J3DModel* mpBrModel[3];
    /* 0x18E4 */ J3DModel* mpBrModel2[3];
    /* 0x18F0 */ f32 m18F0;
    /* 0x18F4 */ u8 m18F4[0x1906 - 0x18F4];
    /* 0x1906 */ s8 m1906;
    /* 0x1907 */ u8 m1907[0x1930 - 0x1907];
    /* 0x1930 */ dCcD_Stts mStts2;
    /* 0x196C */ dCcD_Cyl mCyl;
    /* 0x1A9C */ u8 m1A9C[0x1AA4 - 0x1A9C];
    /* 0x1AA4 */ f32 m1AA4;
    /* 0x1AA8 */ u8 m1AA8[0x1AB4 - 0x1AA8];
    /* 0x1AB4 */ dBgS_AcchCir mAcchCir2;
    /* 0x1AF4 */ dBgS_ObjAcch mAcch2;
    /* 0x1CB8 */ u32 mShadowId;
    /* 0x1CBC */ s8 m1CBC;
};  // Size: 0x1CC0

STATIC_ASSERT(sizeof(mt_class) == 0x1CC0);

#endif /* D_A_MT_H */
