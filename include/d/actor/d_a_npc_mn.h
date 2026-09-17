#ifndef D_A_NPC_MN_H
#define D_A_NPC_MN_H

#include "d/d_npc.h"
#include "d/d_cc_d.h"

struct sMnAnmDat {
    u8 mAnm, mMorf;
    s8 mRepeat;
};

class daNpcMn_c : public fopNpc_npc_c {
public:
    u8 getNpcNo() { return mNpcNo; }
    request_of_phase_process_class* getPhaseP() { return &mResPhase; }
    u8 getPosFlag() { return mPosNo; }
    void setResFlag(u8 flag) { mResFlag = flag; }

    enum Prm_e { PRM_BITS = 8, PRM_SWITCH_SHIFT = 8, PRM_SWITCH2_SHIFT = 16, PRM_RAIL_SHIFT = 24 };
    static const char m_arcname[];
    daNpcMn_c();
    cPhs_State _create();
    BOOL createHeap();
    cPhs_State createInit();
    bool _delete();
    bool _draw();
    bool _execute();
    u8 executeCommon();
    void executeSetMode(unsigned char);
    u8 executeWaitInit();
    void executeWait();
    u8 executeTalkInit();
    void executeTalk();
    u8 executeTalk3Init();
    void executeTalk3();
    u8 executeWalkInit();
    void executeWalk();
    u8 executeTurnInit();
    void executeTurn();
    void checkOrder();
    void eventOrder();
    void eventMove();
    void privateCut();
    void eventMesSetInit(int);
    bool eventMesSet();
    void eventGetItemInit();
    void eventWaitInit(int);
    bool eventWait(int);
    void eventSwOnInit(int);
    bool eventSwOn();
    void eventHatchInit();
    bool eventHatch();
    void eventBikkuriInit(int);
    bool eventBikkuri();
    void eventTurnInit();
    bool eventTurn(int);
    void eventWalkInit();
    bool eventWalk();
    void eventLookInit();
    bool eventLook();
    void eventJumpInit(int);
    bool eventJump();
    u16 talk2(int);
    u16 talk3(int);
    virtual u16 next_msgStatus(unsigned long*);
    virtual u32 getMsg();
    u32 getMsg3();
    void chkMsg();
    void setMessage(unsigned long);
    void setAnmFromMsgTag();
    bool getPrmNpcNo();
    u8 getPrmRailID();
    u8 getPrmSwitchBit();
    u8 getPrmSwitchBit2();
    void setMtx();
    void chkAttention();
    void lookBack();
    BOOL initTexPatternAnm(bool);
    void playTexPatternAnm();
    void playAnm();
    void setAnm(unsigned char, int, float);
    bool setAnmTbl(sMnAnmDat*);
    s16 XyCheckCB(int);
    int getRand(int);
    void setCollision(dCcD_Cyl*, cXyz, float, float);
    BOOL chkEndEvent();
    u8 chkPosNo();
    u8 getPosNo();
    BOOL isChangePos(unsigned char);

public:
    u8 m6C4[0x60];
    request_of_phase_process_class mResPhase, mCreatePhase;
    J3DModel* mpBagModel;
    J3DAnmTexPattern* m_head_tex_pattern;
    mDoExt_btpAnm mBtp;
    u32 mShadowId;
    dNpc_PathRun_c mPath;
    u8 m75C[12];
    cXyz mLookPos;
    sMnAnmDat* mpAnmTable;
    u32* mpMessageTable;
    f32 mTargetSpeed, mMorfOverride, mAttentionDistance, mJumpSpeed;
    int mItemNo;
    u32 mFigureID;
    bool mNoTurn;
    s16 mEventId, mWaitTimer, mReturnTimer, mEventTimer, mAttentionAngle, mTargetAngle;
    u16 mPreviousMsgStatus, mFlags;
    u8 m7A6[2];
    s16 mBlinkTimer, mTargetTurnSpeed, mTurnSpeed, mLookAngle;
    u8 mTalking, mHasAttention, mEventOrder, mBtpFrame, mMode, mResFlag, mNpcNo, mEventFlags, mAnmIndex, mAnmFlags;
    s8 mAnmRepeat, mCutIndex;
    u8 m7BC;
    s8 mLookMode;
    u8 mCanTurn, mCanLook, mNearPlayer, mPosNo;
    s8 mBagJoint;
    u8 mScopeTalkState, mLookFigure, mFigureMessage;
};

STATIC_ASSERT(sizeof(daNpcMn_c) == 0x7C8);

#endif /* D_A_NPC_MN_H */
