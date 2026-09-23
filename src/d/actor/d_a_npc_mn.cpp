/**
 * d_a_npc_mn.cpp
 * NPC - Manny
 */
#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_mn.h"
#include "m_Do/m_Do_ext.h"
#include "d/d_com_inf_game.h"
#include "d/d_com_lib_game.h"
#include "d/d_snap.h"
#include "d/d_a_obj.h"
#include "d/actor/d_a_obj_figure.h"
#include "f_op/f_op_actor_mng.h"
#include "res/Object/Mn.h"
const char daNpcMn_c::m_arcname[] = "Mn";
static const int l_bmd_ix_tbl[] = {dRes_ID_MN_BDL_MN_e};
static const int l_etc_bmd_ix_tbl[] = {dRes_ID_MN_BDL_MN_BAG_e};
static const int l_bck_ix_tbl[] = {dRes_ID_MN_BCK_MN_WAIT01_e, dRes_ID_MN_BCK_MN_WAIT02_e, dRes_ID_MN_BCK_MN_TALK01_e, dRes_ID_MN_BCK_MN_TALK02_e, dRes_ID_MN_BCK_MN_WALK_e, dRes_ID_MN_BCK_MN_BIKKURI_e, dRes_ID_MN_BCK_MN_JUMP01_e, dRes_ID_MN_BCK_MN_JUMP02_e};
static const int l_btp_ix_tbl[] = {dRes_ID_MN_BTP_MN_MABA_e};
static char* l_npc_staff_id = "Mn";
static char* l_arcname_tbl[] = {"Mn"};
static sMnAnmDat l_npc_anm_wait[] = {{0,8,-1}};
static sMnAnmDat l_npc_anm_wait2[] = {{1,8,-1}};
static sMnAnmDat l_npc_anm_talk[] = {{2,8,-1}};
static sMnAnmDat l_npc_anm_talk2[] = {{3,8,-1}};
static sMnAnmDat l_npc_anm_walk[] = {{4,8,-1}};
static sMnAnmDat l_npc_anm_bikkuri[] = {{5,8,1},{0,8,-1}};
static sMnAnmDat l_npc_anm_jump1[] = {{6,8,1},{255,8,0}};
static sMnAnmDat l_npc_anm_jump2[] = {{7,8,-1}};
struct MnNpcDat {
    s16 maxHeadX, maxHeadY, maxBackX, maxBackY, minHeadX, minHeadY, minBackX, minBackY, maxTurn;
    f32 playerEyeOffset, attentionYOffset, eyeYOffset, attentionDistance, nearDistance;
    s16 attentionAngle, turnSpeed, walkTurnSpeed;
    f32 collisionRadius, animScale, moveSpeed;
    s16 angleScale, angleMax, waitMin, waitMax, walkMin, walkMax, returnTime;
    u8 canTurn, canLook;
};
static MnNpcDat l_npc_dat[] = {
    {4000,10000,0,6000,-2000,-10000,-6000,-4000,1500,0,150,120,250,0,32767,1300,400,60,0.5f,3.0f,6,1024,60,90,200,300,30,1,1},
    {4000,10000,0,6000,-2000,-10000,-6000,-4000,1500,0,150,120,250,0,32767,1300,400,60,0.5f,1.5f,6,1024,60,90,200,300,30,1,1},
};
static u32 l_msg_mn_1st_talk[] = {0x35E9,0x35EA,0x35EB,0};
static u32 l_msg_mn_2nd_talk[] = {0x35EC,0x35ED,0x35EE,0};
static u32 l_msg_mn_1st_talk_in[] = {0x35F1,0};
static u32 l_msg_mn_2nd_talk_in[] = {0x35F2,0};
static u32 l_msg_mn_3rd_talk_in[] = {0x35F0,0};
static u32 l_msg_mn_comp_1st[] = {0x35F3,0x35F4,0x35F5,0};
static u32 l_msg_mn_comp_2nd[] = {0x35F6,0};
static u32 l_msg_mn_figure[] = {0x35F7,0x35F8,0x35F9,0x35FA,0x35FB,0x35FC,0x35FD,0x35FE,0x35FF,0x3600};
static dCcD_SrcCyl l_cyl_src = {
    {
        {
            0, {
                {
                    0,0,0
                }, {
                    0xFFFFFFFF, 3
                }, 0x15
            }
        }, {
            0,0,0,0,0
        }, {
            0,0,0,0,4
        }, {
            0
        }
    },
    {{{0,0,0},40,160}},
};
static char* l_room_name[] = {"sea","Pfigure","figureA","figureB","figureC","figureD","figureE","figureF","figureG","K_Testb"};
static u16 l_figure_comp[] = {0x95FF,0x94FF,0x93FF,0x92FF,0x91FF,0x90FF,0x8FFF,0x8EFF,0x8DFF,0x8CFF,0xB1FF,0x9CFF,0x84FF,0x83FF,0x82FF,0x81FF,0x80FF};
/* 00000078-00000230       .text __ct__9daNpcMn_cFv */
daNpcMn_c::daNpcMn_c() {
    mResFlag = 0;
    mMode = 0;
    mTargetSpeed = 0.0f;
    mWaitTimer = 0;
    mMorfOverride = -1.0f;
    mLookMode = 0;
    mNoTurn = true;
    mTargetAngle = home.angle.y;
    mAnmIndex = 0;
    mFlags = 0;
    mPosNo = chkPosNo();
    mNpcNo = getPrmNpcNo();
}

/* 000005E0-000006F8       .text daNpc_Mn_nodeCallBack__FP7J3DNodei */
static BOOL daNpc_Mn_nodeCallBack(J3DNode* node, int phase) {
    if (phase == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpcMn_c* npc = (daNpcMn_c*)model->getUserArea();
        int joint = ((J3DJoint*)node)->getJntNo();
#if VERSION == VERSION_DEMO
        PSMTXCopy(model->getAnmMtx(joint), *calc_mtx);
        if (joint == npc->m_jnt.getHeadJntNum()) {
            cMtx_XrotM(*calc_mtx, npc->m_jnt.getHead_y());
            cMtx_ZrotM(*calc_mtx, -npc->m_jnt.getHead_x());
        }
        if (joint == npc->m_jnt.getBackboneJntNum()) {
            cMtx_XrotM(*calc_mtx, npc->m_jnt.getBackbone_y());
            cMtx_ZrotM(*calc_mtx, -npc->m_jnt.getBackbone_x());
        }
#else
        MTXCopy(model->getAnmMtx(joint), *calc_mtx);
        if (joint == npc->m_jnt.getHeadJntNum()) {
            mDoMtx_XrotM(*calc_mtx, npc->m_jnt.getHead_y());
            mDoMtx_ZrotM(*calc_mtx, -npc->m_jnt.getHead_x());
        }
        if (joint == npc->m_jnt.getBackboneJntNum()) {
            mDoMtx_XrotM(*calc_mtx, npc->m_jnt.getBackbone_y());
            mDoMtx_ZrotM(*calc_mtx, -npc->m_jnt.getBackbone_x());
        }
#endif
        MtxP mtx = *calc_mtx;
        model->setAnmMtx(joint, mtx);
        MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
    }
    return TRUE;
}

/* 000006F8-00000718       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* actor) {
    return ((daNpcMn_c*)actor)->createHeap();
}

/* 00000718-0000085C       .text phase_1__FP9daNpcMn_c */
static cPhs_State phase_1(daNpcMn_c* npc) {
    fopAcM_SetupActor(npc, daNpcMn_c);
    switch (npc->getPosFlag()) {
    case 0:
        {
            dComIfGs_setEventReg(0x870F, 0);
            int sw = npc->getPrmSwitchBit();
            if (DEMO_SELECT(fopAcM_isSwitch(npc, sw), dComIfGs_isSwitch(sw, npc->home.roomNo))) {
                return cPhs_STOP_e;
            }
            break;
        }
    default:
        {
            u8 pos = dComIfGs_getEventReg(0x870F);
            if (npc->isChangePos(pos)) {
                u8 next;
                if (dComIfGs_isEventBit(0x3A01)) {
                    next = npc->getPosNo();
                } else {
                    next = 1;
                }
                pos = next;
                dComIfGs_setEventReg(0x870F, next);
            }
            if ((u8)pos != npc->getPosFlag()) {
                return cPhs_STOP_e;
            }
            break;
        }
    }
    npc->setResFlag(1);
    return cPhs_NEXT_e;
}

/* 0000085C-000008D4       .text phase_2__FP9daNpcMn_c */
static cPhs_State phase_2(daNpcMn_c* npc) {
    cPhs_State phase = dComIfG_resLoad(npc->getPhaseP(), l_arcname_tbl[0]);
    if (phase == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(npc, CheckCreateHeap, 0)) {
            phase = npc->createInit();
        } else {
#if VERSION > VERSION_DEMO
            npc->mpMorf = NULL;
#endif
            phase = cPhs_ERROR_e;
        }
    }
    return phase;
}

/* 000008D4-00000904       .text _create__9daNpcMn_cFv */
cPhs_State daNpcMn_c::_create() {
    static request_of_phase_process_fn l_method[] = {(request_of_phase_process_fn)phase_1, (request_of_phase_process_fn)phase_2, NULL};
    return dComLbG_PhaseHandler(&mCreatePhase, l_method, this);
}

/* 00000904-00000BE0       .text createHeap__9daNpcMn_cFv */
BOOL daNpcMn_c::createHeap() {
    J3DModelData* data = (J3DModelData*)dComIfG_getObjectIDRes(l_arcname_tbl[0], l_bmd_ix_tbl[0]);
    mpMorf = new mDoExt_McaMorf(data, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectIDRes(l_arcname_tbl[0], l_bck_ix_tbl[mAnmIndex]), 2, 1.0f, 0, -1, TRUE, NULL, 0x80000, 0x15021222);
    m_jnt.setHeadJntNum(data->getJointName()->getIndex("head"));
    JUT_ASSERT(DEMO_SELECT(995, 996), m_jnt.getHeadJntNum() >= 0);
    m_jnt.setBackboneJntNum(data->getJointName()->getIndex("backbone"));
    JUT_ASSERT(DEMO_SELECT(999, 1000), m_jnt.getBackboneJntNum() >= 0);
    if (!initTexPatternAnm(false)) {
        return FALSE;
    }
    for (u16 i = 0; i < data->getJointNum(); i++) {
        if (i == m_jnt.getHeadJntNum() || i == m_jnt.getBackboneJntNum()) {
            data->getJointNodePointer(i)->setCallBack(daNpc_Mn_nodeCallBack);
        }
    }
    mpMorf->getModel()->setUserArea((u32)this);
    mAcchCir.SetWall(30.0f, 30.0f);
#if VERSION == VERSION_DEMO
    mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
#else
    mObjAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, &current.angle, &shape_angle);
#endif
    mpBagModel = mDoExt_J3DModel__create((J3DModelData*)dComIfG_getObjectIDRes(l_arcname_tbl[0], l_etc_bmd_ix_tbl[0]), 0x80000, 0x11000002);
    if (mpBagModel == NULL) {
        return FALSE;
    }
    mBagJoint = data->getJointName()->getIndex("shoulderR");
    return TRUE;
}

/* 00000BE0-00000C00       .text daNpcMn_XyCheckCB__FPvi */
static s16 daNpcMn_XyCheckCB(void* npc, int value) {
    return ((daNpcMn_c*)npc)->XyCheckCB(value);
}

/* 00000C00-00000F3C       .text createInit__9daNpcMn_cFv */
cPhs_State daNpcMn_c::createInit() {
    int weight = 0xFF;
    u8 rail = getPrmRailID();
    if (rail != 0xFF) {
        mPath.setInf(rail, DEMO_SELECT(fopAcM_GetRoomNo(this), current.roomNo), 1);
        if (!mPath.isPath()) {
            return cPhs_ERROR_e;
        }
        dPath_GetNextRoomPath(mPath.getPath(), -1);
        fopAcM_OffStatus(this, 0x80);
        u8 point = 0;
        for (; point < mPath.maxPoint(); point++) {
            if (mPath.getPath()->m_points[point].mArg0 == 0) {
                break;
            }
        }
        if (point == mPath.maxPoint()) {
            point = 0;
        }
        mPath.setIdx(point);
        old.pos = mPath.getPoint(point);
        current.pos = old.pos;
        mPath.incIdxLoop();
        mWaitTimer = 1;
        weight = 0xFE;
    }
#if VERSION == VERSION_DEMO
    mStts.Init(weight, 0xFF, this);
    mCyl.Set(dNpc_cyl_src);
    mCyl.SetStts(&mStts);
    setCollision(&mCyl, current.pos, l_npc_dat[mNpcNo].collisionRadius, 150.0f);
#endif
    gravity = -9.0f;
    setAnmTbl(l_npc_anm_wait);
    mEventId = dComIfGp_evmng_getEventIdx("FIGURE_HATCH_OPEN");
    eventInfo.setXyCheckCB(daNpcMn_XyCheckCB);
    mEventCut.setActorInfo2(l_npc_staff_id, this);
    mTurnSpeed = 0;
    mTalking = 0;
    mHasAttention = 0;
    mNearPlayer = 0;
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -70.0f, 0.0f, -70.0f, 70.0f, 200.0f, 70.0f);
    attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xAA;
    attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xAA;
    attention_info.flags = 0xA;
    m_jnt.setParam(l_npc_dat[mNpcNo].maxBackX,l_npc_dat[mNpcNo].maxBackY,l_npc_dat[mNpcNo].minBackX,l_npc_dat[mNpcNo].minBackY,l_npc_dat[mNpcNo].maxHeadX,l_npc_dat[mNpcNo].maxHeadY,l_npc_dat[mNpcNo].minHeadX,l_npc_dat[mNpcNo].minHeadY,l_npc_dat[mNpcNo].maxTurn);
    mCanTurn = l_npc_dat[mNpcNo].canTurn;
    mCanLook = l_npc_dat[mNpcNo].canLook;
    mAttentionDistance = l_npc_dat[mNpcNo].attentionDistance;
    mAttentionAngle = l_npc_dat[mNpcNo].attentionAngle;
    mObjAcch.CrrPos(*dComIfG_Bgsp());
#if VERSION == VERSION_DEMO
    current.pos.y = home.pos.y = mObjAcch.GetGroundH();
    setMtx();
#else
    if (mObjAcch.GetGroundH() != -1000000000.0f) {
        current.pos.y = home.pos.y = mObjAcch.GetGroundH();
    }
    setMtx();
    mpMorf->getModel()->calc();
    mStts.Init(weight, 0xFF, this);
    mCyl.Set(dNpc_cyl_src);
    mCyl.SetStts(&mStts);
    setCollision(&mCyl, current.pos, l_npc_dat[mNpcNo].collisionRadius, 150.0f);
#endif
    return cPhs_COMPLEATE_e;
}

/* 00000F3C-00000FE4       .text _delete__9daNpcMn_cFv */
bool daNpcMn_c::_delete() {
#if VERSION == VERSION_DEMO
    if (mResFlag) {
        dComIfG_deleteObjectRes(l_arcname_tbl[0]);
    }
    if (mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
#else
    dComIfG_resDelete(&mResPhase, l_arcname_tbl[0]);
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
#endif
    if (dComIfGp_isEnableNextStage() && !strcmp(dComIfGp_getNextStageName(), "sea")) {
        dComIfGs_setEventReg(0x870F, 0);
    }
    return true;
}

/* 00000FE4-00001154       .text _draw__9daNpcMn_cFv */
// NONMATCHING - retail: the mpBagModel load is scheduled after the g_env_light address instead of before it
bool daNpcMn_c::_draw() {
    if (dComIfGs_isTmpBit(0x408)) {
        return true;
    }
    J3DModel* model = mpMorf->getModel();
    J3DModelData* data = model->getModelData();
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(model, &tevStr);
    mBtp.entry(data, mBtpFrame);
    mpMorf->updateDL();
    mBtp.remove(data);
    J3DModel* bagModel = mpBagModel;
    g_env_light.setLightTevColorType(bagModel, &tevStr);
    MtxP mtx = model->getAnmMtx(mBagJoint);
    MTXCopy(mtx, mpBagModel->getBaseTRMtx());
    mDoExt_modelUpdateDL(mpBagModel);
    cXyz pos(current.pos.x, 150.0f + current.pos.y, current.pos.z);
    mShadowId = dComIfGd_setShadow(mShadowId, 1, mpMorf->getModel(), &pos, 800.0f, 20.0f, current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    dSnap_RegistFig(DSNAP_TYPE_UNKA8, this, 1.0f, 1.0f, 1.0f);
    return true;
}
static u8 (daNpcMn_c::*l_execute_init[])() = {&daNpcMn_c::executeWaitInit,&daNpcMn_c::executeTalkInit,&daNpcMn_c::executeWalkInit,&daNpcMn_c::executeTurnInit,&daNpcMn_c::executeTalk3Init};
static void (daNpcMn_c::*moveProc[])() = {&daNpcMn_c::executeWait,&daNpcMn_c::executeTalk,&daNpcMn_c::executeWalk,&daNpcMn_c::executeTurn,&daNpcMn_c::executeTalk3};
/* 00001154-00001344       .text _execute__9daNpcMn_cFv */
bool daNpcMn_c::_execute() {
    chkAttention();
    checkOrder();
    if (!dComIfGp_event_runCheck() || eventInfo.checkCommandTalk() || mScopeTalkState) {
        (this->*moveProc[mMode])();
    } else {
        eventMove();
    }
    eventOrder();
    playTexPatternAnm();
    playAnm();
    if (mAnmIndex == 4) {
        cLib_chaseF(&speedF, mTargetSpeed, 0.3f);
        f32 playSpeed = speedF * l_npc_dat[mNpcNo].animScale;
        if (playSpeed < 0.5f) {
            playSpeed = 0.5f;
        }
        mpMorf->setPlaySpeed(playSpeed);
    } else cLib_chaseF(&speedF, mTargetSpeed, 0.1f);
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mObjAcch.CrrPos(*dComIfG_Bgsp());
    setCollision(&mCyl, current.pos, l_npc_dat[mNpcNo].collisionRadius, 150.0f);
    attention_info.position.set(current.pos.x, current.pos.y + l_npc_dat[mNpcNo].attentionYOffset, current.pos.z);
    eyePos.set(current.pos.x, current.pos.y + l_npc_dat[mNpcNo].eyeYOffset, current.pos.z);
    lookBack();
    setMtx();
    return false;
}

/* 00001344-000013B4       .text executeCommon__9daNpcMn_cFv */
u8 daNpcMn_c::executeCommon() {
    if (mHasAttention) {
        mEventOrder = 1;
    } else {
        mEventOrder = 0;
    }
    if (mTalking == 1 && mMode != 1) {
        executeSetMode(1);
    }
    return mTalking;
}

/* 000013B4-0000140C       .text executeSetMode__9daNpcMn_cFUc */
void daNpcMn_c::executeSetMode(u8 mode) {
    mTargetSpeed = 0.0f;
    speedF = 0.0f;
    mMode = (this->*l_execute_init[mode])();
}

/* 0000140C-00001518       .text executeWaitInit__9daNpcMn_cFv */
u8 daNpcMn_c::executeWaitInit() {
    speedF = 0.0f;
    if (mLookFigure) {
        setAnmTbl(l_npc_anm_wait2);
        mWaitTimer = 150.0f + cM_rndF(30.0f);
    } else {
        setAnmTbl(l_npc_anm_wait);
        mWaitTimer = l_npc_dat[mNpcNo].waitMin + cM_rndF(l_npc_dat[mNpcNo].waitMax - l_npc_dat[mNpcNo].waitMin);
    }
    return 0;
}

/* 00001518-000017CC       .text executeWait__9daNpcMn_cFv */
void daNpcMn_c::executeWait() {
    if (!executeCommon()) {
        if (mPosNo == 0) {
            if (dComIfGp_checkPlayerStatus0(0, 0x200000)) {
                mAttentionDistance = (s16)(2.0f * l_npc_dat[mNpcNo].attentionDistance);
            } else {
                mAttentionDistance = l_npc_dat[mNpcNo].attentionDistance;
                mFlags &= ~1;
            }
            if (mHasAttention && dComIfGs_isEventBit(0x2F08) && dComIfGp_checkPlayerStatus0(0, 0x200000)) {
                dComIfGp_setScopeType(1);
                if (dComIfGp_getMesgStatus() == 11 && !(mFlags & 1)) {
                    mFlags |= 1;
                    executeSetMode(4);
                }
            }
            if (!(mEventFlags & 1)) {
                int sw = getPrmSwitchBit();
                if (DEMO_SELECT(fopAcM_isSwitch(this, sw), dComIfGs_isSwitch(sw, home.roomNo))) {
                    sw = getPrmSwitchBit();
#if VERSION == VERSION_DEMO
                    fopAcM_onSwitch(this, sw);
#else
                    dComIfGs_onSwitch(sw, home.roomNo);
#endif
                    mEventFlags |= 1;
                    mCanTurn = 0;
                    mCanLook = 0;
                    mEventOrder = 3;
                }
            }
        } else if (mLookFigure) {
            if (mWaitTimer == 0) {
                mLookFigure = 0;
                executeSetMode(3);
            } else {
                mWaitTimer--;
                daObjFigure_c* figure = (daObjFigure_c*)fopAcM_searchFromName("Figure", 0xFF, mFigureID);
                if (figure != NULL && figure->isDispFigure()) {
                    mLookPos = figure->eyePos;
                    mLookMode = 1;
                    mNoTurn = false;
                    m_jnt.setTrn();
                }
            }
        } else if (mPath.isPath() && mWaitTimer != 0 && !mHasAttention && !mNearPlayer) {
            mWaitTimer--; if (mWaitTimer == 0) { mLookFigure = 0; executeSetMode(3); }
        }
    }
}

/* 000017CC-000017D4       .text executeTalkInit__9daNpcMn_cFv */
u8 daNpcMn_c::executeTalkInit() {
    return 1;
}

/* 000017D4-0000184C       .text executeTalk__9daNpcMn_cFv */
void daNpcMn_c::executeTalk() {
    executeCommon();
    if (talk2(1) == 0x12) {
        mTalking = 0;
        executeSetMode(0);
        dComIfGp_event_reset();
    } else {
        setAnmFromMsgTag();
    }
}

/* 0000184C-0000185C       .text executeTalk3Init__9daNpcMn_cFv */
u8 daNpcMn_c::executeTalk3Init() {
    mScopeTalkState = 0;
    return 4;
}

/* 0000185C-0000191C       .text executeTalk3__9daNpcMn_cFv */
void daNpcMn_c::executeTalk3() {
    switch (mScopeTalkState) {
    case 0:
    case 1:
        if (eventInfo.checkCommandDemoAccrpt()) {
            mScopeTalkState = 2;
        } else {
            fopAcM_orderPotentialEvent(this, 10, 0, 0);
            eventInfo.onCondition(2);
            mScopeTalkState = 1;
        } break;
    case 2:
        if (talk3(1) == 0x12) {
            mScopeTalkState = 0;
            executeSetMode(0);
            dComIfGp_event_reset();
        } break;
    }
}

/* 0000191C-00001948       .text executeWalkInit__9daNpcMn_cFv */
u8 daNpcMn_c::executeWalkInit() {
    setAnmTbl(l_npc_anm_walk);
    return 2;
}

/* 00001948-00001BD4       .text executeWalk__9daNpcMn_cFv */
void daNpcMn_c::executeWalk() {
    if (!executeCommon()) {
        bool end = false;
        if (mPath.chkPointPass(current.pos, (u8)(mPath.getDir() != 0))) {
            mFigureID = mPath.pointArg(mPath.getIdx());
            if (mFigureID != 0xFF) {
                daObjFigure_c* figure = (daObjFigure_c*)fopAcM_searchFromName("Figure", 0xFF, mFigureID);
                if (figure != NULL && figure->isDispFigure()) {
                    mFigureMessage = getRand(5);
                    mLookFigure = 1;
                    executeSetMode(0);
                }
            }
            if (!mPath.nextIdxAuto()) {
                end = true;
            }
        }
        if (mHasAttention || mNearPlayer) {
            mLookFigure = 0;
            executeSetMode(0);
        } else if (!end) {
            if (!mLookFigure) {
                cXyz pos = mPath.getPoint(mPath.getIdx());
                s16 angle;
                dNpc_calc_DisXZ_AngY(current.pos, pos, NULL, &angle);
                mLookAngle = mTargetAngle = angle;
                mNoTurn = false;
                mTargetTurnSpeed = l_npc_dat[mNpcNo].walkTurnSpeed;
                mLookMode = 2;
                m_jnt.setTrn();
                mTargetSpeed = l_npc_dat[mNpcNo].moveSpeed;
            }
        } else {
            mPath.turnDir();
            cXyz pos = mPath.getPoint(mPath.getIdx());
            s16 angle;
            dNpc_calc_DisXZ_AngY(current.pos, pos, NULL, &angle);
            mTargetAngle = angle;
            mPath.setInf(0xFF, DEMO_SELECT(fopAcM_GetRoomNo(this), current.roomNo), 1);
            executeSetMode(0);
        }
    }
}

/* 00001BD4-00001D18       .text executeTurnInit__9daNpcMn_cFv */
u8 daNpcMn_c::executeTurnInit() {
    cXyz pos = mPath.getPoint(mPath.getIdx());
    s16 angle;
    dNpc_calc_DisXZ_AngY(current.pos, pos, NULL, &angle);
    if (angle == current.angle.y) {
        setAnmTbl(l_npc_anm_walk);
        mWaitTimer = l_npc_dat[mNpcNo].walkMin + cM_rndF(l_npc_dat[mNpcNo].walkMax - l_npc_dat[mNpcNo].walkMin);
        return 2;
    }
    return 3;
}

/* 00001D18-00001DE8       .text executeTurn__9daNpcMn_cFv */
void daNpcMn_c::executeTurn() {
    if (!executeCommon()) {
        cXyz pos = mPath.getPoint(mPath.getIdx());
        s16 angle;
        dNpc_calc_DisXZ_AngY(current.pos, pos, NULL, &angle);
        mLookAngle = angle;
        mNoTurn = false;
        mLookMode = 2;
        m_jnt.setTrn();
        if (current.angle.y == angle) {
            executeSetMode(2);
        }
    }
}

/* 00001DE8-00001E80       .text checkOrder__9daNpcMn_cFv */
void daNpcMn_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        if (dComIfGp_evmng_startCheck(mEventId) && mEventOrder == 3) {
            mEventOrder = 0;
        }
    } else if (eventInfo.checkCommandTalk() && (mEventOrder == 2 || mEventOrder == 1)) { mTalking = 1; executeSetMode(1); }
}

/* 00001E80-00001F00       .text eventOrder__9daNpcMn_cFv */
void daNpcMn_c::eventOrder() {
    if (mEventOrder == 2 || mEventOrder == 1) {
        eventInfo.onCondition(1);
        if (mEventOrder == 2) {
            fopAcM_orderSpeakEvent(this);
        }
    } else if (mEventOrder == 3) {
        fopAcM_orderChangeEventId(dComIfGp_getPlayer(0), this, mEventId, 0, 0xFFFF);
    }
}

/* 00001F00-00001F74       .text eventMove__9daNpcMn_cFv */
void daNpcMn_c::eventMove() {
    if (!chkEndEvent()) {
        bool flag = mEventCut.getAttnFlag();
        if (mEventCut.cutProc()) {
            if (!mEventCut.getAttnFlag()) {
                mEventCut.setAttnFlag(flag);
            }
        } else {
            privateCut();
            setAnmFromMsgTag();
        }
    }
}

/* 00001F74-00002194       .text privateCut__9daNpcMn_cFv */
void daNpcMn_c::privateCut() {
    static char* cut_name_tbl[] = {"MES_SET","GET_ITEM","WAIT","HATCH","BIKKURI","TURN","WALK","LOOK","JUMP","SWON"};
    int staff = dComIfGp_evmng_getMyStaffId(l_npc_staff_id);
    if (staff != -1) {
        mCutIndex = dComIfGp_evmng_getMyActIdx(staff, cut_name_tbl, 10, TRUE, 0);
        if (mCutIndex == -1) {
            dComIfGp_evmng_cutEnd(staff);
        } else {
            if (dComIfGp_evmng_getIsAddvance(staff)) {
                switch (mCutIndex) {
                case 0:
                    eventMesSetInit(staff);
                    break;
                case 1:
                    eventGetItemInit();
                    break;
                case 2:
                    eventWaitInit(staff);
                    break;
                case 3:
                    eventHatchInit();
                    break;
                case 4:
                    eventBikkuriInit(staff);
                    break;
                case 5:
                    eventTurnInit();
                    break;
                case 6:
                    eventWalkInit();
                    break;
                case 7:
                    eventLookInit();
                    break;
                case 8:
                    eventJumpInit(staff);
                    break;
                case 9:
                    eventSwOnInit(staff);
                    break;
                }
            }
            bool done;
            switch (mCutIndex) {
            case 0:
                done = eventMesSet();
                break;
            case 2:
                done = eventWait(staff);
                break;
            case 3:
                done = eventHatch();
                break;
            case 4:
                done = eventBikkuri();
                break;
            case 5:
                done = eventTurn(staff);
                break;
            case 6:
                done = eventWalk();
                break;
            case 7:
                done = eventLook();
                break;
            case 8:
                done = eventJump();
                break;
            case 9:
                done = eventSwOn();
                break;
            default:
                done = true;
                break;
            }
            if (done) {
                dComIfGp_evmng_cutEnd(staff);
            }
        }
    }
}

/* 00002194-0000226C       .text eventMesSetInit__9daNpcMn_cFi */
void daNpcMn_c::eventMesSetInit(int staff) {
    int* msg = dComIfGp_evmng_getMyIntegerP(staff, "MsgNo");
    if (msg != NULL) {
        mpMessageTable = NULL;
        switch (*msg) {
        case 0:
            setMessage(getMsg());
            break;
        case 1:
            break;
        default:
            setMessage(*msg);
            break;
        }
        if (mpMessageTable != NULL) {
            setMessage(*mpMessageTable);
        }
    } else {
        mpMessageTable++;
        setMessage(*mpMessageTable);
    }
}

/* 0000226C-000022A0       .text eventMesSet__9daNpcMn_cFv */
bool daNpcMn_c::eventMesSet() {
    return talk2(0) == 0x12;
}

/* 000022A0-000022F8       .text eventGetItemInit__9daNpcMn_cFv */
void daNpcMn_c::eventGetItemInit() {
    fpc_ProcID id = fopAcM_createItemForPresentDemo(&current.pos, mItemNo, 0, -1, -1, NULL, NULL);
    if (id != fpcM_ERROR_PROCESS_ID_e) {
        dComIfGp_event_setItemPartnerId(id);
    }
}

/* 000022F8-00002358       .text eventWaitInit__9daNpcMn_cFi */
void daNpcMn_c::eventWaitInit(int staff) {
    int* timer = dComIfGp_evmng_getMyIntegerP(staff, "Timer");
    mEventTimer = 0;
    if (timer != NULL) {
        mEventTimer = *timer;
    }
}

/* 00002358-000023E8       .text eventWait__9daNpcMn_cFi */
bool daNpcMn_c::eventWait(int staff) {
    if (mEventTimer != 0) {
        mEventTimer--;
        return false;
    }
    int* sw = dComIfGp_evmng_getMyIntegerP(staff, "SwOn");
    if (sw != NULL) {
        int bit = getPrmSwitchBit2();
        dComIfGs_onSwitch(bit, DEMO_SELECT(fopAcM_GetHomeRoomNo(this), home.roomNo));
    }
    return true;
}

/* 000023E8-00002448       .text eventSwOnInit__9daNpcMn_cFi */
void daNpcMn_c::eventSwOnInit(int staff) {
    int* timer = dComIfGp_evmng_getMyIntegerP(staff, "Timer");
    mEventTimer = 0;
    if (timer != NULL) {
        mEventTimer = *timer;
    }
}

/* 00002448-000024AC       .text eventSwOn__9daNpcMn_cFv */
bool daNpcMn_c::eventSwOn() {
    if (mEventTimer != 0) {
        mEventTimer--;
        return false;
    }
    int bit = getPrmSwitchBit2();
    dComIfGs_onSwitch(bit, DEMO_SELECT(fopAcM_GetHomeRoomNo(this), home.roomNo));
    return true;
}

/* 000024AC-00002540       .text eventHatchInit__9daNpcMn_cFv */
void daNpcMn_c::eventHatchInit() {
    fopAc_ac_c* hatch = fopAcM_searchFromName("Ohatch", 0, 0);
    if (hatch != NULL) {
        s16 angle;
        dNpc_calc_DisXZ_AngY(current.pos, hatch->current.pos, NULL, &angle);
        mTargetAngle = angle;
    }
}

/* 00002540-00002578       .text eventHatch__9daNpcMn_cFv */
bool daNpcMn_c::eventHatch() {
    mLookAngle = mTargetAngle;
    mNoTurn = false;
    mLookMode = 2;
    m_jnt.setTrn();
    return (u8)(current.angle.y == mTargetAngle);
}

/* 00002578-000025EC       .text eventBikkuriInit__9daNpcMn_cFi */
void daNpcMn_c::eventBikkuriInit(int staff) {
    int* timer = dComIfGp_evmng_getMyIntegerP(staff, "Timer");
    mEventTimer = 1;
    if (timer != NULL) {
        mEventTimer = *timer;
    }
    if (mEventTimer <= 0) {
        mEventTimer = 1;
    }
}

/* 000025EC-00002644       .text eventBikkuri__9daNpcMn_cFv */
bool daNpcMn_c::eventBikkuri() {
    if (mEventTimer != 0) {
        mEventTimer--;
        if (mEventTimer == 0) {
            setAnmTbl(l_npc_anm_bikkuri);
        }
        return false;
    }
    return mAnmFlags & 1;
}

/* 00002644-0000266C       .text eventTurnInit__9daNpcMn_cFv */
void daNpcMn_c::eventTurnInit() {
    setAnmTbl(l_npc_anm_wait);
}

/* 0000266C-000027B4       .text eventTurn__9daNpcMn_cFi */
bool daNpcMn_c::eventTurn(int staff) {
    int* modeP = dComIfGp_evmng_getMyIntegerP(staff, "TurnMode");
    int mode = 0;
    if (modeP != NULL) {
        mode = *modeP;
    }
    cXyz pos;
    switch (mode) {
    case 1:
        {
            fopAc_ac_c* hatch = fopAcM_searchFromName("Ohatch", 0, 0);
            if (hatch != NULL) {
                pos = hatch->current.pos;
                break;
            }
        }
    default:
        pos = mPath.getPoint(mPath.getIdx());
        break;
    }
    s16 angle;
    dNpc_calc_DisXZ_AngY(current.pos, pos, NULL, &angle);
    mLookAngle = angle;
    mNoTurn = false;
    mLookMode = 2;
    m_jnt.setTrn();
    if (current.angle.y == angle) {
        return true;
    }
    return false;
}

/* 000027B4-000027DC       .text eventWalkInit__9daNpcMn_cFv */
void daNpcMn_c::eventWalkInit() {
    setAnmTbl(l_npc_anm_walk);
}

/* 000027DC-0000293C       .text eventWalk__9daNpcMn_cFv */
bool daNpcMn_c::eventWalk() {
    bool end = false;
    if (mPath.chkPointPass(current.pos, (u8)(mPath.getDir() != 0)) && !mPath.nextIdxAuto()) {
        end = true;
    }
    if (!end) {
        cXyz pos = mPath.getPoint(mPath.getIdx());
        s16 angle;
        dNpc_calc_DisXZ_AngY(current.pos, pos, NULL, &angle);
        mLookAngle = mTargetAngle = angle;
        mNoTurn = false;
        mTargetTurnSpeed = l_npc_dat[mNpcNo].walkTurnSpeed;
        mLookMode = 2;
        m_jnt.setTrn();
        mTargetSpeed = l_npc_dat[mNpcNo].moveSpeed;
    } else {
        speedF = 0.0f;
        mTargetSpeed = 0.0f;
        return true;
    }
    return false;
}

/* 0000293C-00002964       .text eventLookInit__9daNpcMn_cFv */
void daNpcMn_c::eventLookInit() {
    setAnmTbl(l_npc_anm_jump1);
}

/* 00002964-00002988       .text eventLook__9daNpcMn_cFv */
bool daNpcMn_c::eventLook() {
    return mpMorf->getFrame() >= 72.0f;
}

/* 00002988-00002A90       .text eventJumpInit__9daNpcMn_cFi */
void daNpcMn_c::eventJumpInit(int staff) {
    f32* speedX = dComIfGp_evmng_getMyFloatP(staff, "SpeedX");
    f32* speedY = dComIfGp_evmng_getMyFloatP(staff, "SpeedY");
    f32* gravityP = dComIfGp_evmng_getMyFloatP(staff, "Gravity");
    if (speedX != NULL) {
        mTargetSpeed = speedF = *speedX;
    } else {
        mTargetSpeed = speedF = 3.0f;
    }
    if (speedY != NULL) {
        mJumpSpeed = *speedY;
    } else {
        mJumpSpeed = 200.0f;
    }
    if (gravityP != NULL) {
        gravity = *gravityP;
    }
    mTargetSpeed = 40.0f;
    mJumpSpeed = 25.0f;
    gravity = -1.0f;
}

/* 00002A90-00002AE8       .text eventJump__9daNpcMn_cFv */
bool daNpcMn_c::eventJump() {
    current.pos.y += mJumpSpeed;
    mJumpSpeed += gravity;
    if (mJumpSpeed < 0.0f && mObjAcch.ChkGroundLanding()) {
        speedF = 0.0f;
        mTargetSpeed = 0.0f;
        return true;
    }
    return false;
}

/* 00002AE8-00002C30       .text talk2__9daNpcMn_cFi */
u16 daNpcMn_c::talk2(int init) {
    u16 status = 0xFF;
    if (mCurrMsgBsPcId == fpcM_ERROR_PROCESS_ID_e) {
        if (init == 1) {
            mCurrMsgNo = getMsg();
        }
        mCurrMsgBsPcId = fopMsgM_messageSet(mCurrMsgNo, this);
        mpCurrMsg = NULL;
        mPreviousMsgStatus = 0xFFFF;
    } else if (mpCurrMsg != NULL) {
        status = mpCurrMsg->mStatus;
        switch (status) {
        case 14:
            mpCurrMsg->mStatus = next_msgStatus(&mCurrMsgNo);
            if (mpCurrMsg->mStatus == 15) {
                fopMsgM_messageSet(mCurrMsgNo);
            }
            break;
        case 6:
            if (mPreviousMsgStatus == 15) {
                chkMsg();
            }
            break;
        case 18:
            mpCurrMsg->mStatus = 19;
            mCurrMsgBsPcId = fpcM_ERROR_PROCESS_ID_e;
            break;
        }
        mPreviousMsgStatus = status;
        anmAtr(status);
    } else mpCurrMsg = fopMsgM_SearchByID(mCurrMsgBsPcId);
    return status;
}

/* 00002C30-00002D68       .text talk3__9daNpcMn_cFi */
u16 daNpcMn_c::talk3(int init) {
    u16 status = 0xFF;
    if (mCurrMsgBsPcId == fpcM_ERROR_PROCESS_ID_e) {
        if (init == 1) {
            mCurrMsgNo = getMsg3();
        }
        mCurrMsgBsPcId = fopMsgM_scopeMessageSet(mCurrMsgNo);
        mpCurrMsg = NULL;
        mPreviousMsgStatus = 0xFFFF;
    } else if (mpCurrMsg != NULL) {
        status = dComIfGp_getScopeMesgStatus();
        switch (status) {
        case 14:
            dComIfGp_setScopeMesgStatus(next_msgStatus(&mCurrMsgNo));
            if (dComIfGp_getScopeMesgStatus() == 15) {
                fopMsgM_scopeMessageSet(mCurrMsgNo);
            }
            break;
        case 6:
            if (mPreviousMsgStatus == 15) {
                chkMsg();
            }
            break;
        case 18:
            dComIfGp_setScopeMesgStatus(17);
            mCurrMsgBsPcId = fpcM_ERROR_PROCESS_ID_e;
            break;
        }
        mPreviousMsgStatus = status;
        anmAtr(status);
    } else mpCurrMsg = fopMsgM_SearchByID(mCurrMsgBsPcId);
    return status;
}

/* 00002D68-00002DB8       .text next_msgStatus__9daNpcMn_cFPUl */
u16 daNpcMn_c::next_msgStatus(u32* msg) {
    u16 result = 15;
    if (mpMessageTable != NULL) {
        mpMessageTable++;
        switch (*mpMessageTable) {
        case 0:
            mpMessageTable = NULL;
            result = 16;
            break;
        default:
            *msg = *mpMessageTable;
            break;
        }
    } else result = 16;
    return result;
}

/* 00002DB8-00002F7C       .text getMsg__9daNpcMn_cFv */
u32 daNpcMn_c::getMsg() {
    u32 msg = 0;
    mpMessageTable = NULL;
    if (!dComIfGp_event_chkTalkXY()) {
        if (mPosNo == 0) {
            if (!dComIfGs_isEventBit(0x2F08)) {
                dComIfGs_onEventBit(0x2F08);
                mpMessageTable = l_msg_mn_1st_talk;
            } else {
                mpMessageTable = l_msg_mn_2nd_talk;
            }
        } else if (mPosNo == 1 && dComIfGs_isEventBit(0x3D08)) {
            dComIfGs_onEventBit(0x2F04);
            if (!dComIfGs_isEventBit(0x3120)) {
                dComIfGs_onEventBit(0x3120);
                mpMessageTable = l_msg_mn_comp_1st;
            } else {
                mpMessageTable = l_msg_mn_comp_2nd;
            }
        } else if (!dComIfGs_isEventBit(0x2F04)) {
            dComIfGs_onEventBit(0x2F04);
            mpMessageTable = l_msg_mn_1st_talk_in;
        } else if (mLookFigure) {
            msg = l_msg_mn_figure[mFigureMessage];
        } else if (!dComIfGs_isEventBit(0x3A01)) {
            mpMessageTable = l_msg_mn_2nd_talk_in;
        } else {
            mpMessageTable = l_msg_mn_3rd_talk_in;
        }
    }
    if (mpMessageTable != NULL) {
        msg = *mpMessageTable;
    }
    return msg;
}

/* 00002F7C-00002F8C       .text getMsg3__9daNpcMn_cFv */
u32 daNpcMn_c::getMsg3() {
    mpMessageTable = NULL;
    return 0x35EF;
}

/* 00002F8C-00002F90       .text chkMsg__9daNpcMn_cFv */
void daNpcMn_c::chkMsg() {
}

/* 00002F90-00002F98       .text setMessage__9daNpcMn_cFUl */
void daNpcMn_c::setMessage(u32 msg) {
    mCurrMsgNo = msg;
}

/* 00002F98-0000302C       .text setAnmFromMsgTag__9daNpcMn_cFv */
void daNpcMn_c::setAnmFromMsgTag() {
    switch (dComIfGp_getMesgAnimeAttrInfo()) {
    case 0:
        setAnmTbl(l_npc_anm_wait);
        break;
    case 1:
        setAnmTbl(l_npc_anm_talk);
        break;
    case 2:
        setAnmTbl(l_npc_anm_talk2);
        break;
    case 3:
        setAnmTbl(l_npc_anm_wait2);
        break;
    }
    dComIfGp_setMesgAnimeAttrInfo(0xFF);
}

/* 0000302C-0000303C       .text getPrmNpcNo__9daNpcMn_cFv */
bool daNpcMn_c::getPrmNpcNo() {
    return mPosNo != 0;
}

/* 0000303C-00003068       .text getPrmRailID__9daNpcMn_cFv */
u8 daNpcMn_c::getPrmRailID() {
    return daObj::PrmAbstract(this, PRM_BITS, PRM_RAIL_SHIFT);
}

/* 00003068-00003094       .text getPrmSwitchBit__9daNpcMn_cFv */
u8 daNpcMn_c::getPrmSwitchBit() {
    return daObj::PrmAbstract(this, PRM_BITS, PRM_SWITCH_SHIFT);
}

/* 00003094-000030C0       .text getPrmSwitchBit2__9daNpcMn_cFv */
u8 daNpcMn_c::getPrmSwitchBit2() {
    return daObj::PrmAbstract(this, PRM_BITS, PRM_SWITCH2_SHIFT);
}

/* 000030C0-00003148       .text setMtx__9daNpcMn_cFv */
void daNpcMn_c::setMtx() {
    mpMorf->getModel()->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00003148-00003478       .text chkAttention__9daNpcMn_cFv */
void daNpcMn_c::chkAttention() {
    mNearPlayer = 0;
    if (mEventCut.getAttnFlag()) {
        mLookPos = mEventCut.getAttnPos();
        mLookMode = 1;
        if (mCanTurn) {
            mNoTurn = false;
            m_jnt.setTrn();
        } else mNoTurn = true;
        if (!mHasAttention) {
            mHasAttention = 1;
        }
    } else {
        fopAc_ac_c* player = dComIfGp_getLinkPlayer();
        f32 distanceLimit = mAttentionDistance;
        int angleLimit = mAttentionAngle;
        f32 distance;
        s16 angle;
        dNpc_calc_DisXZ_AngY(current.pos, player->current.pos, &distance, &angle);
        if (mHasAttention) {
            distanceLimit += 40.0f;
            angleLimit += 0x71C;
        }
        angle -= shape_angle.y;
        if (distanceLimit > distance && angleLimit > abs(angle)) {
            mLookPos = dNpc_playerEyePos(l_npc_dat[mNpcNo].playerEyeOffset);
            mLookMode = 1;
            if (mCanTurn) {
                mNoTurn = false;
            } else {
                mNoTurn = true;
            }
            if (!mCanLook) {
                mLookAngle = mTargetAngle;
                mNoTurn = false;
                mLookMode = 2;
                m_jnt.setTrn();
            }
            if (!mHasAttention) {
                mHasAttention = 1;
            }
        } else {
            if (mHasAttention == 1) {
                mHasAttention = 0;
                mReturnTimer = l_npc_dat[mNpcNo].returnTime;
            }
            if (l_npc_dat[mNpcNo].nearDistance > distance) {
                mLookPos = dNpc_playerEyePos(l_npc_dat[mNpcNo].playerEyeOffset);
                mLookMode = 1;
                if (mCanTurn) {
                    mNoTurn = false;
                } else {
                    mNoTurn = true;
                }
                if (!mCanLook) {
                    mLookAngle = mTargetAngle;
                    mNoTurn = false;
                    mLookMode = 2;
                    m_jnt.setTrn();
                }
                mNearPlayer = 1;
            } else {
                mLookMode = 0;
                if (!mPath.isPath()) {
                    if (mReturnTimer != 0) {
                        mReturnTimer--;
                    } else {
                        mLookAngle = mTargetAngle;
                        mNoTurn = false;
                        mLookMode = 2;
                        m_jnt.setTrn();
                    }
                }
            }
        }
    }
    mTargetTurnSpeed = l_npc_dat[mNpcNo].turnSpeed;
}

/* 00003478-000035C4       .text lookBack__9daNpcMn_cFv */
void daNpcMn_c::lookBack() {
    s16 turnSpeed = mTargetTurnSpeed;
    s16 angle = current.angle.y;
    cXyz* targetP = NULL;
    cXyz target;
    cXyz source = eyePos;
    bool noTurn = mNoTurn;
    switch (mLookMode) {
    case 0:
        break;
    case 1:
        target = mLookPos;
        targetP = &target;
        break;
    case 2:
        angle = mLookAngle;
        break;
    }
    if (mTalking && mCanTurn) {
        noTurn = false;
        m_jnt.setTrn();
    }
    if (m_jnt.trnChk()) {
        if (mEventCut.getTurnSpeed() != 0) {
            turnSpeed = mEventCut.getTurnSpeed();
        }
        cLib_addCalcAngleS2(&mTurnSpeed, turnSpeed, 4, 0x800);
    } else mTurnSpeed = 0;
    m_jnt.lookAtTarget(&current.angle.y, targetP, source, angle, mTurnSpeed, noTurn);
    shape_angle = current.angle;
}

/* 000035C4-000036D0       .text initTexPatternAnm__9daNpcMn_cFb */
BOOL daNpcMn_c::initTexPatternAnm(bool modify) {
    J3DModelData* data = mpMorf->getModel()->getModelData();
    m_head_tex_pattern = (J3DAnmTexPattern*)dComIfG_getObjectIDRes(l_arcname_tbl[0], l_btp_ix_tbl[0]);
    JUT_ASSERT(DEMO_SELECT(2574, 2585), m_head_tex_pattern != 0);
    if (!mBtp.init(data, m_head_tex_pattern, TRUE, 2, 1.0f, 0, -1, modify, FALSE)) {
        return FALSE;
    }
    mBtpFrame = 0;
    mBlinkTimer = 0;
    return TRUE;
}

/* 000036D0-0000373C       .text playTexPatternAnm__9daNpcMn_cFv */
void daNpcMn_c::playTexPatternAnm() {
    if (cLib_calcTimer(&mBlinkTimer) == 0) {
        if (mBtpFrame >= m_head_tex_pattern->getFrameMax()) {
            mBtpFrame -= m_head_tex_pattern->getFrameMax();
            mBlinkTimer = 120;
        } else {
            mBtpFrame++;
        }
    }
}

/* 0000373C-000037F8       .text playAnm__9daNpcMn_cFv */
void daNpcMn_c::playAnm() {
    if (mpMorf->play(NULL, 0, 0) && mpAnmTable != NULL && mAnmRepeat > 0) {
        mAnmRepeat--;
        if (mAnmRepeat == 0) {
            mpAnmTable++;
            if (setAnmTbl(mpAnmTable)) {
                mAnmFlags |= 1;
            }
        } else {
            setAnm(mpAnmTable->mAnm, 0, 0.0f);
        }
    }
}

/* 000037F8-000038C8       .text setAnm__9daNpcMn_cFUcif */
void daNpcMn_c::setAnm(u8 index, int loop, f32 morf) {
    if (mMorfOverride >= 0.0f) {
        morf = mMorfOverride;
        mMorfOverride = -1.0f;
    }
    J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectIDRes(l_arcname_tbl[0], l_bck_ix_tbl[index]);
    mpMorf->setAnm(anm, loop, morf, 1.0f, 0.0f, -1.0f, NULL);
    mAnmIndex = index;
}

/* 000038C8-00003974       .text setAnmTbl__9daNpcMn_cFP9sMnAnmDat */
bool daNpcMn_c::setAnmTbl(sMnAnmDat* table) {
    mAnmFlags &= ~1;
    if (table->mAnm == 0xFF) {
        mpAnmTable = NULL;
        return true;
    }
    mpAnmTable = table;
    mAnmRepeat = mpAnmTable->mRepeat;
    int loop = 2;
    if (mAnmRepeat > 0) {
        loop = 0;
    }
    if (mAnmIndex != mpAnmTable->mAnm || loop == 0) {
        setAnm(mpAnmTable->mAnm, loop, mpAnmTable->mMorf);
    }
    return false;
}

/* 00003974-0000397C       .text XyCheckCB__9daNpcMn_cFi */
s16 daNpcMn_c::XyCheckCB(int) {
    return 0;
}

/* 0000397C-000039E0       .text getRand__9daNpcMn_cFi */
int daNpcMn_c::getRand(int max) {
    int result = cM_rndF(max);
    if (result == max) {
        result = 0;
    }
    return result;
}

/* 000039E0-00003A58       .text setCollision__9daNpcMn_cFP8dCcD_Cyl4cXyzff */
void daNpcMn_c::setCollision(dCcD_Cyl* cyl, cXyz pos, f32 radius, f32 height) {
    cyl->SetC(pos);
    cyl->SetR(radius);
    cyl->SetH(height);
    dComIfG_Ccsp()->Set(cyl);
}

/* 00003A58-00003AC4       .text chkEndEvent__9daNpcMn_cFv */
BOOL daNpcMn_c::chkEndEvent() {
    if (dComIfGp_evmng_endCheck(mEventId)) {
        dComIfGp_event_reset();
        fopAcM_delete(this);
        return TRUE;
    }
    return FALSE;
}

/* 00003AC4-00003B38       .text chkPosNo__9daNpcMn_cFv */
u8 daNpcMn_c::chkPosNo() {
    u8 i = 0;
    for (; i < 10; i++) {
        if (!strcmp(dComIfGp_getNextStageName(), l_room_name[i])) {
            break;
        }
    }
    return i;
}

/* 00003B38-00003CD8       .text getPosNo__9daNpcMn_cFv */
u8 daNpcMn_c::getPosNo() {
    if (dComIfGs_isEventBit(0x3D08) && !dComIfGs_isEventBit(0x3120)) {
        return 1;
    }
    int count[8];
    count[0] = 1;
    for (int i = 1; i < 8; i++) {
        count[i] = 0;
    }
    for (int i = 0; i < 134; i++) {
        if (i / 8 < 17) {
            int bit = i % 8;
            u8 flags = dComIfGs_getEventReg(l_figure_comp[i / 8]);
            if (flags & (1 << bit)) {
                int room = dSnap_GetFigRoomId(i);
                if (room != 0xFF && room < 8) {
                    count[room]++;
                }
            }
        }
    }
    int rooms = 0;
    for (int i = 0; i < 8; i++) {
        if (count[i] != 0) {
            rooms++;
        }
    }
    int choice = getRand(rooms);
    for (int i = 0; i < 8; i++) {
        if (count[i] != 0) {
            if (choice != 0) {
                choice--;
            } else {
                return (u8)(i + 1);
            }
        }
    }
    return 1;
}

/* 00003CD8-00003CE8       .text isChangePos__9daNpcMn_cFUc */
BOOL daNpcMn_c::isChangePos(u8 pos) {
    return pos == 0;
}

/* 00003CE8-00003D08       .text daNpc_MnCreate__FPv */
static cPhs_State daNpc_MnCreate(void* i_this) {
    return ((daNpcMn_c*)i_this)->_create();
}

/* 00003D08-00003D2C       .text daNpc_MnDelete__FPv */
static BOOL daNpc_MnDelete(void* i_this) {
    return ((daNpcMn_c*)i_this)->_delete();
}

/* 00003D2C-00003D50       .text daNpc_MnExecute__FPv */
static BOOL daNpc_MnExecute(void* i_this) {
    return ((daNpcMn_c*)i_this)->_execute();
}

/* 00003D50-00003D74       .text daNpc_MnDraw__FPv */
static BOOL daNpc_MnDraw(void* i_this) {
    return ((daNpcMn_c*)i_this)->_draw();
}

/* 00003D74-00003D7C       .text daNpc_MnIsDelete__FPv */
static BOOL daNpc_MnIsDelete(void*) {
    return TRUE;
}
static actor_method_class daNpc_MnMethodTable = {
    (process_method_func)daNpc_MnCreate,
    (process_method_func)daNpc_MnDelete,
    (process_method_func)daNpc_MnExecute,
    (process_method_func)daNpc_MnIsDelete,
    (process_method_func)daNpc_MnDraw,
};
actor_process_profile_definition g_profile_NPC_MN = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_MN_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpcMn_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_MN_e,
    /* Actor SubMtd */ &daNpc_MnMethodTable,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
