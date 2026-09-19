#ifndef D_A_NPC_P2_H
#define D_A_NPC_P2_H

#include "d/d_npc.h"
#include "d/d_particle.h"
#include "m_Do/m_Do_hostIO.h"
#include "SSystem/SComponent/c_phase.h"

class daNpc_P2_c : public fopAc_ac_c {
public:
    typedef int (daNpc_P2_c::*ActionFunc)(void*);

    cXyz getAttentionBasePos() { return mAttnPos; }
    s16 getBackbone_x() { return mJnt.getBackbone_x(); }
    s16 getBackbone_y() { return mJnt.getBackbone_y(); }
    cXyz getEyePos() { return mEyePos; }
    s16 getHead_x() { return mJnt.getHead_x(); }
    s16 getHead_y() { return mJnt.getHead_y(); }
    void incAttnSetCount() {
        if (mAttnSetCount != 0xFF) {
            mAttnSetCount++;
        }
    }
    void setAction(ActionFunc, void*) {}
    void setEyeStopFlag() { mEyeStopFlag = true; }

    BOOL initTexPatternAnm(bool);
    void playTexPatternAnm();
    void setAnm();
    void setTexAnm();
    void setAttention();
    void chkAttention();
    void lookBack();
    void setMtx();
    void setCollision();
    void smoke_set();
    u16 next_msgStatus(unsigned long*);
    u32 getMsg();
    void talkInit();
    void anmAtr(unsigned short);
    u16 talk(bool);
    void eventOrder();
    void checkOrder();
    void demo_wait_2();
    void demo_intro_2();
    void goal_wait_2();
    void demo_goal_2();
    void demo_bomb_get();
    void demo_wait();
    void demo_intro();
    void demo_lift();
    void demo_jump();
    void goal_goalpos_to_talkpos();
    void goal_talkpos_to_goalpos();
    void goal_goalpos_wait();
    void goal_talkpos_wait();
    void demo_goal();
    void goal_talkpos_talk();
    void treasure_wait();
    void treasure_wait_talk();
    void demo_arrive();
    void wait01();
    void zukotelescope();
    void moccowait();
    void talk01();
    int intro_action(void*);
    int wait_action(void*);
    bool _execute();
    void draw_item(J3DModel*, signed char);
    void drawDagger();
    void drawHead();
    void drawShadow();
    void drawP2a();
    void drawP2b();
    void drawP2c();
    bool _draw();
    void getArg();
    BOOL _createHeap();
    void createInit();
    cPhs_State _create();
    bool _delete();
    void cutProc();
    void cutTalkStart(int);
    void cutTalkProc(int);
    void cutRideSwitchStart(int);
    void cutRideSwitchProc(int);
    void cutRunWaitStart(int);
    void cutRunWaitProc(int);
    void searchNearLift(void*, void*);
    void cutJumpToLiftStart(int);
    void cutJumpToLiftProc(int);
    void searchNearRope(void*, void*);
    void cutLiftToRopeStart(int);
    void cutLiftToRopeProc(int);
    void cutRopeTalkStart(int);
    void cutRopeTalkProc(int);
    void cutRopeToLiftStart(int);
    void cutRopeToLiftProc(int);
    void cutJumpToGoalStart(int);
    void cutJumpToGoalProc(int);
    void cutJumpStart(int);
    void cutJumpProc(int);
    void cutSetAnmStart(int);
    void cutSetAnmProc(int);
    void cutSwOnStart(int);
    void cutSwOnProc(int);
    void cutSwOffStart(int);
    void cutSwOffProc(int);
    void cutSurpriseStart(int);
    void cutSurpriseProc(int);
    void cutOmamoriInitStart(int);
    void cutOmamoriInitProc(int);
    void cutOmamoriEndStart(int);
    void cutOmamoriEndProc(int);

    static const u32 m_heapsize[3];
    static const char m_arc_name[3];

public:
    /* 0x290 */ u8 mType;
    /* 0x291 */ u8 m291;
    /* 0x292 */ u8 mSwitchNo;
    /* 0x293 */ u8 m293;
    /* 0x294 */ request_of_phase_process_class mPhs;
    /* 0x29C */ mDoExt_McaMorf* mpMorf;
    /* 0x2A0 */ mDoExt_McaMorf* mpMorf2;
    /* 0x2A4 */ mDoExt_btpAnm mBtpAnm;
    /* 0x2B8 */ J3DModel* mpHeadModel;
    /* 0x2BC */ J3DModel* mpModel2BC;
    /* 0x2C0 */ J3DModel* mpModel2C0;
    /* 0x2C4 */ J3DModel* mpModel2C4;
    /* 0x2C8 */ J3DModel* mpModel2C8;
    /* 0x2CC */ dNpc_JntCtrl_c mJnt;
    /* 0x300 */ dNpc_EventCut_c mEventCut;
    /* 0x36C */ u8 m36C[0x378 - 0x36C];
    /* 0x378 */ u8 mBtpFrame;
    /* 0x379 */ u8 m379;
    /* 0x37A */ s16 mBtpTimer;
    /* 0x37C */ dBgS_ObjAcch mAcch;
    /* 0x540 */ dBgS_AcchCir mAcchCir;
    /* 0x580 */ dCcD_Stts mStts;
    /* 0x5BC */ dCcD_Cyl mCyl;
    /* 0x6EC */ u8 m6EC[0x6F8 - 0x6EC];
    /* 0x6F8 */ cXyz mEyePos;
    /* 0x704 */ cXyz mAttnPos;
    /* 0x710 */ u8 mAttnSetCount;
    /* 0x711 */ u8 m711[0x725 - 0x711];
    /* 0x725 */ u8 mHasAttention;
    /* 0x726 */ u8 m726[0x728 - 0x726];
    /* 0x728 */ u32 mMsgNo;
    /* 0x72C */ u8 m72C;
    /* 0x72D */ u8 m72D[0x748 - 0x72D];
    /* 0x748 */ s16 m748;
    /* 0x74A */ u8 m74A[0x74C - 0x74A];
    /* 0x74C */ int m74C;
    /* 0x750 */ u8 mEyeStopFlag;
    /* 0x751 */ u8 m751;
    /* 0x752 */ u8 m752[0x754 - 0x752];
    /* 0x754 */ dPa_smokeEcallBack mSmoke;
    /* 0x774 */ cXyz mSavedPos;
    /* 0x780 */ csXyz mSavedAngle;
    /* 0x786 */ u8 m786[0x7B0 - 0x786];
    /* 0x7B0 */ cXyz mSoundPos;
    /* 0x7BC */ u8 m7BC[0x7C1 - 0x7BC];
    /* 0x7C1 */ u8 m7C1;
    /* 0x7C2 */ u8 m7C2[0x7D0 - 0x7C2];
    /* 0x7D0 */ s8 m7D0;
    /* 0x7D1 */ s8 m7D1;
    /* 0x7D2 */ u8 m7D2;
    /* 0x7D3 */ u8 mAnmNo;
    /* 0x7D4 */ u8 m7D4;
    /* 0x7D5 */ s8 m7D5;
    /* 0x7D6 */ s8 m7D6;
    /* 0x7D7 */ u8 m7D7;
    /* 0x7D8 */ u8 m7D8[0x7F4 - 0x7D8];
    /* 0x7F4 */ int m7F4;
    /* 0x7F8 */ u8 m7F8[0x804 - 0x7F8];
    /* 0x804 */ int m804;
    /* 0x808 */ u8 m808;
    /* 0x809 */ s8 mTalkState;
    /* 0x80A */ s8 m80A;
    /* 0x80B */ u8 m80B;
};  // Size: 0x80C

STATIC_ASSERT(sizeof(daNpc_P2_c) == 0x80C);

class daNpc_P2_childHIO_c {
public:
    daNpc_P2_childHIO_c();
    virtual ~daNpc_P2_childHIO_c() {}

public:
    /* 0x04 */ dNpc_HIO_c mNpc;
    /* 0x2C */ u8 m2C;
    /* 0x30 */ f32 m30;
    /* 0x34 */ f32 m34;
    /* 0x38 */ f32 m38;
    /* 0x3C */ f32 m3C;
    /* 0x40 */ f32 m40;
    /* 0x44 */ f32 m44;
    /* 0x48 */ f32 m48;
    /* 0x4C */ f32 m4C;
    /* 0x50 */ f32 m50;
    /* 0x54 */ f32 m54;
    /* 0x58 */ f32 m58;
    /* 0x5C */ f32 m5C;
    /* 0x60 */ f32 m60;
    /* 0x64 */ f32 m64;
    /* 0x68 */ f32 m68;
    /* 0x6C */ u8 m6C;
    /* 0x70 */ f32 m70;
    /* 0x74 */ f32 m74;
    /* 0x78 */ f32 m78;
    /* 0x7C */ f32 m7C;
    /* 0x80 */ f32 m80;
    /* 0x84 */ f32 m84;
    /* 0x88 */ f32 m88;
    /* 0x8C */ f32 m8C;
    /* 0x90 */ f32 m90;
    /* 0x94 */ f32 m94;
    /* 0x98 */ f32 m98;
    /* 0x9C */ f32 m9C;
    /* 0xA0 */ f32 mA0;
    /* 0xA4 */ f32 mA4;
    /* 0xA8 */ f32 mA8;
    /* 0xAC */ f32 mAC;
    /* 0xB0 */ f32 mB0;
    /* 0xB4 */ f32 mB4;
    /* 0xB8 */ f32 mB8;
    /* 0xBC */ f32 mBC;
    /* 0xC0 */ f32 mC0;
    /* 0xC4 */ f32 mC4;
    /* 0xC8 */ f32 mC8;
    /* 0xCC */ f32 mCC;
    /* 0xD0 */ f32 mD0;
};  // Size: 0xD4

STATIC_ASSERT(sizeof(daNpc_P2_childHIO_c) == 0xD4);

class daNpc_P2_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_P2_HIO_c();
    virtual ~daNpc_P2_HIO_c() {}

public:
    /* 0x04 */ daNpc_P2_childHIO_c mChild[3];
    /* 0x280 */ f32 m280;
    /* 0x284 */ f32 m284;
};  // Size: 0x288

STATIC_ASSERT(sizeof(daNpc_P2_HIO_c) == 0x288);

#endif /* D_A_NPC_P2_H */
