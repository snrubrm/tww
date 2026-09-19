#ifndef D_A_GY_H
#define D_A_GY_H

#include "f_op/f_op_actor.h"
#include "d/d_cc_d.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_bg_s_acch.h"
#include "d/d_particle.h"
#include "m_Do/m_Do_hostIO.h"

class daGy_Ctrl_c;
class JntHit_c;
class J3DNode;
class J3DModel;

class daGy_c : public fopEn_enemy_c {
public:
    void _nodeControl(J3DNode*, J3DModel*);
    BOOL _createHeap();
    void setMtx();
    void setAnm();
    void setAtCollision();
    void setCollision();
    void setAimSpeedF();
    void modeDiveInit();
    void modeDive();
    void modeCircleInit();
    void modeCircle();
    void modeWithCircleInit();
    void modeWithCircle();
    void modeAttackInit();
    void modeWithAttackInit();
    void modeWithAttack();
    void modeAttack();
    void modeAttackPlayerInit();
    void modeAttackPlayer();
    void modeAttackBackInit();
    void modeAttackBack();
    void modeDamageInit();
    void modeDamage();
    void modeDeleteInit();
    void modeDelete();
    void modeDeleteBombInit();
    void modeDeleteBomb();
    void modeProcCall();
    void createWave();
    void setWave();
    void lineCheck(cXyz*, cXyz*);
    void checkTgHit();
    void getWaterY();
    bool _execute();
    void drawDebug();
    bool _draw();
    void createInit();
    cPhs_State _create();
    bool _delete();

    static const u32 m_heapsize;
    static const char m_arc_name[];

public:
    /* 0x2AC */ u8 m2AC[0x2B0 - 0x2AC];
    /* 0x2B0 */ s32 m2B0;
    /* 0x2B4 */ u8 m2B4[0x2B8 - 0x2B4];
    /* 0x2B8 */ daGy_Ctrl_c* mpCtrl;
    /* 0x2BC */ u8 m2BC[0x2D4 - 0x2BC];
    /* 0x2D4 */ mDoExt_McaMorf* mpMorf;
    /* 0x2D8 */ mDoExt_invisibleModel m2D8;
    /* 0x2E0 */ dBgS_Acch mAcch;
    /* 0x4A4 */ dBgS_AcchCir mAcchCir;
    /* 0x4E4 */ f32 m4E4;
    /* 0x4E8 */ u8 m4E8[0x4F0 - 0x4E8];
    /* 0x4F0 */ f32 mAimSpeedF;
    /* 0x4F4 */ u8 m4F4[0x508 - 0x4F4];
    /* 0x508 */ f32 m508;
    /* 0x50C */ dCcD_GObjInf m50C;
    /* 0x604 */ u8 m604[0x624 - 0x604];
    /* 0x624 */ cM3dGSph m624;
    /* 0x638 */ u8 m638[0x8C4 - 0x638];
    /* 0x8C4 */ dCcD_GStts mGStts;
    /* 0x8E4 */ u32 m8E4;
    /* 0x8E8 */ u8 m8E8[0x924 - 0x8E8];
    /* 0x924 */ s32 m924;
    /* 0x928 */ s32 m928;
    /* 0x92C */ u8 m92C[0x978 - 0x92C];
    /* 0x978 */ dCcD_GStts m978;
    /* 0x998 */ dCcD_GObjInf m998;
    /* 0xA90 */ u8 mA90[0xAE0 - 0xA90];
    /* 0xAE0 */ dBgS_AcchCir mAE0;
    /* 0xB20 */ dBgS_Acch mB20;
    /* 0xCE4 */ JntHit_c* mpJntHit;
    /* 0xCE8 */ u8 mCE8[0xD08 - 0xCE8];
    /* 0xD08 */ cXyz mD08;
    /* 0xD14 */ s8 mAnmIdx;
    /* 0xD15 */ s8 mPrmIdx;
    /* 0xD16 */ s8 mOldPrmIdx;
    /* 0xD18 */ dPa_waveEcallBack mD18;
    /* 0xD7C */ dPa_waveEcallBack mD7C;
    /* 0xDE0 */ dPa_splashEcallBack mDE0;
    /* 0xDFC */ cXyz mDFC;
    /* 0xE08 */ u8 mE08[0xE14 - 0xE08];
    /* 0xE14 */ dBgS_LinChk mLinChk;
    /* 0xE80 */ u8 mE80;
    /* 0xE81 */ u8 mE81[0xE84 - 0xE81];
    /* 0xE84 */ cXyz mE84;
}; // size = 0xE90

class daGy_HIO_c : public mDoHIO_entry_c {
public:
    daGy_HIO_c();
    virtual ~daGy_HIO_c() {}

public:
    /* 0x04 */ u8 m04[0x48 - 0x04];
    /* 0x48 */ f32 m48;
    /* 0x4C */ f32 m4C;
    /* 0x50 */ f32 m50;
    /* 0x54 */ f32 m54;
    /* 0x58 */ u8 m58[0x94 - 0x58];
    /* 0x94 */ u8 m94;
    /* 0x95 */ u8 m95[0xAC - 0x95];
    /* 0xAC */ f32 mAC;
    /* 0xB0 */ u8 mB0[0xB8 - 0xB0];
    /* 0xB8 */ f32 mB8;
    /* 0xBC */ f32 mBC;
    /* 0xC0 */ f32 mC0;
    /* 0xC4 */ u8 mC4[0xCC - 0xC4];
    /* 0xCC */ f32 mCC;
    /* 0xD0 */ f32 mD0;
    /* 0xD4 */ u8 mD4[0xEC - 0xD4];
    /* 0xEC */ f32 mEC;
    /* 0xF0 */ f32 mF0;
    /* 0xF4 */ f32 mF4;
    /* 0xF8 */ f32 mF8;
    /* 0xFC */ f32 mFC;
    /* 0x100 */ f32 m100;
    /* 0x104 */ u8 m104[0x19C - 0x104];
}; // size = 0x19C

#endif /* D_A_GY_H */
