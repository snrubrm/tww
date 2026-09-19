#ifndef D_A_NPC_KO1_H
#define D_A_NPC_KO1_H

#include "d/d_npc.h"
#include "m_Do/m_Do_hostIO.h"
#include "SSystem/SComponent/c_phase.h"

class J3DNode;

class daNpc_Ko1_c : public fopNpc_npc_c {
public:
    typedef int (daNpc_Ko1_c::*ActionFunc)(void*);

    struct anm_prm_c {
        /* 0x00 */ s8 mAnm;
        /* 0x01 */ s8 mTex;
        /* 0x04 */ f32 mMorf;
        /* 0x08 */ f32 mSpeed;
        /* 0x0C */ int mLoop;
        /* 0x10 */ int mFrame;
    };  // Size: 0x14

    void nodeHedControl(J3DNode*, J3DModel*);
    void nodeBlnControl(J3DNode*, J3DModel*);
    void nodeKo1Control(J3DNode*, J3DModel*);
    void init_HNA_0();
    void init_HNA_1();
    void init_HNA_2();
    void init_HNA_3();
    void init_HNA_4();
    void init_BOU_0();
    void init_BOU_1();
    void init_BOU_2();
    void init_BOU_3();
    void createInit();
    void setMtx(bool);
    int anmNum_toResID(int);
    int headAnmNum_toResID(int);
    int balloon_anmNum_toResID(int);
    int btpNum_toResID(int);
    bool setBtp(bool, int);
    bool iniTexPttrnAnm(bool);
    void plyTexPttrnAnm();
    void setAnm_tex(signed char);
    BOOL setAnm_anm(anm_prm_c*);
    BOOL set_balloonAnm_anm(anm_prm_c*);
    void set_balloonAnm_NUM(int);
    void setAnm_NUM(int, int);
    BOOL setAnm();
    void setPlaySpd(float);
    void chg_anmTag();
    void control_anmTag();
    void chg_anmAtr(unsigned char);
    void control_anmAtr();
    void setAnm_ATR(int);
    virtual void anmAtr(unsigned short);
    void eventOrder();
    void checkOrder();
    bool chk_talk();
    void chk_manzai_1();
    bool chk_partsNotMove();
    void lookBack();
    virtual u16 next_msgStatus(unsigned long*);
    u32 getMsg_HNA_0();
    u32 getMsg_HNA_1();
    u32 getMsg_HNA_2();
    u32 getMsg_HNA_3();
    u32 getMsg_BOU_0();
    u32 getMsg_BOU_1();
    s8 bitCount(unsigned char);
    u32 getMsg_BOU_2();
    virtual u32 getMsg();
    bool chkAttention();
    void setAttention(bool);
    fopAc_ac_c* searchByID(fpc_ProcID);
    void partner_srch_sub(void* (*)(void*, void*));
    void partner_srch();
    void check_landOn();
    void ko_setPthPos();
    void set_tgtPos(cXyz);
    void ko_movPass();
    void ko_clcMovSpd();
    void ko_clcSwmSpd();
    void ko_nMove();
    void chk_routeAngle(cXyz&, short*);
    void routeWallCheck(cXyz&, cXyz&, short*);
    void chk_ForwardGroundY(short);
    void chk_wallJump(short);
    void routeCheck(float, short*);
    void chk_start_swim();
    void get_crsActorID();
    void chk_areaIn(float, cXyz);
    void setPrtcl_Hamon(float, float);
    void setPrtcl_HanaPachi();
    void charDecide(int);
    void event_actionInit(int);
    BOOL event_action();
    void privateCut(int);
    void endEvent();
    int isEventEntry();
    void event_proc(int);
    bool set_action(int (daNpc_Ko1_c::*)(void*), void*);
    void clrSpd();
    void setStt(signed char);
    BOOL wait_1();
    BOOL wait_2();
    BOOL wait_3();
    BOOL wait_4();
    BOOL wait_5(signed char);
    BOOL wait_6();
    BOOL wait_7();
    BOOL wait_9();
    BOOL wait_a();
    BOOL walk_1();
    BOOL walk_2(signed char, signed char);
    BOOL walk_3();
    BOOL swim_1();
    BOOL swim_2();
    BOOL attk_1();
    BOOL attk_2(signed char, signed char);
    BOOL attk_3();
    BOOL down_1(signed char);
    BOOL talk_1();
    BOOL talk_2();
    BOOL manzai();
    BOOL neru_1();
    BOOL neru_2();
    int hana_action1(void*);
    int hana_action2(void*);
    int hana_action3(void*);
    int hana_action4(void*);
    int hana_action5(void*);
    int wait_action1(void*);
    int wait_action2(void*);
    int wait_action3(void*);
    int wait_action4(void*);
    void demo();
    void shadowDraw();
    BOOL _draw();
    BOOL _execute();
    BOOL _delete();
    cPhs_State _create();
    void create_Anm();
    void create_hed_Anm();
    void create_bln_Anm();
    void create_itm_Mdl();
    BOOL CreateHeap();

public:
    /* 0x6C4 */ request_of_phase_process_class mPhs;
    /* 0x6CC */ s8 m_hed_jnt_num;
    /* 0x6CD */ s8 m_bbone_jnt_num;
    /* 0x6CE */ s8 m_armR2_jnt_num;
    /* 0x6CF */ s8 m_hed_2_jnt_num;
    /* 0x6D0 */ s8 m_bln_loc_jnt_num;
    /* 0x6D1 */ s8 m_bln_jnt_num;
    /* 0x6D2 */ u8 m6D2[0x6D4 - 0x6D2];
    /* 0x6D4 */ Mtx mHeadMtx;
    /* 0x704 */ mDoExt_McaMorf* mpBalloonMorf;
    /* 0x708 */ u8 m708[0x710 - 0x708];
    /* 0x710 */ mDoExt_McaMorf* mpHeadMorf;
    /* 0x714 */ J3DAnmTexPattern* m_hed_tex_pttrn;
    /* 0x718 */ mDoExt_btpAnm mBtp;
    /* 0x72C */ u8 mBtpFrame;
    /* 0x72D */ u8 m72D;
    /* 0x72E */ s16 mBlinkTimer;
    /* 0x730 */ ActionFunc mAction;
    /* 0x73C */ u8 m73C[0x748 - 0x73C];
    /* 0x748 */ dNpc_EventCut_c mCut;
    /* 0x7B4 */ fpc_ProcID mPartnerId;
    /* 0x7B8 */ u8 m7B8[0x7DC - 0x7B8];
    /* 0x7DC */ cXyz mEyePos;
    /* 0x7E8 */ u8 m7E8[0x824 - 0x7E8];
    /* 0x824 */ f32 mLastFrame;
    /* 0x828 */ f32 mBalloonLastFrame;
    /* 0x82C */ f32 mTargetSpeed;
    /* 0x830 */ u8 m830[0x834 - 0x830];
    /* 0x834 */ f32 mSpeedStep;
    /* 0x838 */ u8 m838[0x83C - 0x838];
    /* 0x83C */ f32 mArriveDistance;
    /* 0x840 */ s16 mOldHeadY;
    /* 0x842 */ s16 mOldBackY;
    /* 0x844 */ s16 mOldActorY;
    /* 0x846 */ u8 m846[0x848 - 0x846];
    /* 0x848 */ u32 mPairedMsgNo;
    /* 0x84C */ u8 m84C[0x85E - 0x84C];
    /* 0x85E */ s8 mBalloonAnmEnded;
    /* 0x85F */ s8 mBalloonAnmRepeat;
    /* 0x860 */ s8 mAnmEnded;
    /* 0x861 */ s8 mAnmRepeat;
    /* 0x862 */ u8 m862;
    /* 0x863 */ u8 mPresentItem;
    /* 0x864 */ u8 m864[0x86B - 0x864];
    /* 0x86B */ u8 m86B;
    /* 0x86C */ u8 m86C;
    /* 0x86D */ u8 m86D;
    /* 0x86E */ u8 m86E[0x870 - 0x86E];
    /* 0x870 */ int mUpdateEye;
    /* 0x874 */ u8 m874;
    /* 0x875 */ u8 mTalking;
    /* 0x876 */ u8 mNoTurn;
    /* 0x877 */ u8 m877[0x89A - 0x877];
    /* 0x89A */ s8 mCutIndex;
    /* 0x89B */ u8 mEventAction;
    /* 0x89C */ u8 mAnmAttr;
    /* 0x89D */ u8 mAnmTag;
    /* 0x89E */ s8 mTexIndex;
    /* 0x89F */ s8 mAnmIndex;
    /* 0x8A0 */ u8 m8A0;
    /* 0x8A1 */ s8 mBalloonAnmIndex;
    /* 0x8A2 */ s8 mEventOrder;
    /* 0x8A3 */ s8 mState;
    /* 0x8A4 */ u8 m8A4;
    /* 0x8A5 */ s8 mLookMode;
    /* 0x8A6 */ s8 mType;
    /* 0x8A7 */ s8 mStaff;
    /* 0x8A8 */ s8 mActionState;
    /* 0x8A9 */ s8 mMessageState;
    /* 0x8AA */ u8 m8AA[0x8AC - 0x8AA];
};  // Size: 0x8AC

STATIC_ASSERT(sizeof(daNpc_Ko1_c) == 0x8AC);

class daNpc_Ko1_childHIO_c : public mDoHIO_entry_c {
public:
    struct hio_prm_c {
        /* 0x00 */ s16 mMaxHeadX;
        /* 0x02 */ s16 mMaxHeadY;
        /* 0x04 */ s16 mMinHeadX;
        /* 0x06 */ s16 mMinHeadY;
        /* 0x08 */ s16 mMaxBackBoneX;
        /* 0x0A */ s16 mMaxBackBoneY;
        /* 0x0C */ s16 mMinBackBoneX;
        /* 0x0E */ s16 mMinBackBoneY;
        /* 0x10 */ s16 mMaxTurnStep;
        /* 0x12 */ s16 mTurnSpeed;
        /* 0x14 */ f32 mAttentionOffsetY;
        /* 0x18 */ u8 mDebug;
        /* 0x1A */ s16 m1A;
        /* 0x1C */ s16 m1C;
        /* 0x1E */ s16 m1E;
        /* 0x20 */ f32 m20;
        /* 0x24 */ f32 m24;
        /* 0x28 */ f32 m28;
        /* 0x2C */ f32 m2C;
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
    };  // Size: 0x58

    daNpc_Ko1_childHIO_c();
    inline virtual ~daNpc_Ko1_childHIO_c();

public:
    /* 0x04 */ hio_prm_c mPrm;
    /* 0x5C */ int mIndex;
};  // Size: 0x60

STATIC_ASSERT(sizeof(daNpc_Ko1_childHIO_c) == 0x60);

class daNpc_Ko1_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Ko1_HIO_c();
    inline virtual ~daNpc_Ko1_HIO_c();

public:
    /* 0x04 */ s8 mNo;
    /* 0x08 */ int mSelected;
    /* 0x0C */ daNpc_Ko1_childHIO_c mChild[2];
};  // Size: 0xCC

STATIC_ASSERT(sizeof(daNpc_Ko1_HIO_c) == 0xCC);

#endif /* D_A_NPC_KO1_H */
