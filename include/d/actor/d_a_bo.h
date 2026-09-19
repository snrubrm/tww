#ifndef D_A_BO_H
#define D_A_BO_H

#include "f_op/f_op_actor.h"
#include "c/c_damagereaction.h"
#include "SSystem/SComponent/c_bg_s_gnd_chk.h"
#include "SSystem/SComponent/c_phase.h"

class mDoExt_McaMorf;
class mDoExt_brkAnm;

class yodare_ato_PcallBack_c : public dPa_smokePcallBack {
public:
    void execute(JPABaseEmitter*, JPABaseParticle*);

public:
    /* 0x04 */ cBgS_GndChk mGndChk;
};

class bo_class : public fopEn_enemy_c {
public:
    /* 0x2AC */ request_of_phase_process_class mPhase;
    /* 0x2B4 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x2B8 */ mDoExt_McaMorf* mpMorfUP;
    /* 0x2BC */ mDoExt_McaMorf* mpMorfDW;
    /* 0x2C0 */ u8 m2C0;
    /* 0x2C1 */ u8 m2C1[0x2DC - 0x2C1];
    /* 0x2DC */ int mCurrBckIdx;
    /* 0x2E0 */ u8 m2E0[0x304 - 0x2E0];
    /* 0x304 */ cXyz m304;
    /* 0x310 */ u8 m310[0x334 - 0x310];
    /* 0x334 */ csXyz m334;
    /* 0x33A */ csXyz m33A;
    /* 0x340 */ csXyz m340;
    /* 0x346 */ u8 m346[0x348 - 0x346];
    /* 0x348 */ s16 m348;
    /* 0x34A */ u8 m34A[0x352 - 0x34A];
    /* 0x352 */ csXyz m352;
    /* 0x358 */ u8 m358[0x394 - 0x358];
    /* 0x394 */ f32 m394;
    /* 0x398 */ u8 m398[0x39C - 0x398];
    /* 0x39C */ cXyz m39C;
    /* 0x3A8 */ u8 m3A8[0x3B0 - 0x3A8];
    /* 0x3B0 */ dPa_smokeEcallBack mSmokeCb;
    /* 0x3D0 */ JPABaseEmitter* m3D0;
    /* 0x3D4 */ JPABaseEmitter* m3D4;
    /* 0x3D8 */ u8 m3D8[0x9FC - 0x3D8];
    /* 0x9FC */ enemyice mEnemyIce;
    /* 0xDB4 */ enemyfire mEnemyFire;
    /* 0xFDC */ u8 mFDC[0xFE4 - 0xFDC];
};

STATIC_ASSERT(sizeof(bo_class) == 0xFE4);

#endif /* D_A_BO_H */
